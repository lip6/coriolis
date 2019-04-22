// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |         ISPD04 IBM Standard Cell Benchmarks                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Ispd05Bookshelf.cpp"                    |
// +-----------------------------------------------------------------+


#include  <memory>
#include  "vlsisapd/utilities/Dots.h"
#include  "vlsisapd/bookshelf/Exception.h"
#include  "vlsisapd/bookshelf/Node.h"
#include  "vlsisapd/bookshelf/Pin.h"
#include  "vlsisapd/bookshelf/Net.h"
#include  "vlsisapd/bookshelf/Row.h"
#include  "vlsisapd/bookshelf/Circuit.h"
#include  "vlsisapd/bookshelf/Parser.h"
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Net.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Library.h"
#include  "hurricane/UpdateSession.h"
#include  "crlcore/Utilities.h"
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/CellGauge.h"
#include  "crlcore/ToolBox.h"
#include  "crlcore/Ispd05Bookshelf.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;


  DbU::Unit  pitch = 5.0;


  void  addSupplyNets ( Cell* cell )
  {
    Net* vss = Net::create ( cell, "vss" );
    vss->setExternal ( true );
    vss->setGlobal   ( true );
    vss->setType     ( Net::Type::GROUND );

    Net* vdd = Net::create ( cell, "vdd" );
    vdd->setExternal ( true );
    vdd->setGlobal   ( true );
    vdd->setType     ( Net::Type::POWER );
  }


  Cell* toMasterCell ( AllianceFramework* af, Bookshelf::Node* node )
  {
    Technology* technology = DataBase::getDB()->getTechnology();
    Layer*      METAL1     = technology->getLayer ( "METAL1" );

    Cell* master = af->createCell( node->getName() );
    Box  abutmentBox( DbU::fromLambda( 0.0 )
                    , DbU::fromLambda( 0.0 )
                    , node->getWidth ()*pitch
                    , node->getHeight()*pitch
                    );
    master->setAbutmentBox( abutmentBox );
    Point cellCenter = abutmentBox.getCenter();

    addSupplyNets( master );

    for ( auto ipin : node->getPins() ) {
      Bookshelf::Pin* pin = ipin.second;
      Point pinCenter ( cellCenter.getX() + pin->getX()*pitch
                      , cellCenter.getY() + pin->getY()*pitch );
      if (not abutmentBox.contains(pinCenter))
        cerr << Error( "Ispd05::load(): Pin <%s> of node <%s> is outside abutment box.\n"
                       "        (AB:[%dx%d], pin:(%d,%d))"
                     , pin->getNet()->getName().c_str()
                     , getString(node->getName()).c_str()
                     , node->getWidth(), node->getHeight()
                     , pin->getX()     , pin->getY() ) << endl;;

      Net* net = master->getNet( pin->getNet()->getName() );
      if (not net)
        net = Net::create( master, pin->getNet()->getName() );
      net->setExternal( true );

      Vertical* segment = Vertical::create( net
                                          , METAL1
                                          , pinCenter.getX()
                                          , DbU::fromLambda(2.0)
                                          , pinCenter.getY() - DbU::fromLambda(0.5)
                                          , pinCenter.getY() + DbU::fromLambda(0.5)
                                          );
      NetExternalComponents::setExternal( segment );
    }

    return master;
  }


  Transformation  toTransformation ( Bookshelf::Node* node )
  {
    DbU::Unit x      = node->getX     ()*pitch;
    DbU::Unit y      = node->getY     ()*pitch;
    DbU::Unit width  = node->getWidth ()*pitch;
    DbU::Unit height = node->getHeight()*pitch;


	switch (node->getOrientation()) {
      case Bookshelf::Orientation::Disabled:
      case Bookshelf::Orientation::N:  return Transformation( x       , y       , Transformation::Orientation::ID );
      case Bookshelf::Orientation::W:  return Transformation( x       , y+width , Transformation::Orientation::R1 );
      case Bookshelf::Orientation::S:  return Transformation( x+width , y+height, Transformation::Orientation::R2 );
      case Bookshelf::Orientation::E:  return Transformation( x+height, y       , Transformation::Orientation::R3 );
      case Bookshelf::Orientation::FN: return Transformation( x+width , y       , Transformation::Orientation::MX );
      case Bookshelf::Orientation::FW: return Transformation( x+height, y+width , Transformation::Orientation::XR );
      case Bookshelf::Orientation::FS: return Transformation( x       , y+height, Transformation::Orientation::MY );
      case Bookshelf::Orientation::FE: return Transformation( x+height, y+width , Transformation::Orientation::YR );
	}
	return Transformation();
  }


} // End of anonymous namespace.


namespace CRL {

  using std::string;
  using std::map;
  using std::unique_ptr;
  using Hurricane::Library;
  using Hurricane::Transformation;
  using Hurricane::UpdateSession;
  using Utilities::Dots;


  Cell* Ispd05::load ( string benchmark )
  {
    AllianceFramework* af = AllianceFramework::get();
    pitch = af->getCellGauge()->getPitch();

    size_t count = 0;
    UpdateSession::open ();

    unique_ptr<Bookshelf::Circuit> circuit ( Bookshelf::Circuit::parse( benchmark
                                                                      , Bookshelf::Circuit::AllSlots
                                                                      , Bookshelf::Parser::NoFlags
                                                                      ) );

    cmess1 << "  o  Converting <" << benchmark << "> from Bookshelf to Hurricane." << endl;
    Cell* cell = af->createCell( benchmark );

    addSupplyNets( cell );

    Dots  dots ( cmess2, "       ", 80, 1000 );
    
    cmess1 << "     - Converting Nets." << endl;
    for ( auto net : circuit->getNets() ) {
      dots.dot();
      Net::create ( cell, net->getName() );

      if (++count % 1000) {
        UpdateSession::close ();
        UpdateSession::open ();
      }
    }
    dots.finish( Dots::Reset|Dots::FirstDot );

    cmess1 << "     - Converting Nodes." << endl;
    for ( auto inode : circuit->getNodes() ) {
      dots.dot();
      Bookshelf::Node* node = inode.second;

      Cell* master = toMasterCell( af, node );
      if (master == NULL) {
        cerr << Warning( "Skipping megacell <%s>.", node->getName().c_str() ) << endl;
        continue;
      }

      Instance* instance = Instance::create( cell, node->getName(), master );
      instance->setTransformation( toTransformation(node) );
      if (node->isFixed())
        instance->setPlacementStatus( Instance::PlacementStatus::FIXED );

      for ( auto ipin : node->getPins() ) {
        Name netName   = ipin.second->getNet()->getName();
        Net* masterNet = master->getNet( netName );
        instance->getPlug( masterNet )->setNet( cell->getNet(netName) );
      }

      if (++count % 1000) {
        UpdateSession::close ();
        UpdateSession::open ();
      }
    }
    dots.finish( Dots::Reset|Dots::FirstDot );

  // We assumes that the rows define a filled rectangular area.
    cmess1 << "     - Converting scl Rows." << endl;
    dots.setDivider( 10 );
    Box abutmentBox;
    for ( auto irow : circuit->getRows() ) {
      dots.dot();

      double x1 = (irow->getSubrowOrigin())*pitch;
      double y1 = (irow->getCoordinate()  )*pitch;
      double x2 = (irow->getSitewidth()*irow->getNumsites())*pitch + x1;
      double y2 = (irow->getHeight()      )*pitch + y1;

      Box rowBox = Box( x1, y1, x2, y2 );
      abutmentBox.merge( rowBox );
    }
    cell->setAbutmentBox( abutmentBox );
    dots.finish( Dots::Reset|Dots::FirstDot );

    UpdateSession::close();

    return cell;
  }


}  // End of CRL namespace.
