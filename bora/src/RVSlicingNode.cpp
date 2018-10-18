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
// |  C++ Module  :  "./RVSlicingNode.h"                             |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/NetExternalComponents.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/AllianceFramework.h"
#include "bora/RVSlicingNode.h"


namespace Bora {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DataBase;
  using Hurricane::Box;
  using Hurricane::NetExternalComponents;


// -------------------------------------------------------------------
// Class  :  "Bora::RVSlicingNode".


  RVSlicingNode::RVSlicingNode ( DbU::Unit width )
    : RHVSlicingNode()
  {
    RVBoxSet* node = RVBoxSet::create( width );
    _nodeSets->push_back( node );
    _boxSet   = node;
    _minWidth = width;
    if (_parent) _minHeight = _parent->getHeight();
    else         _minHeight = 0;

    DbU::Unit vpitch = _rg->getVerticalPitch();
    if (width % vpitch)
      cerr << Warning( "RVSlicingNode::RVSlicingNode(): On %s, width is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(width).c_str()
                     , DbU::getValueString(vpitch).c_str()
                     ) << endl;
  }


  RVSlicingNode::RVSlicingNode ( Hurricane::Instance* instance )
    : RHVSlicingNode()
  {
    RVBoxSet* node = RVBoxSet::create( instance->getMasterCell()->getAbutmentBox().getWidth() );
    _nodeSets->push_back( node );
    _boxSet    = node;
    _minHeight = instance->getMasterCell()->getAbutmentBox().getWidth();
    setRailInstance( instance );
    if (_parent) _minWidth = _parent->getWidth();
    else         _minWidth = 0;
  }


  RVSlicingNode::~RVSlicingNode ()
  { }


  RVSlicingNode* RVSlicingNode::create ( DbU::Unit width )
  {
    return new RVSlicingNode( width );
  }


  RVSlicingNode* RVSlicingNode::create ( Hurricane::Net*   net
                                       , Hurricane::Layer* layer
                                       , int               npitch
                                       , string            cname
                                       , string            iname )
  {
    Cell* cell   = Hurricane::Cell::create( Hurricane::DataBase::getDB()->getRootLibrary(), cname );
    Net*  subnet = Hurricane::Net::create( cell, net->getName() );
  
    DbU::Unit width = CRL::AllianceFramework::get()->getRoutingGauge()->getVerticalPitch()* npitch;
    subnet->setExternal( true );

    Hurricane::Vertical* v = Hurricane::Vertical::create( subnet, layer, width/2, width );
    NetExternalComponents::setExternal( v );

    Hurricane::Instance* instance = Hurricane::Instance::create( _cell, iname, cell ); 
    instance->getPlug( subnet )->setNet( net );
    cell->setAbutmentBox( Box( 0, 0, width, 0) );

    SlicingNode::addRailSegments( v );

    return new RVSlicingNode( instance ); 
  }


  RVSlicingNode* RVSlicingNode::clone ( unsigned int tr )
  {
    RVSlicingNode* node = RVSlicingNode::create( getWidth() );
    return node; 
  }

  
  DbU::Unit  RVSlicingNode::getHeight () const
  { return (_parent) ? _parent->getHeight() : 0; }


  void  RVSlicingNode::setWidth ( DbU::Unit width )
  { _boxSet->setWidth( width ); }


  void  RVSlicingNode::estimateChannelsSize ()
  {
    cdebug_log(535,0) << "RVSlicingNode::estimateChannelsSize() " << this << endl;

    if (not getRailInstance()) {
      if (_gcell and _rg) {
        DbU::Unit vpitch = _rg->getVerticalPitch();

        if (getMaxWireOccupation() > 0) {
          int occupancyV = getMaxWireOccupation();  // +1 for having enough space
          cdebug_log(535,0) << this << " has " << getMaxWireOccupation() << " (-1) tracks" << endl;
            
          if (occupancyV % 2) ++occupancyV;
          updateMasterSize( occupancyV * vpitch );
        } else
          updateMasterSize( 2 * vpitch );
        setRoutingEstimated( RoutingEstimated );
      } else
        cerr << Error( "RHSlicingNode::estimateChannelsSize(: Technology missing." ) << endl;
    }
  }


  void  RVSlicingNode::_expandRoutingChannel ()
  {
    SlicingNode* master = this;
    while ( master->getMaster() ) master = master->getMaster();
    setWidth( master->getWidth() );
    setRoutingEstimated( RoutingEstimated );
  }


  void  RVSlicingNode::expandRoutingChannel ( DbU::Unit height, DbU::Unit width )
  {
    setWidth( width );
    setRoutingEstimated( RoutingEstimated );
  }


  void  RVSlicingNode::setGCell ( Anabatic::GCell* gcell )
  {
    cdebug_log(535,1) << "RVSlicingNode::setGCell() " << gcell << endl;

    _gcell = gcell;
    if (_gcell) {
      if (_railInstance) _gcell->setType( Anabatic::Flags::VRailGCell );
      else               _gcell->setType( Anabatic::Flags::VChannelGCell );
    }
    
    cdebug_tabw(535,-1);
  }


  void  RVSlicingNode::updateMasterSize ( DbU::Unit size )
  {
    SlicingNode* master = this;
    while ( master->getMaster() ) master = master->getMaster();
    if (master->getWidth() < size) master->setWidth( size );
  }


  void RVSlicingNode::setHeight( DbU::Unit height )
  {
    cerr << Error( "RVSlicingNode::setWidth(): Unimplemented on this type." ) << endl;
  }


  string  RVSlicingNode::_getTypeName () const
  { return "RVSlicingNode"; }


}  // Bora namespace.
