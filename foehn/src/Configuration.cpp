// -*- mode: C++; explicit-buffer-name: "Configuration.cpp<foehn>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Configuration.cpp"                           |
// +-----------------------------------------------------------------+


#include <iostream>
#include <iomanip>
#include <vector>
#include "hurricane/configuration/Configuration.h"
#include "hurricane/Warning.h"
#include "hurricane/Error.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "foehn/Configuration.h"


namespace Foehn {

  using  std::cout;
  using  std::cerr;
  using  std::endl;
  using  std::string;
  using  std::setprecision;
  using  std::ostringstream;
  using  std::vector;
  using  Hurricane::tab;
  using  Hurricane::Warning;
  using  Hurricane::Error;


// -------------------------------------------------------------------
// Class  :  "Foehn::Configuration".


  Configuration::Configuration ()
    : _dffPattern             ( Cfg::getParamString("foehn.dffPattern", "^sff.*")->asString() )
    , _ignoredNetPattern      ( Cfg::getParamString("foehn.ignoredNetPattern", "^ck.*")->asString() )
    , _ignoredMasterNetPattern( Cfg::getParamString("foehn.ignoredMasterNetPattern", "^$")->asString() )
    , _dffRe             (new regex_t)
    , _ignoredNetRe      (new regex_t)
    , _ignoredMasterNetRe(new regex_t)
  {
    setDffRe             ( _dffPattern );
    setIgnoredNetRe      ( _ignoredNetPattern );
    setIgnoredMasterNetRe( _ignoredMasterNetPattern );
  }


  Configuration::Configuration ( const Configuration& other )
    : _dffPattern             (other._dffPattern)
    , _ignoredNetPattern      (other._ignoredNetPattern)
    , _ignoredMasterNetPattern(other._ignoredMasterNetPattern)
    , _dffRe             (new regex_t)
    , _ignoredNetRe      (new regex_t)
    , _ignoredMasterNetRe(new regex_t)
  {
    setDffRe             ( _dffPattern );
    setIgnoredNetRe      ( _ignoredNetPattern );
    setIgnoredMasterNetRe( _ignoredMasterNetPattern );
  }


  Configuration::~Configuration ()
  {
    regfree( _dffRe );
    regfree( _ignoredNetRe );
    regfree( _ignoredMasterNetRe );
  }


  Configuration* Configuration::clone () const
  { return new Configuration(*this); }


  void  Configuration::_setRegex ( regex_t*& regex, string pattern, string attr )
  {
    char regexError[1024];
    int  regexCode;

    if ((regexCode = regcomp(regex,pattern.c_str(),REG_EXTENDED|REG_NOSUB))) { 
      regerror( regexCode, regex, regexError, 1024 );
      throw Error ( "Foehn::Configuration::_setRegex(): Invalid regular expresssion for \"%s\"\n"
                    "        (pattern=\"%s\" : %s)"
                  , attr, pattern, regexError );
    }   
  }

  
  void  Configuration::setDffRe ( string pattern )
  {
    _dffPattern = pattern;
    _setRegex( _dffRe , _dffPattern , "_dffPattern" );
  }

  
  void  Configuration::setIgnoredNetRe ( string pattern )
  {
    _ignoredNetPattern = pattern;
    _setRegex( _ignoredNetRe , _ignoredNetPattern , "_ignoredNetPattern" );
  }

  
  void  Configuration::setIgnoredMasterNetRe ( string pattern )
  {
    _ignoredMasterNetPattern = pattern;
    _setRegex( _ignoredMasterNetRe , _ignoredMasterNetPattern , "_ignoredNetPattern" );
  }
  

  bool  Configuration::isDff ( string name ) const
  {
    if (not _dffRe) return false;
    return regexec( _dffRe, name.c_str(), 0, NULL, 0 ) == 0;
  }
  

  bool  Configuration::isIgnoredNet ( string name ) const
  {
    if (not _ignoredNetRe) return false;
    return regexec( _ignoredNetRe, name.c_str(), 0, NULL, 0 ) == 0;
  }
  

  bool  Configuration::isIgnoredMasterNet ( string name ) const
  {
    if (not _ignoredMasterNetRe) return false;
    return regexec( _ignoredMasterNetRe, name.c_str(), 0, NULL, 0 ) == 0;
  }


  void  Configuration::print ( const Cell* cell ) const
  {
    if (not cmess1.enabled()) return;

    cout << "  o  Configuration of ToolEngine<Foehn> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asString("     - DFF pattern"                , _dffPattern             ) << endl;
    cout << Dots::asString("     - Ignored nets pattern"       , _ignoredNetPattern      ) << endl;
    cout << Dots::asString("     - Ignored master nets pattern", _ignoredMasterNetPattern) << endl;
  }


  string  Configuration::_getTypeName () const
  { return "Foehn::Configuration"; }


  string  Configuration::_getString () const
  {
    ostringstream  os;
    os << "<" << _getTypeName() << ">";
    return os.str();
  }


  Record* Configuration::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add( getSlot( "_dffPattern"             , _dffPattern              ));
    record->add( getSlot( "_ignoredNetPattern"      , _ignoredNetPattern       ));
    record->add( getSlot( "_ignoredMasterNetPattern", _ignoredMasterNetPattern ));
    return record;
  }


}  // Foehn namespace.
