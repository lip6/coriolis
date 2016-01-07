// -*- mode: C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2015, All Rights Reserved
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
#include "hurricane/DesignBlob.h"
#include "hurricane/DataBase.h"
#include "hurricane/Library.h"
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
    jsonWrite( w, "_library" , DataBase::getDB()->getRootLibrary() );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonDesignBlob".

  JsonDesignBlob::JsonDesignBlob ( unsigned long flags )
    : JsonObject(flags)
  {
    ltrace(51) << "JsonDesignblob::JsonDesignblob()" << endl;

    add( "_library", typeid(Library*) );
    add( "_topCell", typeid(string)   );
  }


  string JsonDesignBlob::getTypeName () const
  { return "DesignBlob"; }


  JsonDesignBlob* JsonDesignBlob::clone ( unsigned long flags ) const
  { return new JsonDesignBlob ( flags ); }


  void JsonDesignBlob::toData ( JsonStack& stack )
  {
    check( stack, "JsonDesignBlob::toData" );

    DesignBlob* designBlob = new DesignBlob ( DataBase::getDB()->getCell(get<string>(stack,"_topCell")) );
    
    update( stack, designBlob );
  }


} // Hurricane namespace.
