// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./JsonParameter.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/viewer/JsonParameter.h"


void  jsonWrite ( JsonWriter* w, const Cfg::Configuration* cfg )
{
  w->startObject();
  jsonWrite( w, "@typename"  , "Cfg::Configuration" );
  jsonWrite( w, "+parameters", cfg->getParameters() );

  // w->key( "+parameters" );
  // w->startArray();
  // for ( auto mapElement : cfg->getParameters() ) jsonWrite( w, mapElement.second );
  // w->endArray();
  
  w->endObject();
}


void  jsonWrite ( JsonWriter* w, const Cfg::Parameter* p )
{
  w->startObject();
  jsonWrite( w, "@typename", "Cfg::Parameter" );
  jsonWrite( w, "_id"      , p->getId() );
  jsonWrite( w, "_type"    , Cfg::Parameter::typeToString(p->getType()) );
  jsonWrite( w, "_priority", Cfg::Parameter::priorityToString(p->getPriority()) );
  jsonWrite( w, "_value"   , p->asString() );
  jsonWrite( w, "_flags"   , p->getFlags() );
  
  if (p->getType() == Cfg::Parameter::Int) {
    if (p->hasMin()) jsonWrite( w, "_minInt", p->getMinInt() );
    if (p->hasMax()) jsonWrite( w, "_maxInt", p->getMaxInt() );
  }
  if (p->getType() == Cfg::Parameter::Double) {
    if (p->hasMin()) jsonWrite( w, "_minDouble", p->getMinDouble() );
    if (p->hasMax()) jsonWrite( w, "_maxDouble", p->getMaxDouble() );
  }

  w->endObject();
}



namespace Hurricane {

  using Cfg::Parameter;
  using Cfg::Configuration;


// -------------------------------------------------------------------
// Class  :  "JsonConfiguration"

  Initializer<JsonConfiguration>  jsonConfigurationInit ( 0 );


  void  JsonConfiguration::initialize ()
  { JsonTypes::registerType( new JsonConfiguration (JsonWriter::RegisterMode) ); }


  JsonConfiguration::JsonConfiguration( unsigned long flags )
    : JsonObject(flags)
  {
    add( "+parameters", typeid(JsonArray) );
  }


  string JsonConfiguration::getTypeName () const
  { return "Cfg::Configuration"; }


  JsonConfiguration* JsonConfiguration::clone ( unsigned long flags ) const
  { return new JsonConfiguration ( flags ); }


  void JsonConfiguration::toData ( JsonStack& stack )
  {
    check( stack, "JsonConfiguration::toData" );

    Configuration* cfg = Configuration::get();

    update( stack, cfg );
  }


// -------------------------------------------------------------------
// Class  :  "JsonParameter"

  Initializer<JsonParameter>  jsonParameterInit ( 0 );


  void  JsonParameter::initialize ()
  { JsonTypes::registerType( new JsonParameter (JsonWriter::RegisterMode) ); }


  JsonParameter::JsonParameter( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_id"      , typeid(string)  );
    add( "_type"    , typeid(string)  );
    add( "_priority", typeid(string)  );
    add( "_value"   , typeid(string)  );
    add( "_flags"   , typeid(int64_t) );
  }


  string JsonParameter::getTypeName () const
  { return "Cfg::Parameter"; }


  JsonParameter* JsonParameter::clone ( unsigned long flags ) const
  { return new JsonParameter ( flags ); }


  void JsonParameter::toData ( JsonStack& stack )
  {
    check( stack, "JsonParameter::toData" );

    Configuration* cfg = get<Configuration*>( stack, "_configuration" );

    Parameter* param     = NULL;
    string     id        = get<string> ( stack, "_id"       );
    string     stype     = get<string> ( stack, "_type"     );
    string     spriority = get<string> ( stack, "_priority" );
    string     value     = get<string> ( stack, "_value"    );
    int        flags     = get<int64_t>( stack, "_flags"    );

    Parameter::Type      type      = Parameter::stringToType    (stype);
    Parameter::Priority  priority  = Parameter::stringToPriority(spriority);

    param = cfg->getParameter( id );
    if (not param) {
      param = cfg->addParameter( id, type, value, priority );
    } else {
      if (param->getType() != type) {
        cerr << Error( "JsonParameter::toData(): Type incoherency on parameter \"%s\", not restored."
                     , id.c_str() ) << endl;
      }
      param->setRawString( value, Parameter::Interactive );
      param->setPriority( priority );
    }

    if (type == Parameter::Int) {
      if (flags & Parameter::HasMin) {
        add( "_minInt", typeid(int64_t) );
        param->setMin( (int)get<int64_t>( stack, "_minInt" ), Parameter::Interactive );
      }
      if (flags & Parameter::HasMax) {
        add( "_maxInt", typeid(int64_t) );
        param->setMax( (int)get<int64_t>( stack, "_maxInt" ), Parameter::Interactive );
      }
    }

    if (type == Parameter::Double) {
      if (flags & Parameter::HasMin) {
        add( "_minDouble", typeid(int64_t) );
        param->setMin( (double)get<int64_t>( stack, "_minDouble" ), Parameter::Interactive );
      }
      if (flags & Parameter::HasMax) {
        add( "_maxDouble", typeid(int64_t) );
        param->setMax( (double)get<int64_t>( stack, "_maxDouble" ), Parameter::Interactive );
      }
    }

    update( stack, param );
  }



}  // Hurricane namespace.
