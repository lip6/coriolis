
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
// |  C++ Header  :       "./Parameter.h"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CFG_CONFIGURATION_PARAMETER__
#define  __CFG_CONFIGURATION_PARAMETER__

#include  <vector>
#include  <string>
#include  <sstream>
#include  <iostream>


namespace Cfg {


  class Parameter {
    public:
      enum Type  { String     = 1
                 , Int        = 2
                 , Double     = 3
                 , Bool       = 4
                 , Percentage = 5
                 , Enumerate  = 6
                 };
      enum Flags { HasMin     = 0x1
                 , HasMax     = 0x2
                 };
    public:
      class EnumValue {
        public:
          inline EnumValue ( const std::string&, int );
        public:
          std::string  _label;
          int          _value;
      };
    public:
      static std::string        typeToString  ( Type );
    public:
                                Parameter     ( const std::string& id
                                              , Type               type
                                              , const std::string& value );
      inline const std::string& getId         () const;
      inline const Type         getType       () const;
      inline const std::vector<EnumValue>&
                                getValues     () const;
      inline const std::vector<std::string>&
                                getSlaves     () const;
      inline int                getFlags      () const;
      inline bool               hasFlags      ( int mask ) const;
      inline int                getMinInt     () const;
      inline int                getMaxInt     () const;
      inline double             getMinDouble  () const;
      inline double             getMaxDouble  () const;
      inline bool               checkValue    ( int ) const;
      inline bool               checkValue    ( double ) const;
      inline const std::string& asString      () const;
      inline bool               asBool        () const;
      inline int                asInt         () const;
      inline double             asDouble      () const;
      inline double             asPercentage  () const;
      inline void               addValue      ( const std::string&, int );
      inline void               addSlave      ( const std::string& );
      inline void               setString     ( const std::string& );
      inline void               setFlags      ( int mask );
      inline void               unsetFlags    ( int mask );
      inline void               setInt        ( int );
      inline void               setDouble     ( double );
      inline void               setBool       ( bool );
      inline void               setPercentage ( double );
      inline void               setMin        ( int );
      inline void               setMax        ( int );
      inline void               setMin        ( double );
      inline void               setMax        ( double );
    private:
    // Attributes.
      std::string               _id;
      Type                      _type;
      std::string               _value;
      std::vector<EnumValue>    _values;
      int                       _flags;
      int                       _minInt;
      int                       _maxInt;
      double                    _minDouble;
      double                    _maxDouble;
      std::vector<std::string>  _slaves;
  };


// Inline Methods.
  inline const std::string&     Parameter::getId        () const { return _id; }
  inline const Parameter::Type  Parameter::getType      () const { return _type; }
  inline int                    Parameter::getFlags     () const { return _flags; }
  inline bool                   Parameter::hasFlags      ( int mask ) const { return (_flags & mask); }
  inline int                    Parameter::getMinInt    () const { return _minInt; }
  inline int                    Parameter::getMaxInt    () const { return _maxInt; }
  inline double                 Parameter::getMinDouble () const { return _minDouble; }
  inline double                 Parameter::getMaxDouble () const { return _maxDouble; }
  inline const std::string&     Parameter::asString     () const { return _value; }
  inline void                   Parameter::setString    ( const std::string& s ) { _value = s; }

  inline bool  Parameter::checkValue ( int value ) const {
    bool ok = not (   ( (_flags&HasMin) and (value < _minInt) )
                   or ( (_flags&HasMax) and (value > _maxInt) ) );
    // std::cerr << "flags:" << _flags << " "
    //           << _minInt << " < " << value << " < " << _maxInt
    //           << " : " << std::boolalpha << ok << std::endl;
    return ok;
  }

  inline bool  Parameter::checkValue ( double value ) const {
    bool ok = not (   ( (_flags&HasMin) and (value < _minDouble) )
                   or ( (_flags&HasMax) and (value > _maxDouble) ) );
    return ok;
  }

  inline const std::vector<Parameter::EnumValue>& Parameter::getValues () const { return _values; }
  inline const std::vector<std::string>&          Parameter::getSlaves () const { return _slaves; }
  inline void                                     Parameter::addSlave  ( const std::string& id ) { _slaves.push_back ( id ); }

  inline void  Parameter::addValue ( const std::string& label, int value ) {
    if ( _type != Enumerate ) {
      std::cerr << "[ERROR] Cannot add item on parameter <" << _id
                << ">, not enumerated." << std::endl;
      return;
    }
    _values.push_back ( EnumValue(label,value) );
  }


  inline bool  Parameter::asBool () const
  { std::istringstream s ( _value ); bool b; s >> std::boolalpha >> b; return b; }

  inline int  Parameter::asInt () const
  { std::istringstream s ( _value ); int i; s >> i; return i; }

  inline double  Parameter::asDouble () const
  { std::istringstream s ( _value ); double d; s >> d; return d; }

  inline double  Parameter::asPercentage () const
  { std::istringstream s ( _value ); double r; s >> r; return r*100.0; }

  inline void  Parameter::setInt        ( int i )      { std::ostringstream s; s << i; _value = s.str(); } 
  inline void  Parameter::setDouble     ( double d )   { std::ostringstream s; s << d; _value = s.str(); } 
  inline void  Parameter::setBool       ( bool b )     { std::ostringstream s; s << std::boolalpha << b; _value = s.str(); }
  inline void  Parameter::setPercentage ( double d )   { std::ostringstream s; s << (d/100.0); _value = s.str(); }
  inline void  Parameter::setFlags      ( int mask )   { _flags |= mask; }
  inline void  Parameter::unsetFlags    ( int mask )   { _flags &= ~mask; }
  inline void  Parameter::setMin        ( int min )    { _minInt = min; setFlags(HasMin); }
  inline void  Parameter::setMax        ( int max )    { _maxInt = max; setFlags(HasMax); }

  inline void  Parameter::setMin ( double min )
  { _minDouble = min; setFlags(HasMin); if (_type==Percentage) _minDouble/=100.0; }

  inline void  Parameter::setMax ( double max )
  { _maxDouble = max; setFlags(HasMax); if (_type==Percentage) _maxDouble/=100.0; }

  inline Parameter::EnumValue::EnumValue ( const std::string& label, int value )
    : _label(label), _value(value) { }


}  // End of Cfg namespace.


#endif  // __CFG_CONFIGURATION_PARAMETER__
