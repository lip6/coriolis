// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Environnment.cpp"                            |
// +-----------------------------------------------------------------+


#include <cctype>
#include <cstdlib>
#include <map>
#include <iomanip>
#include "crlcore/Utilities.h"
#include "crlcore/Environment.h"
#include "crlcore/AllianceFramework.h"


namespace {


  using namespace CRL;


  const char* badRegex =
    "Environment::Environment() :\n"
    "  Invalid regular expression for %s :\n"
    "    %s.\n";
  const char* badSCALE_X =
    "Environment::Environment() :\n"
    "    Only three values are allowed for SCALE : 1, 10 & 100.\n"
    "    (current value := %ld)";
  const char* badEnvironment =
    "Environment::Environment() :\n"
    "    %s logical format \"%s\" incoherent with physical format \"%s\".\n";


} // End of anonymous namespace.


namespace CRL {

  using namespace std;
  using Hurricane::Initializer;
  using Hurricane::JsonTypes;


// -------------------------------------------------------------------
// Class  :  "Environment".

  Environment::Environment ()
    : _CORIOLIS_TOP       (CORIOLIS_TOP)
    , _displayStyle       ()
    , _SCALE_X            (10)
    , _IN_LO              ("vst")
    , _IN_PH              ("ap")
    , _OUT_LO             ("vst")
    , _OUT_PH             ("ap")
    , _CATALOG            ("CATAL")
    , _inConstructor      (true)
  {
    setPOWER    ( "vdd" );
    setGROUND   ( "vss" );
    setCLOCK    ( "^ck$" );
    setBLOCKAGE ( "^obs$" );
    setPad      ( "^.*_px$" );

    _LIBRARIES.append ( ".", "working" );

    _inConstructor = false;
  }


  Environment::~Environment ()
  {
    regfree ( &_PowerRegex    );
    regfree ( &_GroundRegex   );
    regfree ( &_ClockRegex    );
    regfree ( &_BlockageRegex );
    regfree ( &_padRegex      );
  }


  bool  Environment::isPOWER ( const char* name ) const
  {
    return regexec ( &_PowerRegex, name, 0, NULL, 0 ) == 0;
  }


  bool  Environment::isGROUND ( const char* name ) const
  {
    return regexec ( &_GroundRegex, name, 0, NULL, 0 ) == 0;
  }


  bool  Environment::isCLOCK ( const char* name ) const
  {
    return regexec ( &_ClockRegex, name, 0, NULL, 0 ) == 0;
  }


  bool  Environment::isBLOCKAGE ( const char* name ) const
  {
    return regexec ( &_BlockageRegex, name, 0, NULL, 0 ) == 0;
  }


  bool  Environment::isPad ( const char* name ) const
  {
    return regexec ( &_padRegex, name, 0, NULL, 0 ) == 0;
  }


  void  Environment::setPOWER ( const char* value )
  {
    _POWER = value;
    _setRegex ( &_PowerRegex , _POWER , "Power" );
  }


  void  Environment::setGROUND ( const char* value )
  {
    _GROUND = value;
    _setRegex ( &_GroundRegex , _GROUND , "Ground" );
  }


  void  Environment::setCLOCK ( const char* value )
  {
    _CLOCK = value;
    _setRegex ( &_ClockRegex , _CLOCK , "Clock" );
  }


  void  Environment::setBLOCKAGE ( const char* value )
  {
    _BLOCKAGE = value;
    _setRegex ( &_BlockageRegex , _BLOCKAGE , "Blockage" );
  }


  void  Environment::setPad ( const char* value )
  {
    _pad = value;
    _setRegex ( &_padRegex , _pad , "Pad" );
  }


  string  Environment::getPrint () const
  {
    ostringstream s;

    s << "\n"
      << "  o  Hurricane/Alliance environment.\n";

    s << "     o  Libraries.\n"
      << "        - Catalog:  \"" << _CATALOG << "\"\n"
      << "        - Working Library:\n"
      << "          [ 0]:\"" << _LIBRARIES[0].getPath() << "\"\n"
      << "        - System Libraries:\n";

    if ( _LIBRARIES.getSize() <= 1 ) {
      s << "          <not set or empty>.\n";
    } else {
      for ( size_t i = 1; i < _LIBRARIES.getSize() ; i++ ) {
        s << "          [" << setw(2) << i << "]:\"" << _LIBRARIES[i].getPath() << "\"\n";
      }
    }

    s << "     o  I/O Formats.\n" 
      << Dots::asULong ( "        - Scale", _SCALE_X ) << "\n"
      << Dots::asString( "        - Input, logical"  , _IN_LO    )  << "\n"
      << Dots::asString( "        - Input, physical" , _IN_PH    )  << "\n"
      << Dots::asString( "        - Output, logical" , _OUT_LO   )  << "\n"
      << Dots::asString( "        - Output, physical", _OUT_PH   )  << "\n"
      << "     o  Special Signals.\n"
      << Dots::asString( "        - Power Signal"    , _POWER    )  << "\n"
      << Dots::asString( "        - Ground Signal"   , _GROUND   )  << "\n"
      << Dots::asString( "        - Clock Signal"    , _CLOCK    )  << "\n"
      << Dots::asString( "        - Blockages"       , _BLOCKAGE )  << "\n"
      << "     o  Special Cells.\n"
      << Dots::asString( "        - Pads"            , _pad      )  << "\n\n";

    return s.str();
  }


  void  Environment::_setRegex ( regex_t* regex, const string& pattern, const char* name )
  {
    char regexError[1024];
    int  regexCode;

    if ( !_inConstructor ) regfree ( regex );

    if ( ( regexCode = regcomp(regex,getString(pattern).c_str(),REG_EXTENDED|REG_NOSUB) ) ) {
      regerror ( regexCode, regex, regexError, 1024 );
      throw Error ( badRegex, name, regexError );
    }
  }


  void  Environment::validate () const
  {
    switch ( _SCALE_X ) {
      case 1:   break;
      case 10:  break;
      case 100: break;
      default:
        throw Error ( badSCALE_X , _SCALE_X );
    }

    bool  coherency = false;
    if      (  ( _IN_LO == "vst"   ) && ( _IN_PH == "ap" ) ) coherency = true;
    else if (  ( _IN_LO == "spi"   ) && ( _IN_PH == "ap" ) ) coherency = true;
    else if (  ( _IN_LO == "bench" ) && ( _IN_PH == "ap" ) ) coherency = true;
    else if (  ( _IN_LO == "def"   ) && ( _IN_PH == "def") ) coherency = true;
    else if (  ( _IN_LO == "aux"   ) && ( _IN_PH == "aux") ) coherency = true;
    else if (  ( _IN_LO == "oa"    ) && ( _IN_PH == "oa" ) ) coherency = true;
    if ( !coherency )
      throw Error ( badEnvironment, "Input", _IN_LO.c_str(), _IN_PH.c_str() );

    coherency = false;
    if      (  ( _OUT_LO == "vst" ) && ( _OUT_PH == "ap" ) ) coherency = true;
    else if (  ( _OUT_LO == "spi" ) && ( _OUT_PH == "ap" ) ) coherency = true;
    else if (  ( _OUT_LO == "vst" ) && ( _OUT_PH == "gds") ) coherency = true;
    else if (  ( _OUT_LO == "def" ) && ( _OUT_PH == "def") ) coherency = true;
    else if (  ( _OUT_LO == "aux" ) && ( _OUT_PH == "aux") ) coherency = true;
    else if (  ( _OUT_LO == "oa"  ) && ( _OUT_PH == "oa" ) ) coherency = true;
    if ( !coherency )
      throw Error ( badEnvironment, "Output", _OUT_LO.c_str(), _OUT_PH.c_str() );
  }


  const char* Environment::getEnv ( const char* variable, const char* defaultValue )
  {
    char *value;

    value = getenv ( variable );
    if ( value == NULL ) return defaultValue;

    return value;
  }


  void  Environment::addSYSTEM_LIBRARY ( const char* value, const char* libName, unsigned int mode )
  {
    if ((mode == Prepend) or (mode == Append)) {
      size_t duplicate = _LIBRARIES.hasLib(libName);
      if (duplicate != SearchPath::npos) _LIBRARIES.remove( duplicate );

      if (mode == Prepend) _LIBRARIES.prepend(value,libName);
      if (mode == Append ) _LIBRARIES.append (value,libName);
      return;
    }

    string newLibName = libName;
    for ( size_t i=0 ; i < _LIBRARIES.getSize() ; ++i ) {
      if ( newLibName == _LIBRARIES[i].getName() ) {
        _LIBRARIES.replace ( value, newLibName, i );
        return;
      }
    }
    _LIBRARIES.append (value,libName);
  }


  string  Environment::getLIBRARYPath ( size_t i )
  {
    if (i >= _LIBRARIES.getSize()) return "";
    return _LIBRARIES[i].getPath();
  }


  string  Environment::_getString () const
  { return "<Environment>"; }


  Record *Environment::_getRecord () const
  {
    Record* record = new Record ( "<Environment>" );
    record->add ( getSlot ( "_CORIOLIS_TOP"       , &_CORIOLIS_TOP        ) );
    record->add ( getSlot ( "_displayStyle"       , &_displayStyle        ) );
    record->add ( getSlot ( "_SCALE_X"            , &_SCALE_X             ) );
    record->add ( getSlot ( "_IN_LO"              , &_IN_LO               ) );
    record->add ( getSlot ( "_IN_PH"              , &_IN_PH               ) );
    record->add ( getSlot ( "_OUT_LO"             , &_OUT_LO              ) );
    record->add ( getSlot ( "_OUT_PH"             , &_OUT_PH              ) );
    record->add ( getSlot ( "_POWER"              , &_POWER               ) );
    record->add ( getSlot ( "_GROUND"             , &_GROUND              ) );
    record->add ( getSlot ( "_CLOCK"              , &_CLOCK               ) );
    record->add ( getSlot ( "_BLOCKAGE"           , &_BLOCKAGE            ) );
    record->add ( getSlot ( "_pad"                , &_pad                 ) );
    record->add ( getSlot ( "_LIBRARIES"          , &_LIBRARIES           ) );
    return record;
  }


  void  Environment::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename"    , _getTypeName() );
    jsonWrite( w, "_CORIOLIS_TOP", _CORIOLIS_TOP  );
    jsonWrite( w, "_displayStyle", _displayStyle  );
    jsonWrite( w, "_SCALE_X"     , _SCALE_X       );
    jsonWrite( w, "_IN_LO"       , _IN_LO         );
    jsonWrite( w, "_IN_PH"       , _IN_PH         );
    jsonWrite( w, "_OUT_LO"      , _OUT_LO        );
    jsonWrite( w, "_OUT_PH"      , _OUT_PH        );
    jsonWrite( w, "_POWER"       , _POWER         );
    jsonWrite( w, "_GROUND"      , _GROUND        );
    jsonWrite( w, "_CLOCK"       , _CLOCK         );
    jsonWrite( w, "_BLOCKAGE"    , _BLOCKAGE      );
    jsonWrite( w, "_pad"         , _pad           );
    jsonWrite( w, "_CATALOG"     , _CATALOG       );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonEnvironment".

  Initializer<JsonEnvironment>  jsonEnvironmentInit ( 0 );


  void  JsonEnvironment::initialize ()
  { JsonTypes::registerType( new JsonEnvironment (JsonWriter::RegisterMode) ); }


  JsonEnvironment::JsonEnvironment ( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_CORIOLIS_TOP", typeid(string)  );
    add( "_displayStyle", typeid(string)  );
    add( "_SCALE_X"     , typeid(int64_t) );
    add( "_IN_LO"       , typeid(string)  );
    add( "_IN_PH"       , typeid(string)  );
    add( "_OUT_LO"      , typeid(string)  );
    add( "_OUT_PH"      , typeid(string)  );
    add( "_POWER"       , typeid(string)  );
    add( "_GROUND"      , typeid(string)  );
    add( "_BLOCKAGE"    , typeid(string)  );
    add( "_pad"         , typeid(string)  );
    add( "_CATALOG"     , typeid(string)  );
    add( "_CLOCK"       , typeid(string)  );
  }

  string  JsonEnvironment::getTypeName () const
  { return "Environment"; }


  JsonEnvironment* JsonEnvironment::clone ( unsigned long flags ) const
  { return new JsonEnvironment ( flags ); }


  void JsonEnvironment::toData ( JsonStack& stack )
  {
    check( stack, "JsonEnvironment::toData" );

    Environment* environement = AllianceFramework::get()->getEnvironment();
    environement->setDisplayStyle( get<string> (stack,"_displayStyle").c_str() );
    environement->setSCALE_X     ( get<int64_t>(stack,"_SCALE_X"     )         );
    environement->setIN_LO       ( get<string> (stack,"_IN_LO"       ).c_str() );
    environement->setIN_PH       ( get<string> (stack,"_IN_PH"       ).c_str() );
    environement->setOUT_LO      ( get<string> (stack,"_OUT_LO"      ).c_str() );
    environement->setOUT_PH      ( get<string> (stack,"_OUT_PH"      ).c_str() );
    environement->setPOWER       ( get<string> (stack,"_POWER"       ).c_str() );
    environement->setGROUND      ( get<string> (stack,"_GROUND"      ).c_str() );
    environement->setCLOCK       ( get<string> (stack,"_CLOCK"       ).c_str() );
    environement->setBLOCKAGE    ( get<string> (stack,"_BLOCKAGE"    ).c_str() );
    environement->setPad         ( get<string> (stack,"_pad"         ).c_str() );
    environement->setCATALOG     ( get<string> (stack,"_CATALOG"     ).c_str() );

    update( stack, environement );
  }


} // End of CRL namespace.
