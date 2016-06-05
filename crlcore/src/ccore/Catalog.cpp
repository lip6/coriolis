// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Catalog.cpp"                            |
// +-----------------------------------------------------------------+


# include <iomanip>
using namespace std;

#include "hurricane/Initializer.h"
#include "hurricane/SharedPath.h"
#include "hurricane/Collection.h"
#include "hurricane/DataBase.h"
#include "hurricane/Library.h"
#include "hurricane/Name.h"
#include "crlcore/Utilities.h"
#include "crlcore/Catalog.h"
#include "crlcore/AllianceFramework.h"


namespace CRL {

  using Hurricane::tab;
  using Hurricane::Initializer;
  using Hurricane::JsonTypes;
  using Hurricane::SharedPath;
  using Hurricane::DataBase;

  const char* MissingStateProperty = "%s:\n\n  Missing Catalog State Property in cell \"%s\".\n";


// -------------------------------------------------------------------
// Class :  "Catalog::State".


  Catalog::State::~State ()
  {
    if ( _cell )
      _cell->removeProperty ( CatalogProperty::getPropertyName() );
  }


  Cell* Catalog::State::setCell ( Cell* cell )
  {
    _cell = cell;
    if (_cell) {
      if (isPad ()) _cell->setPad ( true );
      if (isFeed()) _cell->setFeed( true );
    }
    return _cell;
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

    s += (isFlattenLeaf()) ? 'C' : '-';
    s += (isFeed()       ) ? 'F' : '-';
    s += (isPad()        ) ? 'P' : '-';
    s += (isGds()        ) ? 'G' : '-';
    s += (isDelete()     ) ? 'D' : '-';
    s += (isInMemory()   ) ? 'm' : '-';

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


  void  Catalog::State::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename", _getTypeName() );
    jsonWrite( w, "_flags"   , _getString()   );
    jsonWrite( w, "_depth"   , _depth         );

    string cellName = "";
    if (_cell) cellName = getString( _cell->getHierarchicalName() );
    jsonWrite( w, "_cell", cellName );
    w->endObject();
  }


  Initializer<Catalog::State::JsonState>  jsonCatalogStateInit ( 20 );


  Catalog::State::JsonState::JsonState ( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_flags", typeid(string)  );
    add( "_depth", typeid(int64_t) );
    add( "_cell" , typeid(string)  );
  }


  string  Catalog::State::JsonState::getTypeName () const
  { return "Catalog::State"; }


  void  Catalog::State::JsonState::initialize ()
  { JsonTypes::registerType( new JsonState (JsonWriter::RegisterMode) ); }


  Catalog::State::JsonState* Catalog::State::JsonState::clone ( unsigned long flags ) const
  { return new JsonState ( flags ); }


  void Catalog::State::JsonState::toData ( JsonStack& stack )
  {
    check( stack, "Catalog::State::JsonState::toData" );

    string       cellPath = get<string> ( stack, "_cell" );
    string       sflags   = get<string> ( stack, "_flags" );
    unsigned int depth    = get<int64_t>( stack, "_depth" );

    char    separator   = SharedPath::getNameSeparator();
    size_t  dot         = cellPath.rfind( separator );
    string  cellName    = cellPath.substr(dot+1);
    string  libraryName = cellPath.substr(0,dot);

    Library* library = DataBase::getDB()->getLibrary( libraryName
                                                    , DataBase::CreateLib|DataBase::WarnCreateLib );
    Cell*    cell    = library->getCell( cellName );

    Catalog*        catalog = AllianceFramework::get()->getCatalog();
    Catalog::State* state   = catalog->getState( cellName );

    if (not state) state = catalog->getState( cellName, true );
    if (state->getCell   () != cell   ) state->setCell   ( cell );
    if (state->getLibrary() != library) state->setLibrary( library );

    state->setDepth( depth );
    state->setFlattenLeaf( (sflags[0] == 'C') );
    state->setFeed(        (sflags[1] == 'F') );
    state->setPad(         (sflags[2] == 'P') );
    state->setGds(         (sflags[3] == 'G') );
    state->setDelete(      (sflags[4] == 'D') );
    state->setInMemory(    (sflags[5] == 'm') );

    update( stack, state );
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

  Name  CatalogProperty::_name = "Catalog::State::Property";


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


  bool  CatalogProperty::hasJson () const
  { return true; }


  void  CatalogProperty::toJson ( JsonWriter* w, const DBo* ) const
  {
    w->startObject();
    std::string tname = getString(getPropertyName());
    jsonWrite( w, "@typename", tname  );
    jsonWrite( w, "_state"   , _state );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonCatalogProperty"

  Initializer<JsonCatalogProperty>  jsonCatalogPropertyInit ( 20 );


  JsonCatalogProperty::JsonCatalogProperty ( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_state", typeid(Catalog::State*) );
  }


  string  JsonCatalogProperty::getTypeName () const
  { return getString(CatalogProperty::getPropertyName()); }


  void  JsonCatalogProperty::initialize ()
  { JsonTypes::registerType( new JsonCatalogProperty (JsonWriter::RegisterMode) ); }


  JsonCatalogProperty* JsonCatalogProperty::clone ( unsigned long flags ) const
  { return new JsonCatalogProperty ( flags ); }


  void JsonCatalogProperty::toData ( JsonStack& stack )
  {
    check( stack, "JsonCatalogProperty::toData" );

    DBo*             dbo      = stack.back_dbo();
    Catalog::State*  state    = get<Catalog::State*>( stack, "_state" );
    CatalogProperty* property = NULL;

    cdebug.log(19) << "topDBo:" << dbo << endl;

    Cell* cell = dynamic_cast<Cell*>( dbo );
    if (cell) {
      Property* base = cell->getProperty( CatalogProperty::getPropertyName() );
      if (base) {
        property = static_cast<CatalogProperty*>( base );
        if (property->getState() != state) {
          cerr << Error( "JsonCatalogProperty::toData(): State object incoherency on Cell \"%s\"."
                       , getString(cell->getName()).c_str()
                       ) << endl;
        }
      } else {
        property = CatalogProperty::create( state );
        cell->put( property );
      }
    }
  // NULL Cell means we are parsing the Catalog.
    
    update( stack, property );
  }


// -------------------------------------------------------------------
// Class  :  "CatalogExtension"


  const Cell*     CatalogExtension::_owner = NULL;
  Catalog::State* CatalogExtension::_cache = NULL;


  Catalog::State* CatalogExtension::get ( const Cell* cell )
  {
    if ( cell == _owner ) return _cache;
    _owner = cell;

    Property* property = _owner->getProperty ( CatalogProperty::getPropertyName() );
    if ( property ) _cache = static_cast<CatalogProperty*>(property)->getState();
    else            _cache = NULL;

    return _cache;
  }


} // End of CRL namespace.
