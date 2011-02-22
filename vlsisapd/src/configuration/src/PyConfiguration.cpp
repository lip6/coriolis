
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2011, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyConfiguration.cpp"                    |
// +-----------------------------------------------------------------+


#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace boost::python;

#include "vlsisapd/openChams/PySTLMapWrapper.h"
using OpenChams::map_item;

#include "vlsisapd/configuration/Configuration.h"


namespace {

  using namespace Cfg;


  template<typename EnumType>
  struct EnumToInt {
      static PyObject* convert ( EnumType& type )
      { return incref ( object((int)type).ptr() ); }
  };


// Thin wrappers for function members overloads.
  void  parameterSetMinInt1    ( Parameter& self, int    min ) { self.setMin(min); }
  void  parameterSetMinDouble1 ( Parameter& self, double min ) { self.setMin(min); }
  void  parameterSetMaxInt1    ( Parameter& self, int    max ) { self.setMax(max); }
  void  parameterSetMaxDouble1 ( Parameter& self, double max ) { self.setMax(max); }

// STL map wrapper for boost::python.
  template<typename Map>
  class MapWrapper {
    public:
      typedef typename Map::key_type    Key;
      typedef typename Map::mapped_type Value;
    public:
      static Value& get    ( Map&, const Key& );
      static void   set    ( Map&, const Key&, const Value& );
      static void   del    ( Map&, const Key& );
      static bool   in     ( Map&, const Key& );
      static list   keys   ( Map& );
      static list   values ( Map& );
      static list   items  ( Map& );
  };


  inline void  KeyError () { PyErr_SetString(PyExc_KeyError, "Key not found"); }

  
  template<typename Map>
  typename MapWrapper<Map>::Value& MapWrapper<Map>::get ( Map& m, const typename MapWrapper<Map>::Key& k )
  {
    static Value notFound;
    if( m.find(k) != m.end() ) return m[k];
    KeyError ();
    return notFound;
  }

  template<typename Map>
  void  MapWrapper<Map>::set ( Map& m, const MapWrapper<Map>::Key& k, const typename MapWrapper<Map>::Value& v )
  { m[k] = v; }

  template<typename Map>
  void  MapWrapper<Map>::del ( Map& m, const typename MapWrapper<Map>::Key& k)
  {
    if( m.find(k) != m.end() ) m.erase(k);
    else KeyError ();
  }

  template<typename Map>
  bool  MapWrapper<Map>::in ( Map& m, const typename MapWrapper<Map>::Key& k )
  { return m.find(k) != m.end(); }

  template<typename Map>
  list  MapWrapper<Map>::keys ( Map& m )
  {
    list l;
    for(typename Map::iterator it = m.begin() ; it != m.end() ; ++it )
      l.append ( it->first );
    return l;
  }

  template<typename Map>
  list  MapWrapper<Map>::values ( Map& m)
  {
    list l;
    for( typename Map::iterator it=m.begin(); it != m.end() ; ++it )
      l.append ( it->second );
    return l;
  }

  template<typename Map>
  list  MapWrapper<Map>::items ( Map& m ) {
    list l;
    for( typename Map::iterator it=m.begin(); it!=m.end(); ++it )
      l.append( make_tuple(it->first,it->second) );
    return l;
  }


} // End of anonymous namespace.


namespace Cfg {


  BOOST_PYTHON_MODULE(Cfg) {

    typedef  void (Parameter::* VoidIntPrioritySign    )(int   ,Parameter::Priority)  ;
    typedef  void (Parameter::* VoidDoublePrioritySign )(double,Parameter::Priority)  ;
    typedef  void (Parameter::* VoidIntSign            )(int   );
    typedef  void (Parameter::* VoidDoubleSign         )(double);
    typedef  bool (Parameter::* BoolIntConstSign       )(int   ) const;
    typedef  bool (Parameter::* BoolDoubleConstSign    )(double) const;
    typedef  bool (Parameter::* BoolBoolSign           )(bool  );
    typedef  bool (Parameter::* BoolIntSign            )(int   );
    typedef  bool (Parameter::* BoolDoubleSign         )(double);
    typedef  bool (Parameter::* BoolStringSign         )(std::string);
    typedef  bool (Parameter::* BoolStringFlagsSign    )(std::string,unsigned int);

  // Parameter overloaded function members.
    BoolIntConstSign     paramCheckValueInt    = (BoolIntConstSign)   &Parameter::checkValue;
    BoolDoubleConstSign  paramCheckValueDouble = (BoolDoubleConstSign)&Parameter::checkValue;
    BoolStringSign       paramSetString1       = (BoolStringSign)     &Parameter::setString;
    BoolStringFlagsSign  paramSetString2       = (BoolStringFlagsSign)&Parameter::setString;
    BoolBoolSign         paramSetBool1         = (BoolBoolSign)       &Parameter::setBool;
    BoolIntSign          paramSetInt1          = (BoolIntSign)        &Parameter::setInt;
    BoolDoubleSign       paramSetDouble1       = (BoolDoubleSign)     &Parameter::setDouble;
    BoolDoubleSign       paramSetPercentage1   = (BoolDoubleSign)     &Parameter::setPercentage;

    implicitly_convertible<Parameter::Type    ,int>();
    implicitly_convertible<Parameter::Flags   ,int>();
    implicitly_convertible<Parameter::Priority,int>();

  //to_python_converter<const Parameter::Type    ,EnumToInt<const Parameter::Type>     >();

    {
      scope  paramScope
        ( class_<Parameter>("Parameter", init<const std::string&, Parameter::Type, const std::string&
                                             ,optional<Parameter::Priority> >())
                           .def("isFile"            , &Parameter::isFile)
                           .def("isPath"            , &Parameter::isPath)
                           .def("hasMin"            , &Parameter::hasMin)
                           .def("hasMax"            , &Parameter::hasMax)
                           .def("hasNeedRestart"    , &Parameter::hasNeedRestart)
                           .def("hasMustExist"      , &Parameter::hasMustExist)
                         //.def("hasFlags"          , &Parameter::hasFlags)
                           .def("getId"             , &Parameter::getId, return_value_policy<reference_existing_object>())
                           .def("getMinInt"         , &Parameter::getMinInt)
                           .def("getMaxInt"         , &Parameter::getMaxInt)
                           .def("getMinDouble"      , &Parameter::getMinDouble)
                           .def("getMaxDouble"      , &Parameter::getMaxDouble)
                           .def("checkValue"        , paramCheckValueInt)
                           .def("checkValue"        , paramCheckValueDouble)
                           .def("asString"          , &Parameter::asString, return_value_policy<reference_existing_object>())
                           .def("asPercentageString", &Parameter::asPercentageString)
                           .def("asBool"            , &Parameter::asBool)
                           .def("asInt"             , &Parameter::asInt)
                           .def("asPercentage"      , &Parameter::asPercentage)
                           .def("addValue"          , &Parameter::addValue)
                           .def("addSlave"          , &Parameter::addSlave)
                           .def("setPriority"       , &Parameter::setPriority)
                           .def("setString"         , &Parameter::setString)
                           .def("setString"         , paramSetString1)
                           .def("setString"         , paramSetString2)
                           .def("setBool"           , &Parameter::setBool)
                           .def("setBool"           , paramSetBool1)
                           .def("setInt"            , &Parameter::setInt)
                           .def("setInt"            , paramSetInt1)
                           .def("setDouble"         , &Parameter::setDouble)
                           .def("setDouble"         , paramSetDouble1)
                           .def("setPercentage"     , &Parameter::setPercentage)
                           .def("setPercentage"     , paramSetPercentage1)
                           .def("setMin"            , &parameterSetMinInt1)
                           .def("setMin"            , &parameterSetMinDouble1)
                           .def("setMax"            , &parameterSetMaxInt1)
                           .def("setMax"            , &parameterSetMaxDouble1)

                         //.add_property("flags", &Parameter::getFlags, &Parameter::setFlags)
                           .add_property("type" , &Parameter::getType)
                         );

      enum_<Parameter::Type>("Type")
      .value("Unknown"   ,Parameter::Unknown)
      .value("String"    ,Parameter::String)
      .value("Bool"      ,Parameter::Bool)
      .value("Int"       ,Parameter::Int)
      .value("Enumerate" ,Parameter::Enumerate)
      .value("Double"    ,Parameter::Double)
      .value("Percentage",Parameter::Percentage)
      ;
      
      enum_<Parameter::Priority>("Priority")
        .value("UseDefault"        ,Parameter::UseDefault)
        .value("ApplicationBuiltin",Parameter::ApplicationBuiltin)
        .value("ConfigurationFile" ,Parameter::ConfigurationFile)
        .value("CommandLine"       ,Parameter::CommandLine)
        .value("Interactive"       ,Parameter::Interactive)
        ;
      
      enum_<Parameter::Flags>("Flags")
        .value("HasMin"         , Parameter::HasMin)
        .value("HasMax"         , Parameter::HasMax)
        .value("IsFile"         , Parameter::IsFile)
        .value("IsPath"         , Parameter::IsPath)
        .value("NeedRestart"    , Parameter::NeedRestart)
        .value("MustExist"      , Parameter::MustExist)
        .value("TypeCheck"      , Parameter::TypeCheck)
        .value("FromString"     , Parameter::FromString)
        .value("AllRequirements", Parameter::AllRequirements)
      ;
    }

    typedef  std::map <const std::string,Parameter*>  ParametersMap;
    typedef  std::pair<const std::string,Parameter*>  ParametersPair;

    class_<ParametersPair>("ParametersPair")
      .def_readonly ("key"  , &ParametersPair::first )
      .def_readwrite("value", &ParametersPair::second)
      ;
    class_<ParametersMap>("ParametersMap")
      .def("__len__"      , &ParametersMap::size)
      .def("clear"        , &ParametersMap::clear)
      .def("__getitem__"  , &MapWrapper<ParametersMap>::get, return_value_policy<reference_existing_object>())
      .def("__setitem__"  , &MapWrapper<ParametersMap>::set, return_value_policy<reference_existing_object>())
      .def("__delitem__"  , &MapWrapper<ParametersMap>::del)
      .def("__contains__" , &MapWrapper<ParametersMap>::in )
      .def("has_key"      , &MapWrapper<ParametersMap>::in )
      .def("keys"         , &MapWrapper<ParametersMap>::keys )
      .def("values"       , &MapWrapper<ParametersMap>::values )
      .def("items"        , &MapWrapper<ParametersMap>::items )
      ;

  // Configuration overloaded function members.
    Parameter* (Configuration::*CfgGetParameter1) ( const std::string& ) const                  = (Parameter* (Configuration:: *)(const std::string&) const)                &Configuration::getParameter;
    Parameter* (Configuration::*CfgGetParameter2) ( const std::string&, Parameter::Type ) const = (Parameter* (Configuration:: *)(const std::string&,Parameter::Type) const)&Configuration::getParameter;

    {
      scope  confScope
        ( class_<Configuration>("Configuration", no_init)
        .def("get"                , &Configuration::get                , return_value_policy<reference_existing_object>())
        .def("getParameter"       , CfgGetParameter1                   , return_value_policy<reference_existing_object>())
        .def("getParameter"       , CfgGetParameter2                   , return_value_policy<reference_existing_object>())
        .def("pushDefaultPriority", &Configuration::pushDefaultPriority)
        .def("popDefaultPriority" , &Configuration::popDefaultPriority )
        .def("getDefaultPriority" , &Configuration::getDefaultPriority )
        .def("getParameters"      , &Configuration::getParameters      , return_value_policy<reference_existing_object>())
        .staticmethod("get")
        .staticmethod("pushDefaultPriority")
        .staticmethod("popDefaultPriority")
        .staticmethod("getDefaultPriority")
        .add_property("flags", &Configuration::getFlags, &Configuration::setFlags)
        );

      implicitly_convertible<Configuration::Flags,int>();

      enum_<Configuration::Flags>("Flags")
        .value("DriveValues",Configuration::DriveValues)
        .value("DriveLayout",Configuration::DriveLayout)
        ;
    }


  // Configuration overloaded functions.
    typedef  Parameter* (*ParamStringSign      )(const std::string&);
    typedef  Parameter* (*ParamStringBoolSign  )(const std::string&, bool);
    typedef  Parameter* (*ParamStringIntSign   )(const std::string&, int);
    typedef  Parameter* (*ParamStringDoubleSign)(const std::string&, double);
    typedef  Parameter* (*ParamStringStringSign)(const std::string&, const std::string&);

    ParamStringSign        getParamString1     = (ParamStringSign      )getParamString;
    ParamStringStringSign  getParamString2     = (ParamStringStringSign)getParamString;
    ParamStringSign        getParamBool1       = (ParamStringSign      )getParamBool;
    ParamStringBoolSign    getParamBool2       = (ParamStringBoolSign  )getParamBool;
    ParamStringSign        getParamInt1        = (ParamStringSign      )getParamInt;
    ParamStringIntSign     getParamInt2        = (ParamStringIntSign   )getParamInt;
    ParamStringSign        getParamEnumerate1  = (ParamStringSign      )getParamEnumerate;
    ParamStringIntSign     getParamEnumerate2  = (ParamStringIntSign   )getParamEnumerate;
    ParamStringSign        getParamDouble1     = (ParamStringSign      )getParamDouble;
    ParamStringDoubleSign  getParamDouble2     = (ParamStringDoubleSign)getParamDouble;
    ParamStringSign        getParamPercentage1 = (ParamStringSign      )getParamPercentage;
    ParamStringDoubleSign  getParamPercentage2 = (ParamStringDoubleSign)getParamPercentage;

    def("getParamString"    , getParamString1    , return_value_policy<reference_existing_object>());
    def("getParamString"    , getParamString2    , return_value_policy<reference_existing_object>());
    def("getParamBool"      , getParamBool1      , return_value_policy<reference_existing_object>());
    def("getParamBool"      , getParamBool2      , return_value_policy<reference_existing_object>());
    def("getParamInt"       , getParamInt1       , return_value_policy<reference_existing_object>());
    def("getParamInt"       , getParamInt2       , return_value_policy<reference_existing_object>());
    def("getParamEnumerate" , getParamEnumerate1 , return_value_policy<reference_existing_object>());
    def("getParamEnumerate" , getParamEnumerate2 , return_value_policy<reference_existing_object>());
    def("getParamDouble"    , getParamDouble1    , return_value_policy<reference_existing_object>());
    def("getParamDouble"    , getParamDouble2    , return_value_policy<reference_existing_object>());
    def("getParamPercentage", getParamPercentage1, return_value_policy<reference_existing_object>());
    def("getParamPercentage", getParamPercentage2, return_value_policy<reference_existing_object>());

  } // End of Configuration BOOST_PYTHON_MODULE.



} // End of Cfg namespace.
