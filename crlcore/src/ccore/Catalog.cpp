
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Catalog.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include <iomanip>
using namespace std;

#include  "hurricane/Collection.h"
#include  "hurricane/Library.h"
#include  "hurricane/Name.h"

#include "crlcore/Utilities.h"
#include "crlcore/Catalog.h"


namespace CRL {


  const char* MissingStateProperty   = "%s:\n\n  Missing Catalog State Property in cell \"%s\".\n";


// -------------------------------------------------------------------
// Class :  "Catalog::State".


  Catalog::State::~State ()
  {
    if ( _cell )
      _cell->removeProperty ( CatalogProperty::getPropertyName() );
  }


  void   Catalog::State::merge ( const State& other )
  {
    if ( (_cell   ==NULL) && other._cell    ) _cell    = other._cell;
    if ( (_library==NULL) && other._library ) _library = other._library;

    _flags |= other._flags;
    _depth  = max ( _depth, other._depth );
  }


  string  Catalog::State::_getString () const
  {
    string  s;

    if ( isFlattenLeaf() ) s += 'C';
    if ( isFeed()        ) s += 'F';
    if ( isGds()         ) s += 'G';
    if ( isDelete()      ) s += 'D';

    return s;
  }


  Record* Catalog::State::_getRecord () const
  {
    Record* record = new Record ( "<State " + _getString() + " >" );
    if ( record != NULL ) {
      record->add ( getSlot("_flags"  , _flags  ) );
      record->add ( getSlot("_depth"  , _depth  ) );
      record->add ( getSlot("_cell"   , _cell   ) );
      record->add ( getSlot("_library", _library) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Catalog".

  Catalog::~Catalog ()
  {
    clear ();
  }


  Catalog::State* Catalog::getState ( const Name& name, bool add )
  {
    map<Name,State*>::iterator  it;

    if ( (it=_states.find(name)) != _states.end() ) return it->second;

    State *state = NULL;
    if ( add ) {
      state = new State ();
      _states [ name ] = state;
    }
    return ( state );
  }


  void  Catalog::mergeState ( const Name& name, const State& other )
  {
    map<Name,State*>::iterator  it;
    State* state;

    if ( (it=_states.find(name)) == _states.end() )
      state = getState ( name, true );
    else
      state = it->second;

    state->merge ( other );
  }


  bool  Catalog::deleteState ( const Name& name )
  {
    map<Name,State*>::iterator  it;

    if ( (it=_states.find(name)) == _states.end() ) return false;

    State *state = it->second;

    delete state;
    _states.erase ( it );

    return true;
  }


  void  Catalog::clear ()
  {
    map<Name,State*>::const_iterator  istate;
    for ( istate=_states.begin() ; istate!=_states.end() ; istate++ ) {
      delete istate->second;
    }
    _states.clear ();
  }


  bool  Catalog::readLine ( const string& s, string& name, State* state )
  {
    static string spaceChars = " 	";

    size_t start = s.find_first_not_of ( spaceChars );
    size_t stop  = s.find_first_of     ( spaceChars, start );

    if ( stop == string::npos ) return false;

    name = s.substr ( start, stop-start );

    start = s.find_first_not_of ( spaceChars, stop );
    if ( start == string::npos ) return false;
  
    for ( ; start<s.size() ; start++ ) {
      switch ( s[start] ) {
        case 'C': state->setFlattenLeaf ( true ); break;
        case 'F': state->setFeed        ( true ); break;
        case 'G': state->setGds         ( true ); break;
        case 'D': state->setDelete      ( true ); break;
        default: return false;
      }
    }

    return true;
  }


  bool  Catalog::loadFromFile ( const string& path, Library* library )
  {
    IoFile  catalogFile ( path );

    catalogFile.open ( "r" );
    if ( !catalogFile.isOpen() ) return false;

    string  cellName;
    char    rawLine[1024];

    while ( !catalogFile.eof() ) {
      State   cellState;
      cellState.setLibrary ( library );

      string line = catalogFile.readLine(rawLine,1023);
      if ( !line.size() ) continue;

      if ( !readLine (line,cellName,&cellState) ) {
        cerr << "[WARNING] Catalog::LoadFromFile() :\n"
             << "  Malformed catalog line \"" << line
             << "\"\n  (file: " << path << ", line: " << catalogFile.getLineNumber() << ")" << endl;
        continue;
      }
      mergeState ( cellName, cellState );
    }
    catalogFile.close ();

    return true;
  }


  string  Catalog::_getPrint () const
  {
    map<Name,State*>::const_iterator  it;
    ostringstream  s;

    for ( it=_states.begin() ; it!=_states.end() ; it++ ) {
      s << left << setw(30) << getString(it->first) << getString(it->second) << endl;
    }

    return s.str();
  }


  string  Catalog::_getString () const
  {
    ostringstream  s;

    s << "<StateMap " << _states.size() << ">";
    return s.str();
  }


  Record* Catalog::_getRecord () const
  {
    map<Name,State*>::const_iterator  it;

    Record* record = new Record ( "<Catalog>" );

    record->add ( getSlot ( "States", &_states ) );

    return record;
  }


// -------------------------------------------------------------------
// Class  :  "CatalogProperty"


  Name  CatalogProperty::_name = "Alliance Catalog State";


  CatalogProperty* CatalogProperty::create ( Catalog::State* state )
  {
    CatalogProperty *property = new CatalogProperty ( state );

    property->_postCreate ();
    return property;
  }


  void  CatalogProperty::onReleasedBy ( DBo* owner )
  {
    _state->setCell ( NULL );
    PrivateProperty::onReleasedBy ( owner );
  }


  Name  CatalogProperty::getPropertyName ()
  {
    return _name;
  }


  Name  CatalogProperty::getName () const
  {
    return getPropertyName();
  }


  string  CatalogProperty::_getTypeName () const
  {
    return _TName ( "CatalogProperty" );
  }


  string  CatalogProperty::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(_state) );

    return s;
  }


  Record* CatalogProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot("_name" ,_name ) );
      record->add( getSlot("_state",_state) );
    }
    return record;
  }


} // End of CRL namespace.
