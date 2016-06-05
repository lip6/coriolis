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
// |  C++ Header  :       "./hurricane/viewer/JsonParameter.h"       |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_JSON_PARAMETER_H
#define  HURRICANE_JSON_PARAMETER_H

#include "vlsisapd/configuration/Parameter.h"
#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/Commons.h"


void  jsonWrite ( JsonWriter*, const Cfg::Parameter* );
void  jsonWrite ( JsonWriter*, const Cfg::Configuration* );

inline void  jsonWrite ( JsonWriter* w, const std::string& key, const Cfg::Parameter* p )
{ w->key(key); jsonWrite( w, p ); }

inline void  jsonWrite ( JsonWriter* w, const std::string& key, const Cfg::Configuration* cfg )
{ w->key(key); jsonWrite( w, cfg );  }


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
// Class  :  "JsonConfiguration".

  class JsonConfiguration : public JsonObject {
    public:
      static void                initialize        ();
                                 JsonConfiguration ( unsigned long );
      virtual string             getTypeName       () const;
      virtual JsonConfiguration* clone             ( unsigned long ) const;
      virtual void               toData            ( JsonStack& ); 
  };


}  // Hurricane namespace.

#endif  // HURRICANE_JSON_PARAMETER_H
