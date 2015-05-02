// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        Alliance VHDL / Hurricane  Interface                     |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./alliance/vst/VhdlEntity.cpp"                |
// +-----------------------------------------------------------------+


#include <iomanip>
#include <ctime>
#include "hurricane/Warning.h"
#include "hurricane/Cell.h"
#include "crlcore/VhdlPortMap.h"
#include "crlcore/VhdlEntity.h"


namespace {

  using Hurricane::Net;
  using Hurricane::Instance;
  using Vhdl::Signal;
  using Vhdl::PortMap;
  using Vhdl::Entity;


  class ComparePort {
    public:
      ComparePort ( unsigned int flags ) : _flags(flags) { };
    public:
      inline bool operator() ( const Signal* lhs, const Signal* rhs ) const
      {
        bool isSupply1 = lhs->getNet()->isSupply();
        bool isSupply2 = rhs->getNet()->isSupply();
        if (isSupply1 != isSupply2) return not isSupply1;

        if (_flags & Entity::AsPortSignal) {
          unsigned int  dir1 = (unsigned int)lhs->getDirection() & Net::Direction::INOUT;
          unsigned int  dir2 = (unsigned int)rhs->getDirection() & Net::Direction::INOUT;
          if (dir1 != dir2) {
            if (not dir1) return false;
            if (not dir2) return true;
            return dir1 < dir2;
          }
        }

        bool isV1 = lhs->isVector();
        bool isV2 = rhs->isVector();
        if (isV1 != isV2) return not isV1;

        size_t range1 = lhs->getMax() - lhs->getMin();
        size_t range2 = rhs->getMax() - rhs->getMin();
        if (range1 != range2) return range1 < range2;

        return lhs->getName() < rhs->getName();
      }
    private:
      unsigned int  _flags;
  };


  class ComparePortMap {
    public:
      ComparePortMap () : _comparePort(Entity::AsPortSignal) { };
    public:
      inline bool operator() ( const PortMap* lhs, const PortMap* rhs ) const
      { return _comparePort( lhs->getSignal(), rhs->getSignal() ); }
    private:
      ComparePort  _comparePort;
  };


}


namespace Vhdl {

  using namespace std;
  using Hurricane::Warning;
  using Hurricane::Property;
  using Hurricane::_TName;
  using Hurricane::ForEachIterator;


  Tabulation  tab ("  ");


// -------------------------------------------------------------------
// Class  :  "::Entity".

  vector<Entity*>  Entity::_entities;
  ptrdiff_t        Entity::_offset = 0;


  Entity::Entity ( EntityProperty* property, Cell* cell, unsigned int flags )
    : _signals()
    , _globals()
    , _flags  (flags)
  {
    if (not _offset) {
    //_offset = offsetof(EntityProperty,_entity);
      _offset = (ptrdiff_t)this - (ptrdiff_t)property;
    }

    if (_flags == NoFlags) _flags = EntityMode;

    forEach ( Net*, inet, cell->getNets() ) {
      if (not inet->isExternal() and (flags & ComponentMode)) continue;

      string  stem;
      size_t  index  = 0;
      if (parseNetName(*inet,stem,index)) {
        if (inet->isGlobal()) {
          cerr << Warning( "Vhdl::Entity::Entity(): Net is both vectorized and global, this is not allowed.\n"
                         "         On Net <%s> of Cell <%s>."
                         , getString(inet->getName()).c_str()
                         , getString(cell->getName()).c_str()
                         ) << endl;
        }

        VectorSignal* signal = const_cast<VectorSignal*>( dynamic_cast<const VectorSignal*>( getSignal(stem) ) );
        if (not signal)
          signal = new VectorSignal ( stem );
        signal->addNet( index, *inet );
        _signals.insert( signal );
      } else {
        _signals.insert( new ScalarSignal(*inet) );
        if (inet->isGlobal())
          _globals.insert( new ScalarSignal(*inet) );
      }
    }

    _entities.push_back( this );
  }


  Entity::~Entity ()
  {
    for ( auto signal : _signals ) delete signal;
    for ( auto ientity=_entities.begin() ; ientity!=_entities.end() ; ++ientity ) {
      if (*ientity == this) {
        _entities.erase( ientity );
        break;
      }
    }
  }


  vector<Entity*>& Entity::getAllEntities ()
  { return _entities; }


  bool  Entity::parseNetName (  const Net* net, string& stem, size_t& index  )
  {
    string error;
    string name     = getString(net->getName());
    size_t leftpar  = name.find( '(' );
    size_t rightpar = name.find( ')' );

    if (leftpar == string::npos) {
      stem  = name;
      index = 0;

      if (rightpar != string::npos) error = "unmatched right parenthesis";
    } else {
      if (rightpar == string::npos)
        error = "unmatched left parenthesis";
      else if (name.find('(',leftpar+1) != string::npos)
        error = "multiple left parenthesis";
      else if (name.find(')',rightpar+1) != string::npos)
        error = "multiple right parenthesis";
      else if (leftpar >= rightpar)
        error = "left parenthesis *after* the right one";
      else {
        size_t endindex = 0;
        int    value    = stoi( name.substr(leftpar+1), &endindex );

        if (endindex != rightpar-leftpar-1)
          error = "unable to convert index (not a number)";
        else if (value < 0)
          error = "negative index";
        else {
          stem  = name.substr( 0, leftpar );
          index = (size_t)value;
          return true;
        }
      }
    }

    if (not error.empty()) {
      cerr << Warning( "Entity::parseVector() Net has not a valid VHDL name, %s.\n"
                       "          %s\n"
                     , error.c_str()
                     , getString(net->getName()).c_str()
                     ) << endl;
    }

    return false;
  }


  void  Entity::toEntityMode ()
  {
    if (isEntityMode()) return;

    forEach ( Net*, inet, getCell()->getNets() ) {
      if (inet->isExternal()) continue;

      string  stem;
      size_t  index  = 0;
      if (parseNetName(*inet,stem,index)) {
        VectorSignal* signal = const_cast<VectorSignal*>( dynamic_cast<const VectorSignal*>( getSignal(stem) ) );
        if (not signal)
          signal = new VectorSignal ( stem );
        signal->addNet( index, *inet );
        _signals.insert( signal );
      } else {
        _signals.insert( new ScalarSignal(*inet) );
      }
    }
  }


  const Cell* Entity::getCell () const
  {
    EntityProperty* property = (EntityProperty*)((ptrdiff_t)(this) - _offset);
    return (const Cell*)property->getOwner();
  }


  const SignalSet* Entity::getSignals () const
  { return &_signals; }


  const Signal* Entity::getSignal ( string name ) const
  {
    VectorSignal key ( name );
    auto isignal = _signals.find( &key );
    if (isignal != _signals.end()) return *isignal;

    return NULL;
  }


  const Signal* Entity::getGlobal ( string name ) const
  {
    VectorSignal key ( name );
    auto isignal = _globals.find( &key );
    if (isignal != _globals.end()) return *isignal;

    return NULL;
  }


  void Entity::toSignals ( ostream& out ) const
  {
    size_t          width           = 0;
    vector<Signal*> internalSignals;
    for ( auto isignal=_signals.begin() ; isignal!=_signals.end() ; ++isignal ) {
      if (not (*isignal)->isExternal()) {
        width = max( width, (*isignal)->getName().size() );
        internalSignals.push_back( *isignal );
      }
    }
    sort( internalSignals.begin(), internalSignals.end(), ComparePort(Entity::AsInnerSignal) );
    
    for ( auto isignal=internalSignals.begin(); isignal!=internalSignals.end() ; ++isignal ) {
      out << tab;
      (*isignal)->toVhdlPort( out, width, Entity::AsInnerSignal );
      out << ";\n";
    }
    out << "\n";
  }


  void Entity::toPort ( ostream& out ) const
  {
    out << tab << "port ( ";

    size_t          width = 0;
    vector<Signal*> ioSignals;
    for ( auto isignal=_signals.begin() ; isignal!=_signals.end() ; ++isignal ) {
      if ((*isignal)->isExternal()) {
        width = max( width, (*isignal)->getName().size() );
        ioSignals.push_back( *isignal );
      }
    }
    sort( ioSignals.begin(), ioSignals.end(), ComparePort(Entity::AsPortSignal) );
    
    size_t ioCount = 0;
    for ( auto isignal=ioSignals.begin(); isignal!=ioSignals.end() ; ++isignal ) {
      if (ioCount) out << "\n" << tab << "     ; ";
      (*isignal)->toVhdlPort( out, width, Entity::AsPortSignal );
      ++ioCount;
    }
    out << "\n" << tab << "     );";
  }


  void Entity::toEntity ( ostream& out ) const
  {
    time_t  clock = time( nullptr );
    tm      tm    = *localtime( &clock );
    char    stamp[1024];

    strftime( stamp, 1024, "%b %d, %Y, %H:%M", &tm );

    out << "\n";
    out << "-- =======================================================================\n";
    out << "-- Coriolis Structural VHDL Driver\n";
    out << "-- Generated on " << stamp << "\n";
    out << "-- \n";
    out << "-- To be interoperable with Alliance, it uses it's special VHDL subset.\n";
    out << "-- (\"man vhdl\" under Alliance for more informations)\n";
    out << "-- =======================================================================\n";
    out << "\n";
    out << tab++ << "entity " << getCell()->getName() << " is\n";
    toPort( out );
    out << --tab << "\nend " << getCell()->getName() << ";\n\n";

    out << "architecture structural of " <<  getCell()->getName() << " is\n\n";
    ++tab;

    set<Cell*> masterCells;
    forEach ( Instance*, iinstance, getCell()->getInstances() ) {
      masterCells.insert( iinstance->getMasterCell() );
    }

    for ( auto icell : masterCells ) {
      Vhdl::Entity* component = Vhdl::EntityExtension::create( icell, Vhdl::Entity::ComponentMode );
      component->toComponent( out );
      out << "\n";
    }

    toSignals( out );

    out << "\n" << --tab << "begin\n\n";
    ++tab;

    forEach ( Instance*, iinstance, getCell()->getInstances() ) {
      toInstance( out, *iinstance );
      out << "\n";
    }

    --tab;
    out << "end structural;\n";
  }


  void Entity::toComponent ( ostream& out ) const
  {
    out << tab++ << "component " << getCell()->getName() << "\n";
    toPort( out );
    out << "\n" << --tab << "end component;\n";
  }


  void Entity::toInstance ( ostream& out, Instance* instance ) const
  {
    out << tab << instance->getName() << " : " << instance->getMasterCell()->getName() << "\n";
    out << tab << "port map ( ";

    Entity* masterEntity = EntityExtension::get( instance->getMasterCell() );
    if (not masterEntity) {
      masterEntity = EntityExtension::create( instance->getMasterCell(), ComponentMode );
    }

    size_t            width = 0;
    vector<PortMap*>  portMaps;
    const SignalSet*  masterSignals = masterEntity->getSignals();
    for ( auto isignal=masterSignals->begin() ; isignal!=masterSignals->end() ; ++isignal ) {
      if ((*isignal)->isExternal()) {
        width = max( width, (*isignal)->getName().size() );
        portMaps.push_back( PortMap::create(*isignal) );
        portMaps.back()->doMapping( instance );
      }
    }
    sort( portMaps.begin(), portMaps.end(), ComparePortMap() );
    
    size_t ioCount = 0;
    for ( auto iportmap=portMaps.begin(); iportmap!=portMaps.end() ; ++iportmap ) {
      if (ioCount) out << "\n" << tab << "         , ";
      (*iportmap)->toVhdlPortMap( out, width );
      ++ioCount;

      delete *iportmap;
    }

    out << "\n" << tab << "         );\n";
  }


  string  Entity::_getString () const
  {
    string  s = getString(getCell()->getName());
    return s;
  }


  Record* Entity::_getRecord () const
  {
    Record* record = new Record ( "<Entity " + _getString() + " >" );
    if (record != NULL) {
      record->add( getSlot("_signals", _signals) );
      record->add( getSlot("_flags"  , _flags  ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "EntityProperty"

  Name  EntityProperty::_name = "Vhdl::Entity";


  EntityProperty* EntityProperty::create ( Cell* owner, unsigned int flags )
  {
    EntityProperty *property = new EntityProperty( owner, flags );

    property->_postCreate ();
    return property;
  }


  void  EntityProperty::onReleasedBy ( DBo* owner )
  { PrivateProperty::onReleasedBy( owner ); }


  Name  EntityProperty::getPropertyName ()
  { return _name; }


  Name  EntityProperty::getName () const
  { return getPropertyName(); }


  string  EntityProperty::_getTypeName () const
  { return _TName( "EntityProperty" ); }


  string  EntityProperty::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(&_entity) );

    return s;
  }


  Record* EntityProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot( "_name"  ,  _name   ) );
      record->add( getSlot( "_entity", &_entity ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "EntityExtension"


  const Cell*    EntityExtension::_owner = NULL;
  Entity*        EntityExtension::_cache = NULL;


  Entity* EntityExtension::get ( const Cell* cell )
  {
    if (cell != _owner) {
      _owner = cell;

      Property* property = _owner->getProperty( EntityProperty::getPropertyName() );
      if (property) _cache = static_cast<EntityProperty*>(property)->getEntity();
      else          _cache = NULL;
    }
    return _cache;
  }


  Entity* EntityExtension::create ( Cell* cell, unsigned int flags )
  {
    get( cell );
    if (not _cache) {
      EntityProperty* property = new EntityProperty( cell, flags );
      cell->put( property );

      _cache = property->getEntity();
    } else {
      if ( (flags & Entity::EntityMode) and (not _cache->isEntityMode()) )
        _cache->toEntityMode();
    }

    return _cache;
  }


  void  EntityExtension::destroy ( Cell* cell )
  {
    Property* property = cell->getProperty( EntityProperty::getPropertyName() );
    if (property) static_cast<EntityProperty*>(property)->destroy();

    _owner = NULL;
    _cache = NULL;
  }


  void  EntityExtension::destroyAll ()
  {
    vector<Entity*>& entities = Entity::getAllEntities();

    while ( not entities.empty() ) destroy( const_cast<Cell*>(entities.back()->getCell()) );
  }



}  // Vhdl namespace.
