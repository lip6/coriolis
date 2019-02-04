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
#include "hurricane/RoutingPad.h"
#include "hurricane/NetRoutingProperty.h"
#include "hurricane/analog/Device.h"
#include "katana/KatanaEngine.h"
#include "bora/HVSlicingNode.h"
#include "bora/HSlicingNode.h"
#include "bora/VSlicingNode.h"
#include "bora/DSlicingNode.h"
#include "bora/RHVSlicingNode.h"


namespace Bora {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Point;
  using Hurricane::Box;
  using Hurricane::RoutingPad;
  using Hurricane::NetRoutingState;
  using Hurricane::NetRoutingExtension;
  using Analog::Device;


// -------------------------------------------------------------------
// Class  :  "Bora::HVSlicingNode".


  HVSlicingNode::HVSlicingNode ( unsigned int type, unsigned int alignment )
    : Super( type, NodeSets::create(), alignment, NULL )
    , _children       ()
    , _toleranceRatioH(0)
    , _toleranceRatioW(0)
    , _toleranceBandH (0)
    , _toleranceBandW (0)
    , _symmetries     ()
    , _slicingRouting ()
    , _netSymmetries  ()
  { }


  HVSlicingNode::~HVSlicingNode ()
  { }


  DbU::Unit  HVSlicingNode::getToleranceRatioH () const
  { return _toleranceRatioH; }

  
  DbU::Unit  HVSlicingNode::getToleranceRatioW () const
  { return _toleranceRatioW; }

  
  DbU::Unit  HVSlicingNode::getToleranceBandH  () const
  { return _toleranceBandH;  }

  
  DbU::Unit  HVSlicingNode::getToleranceBandW  () const
  { return _toleranceBandW;  }

  
  void  HVSlicingNode::setToleranceRatioH ( DbU::Unit tolerance ) 
  {
    _toleranceRatioH = tolerance; 
    getRoot()->_resetSlicingTree();
  }


  void  HVSlicingNode::setToleranceRatioW ( DbU::Unit tolerance ) 
  {
    _toleranceRatioW = tolerance; 
    getRoot()->_resetSlicingTree();
  }


  void  HVSlicingNode::setToleranceBandH  ( DbU::Unit tolerance ) 
  {
    _toleranceBandH = tolerance; 
    resetSlicingTree();
  }


  void  HVSlicingNode::setToleranceBandW  ( DbU::Unit tolerance ) 
  {
    _toleranceBandW = tolerance; 
    resetSlicingTree();
  }


  void  HVSlicingNode::recursiveSetToleranceRatioH ( DbU::Unit tolerance )
  {
    _recursiveSetToleranceRatioH( tolerance );
    getRoot()->_resetSlicingTree();
  }


  void  HVSlicingNode::recursiveSetToleranceRatioW ( DbU::Unit tolerance )
  {
    _recursiveSetToleranceRatioW( tolerance );
    getRoot()->_resetSlicingTree();
  }


  void  HVSlicingNode::recursiveSetToleranceBandH ( DbU::Unit tolerance )
  {
    resetSlicingTree();
    _recursiveSetToleranceBandH( tolerance );
  }


  void  HVSlicingNode::recursiveSetToleranceBandW ( DbU::Unit tolerance )
  {
    resetSlicingTree();
    _recursiveSetToleranceBandW( tolerance );
  }


  void  HVSlicingNode::_recursiveSetToleranceRatioH ( DbU::Unit tolerance )
  {
    for ( SlicingNode* node : _children ) {
      if ( (not node->isDevice()) and (not node->isRouting()) )
        node->recursiveSetToleranceRatioH( tolerance );
    }
    _toleranceRatioH = tolerance; 
  }


  void  HVSlicingNode::_recursiveSetToleranceRatioW ( DbU::Unit tolerance )
  {
    for ( SlicingNode* node : _children ) {
      if ( (not node->isDevice()) and (not node->isRouting()) )
        node->recursiveSetToleranceRatioW( tolerance );
    }
    _toleranceRatioW = tolerance; 
  }


  void  HVSlicingNode::_recursiveSetToleranceBandH ( DbU::Unit tolerance )
  {
    for ( SlicingNode* node : _children ) {
      if ( (not node->isDevice()) and (not node->isRouting()) )
        node->recursiveSetToleranceBandH( tolerance );
    }
    _toleranceBandH = tolerance; 
  }


  void  HVSlicingNode::_recursiveSetToleranceBandW ( DbU::Unit tolerance )
  {
    for ( SlicingNode* node : _children ) {
      if ( (not node->isDevice()) and (not node->isRouting()) )
        node->recursiveSetToleranceBandW( tolerance );
    }
    _toleranceBandW = tolerance; 
  }


  void  HVSlicingNode::setTolerances ( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )
  {
    resetSlicingTree();
    _toleranceRatioH = trh;
    _toleranceRatioW = trw;
    _toleranceBandH  = tbh;
    _toleranceBandW  = tbw;
  }


  void  HVSlicingNode::recursiveSetTolerances ( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )
  {
    resetSlicingTree();
    _recursiveSetTolerances( trh, trw, tbh, tbw );
  }


  void  HVSlicingNode::_recursiveSetTolerances ( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )
  {
    _recursiveSetToleranceRatioH(trh);
    _recursiveSetToleranceRatioW(trw);
    _recursiveSetToleranceBandH (tbh);
    _recursiveSetToleranceBandW (tbw);
  }


  bool  HVSlicingNode::hasEmptyChildrenNodeSets () const
  {
  // WARNING: The original code was nonsensical, have to check it.
    for ( SlicingNode* node : _children ) {
      NodeSets* ns = node->getNodeSets();
      if (ns->empty() and not node->isRouting() )
        return true;
    }
    return false;
  }


  size_t  HVSlicingNode::getChildIndex ( SlicingNode* node ) const
  {
    for ( size_t i=0 ; i<_children.size() ; ++i ) {
      if (   (_children[i]->getType()   == node->getType()  )
         and (_children[i]->getHeight() == node->getHeight()) 
         and (_children[i]->getWidth () == node->getWidth ()) 
         and (_children[i]->getX()      == node->getX()     ) 
         and (_children[i]->getY()      == node->getY()     ) )
        return i;
    }
    return NodeSets::NotFound;
  }


  SlicingNode* HVSlicingNode::getChild ( size_t index ) const
  { return _children[index]; }


  void  HVSlicingNode::createChild ( unsigned int type, unsigned int alignment )
  {
    if (type == HorizontalSNode) {
      HSlicingNode* hsn = HSlicingNode::create( alignment );
      hsn->setTolerances( getToleranceRatioH()
                        , getToleranceRatioW()
                        , getToleranceBandH ()
                        , getToleranceBandW ()
                        );
      push_back( hsn );
      resetSlicingTree();
    } else if (type == VerticalSNode) { 
      VSlicingNode* vsn = VSlicingNode::create( alignment );
      vsn->setTolerances( getToleranceRatioH()
                        , getToleranceRatioW()
                        , getToleranceBandH ()
                        , getToleranceBandW ()
                        );
      push_back( vsn ); 
      resetSlicingTree();
    }
    else 
      cerr << Warning( "HVSlicingNode::createChild(SlicingType, Alignment): Unknown type." ) << endl;
  }   


  void  HVSlicingNode::createChild ( NodeSets*    nodeSets
                                   , unsigned int alignment
                                   , Instance*    instance
                                   , BoxSet*      boxSet
                                   )
  {
    DSlicingNode* node = DSlicingNode::create( nodeSets, alignment, instance, boxSet );
    node->setParent( this );
    push_back( node ); 
    resetSlicingTree();
  }
  

  void  HVSlicingNode::createChild ( size_t childIndex, size_t copyIndex, unsigned int tr )
  {
    if (childIndex != copyIndex) {
      SlicingNode* node = this->getChild( childIndex )->clone(tr);
      insertNode( node, copyIndex ); 
      _symmetries.push_back( Symmetry( min(childIndex, copyIndex), max(childIndex, copyIndex) ) );
      resetSlicingTree();
      normalizeSymmetries();
    }
    else
      cerr << Warning( "HVSlicingNode::createChild(size_t,size_t, unsigned int): Indexes cannot be the same." ) << endl;
  }  


  void  HVSlicingNode::insertNode ( SlicingNode* node, size_t index )
  {
    vector<SlicingNode*>::iterator it = _children.begin();
    for ( size_t i=0; i < index; i++ ) { if (it != _children.end()) ++it; }
    _children.insert( it, node );
    node->setParent( this );
    resetSlicingTree();
  }


  void  HVSlicingNode::push_back ( SlicingNode* node, bool reset ) 
  {
    node->setParent( this );
    _children.push_back( node ); 
    if (reset) resetSlicingTree();
  }


  void  HVSlicingNode::push_front ( SlicingNode* node ) 
  {
    node->setParent( this );
    _children.insert( _children.begin(), node ); 
    resetSlicingTree();
  }


  void HVSlicingNode::removeNode ( SlicingNode* node ) 
  {
    size_t index = 0;
    for ( VSlicingNodes::iterator inode = _children.begin()
        ; inode != _children.end()
        ; ++inode, ++index ) {
      if ((*inode) == node) {
        _children.erase( inode );
        removeSymmetry( index );
        resetSlicingTree();
        node->removeParent();
        node->setX        ( 0 );
        node->setY        ( 0 );
        node->setPlaced   ( false );
        return;
      }
    }
  }


  bool  HVSlicingNode::isSymmetry ( size_t index, Symmetry& symmetry )
  {
    if (_symmetries.empty()) return false;

    for ( LSymmetries::iterator isym=_symmetries.begin() ; isym != _symmetries.end() ; ++isym ) {
      if ((*isym).second == index) {
        symmetry = Symmetry( (*isym) ); 
        return true;
      }
    }
    return false;
  }


  bool  HVSlicingNode::isSymmetry ( size_t index )
  {
    if (_symmetries.empty()) return false;

    for ( Symmetry& sym : _symmetries ) {
      if (sym.second == index) return true;
    }
    return false;
  }


  void  HVSlicingNode::addSymmetry ( size_t childIndex, size_t copyIndex, bool reset )
  {
    if (childIndex >= getChildren().size()) {
      cerr << Warning( "HVSlicingNode::addSymmetry(size_t,size_t): Child index out of range %d (> %d), symmetry is ignored." 
                     , childIndex, getChildren().size() ) << endl;
      return;
    }

    if (copyIndex >= getChildren().size()) {
      cerr << Warning( "HVSlicingNode::addSymmetry(size_t,size_t): Copy index out of range %d (> %d), symmetry is ignored." 
                     , copyIndex, getChildren().size() ) << endl;
      return;
    }

    if (childIndex == copyIndex) {
      cerr << Warning( "HVSlicingNode::addSymmetry(size_t,size_t): Indexes cannot be identical (%d), symmetry is ignored." 
                     , childIndex ) << endl;
      return;
    }

    unsigned int symmetryType = 0;
    if (getType() == HorizontalSNode) symmetryType = HSymmetry;
    if (getType() == VerticalSNode  ) symmetryType = VSymmetry;
  
  
    if (not getChild(childIndex)->isSymmetric(getChild(copyIndex), symmetryType, ShowDiff)) { 
      cerr << Warning( "HVSlicingNode::addSymmetry(int,int)): Children %d and %d are not the same, symmetry is ignored." 
                     , childIndex, copyIndex ) << endl;
      return;
    }
  
    _symmetries.push_back( Symmetry( min(childIndex, copyIndex), max(childIndex, copyIndex) ) ); 
    if (reset) resetSlicingTree();
    unsigned int flag = 0;
    if (getType() == HorizontalSNode) flag = HSymmetry;
    if (getType() == VerticalSNode  ) flag = VSymmetry;
    getChild(copyIndex)->setSymmetryFlag( flag );

    getChild( copyIndex )->setMaster(getChild( childIndex ));
    normalizeSymmetries();
  }

  
  void  HVSlicingNode::removeSymmetry ( size_t index )
  {
    bool    first        = true;
    bool    isReference  = true;
    size_t  erasedFirst  = 0;
    size_t  erasedSecond = 0;
    
    for ( LSymmetries::iterator it = _symmetries.begin(); it != _symmetries.end(); it++ ) {
      if ( ( ((*it).first == index) or ((*it).second == index)) and first ) {
        LSymmetries::iterator itToerase = it;
        it++;
        resetSlicingTree(); 
        first = false;
        
        if ((*it).first == index){
          erasedFirst  = (*itToerase).first;
          erasedSecond = (*itToerase).second;
        } else
          isReference = false;

        _symmetries.erase(itToerase);
        it--;
      } else { 
        if ( not first and isReference ) {
          if ((*it).first == erasedFirst) (*it).first = erasedSecond;
        } 
        if ((*it).first  > index) (*it).first--;
        if ((*it).second > index) (*it).second--;
      }
    }
  }


  void  HVSlicingNode::normalizeSymmetries ()
  {
  // Notes :
  //   Symmetries must be described in a specific way, in case it is not, this method correct it.
  // Examples: 
  //   WRONG: Symmetries [< 1, 2 >, < 2, 3 >, < 3, 4 >]; RIGHT: [< 1, 2 >, < 1, 3 >, < 1, 4 >];
  //   WRONG: Symmetries [< 3, 4 >, < 5, 6 >, < 1, 2 >]; RIGHT: [< 1, 2 >, < 3, 4 >, < 5, 6 >];
  //   WRONG: Symmetries [< 1, 2 >, < 1, 2 >, < 3, 4 >]; RIGHT: [< 1, 2 >, < 3, 4 >];

    LSymmetries adjustedSymmetries;
    for ( Symmetry& sym : _symmetries ) {
      if (sym.first > sym.second) adjustedSymmetries.push_back( Symmetry( sym.second, sym.first  ) );
      else                        adjustedSymmetries.push_back( Symmetry( sym.first , sym.second ) ); 
    }
    adjustedSymmetries.sort();

    for ( LSymmetries::iterator isym = adjustedSymmetries.begin()
        ; isym != adjustedSymmetries.end()
        ; isym++ ) {
      for ( LSymmetries::iterator idup = isym
          ; idup != adjustedSymmetries.end()
          ; idup++ ) {
        if (idup == isym) continue;

        if ((*isym).second == (*idup).first) (*idup).first = (*isym).first;
        if ((*isym).second <  (*idup).first) break;
      }
    }

    _symmetries = adjustedSymmetries;
  }


  void  HVSlicingNode::_resetSlicingTree ()
  {
    _x = 0;
    _y = 0;
    setPlaced( false );
    
    if (not isPreset()) {
      _boxSet = NULL;
      setSet( false );
      
      for ( SlicingNode* child : _children ) child->_resetSlicingTree();
    }
  }


  void  HVSlicingNode::print () const
  {
    SlicingNode::print();

    if (not _symmetries.empty()) {
      cerr << "Symmetries: " << endl;
      for ( const Symmetry& sym : _symmetries )
        cerr << "Children: " << sym.first << " and " << sym.second << endl;
      cerr << endl;
    } else
      cerr << "Symmetries: None" << endl;

    if (not _slicingRouting.empty()) {
      cerr<< "Slicing Routing: " << endl;

      size_t index = 0;
      for ( const RHVSlicingNode* node : _slicingRouting ) {
        cerr << "---------------- " << setprecision(4) << index << " -----------------" << endl;
        cerr << "Print - Slicing Routing: ";
        cerr <<   "X: "      << DbU::getPhysical(node->getX     (), DbU::Micro);
        cerr << ", Y: "      << DbU::getPhysical(node->getY     (), DbU::Micro);
        cerr << ", height: " << DbU::getPhysical(node->getHeight(), DbU::Micro);
        cerr << ", width: "  << DbU::getPhysical(node->getWidth (), DbU::Micro) << endl;
        cerr << "GCell: "    << node->getGCell() << endl;

        if(node->getGCell()){
          cerr << "GCell      : " << node->getGCell() << endl;
      
          cerr << "Edges : " << endl;
          for ( const Anabatic::Edge* edge : node->getGCell()->getNorthEdges() )
            cerr << edge->getOpposite(node->getGCell()) << endl;

          for ( const Anabatic::Edge* edge : node->getGCell()->getSouthEdges() )
            cerr << edge->getOpposite(node->getGCell()) << endl;

          for ( const Anabatic::Edge* edge : node->getGCell()->getEastEdges() )
            cerr << edge->getOpposite(node->getGCell()) << endl;

          for ( const Anabatic::Edge* edge : node->getGCell()->getWestEdges() )
            cerr << edge->getOpposite(node->getGCell()) << endl;
        }

        cerr << "------------------------------------" << endl;
        ++index;
      }
    } else
      cerr << "Slicing Routing: empty" << endl;

    cerr << endl;
  }


  void  HVSlicingNode::printChildren () const
  {
    size_t index = 0;
    for ( const SlicingNode* child : _children ) {
      if ( child->isDevice() or child->isRouting() ) {
        cerr << "-- Children: " << index << "/" << _children.size()-1 << " --" << endl;
        child->print(); 
      } else {
        cerr << "-- Children: " << index << "/" << _children.size()-1 << " --" << endl;
        child->print();
        child->printChildren();
      }
      ++index;
    }
  }


  void  HVSlicingNode::printLine () const
  {
    SlicingNode::printLine();

    if (not _symmetries.empty()) {
      cerr << "Symmetries: " << endl;
      for ( const Symmetry& sym : _symmetries )
        cerr << "Children: " << sym.first << " and " << sym.second << endl;
      cerr << endl;
    }
  }

  
  void  HVSlicingNode::printChildrenLine () const
  {
    size_t index = 0;
    for ( const SlicingNode* child : _children ) {
      if ( child->isDevice() or child->isRouting() ) {
        cerr << "-- Children: " << index << "/" << _children.size()-1 << " --" << endl;
        child->printLine(); 
      } else {
        cerr << endl;
        cerr << "-- Children: " << index << "/" << _children.size()-1 << " --" << endl;
        child->printLine();
        child->printChildrenLine();
      }
      ++index;
    }
  }


  bool  HVSlicingNode::recursiveCheckPreset () const
  {
    bool preset = isPreset();
    if (preset) {
      for ( const SlicingNode* child : _children ) preset = child->recursiveCheckPreset(); 
    }
    return preset;
  }


  bool  HVSlicingNode::recursiveCheckSet () const
  {
    bool rset = isSet();   
    if (rset) {
      for ( const SlicingNode* child : _children ) rset = child->recursiveCheckSet(); 
    }
    return rset;
  }


  bool  HVSlicingNode::recursiveCheckPlaced () const
  {
    bool placed = isPlaced();
    if (placed) {
      for ( const SlicingNode* child : _children ) placed = child->recursiveCheckPlaced(); 
    }
    return placed;
  }


  int HVSlicingNode::getLeafNumber() const
  {
    int leafs = 0;
    for ( const SlicingNode* child : _children ) leafs += child->getLeafNumber(); 
    return leafs;  
  }


  double  HVSlicingNode::getDevicesArea () const
  {
    double area = 0.0;
    for ( const SlicingNode* child : _children ) area += child->getDevicesArea();
    return area;
  }


  double  HVSlicingNode::getOccupationArea () const
  {
    double estimation = 0;
    if (recursiveCheckPlaced())
      estimation = (100 * getDevicesArea()) / (getHeight() * getWidth());
    else
      cerr << Warning( "HVSlicingNode::getSpaceEstimation()): SlicingNodes dimensions need to be set first before being estimated." ) << endl;

    return estimation;
  }


  void  HVSlicingNode::setGlobalSize ( DbU::Unit height, DbU::Unit width )
  {
    if (not _nodeSets->empty()) {
      DbU::Unit  bestH    = 0;
      DbU::Unit  bestW    = 0;
      DbU::Unit  currentH = 0;
      DbU::Unit  currentW = 0;
      BoxSet*    boxSet   = _nodeSets->at( 0 );
      
      for ( BoxSet* bs : _nodeSets->getBoxSets() ) {
        currentH = bs->getHeight();
        currentW = bs->getWidth();
        if ( (currentH <= height) and (currentW <= width) ) {
          if ( ((height-currentH) <= _toleranceRatioH) and ((height-bestH) <= _toleranceRatioH) ) {
            if (currentW > bestW) { 
              bestH  = currentH;
              bestW  = currentW;
              boxSet = bs;
            }
          } else if (currentH > bestH) {
            bestH  = currentH;
            bestW  = currentW;
            boxSet = bs;
          }
        }
      }

      _setGlobalSize( boxSet );
    } else
      cerr << Warning( "HVSlicingNode::setGlobalSize(DbU::Unit h, DbU::Unit w): NodeSets empty. UpdateGlobalSize needs to be used first or with higher tolerances." ) << endl;
  }


  void  HVSlicingNode::setGlobalSize ( size_t index )
  {
    if (not _nodeSets->empty()) {
      if (index+1 > _nodeSets->size())
        cerr << Warning( "HVSlicingNode::setGlobalSize(size_t): Out of bound index." ) << endl;
      else
        _setGlobalSize( _nodeSets->at(index) );
    } else
      cerr << Warning( "HVSlicingNode::setGlobalSize(size_t): NodeSets empty. UpdateGlobalSize needs to be used first or with higher tolerances." ) << endl;
  }


  void  HVSlicingNode::_setGlobalSize ( BoxSet* boxSet ) 
  {
    if (not isPreset()) {
      if (not getMaster())
        _setBoxSet( boxSet );  
      else
        _setBoxSet( _master->getBoxSet() );  

      if (  ((getType() == HorizontalSNode) and (isHSymmetry()))
         or ((getType() == VerticalSNode  ) and (isVSymmetry())) ) {
        vector<BoxSet*>::const_iterator ibs = boxSet->getSet().begin();
        for ( VSlicingNodes::reverse_iterator ichild = _children.rbegin()
            ; ichild != _children.rend(); ++ichild ) {
          (*ichild)->_setGlobalSize( *ibs );
          ibs++;
        }
      } else {
        vector<BoxSet*>::const_iterator ibs = boxSet->getSet().begin();
        for ( VSlicingNodes::iterator ichild = _children.begin()
            ; ichild != _children.end(); ichild++){
          (*ichild)->_setGlobalSize( *ibs );
          ibs++;
        }
      }
    }
  }


  void  HVSlicingNode::preDestroy ()
  {
    Super::preDestroy();
    for ( SlicingNode* child : _children ) child->removeParent();
  }


  void  HVSlicingNode::destroy ()
  {
    HVSlicingNode::preDestroy();
    delete this;
  }


  void  HVSlicingNode::preRecursiveDestroy ()
  {
    Super::preRecursiveDestroy();
    for ( SlicingNode* child : _children ) {
      if ( (child->getType() == HorizontalSNode) or (child->getType() == VerticalSNode) ) {
        child->removeParent();
        child->recursiveDestroy();
      } else {
        child->removeParent();
        child->destroy();
      }
    }
  }


  void  HVSlicingNode::recursiveDestroy ()
  {
    HVSlicingNode::preRecursiveDestroy();
    delete this;
  }


  LSlicingNodes  HVSlicingNode::getLeaves ()
  {
    LSlicingNodes leaves;
    for ( SlicingNode* child : _children ) {
      if ( (child->getType() == DeviceSNode) or (child->getType() == RoutingSNode) )
        leaves.push_back( child );
      else
        leaves.splice( leaves.end(), child->getLeaves() );
    }
    return leaves;
  }


  bool  HVSlicingNode::checkInitialPlacement ( int& cpt ) const
  {
  // Notes::
  //   Initial placement criteria is having at least 2 devices placed at (x,y) = (0,0)

    bool initialPlacement = false;
    if (cpt < 2) {
      initialPlacement = true;
      for ( const SlicingNode* child : _children ) {
        if (cpt < 2) initialPlacement = child->checkInitialPlacement( cpt );
      }
    }
    return initialPlacement;
  }


  bool  HVSlicingNode::isSame ( SlicingNode* node, unsigned int flags ) const
  {
    if (   (getType()           == node->getType()          )
       and (getNbChild()        == node->getNbChild()       ) 
       and (getToleranceBandH() == node->getToleranceBandH())
       and (getToleranceBandW() == node->getToleranceBandW()) ) {
      bool isSame = true;
      
      for ( size_t ichild = 0 ; isSame and (ichild < getNbChild()) ; ++ichild ) {
        isSame = getChild(ichild)->isSame( node->getChild(ichild) );
      }

      if (not isSame and (flags & ShowDiff))
        cerr << "HVSlicingNode::isSame() " << this << " and " << node << " differs (childs)." << endl;

      return isSame;
    } else {
      if (flags & ShowDiff)
        cerr << "HVSlicingNode::isSame() " << this << " and " << node << " differs." << endl;
      return false;
    }
  }


  bool  HVSlicingNode::checkCellInstances ( Cell* cell )
  {
    if (not cell) {
      cerr << Warning( "HVSlicingNode::checkCellInstances(): Cell is NULL." ) << endl;
      return false;
    }

    for ( SlicingNode* child : _children ) {
      if (not child->checkCellInstances(cell)) return false;
    }
    return true;
  }


  SlicingNode* HVSlicingNode::findInstance ( Instance* instance )
  {
    for ( SlicingNode* child : _children ) {
      if ( (child->getType() == HorizontalSNode) or (child->getType() == VerticalSNode) ) {
        SlicingNode* node = child->findInstance( instance );
        if (node) return node;
      } else if (child->getType() == DeviceSNode) {
        if ( child->getInstance() == instance )
          return child;
      }
    }
    return NULL;
  }


  SlicingNode* HVSlicingNode::findSlicingNode ( Anabatic::GCell* gcell )
  {
    if (getGCell() == gcell) return this;
    
    for ( RHVSlicingNode* node : _slicingRouting ) {
      if (node->getGCell() == gcell) return node;
    }
  
    for ( SlicingNode* child : _children ) {
      if( (child->getType() == HorizontalSNode) or (child->getType() == VerticalSNode) ) {
        SlicingNode* node = child->findSlicingNode( gcell );
        if (node) return node;
      } else if (child->getType() == DeviceSNode) {
        if (child->getGCell() == gcell) 
          return child;
      }
    }

    return NULL;
  }


  void HVSlicingNode::resetSlicingRouting ()
  {
    for ( RHVSlicingNode* node  : _slicingRouting ) node->resetSize();
    for ( SlicingNode*    child : _children ) {
      if ( (child->getType() == HorizontalSNode) or (child->getType() == VerticalSNode) )
        child->resetSlicingRouting();
    }
    setRoutingEstimated(0);
  }


  void  HVSlicingNode::destroySlicingRouting ()
  {
    for ( RHVSlicingNode* node : _slicingRouting ) node->destroy();
    _slicingRouting.clear();

    for ( SlicingNode* child : _children ) {
      if ( (child->getType() == HorizontalSNode) or (child->getType() == VerticalSNode) )
        child->destroySlicingRouting();
    }

    setRoutingEstimated(0);
  }


  size_t  HVSlicingNode::getRoutingIndex ( SlicingNode* node ) const
  {
    size_t index = 0;
    for ( const RHVSlicingNode* node : _slicingRouting ) {
      if (   (node->getHeight() == node->getHeight()) 
         and (node->getWidth () == node->getWidth ()) 
         and (node->getX     () == node->getX     ()) 
         and (node->getY     () == node->getY     ()) )
        return index;
      ++index;
    }

    return NodeSets::NotFound;
  }


  SlicingNode* HVSlicingNode::getSlicingRouting ( size_t index ) const
  { return _slicingRouting.at( index ); }


  void  HVSlicingNode::restrictDevices ()
  {
    for ( SlicingNode*    child : _children       ) child->restrictDevices();
    for ( RHVSlicingNode* node  : _slicingRouting ) node ->restrictDevices();
  }


  void  HVSlicingNode::setVertexRestriction ( Net* net, Katana::KatanaEngine* katana )
  { 
    cdebug_log(536,1) << "HVSlicingNode::setVertexRestriction(Net*,KatanaEngine*)" << endl;

    restrictDevices();
    vector<RoutingPad*> rps;
    for ( RoutingPad* rp : net->getRoutingPads() ) rps.push_back( rp );
    for ( RoutingPad* rp : rps ) {
      Box    rpBb   = rp->getBoundingBox();
      Point  center = rpBb.getCenter();
      Anabatic::GCell* gcell  = katana->getGCellUnder( center );
        
      if (not gcell) {
        cerr << Error( "HVSlicingNode::setVetexRestriction(): %s of %s is not under any GCell.\n"
                       "        It will be ignored ans the routing will be incomplete."
                     , getString(rp  ).c_str()
                     , getString(net).c_str()
                     ) << endl;
        continue;
      }

      Anabatic::Vertex* vertex = gcell->getObserver<Anabatic::Vertex>(Anabatic::GCell::Observable::Vertex);
    // Analog Restrictions
      Plug*             plug   = dynamic_cast<Plug*>( rp->getPlugOccurrence().getEntity() );
      Cell*             cell   = plug->getInstance()->getMasterCell();
      Device*           device = dynamic_cast<Device*>( cell );

      if (device) {
        cdebug_log(536,0) << "Underlying device: " << device << endl;

        unsigned int rule = device->getRestrictions2(plug->getMasterNet());
        vertex->clearRestriction();

        cdebug_log(536,0) << "Restrictions rule: " << rule << endl;
        if (rule & WestBlocked ) vertex->setWRestricted();
        if (rule & EastBlocked ) vertex->setERestricted();
        if (rule & SouthBlocked) vertex->setSRestricted();
        if (rule & NorthBlocked) vertex->setNRestricted();
        cdebug_log(536,0) << "Applied restrictions: " << vertex << endl;
      } else {
        vertex->clearRestriction();
      }
    }
    
    cdebug_tabw(536,-1);
  }


  void  HVSlicingNode::estimateChannelsSize ()
  {
    for ( RHVSlicingNode* node  : _slicingRouting ) node ->estimateChannelsSize();
    for ( SlicingNode*    child : _children       ) child->estimateChannelsSize();
  }


  void  HVSlicingNode::expandRoutingChannel ()
  {
    estimateChannelsSize();
    _expandRoutingChannel();
  }


  void  HVSlicingNode::_expandRoutingChannel ()
  {
    for ( RHVSlicingNode* node : _slicingRouting ) node->_expandRoutingChannel();
    for ( SlicingNode*    child : _children ) {
      if (child->getType() != RoutingSNode) child->_expandRoutingChannel();
    }
    setRoutingEstimated(RoutingEstimated);
    if (not _parent) {
      for ( SlicingNode* child : _children ) child->adjustBorderChannels();
    }
  }


  void  HVSlicingNode::expandRoutingChannel ( DbU::Unit height, DbU::Unit width )
  {
    for ( RHVSlicingNode* node : _slicingRouting ) node->expandRoutingChannel( height, width );
    for ( SlicingNode*    child : _children ) {
      if (child->getType() != RoutingSNode) child->expandRoutingChannel( height, width );
    }
    setRoutingEstimated( RoutingEstimated );
    if (_parent == NULL)
      for ( SlicingNode* child : _children ) child->adjustBorderChannels();
  }


  bool  HVSlicingNode::isRoutingEstimated () const
  {
    bool estimated = true;
    if (_slicingRouting.empty()){
      estimated = false;
    } else {
      for ( RHVSlicingNode* node : _slicingRouting ) estimated &= node->isRoutingEstimated();
    }

    for ( SlicingNode* child : _children ) {
      if ( (child->getType() == HorizontalSNode ) or (child->getType() == VerticalSNode) )
        estimated &= child->isRoutingEstimated();
    }

    return estimated;
  }


  void  HVSlicingNode::updateGCellPosition ()
  {
    cdebug_log(535,1) << "HVSlicingNode::updateGCellPosition()" << endl;

    for ( SlicingNode* node  : _slicingRouting ) node ->updateGCellPosition();
    for ( SlicingNode* child : _children       ) child->updateGCellPosition();

    cdebug_tabw(535,-1);
  }


  void  HVSlicingNode::updateGContacts ()
  {
    cdebug_log(535,1) << "HVSlicingNode::updateGContacts()" << endl;

    for ( SlicingNode* node  : _slicingRouting ) node ->updateGContacts();
    for ( SlicingNode* child : _children       ) child->updateGContacts();

    cdebug_tabw(535,-1);
  }


  void  HVSlicingNode::clearGCells ()
  {
    for ( RHVSlicingNode* node  : _slicingRouting ) node ->clearGCells();
    for ( SlicingNode*    child : _children       ) child->clearGCells();
    _gcell = NULL;
  }


  int  HVSlicingNode::getNbDevices ()
  {
    int count = 0;
    for ( SlicingNode* child : _children ) count += child->getNbDevices();
    return count;
  }
  

  string  HVSlicingNode::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " childs:" + getString(_children.size()) );
    return s;
  }


  string  HVSlicingNode::_getTypeName () const
  { return "HVSlicingNode"; }


  void  HVSlicingNode::setSymmetryFlag ( unsigned int flag )
  {
    SlicingNode::setSymmetryFlag( flag );
    for ( RHVSlicingNode* node  : _slicingRouting ) node ->setSymmetryFlag( flag );
    for ( SlicingNode*    child : _children       ) child->setSymmetryFlag( flag );
  }


  void  HVSlicingNode::setMaster ( SlicingNode* master ) 
  {
    _master = master;
  
    if (  ((getType() == HorizontalSNode) and isHSymmetry())
       or ((getType() == VerticalSNode  ) and isVSymmetry()) ) {
      for ( size_t i=0 ; i<_children.size() ; ++i )
        _children[ _children.size()-1 - i ]->setMaster( master->getChild(i) );
    } else {
      for ( size_t i=0 ; i<_children.size() ; ++i )
        _children[ i ]->setMaster( master->getChild(i) );
    }
  }


  bool  HVSlicingNode::isSymmetric ( SlicingNode* node, unsigned int symmetryType, unsigned int flags ) const
  {
    if (   (getType()           == node->getType()          )
       and (getNbChild()        == node->getNbChild()       ) 
       and (getToleranceBandH() == node->getToleranceBandH())
       and (getToleranceBandW() == node->getToleranceBandW()) ) {
      bool isSame = true;

      if (  ((getType() == HorizontalSNode) and (symmetryType == HSymmetry))
         or ((getType() == VerticalSNode  ) and (symmetryType == VSymmetry)) ) {
        size_t nbChilds = _children.size();
        for ( size_t i=0 ; isSame and (i<nbChilds) ; ++i ) {
          isSame = getChild(i)->isSymmetric( node->getChild(nbChilds-1-i), symmetryType );
        }
      } else {
        size_t nbChilds = _children.size();
        for ( size_t i=0 ; isSame and (i<nbChilds) ; ++i ) {
          isSame = getChild(i)->isSymmetric( node->getChild(i), symmetryType );
        }
      }

      if (not isSame and (flags & ShowDiff))
        cerr << Warning( "HVSlicingNode::isSame() %s and %s differs (childs)."
                       , getString(this).c_str()
                       , getString(node).c_str() ) << endl;
      return isSame;
    }

    if (flags & ShowDiff)
      cerr << Warning( "HVSlicingNode::isSame() %s and %s differs."
                     , getString(this).c_str()
                     , getString(node).c_str() ) << endl;
    return false;
  }


  bool  HVSlicingNode::checkSymmetryNet ( unsigned int type, Net* net1, Net* net2 ) const
  {
    for ( const NetSymmetry& netSym : _netSymmetries ) {
      if (   (get<1>(netSym) == net1)
         and (get<2>(netSym) == net2)
         and (get<0>(netSym) == type) )
        return true;
    }
    return false;
  }


  void  HVSlicingNode::addSymmetryNet ( unsigned int type, Net* net1, Net* net2 )
  {
    if (checkSymmetryNet(type,net1,net2)) {
      cerr << Warning( "HVSlicingNode::addSymmetryNet(): Net symmetry already set." ) << endl;
      return;
    }

    unsigned int ftype = type;
    if      (type == 1) ftype = NetRoutingState::Vertical;
    else if (type == 2) ftype = NetRoutingState::Horizontal;
    _netSymmetries.push_back( make_tuple(ftype,net1,net2) );
  }


  void  HVSlicingNode::updateNetConstraints ()
  {
    if (not _cell) {
      cerr << Warning( "HVSlicingNode::updateNetConstraints(): Cell not set." ) << endl;
      return;
    }

    for ( NetSymmetry& netSym : _netSymmetries ) {
      if (get<2>(netSym)) {
        Net* masterNet = get<1>(netSym);
        if (not masterNet)
          cerr << Warning( "HVSlicingNode::updateNetConstraints() Reference Net not set." ) << endl;
        else {
          NetRoutingState* masterState = NetRoutingExtension::get( masterNet );
          if (not masterState) masterState = NetRoutingExtension::create( masterNet );

          masterState->setFlags  ( NetRoutingState::AutomaticGlobalRoute
                                 | NetRoutingState::Symmetric
                                 | NetRoutingState::SymmetricMaster
                                 | get<0>(netSym) );
          
          Net*             slaveNet   = get<2>(netSym);
          NetRoutingState* slaveState = NetRoutingExtension::get( slaveNet );
          if (not slaveState) slaveState = NetRoutingExtension::create( slaveNet );
          
          slaveState ->setFlags ( NetRoutingState::AutomaticGlobalRoute
                                | NetRoutingState::Symmetric
                                | get<0>((netSym)) );
          slaveState ->setSymNet( masterNet );
          masterState->setSymNet( slaveNet  );
        }
      } else {
        Net* net = get<1>(netSym);
        if (not net)
          cerr << Warning( "HVSlicingNode::updateNetConstraints() Single master Net not set." ) << endl;
        else {
          NetRoutingState* state = NetRoutingExtension::get( net );
          if (not state) state = NetRoutingExtension::create( net );

          state->setFlags( NetRoutingState::AutomaticGlobalRoute
                         | NetRoutingState::Symmetric
                         | get<0>(netSym) );
        }
      }
    }

    for ( SlicingNode* child : _children ) {
      if ( (child->getType() == HorizontalSNode) or (child->getType() == VerticalSNode) )
        child->updateNetConstraints();
    }
  }


  void  HVSlicingNode::updateSymNetAxis ()
  {
    if (not _cell) {
      cerr << Warning( "HVSlicingNode::updateSymNetAxis(): Cell not set." ) << endl;
      return;
    }
    
    for ( const NetSymmetry& symNet : _netSymmetries ) {
      SlicingNode* n1      = getChild( _symmetries.front().first  );
      SlicingNode* n2      = getChild( _symmetries.front().second );
      DbU::Unit    xCenter = (n1->getX() + n2->getX() + n2->getWidth())/2;
      DbU::Unit    yCenter = (n1->getY() + n2->getY() + n2->getHeight())/2;

      if (get<2>(symNet)) {
        Net* masterNet = get<1>( symNet );
        if (not masterNet)
          cerr << Warning( "HVSlicingNode::updateSymNetAxis() Master net not set." ) << endl;
        else {
          NetRoutingState* masterState = NetRoutingExtension::get( masterNet );
          if (not masterState)
            cerr << Warning( "HVSlicingNode::updateSymNetAxis(): UpdateNetConstraint need to be called first." ) << endl;
          else {
            if      (get<0>(symNet) == NetRoutingState::Vertical  ) masterState->setSymAxis( xCenter );
            else if (get<0>(symNet) == NetRoutingState::Horizontal) masterState->setSymAxis( yCenter );
            else
              cerr << Warning( "HVSlicingNode::updateSymNetAxis(): Unknown NetRoutingState type." ) << endl;
        
            Net*             slaveNet   = get<2>(symNet);
            NetRoutingState* slaveState = NetRoutingExtension::get( slaveNet );
            if (not slaveState)
              cerr << Warning( "HVSlicingNode::updateSymNetAxis(): updateSymNetAxis need to be called first." ) << endl;
            else {
              if      (get<0>(symNet) == NetRoutingState::Vertical  ) slaveState->setSymAxis( xCenter );
              else if (get<0>(symNet) == NetRoutingState::Horizontal) slaveState->setSymAxis( yCenter );
              else
                cerr << Warning( "HVSlicingNode::updateSymNetAxis(): Unknown NetRoutingState type." ) << endl;
            }
          }
        }
      } else {
        Net* net = get<1>(symNet);
        if (not net)
          cerr << Warning( "HVSlicingNode::updateSymNetAxis() Signle master net not set." ) << endl;
        else {
          NetRoutingState* state = NetRoutingExtension::get( net );
          if (not state)
            cerr << Warning( "HVSlicingNode::updateSymNetAxis(): UpdateNetConstraint need to be called first." ) << endl;
          else {
            if      (get<0>(symNet) == NetRoutingState::Vertical  ) state->setSymAxis( xCenter );
            else if (get<0>(symNet) == NetRoutingState::Horizontal) state->setSymAxis( yCenter );
            else
              cerr << Warning( "HVSlicingNode::updateSymNetAxis(): Unknown NetRoutingState type." ) << endl;
          }
        }
      }

      for ( SlicingNode* child : _children ) {
        if ( (child->getType() == HorizontalSNode) or (child->getType() == VerticalSNode) )
          child->updateSymNetAxis();
      }
    }
  }


  void HVSlicingNode::flattenDigitalNets ()
  {
    if (not _cell) {
      cerr << Warning( "HVSlicingNode::flattenDigitalNets(): Cell not set." ) << endl;
      return;
    }

    for ( SlicingNode* child : _children ) {
      if ( (child->getType() == HorizontalSNode) or (child->getType() == VerticalSNode) ) {
        child->flattenDigitalNets();
      } else if (child->getType() == DeviceSNode) {
        if (   child->getGCell()
           and child->getInstance()
           and child->getGCell()->isMatrix()) {
          _cell->flattenNets( child->getInstance()
                            , Cell::Flags::BuildRings|Cell::Flags::WarnOnUnplacedInstances );
        }
      }
    }
  }


  void  HVSlicingNode::updateWireOccupation ( Anabatic::Dijkstra* dijkstra )
  {
    cdebug_log(535,1) << "HVSlicingNode::updateWireOccupation() on " << this << endl;

    if (not _parent) {
      for ( Anabatic::Vertex* vertex : dijkstra->getSources() ) {
        cdebug_log(535,0) << "> " << vertex << endl;

        Anabatic::GCell* gcell = vertex->getGCell();
        SlicingNode*     snode = findSlicingNode( gcell );

        if (snode) {
          cdebug_log(535,0) << "| isRouting():" << snode->isRouting() << endl; 
        
          if (snode->isRouting() and vertex->hasAData() )
            snode->addWireOccupation( vertex->getIMin(), vertex->getIMax(), dijkstra->getNet() );
        }
      }
    }

    cdebug_tabw(535,-1);
  }


  void  HVSlicingNode::resetWireOccupation ()
  {
    for ( RHVSlicingNode* node  : _slicingRouting ) node ->resetWireOccupation();
    for ( SlicingNode*    child : _children       ) child->resetWireOccupation();
  }


}  // Bora namespace.
