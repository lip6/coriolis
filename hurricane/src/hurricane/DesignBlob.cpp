// -*- mode: C++ -*-
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
// |  C++ Module  :  "./DesignBlob.cpp"                              |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/DataBase.h"
#include "hurricane/Cell.h"
#include "hurricane/DesignBlob.h"


namespace Hurricane {

  using std::cerr;
  using std::endl;


// -------------------------------------------------------------------
// Class  :  "DesignBlob".

  void DesignBlob::toJson( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename", _getTypeName() );
    jsonWrite( w, "_topCell" , getTopCell()->getHierarchicalName() );
    jsonWrite( w, "_database", DataBase::getDB()                   );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonDesignBlob".

  Initializer<JsonDesignBlob>  jsonDesignBlobInit ( 0 );


  void  JsonDesignBlob::initialize ()
  { JsonTypes::registerType( new JsonDesignBlob (JsonWriter::RegisterMode) ); }


  JsonDesignBlob::JsonDesignBlob ( unsigned long flags )
    : JsonObject(flags)
  {
    ltrace(51) << "JsonDesignblob::JsonDesignblob()" << endl;

    add( "_topCell" , typeid(string)    );
    add( "_database", typeid(DataBase*) );
  }


  string JsonDesignBlob::getTypeName () const
  { return "DesignBlob"; }


  JsonDesignBlob* JsonDesignBlob::clone ( unsigned long flags ) const
  { return new JsonDesignBlob ( flags ); }


  void JsonDesignBlob::toData ( JsonStack& stack )
  {
    check( stack, "JsonDesignBlob::toData" );

    DesignBlob* designBlob = new DesignBlob
      ( DataBase::getDB()->getCell( get<string>(stack,"_topCell"), DataBase::NoFlags ) );
    
    update( stack, designBlob );
  }


} // Hurricane namespace.
