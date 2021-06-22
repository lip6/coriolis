// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |         S P I C E  / Hurricane  Interface                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./SpiceEntity.cpp"                            |
// +-----------------------------------------------------------------+


#include <iomanip>
#include <ctime>
#include "hurricane/Warning.h"
#include "hurricane/Cell.h"
#include "crlcore/SpiceEntity.h"


namespace {

  using Hurricane::Net;
  using Hurricane::Instance;
  using Spice::Bit;
  using Spice::Entity;

  
}


namespace Spice {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Property;
  using Hurricane::_TName;
  using Hurricane::Plug;


  class ComparePlugBySpiceIndex {
    public:
      inline bool operator() ( const Plug* lhs, const Plug* rhs ) const
      { return BitExtension::getIndex( lhs->getMasterNet() )
             < BitExtension::getIndex( rhs->getMasterNet() ); }
  };


// -------------------------------------------------------------------
// Class  :  "::Entity".

  vector<Entity*>  Entity::_entities;
  ptrdiff_t        Entity::_offset = 0;


  Entity::Entity ( EntityProperty* property, Cell* cell )
    : _bits()
    , _powerNode (Bit::nindex)
    , _groundNode(Bit::nindex)
  {
    if (not _offset) {
    //_offset = offsetof(EntityProperty,_entity);
      _offset = (ptrdiff_t)this - (ptrdiff_t)property;
    }

    map< string, Net*, greater<string> >  orderedNets;
    for ( Net* net : cell->getNets() ) {
      orderedNets.insert( make_pair( getString(net->getName()), net ) );
    }
    for ( auto item : orderedNets ) {
      Bit* bit = BitExtension::create( item.second, _bits.size() );
      _bits.push_back( bit );
      if (item.second->isPower ()) _powerNode  = bit->getIndex();
      if (item.second->isGround()) _groundNode = bit->getIndex();
    }

    _entities.push_back( this );
  }


  Entity::~Entity ()
  {
    for ( auto bit : _bits ) bit->destroy();
    for ( auto ientity=_entities.begin() ; ientity!=_entities.end() ; ++ientity ) {
      if (*ientity == this) {
        _entities.erase( ientity );
        break;
      }
    }
  }


  vector<Entity*>& Entity::getAllEntities ()
  { return _entities; }


  void  Entity::orderPlugs ( Instance* instance, vector<Plug*>& ordereds )
  {
    ordereds.clear();
  //Entity* entity = EntityExtension::create( instance->getMasterCell() );
    for ( Plug* plug : instance->getPlugs() )
      ordereds.push_back( plug );
    sort( ordereds.begin(), ordereds.end(), ComparePlugBySpiceIndex() );
  }


  const Cell* Entity::getCell () const
  {
    EntityProperty* property = (EntityProperty*)((ptrdiff_t)(this) - _offset);
    return (const Cell*)property->getOwner();
  }


  void  Entity::toNodeList ( ostream& out, bool asInterf ) const
  {
    for ( Bit* bit : _bits ) {
      const Net* net = bit->getNet();
      if (not net->isExternal() and asInterf) continue;
      out << "* " << ((asInterf) ? "INTERF" : "NET")
          << setw(6) << BitExtension::getName(net)
          << "  " << net->getName() << ".\n";
    }
    out << "\n";
  }


  void Entity::toEntity ( ostream& out ) const
  {
    set<Cell*,DBo::CompareById>  processedsModels;
    string  cellName = getString( getCell()->getName() );
    time_t  clock    = time( nullptr );
    tm      tm       = *localtime( &clock );
    char    stamp[1024];
    strftime( stamp, 1024, "%b %d, %Y, %H:%M", &tm );

    out << "* Coriolis Structural SPICE Driver\n";
    out << "* Generated on " << stamp << "\n";
    out << "* Cell/Subckt \"" << cellName << "\".\n";
    out << "* \n";
    toNodeList( out, true );

    for ( Instance* instance : getCell()->getInstances() ) {
      Cell* model = instance->getMasterCell();
      if (processedsModels.find(model) != processedsModels.end())
         continue;
      out << ".include " << model->getName() << ".spi\n";
      processedsModels.insert( model );
    }
    out << "\n";

    out << ".subckt " << getCell()->getName();
    for ( Bit* bit : _bits ) {
      if (not bit->getNet()->isExternal()) continue;
      out << " " << bit->getName();
    }
    out << "\n";
    toNodeList( out, false );

    for ( Instance* instance : getCell()->getInstances() ) {
      Entity* masterEntity = EntityExtension::create( instance->getMasterCell() );
      out << "x" << instance->getName();
      vector<Plug*> sortedPlugs;
      masterEntity->orderPlugs( instance, sortedPlugs );
      for ( Plug* plug : sortedPlugs ) {
        Net* plugNet = plug->getNet();
        if (plugNet) {
          Bit* bit = BitExtension::get( plugNet );
          if (bit)
            out << " " << bit->getName();
          else {
            cerr << Error( "SpiceEntity::toEntity(): In %s, Net \"%s\" is missing a Spice::Bit extension.\n"
                         , getString(getCell()).c_str()
                         , getString(plugNet).c_str()
                         ) << endl;
          }
        } else {
          Net* masterNet = plug->getMasterNet();
          if (masterNet->isGlobal()) {
            plugNet = getCell()->getNet( masterNet->getName() );
            if (plugNet) {
              Bit* bit = BitExtension::get( plugNet );
              if (bit)
                out << " " << bit->getName();
              else {
                cerr << Error( "SpiceEntity::toEntity(): In %s, Net \"%s\" is missing a Spice::Bit extension.\n"
                             , getString(getCell()).c_str()
                             , getString(plugNet).c_str()
                             ) << endl;
              }
            } else {
              cerr << Error( "SpiceEntity::toEntity(): In %s, unconnected Plug,\n"
                             "        %s"
                           , getString(getCell()).c_str()
                           , getString(plug).c_str()
                           ) << endl;
              out << " ?";
            }
          }
        }
      }
      out << " " << instance->getMasterCell()->getName() << "\n";
    }
    out << ".ends " << getCell()->getName() << "\n";
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
      record->add( getSlot("_bits"      , _bits       ) );
      record->add( getSlot("_powerNode" , _powerNode  ) );
      record->add( getSlot("_groundNode", _groundNode ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "EntityProperty"

  Name  EntityProperty::_name = "Spice::Entity";


  EntityProperty* EntityProperty::create ( Cell* owner )
  {
    EntityProperty *property = new EntityProperty( owner );

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


  Entity* EntityExtension::create ( Cell* cell )
  {
    get( cell );
    if (not _cache) {
      EntityProperty* property = new EntityProperty( cell );
      cell->put( property );

      _cache = property->getEntity();
    }
    return _cache;
  }


  void  EntityExtension::destroy ( Cell* cell )
  {
    Property* property = cell->getProperty( EntityProperty::getPropertyName() );
    if (property) cell->remove( property );

    _owner = NULL;
    _cache = NULL;
  }


  void  EntityExtension::destroyAll ()
  {
    vector<Entity*>& entities = Entity::getAllEntities();

    while ( not entities.empty() ) destroy( const_cast<Cell*>(entities.back()->getCell()) );
  }



}  // Spice namespace.
