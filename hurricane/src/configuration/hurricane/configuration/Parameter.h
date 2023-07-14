// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) SU 2008-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/configuration/Parameter.h"         |
// +-----------------------------------------------------------------+


#pragma  once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <hurricane/Commons.h>


namespace Cfg {


  class Parameter {
    public:
      enum Type     { Unknown         = 0
                    , String          = 1
                    , Bool            = 2
                    , Int             = 3
                    , Enumerate       = 4
                    , Double          = 5
                    , Percentage      = 6
                    };           
      enum Flags    { HasMin          = 0x01
                    , HasMax          = 0x02
                    , IsFile          = 0x04
                    , IsPath          = 0x08
                    , NeedRestart     = 0x10
                    , MustExist       = 0x20
                    , TypeCheck       = 0x40
                    , FromString      = 0x80
                    , AllRequirements = HasMin|HasMax|IsFile|IsPath|NeedRestart|MustExist|TypeCheck
                    };
      enum Priority { UseDefault         = 0
                    , ApplicationBuiltin = 1
                    , ConfigurationFile  = 2
                    , UserFile           = 3
                    , CommandLine        = 4
                    , Interactive        = 5
                    };
      typedef std::function< void(Parameter*) >  ParameterChangedCb_t;
    public:
      class EnumValue {
        public:
          inline EnumValue ( std::string, int );
        public:
          std::string  _label;
          int          _value;
      };
    public:
      static std::string        typeToString        ( Type );
      static std::string        priorityToString    ( Priority );
      static Type               stringToType        ( std::string );
      static Priority           stringToPriority    ( std::string );
      static Priority           pushDefaultPriority ( Priority );
      static Priority           popDefaultPriority  ();
      static Priority           getDefaultPriority  ();
    public:                                        
                                Parameter           ( std::string id
                                                    , Type        type
                                                    , std::string value
                                                    , Priority    priority=UseDefault );
      inline bool               isFile              () const;
      inline bool               isPath              () const;
      inline bool               hasMin              () const;
      inline bool               hasMax              () const;
      inline bool               hasNeedRestart      () const;
      inline bool               hasMustExist        () const;
      inline bool               hasFlags            ( int mask ) const;
      inline std::string        getId               () const;
      inline Type               getType             () const;
      inline Priority           getPriority         () const;
      inline const std::vector<EnumValue>&          
                                getValues           () const;
      inline const std::vector<std::string>&        
                                getSlaves           () const;
      inline int                getFlags            () const;
      inline int                getMinInt           () const;
      inline int                getMaxInt           () const;
      inline double             getMinDouble        () const;
      inline double             getMaxDouble        () const;
      inline bool               checkValue          ( int ) const;
      inline bool               checkValue          ( double ) const;
      inline std::string        asString            () const;
             std::string        asPercentageString  () const;
             bool               asBool              () const;
             int                asInt               () const;
             double             asDouble            () const;
             double             asPercentage        () const;
      inline void               addValue            ( std::string, int );
      inline void               addSlave            ( std::string );
      inline void               setPriority         ( Priority );
      inline void               setFlags            ( int mask );
      inline void               unsetFlags          ( int mask );
             bool               setRawString        ( std::string , Priority priority=UseDefault );
             bool               setString           ( std::string
                                                    , Priority     priority=UseDefault
                                                    , unsigned int flags   =AllRequirements
                                                    );
             bool               setBool             ( bool  , Priority priority=UseDefault );
             bool               setInt              ( int   , Priority priority=UseDefault );
             bool               setDouble           ( double, Priority priority=UseDefault );
             bool               setPercentage       ( double, Priority priority=UseDefault );
             void               setMin              ( int   , Priority priority=UseDefault );
             void               setMax              ( int   , Priority priority=UseDefault );
             void               setMin              ( double, Priority priority=UseDefault );
             void               setMax              ( double, Priority priority=UseDefault );
      inline void               registerCb          ( void* tag, ParameterChangedCb_t );
      inline void               unregisterCb        ( void* tag );
      inline void               valueChanged        ();
      inline std::string        _getString          () const;
    private:                                        
      inline void               _onValueChanged     ();
      inline bool               _updatePriority     ( Priority );
             bool               _doChange           ( unsigned int flags, std::string, bool, int, double );
    private:
    // Attributes.
      static std::vector<Priority>  _defaultPriorities;
      std::string                   _id;
      Type                          _type;
      std::string                   _value;
      std::vector<EnumValue>        _values;
      Priority                      _priority;
      int                           _flags;
      int                           _minInt;
      int                           _maxInt;
      double                        _minDouble;
      double                        _maxDouble;
      std::vector<std::string>      _slaves;
      std::vector< std::pair<void*,ParameterChangedCb_t> >  _callbacks;
  };


// Inline Methods.
  inline bool                   Parameter::isFile         () const { return hasFlags(IsFile); };
  inline bool                   Parameter::isPath         () const { return hasFlags(IsPath); };
  inline bool                   Parameter::hasMin         () const { return hasFlags(HasMin); };
  inline bool                   Parameter::hasMax         () const { return hasFlags(HasMax); };
  inline bool                   Parameter::hasNeedRestart () const { return hasFlags(NeedRestart); };
  inline bool                   Parameter::hasMustExist   () const { return hasFlags(MustExist); };
  inline std::string            Parameter::getId          () const { return _id; }
  inline Parameter::Type        Parameter::getType        () const { return _type; }
  inline Parameter::Priority    Parameter::getPriority    () const { return _priority; }
  inline int                    Parameter::getFlags       () const { return _flags; }
  inline bool                   Parameter::hasFlags       ( int mask ) const { return (_flags & mask); }
  inline int                    Parameter::getMinInt      () const { return _minInt; }
  inline int                    Parameter::getMaxInt      () const { return _maxInt; }
  inline double                 Parameter::getMinDouble   () const { return _minDouble; }
  inline double                 Parameter::getMaxDouble   () const { return _maxDouble; }
  inline std::string            Parameter::asString       () const { return _value; }
  inline void                   Parameter::setFlags       ( int mask ) { _flags |= mask; }
  inline void                   Parameter::unsetFlags     ( int mask ) { _flags &= ~mask; }
  inline void                   Parameter::setPriority    ( Priority priority ) { _priority = priority; }
  inline void                   Parameter::valueChanged   () { _onValueChanged(); }

  inline bool  Parameter::checkValue ( int value ) const {
    bool ok = not (   ( (_flags&HasMin) and (value < _minInt) )
                   or ( (_flags&HasMax) and (value > _maxInt) ) );
    // std::cerr << "flags:" << _flags << " "
    //           << _minInt << " < " << value << " < " << _maxInt
    //           << " : " << std::boolalpha << ok << std::endl;
    // std::cerr << " " << std::boolalpha << ok;
    return ok;
  }

  inline bool  Parameter::checkValue ( double value ) const {
    // std::cerr << " (double) " << _minDouble << "<" << value << "<" << _maxDouble;
    bool ok = not (   ( (_flags&HasMin) and (value < _minDouble) )
                   or ( (_flags&HasMax) and (value > _maxDouble) ) );
    // std::cerr << " " << std::boolalpha << ok;
    return ok;
  }

  inline const std::vector<Parameter::EnumValue>& Parameter::getValues () const { return _values; }
  inline const std::vector<std::string>&          Parameter::getSlaves () const { return _slaves; }
  inline void                                     Parameter::addSlave  ( std::string id ) { _slaves.push_back ( id ); }

  inline void  Parameter::addValue ( std::string label, int value ) {
    if ( _type != Enumerate ) {
      std::cerr << "[ERROR] Cannot add item on parameter <" << _id
                << ">, not enumerated." << std::endl;
      return;
    }
    _values.push_back ( EnumValue(label,value) );
  }

  inline Parameter::EnumValue::EnumValue ( std::string label, int value )
    : _label(label), _value(value) { }

  inline void  Parameter::registerCb ( void* tag, ParameterChangedCb_t cb )
  {
    _callbacks.push_back(std::make_pair(tag,cb)); cb(this);
  }

  inline void  Parameter::unregisterCb ( void* tag )
  {
    for ( auto icb = _callbacks.begin() ; icb != _callbacks.end() ; ++icb ) {
      if ( (*icb).first == tag ) { _callbacks.erase( icb ); break; }
    }
  }

  inline bool  Parameter::_updatePriority ( Priority priority )
  {
    if ( priority == UseDefault ) priority = getDefaultPriority();
    if ( priority < _priority   ) return false;
    _priority = priority;
    return true;
  }

  inline void  Parameter::_onValueChanged ()
  { for ( size_t icb=0 ; icb<_callbacks.size() ; ++icb ) _callbacks[icb].second( this ); }


  inline std::string  Parameter::_getString () const
  {
    std::string s = "<Parameter id=\"" + _id
                  + "\" type=" + typeToString(_type)
                  + " value=\"" + asString() + "\">";
    return s;
  }


}  // Cfg namespace.


template<> inline std::string  getString<const Cfg::Parameter*> ( const Cfg::Parameter* p )
{ return p->_getString(); }


template<> inline std::string  getString<Cfg::Parameter*> ( Cfg::Parameter* p )
{ return p->_getString(); }


inline std::ostream& operator<< ( std::ostream& o, const Cfg::Parameter* p )
{
  if (not p) return o << "NULL [const Parameter*]";
  return o << "&" << getString<const Cfg::Parameter*>(p);
}


inline std::ostream& operator<< ( std::ostream& o, Cfg::Parameter* p )
{
  if (not p) return o << "NULL [const Parameter*]";
  return o << "&" << getString<const Cfg::Parameter*>(p);
}
