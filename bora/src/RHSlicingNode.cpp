// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./RHSlicingNode.h"                             |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/NetExternalComponents.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/AllianceFramework.h"
#include "bora/RHSlicingNode.h"


namespace Bora {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DataBase;
  using Hurricane::Box;
  using Hurricane::NetExternalComponents;


// -------------------------------------------------------------------
// Class  :  "Bora::RHSlicingNode".


  RHSlicingNode::RHSlicingNode ( DbU::Unit height )
    : RHVSlicingNode()
  {
    RHBoxSet* node = RHBoxSet::create( height );
    _nodeSets->push_back( node );
    _boxSet    = node;
    _minHeight = height;
    if (_parent) _minWidth = _parent->getWidth();
    else         _minWidth = 0;

    DbU::Unit hpitch = _rg->getHorizontalPitch();
    if (height % hpitch)
      cerr << Warning( "RHSlicingNode::RHSlicingNode(): On %s, height is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(height).c_str()
                     , DbU::getValueString(hpitch).c_str()
                     ) << endl;
  }


  RHSlicingNode::RHSlicingNode ( Hurricane::Instance* instance )
    : RHVSlicingNode()
  {
    RHBoxSet* node = RHBoxSet::create( instance->getMasterCell()->getAbutmentBox().getHeight() );
    _nodeSets->push_back( node );
    _boxSet    = node;
    _minHeight = instance->getMasterCell()->getAbutmentBox().getHeight();
    setRailInstance( instance );
    if (_parent) _minWidth = _parent->getWidth();
    else         _minWidth = 0;
  }
  

  RHSlicingNode::~RHSlicingNode ()
  { }


  RHSlicingNode* RHSlicingNode::create ( DbU::Unit height )
  {
    return new RHSlicingNode( height ); 
  }


  RHSlicingNode* RHSlicingNode::create ( Hurricane::Net*   net
                                       , Hurricane::Layer* layer
                                       , int               npitch
                                       , string            cname
                                       , string            iname )
  {
    Cell*     cell   = Hurricane::Cell::create( Hurricane::DataBase::getDB()->getRootLibrary(), cname );
    Net*      subnet = Hurricane::Net::create( cell, net->getName() );
    DbU::Unit height = CRL::AllianceFramework::get()->getRoutingGauge()->getHorizontalPitch() * npitch;
    subnet->setExternal( true );

    Hurricane::Horizontal* h = Hurricane::Horizontal::create( subnet, layer, height/2, height );
    NetExternalComponents::setExternal( h );

    Hurricane::Instance* instance = Hurricane::Instance::create( _cell, iname, cell ); 
    instance->getPlug( subnet )->setNet( net );
    cell->setAbutmentBox( Box( 0, 0, 0, height ) );
    SlicingNode::addRailSegments( h );

    return new RHSlicingNode( instance ); 
  }


  RHSlicingNode* RHSlicingNode::clone ( unsigned int tr )
  {
    RHSlicingNode* node = RHSlicingNode::create( getHeight() );
    return node; 
  }

  
  DbU::Unit RHSlicingNode::getWidth () const
  { return (_parent) ? _parent->getWidth() : 0; }


  void  RHSlicingNode::setHeight ( DbU::Unit height )
  {
    _boxSet->setHeight( height );

    DbU::Unit hpitch = _rg->getHorizontalPitch();
    if (height % hpitch)
      cerr << Warning( "RHSlicingNode::getHeight(): On %s, height is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(height).c_str()
                     , DbU::getValueString(hpitch).c_str()
                     ) << endl;
  }


  void  RHSlicingNode::estimateChannelsSize ()
  {
    cdebug_log(535,0) << "RHSlicingNode::estimateChannelsSize() " << this << endl;

    if (not getRailInstance()) {
      if (_gcell and _rg) {
        DbU::Unit hpitch = _rg->getHorizontalPitch();

        if (getMaxWireOccupation() > 0) {
          int occupancyH = getMaxWireOccupation();  //+1 for having enough space
          if (occupancyH % 2) ++occupancyH;
          updateMasterSize( occupancyH*hpitch );
        } else
          updateMasterSize( 2*hpitch );

        setRoutingEstimated(RoutingEstimated);
      } else
        cerr << Error( "RHSlicingNode::estimateChannelsSize()): Technology missing." ) << endl;
    }
  }


  void  RHSlicingNode::_expandRoutingChannel ()
  {
    SlicingNode* master = this;
    while ( master->getMaster() ) master = master->getMaster();

    cdebug_log(535,0) << "RHSlicingNode::_expandRoutingChannel() "
                      << this << " height:" << DbU::getValueString(master->getHeight()) << endl;

    setHeight( master->getHeight() );
    setRoutingEstimated( RoutingEstimated );
  }


  void  RHSlicingNode::expandRoutingChannel ( DbU::Unit height, DbU::Unit width )
  {
    cdebug_log(535,0) << "RHSlicingNode::expandRoutingChannel(DbU::Unit,DbU::Unit) "
                      << this << " height:" << DbU::getValueString(height) << endl;

    setHeight( height );
    setRoutingEstimated( RoutingEstimated );
  }


  void  RHSlicingNode::updateMasterSize ( DbU::Unit size )
  {
    SlicingNode* master = this;
    while ( master->getMaster() ) master = master->getMaster();
    if (master->getHeight() < size) master->setHeight( size );
  }


  void  RHSlicingNode::setGCell ( Anabatic::GCell* gcell )
  {
    cdebug_log(535,1) << "RHSlicingNode::setGCell() " << gcell << endl;

    _gcell = gcell;
    if (_gcell) {
      if (_railInstance) _gcell->setType( Anabatic::Flags::HRailGCell );
      else               _gcell->setType( Anabatic::Flags::HChannelGCell );
    }

    cdebug_tabw(535,-1);
  }


  void  RHSlicingNode::setWidth ( DbU::Unit width )
  {
    cerr << Error( "RHSlicingNode::setWidth(): Unimplemented on this type." ) << endl;
  }


  string  RHSlicingNode::_getTypeName () const
  { return "RHSlicingNode"; }


}
