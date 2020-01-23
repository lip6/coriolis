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
// |  C++ Module  :  "./HVSlicingNode.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "crlcore/RoutingGauge.h"
#include "bora/HVSetState.h"
#include "bora/HSlicingNode.h"
#include "bora/RHSlicingNode.h"


namespace Bora {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;


// -------------------------------------------------------------------
// Class  :  "Bora::HSlicingNode".

  
  int  HSlicingNode::_count    = 0;
  int  HSlicingNode::_countAll = 0;


  HSlicingNode::HSlicingNode ( unsigned int type, unsigned int alignment )
    : HVSlicingNode(type,alignment)
  { ++_count; }


  HSlicingNode::~HSlicingNode ()
  { --_count; }


  HSlicingNode* HSlicingNode::create ( unsigned int alignment )
  {
    --_countAll;
    return new HSlicingNode( HorizontalSNode, alignment );
  }


  void  HSlicingNode::createRouting( DbU::Unit space )
  {
    push_back( RHSlicingNode::create( space ) ); 
    resetSlicingTree();
  } 


  void  HSlicingNode::print () const
  {
    cerr << "- Print from Slicing Node - " << endl;
    cerr << "SlicingType: Horizontal Node" << endl; 
    if      (isAlignLeft  ()) cerr << "Alignment  : Left"    << endl;
    else if (isAlignCenter()) cerr << "Alignment  : Middle"  << endl;
    else if (isAlignRight ()) cerr << "Alignment  : Right"   << endl;
    else                      cerr << "Alignment  : Unknown" << endl;
    cerr << "Tolerances : RatioH: " << DbU::getPhysical(_toleranceRatioH,DbU::Micro)
         << ", RatioW: " << DbU::getPhysical(_toleranceRatioW,DbU::Micro)
         << ", BandH: "  << DbU::getPhysical(_toleranceBandH ,DbU::Micro)
         << ", BandW: "  << DbU::getPhysical(_toleranceBandW ,DbU::Micro) << endl; 

    HVSlicingNode::print();
  }


  HSlicingNode* HSlicingNode::clone ( unsigned int tr )
  {
    HSlicingNode* node = HSlicingNode::create( getAlignment() );
    node->setTolerances( getToleranceRatioH()
                       , getToleranceRatioW()
                       , getToleranceBandH ()
                       , getToleranceBandW ()
                       );
    node->setBoxSet    ( getBoxSet()        );
    node->setNodeSets  ( _nodeSets->clone() );
    node->setPreset    ( getPreset()        );
    node->setSet       ( getSet()           );
    node->setPlaced    ( getPlaced()        );
    node->setSymmetries( getSymmetries()    );

    for ( SlicingNode* child : _children ) {
      if (tr == MY) node->push_front( child->clone(tr) );
      else          node->push_back ( child->clone(tr) );
    }
    return node;
  }


  void HSlicingNode::place ( DbU::Unit x, DbU::Unit y )
  {
    if (recursiveCheckSet()) { 
      if (not _slicingRouting.empty()) {
        destroySlicingRouting();
        resetSlicingRouting();
      }
      _place(x,y); 

      if (_slicingRouting.empty())
        createSlicingRouting();

      updateCellAbutmentBox();
    } else
      cerr << Warning( "HSlicingNode::place(): The SlicingTree is not completely set." ) << endl;
  }


  void  HSlicingNode::replace ( DbU::Unit x, DbU::Unit y )
  {
  // WARNING: This will change GCell edges.

    if (recursiveCheckSet()) { 
      _place( x, y, true ); 
      updateCellAbutmentBox();
      updateGCellPosition  ();
      updateGContacts      ();
    } else {
      cerr << Warning( "HSlicingNode::replace(): The SlicingTree is not completely set." ) << endl;
    }
  }


  void HSlicingNode::_place ( DbU::Unit x, DbU::Unit y, bool replace )
  {
    cdebug_log(536,1) << "HSlicingNode::_place(DbU::Unit,DbU::Unit,bool)" << endl;

    vector<RHVSlicingNode*>::iterator itspace = _slicingRouting.begin();
    DbU::Unit xref = x;
    DbU::Unit yref = y;
  
    if (isRoutingEstimated()) {
      (*itspace)->_place( xref, yref, replace );
      yref += (*itspace)->getHeight();
      itspace++;
    }

    for ( SlicingNode* child : _children ) {
      if ( (child->isHorizontal()) or (child->isVertical()) ) { 
        if (child->isAlignLeft()) {
          child->setX( xref );
          child->setY( yref );
        } else if (child->isAlignCenter()) {
          child->setX( xref + (getWidth()/2) - (child->getWidth()/2) );
          child->setY( yref );
        } else if (child->isAlignRight()) {
          child->setX( xref + getWidth() - child->getWidth() );
          child->setY( yref );
        }
      }
      
      if      (child->isAlignLeft()  ) child->_place( xref                                         , yref, replace);
      else if (child->isAlignCenter()) child->_place( xref + (getWidth()/2) - (child->getWidth()/2), yref, replace);
      else if (child->isAlignRight() ) child->_place( xref +  getWidth()    -  child->getWidth()   , yref, replace);
      else if (child->isRouting()    ) child->_place( xref                                         , yref, replace);
      else { 
        cerr << Warning( "HSlicingNode::_place(): Unknown Alignment in SlicingTree." ) << endl; 
        child->print();
      }
      
      xref  = x;
      yref += child->getHeight();
      if (isRoutingEstimated()) {
        (*itspace)->_place( xref, yref, replace );
        yref += (*itspace)->getHeight();
        itspace++;
      }
    }

    setPlaced( Placed );

    cdebug_tabw(536,-1);
  }


  void  HSlicingNode::updateGlobalSize ()
  {
    cdebug_log(535,1) << "HSlicingNode::updateGlobalsize() - " << this << endl;

    for ( SlicingNode* child : _children ) {
      cdebug_log(535,0) << "child: " << child << endl;
      child->updateGlobalSize(); 
    }

    if (not getMaster()) {
      if (getNbChild() == 1) {
        _nodeSets->clear();
        NodeSets* node = _children[0]->getNodeSets();

        for ( BoxSet* bs : node->getBoxSets() ) {
          vector<BoxSet*> bss;

          bss.push_back( bs );
          _nodeSets->push_back( bss, bs->getHeight(), bs->getWidth(), HorizontalSNode );
        }
      } else if ( not hasEmptyChildrenNodeSets() and _nodeSets->empty() ) {
        HSetState state = HSetState( this );
        while ( not state.end() ) state.next();

        _nodeSets = state.getNodeSets();
      }
      if (_nodeSets->empty())
        cerr << Warning( "HSlicingNode::updateGlobalSize(): No solution has been found, try to set larger tolerances." ) << endl;
    } else {
      _nodeSets = _master->getNodeSets();
    }

    cdebug_log(535,0) << "Found " << _nodeSets->size() << " choices" << endl;
    cdebug_tabw(535,-1);
  }


  void  HSlicingNode::preDestroy ()
  {
    HVSlicingNode::preDestroy();
  }


  void  HSlicingNode::destroy ()
  {
    HSlicingNode::preDestroy();
    delete this;
  }


  void  HSlicingNode::preRecursiveDestroy ()
  {
    HVSlicingNode::preRecursiveDestroy();
  }


  void  HSlicingNode::recursiveDestroy ()
  {
    HSlicingNode::preRecursiveDestroy();
    delete this;
  }


  void  HSlicingNode::createSlicingRouting ()
  {
    if (not _boxSet) {
      cerr << Warning( "HSlicingNode::createSlicingRouting(): SlicingTree needs to be placed first." ) << endl; 
      return;
    }
    
    size_t    childrenCount = getNbChild();
    size_t    ichild        = 0;
    DbU::Unit x             = getX();
    DbU::Unit y             = getY();
    DbU::Unit heightValue   = 0;

    if (_parent) {
      if (_parent->getType() == VerticalSNode) {
        if ( (getAlignment() == AlignBottom) or (getAlignment() == AlignTop) ) {
          heightValue = _parent->getHeight() - getHeight(); 
        } else if (getAlignment() == AlignCenter) {
          heightValue = (_parent->getHeight() - getHeight()) / 2; 
        }
      }
    }
      
    DbU::Unit hpitch = _rg->getHorizontalPitch();
    if (heightValue % hpitch)
      cerr << Warning( "HSlicingNode::createSlicingRouting(): On %s, height is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(heightValue).c_str()
                     , DbU::getValueString(hpitch).c_str()
                     ) << endl;
      
    for ( size_t inode=0; inode<childrenCount+1; ++inode ) {
      RHSlicingNode* node = NULL;
      if (inode == 0) {
        if (  (getAlignment() == AlignTop   )
           or (getAlignment() == AlignCenter) ) node = RHSlicingNode::create( heightValue );
        else                                    node = RHSlicingNode::create();
      } else if (inode == childrenCount) {
        if (  (getAlignment() == AlignBottom)
           or (getAlignment() == AlignCenter) ) node = RHSlicingNode::create( heightValue );
        else                                    node = RHSlicingNode::create();
      } else
        node = RHSlicingNode::create();

      node->setParent( this );
        
      if (inode == 0) {
        if (getAlignment() == AlignBottom)        node->place( x, y );
        else if (  (getAlignment() == AlignCenter)
                or (getAlignment() == AlignTop) ) node->place( x, y-heightValue );
      } else
        node->place( x, y );

      if (_master) node->setMaster( _master->getSlicingRouting(inode) );
      _slicingRouting.push_back( node );

      if (inode < childrenCount) y += getChild( ichild++ )->getHeight();
    }
    
    if (_master) {
      if (isHSymmetry()) {
        for ( size_t i=0; i<_slicingRouting.size(); ++i )
          getSlicingRouting( i )->setMaster( _master->getSlicingRouting( _slicingRouting.size()-1-i ) );
      } else {
        for ( size_t i=0; i<_slicingRouting.size(); ++i )
          getSlicingRouting( i )->setMaster( _master->getSlicingRouting( i ) );
      }
    } else if ( not _symmetries.empty() and isAlignCenter() ) {
      for ( size_t i=0; i<(_slicingRouting.size()/2); ++i ) {
        getSlicingRouting( _slicingRouting.size()-1-i )->setMaster( getSlicingRouting(i) );
      }
    }
    
    for ( SlicingNode* child : _children ) {
      if ( child->isHorizontal() or child->isVertical() ) child->createSlicingRouting();
    }   
    
    setRoutingCreated( RoutingCreated ); 
  }


  DbU::Unit  HSlicingNode::getHeight () const 
  {
    DbU::Unit hpitch = _rg->getHorizontalPitch();
    DbU::Unit height = 0;

    if (isRoutingEstimated()){
      for ( SlicingNode*    child : _children       ) height += child->getHeight();
      for ( RHVSlicingNode* node  : _slicingRouting ) height += node ->getHeight();
    } else {
      if (_boxSet != NULL) height = _boxSet->getHeight();
    }

    if (height % hpitch)
      cerr << Warning( "HSlicingNode::getHeight(): On %s, height is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(height).c_str()
                     , DbU::getValueString(hpitch).c_str()
                     ) << endl;
    return height;
  }


  DbU::Unit  HSlicingNode::getWidth () const 
  {
    cdebug_log(536,0) << "HSlicingNode::getWidth()" << endl;

    DbU::Unit vpitch = _rg->getVerticalPitch();
    DbU::Unit width  = 0;

    if (isRoutingEstimated()) {
      SlicingNode* m = NULL;
      for ( SlicingNode* node : _children ) {
        if ( (node->getType() != RoutingSNode) and (node->getWidth() > width) ) {
          width = node->getWidth();
          m = node;
        } 
      }
      if (m->isDevice()) width += vpitch*2;
    } else {
      if (_boxSet) width = _boxSet->getWidth();
    }

    if (width % vpitch)
      cerr << Warning( "HSlicingNode::getWidth(): On %s, width is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(width).c_str()
                     , DbU::getValueString(vpitch).c_str()
                     ) << endl;
    return width;
  }


  void  HSlicingNode::setGCell ( Anabatic::GCell* gcell )
  {
    cdebug_log(535,1) << "HSlicingNode::setGCell(), start Y: "
                      << _slicingRouting[0]->getY() << ", GCell:" << gcell << endl; 

    Anabatic::GCell* childGCell  = gcell;
    Anabatic::GCell* remainGCell = gcell;
    DbU::Unit        y           = _slicingRouting[0]->getY();
    
    for ( size_t ichild=0 ; ichild<_children.size() ; ++ichild ) {
    // Setting up the GCell for the channel *before* the current child.
      cdebug_log(535,0) << "node[" << ichild << "] height:"
                        << DbU::getValueString(_slicingRouting[ ichild ]->getHeight()) << endl;

      y += _slicingRouting[ ichild ]->getHeight();
      remainGCell = childGCell->hcut( y );
      _slicingRouting[ ichild ]->setGCell( childGCell );
      childGCell = remainGCell;

    // Setting up the GCell for the current child.
      cdebug_log(535,0) << "children[" << ichild << "] height:"
                        << DbU::getValueString(_children[ ichild ]->getHeight()) << endl;

      y += _children[ ichild ]->getHeight();
      remainGCell = childGCell->hcut( y );
      _children[ ichild ]->setGCell( childGCell );
      childGCell = remainGCell;
    }

  // Setting up the GCell for the channel *after* the last child.
    _slicingRouting.back()->setGCell( childGCell );
    
    cdebug_tabw(535,-1);
  }


  void  HSlicingNode::adjustBorderChannels ()
  {
    if (_parent) {
      if (_parent->getHeight() > getHeight()) {
        DbU::Unit space = _parent->getHeight() - getHeight();

        if (getAlignment() == AlignTop) {
          RHVSlicingNode* channel = _slicingRouting.front();
          channel->setHeight( channel->getHeight() + space );
        } else if (getAlignment() == AlignCenter) {
          RHVSlicingNode* firstChannel = _slicingRouting.front();
          RHVSlicingNode*  lastChannel = _slicingRouting.back ();
          firstChannel->setHeight( firstChannel->getHeight() + space/2 );
          lastChannel ->setHeight( lastChannel ->getHeight() + space/2 );
        } else if (getAlignment() == AlignBottom) {
          RHVSlicingNode* channel = _slicingRouting.back();
          channel->setHeight( channel->getHeight() + space );
        }
      }
    }

    for ( SlicingNode* child : _children ) child->adjustBorderChannels();
  }


  string  HSlicingNode::_getString () const
  {
    string s = Super::_getString();
    return s;
  }


  string  HSlicingNode::_getTypeName () const
  { return "HSlicingNode"; }


}  // Bora namespace.
