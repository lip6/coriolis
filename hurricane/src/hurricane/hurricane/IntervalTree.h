// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2018-2023, All Rights Reserved
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
// |  C++ Header  :  "./hurricane/IntervalTree.h"                    |
// +-----------------------------------------------------------------+
//
// References:
//    1. Wikipedia, https://en.wikipedia.org/wiki/Red-black_tree
//    2. Introduction to Algorithms, Cormen & Al,
//       Third edition, MIT press, 2011, p. 348.


#pragma  once
#include "hurricane/Interval.h"
#include "hurricane/RbTree.h"

#include <algorithm>

namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::IntervalData".


  template< typename Data >
  class IntervalData : public Interval {
    public:
      inline             IntervalData     ();
      inline             IntervalData     ( const Data&, DbU::Unit vmin, DbU::Unit vmax );
      inline Data&       getData          () const;
      inline DbU::Unit   getChildsVMax    () const;
      inline DbU::Unit   updateChildsVMax ( DbU::Unit lvmax, DbU::Unit rvmax );
             string      _getString       () const;
             Record*     _getRecord       () const;
    private:
      DbU::Unit  childsVMax_;
      Data       data_;
  };

  
  template< typename Data >
  inline  IntervalData<Data>::IntervalData ()
    : Interval(1,-1) 
    , childsVMax_(0)
    , data_      ()
  { }

  
  template< typename Data >
  inline  IntervalData<Data>::IntervalData (  const Data& data, DbU::Unit vmin, DbU::Unit vmax )
    : Interval(vmin,vmax) 
    , childsVMax_(vmax)
    , data_      (data)
  { }


  template< typename Data >
  inline Data& IntervalData<Data>::getData () const
  { return const_cast< IntervalData<Data>* >( this )->data_; }


  template< typename Data >
  inline DbU::Unit  IntervalData<Data>::getChildsVMax () const
  { return childsVMax_; }


  template< typename Data >
  inline DbU::Unit  IntervalData<Data>::updateChildsVMax ( DbU::Unit lvmax, DbU::Unit rvmax )
  { childsVMax_ = std::max( getVMax(), std::max( lvmax, rvmax ) ); return childsVMax_; }


  template< typename Data >
  std::string  IntervalData<Data>::_getString () const
  {
    string s = Interval::_getString();
    s.insert( s.size(), " max:"+DbU::getValueString(childsVMax_) );
    s.insert( s.size(), " v:"+getString(data_) );
    return s;
  }

  
  template< typename Data >
  Record* IntervalData<Data>::_getRecord () const
  {
    Record* record = Interval::_getRecord();
    if (record) {
      record->add(DbU::getValueSlot("childsVMax_", &childsVMax_));
      record->add(getSlot          ("data_"      , &data_      ));
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::IntervalTree".

  template< typename Data >
  class IntervalTree : public RbTree< IntervalData<Data>, Interval::CompareByMinMax > {
    public:
      typedef  RbTree< IntervalData<Data>, Interval::CompareByMinMax >  Super;
    public:
      class overlap_iterator : public Super::iterator {
        public:
          friend class IntervalTree;
        private:
          inline                    overlap_iterator ( const typename Super::Node*, const Interval& );
        public:              
          virtual overlap_iterator& operator++       ();
        private:
          Interval  overlap_;
      };
    public:
      class OverlapElements : public Collection< IntervalData<Data> > {
        public:
        // Sub-Class: Locator.
          class Locator : public Hurricane::Locator< IntervalData<Data> > {
            public:
                                          Locator    ( const IntervalTree&, const Interval& span );
              inline                      Locator    ( const Locator& );
              virtual IntervalData<Data>  getElement () const;
              virtual Locator*            getClone   () const;
              virtual bool                isValid    () const;
              virtual void                progress   ();
              virtual string              _getString () const;
            protected:
              overlap_iterator  iterator_;
          };
        public:
        // Class Elements.
          inline                                    OverlapElements   ( const IntervalTree&, const Interval& );
          inline                                    OverlapElements   ( const OverlapElements& );
          virtual Collection< IntervalData<Data> >* getClone          () const;
          virtual Locator*                          getLocator        () const;
          virtual string                            _getString        () const;
        protected:
          const IntervalTree& tree_;
          const Interval      span_;
      };
    public:
      virtual void              postRotateLeft   ( typename Super::Node* );
      virtual void              postRotateRight  ( typename Super::Node* );
      virtual void              postInsert       ( typename Super::Node* );
      virtual void              postRemove       ( typename Super::Node* );
              size_t            getThickness     () const;
              overlap_iterator  beginOverlaps    ( const Interval& ) const;
      inline  OverlapElements   getOverlaps      ( const Interval& ) const;
    private:
      inline  void              updateChildsVMax ( typename Super::Node* );
  };


  template< typename Data >
  IntervalTree<Data>::overlap_iterator::overlap_iterator ( const typename Super::Node* node, const Interval& overlap )
    : Super::iterator(node)
    , overlap_(overlap)
  { }


  template< typename Data >
  typename IntervalTree<Data>::overlap_iterator& IntervalTree<Data>::overlap_iterator::operator++ ()
  {
    while ( true ) {
      Super::iterator::operator++();
      if (not this->isValid()) break;

      cdebug_log(0,0) << "IntervalTree::overlap_iterator::operator++() "
                      << ::getString(this->getNode()) << std::endl;

      if (this->getNode()->getValue().intersect(overlap_,true)) break;
      cdebug_log(0,0) << "NO intersections" << endl;
      if (overlap_.inferior(this->getNode()->getValue(),true)) {
        cdebug_log(0,0) << "Node is inferior, stop here." << endl;
        this->setNode( NULL );
        break;
      }
    }
    return *this;
  }
  
  
// -------------------------------------------------------------------
// Class  :  "Hurricane::IntervalTree::OverlapOverlapElements" (implementation)

  
  template< typename Data >
  inline IntervalTree<Data>::OverlapElements::Locator::Locator ( const Locator &locator )
    : Hurricane::Locator< IntervalData<Data> >()
    , iterator_(locator.iterator_)
  { }


  template< typename Data >
  IntervalTree<Data>::OverlapElements::Locator::Locator ( const IntervalTree<Data>& tree, const Interval& span )
    : Hurricane::Locator< IntervalData<Data> >()
    , iterator_(tree.beginOverlaps(span))
  { }


  template< typename Data >
  typename IntervalTree<Data>::OverlapElements::Locator* IntervalTree<Data>::OverlapElements::Locator::getClone () const
  { return new Locator(*this); }


  template< typename Data >
  IntervalData<Data>  IntervalTree<Data>::OverlapElements::Locator::getElement () const
  { return (*iterator_); }


  template< typename Data >
  bool  IntervalTree<Data>::OverlapElements::Locator::isValid () const
  { return iterator_.isValid(); }


  template< typename Data >
  void  IntervalTree<Data>::OverlapElements::Locator::progress ()
  {
    if (isValid()) ++iterator_;
  }


  template< typename Data >
  std::string  IntervalTree<Data>::OverlapElements::Locator::_getString () const
  {
    std::string s = "<" + _TName("OverlapElements::Locator")
                        + ">";
    return s;
  }


  template< typename Data >
  inline IntervalTree<Data>::OverlapElements::OverlapElements ( const IntervalTree& tree, const Interval& span )
    : Collection< IntervalData<Data> >()
    , tree_(tree)
    , span_(span)
  { }


  template< typename Data >
  inline IntervalTree<Data>::OverlapElements::OverlapElements ( const OverlapElements& elements )
    : Collection< IntervalData<Data> >()
    , tree_(elements.tree_)
    , span_(elements.span_)
  { }


  template< typename Data >
  Collection< IntervalData<Data> >* IntervalTree<Data>::OverlapElements::getClone () const
  { return new OverlapElements(*this); }


  template< typename Data >
  typename IntervalTree<Data>::OverlapElements::Locator* IntervalTree<Data>::OverlapElements::getLocator () const
  { return new Locator( tree_, span_ ); }


  template< typename Data >
  std::string  IntervalTree<Data>::OverlapElements::_getString () const
  {
    std::string s = "<" + _TName("OverlapElements") + " "
                        + getString(tree_)
                        + ">";
    return s;
  }
  
  
// -------------------------------------------------------------------
// Class  :  "Hurricane::IntervalTree" (implementation).


  template< typename Data >
  inline void  IntervalTree<Data>::updateChildsVMax ( typename Super::Node* node )
  {
    DbU::Unit lchildVMax = (node->getLeft ()) ? node->getLeft ()->getValue().getChildsVMax() : node->getValue().getVMax();
    DbU::Unit rchildVMax = (node->getRight()) ? node->getRight()->getValue().getChildsVMax() : node->getValue().getVMax();

    const_cast< IntervalData<Data>& >( node->getValue() ).updateChildsVMax( lchildVMax, rchildVMax );
  }


  template< typename Data >
  void  IntervalTree<Data>::postRotateLeft ( typename Super::Node* node )
  {
    updateChildsVMax( node );
    if (node->getParent()) updateChildsVMax( node->getParent() );
  }

  
  template< typename Data >
  void  IntervalTree<Data>::postRotateRight ( typename Super::Node* node )
  {
    updateChildsVMax( node );
    if (node->getParent()) updateChildsVMax( node->getParent() );
  }

  
  template< typename Data >
  void  IntervalTree<Data>::postInsert ( typename Super::Node* node )
  {
    cdebug_log(0,1) << "IntervalTree::postInsert() " << node << std::endl;

    while ( node ) {
      cdebug_log(0,0) << "| " << node << std::endl;

      updateChildsVMax( node );
      node = node->getParent();
    }

    cdebug_tabw(0,-1);
  }

  
  template< typename Data >
  void  IntervalTree<Data>::postRemove ( typename Super::Node* node )
  {
    typename Super::Node* parent = node->getParent();
    if (parent) {
      typename Super::Node* child = NULL;

      if (parent->hasLeftChild(node)) child = parent->getRight();
      else                            child = parent->getLeft ();

      DbU::Unit childVMax = (child) ? child->getValue().getChildsVMax() : parent->getValue().getVMax();
      const_cast< IntervalData<Data>& >( parent->getValue() ).updateChildsVMax( childVMax, childVMax );

      postInsert( parent->getParent() );
    }
  }


  template< typename Data >
  size_t  IntervalTree<Data>::getThickness () const
  {
    cdebug_log(0,0) << "IntervalTree::getThickness() " << std::endl;

    vector<DbU::Unit> intervalMaxes;
    for ( const Interval& interval : *this ) {
      intervalMaxes.push_back( interval.getVMax() );
    }
    std::sort( intervalMaxes.begin(), intervalMaxes.end() );

    size_t maxThickness = 0;
    size_t curThickness = 0;
    size_t iMax         = 0;
    for ( const Interval& interval : *this ) {
      while ( (iMax < intervalMaxes.size()) and (intervalMaxes[iMax] <= interval.getVMin()) ) {
        --curThickness;
        cdebug_log(0,0) << "| end:" << intervalMaxes[iMax] << " thickness:" << curThickness << endl; 
        ++iMax;
      }

      maxThickness = std::max( maxThickness, ++curThickness );
      cdebug_log(0,0) << "| begin:" << interval << " thickness:" << curThickness << endl; 
    }

    return maxThickness;
  }
  

  template< typename Data >
  typename IntervalTree<Data>::overlap_iterator  IntervalTree<Data>::beginOverlaps ( const Interval& overlap ) const
  {
    cdebug_log(0,0) << "IntervalTree::beginOverlaps() " << overlap << std::endl;

    const typename Super::Node* current  = this->getRoot();
    const typename Super::Node* leftMost = NULL;
    while ( current ) {
      cdebug_log(0,0) << "| " << ::getString(current) << endl; 

      if (current->getValue().intersect(overlap)) leftMost = current;
      if (    current->getLeft()
         and (overlap.getVMin() < current->getLeft()->getValue().getChildsVMax()) )
        current = current->getLeft();
      else
        current = current->getRight();
    }
    return overlap_iterator( leftMost, overlap );
  }
  

  template< typename Data >
  typename IntervalTree<Data>::OverlapElements  IntervalTree<Data>::getOverlaps ( const Interval& overlap ) const
  {
    cdebug_log(0,0) << "IntervalTree::getOverlaps() " << overlap << std::endl;
    return OverlapElements( *this, overlap );
  }



}  // HUrricane namespace.
