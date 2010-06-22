
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
#include  "vlsisapd/configuration/Parameter.h"


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


  void  Parameter::setString ( const std::string& s, bool check )
  {
    if ( check and (_type != String) )
      cerr << "[ERROR] Parameter::setString(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(String)<< " (type mismatch)." << endl;

    _value = s;
    _onValueChanged();
  }


  void  Parameter::setBool ( bool b )
  {
    if ( _type != Bool )
      cerr << "[ERROR] Parameter::setBool(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Bool)<< " (type mismatch)." << endl;

    std::ostringstream s; s << std::boolalpha << b; _value = s.str();

    _onValueChanged();
  }


  void  Parameter::setInt ( int i )
  {
    if ( (_type != Int) and (_type != Enumerate)  )
      cerr << "[ERROR] Parameter::setInt(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Int)<< " (type mismatch)." << endl;

    std::ostringstream s; s << i; _value = s.str();
    _onValueChanged();
  } 


  void  Parameter::setDouble ( double d )
  {
    if ( (_type != Double) and (_type != Percentage)  )
      cerr << "[ERROR] Parameter::setDouble(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Double)<< " (type mismatch)." << endl;

    std::ostringstream s; s << d; _value = s.str();
    _onValueChanged();
  } 


  void  Parameter::setPercentage ( double d )
  {
    if ( (_type != Double) and (_type != Percentage)  )
      cerr << "[ERROR] Parameter::setPercentage(): Setting " << Parameter::typeToString(_type)
           << " parameter <" << _id
           << "> as " << Parameter::typeToString(Double)<< " (type mismatch)." << endl;

    std::ostringstream s; s << (d/100.0); _value = s.str();
    _onValueChanged();
  }


}  // End of Cfg namespace.
