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
// |  C++ Module  :  "./bora/RHVSlicingNode.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Box.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Transformation.h"
#include "hurricane/UpdateSession.h"
#include "bora/ChannelRouting.h"
#include "bora/RHVSlicingNode.h"


namespace Bora {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Box;
  using Hurricane::Component;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::UpdateSession;
  using Hurricane::Transformation;


// -------------------------------------------------------------------
// Class  :  "Bora::RHVSlicingNode".


  int RHVSlicingNode::_count    = 0;
  int RHVSlicingNode::_countAll = 0;


  RHVSlicingNode::RHVSlicingNode()
    : SlicingNode( RoutingSNode, NodeSets::create(), UnknownAlignment, NULL )
    , _railInstance  (NULL)
    , _minHeight     (0)
    , _minWidth      (0)
    , _wireOccupation(NULL)
  {
    ++_count;
    
    setPreset  (Preset); 
    setSet     (Set   ); 
    setOverCell(OverCell);
    _wireOccupation = ChannelRouting::create();
  }


  RHVSlicingNode::~RHVSlicingNode ()
  { --_count; }


  void  RHVSlicingNode::print () const
  {
    cerr << "- Print from Slicing Node - " << endl;
    if (_railInstance) cerr << "RailInstance: " << _railInstance << endl; 
    SlicingNode::print();
  }


  void  RHVSlicingNode::_setGlobalSize ( DbU::Unit height, DbU::Unit width )
  { }


  void  RHVSlicingNode::preDestroy ()
  {
    if(_wireOccupation) _wireOccupation->destroy();
    SlicingNode::preDestroy();
  }


  void  RHVSlicingNode::destroy ()
  {
    RHVSlicingNode::preDestroy();
    delete this;
  }


  void  RHVSlicingNode::preRecursiveDestroy ()
  {
    SlicingNode::preRecursiveDestroy();
  }


  void RHVSlicingNode::recursiveDestroy ()
  {
    RHVSlicingNode::preRecursiveDestroy();
    delete this;
  }


  void  RHVSlicingNode::resetSize ()
  {
    _boxSet->setHeight( _minHeight );
    _boxSet->setWidth ( _minWidth  );
  }


  bool  RHVSlicingNode::isSame ( SlicingNode* node, unsigned int flags ) const
  {
    return (   (getHeight() == node->getHeight())
           and (getWidth () == node->getWidth ()) );
  }


  bool  RHVSlicingNode::isSymmetric ( SlicingNode* node, unsigned int symmetryType, unsigned int flags ) const
  {
    return (   (getHeight() == node->getHeight())
           and (getWidth () == node->getWidth ()) );
  }


  void  RHVSlicingNode::setRailInstance ( Hurricane::Instance* instance ) 
  { _railInstance = instance; }


  void  RHVSlicingNode::_place ( DbU::Unit x, DbU::Unit y, bool replace )
  {
    SlicingNode::_place( x, y, replace );
    if (_railInstance) {
      Net*   supplyNet = NULL;
      size_t count     = 0;
      for ( Net* net : _railInstance->getMasterCell()->getNets())
        { supplyNet = net; ++count; }

      if (count != 1)
        cerr << Error( "RHVSlicingNode::_place(DbU::Unit,DbU::Unit,bool): Several Nets in Rail Instance Cell." ) << endl;

      Component* wire = NULL;
      count = 0;

      for ( Component* component : supplyNet->getComponents() )
        { wire = component; ++count; }

      if (count != 1)
        cerr << Error( "RHVSlicingNode::_place(DbU::Unitx,DbU::Unit,bool): Several Components in Rail Instance Cell." ) << endl;

      Horizontal* h = dynamic_cast<Horizontal*>(wire);
      Vertical*   v = dynamic_cast<Vertical*>  (wire);
      UpdateSession::open();

      if (h) {
        h->setDxSource( 0 );
        h->setDxTarget( getRootWidth() );

        _railInstance->getMasterCell()->setAbutmentBox( Box( 0, 0, getRootWidth(), getHeight() ) );
        _railInstance->setTransformation( Transformation( _cell->getAbutmentBox().getXMin(), getY() ) );
      } else if (v) {
        v->setDySource( 0 );
        v->setDyTarget( getRootHeight() );
        _railInstance->getMasterCell()->setAbutmentBox( Box( 0, 0, getWidth(), getRootHeight() ) );
        _railInstance->setTransformation(Transformation( getX(), _cell->getAbutmentBox().getYMin() ) );
      } else
        cerr << Error( "RHVSlicingNode::_place(DbU::Unit,DbU::Unit,bool): Unknown segment type." ) << endl;

      _railInstance->setPlacementStatus( Instance::PlacementStatus::PLACED );
      UpdateSession::close();
    }
  }


  void  RHVSlicingNode::addWireOccupation ( DbU::Unit min, DbU::Unit max )
  { if ( _wireOccupation and (min != max) ) _wireOccupation->insertChannel(min, max); }
  
  
  void  RHVSlicingNode::resetWireOccupation ()
  { if (_wireOccupation) _wireOccupation->reset(); }


  int  RHVSlicingNode::getMaxWireOccupation ()
  { return (_wireOccupation) ? _wireOccupation->getMaxCount()+1 : 0; }


  unsigned int  RHVSlicingNode::getAlignment () const
  {
    cerr << Error( "RHVSlicingNode::getAlignment(): Unimplemented on this type." ) << endl;
    return UnknownAlignment;
  }


  NodeSets* RHVSlicingNode::getNodeSets () const
  {
    cerr << Error( "RHVSlicingNode::getNodeSets(): Unimplemented on this type." ) << endl;
    return NULL;
  }


  BoxSet* RHVSlicingNode::getPairH ( DbU::Unit ) const
  {
    cerr << Error( "RHVSlicingNode::getPaitH(): Unimplemented on this type." ) << endl;
    return _boxSet;
  }


  BoxSet* RHVSlicingNode::getPairHW( DbU::Unit , DbU::Unit ) const
  {
    cerr << Error( "RHVSlicingNode::getPaitHW(): Unimplemented on this type." ) << endl;
    return _boxSet;
  }


  void RHVSlicingNode::setPairH( DbU::Unit )
  {
    cerr << Error( "RHVSlicingNode::setPaitH(): Unimplemented on this type." ) << endl;
  }


  void RHVSlicingNode::setPairHW( DbU::Unit , DbU::Unit )
  {
    cerr << Error( "RHVSlicingNode::setPaitHW(): Unimplemented on this type." ) << endl;
  }


}  // Bora namespace.
