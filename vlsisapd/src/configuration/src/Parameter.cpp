
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Parameter.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>
#include  <boost/filesystem/operations.hpp>
namespace bfs = boost::filesystem;

#include  "vlsisapd/configuration/Parameter.h"
#include  "vlsisapd/configuration/Configuration.h"


namespace Cfg {

  using std::cerr;
  using std::endl;
  using std::string;


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


  Parameter::Parameter ( const std::string& id
                       , Type               type
                       , const std::string& value )
    : _id       (id)
    , _type     (type)
    , _value    (value)
    , _flags    (0)
    , _minInt   (0)
    , _maxInt   (0)
    , _minDouble(0.0)
    , _maxDouble(0.0)
  {
    if ( type == Percentage ) {
      setPercentage ( asDouble() );
    }

  //cerr << "New " << typeToString(_type) << " parameter " << _id << " value:" << _value << endl;
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


  bool  Parameter::setString ( const std::string& s, bool check )
  {
    if ( check and (_type != String) )
      cerr << "[ERROR] Parameter::setString(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(String)<< " (type mismatch)." << endl;

    if ( _value == s ) return true;

    _value = s;
    _onValueChanged();
    _checkRequirements();

    return true;
  }


  bool  Parameter::setBool ( bool b )
  {
    if ( _type != Bool )
      cerr << "[ERROR] Parameter::setBool(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Bool)<< " (type mismatch)." << endl;

    std::ostringstream s; s << std::boolalpha << b;
    if ( _value == s.str() ) return true;

    _value = s.str();
    _onValueChanged();
    _checkRequirements();

    return true;
  }


  bool  Parameter::setInt ( int i )
  {
    if ( (_type != Int) and (_type != Enumerate)  )
      cerr << "[ERROR] Parameter::setInt(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Int)<< " (type mismatch)." << endl;

    bool success = checkValue(i);
    if ( success ) {
      std::ostringstream s; s << i;
      if ( _value == s.str() ) return true;

      _value = s.str();
      _onValueChanged();
      _checkRequirements();
    }

    return success;
  } 


  bool  Parameter::setDouble ( double d )
  {
    if ( (_type != Double) and (_type != Percentage)  )
      cerr << "[ERROR] Parameter::setDouble(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Double)<< " (type mismatch)." << endl;

    bool success = checkValue(d);
    if ( success ) {
      std::ostringstream s; s << d;
      if ( _value == s.str() ) return true;

      _value = s.str();
      _onValueChanged();
      _checkRequirements();
    }

    return success;
  } 


  bool  Parameter::setPercentage ( double d )
  {
    if ( (_type != Double) and (_type != Percentage)  )
      cerr << "[ERROR] Parameter::setPercentage(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Double)<< " (type mismatch)." << endl;

    bool success = checkValue(d/100.0);
    if ( success ) {
      std::ostringstream s; s << (d/100.0);
      if ( _value == s.str() ) return true;

      _value = s.str();
      _onValueChanged();
      _checkRequirements();
    }

    return success;
  }


  void  Parameter::_checkRequirements () const
  {
    Configuration* configuration = Configuration::get();

    if ( hasFlags(NeedRestart) ) {
      configuration->addLog ( Configuration::LogRestart, _id );
    }

    if ( hasFlags(MustExist) ) {
      if ( _type == String ) {
        bfs::path filePath = ( asString() );
        if ( not bfs::exists(filePath) )
          configuration->addLog ( Configuration::LogNeedExist, _id );
        else
          configuration->removeLog ( Configuration::LogNeedExist, _id );
      }
    }
  }


}  // End of Cfg namespace.
