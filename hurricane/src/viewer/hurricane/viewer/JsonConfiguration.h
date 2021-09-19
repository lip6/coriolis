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
// |  C++ Header  :       "./hurricane/viewer/JsonConfiguration.h"   |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/Parameter.h"
#include "hurricane/configuration/Configuration.h"
#include "hurricane/Commons.h"


void  jsonWrite ( JsonWriter*, const Cfg::Parameter* );
void  jsonWrite ( JsonWriter*, const Cfg::Parameter::EnumValue );
void  jsonWrite ( JsonWriter*, const Cfg::Configuration* );
void  jsonWrite ( JsonWriter*, const Cfg::WidgetDescription* );
void  jsonWrite ( JsonWriter*, const Cfg::TabDescription* );
void  jsonWrite ( JsonWriter*, const Cfg::LayoutDescription* );


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "JsonParameter".

  class JsonParameter : public JsonObject {
    public:
      static void            initialize    ();
                             JsonParameter ( unsigned long );
      virtual string         getTypeName   () const;
      virtual JsonParameter* clone         ( unsigned long ) const;
      virtual void           toData        ( JsonStack& ); 
  };


// -------------------------------------------------------------------
// Class  :  "JsonParamEnumValue".

  class JsonParamEnumValue : public JsonObject {
    public:
      static void                 initialize         ();
                                  JsonParamEnumValue ( unsigned long );
      virtual string              getTypeName        () const;
      virtual JsonParamEnumValue* clone              ( unsigned long ) const;
      virtual void                toData             ( JsonStack& ); 
  };


// -------------------------------------------------------------------
// Class  :  "JsonParamSlave".

  class JsonParamSlave : public JsonObject {
    public:
      static void             initialize     ();
                              JsonParamSlave ( unsigned long );
      virtual string          getTypeName    () const;
      virtual JsonParamSlave* clone          ( unsigned long ) const;
      virtual void            toData         ( JsonStack& ); 
  };


// -------------------------------------------------------------------
// Class  :  "JsonConfiguration".

  class JsonConfiguration : public JsonObject {
    public:
      static void                initialize        ();
                                 JsonConfiguration ( unsigned long );
      virtual string             getTypeName       () const;
      virtual JsonConfiguration* clone             ( unsigned long ) const;
      virtual void               toData            ( JsonStack& ); 
  };


// -------------------------------------------------------------------
// Class  :  "JsonWidgetDescription".

  class JsonWidgetDescription : public JsonObject {
    public:
      static void                    initialize            ();
                                     JsonWidgetDescription ( unsigned long );
      virtual string                 getTypeName           () const;
      virtual JsonWidgetDescription* clone                 ( unsigned long ) const;
      virtual void                   toData                ( JsonStack& ); 
  };


// -------------------------------------------------------------------
// Class  :  "JsonTabDescription".

  class JsonTabDescription : public JsonObject {
    public:
      static void                 initialize         ();
                                  JsonTabDescription ( unsigned long );
      virtual string              getTypeName        () const;
      virtual JsonTabDescription* clone              ( unsigned long ) const;
      virtual void                toData             ( JsonStack& ); 
  };


// -------------------------------------------------------------------
// Class  :  "JsonLayoutDescription".

  class JsonLayoutDescription : public JsonObject {
    public:
      static void                    initialize            ();
                                     JsonLayoutDescription ( unsigned long );
      virtual                       ~JsonLayoutDescription ();
      virtual string                 getTypeName           () const;
      virtual JsonLayoutDescription* clone                 ( unsigned long ) const;
      virtual void                   toData                ( JsonStack& ); 
  };


}  // Hurricane namespace.
