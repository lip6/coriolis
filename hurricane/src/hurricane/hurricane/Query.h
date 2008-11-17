
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Query.h"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __QUERY_H__
#define  __QUERY_H__


#include  <vector>

#include  "hurricane/Commons.h"
#include  "hurricane/Box.h"
#include  "hurricane/Transformation.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Instance.h"


namespace Hurricane {


  class BasicLayer;
  class Go;
  class QueryStack;


// -------------------------------------------------------------------
// Slave Class  :  "QueryState".


  class QueryState {
    private:
      inline                        QueryState ( Locator<Instance*>*   locator );
      inline                        QueryState ( Locator<Instance*>*   locator
                                               , const Box&            area
                                               , const Transformation& transformation
                                               );
                                    QueryState ( const QueryState& );
              QueryState&           operator=  ( const QueryState& );
      inline                       ~QueryState ();
    private:
              Locator<Instance*>*   _locator;
              Box                   _area;
              Transformation        _transformation;

      friend  class QueryStack;
  };


// QueryState Inline Functions.


  inline QueryState::QueryState ( Locator<Instance*>* locator )
    : _locator(locator)
    , _area()
    , _transformation()
  { }


  inline QueryState::QueryState ( Locator<Instance*>*   locator
                                , const Box&            area
                                , const Transformation& transformation
                                )
    : _locator(locator)
    , _area(area)
    , _transformation(transformation)
  { }


  inline QueryState::~QueryState ()
  {
    if ( _locator ) delete _locator;
  }


// -------------------------------------------------------------------
// Class  :  "QueryStack".


  class QueryStack : public vector<QueryState*> {
    public:
    // Constructor & destructor.
                                    QueryStack           ();
                                   ~QueryStack           ();
    // Accessors.
      inline  Cell*                 getTopCell           ();
      inline  const Box&            getTopArea           () const;
      inline  const Transformation& getTopTransformation () const;
      inline  unsigned int          getStartLevel        () const;
      inline  unsigned int          getStopLevel         () const;
      inline  Cell*                 getMasterCell        ();
      inline  Instance*             getInstance          ();
      inline  const Box&            getArea              () const;
      inline  const Transformation& getTransformation    () const;
    //inline  const Tabulation&     getTab               () const;
    // Modifiers.
      inline  void                  setTopCell           ( Cell*                 cell );
      inline  void                  setTopArea           ( const Box&            area );
      inline  void                  setTopTransformation ( const Transformation& transformation );
      inline  void                  setStartLevel        ( unsigned int          level );
      inline  void                  setStopLevel         ( unsigned int          level );
      inline  void                  init                 ();
      inline  void                  updateTransformation ();
      inline  bool                  levelDown            ();
      inline  void                  levelUp              ();
      inline  void                  levelProgress        ();
      inline  bool                  levelCompleted       ();
      inline  void                  progress             ( bool init=false );

    protected:
    // Internal: Attributes.
    //        Tabulation            _tab;
              Cell*                 _topCell;
              Box                   _topArea;
              Transformation        _topTransformation;
              unsigned int          _startLevel;
              unsigned int          _stopLevel;

    private:
    // Internal: Constructors.
                                    QueryStack           ( const QueryStack& );
              QueryStack&           operator=            ( const QueryStack& );
  };


// QueryStack Inline Functions.


  inline  Cell*                 QueryStack::getTopCell           () { return _topCell; }
  inline  const Box&            QueryStack::getTopArea           () const { return _topArea; }
  inline  const Transformation& QueryStack::getTopTransformation () const { return _topTransformation; }
  inline  unsigned int          QueryStack::getStartLevel        () const { return _startLevel; }
  inline  unsigned int          QueryStack::getStopLevel         () const { return _stopLevel; }
  inline  const Box&            QueryStack::getArea              () const { return back()->_area; }
  inline  const Transformation& QueryStack::getTransformation    () const { return back()->_transformation; }
//inline  const Tabulation&     QueryStack::getTab               () const { return _tab; }


  inline Instance* QueryStack::getInstance ()
  {
    if ( levelCompleted() ) return NULL;
    return back()->_locator->getElement();
  }


  inline Cell* QueryStack::getMasterCell ()
  {
    if ( size() == 1 ) return _topCell;
    if ( !getInstance() ) return NULL;
    return getInstance()->getMasterCell();
  }


  inline  void  QueryStack::setTopCell           ( Cell*                 cell )           { _topCell = cell; }
  inline  void  QueryStack::setTopArea           ( const Box&            area )           { _topArea = area; }
  inline  void  QueryStack::setTopTransformation ( const Transformation& transformation ) { _topTransformation = transformation; }
  inline  void  QueryStack::setStartLevel        ( unsigned int          level )          { _startLevel = level; }
  inline  void  QueryStack::setStopLevel         ( unsigned int          level )          { _stopLevel = level; }


  inline  void  QueryStack::init ()
  {
    while ( !empty() ) levelUp();

    push_back ( new QueryState(NULL,_topArea,_topTransformation) );
  //_tab++;

    progress ( true );
  }


  inline  void  QueryStack::updateTransformation ()
  {
    QueryState* child    = *(rbegin()  );
    QueryState* parent   = *(rbegin()+1);
    Instance*   instance = child->_locator->getElement();

    child->_area           = parent->_area;
    child->_transformation = instance->getTransformation ();

    instance->getTransformation().getInvert().applyOn ( child->_area );
    parent->_transformation.applyOn ( child->_transformation );
  }


  inline  bool  QueryStack::levelDown ()
  {
    if ( size() > _stopLevel ) return false;

    Locator<Instance*>* locator = getMasterCell()->getInstancesUnder(getArea()).getLocator();

    if ( locator->isValid() ) {
      push_back ( new QueryState ( locator ) );

      updateTransformation ();
    //_tab++;

      return true;
    } else
      delete locator;

    return false;
  }


  inline  void  QueryStack::levelUp ()
  {
    delete back ();
    pop_back ();
  //_tab--;
  }


  inline  bool  QueryStack::levelCompleted ()
  {
    if ( !back()->_locator || !back()->_locator->isValid () ) return true;
    return false;
  }


  inline  void  QueryStack::levelProgress ()
  {
    if ( levelCompleted() ) return;

    back()->_locator->progress ();
    if ( !back()->_locator->isValid() ) return;

    updateTransformation ();
  }


  inline  void  QueryStack::progress ( bool init )
  {
    if ( !init ) levelProgress ();
    else {
      if ( !levelDown() && ( size() > _startLevel ) )
        return;
    }

    while ( !empty() ) {
      if ( levelCompleted() ) {
        levelUp ();
      } else {
        if ( levelDown() ) continue;
      }

      if ( size() > _startLevel ) return;
      if ( empty() ) break;
      levelProgress ();
    }
  }


// -------------------------------------------------------------------
// Class  :  "Query".


  class Query {
    public:
    // Types.
      enum  QueryFilter { DoMasterCells   = 1
                        , DoTerminalCells = 2
                        , DoComponents    = 4
                        , DoExtensionGos  = 8
                        , DoAll           =   DoMasterCells
                                            | DoTerminalCells
                                            | DoComponents
                                            | DoExtensionGos
                        };
    public:
    // Constructors & Destructors.
                                    Query                  ();
      virtual                      ~Query                  ();
    // Accessors.
      inline  unsigned int          getStartLevel          () const;
      inline  unsigned int          getStopLevel           () const;
      inline  size_t                getDepth               () const;
      inline  const Transformation& getTransformation      () const;
      inline  const Box&            getArea                () const;
      inline  const BasicLayer*     getBasicLayer          () const;
      inline  Cell*                 getMasterCell          ();
      inline  Instance*             getInstance            ();
    //inline  const Tabulation&     getTab                 () const;
      virtual bool                  hasGoCallback          () const;
      virtual bool                  hasExtensionGoCallback () const;
      virtual bool                  hasMasterCellCallback  () const;
      virtual void                  goCallback             ( Go* go ) = 0;
      virtual void                  extensionGoCallback    ( Go* go ) = 0;
      virtual void                  masterCellCallback     () = 0;
    // Modifiers.
              void                  setQuery               ( Cell*                 cell
                                                           , const Box&            area
                                                           , const Transformation& transformation
                                                           , const BasicLayer*     basicLayer
                                                           , ExtensionSlice::Mask  extensionMask
                                                           , unsigned int          filter
                                                           );
      inline  void                  setCell                ( Cell*                 cell );
      inline  void                  setArea                ( const Box&            area );
      inline  void                  setTransformation      ( const Transformation& transformation );
      inline  void                  setBasicLayer          ( const BasicLayer*     basicLayer );
      inline  void                  setExtensionMask       ( ExtensionSlice::Mask  mode );
      inline  void                  setFilter              ( unsigned int          mode );
      inline  void                  setStartLevel          ( unsigned int          level );
      inline  void                  setStopLevel           ( unsigned int          level );
              void                  doQuery                ();

    protected:
    // Internal: Attributes.
              QueryStack            _stack;
              const BasicLayer*     _basicLayer;
              ExtensionSlice::Mask  _extensionMask;
              unsigned int          _filter;
  };


// Query Inline Functions.

  inline  void  Query::setCell           ( Cell*                 cell )           { _stack.setTopCell(cell); }
  inline  void  Query::setArea           ( const Box&            area )           { _stack.setTopArea(area); }
  inline  void  Query::setTransformation ( const Transformation& transformation ) { _stack.setTopTransformation(transformation); }
  inline  void  Query::setBasicLayer     ( const BasicLayer*     basicLayer )     { _basicLayer = basicLayer; }
  inline  void  Query::setFilter         ( unsigned int          filter )         { _filter = filter; }
  inline  void  Query::setExtensionMask  ( ExtensionSlice::Mask  mask )           { _extensionMask = mask; }
  inline  void  Query::setStartLevel     ( unsigned int          level )          { _stack.setStartLevel(level); }
  inline  void  Query::setStopLevel      ( unsigned int          level )          { _stack.setStopLevel(level); }

  inline  unsigned int          Query::getStartLevel      () const { return _stack.getStartLevel(); }
  inline  unsigned int          Query::getStopLevel       () const { return _stack.getStopLevel(); }
  inline  size_t                Query::getDepth           () const { return _stack.size(); }
  inline  const Box&            Query::getArea            () const { return _stack.getArea(); }
  inline  const Transformation& Query::getTransformation  () const { return _stack.getTransformation(); }
  inline  const BasicLayer*     Query::getBasicLayer      () const { return _basicLayer; }
  inline  Cell*                 Query::getMasterCell      () { return _stack.getMasterCell(); }
  inline  Instance*             Query::getInstance        () { return _stack.getInstance(); }
//inline const Tabulation&      Query::getTab             () const { return _stack.getTab(); }
  


} // End of Hurricane namespace.


#endif // __QUERY_H__
