
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
#include  <boost/function.hpp>


namespace Cfg {


  class Parameter {
    public:
      enum Type  { Unknown     = 0
                 , String      = 1
                 , Bool        = 2
                 , Int         = 3
                 , Enumerate   = 4
                 , Double      = 5
                 , Percentage  = 6
                 };           
      enum Flags { HasMin      = 0x01
                 , HasMax      = 0x02
                 , IsFile      = 0x04
                 , IsPath      = 0x08
                 , NeedRestart = 0x10
                 , MustExist   = 0x20
                 };
      typedef boost::function< void(Parameter*) >  ParameterChangedCb_t;
    public:
      class EnumValue {
        public:
          inline EnumValue ( const std::string&, int );
        public:
          std::string  _label;
          int          _value;
      };
    public:
      static std::string        typeToString       ( Type );
    public:                                        
                                Parameter          ( const std::string& id
                                                   , Type               type
                                                   , const std::string& value );
      inline bool               isFile             () const;
      inline bool               isPath             () const;
      inline bool               hasMin             () const;
      inline bool               hasMax             () const;
      inline bool               hasNeedRestart     () const;
      inline bool               hasMustExist       () const;
      inline bool               hasFlags           ( int mask ) const;
      inline const std::string& getId              () const;
      inline const Type         getType            () const;
      inline const std::vector<EnumValue>&         
                                getValues          () const;
      inline const std::vector<std::string>&       
                                getSlaves          () const;
      inline int                getFlags           () const;
      inline int                getMinInt          () const;
      inline int                getMaxInt          () const;
      inline double             getMinDouble       () const;
      inline double             getMaxDouble       () const;
      inline bool               checkValue         ( int ) const;
      inline bool               checkValue         ( double ) const;
      inline const std::string& asString           () const;
             std::string        asPercentageString () const;
             bool               asBool             () const;
             int                asInt              () const;
             double             asDouble           () const;
             double             asPercentage       () const;
      inline void               addValue           ( const std::string&, int );
      inline void               addSlave           ( const std::string& );
      inline void               setFlags           ( int mask );
      inline void               unsetFlags         ( int mask );
             bool               setString          ( const std::string&, bool check=true );
             bool               setBool            ( bool );
             bool               setInt             ( int );
             bool               setDouble          ( double );
             bool               setPercentage      ( double );
      inline void               setMin             ( int );
      inline void               setMax             ( int );
      inline void               setMin             ( double );
      inline void               setMax             ( double );
      inline void               registerCb         ( ParameterChangedCb_t );
    private:                                       
      inline void               _onValueChanged    ();
             void               _checkRequirements () const;
    private:
    // Attributes.
      std::string                        _id;
      Type                               _type;
      std::string                        _value;
      std::vector<EnumValue>             _values;
      int                                _flags;
      int                                _minInt;
      int                                _maxInt;
      double                             _minDouble;
      double                             _maxDouble;
      std::vector<std::string>           _slaves;
      std::vector<ParameterChangedCb_t>  _callbacks;
  };


// Inline Methods.
  inline bool                   Parameter::isFile         () const { return hasFlags(IsFile); };
  inline bool                   Parameter::isPath         () const { return hasFlags(IsPath); };
  inline bool                   Parameter::hasMin         () const { return hasFlags(HasMin); };
  inline bool                   Parameter::hasMax         () const { return hasFlags(HasMax); };
  inline bool                   Parameter::hasNeedRestart () const { return hasFlags(NeedRestart); };
  inline bool                   Parameter::hasMustExist   () const { return hasFlags(MustExist); };
  inline const std::string&     Parameter::getId          () const { return _id; }
  inline const Parameter::Type  Parameter::getType        () const { return _type; }
  inline int                    Parameter::getFlags       () const { return _flags; }
  inline bool                   Parameter::hasFlags       ( int mask ) const { return (_flags & mask); }
  inline int                    Parameter::getMinInt      () const { return _minInt; }
  inline int                    Parameter::getMaxInt      () const { return _maxInt; }
  inline double                 Parameter::getMinDouble   () const { return _minDouble; }
  inline double                 Parameter::getMaxDouble   () const { return _maxDouble; }
  inline const std::string&     Parameter::asString       () const { return _value; }

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

  inline void  Parameter::registerCb ( ParameterChangedCb_t cb ) { _callbacks.push_back(cb); }

  inline void  Parameter::_onValueChanged ()
  { for ( size_t icb=0 ; icb<_callbacks.size() ; ++icb ) _callbacks[icb]( this ); }


}  // End of Cfg namespace.


#endif  // __CFG_CONFIGURATION_PARAMETER__
