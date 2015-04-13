// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
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
#include <QXmlStreamReader>
#include "crlcore/Utilities.h"
#include "crlcore/XmlParser.h"
#include "crlcore/Environment.h"


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

  const string  sysConfDir = SYS_CONF_DIR;


  class XmlEnvironmentParser : public XmlParser {

    // Methods.
    public:
      static  XmlEnvironmentParser* create               ( Environment&      env
                                                         , QXmlStreamReader* reader );
      static  bool                  load                 ( Environment&      env
                                                         , const string&     path
                                                         , bool              warnNotFound=true );

    // Internal - enum.
    protected:
      enum  TagSet { TagsEnvironment  =  2
                   , TagsVariable
                   , TagsTechnologies
                   , TagsHurricane
                   , TagsReal
                   , TagsDisplay
                   , TagsConfig
                   , TagsLibraries
                   , TagsCatalog
                   , TagsWorking
                   , TagsSystem
                   , TagsLibrary
                   , TagsFormats
                   , TagsScale
                   , TagsInput
                   , TagsOutput
                   , TagsLogical
                   , TagsPhysical
                   , TagsSignals
                   , TagsPOWER
                   , TagsGROUND
                   , TagsClock
                   , TagsBlockage
                   , TagsTableSize
                   };
      enum State   { HurricaneTechnology = 1 
                   , RealTechnology
                   , Display
                   , WorkingLibrary
                   , SystemLibrary
                   , InputFormats
                   , OutputFormats
                   };

    // Internal - Attributes.
              Environment&          _environment;
              map<string,string>    _variables;
              unsigned int          _state;

    // Internal - Constructors.
                                    XmlEnvironmentParser ( Environment&      env
                                                         , QXmlStreamReader* reader=NULL );
                                    XmlEnvironmentParser ( const XmlEnvironmentParser& );
              XmlEnvironmentParser& operator=            ( const XmlEnvironmentParser& );
      
    // Internal - Methods.
              const string&         getVariable          ( const string& name );
              void                  setVariable          ( const string& name, const string& value );
              string&               expandVariables      ( string& value );
              void                  parseEnvironment     ();
              void                  parseVariable        ();
              void                  parseTechnologies    ();
              void                  parseHurricane       ();
              void                  parseReal            ();
              void                  parseDisplay         ();
              void                  parseDisplayStyle    ();
              void                  parseConfig          ();
              void                  parseLibraries       ();
              void                  parseCatalog         ();
              void                  parseWorking         ();
              void                  parseSystem          ();
              void                  parseLibrary         ();
              void                  parseFormats         ();
              void                  parseScale           ();
              void                  parseInput           ();
              void                  parseOutput          ();
              void                  parseLogical         ();
              void                  parsePhysical        ();
              void                  parseSignals         ();
              void                  parsePOWER           ();
              void                  parseGROUND          ();
              void                  parseClock           ();
              void                  parseBlockage        ();
      virtual void                  _postLoad            ();
      virtual const char*           _getMessage          ( MessageId id );

  };



  XmlEnvironmentParser::XmlEnvironmentParser ( Environment& env, QXmlStreamReader* reader )
    : XmlParser(reader,TagsTableSize)
    , _environment(env)
    , _state(0)
  {
    addTagEntry ( TagsStandAlone  , "environment" , (tagParser_t)&XmlEnvironmentParser::parseEnvironment  );
    addTagEntry ( TagsEnvironment , "variable"    , (tagParser_t)&XmlEnvironmentParser::parseVariable     );

    addTagEntry ( TagsEnvironment , "technologies", (tagParser_t)&XmlEnvironmentParser::parseTechnologies );
    addTagEntry ( TagsTechnologies, "hurricane"   , (tagParser_t)&XmlEnvironmentParser::parseHurricane    );
    addTagEntry ( TagsHurricane   , "config"      , (tagParser_t)&XmlEnvironmentParser::parseConfig       );
    addTagEntry ( TagsTechnologies, "real"        , (tagParser_t)&XmlEnvironmentParser::parseReal         );
    addTagEntry ( TagsReal        , "config"      , (tagParser_t)&XmlEnvironmentParser::parseConfig       );
    addTagEntry ( TagsTechnologies, "display"     , (tagParser_t)&XmlEnvironmentParser::parseDisplay      );
    addTagEntry ( TagsDisplay     , "config"      , (tagParser_t)&XmlEnvironmentParser::parseConfig       );
    addTagEntry ( TagsDisplay     , "displaystyle", (tagParser_t)&XmlEnvironmentParser::parseDisplayStyle );

    addTagEntry ( TagsEnvironment , "libraries"   , (tagParser_t)&XmlEnvironmentParser::parseLibraries    );
    addTagEntry ( TagsLibraries   , "catalog"     , (tagParser_t)&XmlEnvironmentParser::parseCatalog      );
    addTagEntry ( TagsLibraries   , "working"     , (tagParser_t)&XmlEnvironmentParser::parseWorking      );
    addTagEntry ( TagsWorking     , "library"     , (tagParser_t)&XmlEnvironmentParser::parseLibrary      );
    addTagEntry ( TagsLibraries   , "system"      , (tagParser_t)&XmlEnvironmentParser::parseSystem       );
    addTagEntry ( TagsSystem      , "library"     , (tagParser_t)&XmlEnvironmentParser::parseLibrary      );

    addTagEntry ( TagsEnvironment , "formats"     , (tagParser_t)&XmlEnvironmentParser::parseFormats      );
    addTagEntry ( TagsFormats     , "scale"       , (tagParser_t)&XmlEnvironmentParser::parseScale        );
    addTagEntry ( TagsFormats     , "input"       , (tagParser_t)&XmlEnvironmentParser::parseInput        );
    addTagEntry ( TagsInput       , "logical"     , (tagParser_t)&XmlEnvironmentParser::parseLogical      );
    addTagEntry ( TagsInput       , "physical"    , (tagParser_t)&XmlEnvironmentParser::parsePhysical     );
    addTagEntry ( TagsFormats     , "output"      , (tagParser_t)&XmlEnvironmentParser::parseOutput       );
    addTagEntry ( TagsOutput      , "logical"     , (tagParser_t)&XmlEnvironmentParser::parseLogical      );
    addTagEntry ( TagsOutput      , "physical"    , (tagParser_t)&XmlEnvironmentParser::parsePhysical     );

    addTagEntry ( TagsEnvironment , "signals"     , (tagParser_t)&XmlEnvironmentParser::parseSignals      );
    addTagEntry ( TagsSignals     , "vdd"         , (tagParser_t)&XmlEnvironmentParser::parsePOWER        );
    addTagEntry ( TagsSignals     , "vss"         , (tagParser_t)&XmlEnvironmentParser::parseGROUND       );
    addTagEntry ( TagsSignals     , "clock"       , (tagParser_t)&XmlEnvironmentParser::parseClock        );
    addTagEntry ( TagsSignals     , "blockage"    , (tagParser_t)&XmlEnvironmentParser::parseBlockage     );

    setVariable ( "CORIOLIS_TOP", _environment.getCORIOLIS_TOP() );
  }


  XmlEnvironmentParser* XmlEnvironmentParser::create ( Environment&      env
                                                     , QXmlStreamReader* reader )
  {
    return new XmlEnvironmentParser ( env, reader );
  }


  bool  XmlEnvironmentParser::load ( Environment& env, const string& path, bool warnNotFound  )
  {
    XmlEnvironmentParser  ep ( env );

    string  envPath = path;
    if ( path.empty() ) {
      if ( sysConfDir[0] == '/' )
        envPath = sysConfDir + "/coriolis2/environment.alliance.xml" ;
      else
        envPath = env.getCORIOLIS_TOP() + "/" + sysConfDir + "/coriolis2/environment.alliance.xml" ;
    }

    return ep._load ( envPath, warnNotFound );
  }


  const string& XmlEnvironmentParser::getVariable ( const string& name )
  {
    static const string nullValue = "";

    map<string,string>::iterator it = _variables.find ( name );
    if ( it != _variables.end() )
      return it->second;

    return nullValue;
  }


  void  XmlEnvironmentParser::setVariable ( const string& name, const string& value )
  {
    if ( !getVariable(name).empty() )
      cerr << "[WARNING] Overriding variable \"" << name << "\".\n" << endl;

    _variables [ name ] = value;
  }


  string& XmlEnvironmentParser::expandVariables ( string& variable )
  {
    size_t  dollar;
    size_t  vbegin;
    size_t  vend;
    bool    braces = false;

    while ( (dollar = variable.find('$')) != string::npos ) {
      if ( variable[dollar+1] == '{' ) {
        braces = true;
        vbegin = dollar+2;
        vend   = variable.find ( '}', vbegin );
        if ( vend == string::npos ) {
          cerr << "[ERROR] Unmatched brace while expanding variables." << endl;
          break;
        }
      } else {
        vbegin = dollar+1;
        for ( vend = vbegin ; vend < variable.size() ; vend++ )
          if ( !isalnum(variable[vend]) ) break;
      }

      const string& substitute = getVariable ( variable.substr(vbegin,vend-vbegin) );
      if ( substitute.empty() ) {
        cerr << "[ERROR] Reference to undefined variable \"" 
             << variable.substr(vbegin,vend-vbegin+(braces?1:0)) << "\"." << endl;
        variable [ dollar ] = '_';
        continue;
      }

      variable.replace ( dollar, vend-dollar+(braces?1:0), substitute );
    }

    return variable;
  }


  void  XmlEnvironmentParser::parseEnvironment ()
  {
    parseTags ( TagsEnvironment );
  }


  void  XmlEnvironmentParser::parseVariable ()
  {
    QString  value;
    QString  name;

    name = _reader->attributes().value("name").toString();
    if ( name.isEmpty() )
      cerr << "[ERROR] Unnamed variable." << endl;
    else {
      value = _reader->attributes().value("value").toString();
      if ( value.isEmpty() )
        cerr << "[ERROR] variable \"" << qPrintable(name) << "\" has no value." << endl;
      else
        setVariable ( name.toStdString(), value.toStdString() );
    }

    parseNoChilds ();
  }


  void  XmlEnvironmentParser::parseTechnologies ()
  {
    parseTags ( TagsTechnologies );
  }


  void  XmlEnvironmentParser::parseHurricane ()
  {
    _state = HurricaneTechnology;
    parseTags ( TagsHurricane );
  }


  void  XmlEnvironmentParser::parseReal ()
  {
    _state = RealTechnology;
    parseTags ( TagsReal );
  }


  void  XmlEnvironmentParser::parseDisplay ()
  {
    _state = Display;
    parseTags ( TagsDisplay );
  }


  void  XmlEnvironmentParser::parseDisplayStyle ()
  {
    QString value;

    value = _reader->attributes().value("name").toString();
    if ( !value.isEmpty() )
      _environment.setDisplayStyle ( value.toStdString().c_str() );
    else
      cerr << "[ERROR] Unnamed display style." << endl;

    parseNoChilds ();
  }


  void  XmlEnvironmentParser::parseConfig ()
  {
#if THIS_IS_DISABLED
    string config = readTextAsString().toStdString();
    expandVariables ( config );
    switch ( _state ) {
      case HurricaneTechnology: _environment.setSYMBOLIC_TECHNOLOGY ( config.c_str() ); break;
      case RealTechnology:      _environment.setREAL_TECHNOLOGY     ( config.c_str() ); break;
      case Display:             _environment.setDISPLAY             ( config.c_str() ); break;
    }
#endif
  }


  void  XmlEnvironmentParser::parseLibraries ()
  {
    parseTags ( TagsLibraries );
  }


  void  XmlEnvironmentParser::parseCatalog ()
  {
    _environment.setCATALOG ( readTextAsString().toStdString().c_str() );
  }


  void  XmlEnvironmentParser::parseWorking ()
  {
    _state = WorkingLibrary;
    parseTags ( TagsWorking );
  }


  void  XmlEnvironmentParser::parseSystem ()
  {
    _state = SystemLibrary;

    QString  operation;

    // operation = _reader->attributes().value("operation").toString();
    // if ( operation.isEmpty() )
    //   _environment.getLIBRARIES().reset();
    // else if ( operation != "append" )
    //   cerr << "[ERROR] Invalid value for attribute \"operation\" of <system>: \""
    //        << qPrintable(operation) << "\"." << endl;

    parseTags ( TagsSystem );
  }


  void  XmlEnvironmentParser::parseLibrary ()
  {
    unsigned int mode          = Environment::Append;
    QString      modeAttribute = _reader->attributes().value("mode").toString();
    QString      nameAttribute = _reader->attributes().value("name").toString();

    if ( not modeAttribute.isEmpty() ) {
      if      ( modeAttribute == "append"  ) mode = Environment::Append;
      else if ( modeAttribute == "prepend" ) mode = Environment::Prepend;
      else if ( modeAttribute == "replace" ) mode = Environment::Replace;
      else
        cerr << "[ERROR] Invalid value for attribute \"mode\" of <library>: \""
             << qPrintable(modeAttribute) << "\"." << endl;
    }

    string library   = readTextAsString().toStdString();
    expandVariables ( library );

    string libName = nameAttribute.toStdString();
    if ( libName.empty() )  libName = SearchPath::extractLibName ( library );

    switch ( _state ) {
      case WorkingLibrary: _environment.setWORKING_LIBRARY ( library.c_str() ); break;
      case SystemLibrary:  _environment.addSYSTEM_LIBRARY  ( library.c_str(), libName.c_str(), mode ); break;
    }
  }


  void  XmlEnvironmentParser::parseFormats ()
  {
    parseTags ( TagsFormats );
  }


  void  XmlEnvironmentParser::parseScale ()
  {
    _environment.setSCALE_X ( readTextAsLong() );
  }


  void  XmlEnvironmentParser::parseInput ()
  {
    _state = InputFormats;
    parseTags ( TagsInput );
  }


  void  XmlEnvironmentParser::parseOutput ()
  {
    _state = OutputFormats;
    parseTags ( TagsOutput );
  }


  void  XmlEnvironmentParser::parseLogical ()
  {
    string format = readTextAsString().toStdString();
    switch ( _state ) {
      case InputFormats:  _environment.setIN_LO  ( format.c_str() ); break;
      case OutputFormats: _environment.setOUT_LO ( format.c_str() ); break;
    }
  }


  void  XmlEnvironmentParser::parsePhysical ()
  {
    string format = readTextAsString().toStdString();
    switch ( _state ) {
      case InputFormats:  _environment.setIN_PH  ( format.c_str() ); break;
      case OutputFormats: _environment.setOUT_PH ( format.c_str() ); break;
    }
  }


  void  XmlEnvironmentParser::parseSignals ()
  {
    parseTags ( TagsSignals );
  }


  void  XmlEnvironmentParser::parsePOWER ()
  {
    _environment.setPOWER ( readTextAsString().toStdString().c_str() );
  }


  void  XmlEnvironmentParser::parseGROUND ()
  {
    _environment.setGROUND ( readTextAsString().toStdString().c_str() );
  }


  void  XmlEnvironmentParser::parseClock ()
  {
    _environment.setCLOCK ( readTextAsString().toStdString().c_str() );
  }


  void  XmlEnvironmentParser::parseBlockage ()
  {
    _environment.setBLOCKAGE ( readTextAsString().toStdString().c_str() );
  }


  void  XmlEnvironmentParser::_postLoad ()
  {
  }


  const char* XmlEnvironmentParser::_getMessage ( MessageId id )
  {
    const char* message = "<unknwown message id>";
    switch ( id ) {
      case OpenFile: message = "environment"; break;
    }

    return message;
  }


} // End of anonymous namespace.




namespace CRL {


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

    _LIBRARIES.append ( "." );

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
    if ( mode == Prepend ) { _LIBRARIES.prepend(value,libName); return; }
    if ( mode == Append  ) { _LIBRARIES.append (value,libName); return; }

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


} // End of CRL namespace.
