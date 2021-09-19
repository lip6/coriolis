// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Parameter.cpp"                          |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/utilities/Path.h"
#include "hurricane/configuration/Parameter.h"
#include "hurricane/configuration/Configuration.h"


namespace Cfg {

  using std::cerr;
  using std::endl;
  using std::string;
  using std::ostringstream;
  using std::boolalpha;
  using std::hex;
  using std::vector;


  vector<Parameter::Priority>  Parameter::_defaultPriorities (1,Parameter::ApplicationBuiltin);


  Parameter::Priority  Parameter::pushDefaultPriority ( Parameter::Priority priority )
  {
    Priority previous = _defaultPriorities.back ();
    _defaultPriorities.push_back ( priority );
    return previous;
  }


  Parameter::Priority  Parameter::popDefaultPriority ()
  {
    Priority previous = _defaultPriorities.back ();
    if ( _defaultPriorities.size() > 1 )
      _defaultPriorities.pop_back ();
    return previous;
  }


  Parameter::Priority  Parameter::getDefaultPriority ()
  { return _defaultPriorities.back(); }


  string  Parameter::typeToString  ( Parameter::Type type )
  {
    switch ( type ) {
      case Unknown:    return "unknown";
      case String:     return "string";
      case Int:        return "int";
      case Double:     return "double";
      case Bool:       return "bool";
      case Percentage: return "percentage";
      case Enumerate:  return "enumerate";
    }
    return "unsupported";
  }


  Parameter::Type  Parameter::stringToType ( std::string stype )
  {
    if      (stype == "string"    ) return String;
    else if (stype == "int"       ) return Int;
    else if (stype == "double"    ) return Double;
    else if (stype == "bool"      ) return Bool;
    else if (stype == "percentage") return Percentage;
    else if (stype == "enumerate" ) return Enumerate;

    return Unknown;
  }


  string  Parameter::priorityToString  ( Parameter::Priority priority )
  {
    switch ( priority ) {
      case UseDefault:         return "UseDefault";
      case ApplicationBuiltin: return "ApplicationBuiltin";
      case ConfigurationFile:  return "ConfigurationFile";
      case UserFile:           return "UserFile";
      case CommandLine:        return "CommandLine";
      case Interactive:        return "Interactive";
    }
    return "UseDefault";
  }


  Parameter::Priority  Parameter::stringToPriority ( std::string spriority )
  {
    if      (spriority == "UseDefault"        ) return UseDefault;
    else if (spriority == "ApplicationBuiltin") return ApplicationBuiltin;
    else if (spriority == "ConfigurationFile" ) return ConfigurationFile;
    else if (spriority == "UserFile"          ) return UserFile;
    else if (spriority == "CommandLine"       ) return CommandLine;
    else if (spriority == "Interactive"       ) return Interactive;

    return UseDefault;
  }


  Parameter::Parameter ( std::string id
                       , Type        type
                       , std::string value
                       , Priority    priority
                       )
    : _id       (id)
    , _type     (type)
    , _value    (value)
    , _values   ()
    , _priority (priority)
    , _flags    (0)
    , _minInt   (0)
    , _maxInt   (0)
    , _minDouble(0.0)
    , _maxDouble(0.0)
    , _slaves   ()
    , _callbacks()
  {
    if ( type == Percentage ) {
      setPercentage ( asDouble() );
    }
    if ( priority == UseDefault ) _priority = getDefaultPriority();

  //cerr << "New " << (void*)this << " " << typeToString(_type) << " parameter " << _id << " value:" << _value << endl;
  }


  string  Parameter::asPercentageString () const
  {
    if ( (_type != Double) and (_type != Percentage) )
      cerr << "[ERROR] Accessing " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Percentage)<< " (type mismatch)." << endl;

    std::istringstream is ( _value ); double r; is >> r;
    std::ostringstream os; os << (r*100.0);

    return os.str();
  }
  

  bool  Parameter::asBool () const
  {
    if ( _type != Bool )
      cerr << "[ERROR] Accessing " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Bool)<< " (type mismatch)." << endl;

    std::istringstream s ( _value ); bool b; s >> std::boolalpha >> b; return b;
  }


  int  Parameter::asInt () const
  {
    if ( (_type != Int) and (_type != Enumerate) )
      cerr << "[ERROR] Accessing " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Int)<< " (type mismatch)." << endl;

    std::istringstream s ( _value ); int i; s >> i; return i;
  }


  double  Parameter::asDouble () const
  {
    if ( (_type != Double) and (_type != Percentage) )
      cerr << "[ERROR] Accessing " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Double)<< " (type mismatch)." << endl;

    std::istringstream s ( _value ); double d; s >> d; return d;
  }


  double  Parameter::asPercentage () const
  {
    if ( (_type != Double) and (_type != Percentage) )
      cerr << "[ERROR] Accessing " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Percentage)<< " (type mismatch)." << endl;

    std::istringstream s ( _value ); double r; s >> r; return r*100.0;
  }


  bool  Parameter::setRawString ( string s, Priority priority )
  {
    if ( not _updatePriority(priority) ) return false;
    return _doChange ( _flags|FromString, s, false, 0, 0.0 );
  }


  bool  Parameter::setString ( std::string s, Priority priority, unsigned int flags )
  {
    if ( not _updatePriority(priority) ) return false;

    flags |= _flags;
    if ( (flags & TypeCheck) and (_type != String) )
      cerr << "[ERROR] Parameter::setString(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(String)<< " (type mismatch)." << endl;

    return _doChange ( flags, s, false, 0, 0.0 );
  }


  bool  Parameter::setBool ( bool b, Priority priority )
  {
    if ( not _updatePriority(priority) ) return false;

    if ( _type != Bool )
      cerr << "[ERROR] Parameter::setBool(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Bool)<< " (type mismatch)." << endl;

    return _doChange ( _flags, "", b, 0, 0.0 );
  }


  bool  Parameter::setInt ( int i, Priority priority )
  {
    if ( not _updatePriority(priority) ) return false;

    if ( (_type != Int) and (_type != Enumerate)  )
      cerr << "[ERROR] Parameter::setInt(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Int)<< " (type mismatch)." << endl;

    return _doChange ( _flags, "", false, i, 0.0 );
  } 


  bool  Parameter::setDouble ( double d, Priority priority )
  {
    if ( not _updatePriority(priority) ) return false;

    if ( (_type != Double) and (_type != Percentage)  )
      cerr << "[ERROR] Parameter::setDouble(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Double)<< " (type mismatch)." << endl;

    return _doChange ( _flags, "", false, 0, d );
  } 


  bool  Parameter::setPercentage ( double d, Priority priority )
  {
    if ( not _updatePriority(priority) ) return false;

    if ( (_type != Double) and (_type != Percentage)  )
      cerr << "[ERROR] Parameter::setPercentage(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Double)<< " (type mismatch)." << endl;

    return _doChange ( _flags, "", false, 0, d );
  }


  void  Parameter::setMin ( int min, Parameter::Priority priority )
  { if (_updatePriority(priority)) { _minInt = min; setFlags(HasMin); } }
  

  void  Parameter::setMax ( int max, Parameter::Priority priority )
  { if (_updatePriority(priority)) { _maxInt = max; setFlags(HasMax); } }


  void  Parameter::setMin ( double min, Parameter::Priority priority )
  { if (_updatePriority(priority)) {
      _minDouble = min;
      setFlags ( HasMin );
      if (_type==Percentage) _minDouble/=100.0;
    }
  }


  void  Parameter::setMax ( double max, Parameter::Priority priority )
  { if (_updatePriority(priority)) {
      _maxDouble = max;
      setFlags ( HasMax );
      if (_type==Percentage) _maxDouble/=100.0;
    }
  }


  bool  Parameter::_doChange ( unsigned int flags, string s, bool b, int i, double d )
  {
  //cerr << "_doChange: " << _id << ":" << _value << " -> \"" << s << "\"|" << b << "|" << i << "|" << d
  //     << " [" << _flags << "]";

    Configuration* configuration = Configuration::get();
    ostringstream  svalue;
    bool           success = true;
    unsigned int   type    = (flags & FromString) ? String : _type;

    switch ( type ) {
      case Unknown:
        break;
      case String:
        svalue << s;
        break;
      case Bool:
        svalue << boolalpha << b;
        break;
      case Enumerate:  
      case Int:
        svalue << i;
        if ( flags & TypeCheck ) success = checkValue(i);
        break;
      case Double:
        svalue << d;
        if ( flags & TypeCheck ) success = checkValue(d);
        break;
      case Percentage:
        svalue << (d/100.0);
        if ( flags & TypeCheck ) success = checkValue(d/100.0);
        break;
    }

    if ( not success ) {
    //cerr << " (" << _flags << "," << _minInt << ") check failed." << endl;
      return false;
    }
    if ( svalue.str() == _value ) {
    //cerr << " no change." << endl;
      return true;
    }

    if ( (flags & NeedRestart) and hasFlags(NeedRestart) ) {
    //cerr << " needrestart";
      configuration->addLog ( Configuration::LogRestart, _id );
    }

    if ( (flags & MustExist) and hasFlags(MustExist) ) {
      if ( _type == String ) {
        //Utilities::Path filePath ( svalue.str() );
        //if ( not filePath.exists() ) {
        //cerr << " needrestart" << _id << endl;
        //  configuration->addLog ( Configuration::LogNeedExist, _id );
        //} else
        //  configuration->removeLog ( Configuration::LogNeedExist, _id );
      }
    }

  //cerr << " updated" << endl;

    _value = svalue.str();
    _onValueChanged();

    return true;
  }


}  // Cfg namespace.
