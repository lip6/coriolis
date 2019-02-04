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
// |  C++ Module  :  "./VSlicingNode.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "crlcore/RoutingGauge.h"
#include "bora/HVSetState.h"
#include "bora/VSlicingNode.h"
#include "bora/RVSlicingNode.h"

namespace Bora {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;


// -------------------------------------------------------------------
// Class  :  "Bora::VSlicingNode".


  int  VSlicingNode::_count    = 0;
  int  VSlicingNode::_countAll = 0;

  
  VSlicingNode::VSlicingNode ( unsigned int type, unsigned int alignment )
    : HVSlicingNode(type,alignment)
  { ++_count; }


  VSlicingNode::~VSlicingNode ()
  { --_count; }


  VSlicingNode* VSlicingNode::create ( unsigned int alignment )
  {
    ++_countAll;
    return new VSlicingNode( VerticalSNode, alignment ); 
  }


  void  VSlicingNode::createRouting ( DbU::Unit space )
  {
    push_back( RVSlicingNode::create( space ) ); 
    resetSlicingTree();
  }


  void  VSlicingNode::print () const
  {
    cerr << "- Print from Slicing Node - " << endl;
    cerr << "SlicingType: Vertical Node"  << endl; 
    if      (isAlignBottom()) cerr << "Alignment  : Bottom"  << endl;
    else if (isAlignCenter()) cerr << "Alignment  : Middle"  << endl;
    else if (isAlignTop   ()) cerr << "Alignment  : Top"     << endl;
    else                      cerr << "Alignment  : Unknown" << endl;
    cerr << "Tolerances : RatioH: " << DbU::getPhysical(_toleranceRatioH,DbU::Micro) << ", RatioW: " << DbU::getPhysical(_toleranceRatioW,DbU::Micro) << ", BandH: " << DbU::getPhysical(_toleranceBandH,DbU::Micro) << ", BandW: " << DbU::getPhysical(_toleranceBandW,DbU::Micro) << endl; 
    HVSlicingNode::print();
  }


  VSlicingNode* VSlicingNode::clone ( unsigned int tr )
  {
    VSlicingNode* node = VSlicingNode::create( this->getAlignment() );
    node->setTolerances( getToleranceRatioH()
                       , getToleranceRatioW()
                       , getToleranceBandH()
                       , getToleranceBandW()
                       );
    node->setBoxSet    ( getBoxSet()        );
    node->setNodeSets  ( _nodeSets->clone() );
    node->setPreset    ( getPreset()        );
    node->setSet       ( getSet()           );
    node->setPlaced    ( getPlaced()        );
    node->setSymmetries( getSymmetries()    );

    for ( SlicingNode* child : _children )  {
      if (tr == MX) node->push_front( child->clone(tr) );
      else          node->push_back ( child->clone(tr) );
    }
    return node;
  }

  
  void  VSlicingNode::place ( DbU::Unit x, DbU::Unit y )
  {
    if (recursiveCheckSet()) {
      if (not _slicingRouting.empty()) {
        destroySlicingRouting();
        resetSlicingRouting();
      }
      _place(x,y); 
      if (_slicingRouting.empty()) createSlicingRouting();
      updateCellAbutmentBox();
    } else
      cerr << Error( "VSlicingNode::place(DbU::Unit x, DbU::Unit y): The SlicingTree is not completely set." ) << endl;
  }

  
  void  VSlicingNode::replace ( DbU::Unit x, DbU::Unit y )
  {
  // WARNING: This will change GCell edges.
    if (recursiveCheckSet()) {
      _place( x, y, true ); 
      updateCellAbutmentBox();
      updateGCellPosition();
      updateGContacts();
    } else {
      cerr << Error( "HVlicingNode::place(DbU::Unit,DbU::Unit): The SlicingTree is not completely set." ) << endl;
    }
  }

  
  void  VSlicingNode::_place ( DbU::Unit x, DbU::Unit y, bool replace )
  {
    cdebug_log(536,1) << "VSlicingNode::_place(DbU::Unit,DbU::Unit,bool)" << endl;

    vector<RHVSlicingNode*>::iterator itspace = _slicingRouting.begin();
    DbU::Unit xref = x;
    DbU::Unit yref = y;
  
    if (isRoutingEstimated()){
      (*itspace)->_place( xref, yref, replace );
      xref += (*itspace)->getWidth();
      itspace++;
    }

    for ( SlicingNode* child : _children ) {
      if ( child->isHorizontal() or child->isVertical() ) { 
        if (child->isAlignBottom()) {
          child->setX( xref );
          child->setY( yref );
        } else if (child->isAlignCenter()) {
          child->setX( xref );
          child->setY( yref + (getHeight() - child->getHeight()) / 2 );
        } else if (child->isAlignTop()) {
          child->setX( xref );
          child->setY( yref + getHeight() - child->getHeight() );
        }
      }
      
      if      (child->isAlignBottom()) child->_place( xref, yref                                       , replace );
      else if (child->isAlignCenter()) child->_place( xref, yref + (getHeight() - child->getHeight())/2, replace );
      else if (child->isAlignTop   ()) child->_place( xref, yref +  getHeight() -  child->getHeight()  , replace );
      else if (child->isRouting    ()) child->_place( xref, yref                                       , replace );
      else { 
        cerr << Error( "VSlicingNode::place(DbU::Unit,DbU::Unit): Unknown Alignment in SlicingTree." ) << endl ; 
        child->print();
      }
    
      xref += child->getWidth();
      yref = y;
      if (isRoutingEstimated()) {
        (*itspace)->_place( xref, yref, replace );
        xref += (*itspace)->getWidth();
        itspace++;
      }
    }

    setPlaced( Placed );
    cdebug_tabw(536,-1);
  }


  void  VSlicingNode::updateGlobalSize ()
  {
    cdebug_log(535,1) << "VSlicingNode::updateGlobalsize() - " << this << endl;

    for ( SlicingNode* child : _children ) child->updateGlobalSize(); 

    if (not getMaster()) {
      if (getNbChild() == 1) {   
        _nodeSets->clear();
        NodeSets* node = _children[0]->getNodeSets();

        for ( BoxSet* bs : node->getBoxSets() ) {
          vector<BoxSet*> bss;

          bss.push_back( bs ); 
          _nodeSets->push_back( bss, bs->getHeight(), bs->getWidth(), VerticalSNode );
        }
      }
      else if ( not hasEmptyChildrenNodeSets() and _nodeSets->empty() ) {
        VSetState state = VSetState( this );
        while ( not state.end() ) state.next();

        _nodeSets = state.getNodeSets();
      }
      if (_nodeSets->empty())
        cerr << Error( "VSlicingNode::updateGlobalSize(): No solution has been found. Try to set larger tolerances.\n"
                       "        - Width tolerance ratio:  %s\n"
                       "        - Height tolerance ratio: %s\n"
                       "        - Width band:             %s\n"
                       "        - Height band:            %s"
                     , DbU::getValueString(getToleranceRatioW()).c_str()
                     , DbU::getValueString(getToleranceRatioH()).c_str()
                     , DbU::getValueString(getToleranceBandW ()).c_str()
                     , DbU::getValueString(getToleranceBandH ()).c_str()
                     ) << endl;
    } else {
      _nodeSets = _master->getNodeSets();
    }

    cdebug_log(535,0) << "Computed " << _nodeSets->size() << " choices" << endl;
    cdebug_tabw(535,-1);
  }


  void  VSlicingNode::preDestroy ()
  {
    HVSlicingNode::preDestroy();
  }


  void  VSlicingNode::destroy ()
  {
    VSlicingNode::preDestroy();
    delete this;
  }


  void  VSlicingNode::preRecursiveDestroy ()
  {
    HVSlicingNode::preRecursiveDestroy();
  }


  void  VSlicingNode::recursiveDestroy ()
  {
    VSlicingNode::preRecursiveDestroy();
    delete this;
  }


  void  VSlicingNode::createSlicingRouting ()
  {
    if (not isPlaced()) {
      cerr << Warning( "VSlicingNode::createSlicingRouting(): SlicingTree needs to be placed first." ) << endl; 
      return;
    }

    size_t    childrenCount = getNbChild();
    size_t    ichild        = 0;
    DbU::Unit x             = getX();
    DbU::Unit y             = getY();
    DbU::Unit widthValue    = 0;

    if (_parent) {
      if ( _parent->getType() == HorizontalSNode) {
        if ( (getAlignment() == AlignLeft) or (getAlignment() == AlignRight) ) {
          widthValue = _parent->getWidth() - getWidth(); 
        } else if (getAlignment() == AlignCenter) {
          widthValue = (_parent->getWidth() - getWidth())/2; 
        }
      }
    } 

    DbU::Unit vpitch = _rg->getVerticalPitch();
    if (widthValue % vpitch)
      cerr << Warning( "VSlicingNode::createSlicingRouting(): On %s, width is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(widthValue).c_str()
                     , DbU::getValueString(vpitch).c_str()
                     ) << endl;

    for ( size_t inode = 0; inode<childrenCount+1; ++inode ) {
      RVSlicingNode* node = NULL;
      if (inode == 0) {
        if (  (getAlignment() == AlignRight )
           or (getAlignment() == AlignCenter) ) node = RVSlicingNode::create( widthValue );
        else                                    node = RVSlicingNode::create();
      } else if (inode == childrenCount) {
        if (  (getAlignment() == AlignLeft  )
           or (getAlignment() == AlignCenter) ) node = RVSlicingNode::create( widthValue );
        else                                    node = RVSlicingNode::create();
      } else
        node = RVSlicingNode::create();

      node->setParent( this );

      if (inode == 0) {
        if (getAlignment() == AlignLeft) node->place( x, y );
        else if (  (getAlignment() == AlignCenter)
                or (getAlignment() == AlignRight) )
          node->place( x-widthValue, y );
      } else
        node->place( x, y );

      _slicingRouting.push_back( node );

      if (inode < childrenCount) x += getChild( ichild++ )->getWidth();
    }
    
    if (_master) {
      if (isVSymmetry()) {
        for ( size_t i=0; i<_slicingRouting.size(); ++i ) {
          getSlicingRouting( i )->setMaster( _master->getSlicingRouting( _slicingRouting.size()-1-i ) );
        }
      } else {
        for ( size_t i=0; i<_slicingRouting.size(); ++i ) {
          getSlicingRouting( i )->setMaster( _master->getSlicingRouting( i ) );
        }
      }
    } else if ( not _symmetries.empty() and isAlignCenter()) {
      for ( size_t i=0; i<(_slicingRouting.size()/2); ++i ) {
        getSlicingRouting( _slicingRouting.size()-1-i )->setMaster( getSlicingRouting( i ) );
      }
    }

    for ( SlicingNode* child : _children ) {
      if ( child->isHorizontal() or child->isVertical() ) child->createSlicingRouting();
    }  

    setRoutingCreated(RoutingCreated);
  }


  DbU::Unit  VSlicingNode::getWidth () const 
  {
    DbU::Unit vpitch = _rg->getVerticalPitch();
    DbU::Unit width  = 0;

    if (isRoutingEstimated()) {
      for ( SlicingNode*    node : _children       ) width += node->getWidth();
      for ( RHVSlicingNode* node : _slicingRouting ) width += node->getWidth();
    } else {
      if (_boxSet) width = _boxSet->getWidth();
    }

    if (width % vpitch)
      cerr << Warning( "VSlicingNode::getWidth(): On %s, width is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(width).c_str()
                     , DbU::getValueString(vpitch).c_str()
                     ) << endl;
    return width;
  }


  DbU::Unit  VSlicingNode::getHeight () const 
  {
    DbU::Unit hpitch = _rg->getHorizontalPitch();
    DbU::Unit height = 0;
  
    if (isRoutingEstimated()) {
      SlicingNode* m = NULL;
      for ( SlicingNode* node : _children ) {
        if ( (node->getType() != RoutingSNode) and (node->getHeight() > height) ) {
          height = node->getHeight();
          m = node;
        }
      }
      if (m->isDevice()) height += hpitch*2;
    } else {
      if (_boxSet) height = _boxSet->getHeight();
    }

    if (height % hpitch)
      cerr << Warning( "VSlicingNode::getHeight(): On %s, height is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(height).c_str()
                     , DbU::getValueString(hpitch).c_str()
                     ) << endl;
    return height;
  }


  void  VSlicingNode::setGCell ( Anabatic::GCell* gcell )
  {
    cdebug_log(535,1) << "VSlicingNode::setGCell(Anabatic::GCell*) " << gcell << endl; 

    Anabatic::GCell* childGCell  = gcell;
    Anabatic::GCell* remainGCell = NULL;
    DbU::Unit        x           = _slicingRouting[0]->getX();

    for ( size_t ichild=0 ; ichild<_children.size() ; ++ichild ) {
    // Setting up the GCell for the channel *before* the current child.
      cdebug_log(535,0) << "node[" << ichild << "] width:"
                        << DbU::getValueString(_slicingRouting[ ichild ]->getWidth()) << endl;

      x += _slicingRouting[ ichild ]->getWidth();
      remainGCell = childGCell->vcut( x );
      _slicingRouting[ ichild ]->setGCell( childGCell );
      childGCell = remainGCell;

    // Setting up the GCell for the current child.
      cdebug_log(535,0) << "children[" << ichild << "] width:"
                        << DbU::getValueString(_slicingRouting[ ichild ]->getWidth()) << endl;

      x += _children[ ichild ]->getWidth();
      remainGCell = childGCell->vcut( x );
      _children[ ichild ]->setGCell( childGCell );
      childGCell = remainGCell;
    }

  // Setting up the GCell for the channel *after* the last child.
    _slicingRouting.back()->setGCell( childGCell );
    
    cdebug_tabw(535,-1);
  }


  void  VSlicingNode::adjustBorderChannels ()
  {
    if (_parent) {
      DbU::Unit space = _parent->getWidth() - getWidth();

      if (getAlignment() == AlignRight) {
        RHVSlicingNode* node = _slicingRouting.front();
        node->setWidth( node->getWidth() + space );
      } else if (getAlignment() == AlignCenter) {
        RHVSlicingNode* first = _slicingRouting.front();
        RHVSlicingNode* last  = _slicingRouting.back ();
        first->setWidth( first->getWidth() + space/2 );
        last ->setWidth( last ->getWidth() + space/2 );
      } else if (getAlignment() == AlignLeft) {
        RHVSlicingNode* node = _slicingRouting.back();
        node->setWidth( node->getWidth() + space );
      }
    }

    for ( SlicingNode* child : _children ) child->adjustBorderChannels();
  }


  string  VSlicingNode::_getString () const
  {
    string s = Super::_getString();
    return s;
  }


  string  VSlicingNode::_getTypeName () const
  { return "VSlicingNode"; }


}  // Bora namespace.
