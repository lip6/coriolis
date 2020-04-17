// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./alliance/vst/VstParser.h"                    |
// +-----------------------------------------------------------------+


#pragma once
#include <string>
#include <vector>
#include <map>
#include <hurricane/Name.h>
#include <hurricane/Entity.h>
namespace Hurricane {
  class Net;
  class Cell;
  class Instance;
}
#include <crlcore/Catalog.h>


namespace Vst {

  extern void  incVhdLineNumber ();


  enum TokenConstants { VhdlTo     = 1
                      , VhdlDownto = 2
                      , VhdlPlus   = 3
                      , VhdlMinus  = 4
                      , VhdlBus    = 5
                      };


  class  Constraint {
    public:
      inline       Constraint ();
    public:
      inline int   getFrom    () const;
      inline int   getTo      () const;
      inline bool  IsSet      () const;
      inline void  Set        ( int from, unsigned int direction, int to );
      inline void  UnSet      () { _set = false; }
      inline void  Init       ( int& index );
      inline void  Next       ( int& index );
      inline bool  End        ( int  index );
    private:
      int   _from;
      int   _to;
      bool  _set;
  };

  inline  Constraint::Constraint ()
    : _from(0), _to(0), _set(false)
  { }

  inline int   Constraint::getFrom () const { return _from; }
  inline int   Constraint::getTo   () const { return _to; }
  inline bool  Constraint::IsSet   () const { return _set; }
  inline void  Constraint::Init    ( int& index ) { index = _from; };

  inline void  Constraint::Set ( int from, unsigned int direction, int to )
  { _set  = true; _from = from; _to   = to; }

  inline void  Constraint::Next ( int& index )
  { (_from < _to) ? index++ : index--; }

  inline bool  Constraint::End ( int index )
  {
    if ( _from < _to ) return index <= _to;
    return index >= _to;
  }


  typedef  std::vector<Hurricane::Net*>         PinVector;
  typedef  std::map<Hurricane::Name,PinVector>  VectorMap;
  typedef  std::map<Hurricane::Cell*,VectorMap,Hurricane::Entity::CompareById>  CellVectorMap;


  class YaccState {
    public:
      std::string                  _vhdFileName;
      int                          _vhdLineNumber;
      int                          _errorCount;
      int                          _maxErrors;
      std::vector<std::string*>    _lexIdentifiers;
      std::deque<Hurricane::Name>  _cellQueue;
      CRL::Catalog::State*         _state;
      Hurricane::Cell*             _cell;
      Hurricane::Cell*             _masterCell;
      Hurricane::Instance*         _instance;
      Constraint                   _constraint;
      std::vector<std::string*>    _identifiersList;
      CellVectorMap                _cellVectorMap;
      PinVector                    _instanceNets;
      PinVector                    _masterNets;
      bool                         _masterPort;
      bool                         _firstPass;
      bool                         _behavioral;
      bool                         _ieeeVhdl;
      bool                         _ieeeWarned;
    public:
      inline              YaccState        ( const std::string& vhdFileName );
      inline             ~YaccState        ();
      inline bool         pushCell         ( Hurricane::Name );
      inline std::string* addLexIdentifier ( const char* );
  };

  
  YaccState::YaccState ( const std::string& vhdFileName )
    : _vhdFileName    (vhdFileName)
    , _vhdLineNumber  (1)
    , _errorCount     (0)
    , _maxErrors      (10)
    , _lexIdentifiers ()
    , _cellQueue      ()
    , _state          (NULL)
    , _cell           (NULL)
    , _masterCell     (NULL)
    , _instance       (NULL)
    , _constraint     ()
    , _identifiersList()
    , _cellVectorMap  ()
    , _instanceNets   ()
    , _masterNets     ()
    , _masterPort     (true)
    , _firstPass      (true) 
    , _behavioral     (false) 
    , _ieeeVhdl       (false) 
    , _ieeeWarned     (false) 
  { }
  
  inline  YaccState::~YaccState ()
  { for ( std::string* s : _lexIdentifiers ) delete s; }

  inline bool  YaccState::pushCell ( Hurricane::Name cellName )
  {
    for ( size_t i=0 ; i<_cellQueue.size(); ++i ) {
      if (_cellQueue[i] == cellName) return false;
    }
    _cellQueue.push_back( cellName );
    return true;
  }

  
  inline std::string* YaccState::addLexIdentifier ( const char* identifier )
  {
    _lexIdentifiers.push_back( new std::string(identifier) );
    return _lexIdentifiers.back();
  }


  class YaccStateStack : public std::vector<YaccState*> {
    public:
      inline YaccState* operator->() { return back(); };
      inline void       pop_back  () { delete back (); std::vector<YaccState*>::pop_back (); }
  };


  extern YaccStateStack  states;


}  // Vst namespace.
