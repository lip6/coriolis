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
// |  C++ Module  :  "./HVSetState.cpp"                              |
// +-----------------------------------------------------------------+


#include "bora/HVSetState.h"
#include "bora/HSlicingNode.h"
#include "bora/VSlicingNode.h"


namespace Bora {

  using namespace std;


// -------------------------------------------------------------------
// Class  :  "Bora::HVSetState".
  

  HVSetState::HVSetState ( HVSlicingNode* node )
    : _HVSnode   ( node )
    , _counter   ( 1 )
    , _currentSet()
    , _nextSet   ()
    , _nodeSets  ( NodeSets::create() )
  {
    initSet();
    initModulos();
  }


  HVSetState::~HVSetState ()
  { }


  NodeSets* HVSetState::getNodeSets ()
  {
    _nodeSets->sort();
    return _nodeSets; 
  }


  void  HVSetState::print ()
  {
    int index = 0;
    cerr << "currentSet:" << endl;

    const VSlicingNodes& children = _HVSnode->getChildren();
    for ( size_t ichild=0 ; ichild<children.size() ; ++ichild ) {
      NodeSets* nodes = children[ ichild ]->getNodeSets();

      cerr << index << ": H = " << nodes->at(_currentSet[ichild])->getHeight()
           <<          ", W = " << nodes->at(_currentSet[ichild])->getWidth () << endl;
    }
    cerr << "counter     = " << _counter << endl;
    cerr << "end counter = " << _modulos.back() << endl;

    cerr << "modulos:" << endl;
    for ( size_t i=0 ; i<_modulos.size() ; ++i )
      cerr << i << ": modulo = " << _modulos[i] << endl;

    _nodeSets->print();
  }


  void  HVSetState::initSet ()
  {
    _nextSet.clear();
    for ( SlicingNode* child : _HVSnode->getChildren() ) {
      if (child->isPreset()) 
        _nextSet.push_back( child->getNodeSets()->findIndex( child->getHeight()
                                                           , child->getWidth () ) );
      else 
        _nextSet.push_back( 0 );
    }
    _currentSet = _nextSet;
  }


  // Notes:
  //
  // Considering a horizontal/vertical  node with X children  and each children
  // has N possibilities:
  //
  // child 0: N0 possibilities,
  // child 1: N1 possibilities,
  //          .
  //          .
  //          .
  // child X: NX possibilities.
  //
  // If a child is preset then, it only has 1 possibility and N = 1. So we have:
  //
  //   number of possibilities to be studied = N0 * N1 * ... * NX.
  //
  // Children are  seen like  a <vector> and  modulos are used  to know  when a
  // child needs to choose its next possibility.
  //
  // It starts like this:
  // - Child 0 goes through its N0 possibilities
  // - When child 0 was at its last  possibility, it goes back to its first one
  //   and child 1 pick its second possibility.
  // - And we keep going . . . 

  void  HVSetState::initModulos ()
  {
    cdebug_log(535,0) << "HVSetState::initModulos()" << endl;

    int modulo = 1;
    
    _modulos.clear();
    _modulos.push_back( 1 );

    const VSlicingNodes& children = _HVSnode->getChildren();
    for ( size_t ichild=0 ; ichild<children.size() ; ++ichild ) {
      if (ichild) _modulos.push_back( modulo );

      if (isSymmetry(ichild)) {
        cdebug_log(535,0) << "child:" << ichild << " is symmetric, one choice." << endl;
        continue;
      }
      if (children[ichild]->isPreset() ) {
        cdebug_log(535,0) << "child:" << ichild << " is preset, one choice." << endl;
        continue;
      }

      cdebug_log(535,0) << "child:" << ichild << " is ordinary, "
                        << "has " << children[ichild]->getNodeSets()->size() << " choices." 
                        << endl;
      modulo *= children[ichild]->getNodeSets()->size();
    }

    _modulos.push_back( modulo );

    for ( size_t i=0 ; i<_modulos.size() ; ++i )
      cdebug_log(535,0) << "_modulos[" << i << "]:" << _modulos[i] << endl;
  }


  void  HVSetState::next ()
  {
  // Notes: Set the next combination. See notes above.
    cdebug_log(535,0) << "HVSetState::next(): counter_:" << _counter << endl;

    Symmetry                 symmetry;
    vector<size_t>::iterator itpair   = _nextSet.begin();

    const VSlicingNodes& children = _HVSnode->getChildren();
    for ( size_t ichild=0 ; ichild<children.size() ; ++ichild ) {
      if (isSymmetry(ichild,symmetry))
        _nextSet[ ichild ] = _nextSet[ symmetry.first ];
      else {
        if ( (((_counter-1)%_modulos[ichild]) == _modulos[ichild]-1)
           and not children[ichild]->isPreset() ) {
          if (_nextSet[ichild]+1 != children[ichild]->getNodeSets()->size() ) _nextSet[ichild]++;
          else                                                                _nextSet[ichild] = 0;
        }
      }
    }

    _counter   += 1;
    _currentSet = _nextSet;
  }


// -------------------------------------------------------------------
// Class  :  "Bora::HSetState".
  

  HSetState::HSetState ( HSlicingNode* node )
    : HVSetState(node)
  { }


  HSetState::~HSetState ()
  { }


  pair<DbU::Unit,DbU::Unit>  HSetState::getCurrentWs ()
  {
  // Notes:
  //   Calculate the min and max width of the current combination Routing nodes
  //   need to be taken into account to not calculate a wrong width.

    DbU::Unit wmin = 0;
    DbU::Unit wmax = 0;

    if (not _currentSet.empty()) { 
      const VSlicingNodes& children = _HVSnode->getChildren();
      for ( size_t ichild=0 ; (wmin == 0) and (ichild<children.size()) ; ++ichild ) {
        NodeSets* nodes = children[ichild]->getNodeSets();
        wmin = nodes->at( _currentSet[ichild] )->getWidth();
      }

      for ( size_t ichild=0 ; ichild<children.size() ; ++ichild ) {
        NodeSets* nodes = children[ichild]->getNodeSets();
        DbU::Unit width = nodes->at( _currentSet[ichild] )->getWidth();

        if ( width and (width < wmin) ) wmin = width;
        if             (width > wmax)   wmax = width;
      }
    }

    return pair<DbU::Unit,DbU::Unit>( wmin, wmax );  
  }


  DbU::Unit  HSetState::getCurrentH ()
  {
  // Notes:
  //   Calculate the height of the current combination.
  //   Routing nodes need to be taken into account to not calculate a wrong height.

    DbU::Unit currentH = 0;

    const VSlicingNodes& children = _HVSnode->getChildren();
    for ( size_t ichild=0 ; ichild<children.size() ; ++ichild ) {
      NodeSets* nodes = children[ichild]->getNodeSets();
      currentH += nodes->at( _currentSet[ichild] )->getHeight();
    }

    return currentH;
  }


  DbU::Unit  HSetState::getCurrentW ()
  {
  // Notes:
  //   Calculate the width of the current combination
  //   Routing nodes need to be taken into account to not calculate a wrong width.

    DbU::Unit currentW = 0;

    const VSlicingNodes& children = _HVSnode->getChildren();
    for ( size_t ichild=0 ; ichild<children.size() ; ++ichild ) {
      NodeSets* nodes = children[ichild]->getNodeSets();

      currentW = std::max( currentW, nodes->at( _currentSet[ichild] )->getWidth() );
    }

    return currentW;
  }
  

  void  HSetState::print ()
  {
    HVSetState::print();
    cerr << "currentH = " << getCurrentH() << endl;
    cerr << "currentW = " << getCurrentW() << endl;
    cerr << endl;
  }

  
  void  HSetState::next ()
  {
    push_back(); 
    HVSetState::next();
  }


  void  HSetState::push_back ()
  {
  // Notes:
  //   Check if conditions on tolerance are filled.
  //   If yes, add the current set to the NodeSets

    pair<DbU::Unit,DbU::Unit> paireWidths = getCurrentWs();
    DbU::Unit                 width       = paireWidths.second;
    DbU::Unit                 wmin        = paireWidths.first;
    DbU::Unit                 height      = 0;

    if (width - wmin <= _HVSnode->getToleranceBandW()) {
      vector<BoxSet*> bss;

      const VSlicingNodes& children = _HVSnode->getChildren();
      for ( size_t ichild=0 ; ichild<children.size() ; ++ichild ) {
        NodeSets* nodes = children[ichild]->getNodeSets();

        bss.push_back( nodes->at( _currentSet[ichild] ) );
        height += bss.back()->getHeight();
      }

    // create the BoxSet of the current accepted set.
      _nodeSets->push_back( bss, height, width, HorizontalSNode );
    }
  }


// -------------------------------------------------------------------
// Class  :  "Bora::VSetState".
  

  VSetState::VSetState ( VSlicingNode* node )
    : HVSetState(node)
  { }


  VSetState::~VSetState ()
  { }


  pair<DbU::Unit,DbU::Unit>  VSetState::getCurrentHs ()
  {
  // Note: Same as HSetState but for Vertical Node (see above).

    DbU::Unit hmin = 0;
    DbU::Unit hmax = 0;

    if (not _currentSet.empty()) {
      const VSlicingNodes& children = _HVSnode->getChildren();
      for ( size_t ichild=0 ; (hmin == 0) and (ichild<children.size()) ; ++ichild ) {
        NodeSets* nodes  = children[ichild]->getNodeSets();
        DbU::Unit height = nodes->at( _currentSet[ichild] )->getHeight();

        if ( height and (height < hmin) ) hmin = height;
        if              (height > hmax)   hmax = height;
      }
    }

    return pair<DbU::Unit,DbU::Unit>( hmin, hmax );  
  }


  DbU::Unit  VSetState::getCurrentH ()
  {
    DbU::Unit currentH = 0;

    const VSlicingNodes& children = _HVSnode->getChildren();
    for ( size_t ichild=0 ; ichild<children.size() ; ++ichild ) {
      NodeSets* nodes  = children[ichild]->getNodeSets();

      currentH = std::max( currentH, nodes->at( _currentSet[ichild] )->getHeight() );
    }

    return currentH;
  }


  DbU::Unit  VSetState::getCurrentW ()
  {
    DbU::Unit currentW = 0;

    const VSlicingNodes& children = _HVSnode->getChildren();
    for ( size_t ichild=0 ; ichild<children.size() ; ++ichild ) {
      NodeSets* nodes  = children[ichild]->getNodeSets();

      currentW += nodes->at( _currentSet[ichild] )->getWidth();
    }

    return currentW;
  }


  void  VSetState::print ()
  {
    HVSetState::print();
    cerr << "currentH = " << getCurrentH() << endl;
    cerr << "currentW = " << getCurrentW() << endl;
    cerr << endl;
  }


  void  VSetState::next ()
  {
    push_back(); 
    HVSetState::next();
  }


  void  VSetState::push_back ()
  {
    pair<DbU::Unit,DbU::Unit> paireHeights = getCurrentHs();
    DbU::Unit                 height       = paireHeights.second;
    DbU::Unit                 hmin         = paireHeights.first;
    DbU::Unit                 width        = 0;

    if (height - hmin <= _HVSnode->getToleranceBandH()) {
      vector<BoxSet*> bss;

      const VSlicingNodes& children = _HVSnode->getChildren();
      for ( size_t ichild=0 ; (hmin == 0) and (ichild<children.size()) ; ++ichild ) {
        NodeSets* nodes  = children[ichild]->getNodeSets();

        bss.push_back( nodes->at( _currentSet[ichild] ) );
        width += bss.back()->getWidth();
      }
      _nodeSets->push_back( bss, height, width, VerticalSNode );
    }
  }


}  // Bora namespace.
