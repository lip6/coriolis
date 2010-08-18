
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |         ISPD04 IBM Standard Cell Benchmarks                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Ispd04Bookshelf.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <memory>
#include  "vlsisapd/bookshelf/Exception.h"
#include  "vlsisapd/bookshelf/Node.h"
#include  "vlsisapd/bookshelf/Pin.h"
#include  "vlsisapd/bookshelf/Net.h"
#include  "vlsisapd/bookshelf/Circuit.h"
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
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/ToolBox.h"
#include  "crlcore/Ispd04Bookshelf.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;


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
    if ( node->getHeight() > 16.0 ) return NULL;

    Technology* technology = DataBase::getDB()->getTechnology();
    Layer*      METAL1     = technology->getLayer ( "METAL1" );

    Cell* master = af->createCell ( node->getName() );
    Box  abutmentBox ( DbU::lambda( 0.0 )
                     , DbU::lambda( 0.0 )
                     , DbU::lambda( node->getWidth()*5.0 )
                     , DbU::lambda( 50.0 )
                     );
    master->setAbutmentBox ( abutmentBox );

    addSupplyNets ( master );

    Segment* segment = Horizontal::create ( master->getNet("vss")
                                          , METAL1
                                          , abutmentBox.getYMin()+DbU::lambda(3.0)
                                          , DbU::lambda(6.0)
                                          , abutmentBox.getXMin()
                                          , abutmentBox.getXMax()
                                          );
    NetExternalComponents::setExternal ( segment );

    segment = Horizontal::create ( master->getNet("vdd")
                                 , METAL1
                                 , abutmentBox.getYMax()-DbU::lambda(3.0)
                                 , DbU::lambda( 6.0)
                                 , abutmentBox.getXMin()
                                 , abutmentBox.getXMax()
                                 );
    NetExternalComponents::setExternal ( segment );

    map<size_t,Bookshelf::Pin*>&          pins = node->getPins();
    map<size_t,Bookshelf::Pin*>::iterator ipin = pins.begin();

    if ( node->getWidth() < (double)pins.size() )
      throw Error("Ispd04::load(): Node <%s> has only %.1f pitchs, cannot hold %zd terminals."
                 ,node->getName().c_str(),node->getWidth(),pins.size());

    DbU::Unit pinXMin = abutmentBox.getXMin() + DbU::lambda(5.0);
    DbU::Unit pinXMax = abutmentBox.getXMax() - ((node->getWidth() > (double)pins.size()) ? DbU::lambda(5.0) : 0);
    DbU::Unit pinX;

    for ( size_t pinnb=0 ; ipin != pins.end() ; ++ipin, ++pinnb ) {
      Net* net = Net::create ( master, (*ipin).second->getNet()->getName() );
      net->setExternal ( true );

      if ( pinnb % 2 ) {
        pinX     = pinXMax;
        pinXMax -= DbU::lambda(5.0);
      } else {
        pinX     = pinXMin;
        pinXMin += DbU::lambda(5.0);
      }

      segment = Vertical::create ( net
                                 , METAL1
                                 , pinX
                                 , DbU::lambda(1.0)
                                 , abutmentBox.getYMin() + DbU::lambda(10.0)
                                 , abutmentBox.getYMax() - DbU::lambda(10.0)
                                 );
      NetExternalComponents::setExternal ( segment );
    }

    return master;
  }


} // End of anonymous namespace.


namespace CRL {

  using std::string;
  using std::map;
  using std::auto_ptr;
  using Hurricane::Library;
  using Hurricane::Transformation;
  using Hurricane::UpdateSession;


  Cell* Ispd04::load ( string benchmark )
  {
    AllianceFramework* af = AllianceFramework::get ();

    UpdateSession::open ();

    auto_ptr<Bookshelf::Circuit> circuit ( Bookshelf::Circuit::parse(benchmark) );

    Cell* cell = af->createCell ( benchmark );

    addSupplyNets ( cell );

    vector<Bookshelf::Net*>&          nets = circuit->getNets ();
    vector<Bookshelf::Net*>::iterator inet = nets.begin();
    for ( ; inet != nets.end() ; ++inet ) {
      Net::create ( cell, (*inet)->getName() );
    }

    map<string,Bookshelf::Node*>&          nodes = circuit->getNodes();
    map<string,Bookshelf::Node*>::iterator inode = nodes.begin();

    Box abutmentBox;

    for ( ; inode != nodes.end() ; ++inode ) {
      Bookshelf::Node* node = (*inode).second;

      if ( node->isTerminal () ) continue;
      Cell* master = toMasterCell ( af, node );

      if ( master == NULL ) {
        cerr << Warning("Skipping megacell <%s>.",node->getName().c_str()) << endl;
        continue;
      }

      Instance* instance = Instance::create ( cell, node->getName(), master );

      map<size_t,Bookshelf::Pin*>&          pins = node->getPins();
      map<size_t,Bookshelf::Pin*>::iterator ipin = pins.begin();

      for ( size_t pinnb=0 ; ipin != pins.end() ; ++ipin, ++pinnb ) {
        Name netName (  (*ipin).second->getNet()->getName());
        Net* masterNet = master->getNet ( netName );
        instance->getPlug ( masterNet )->setNet ( cell->getNet(netName) );

        DbU::Unit x = DbU::lambda( node->getX()       *  5.0);
        DbU::Unit y = DbU::lambda((node->getY()/16.0) * 50.0);

        if ( node->getOrientation() != Bookshelf::Orientation::N ) {
          cerr << Warning("Skipping cell <%s>, unsupported orientation.",node->getName().c_str()) << endl;
          continue;
        }

        Box masterABox = master->getAbutmentBox();
        Transformation::Orientation orientation
          = ( (int)(node->getY())%32 ) ? Transformation::Orientation::MY
                                       : Transformation::Orientation::ID;
      //Transformation::Orientation orientation = Transformation::Orientation::ID;
        Transformation instanceTransformation = getTransformation(masterABox, x, y, orientation);

        instance->setTransformation  ( instanceTransformation );
        instance->setPlacementStatus ( Instance::PlacementStatus::PLACED );

        abutmentBox.merge ( instance->getAbutmentBox() );
      }
    }
    cell->setAbutmentBox ( abutmentBox );

    UpdateSession::close ();

    return cell;
  }


}  // End of CRL namespace.
