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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/DebugSession.h"                    |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_DEBUG_SESSION_H
#define  HURRICANE_DEBUG_SESSION_H

#include <set>
#include <stack>
#include "hurricane/Commons.h"


namespace Hurricane {

  class Name;
  class Net;
  class Cell;

  using std::pair;
  using std::make_pair;
  using std::set;
  using std::stack;


// -------------------------------------------------------------------
// Class  :  "Hurricane::DebugSession".

  class DebugSession {

    public:
    // Static Access.
      static        DebugSession* create       ();
      static inline DebugSession* get          ();
      static inline bool          isTraced     ( const void* symbol );
      static inline void          isTracedNet  ( const Net* );
      static inline void          addToTrace   ( const void* symbol );
      static inline void          addToTrace   ( const Cell*, const Name& );
      static inline void          addToTrace   ( const Net* );
      static inline void          open         ( int minLevel, int maxLevel );
      static inline void          open         ( const void* symbol, int minLevel, int maxLevel );
      static inline void          close        ();
    // Singleton Access.
             inline bool          _isTraced    ( const void* symbol ) const;
             inline void          _addToTrace  ( const void* symbol );
                    void          _addToTrace  ( const Cell*, const Name& );
             inline void          _addToTrace  ( const Net*  net );
    // Inspector Management.
                    Record*       _getRecord   () const;
                    string        _getString   () const;
                    string        _getTypeName () const;

    protected:
    // Internal: Attributes.
      static DebugSession*           _singleton;
             set<const void*>        _symbols;
             stack< pair<int,int> >  _levels;

    protected:
    // Internal: Constructor & Destructor.
                                  DebugSession ();
                                 ~DebugSession ();
    private:
                                  DebugSession ( const DebugSession& );
             DebugSession&        operator=    ( const DebugSession& );
  };


// Inline Functions.

  void  DebugSession::open ( int minLevel, int maxLevel )
  {
    if (cdebug.getMinLevel() < minLevel) minLevel = cdebug.getMinLevel();
    if (cdebug.getMaxLevel() > maxLevel) maxLevel = cdebug.getMaxLevel();

    _singleton->_levels.push( make_pair( cdebug.setMinLevel(minLevel)
                                       , cdebug.setMaxLevel(maxLevel) ) );

  //std::cerr << "DebugSession::open() " << minLevel << ":" << maxLevel << std::endl;
  }


  void  DebugSession::open ( const void* symbol, int minLevel, int maxLevel )
  {
    if (cdebug.getMinLevel() < minLevel) minLevel = cdebug.getMinLevel();
    if (cdebug.getMaxLevel() > maxLevel) maxLevel = cdebug.getMaxLevel();

    if ( _singleton->_isTraced(symbol) ) {
      _singleton->_levels.push( make_pair( cdebug.setMinLevel(minLevel)
                                         , cdebug.setMaxLevel(maxLevel) ) );

    //std::cerr << "DebugSession::open() " << symbol << " " << minLevel << ":" << maxLevel << std::endl;
    } else {
      _singleton->_levels.push ( make_pair( cdebug.getMinLevel()
                                          , cdebug.getMaxLevel() ) );

    //std::cerr << "DebugSession::open() Same level " << minLevel << ":" << maxLevel << std::endl;
    }
  }


  void  DebugSession::close ()
  {
    if ( not _singleton->_levels.empty() ) {
      cdebug.setMinLevel( _singleton->_levels.top().first  );
      cdebug.setMaxLevel( _singleton->_levels.top().second );
      _singleton->_levels.pop ();

    //std::cerr << "DebugSession::close() " << cdebug.getMinLevel() << ":" << cdebug.getMaxLevel() << std::endl;
    }
  }


  DebugSession* DebugSession::get         ()                     { return _singleton; }
  bool          DebugSession::isTraced    ( const void* symbol ) { return _singleton->_isTraced(symbol); }
  void          DebugSession::addToTrace  ( const void* symbol ) { _singleton->_addToTrace(symbol); }
  void          DebugSession::addToTrace  ( const Net*  net    ) { _singleton->_addToTrace(net); }
  void          DebugSession::addToTrace  ( const Cell* cell
                                          , const Name& name   ) { _singleton->_addToTrace( cell, name ); }
  bool          DebugSession::_isTraced   ( const void* symbol ) const { return _symbols.find(symbol) != _symbols.end(); }
  void          DebugSession::_addToTrace ( const void* symbol ) { _symbols.insert( symbol ); }
  void          DebugSession::_addToTrace ( const Net*  net    ) { _addToTrace( static_cast<const void*>(net) ); }



} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::DebugSession);


#endif  // HURRICANE_DEBUG_SESSION_H
