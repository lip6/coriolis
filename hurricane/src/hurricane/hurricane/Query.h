// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Query.h"                           |
// +-----------------------------------------------------------------+


#pragma  once
#include <vector>
#include <iomanip>
#include "hurricane/Commons.h"
#include "hurricane/Box.h"
#include "hurricane/Transformation.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"


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
                                               , const Path&           path
                                               );
                                    QueryState ( const QueryState& );
              QueryState&           operator=  ( const QueryState& );
      inline                       ~QueryState ();
    private:
              Locator<Instance*>*   _locator;
              Box                   _area;
              Transformation        _transformation;
              Path                  _path;

      friend  class QueryStack;
  };


// QueryState Inline Functions.


  inline QueryState::QueryState ( Locator<Instance*>* locator )
    : _locator       (locator)
    , _area          ()
    , _transformation()
    , _path          ()
  { }


  inline QueryState::QueryState ( Locator<Instance*>*   locator
                                , const Box&            area
                                , const Transformation& transformation
                                , const Path&           path
                                )
    : _locator       (locator)
    , _area          (area)
    , _transformation(transformation)
    , _path          (path)
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
      inline  Cell::Flags           getStopCellFlags     () const;
      inline  Cell*                 getMasterCell        ();
      inline  Instance*             getInstance          ();
      inline  const Box&            getArea              () const;
      inline  DbU::Unit             getThreshold         () const;
      inline  const Transformation& getTransformation    () const;
      inline  const Path&           getPath              () const;
    //inline  const Tabulation&     getTab               () const;
    // Modifiers.
      inline  void                  setTopCell           ( Cell*                 cell );
      inline  void                  setTopArea           ( const Box&            area );
      inline  void                  setTopTransformation ( const Transformation& transformation );
      inline  void                  setThreshold         ( DbU::Unit             threshold );
      inline  void                  setStartLevel        ( unsigned int          level );
      inline  void                  setStopLevel         ( unsigned int          level );
      inline  void                  setStopCellFlags     ( Cell::Flags );
      inline  void                  init                 ();
      inline  void                  updateTransformation ();
      inline  bool                  levelDown            ();
      inline  void                  levelUp              ();
      inline  void                  levelProgress        ();
      inline  bool                  levelCompleted       ();
      inline  void                  progress             ( bool init=false );
      inline  size_t                getInstanceCount     () const;

    protected:
    // Internal: Attributes.
    //        Tabulation            _tab;
              Cell*                 _topCell;
              Box                   _topArea;
              DbU::Unit             _threshold;
              Transformation        _topTransformation;
              unsigned int          _startLevel;
              unsigned int          _stopLevel;
              Cell::Flags           _stopCellFlags;
              size_t                _instanceCount;

    private:
    // Internal: Constructors.
                                    QueryStack           ( const QueryStack& );
              QueryStack&           operator=            ( const QueryStack& );
  };


// QueryStack Inline Functions.


  inline  Cell*                 QueryStack::getTopCell           () { return _topCell; }
  inline  const Box&            QueryStack::getTopArea           () const { return _topArea; }
  inline  const Transformation& QueryStack::getTopTransformation () const { return _topTransformation; }
  inline  DbU::Unit             QueryStack::getThreshold         () const { return _threshold; }
  inline  unsigned int          QueryStack::getStartLevel        () const { return _startLevel; }
  inline  unsigned int          QueryStack::getStopLevel         () const { return _stopLevel; }
  inline  Cell::Flags           QueryStack::getStopCellFlags     () const { return _stopCellFlags; }
  inline  const Box&            QueryStack::getArea              () const { return back()->_area; }
  inline  const Transformation& QueryStack::getTransformation    () const { return back()->_transformation; }
  inline  const Path&           QueryStack::getPath              () const { return back()->_path; }
//inline  const Tabulation&     QueryStack::getTab               () const { return _tab; }
  inline  size_t                QueryStack::getInstanceCount     () const { return _instanceCount; }


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
  inline  void  QueryStack::setThreshold         ( DbU::Unit             threshold )      { _threshold = threshold; }
  inline  void  QueryStack::setStartLevel        ( unsigned int          level )          { _startLevel = level; }
  inline  void  QueryStack::setStopLevel         ( unsigned int          level )          { _stopLevel = level; }
  inline  void  QueryStack::setStopCellFlags     ( Cell::Flags           flags )          { _stopCellFlags = flags; }


  inline  void  QueryStack::init ()
  {
    _instanceCount = 0;
    while (not empty()) levelUp();

    push_back( new QueryState(NULL,_topArea,_topTransformation,Path()) );
  //_tab++;

    progress( true );
  }


  inline  void  QueryStack::updateTransformation ()
  {
    QueryState* child    = *(rbegin()  );
    QueryState* parent   = *(rbegin()+1);
    Instance*   instance = child->_locator->getElement();

  //cerr << "Processing " << instance << endl;

    child->_area           = parent->_area;
    child->_transformation = instance->getTransformation ();

    instance->getTransformation().getInvert().applyOn ( child->_area );
    parent->_transformation.applyOn ( child->_transformation );

  //child->_path = Path ( Path(parent->_path,instance->getCell()->getShuntedPath()) , instance );
    child->_path = Path ( parent->_path, instance );
  //cerr << "QueryStack::updateTransformation() " << child->_path << endl;
  }


  inline  bool  QueryStack::levelDown ()
  {
    if (size() > _stopLevel) return false;
    if (getMasterCell()->getFlags().isset(_stopCellFlags)) return false;

  //cerr << "QueryStack::levelDown(): t:" << DbU::getValueString(getThreshold()) << endl;
    Locator<Instance*>* locator =
      getMasterCell()->getInstancesUnder(getArea(),getThreshold()).getLocator();

    if ( locator->isValid() ) {
      push_back ( new QueryState ( locator ) );

      updateTransformation ();
    //_tab++;

      return true;
    } else
      delete locator;

  //cerr << "  Aborting level down" << endl;
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
    if (levelCompleted()) return;

    back()->_locator->progress();
    if (not back()->_locator->isValid()) return;

  //cerr << "  stack:" << std::setw(3) << _instanceCount << ":" << getPath() << endl;
    ++_instanceCount;
    updateTransformation();
  }


  inline  void  QueryStack::progress ( bool init )
  {
    if (not init) levelProgress ();
    else {
      if (not levelDown() and (size() > _startLevel))
        return;
    }

    while (not empty()) {
      if (levelCompleted()) {
        levelUp ();
      } else {
        if (levelDown()) continue;
      }

      if (size() > _startLevel) return;
      if (empty()) break;
      levelProgress();
    }
  }


// -------------------------------------------------------------------
// Class  :  "Query".

  class Query {
    public:
      typedef Hurricane::Mask<int> Mask;
    public:
    // Types.
      enum  QueryFilter { DoMasterCells   =  1
                        , DoTerminalCells =  2
                        , DoComponents    =  4
                        , DoMarkers       =  8
                        , DoRubbers       = 16
                        , DoExtensionGos  = 32
                        , DoAll           = DoMasterCells
                                          | DoTerminalCells
                                          | DoComponents
                                          | DoMarkers
                                          | DoRubbers
                                          | DoExtensionGos
                        };
    public:
    // Constructors & Destructors.
                                    Query                  ();
      virtual                      ~Query                  ();
    // Accessors.
      inline  unsigned int          getStartLevel          () const;
      inline  unsigned int          getStopLevel           () const;
      inline  Cell::Flags           getStopCellFlags       () const;
      inline  size_t                getDepth               () const;
      inline  const Transformation& getTransformation      () const;
      inline  const Box&            getArea                () const;
      inline  DbU::Unit             getThreshold           () const;
      inline  const BasicLayer*     getBasicLayer          () const;
      inline  Cell*                 getMasterCell          ();
      inline  Instance*             getInstance            ();
      inline  Path                  getPath                () const;
    //inline  const Tabulation&     getTab                 () const;
      virtual bool                  hasGoCallback          () const;
      virtual bool                  hasMarkerCallback      () const;
      virtual bool                  hasRubberCallback      () const;
      virtual bool                  hasExtensionGoCallback () const;
      virtual bool                  hasMasterCellCallback  () const;
      virtual void                  goCallback             ( Go*     ) = 0;
      virtual void                  markerCallback         ( Marker* );
      virtual void                  rubberCallback         ( Rubber* );
      virtual void                  extensionGoCallback    ( Go*     ) = 0;
      virtual void                  masterCellCallback     () = 0;
    // Modifiers.
              void                  setQuery               ( Cell*                 cell
                                                           , const Box&            area
                                                           , const Transformation& transformation
                                                           , const BasicLayer*     basicLayer
                                                           , ExtensionSlice::Mask  extensionMask
                                                           , Mask                  filter
                                                           , DbU::Unit             threshold=0
                                                           );
      inline  void                  setCell                ( Cell*                 cell );
      inline  void                  setArea                ( const Box&            area );
      inline  void                  setThreshold           ( DbU::Unit             threshold );
      inline  void                  setTransformation      ( const Transformation& transformation );
      virtual void                  setBasicLayer          ( const BasicLayer*     basicLayer );
      inline  void                  setExtensionMask       ( ExtensionSlice::Mask  mode );
      inline  void                  setFilter              ( Mask                  mode );
      inline  void                  setStartLevel          ( unsigned int          level );
      inline  void                  setStopLevel           ( unsigned int          level );
      inline  void                  setStopCellFlags       ( Cell::Flags );
      virtual void                  doQuery                ();

    protected:
    // Internal: Attributes.
              QueryStack            _stack;
              const BasicLayer*     _basicLayer;
              ExtensionSlice::Mask  _extensionMask;
              Mask                  _filter;
  };


// Query Inline Functions.

  inline  void  Query::setCell           ( Cell*                 cell )           { _stack.setTopCell(cell); }
  inline  void  Query::setArea           ( const Box&            area )           { _stack.setTopArea(area); }
  inline  void  Query::setThreshold      ( DbU::Unit             threshold )      { _stack.setThreshold(threshold); }
  inline  void  Query::setTransformation ( const Transformation& transformation ) { _stack.setTopTransformation(transformation); }
  inline  void  Query::setFilter         ( Mask                  filter )         { _filter = filter; }
  inline  void  Query::setExtensionMask  ( ExtensionSlice::Mask  mask )           { _extensionMask = mask; }
  inline  void  Query::setStartLevel     ( unsigned int          level )          { _stack.setStartLevel(level); }
  inline  void  Query::setStopLevel      ( unsigned int          level )          { _stack.setStopLevel(level); }
  inline  void  Query::setStopCellFlags  ( Cell::Flags           flags )          { _stack.setStopCellFlags(flags); }

  inline  unsigned int          Query::getStartLevel      () const { return _stack.getStartLevel(); }
  inline  unsigned int          Query::getStopLevel       () const { return _stack.getStopLevel(); }
  inline  Cell::Flags           Query::getStopCellFlags   () const { return _stack.getStopCellFlags(); }
  inline  size_t                Query::getDepth           () const { return _stack.size(); }
  inline  const Box&            Query::getArea            () const { return _stack.getArea(); }
  inline  const Transformation& Query::getTransformation  () const { return _stack.getTransformation(); }
  inline  Path                  Query::getPath            () const { return _stack.getPath(); }
  inline  const BasicLayer*     Query::getBasicLayer      () const { return _basicLayer; }
  inline  Cell*                 Query::getMasterCell      () { return _stack.getMasterCell(); }
  inline  Instance*             Query::getInstance        () { return _stack.getInstance(); }
//inline const Tabulation&      Query::getTab             () const { return _stack.getTab(); }
  

} // Hurricane namespace.
