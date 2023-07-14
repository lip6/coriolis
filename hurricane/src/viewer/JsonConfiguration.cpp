// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2016-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./JsonConfiguration.cpp"                  |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/LayoutDescription.h"
#include "hurricane/Error.h"
#include "hurricane/viewer/JsonConfiguration.h"


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

  jsonWrite( w, "+values", p->getValues() );

  w->key( "+slaves" );
  w->startArray();
  for ( std::string slave : p->getSlaves() ) {
    w->startObject();
    jsonWrite( w, "@typename", "Cfg::Parameter::Slave" );
    jsonWrite( w, "_id"      , slave );
    w->endObject();
  }
  w->endArray();

  w->endObject();
}


void  jsonWrite ( JsonWriter* w, const Cfg::Parameter::EnumValue v )
{
  w->startObject();
  jsonWrite( w, "@typename", "Cfg::Parameter::EnumValue" );
  jsonWrite( w, "_label"   ,  v._label );
  jsonWrite( w, "_value"   ,  v._value );
  w->endObject();
}


void  jsonWrite ( JsonWriter* w, const Cfg::Configuration* cfg )
{
  w->startObject();
  jsonWrite( w, "@typename"  , "Cfg::Configuration" );
  jsonWrite( w, "+parameters",  cfg->getParameters() );
  jsonWrite( w, "+layout"    , &cfg->getLayout    () );
  w->endObject();
}


void  jsonWrite ( JsonWriter* w, const Cfg::WidgetDescription* widget )
{
  w->startObject();
  jsonWrite( w, "@typename", "Cfg::WidgetDescription" );
  jsonWrite( w, "_type"    , Cfg::WidgetDescription::typeToString(widget->getType()) );
  jsonWrite( w, "_id"      , widget->getId    () );
  jsonWrite( w, "_label"   , widget->getLabel () );
  jsonWrite( w, "_column"  , widget->getColumn() );
  jsonWrite( w, "_span"    , widget->getSpan  () );
  jsonWrite( w, "_flags"   , widget->getFlags () );
  w->endObject();
}


void  jsonWrite ( JsonWriter* w, const Cfg::TabDescription* tab )
{
  w->startObject();
  jsonWrite( w, "@typename", "Cfg::TabDescription" );
  jsonWrite( w, "_name"    , tab->getName()        );
  jsonWrite( w, "_id"      , tab->getId()          );
  jsonWrite( w, "+widgets" , tab->getWidgets()     );
  w->endObject();
}


void  jsonWrite ( JsonWriter* w, const Cfg::LayoutDescription* layout )
{
  w->startObject();
  jsonWrite( w, "@typename", "Cfg::LayoutDescription" );
  jsonWrite( w, "+tabs"    , layout->getTabs()        );
  w->endObject();
}


namespace Hurricane {

  using Cfg::Parameter;
  using Cfg::Configuration;
  using Cfg::LayoutDescription;
  using Cfg::TabDescription;
  using Cfg::WidgetDescription;


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


  JsonParameter::JsonParameter ( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_id"      , typeid(string)    );
    add( "_type"    , typeid(string)    );
    add( "_priority", typeid(string)    );
    add( "_value"   , typeid(string)    );
    add( "_flags"   , typeid(int64_t)   );
    add( "+values"  , typeid(JsonArray) );
    add( "+slaves"  , typeid(JsonArray) );
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

    if (param) param->valueChanged();

    update( stack, param );
  }


// -------------------------------------------------------------------
// Class  :  "JsonParamEnumValue"

  Initializer<JsonParamEnumValue>  jsonParamEnumValueInit ( 0 );


  void  JsonParamEnumValue::initialize ()
  { JsonTypes::registerType( new JsonParamEnumValue (JsonWriter::RegisterMode) ); }


  JsonParamEnumValue::JsonParamEnumValue ( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_label", typeid(string)  );
    add( "_value", typeid(int64_t) );
  }


  string JsonParamEnumValue::getTypeName () const
  { return "Cfg::Parameter::EnumValue"; }


  JsonParamEnumValue* JsonParamEnumValue::clone ( unsigned long flags ) const
  { return new JsonParamEnumValue ( flags ); }


  void JsonParamEnumValue::toData ( JsonStack& stack )
  {
    check( stack, "JsonParamEnumValue::toData" );

    Parameter* param = get<Parameter*>( stack, ".Cfg::Parameter" );
    string     label = get<string>    ( stack, "_label" );
    int        value = get<int64_t>   ( stack, "_value" );

    if (not param) {
      cerr << Error( "JsonParamEnumValue::toData(): Missing \".Cfg::Parameter\" in stack context.\n"
                     "        (while reading Cfg::Parameter::EnumValue \"%s\")"
                   , label.c_str()
                   ) << endl;
    } else {
      if (stack.issetFlags(JsonWriter::TechnoMode)) {
        param->addValue( label, value );
      } 
    }

    update( stack, NULL );
  }


// -------------------------------------------------------------------
// Class  :  "JsonParamSlave"

  Initializer<JsonParamSlave>  jsonParamSlaveInit ( 0 );


  void  JsonParamSlave::initialize ()
  { JsonTypes::registerType( new JsonParamSlave (JsonWriter::RegisterMode) ); }


  JsonParamSlave::JsonParamSlave ( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_id", typeid(string)  );
  }


  string JsonParamSlave::getTypeName () const
  { return "Cfg::Parameter::Slave"; }


  JsonParamSlave* JsonParamSlave::clone ( unsigned long flags ) const
  { return new JsonParamSlave ( flags ); }


  void JsonParamSlave::toData ( JsonStack& stack )
  {
    check( stack, "JsonParamSlave::toData" );

    Parameter* param = get<Parameter*>( stack, ".Cfg::Parameter" );
    string     id    = get<string>    ( stack, "_id   " );

    if (not param) {
      cerr << Error( "JsonParamSlave::toData(): Missing \".Cfg::Parameter\" in stack context.\n"
                     "        (while reading Cfg::Parameter::Slave \"%s\")"
                   , id.c_str()
                   ) << endl;
    } else {
      if (stack.issetFlags(JsonWriter::TechnoMode)) {
        param->addSlave( id );
      } 
    }

    update( stack, NULL );
  }


// -------------------------------------------------------------------
// Class  :  "JsonWidgetDescription"

  Initializer<JsonWidgetDescription>  jsonWidgetDescriptionInit ( 0 );


  void  JsonWidgetDescription::initialize ()
  { JsonTypes::registerType( new JsonWidgetDescription (JsonWriter::RegisterMode) ); }


  JsonWidgetDescription::JsonWidgetDescription ( unsigned long flags )
    : JsonObject(flags)
  {
    add( ".Cfg::TabDescription", typeid(Cfg::TabDescription*) );
    add( "_type"               , typeid(string)  );
    add( "_id"                 , typeid(string)  );
    add( "_label"              , typeid(string)  );
    add( "_column"             , typeid(int64_t) );
    add( "_span"               , typeid(int64_t) );
    add( "_flags"              , typeid(int64_t) );
  }


  string JsonWidgetDescription::getTypeName () const
  { return "Cfg::WidgetDescription"; }


  JsonWidgetDescription* JsonWidgetDescription::clone ( unsigned long flags ) const
  { return new JsonWidgetDescription ( flags ); }


  void JsonWidgetDescription::toData ( JsonStack& stack )
  {
    check( stack, "JsonWidgetDescription::toData" );

    LayoutDescription* layout = get<LayoutDescription*>( stack, "+layout" );
    TabDescription*    tab    = get<TabDescription*>   ( stack, ".Cfg::TabDescription"    );

    WidgetDescription* widget = NULL;
    string             stype  = get<string> ( stack, "_type"     );
    string             id     = get<string> ( stack, "_id"       );
    string             label  = get<string> ( stack, "_label"    );
    int                column = get<int64_t>( stack, "_column"   );
    int                span   = get<int64_t>( stack, "_span"     );
    int                flags  = get<int64_t>( stack, "_flags"    );

    WidgetDescription::Type  type = WidgetDescription::stringToType(stype);

    if (not layout) {
      cerr << Error( "JsonWidgetDescription::toData(): Missing \"+layout\" in stack context.\n"
                     "        (while reading \"%s\")"
                   , id.c_str()
                   ) << endl;
    } else if (not tab) {
      cerr << Error( "JsonWidgetDescription::toData(): Missing \".Cfg::TabDescription\" in stack context.\n"
                     "        (while reading \"%s\")"
                   , id.c_str()
                   ) << endl;
    } else {
      if (stack.issetFlags(JsonWriter::TechnoMode)) {
        switch ( type ) {
          case WidgetDescription::Rule:    layout->addRule   ( tab->getName() ); break;
          case WidgetDescription::Title:   layout->addTitle  ( tab->getName(), label ); break;
          case WidgetDescription::Section: layout->addSection( tab->getName(), label, column ); break;
          case WidgetDescription::Parameter:
            layout->addParameter( tab->getName(), id, label, column, span, flags );
            widget = layout->getWidget( id );
            break;
        }
      } else {
      // Coherency checking only.
        if (type == WidgetDescription::Parameter) {
          widget = layout->getWidget( id );
          if (not widget) {
            cerr << Error( "JsonWidgetDescription::toData(): Parameter id=\"%s\" has no layout."
                         , id.c_str() ) << endl;
          }
        }
      }
    }

    update( stack, widget );
  }


// -------------------------------------------------------------------
// Class  :  "JsonTabDescription"

  Initializer<JsonTabDescription>  jsonTabDescriptionInit ( 0 );


  void  JsonTabDescription::initialize ()
  { JsonTypes::registerType( new JsonTabDescription (JsonWriter::RegisterMode) ); }


  JsonTabDescription::JsonTabDescription ( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_name"   , typeid(string)    );
    add( "_id"     , typeid(string)    );
    add( "+widgets", typeid(JsonArray) );
  }


  string JsonTabDescription::getTypeName () const
  { return "Cfg::TabDescription"; }


  JsonTabDescription* JsonTabDescription::clone ( unsigned long flags ) const
  { return new JsonTabDescription ( flags ); }


  void JsonTabDescription::toData ( JsonStack& stack )
  {
    check( stack, "JsonTabDescription::toData" );

    LayoutDescription* layout = get<LayoutDescription*>( stack, "+layout" );

    TabDescription* tab  = NULL;
    string          id   = get<string>( stack, "_id"   );
    string          name = get<string>( stack, "_name" );

    if (not layout) {
      cerr << Error( "JsonTabDescription::toData(): Missing \"+layout\" in stack context.\n"
                     "        (while reading TabDescription \"%s\")"
                   , id.c_str()
                   ) << endl;
    } else {
      if (stack.issetFlags(JsonWriter::TechnoMode)) {
        layout->addTab( name, id );
        tab = layout->getTab( name, id );
      } else {
      // Coherency checking only.
        tab = layout->getTab( name, id );
        if (not tab) {
          cerr << Error( "JsonTabDescription::toData(): Tab name=\"%s\", id=\"%s\" has no layout."
                       , name.c_str(), id.c_str() ) << endl;
        }
      }
    }

    update( stack, tab );
  }


// -------------------------------------------------------------------
// Class  :  "JsonLayoutDescription"

  Initializer<JsonLayoutDescription>  jsonLayoutDescriptionInit ( 0 );


  void  JsonLayoutDescription::initialize ()
  { JsonTypes::registerType( new JsonLayoutDescription (JsonWriter::RegisterMode) ); }


  JsonLayoutDescription::JsonLayoutDescription ( unsigned long flags )
    : JsonObject(flags)
  {
    add( "+tabs", typeid(JsonArray) );
  }


  JsonLayoutDescription::~JsonLayoutDescription ()
  {
  }


  string JsonLayoutDescription::getTypeName () const
  { return "Cfg::LayoutDescription"; }


  JsonLayoutDescription* JsonLayoutDescription::clone ( unsigned long flags ) const
  { return new JsonLayoutDescription ( flags ); }


  void JsonLayoutDescription::toData ( JsonStack& stack )
  {
    check( stack, "JsonLayoutDescription::toData" );

    LayoutDescription* layout = &Configuration::get()->getLayout();

    update( stack, layout );
  }


}  // Hurricane namespace.
