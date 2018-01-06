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
// |  Author      :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./NetExternalComponents.cpp"                   |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"


namespace Hurricane {

// -------------------------------------------------------------------
// Class  :  "NetExternalComponents".

  const Name NetExternalComponents::_name = "ExternalComponentsRelation";


  StandardRelation* NetExternalComponents::getRelation ( const Net* net )
  {
    Property* property = net->getProperty(_name);
    if (not property) return NULL;

    StandardRelation* relation = dynamic_cast<StandardRelation*>(property);
    if (not relation)
      throw Error("Bad Property type: Must be a Standard Relation");
    return relation;
  }


  Components NetExternalComponents::get ( const Net* net )
  {
    if (!net->isExternal())
      throw Error("Impossible to retrieve external components on non external net "
                 + net->getName()._getString());
    
    StandardRelation* externalComponentsRelation = getRelation(net);
    if (!externalComponentsRelation)
      return Components();
    return externalComponentsRelation->getSlaveOwners().getSubSet<Component*>();
  }


  void NetExternalComponents::setExternal ( Component* component )
  {
    Net* net = component->getNet();
    if (!net->isExternal())
      throw Error("Impossible to set as external a component member of non external net "
                 + net->getName()._getString());
    StandardRelation* externalComponentsRelation = getRelation(net);
    if (!externalComponentsRelation)
      externalComponentsRelation = StandardRelation::create(net, _name);
    component->put(externalComponentsRelation);
  }


  bool  NetExternalComponents::isExternal ( Component* component )
  {
    Net* net = component->getNet();
    if (!net->isExternal()) return false;

    return component->getProperty(_name) != NULL;
  }


  void  NetExternalComponents::toJson ( JsonWriter* w, const Net* net )
  {
    w->startObject();
    jsonWrite( w, "@typename", "NetExternalComponents" );

    w->setFlags( JsonWriter::UseEntityReference );
    if (net->isExternal()) {
      jsonWrite( w, "+entities", get(net) );
    } else {
      w->key( "+entities" );
      w->startArray();
      w->endArray();
    }
    w->resetFlags( JsonWriter::UseEntityReference );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonNetExternalComponents".

  Initializer<JsonNetExternalComponents>  jsonNetExternalComponentsInit ( 0 );


  void  JsonNetExternalComponents::initialize ()
  { JsonTypes::registerType( new JsonNetExternalComponents (JsonWriter::RegisterMode) ); }


  JsonNetExternalComponents::JsonNetExternalComponents ( unsigned long flags )
    : JsonBaseArray<Entity*>(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    add( "+entities", typeid(JsonArray) );
  }


  JsonNetExternalComponents::~JsonNetExternalComponents ()
  {
    for ( Entity* entity : array() ) {
      Component* component = dynamic_cast<Component*>( entity );
      if (component) NetExternalComponents::setExternal( component );
      else {
        cerr << Error( "JsonNetExternalComponents(): %s in not a Component."
                     , getString(entity).c_str() ) << endl;
      }
    }
  }


  string  JsonNetExternalComponents::getTypeName () const
  { return "NetExternalComponents"; }


  JsonNetExternalComponents* JsonNetExternalComponents::clone( unsigned long flags ) const
  { return new JsonNetExternalComponents ( flags ); }


  void JsonNetExternalComponents::toData ( JsonStack& stack )
  {
    cdebug_tabw(19,1);

    check ( stack, "JsonNetExternalComponents::toData" );
    update( stack, NULL );

    cdebug_tabw(19,-1);
  }


} // End of Hurricane namespace.
