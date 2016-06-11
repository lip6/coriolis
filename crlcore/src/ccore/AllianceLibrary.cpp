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
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./AllianceLibrary.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/Warning.h"
#include "hurricane/SharedPath.h"
#include "hurricane/Library.h"
#include "hurricane/DataBase.h"
#include "crlcore/Utilities.h"
#include "crlcore/AllianceLibrary.h"
#include "crlcore/AllianceFramework.h"


namespace CRL {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::Initializer;
  using Hurricane::JsonTypes;
  using Hurricane::Warning;
  using Hurricane::Name;
  using Hurricane::SharedPath;
  using Hurricane::DataBase;


// -------------------------------------------------------------------
// Class  :  "AllianceDirectory".

  AllianceLibrary::AllianceLibrary ()
    : _path("")
    , _library(NULL)
  {
  }


  AllianceLibrary::AllianceLibrary ( const Name& path, Library* library )
    : _path(path)
    , _library(library)
  {
    if ( _path.isEmpty() )
      throw ( Error("Can't create "+_TName("AllianceLibrary")+" : empty path") );
  }


  AllianceLibrary& AllianceLibrary::operator= ( const AllianceLibrary& d )
  {
    _path    = d._path;
    _library = d._library;
    return *this;
  }


  AllianceLibrary& AllianceLibrary::operator= ( const string& path )
  {
    _path    = path;
    _library = NULL;
    return *this;
  }


  string  AllianceLibrary::_getString () const
  {
    return ( "<" + _TName("AllianceLibrary") + " " + getString(_library->getName()) + ">" );
  }


  Record* AllianceLibrary::_getRecord() const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot("_path"   ,&_path   ) );
    record->add ( getSlot("_library", _library) );
    return ( record );
  }


  void  AllianceLibrary::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename", _getTypeName() );
    jsonWrite( w, "_path"    , _path  );
    jsonWrite( w, "_library" , _library->getHierarchicalName() );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonAllianceLibrary".

  Initializer<JsonAllianceLibrary>  jsonAllianceLibraryInit ( 0 );


  void  JsonAllianceLibrary::initialize ()
  { JsonTypes::registerType( new JsonAllianceLibrary (JsonWriter::RegisterMode) ); }


  JsonAllianceLibrary::JsonAllianceLibrary ( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_path"   , typeid(string) );
    add( "_library", typeid(string) );
  }

  string  JsonAllianceLibrary::getTypeName () const
  { return "AllianceLibrary"; }


  JsonAllianceLibrary* JsonAllianceLibrary::clone ( unsigned long flags ) const
  { return new JsonAllianceLibrary ( flags ); }


  void JsonAllianceLibrary::toData ( JsonStack& stack )
  {
    check( stack, "JsonAllianceLibrary::toData" );

    string libDbPath = get<string>( stack, "_library" );
    string libOsPath = get<string>( stack, "_path"    );

    AllianceFramework* af       = AllianceFramework::get();
    Library*           library  = DataBase::getDB()->getLibrary( libDbPath
                                                               , DataBase::CreateLib|DataBase::WarnCreateLib );
    AllianceLibrary*   aLibrary = NULL;

    cdebug_log(19,0) << "| " << libDbPath << " : " << library << endl;

    if (library) {
      aLibrary = af->getAllianceLibrary( library );
    }
    if (not aLibrary) {
      char     separator = SharedPath::getNameSeparator();
      size_t   dot       = libDbPath.rfind( separator );
      Name     libName   = libDbPath.substr(dot+1);
      aLibrary = af->getAllianceLibrary( libName, AllianceFramework::AppendLibrary );

      if (not aLibrary) {
        aLibrary = af->getAllianceLibrary( libOsPath
                                         , AllianceFramework::CreateLibrary
                                         | AllianceFramework::AppendLibrary );
      }
      if (not library) library = aLibrary->getLibrary();
    }

    cdebug_log(19,0) << "| Associates to: " << aLibrary << endl;

    if (aLibrary->getLibrary() != library) {
      cerr << Warning( "JsonAllianceLibrary::toData(): Underlying Hurricane Library discrepency for \"%s\".\n"
                       "          (may be caused by a change in search path order)"
                     , getString(library->getName()).c_str()
                     ) << endl;
    }
    if (aLibrary->getPath() != Name(libOsPath)) {
      cerr << Warning( "JsonAllianceLibrary::toData(): Underlying OS path discrepency for \"%s\":\n"
                       "          - Blob:   %s\n"
                       "          - System: %s"
                     , getString(library->getName()).c_str()
                     , libOsPath.c_str()
                     , getString(aLibrary->getPath()).c_str()
                     ) << endl;
    }

    update( stack, aLibrary );
  }



} // End of CRL namespace.
