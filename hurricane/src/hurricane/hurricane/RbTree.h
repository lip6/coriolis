// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2018-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/RbTree.h"                          |
// +-----------------------------------------------------------------+
//
// References:
//    1. Wikipedia, https://en.wikipedia.org/wiki/Red-black_tree
//    2. Introduction to Algorithms, Cormen & Al,
//       Third edition, MIT press, 2011, p. 308.


#pragma  once
#include <functional>
#include <fstream>
#include <sstream>
#include "hurricane/Error.h"
#include "hurricane/Collection.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::RbTree".
  
  template< typename Data, typename Compare=std::less<Data> >
  class RbTree {
    public:
      typedef  Data     DataType;
      typedef  Compare  CompareType;
    public:
      static const long  NoFlags = 0;
      static const long  Black   = (1<< 0);
      static const long  GoRight = (1<< 1);
    public:
      class Node {
        public:
          inline               Node           ( const Data& value, Node* parent );
          virtual             ~Node           ();
          inline  bool         isRed          () const;
          inline  bool         isBlack        () const;
          inline  bool         isRoot         () const;
          inline  bool         isLeaf         () const;
          inline  bool         hasLeftChild   ( const Node* ) const;
          inline  bool         hasRightChild  ( const Node* ) const;
          inline  const Data&  getValue       () const;
          inline  Node*        getLeft        () const;
          inline  Node*        getRight       () const;
          inline  Node*        getParent      () const;
          inline  Node*        getGrandParent () const;
          inline  Node*        getSibling     () const;
          inline  Node*        getUncle       () const;
          inline  Node*        getMin         () const;
          inline  Node*        getMax         () const;
          inline  long         getFlags       () const;
          inline  void         setLeft        ( Node* );
          inline  void         setRight       ( Node* );
          inline  void         setParent      ( Node* );
          inline  void         setFlags       ( long );
          inline  void         resetFlags     ( long );
          inline  void         setRed         ();
          inline  void         setBlack       ();
          inline  void         copyColor      ( Node* );
                  void         updateEdge     ( Node* oldChild, Node* newChild );
                  void         clear          ();
          inline  void         swap           ( Node* );
          virtual std::string  _getString     () const;
          virtual Record*      _getRecord     () const;
        private:
                 Data    value_;
                 long    flags_;
                 Node*   parent_;
                 Node*   left_;
                 Node*   right_;
      };
    public:
      class iterator {
          friend class RbTree;
        protected:
          inline              iterator   ( const Node* );
        public:              
          virtual iterator&   operator++ ();
          inline  bool        operator== ( const iterator& ) const;
          inline  bool        operator!= ( const iterator& ) const;
          inline  const Data& operator*  () const;
          inline  bool        isValid    () const;
          inline  const Node* getNode    () const;
          inline  void        setNode    ( const Node* );
        private:
          const Node* node_;
      };
    public:
      class Elements : public Collection<Data> {
        public:
        // Sub-Class: Locator.
          class Locator : public Hurricane::Locator<Data> {
            public:
                               Locator    ( const RbTree& );
              inline           Locator    ( const Locator& );
              virtual Data     getElement () const;
              virtual Locator* getClone   () const;
              virtual bool     isValid    () const;
              virtual void     progress   ();
              virtual string   _getString () const;
            protected:
              iterator  iterator_;
          };
        public:
        // Class Elements.
          inline                    Elements   ( const RbTree& );
          inline                    Elements   ( const Elements& );
          virtual Collection<Data>* getClone   () const;
          virtual Locator*          getLocator () const;
          virtual string            _getString () const;
        protected:
          const RbTree& tree_;
      };
    public:
                                RbTree          ();
                               ~RbTree          ();
              void              rotateLeft      ( Node* );
              void              rotateRight     ( Node* );
      virtual void              postRotateLeft  ( Node* );
      virtual void              postRotateRight ( Node* );
              iterator          find            ( const Data& value );
              void              insert          ( const Data& value );
              void              remove          ( const Data& value );
      virtual void              postInsert      ( Node* );
      virtual void              postRemove      ( Node* );
      inline  size_t            size            () const;
      inline  bool              empty           () const;
      inline  iterator          begin           () const;
      inline  iterator          end             () const;
              iterator          lower_bound     ( const Data& value ) const;
              iterator          upper_bound     ( const Data& value ) const;
      inline  Elements          getElements     () const;
      inline  void              write           ( std::string ) const;
      inline  void              clear          ();
      virtual std::string       _getTypeName    () const;
      virtual std::string       _getString      () const;
      virtual Record*           _getRecord      () const;
      friend  std::ostream&     operator<<      ( std::ostream& o, const Node* node )
                                                {
                                                  if (node) {
                                                    o << "<Node " << ::getString(node->getValue()) << " ";
                                                    o << ((node->isBlack ()) ? "B" : "R");
                                                    o << ((node->getLeft ()) ? "l" : "-");
                                                    o << ((node->getRight()) ? "r" : "-");
                                                    o << ">";
                                                  } else
                                                    o << "<Node NULL>";
                                                  return o;
                                                }
    public:
      inline  Node*             getRoot         () const;
    private:                                    
              void              insertRecurse   ( Node* );
              void              insertRepair    ( Node*, size_t rdepth );
              void              removeRepair    ( Node*, size_t rdepth );
    private:
      size_t   count_;
      Node*    root_;
      Compare  compare_;
  };

  
// -------------------------------------------------------------------
// Class  :  "Hurricane::RbTree::Node".
  
  
  template< typename Data, typename Compare >
  inline RbTree<Data,Compare>::Node::Node ( const Data& value, Node* parent )
    : value_ (value)
    , flags_ (NoFlags)
    , parent_(parent)
    , left_  (NULL)
    , right_ (NULL)
  { }
  
  template< typename Data, typename Compare >
  RbTree<Data,Compare>::Node::~Node ()
  { }
  
  template< typename Data, typename Compare >inline bool        RbTree<Data,Compare>::Node::isRed          () const { return not isBlack(); }
  template< typename Data, typename Compare >inline bool        RbTree<Data,Compare>::Node::isBlack        () const { return (flags_ & RbTree::Black); }
  template< typename Data, typename Compare >inline bool        RbTree<Data,Compare>::Node::isRoot         () const { return not parent_; }
  template< typename Data, typename Compare >inline bool        RbTree<Data,Compare>::Node::isLeaf         () const { return not left_ and not right_; }
  template< typename Data, typename Compare >inline bool        RbTree<Data,Compare>::Node::hasLeftChild   ( const Node* child ) const { return (left_  == child); }
  template< typename Data, typename Compare >inline bool        RbTree<Data,Compare>::Node::hasRightChild  ( const Node* child ) const { return (right_ == child); }
  template< typename Data, typename Compare >inline const Data& RbTree<Data,Compare>::Node::getValue       () const { return value_; }
  template< typename Data, typename Compare >inline long        RbTree<Data,Compare>::Node::getFlags       () const { return flags_; }
  template< typename Data, typename Compare >inline void        RbTree<Data,Compare>::Node::setLeft        ( Node* node ) { left_  = node; if (node) node->setParent( this ); }
  template< typename Data, typename Compare >inline void        RbTree<Data,Compare>::Node::setRight       ( Node* node ) { right_ = node; if (node) node->setParent( this ); }
  template< typename Data, typename Compare >inline void        RbTree<Data,Compare>::Node::setParent      ( Node* node ) { parent_ = node; }
  template< typename Data, typename Compare >inline void        RbTree<Data,Compare>::Node::setFlags       ( long flags ) { flags_ |= flags; }
  template< typename Data, typename Compare >inline void        RbTree<Data,Compare>::Node::resetFlags     ( long flags ) { flags_ &= ~flags; }
  template< typename Data, typename Compare >inline void        RbTree<Data,Compare>::Node::setRed         () { resetFlags( RbTree::Black ); }
  template< typename Data, typename Compare >inline void        RbTree<Data,Compare>::Node::setBlack       () {   setFlags( RbTree::Black ); }
  

  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::Node* RbTree<Data,Compare>::Node::getLeft () const
  { return left_; }


  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::Node* RbTree<Data,Compare>::Node::getRight () const
  { return right_; }


  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::Node* RbTree<Data,Compare>::Node::getParent () const
  { return parent_; }


  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::Node* RbTree<Data,Compare>::Node::getGrandParent () const
  { return (parent_) ? parent_->parent_ : NULL; }
  

  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::Node* RbTree<Data,Compare>::Node::getSibling () const
  {
    if (not parent_) return NULL;
    return (parent_->getLeft() == this) ? parent_->getRight() : parent_->getLeft();
  }
  
  
  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::Node* RbTree<Data,Compare>::Node::getUncle () const
  {
    if (not getParent()) return NULL;
    return getParent()->getSibling();
  }
  
  
  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::Node* RbTree<Data,Compare>::Node::getMin () const
  {
    if (not getLeft()) return NULL;
  
    Node* left = getLeft();
    while ( left->getLeft() ) left = left->getLeft();
    return left;
  }
  
  
  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::Node* RbTree<Data,Compare>::Node::getMax () const
  {
    if (not getRight()) return NULL;
  
    Node* right = getRight();
    while ( right->getRight() ) right = right->getRight();
    return right;
  }
  
  
  template< typename Data, typename Compare >
  inline void  RbTree<Data,Compare>::Node::swap ( Node* other )
  {
    cdebug_log(0,0) << "Node::swap()" << endl;
    cdebug_log(0,0) << "| " << value_ << endl;
    cdebug_log(0,0) << "| " << other->value_ << endl;
    Data tmp = value_; value_ = other->value_; other->value_ = tmp;
    cdebug_log(0,0) << "| " << value_ << endl;
    cdebug_log(0,0) << "| " << other->value_ << endl;
  }
  
  
  template< typename Data, typename Compare >
  inline void  RbTree<Data,Compare>::Node::copyColor ( Node* node )
  { resetFlags( RbTree::Black ); setFlags( node->getFlags() & RbTree::Black ); }
  
  
  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::Node::updateEdge ( Node* oldChild, Node* newChild )
  {
    if (left_  == oldChild) { left_  = newChild; return; }
    if (right_ == oldChild) { right_ = newChild; }
  }

  
  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::Node::clear ()
  {
    if (left_ ) left_ ->clear();
    if (right_) right_->clear();
    delete this;
  }


  template< typename Data, typename Compare >
  std::string  RbTree<Data,Compare>::Node::_getString () const
  { return "<RbTree<>::Node " + getString(getValue()) + ">"; }

  
  template< typename Data, typename Compare >
  Record* RbTree<Data,Compare>::Node::_getRecord () const
  {
    Record* record = new Record( getString(this) );
    record->add(getSlot( "value_" , &value_  ));
    record->add(getSlot( "flags_" ,  flags_  ));
    record->add(getSlot( "parent_",  parent_ ));
    record->add(getSlot( "left_"  ,  left_   ));
    record->add(getSlot( "right_" ,  right_  ));
    return record;
  }
  
  
// -------------------------------------------------------------------
// Class  :  "Hurricane::RbTree::iterator".
  
  
  template< typename Data, typename Compare >
  inline RbTree<Data,Compare>::iterator::iterator ( const Node* node )
    : node_(node)
  { }
  
  
  template< typename Data, typename Compare > inline bool        RbTree<Data,Compare>::iterator::operator== ( const iterator& other ) const { return (node_ == other.node_); }
  template< typename Data, typename Compare > inline bool        RbTree<Data,Compare>::iterator::operator!= ( const iterator& other ) const { return (node_ != other.node_); }
  template< typename Data, typename Compare > inline const Data& RbTree<Data,Compare>::iterator::operator*  () const { return  node_->getValue(); }
  template< typename Data, typename Compare > inline bool        RbTree<Data,Compare>::iterator::isValid    () const { return (node_); }
  template< typename Data, typename Compare > inline void        RbTree<Data,Compare>::iterator::setNode    ( const Node* node ) { node_ = node; }


  template< typename Data, typename Compare >
  inline const typename RbTree<Data,Compare>::Node* RbTree<Data,Compare>::iterator::getNode () const
  { return node_; }
  

  template< typename Data, typename Compare >
  typename RbTree<Data,Compare>::iterator& RbTree<Data,Compare>::iterator::operator++ ()
  {
    if (node_) {
      const Node* right = node_->getRight();
      if (right) {
        const Node* minChild = right->getMin();
        if (minChild) node_ = minChild;
        else          node_ = right;
      } else {
        const Node* parent = node_->getParent();
        while ( parent and not parent->hasLeftChild(node_)) {
          node_  = parent;
          parent = parent->getParent();
        }
        node_ = parent;
      }
    }
  
    return *this;
  }
  
  
// -------------------------------------------------------------------
// Class  :  "Hurricane::RbTree::Elements".

  
  template< typename Data, typename Compare >
  inline RbTree<Data,Compare>::Elements::Locator::Locator ( const Locator &locator )
    : Hurricane::Locator<Data>()
    , iterator_(locator.iterator_)
  { }


  template< typename Data, typename Compare >
  RbTree<Data,Compare>::Elements::Locator::Locator ( const RbTree& tree )
    : Hurricane::Locator<Data>()
    , iterator_(tree.begin())
  { }


  template< typename Data, typename Compare >
  typename RbTree<Data,Compare>::Elements::Locator* RbTree<Data,Compare>::Elements::Locator::getClone () const
  { return new Locator(*this); }


  template< typename Data, typename Compare >
  Data RbTree<Data,Compare>::Elements::Locator::getElement () const
  { return (*iterator_); }


  template< typename Data, typename Compare >
  bool  RbTree<Data,Compare>::Elements::Locator::isValid () const
  { return iterator_.isValid(); }


  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::Elements::Locator::progress ()
  {
    if (isValid()) ++iterator_;
  }


  template< typename Data, typename Compare >
  std::string  RbTree<Data,Compare>::Elements::Locator::_getString () const
  {
    std::string s = "<" + _TName("Elements::Locator")
                        + ">";
    return s;
  }


  template< typename Data, typename Compare >
  inline RbTree<Data,Compare>::Elements::Elements ( const RbTree& tree )
    : Collection<Data>()
    , tree_           (tree)
  { }


  template< typename Data, typename Compare >
  inline RbTree<Data,Compare>::Elements::Elements ( const Elements& elements )
    : Collection<Data>()
    , tree_           (elements.tree_)
  { }


  template< typename Data, typename Compare >
  Collection<Data>* RbTree<Data,Compare>::Elements::getClone () const
  { return new Elements(*this); }


  template< typename Data, typename Compare >
  typename RbTree<Data,Compare>::Elements::Locator* RbTree<Data,Compare>::Elements::getLocator () const
  { return new Locator( tree_ ); }


  template< typename Data, typename Compare >
  std::string  RbTree<Data,Compare>::Elements::_getString () const
  {
    std::string s = "<" + _TName("Elements") + " "
                        + getString(tree_)
                        + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::RbTree".
  
  
  template< typename Data, typename Compare >
  RbTree<Data,Compare>::RbTree ()
    : count_  (0)
    , root_   (NULL)
    , compare_()
  { }
  
  
  template< typename Data, typename Compare >
  RbTree<Data,Compare>::~RbTree ()
  { }
  
  
  template< typename Data, typename Compare > inline bool    RbTree<Data,Compare>::empty () const { return not count_; }
  template< typename Data, typename Compare > inline size_t  RbTree<Data,Compare>::size  () const { return count_; }

  
  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::Node* RbTree<Data,Compare>::getRoot () const
  { return root_; }

  
  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::iterator  RbTree<Data,Compare>::end () const
  { return iterator( NULL ); }
  
  
  template< typename Data, typename Compare >
  inline typename RbTree<Data,Compare>::iterator  RbTree<Data,Compare>::begin () const
  { return iterator( (root_ and root_->getLeft()) ? root_->getMin() : root_ ); }
  

  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::postRotateLeft ( typename RbTree<Data,Compare>::Node* )
  { }
  

  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::postRotateRight ( typename RbTree<Data,Compare>::Node* )
  { }


  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::postInsert ( typename RbTree<Data,Compare>::Node* )
  { }
  

  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::postRemove ( typename RbTree<Data,Compare>::Node* )
  { }


  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::rotateLeft ( typename RbTree<Data,Compare>::Node* node )
  {
    cdebug_log(0,0) << "RbTree::rotateLeft() " << node->getValue() << std::endl;
    
    Node* rchild = node->getRight();
  
    rchild->setParent( node->getParent() );
    node  ->setParent( rchild );
    node  ->setRight ( rchild->getLeft() );
    rchild->setLeft  ( node );
  
    if (not rchild->getParent()) root_ = rchild;
    else rchild->getParent()->updateEdge( node, rchild );

    postRotateLeft( node );
  }
  
  
  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::rotateRight ( typename RbTree<Data,Compare>::Node* node )
  {
    cdebug_log(0,0) << "RbTree::rotateRight() " << node->getValue() << std::endl;
    
    Node* lchild = node->getLeft();
  
    lchild->setParent( node->getParent() );
    node  ->setParent( lchild );
    node  ->setLeft  ( lchild->getRight() );
    lchild->setRight ( node );
  
    if (not lchild->getParent()) root_ = lchild;
    else lchild->getParent()->updateEdge( node, lchild );

    postRotateRight( node );
  }
  
  
  template< typename Data, typename Compare >
  typename RbTree<Data,Compare>::iterator  RbTree<Data,Compare>::find ( const Data& value )
  {
    cdebug_log(0,1) << "RbTree::find() value:" << value << std::endl;
  
    Node* current = root_;
    while ( current ) {
      cdebug_log(0,0) << "| " << current->getValue() << std::endl;
  
      if (current->getValue() == value) {
        cdebug_log(0,-1) << "> Value found: " << current->getValue() << std::endl;
        return iterator(current);
      }
  
      if (compare_(value,current->getValue())) {
        current = current->getLeft ();
        cdebug_log(0,0) << "| Go left " << ((current) ? ::getString(current->getValue()) : "NULL")  << std::endl;
      }
      else {
        current = current->getRight();
        cdebug_log(0,0) << "| Go right " << ((current) ? ::getString(current->getValue()) : "NULL")  << std::endl;
      }
    }
  
    cdebug_log(0,-1) << "Value not found." << std::endl;
    return end();
  }
  
  
  template< typename Data, typename Compare >
  typename RbTree<Data,Compare>::iterator  RbTree<Data,Compare>::lower_bound ( const Data& value ) const
  {
    cdebug_log(0,1) << "RbTree::lower_bound() value:" << value << std::endl;
    if (not root_) { cdebug_tabw(0,-1); return end(); }
  
    Node* current = root_;
    Node* lbound  = NULL;
    while ( current ) {
      cdebug_log(0,0) << "| " << current <<std::endl;
  
      if (compare_(value,current->getValue())) { current = current->getLeft (); continue; }
      if (compare_(current->getValue(),value)) { current = current->getRight(); continue; }
  
      lbound  = current;
      current = current->getLeft();
    }
  
    cdebug_log(0,-1) << "lower_bound: " << lbound <<std::endl;
    return iterator( lbound );
  }
  
  
  template< typename Data, typename Compare >
  typename RbTree<Data,Compare>::iterator  RbTree<Data,Compare>::upper_bound ( const Data& value ) const
  {
    cdebug_log(0,1) << "RbTree::uppper_bound() value:" << value << std::endl;
    if (not root_) { cdebug_tabw(0,-1); return end(); }
  
    Node* ubound  = NULL;
    Node* current = root_;
    while ( current ) {
      cdebug_log(0,0) << "| " << current << std::endl;
  
      if (compare_(value,current->getValue())) { ubound = current; current = current->getLeft (); continue; }
      if (compare_(current->getValue(),value)) {                   current = current->getRight(); continue; }
  
      current = current->getRight();
    }
  
    cdebug_log(0,-1) << "upper_bound: " << ubound << std::endl;
    return iterator( ubound );
  }
  

  template< typename Data, typename Compare >
  typename RbTree<Data,Compare>::Elements  RbTree<Data,Compare>::getElements () const
  { return Elements( *this ); }
  
  
  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::remove ( const Data& value )
  {
    cdebug_log(0,1) << "RbTree::remove() value:" << value << std::endl;
  
    Node* rmNode = const_cast<Node*>( find( value ).getNode() );
    if (not rmNode) {
      cdebug_log(0,-1) << "No node of value=" << value << std::endl;
      return;
    }
  
    Node* rmLeaf = nullptr;
    if (rmNode->getLeft() and rmNode->getRight()) {
      rmLeaf = rmNode->getLeft();
      Node* rmMax = rmLeaf->getMax();
      if (rmMax) rmLeaf = rmMax;
      
      rmNode->swap( rmLeaf );
      std::swap( rmNode, rmLeaf );
    }

    removeRepair( rmNode, 0 );
    postRemove  ( rmNode );
  
    Node* parent = rmNode->getParent();
    Node* child  = (rmNode->getLeft()) ? rmNode->getLeft() : rmNode->getRight();
  
    if (parent) {
      if (parent->hasLeftChild(rmNode)) parent->setLeft ( child );
      else                              parent->setRight( child );
    } else {
      root_ = child;
      if (child) {
        child->setParent( NULL );
        child->setBlack ();
      }    
    }
  
    cdebug_log(0,0) << "delete " << rmNode->getValue() << std::endl;
    delete rmNode;
    
    --count_;
  
    cdebug_tabw(0,-1);
  }
  
  
  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::removeRepair ( typename RbTree<Data,Compare>::Node* rmNode, size_t depth )
  {
    cdebug_log(0,1) << "RbTree::removeRepair() rmNode:" << rmNode->getValue()
                    << " depth:" << depth << std::endl;

    if (not rmNode) {
      cdebug_tabw(0,-1);
      return ;
    }
  
    if (rmNode->isBlack()) {
      Node* parent = rmNode->getParent();
    // Case 1.
      if (not parent) {
        cdebug_log(0,0) << "Case 1: Do nothing." << std::endl;
        cdebug_tabw(0,-1);
        return;
      } else {
      // Case 2.
        Node* sibling = rmNode->getSibling();
  
        if (sibling->isRed()) {
          cdebug_log(0,0) << "Case 2" << std::endl;
  
          parent ->setRed();
          sibling->setBlack();
          if (parent->hasLeftChild(rmNode)) rotateLeft ( parent );
          else                              rotateRight( parent );
        }
  
      // Case 3.
        sibling = rmNode->getSibling();
        
        if (   parent->isBlack()
           and sibling->isBlack()
           and (not sibling->getLeft () or sibling->getLeft ()->isBlack())
           and (not sibling->getRight() or sibling->getRight()->isBlack()) ) {
          cdebug_log(0,0) << "Case 3" << std::endl;
  
          sibling->setRed();
          removeRepair( parent, depth+1 );
  
          cdebug_tabw(0,-1);
          return;
        }
  
      // Case 4.
        if (   parent->isRed()
           and sibling->isBlack()
           and (not sibling->getLeft () or sibling->getLeft ()->isBlack())
           and (not sibling->getRight() or sibling->getRight()->isBlack()) ) {
          cdebug_log(0,0) << "Case 4" << std::endl;
  
          sibling->setRed  ();
          parent ->setBlack();
  
          cdebug_tabw(0,-1);
          return;
        }
  
      // Case 5.
        if (sibling->isBlack()) {
          if (parent->hasLeftChild(rmNode)
             and (not sibling->getRight() or  sibling->getRight()->isBlack())
             and (    sibling->getLeft () and sibling->getLeft ()->isRed  ()) ) {
            cdebug_log(0,0) << "Case 5 (rmNode is *left* child)" << std::endl;
  
            sibling->setRed();
            sibling->getLeft()->setBlack();
            rotateRight( sibling );
          } else if (parent->hasRightChild(rmNode)
                    and (not sibling->getLeft () or  sibling->getLeft ()->isBlack())
                    and (    sibling->getRight() and sibling->getRight()->isRed  ()) ) {
            cdebug_log(0,0) << "Case 5 (rmNode is *right* child)" << std::endl;
  
            sibling->setRed();
            sibling->getRight()->setBlack();
            rotateLeft( sibling );
          }
          sibling = rmNode->getSibling();
        }
          
      // Case 6.
        sibling->copyColor( parent );
        parent->setBlack();
  
        if (parent->hasLeftChild(rmNode)) {
          cdebug_log(0,0) << "Case 6 (rmNode is *left* child)" << std::endl;
  
          sibling->getRight()->setBlack();
          rotateLeft( parent );
        } else {
          cdebug_log(0,0) << "Case 6 (rmNode is *right* child)" << std::endl;
  
          sibling->getLeft()->setBlack();
          rotateRight( parent );
        }
      }
    }
  
    cdebug_tabw(0,-1);
  }
  
  
  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::insert ( const Data& value )
  {
    cdebug_log(0,1) << "RbTree::insert() value:" << value << std::endl;
  
    Node* node = new Node( value, NULL );
    ++count_;
    cdebug_log(0,0) << "New node:" << node << std::endl;
  
    insertRecurse( node );
    postInsert   ( node );
    insertRepair ( node, 0 );
  
    cdebug_tabw(0,-1);
  }
  
  
  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::insertRecurse ( typename RbTree<Data,Compare>::Node* node )
  {
    cdebug_log(0,1) << "RbTree::insertRecurse() id:" << node << std::endl;
    
    if (not root_) { root_ = node; cdebug_tabw(0,-1); return; }
  
    Node* current = root_;
    while ( current ) {
      Node* child = NULL;
  
      if (compare_(node->getValue(),current->getValue())) {
        child = current->getLeft();
        if (not child) {
          cdebug_log(0,0) << "* Set as left child." << std::endl;
  
          current->setLeft( node );
          node->setParent( current );
          break;
        }
      } else {
        child = current->getRight();
        if (not child) {
          cdebug_log(0,0) << "* Set as right child." << std::endl;
  
          current->setRight( node );
          node->setParent( current );
          break;
        }
      }
  
      cdebug_log(0,0) << "> Down L/R child:" << child << std::endl;
      current = child;
    }
  
    cdebug_tabw(0,-1);
  }
  

  template< typename Data, typename Compare >
  void  RbTree<Data,Compare>::insertRepair ( typename RbTree<Data,Compare>::Node* node, size_t rdepth )
  {
    cdebug_log(0,1) << "RbTree::insertRepair() id:" << node << std::endl;
  
    ++rdepth;
    
  // Case 1.
    if (node == root_) {
      cdebug_log(0,-1) << "Case 1" << std::endl;
      node->setBlack();
      return;
    }
  
  // Case 2.
    if (node->getParent()->isBlack()) {
      cdebug_log(0,-1) << "Case 2" << std::endl;
      return;
    }
  
  // Case 3.
    Node* uncle = node->getUncle();
    if (uncle and uncle->isRed()) {
      cdebug_log(0,0) << "Case 3: uncle:" << uncle << std::endl;
  
      node->getParent()->setBlack();
      uncle->setBlack();
      Node* grandParent = node->getGrandParent();
      grandParent->setRed();
      insertRepair( grandParent, rdepth );
  
      cdebug_tabw(0,-1);
      return;
    }
  
  // Case 4, step 1.
    Node* parent      = node->getParent();
    Node* grandParent = node->getGrandParent();
  
    if (grandParent->getLeft() and (node == grandParent->getLeft()->getRight()) ) {
      cdebug_log(0,0) << "Case 4.1: right of left" << std::endl;
      rotateLeft( parent );
      node = node->getLeft();
    } else if (grandParent->getRight() and (node == grandParent->getRight()->getLeft()) ) {
      cdebug_log(0,0) << "Case 4.1: left of right" << std::endl;
      rotateRight( parent );
      node = node->getRight();
    }
  
  // Case 4, step 2.
    cdebug_log(0,0) << "Case 4.2" << std::endl;
    parent      = node->getParent();
    grandParent = node->getGrandParent();
  
    if (node == parent->getLeft()) rotateRight( grandParent );
    else                           rotateLeft ( grandParent );
  
    parent->setBlack();
    grandParent->setRed();
  
    cdebug_tabw(0,-1);
  }
  
  
  template< typename Data, typename Compare >
  inline void  RbTree<Data,Compare>::clear ()
  { if (root_) root_->clear(); root_ = NULL; count_ = 0; }


  template< typename Data, typename Compare >
  std::string  RbTree<Data,Compare>::_getTypeName () const
  { return "RbTree<>"; }


  template< typename Data, typename Compare >
  std::string  RbTree<Data,Compare>::_getString () const
  { return "<" + _getTypeName() + " " + getString(size()) + ">"; }

  
  template< typename Data, typename Compare >
  Record* RbTree<Data,Compare>::_getRecord () const
  {
    cerr << "RbTree::_getRecord() root_:" << root_ << endl;
    
    Record* record = new Record( getString(this) );
    record->add(getSlot             ( "count_", count_ ));
    record->add(getSlot<const Node*>( "root_" , root_  ));

    cerr << "Root Record: 0x" << dynamic_cast< SlotTemplate<const Node*>* >(record->getSlot(1))->getDataRecord() << endl;
    return record;
  }
  
  
// -------------------------------------------------------------------
// Class  :  "RbTreeToDot".


  template< typename Data, typename Compare, template<typename,typename> class RbTree >
  class RbTreeToDot {
    public:
                     RbTreeToDot ( const RbTree<Data,Compare>* tree );
    public:   
             void    write       ( std::string   ) const;
             void    write       ( std::ostream& ) const;
    private:  
             void    setupIds    ( typename RbTree<Data,Compare>::Node* );
             void    toDot       ( std::ostream&, typename RbTree<Data,Compare>::Node* ) const;
      inline size_t  getId       ( typename RbTree<Data,Compare>::Node* ) const;
    private:
      const RbTree<Data,Compare>*                       tree_;
      map<typename RbTree<Data,Compare>::Node*,size_t>  nodeIds_;
  };


  template< typename Data, typename Compare, template<typename,typename> class RbTree >
  inline size_t  RbTreeToDot<Data,Compare,RbTree>::getId ( typename RbTree<Data,Compare>::Node* r ) const
  {
    typename map<typename RbTree<Data,Compare>::Node*,size_t>::const_iterator it = nodeIds_.find( r );
    return (it != nodeIds_.end()) ? (*it).second : 0;
  }


  template< typename Data, typename Compare, template<typename,typename> class RbTree >
  RbTreeToDot<Data,Compare,RbTree>::RbTreeToDot ( const RbTree<Data,Compare>* tree )
    : tree_   ( tree )
    , nodeIds_()
  { setupIds( tree_->getRoot() ); }


  template< typename Data, typename Compare, template<typename,typename> class RbTree >
  void  RbTreeToDot<Data,Compare,RbTree>::setupIds ( typename RbTree<Data,Compare>::Node* node )
  {
    if (not node) return;

    setupIds( node->getLeft() );
    nodeIds_.insert( make_pair( node, nodeIds_.size() ) );
    setupIds( node->getRight() );
  }

  
  template< typename Data, typename Compare, template<typename,typename> class RbTree >
  void  RbTreeToDot<Data,Compare,RbTree>::write ( std::string path ) const
  {
    cdebug_log(0,0) << "RbTreeToDot::write() path=\"" << path << "\"" << endl;
  
    std::ofstream dotstream ( path );
    write( dotstream );
    dotstream.close();
  }


  template< typename Data, typename Compare, template<typename,typename> class RbTree >
  void  RbTreeToDot<Data,Compare,RbTree>::write ( std::ostream& o ) const
  {
    o << "digraph RbTree {\n";
    o << "  ratio=\"1.0\";\n";
    toDot( o, tree_->getRoot() );
    o << "}";
  }

  
  template< typename Data, typename Compare, template<typename,typename> class RbTree >
  void  RbTreeToDot<Data,Compare,RbTree>::toDot ( ostream& o, typename RbTree<Data,Compare>::Node* node ) const
  {
    if (not node) return;
    
    string svalue = ::getString( node->getValue() );
    o << "  id_" << getId(node) << "  "
      << "[label=\"id:" << getId(node) << "\\n" << split( svalue )
      << "\""
      << ",color="     << (node->isRed() ? "red" : "black")
      << ",fontcolor=" << (node->isRed() ? "red" : "black")
      << "];\n";
  
    if (node->getParent()) {
      o << "  id_" << getId(node) << " -> id_" << getId(node->getParent()) << "  "
        << "[label=\" p \""
        << ",color="     << (node->getParent()->isRed() ? "red" : "black")
        << ",fontcolor=" << (node->getParent()->isRed() ? "red" : "black")
        << ",style=dotted"
        << "];\n";
    }
  
    if (node->getLeft()) {
      o << "  id_" << getId(node) << " -> id_" << getId(node->getLeft()) << "  "
        << "[label=\" L \""
        << ",color="     << (node->isRed() ? "red" : "black")
        << ",fontcolor=" << (node->isRed() ? "red" : "black")
        << "];\n";
    }
    if (node->getRight()) {
      o << "  id_" << getId(node) << " -> id_" << getId(node->getRight()) << "  "
        << "[label=\" R \",style=bold"
        << ",color="     << (node->isRed() ? "red" : "black")
        << ",fontcolor=" << (node->isRed() ? "red" : "black")
        << "];\n";
    }
  
    if (node->getLeft ()) toDot( o, node->getLeft () );
    if (node->getRight()) toDot( o, node->getRight() );
  }

  
  template< typename Data, typename Compare >
  inline void  RbTree<Data,Compare>::write ( std::string path ) const
  { RbTreeToDot< Data, Compare, RbTree >(this).write( path ); }


}  // Hurricane namespace.
