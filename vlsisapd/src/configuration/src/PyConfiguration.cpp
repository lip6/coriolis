
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyConfiguration.cpp"                         |
// +-----------------------------------------------------------------+


#include  <sstream>
#include  "vlsisapd/configuration/BoostPythonStlWrappers.h"
#include  "vlsisapd/configuration/Configuration.h"
#include  "vlsisapd/configuration/ParameterWidget.h"
using namespace boost::python;


namespace {

  using namespace Cfg;


// Thin wrappers for function members overloads.
  bool  paramSetString1 ( Parameter& self, const std::string& value )
  { return self.setString(value); }

  bool  paramSetString2 ( Parameter& self, const std::string& value, unsigned int flags )
  { return self.setString(value,Configuration::getDefaultPriority(),flags); }

  bool  paramSetString3 ( Parameter& self, const std::string& value, unsigned int flags, Parameter::Priority pri )
  { return self.setString(value,pri,flags); }

  bool  paramSetBool1       ( Parameter& self, bool   value ) { return self.setBool(value); }
  bool  paramSetInt1        ( Parameter& self, int    value ) { return self.setInt(value); }
  bool  paramSetDouble1     ( Parameter& self, double value ) { return self.setDouble(value); }
  bool  paramSetPercentage1 ( Parameter& self, double value ) { return self.setPercentage(value); }

  bool  paramSetBool2       ( Parameter& self, bool   value, Parameter::Priority pri ) { return self.setBool(value,pri); }
  bool  paramSetInt2        ( Parameter& self, int    value, Parameter::Priority pri ) { return self.setInt(value,pri); }
  bool  paramSetDouble2     ( Parameter& self, double value, Parameter::Priority pri ) { return self.setDouble(value,pri); }
  bool  paramSetPercentage2 ( Parameter& self, double value, Parameter::Priority pri ) { return self.setPercentage(value,pri); }

  void  paramSetMinInt1     ( Parameter& self, int    min ) { self.setMin(min); }
  void  paramSetMinDouble1  ( Parameter& self, double min ) { self.setMin(min); }
  void  paramSetMaxInt1     ( Parameter& self, int    max ) { self.setMax(max); }
  void  paramSetMaxDouble1  ( Parameter& self, double max ) { self.setMax(max); }

  std::string  reprParameter ( const Parameter* p )
  {
    std::ostringstream repr;
    repr << "<Parameter id=\"" << p->getId()
         <<       "\" type=\"" << Parameter::typeToString(p->getType())
         <<      "\" value=\"";

    if ( p->getType() == Parameter::Percentage ) repr << p->asPercentageString();
    else repr << p->asString();

    repr << "\">";

    return repr.str();
  }


  void  layoutAddTab ( LayoutDescription& self
                     , const std::string& tabName
                     , const std::string& id
                     )
  { self.addTab(tabName,id); }


  void  layoutAddSection2 ( LayoutDescription& self
                          , const std::string& tabName
                          , const std::string& section
                          )
  { self.addSection(tabName,section); };


  void  layoutAddSection3 ( LayoutDescription& self
                          , const std::string& tabName
                          , const std::string& section
                          , int                column
                          )
  { self.addSection(tabName,section,column); };


  void  layoutAddParameter3 ( LayoutDescription& self
                            , const std::string& tabName
                            , const std::string& id
                            , const std::string& label
                            )
  { self.addParameter(tabName,id,label); };


  void  layoutAddParameter4 ( LayoutDescription& self
                            , const std::string& tabName
                            , const std::string& id
                            , const std::string& label
                            , int                column
                            )
  { self.addParameter(tabName,id,label,column); };


  void  layoutAddParameter5 ( LayoutDescription& self
                            , const std::string& tabName
                            , const std::string& id
                            , const std::string& label
                            , int                column
                            , int                span
                            )
  { self.addParameter(tabName,id,label,column,span); };


  void  layoutAddParameter6 ( LayoutDescription& self
                            , const std::string& tabName
                            , const std::string& id
                            , const std::string& label
                            , int                column
                            , int                span
                            , unsigned int       flags
                            )
  { self.addParameter(tabName,id,label,column,span,flags); };

  Parameter* cfgGetParameter1 ( const Configuration& self, const std::string& id )
  { return self.getParameter(id); }

  Parameter* cfgGetParameter2 ( const Configuration& self, const std::string& id, Parameter::Type type )
  { return self.getParameter(id,type); }

  Parameter* cfgAddParameter2 ( Configuration& self, const std::string& id, Parameter::Type type, const std::string& value )
  { return self.addParameter(id,type,value); }

  Parameter* cfgAddParameter3 ( Configuration& self, const std::string& id, Parameter::Type type, const std::string& value, Parameter::Priority pri )
  { return self.addParameter(id,type,value,pri); }

  Parameter* getParamString1     ( const std::string& id ) { return getParamString(id); }
  Parameter* getParamBool1       ( const std::string& id ) { return getParamBool(id); }
  Parameter* getParamInt1        ( const std::string& id ) { return getParamInt(id); }
  Parameter* getParamEnumerate1  ( const std::string& id ) { return getParamEnumerate(id); }
  Parameter* getParamDouble1     ( const std::string& id ) { return getParamDouble(id); }
  Parameter* getParamPercentage1 ( const std::string& id ) { return getParamPercentage(id); }

  Parameter* getParamString2     ( const std::string& id, const std::string& value ) { return getParamString(id,value); }
  Parameter* getParamInt2        ( const std::string& id, int                value ) { return getParamInt(id,value); }
  Parameter* getParamEnumerate2  ( const std::string& id, int                value ) { return getParamEnumerate(id,value); }
  Parameter* getParamBool2       ( const std::string& id, bool               value ) { return getParamBool(id,value); }
  Parameter* getParamDouble2     ( const std::string& id, double             value ) { return getParamDouble(id,value); }
  Parameter* getParamPercentage2 ( const std::string& id, double             value ) { return getParamPercentage(id,value); }


} // End of anonymous namespace.


namespace Cfg {


  BOOST_PYTHON_MODULE(Cfg) {

    VectorWrapper< std::vector<std::string>          >::wrap ( "StringVector" );
    VectorWrapper< std::vector<Parameter::EnumValue> >::wrap ( "EnumValueVector" );

  //typedef  void (Parameter::* VoidIntPrioritySign    )(int   ,Parameter::Priority);
  //typedef  void (Parameter::* VoidDoublePrioritySign )(double,Parameter::Priority);
  //typedef  void (Parameter::* VoidIntSign            )(int   );
  //typedef  void (Parameter::* VoidDoubleSign         )(double);
    typedef  bool (Parameter::* BoolIntConstSign       )(int   ) const;
    typedef  bool (Parameter::* BoolDoubleConstSign    )(double) const;
  //typedef  bool (Parameter::* BoolBoolSign           )(bool  );
  //typedef  bool (Parameter::* BoolIntSign            )(int   );
  //typedef  bool (Parameter::* BoolDoubleSign         )(double);
  //typedef  bool (Parameter::* BoolStringSign         )(std::string);
  //typedef  bool (Parameter::* BoolStringFlagsSign    )(std::string,unsigned int);

  // Parameter overloaded function members.
    BoolIntConstSign     paramCheckValueInt    = (BoolIntConstSign)   &Parameter::checkValue;
    BoolDoubleConstSign  paramCheckValueDouble = (BoolDoubleConstSign)&Parameter::checkValue;

    implicitly_convertible<Parameter::Type    ,int>();
    implicitly_convertible<Parameter::Flags   ,int>();
    implicitly_convertible<Parameter::Priority,int>();

  //to_python_converter<const Parameter::Type,EnumToInt<const Parameter::Type> >();

    {
      scope  paramScope
        ( class_<Parameter>("Parameter", init<const std::string&, Parameter::Type, const std::string&
                                             ,optional<Parameter::Priority> >())
        .def("__repr__"          , reprParameter)
        .def("isFile"            , &Parameter::isFile)
        .def("isPath"            , &Parameter::isPath)
        .def("hasMin"            , &Parameter::hasMin)
        .def("hasMax"            , &Parameter::hasMax)
        .def("hasNeedRestart"    , &Parameter::hasNeedRestart)
        .def("hasMustExist"      , &Parameter::hasMustExist)
        .def("hasFlags"          , &Parameter::hasFlags)
        .def("getId"             , &Parameter::getId, return_value_policy<copy_const_reference>())
        .def("getMinInt"         , &Parameter::getMinInt)
        .def("getMaxInt"         , &Parameter::getMaxInt)
        .def("getMinDouble"      , &Parameter::getMinDouble)
        .def("getMaxDouble"      , &Parameter::getMaxDouble)
        .def("checkValue"        , paramCheckValueInt)
        .def("checkValue"        , paramCheckValueDouble)
        .def("asString"          , &Parameter::asString, return_value_policy<copy_const_reference>())
        .def("asPercentageString", &Parameter::asPercentageString)
        .def("asBool"            , &Parameter::asBool)
        .def("asInt"             , &Parameter::asInt)
        .def("asPercentage"      , &Parameter::asPercentage)
        .def("getSlaves"         , &Parameter::getSlaves, return_value_policy<reference_existing_object>())
        .def("getValues"         , &Parameter::getValues, return_value_policy<reference_existing_object>())
        .def("addValue"          , &Parameter::addValue)
        .def("addSlave"          , &Parameter::addSlave)
        .def("setPriority"       , &Parameter::setPriority)
        .def("setString"         , paramSetString1)
        .def("setString"         , paramSetString2)
        .def("setString"         , paramSetString3)
        .def("setBool"           , paramSetBool1)
        .def("setBool"           , paramSetBool2)
        .def("setInt"            , paramSetInt1)
        .def("setInt"            , paramSetInt2)
        .def("setDouble"         , paramSetDouble1)
        .def("setDouble"         , paramSetDouble2)
        .def("setPercentage"     , paramSetPercentage1)
        .def("setPercentage"     , paramSetPercentage2)
        .def("setMin"            , paramSetMinInt1)
        .def("setMin"            , paramSetMinDouble1)
        .def("setMax"            , paramSetMaxInt1)
        .def("setMax"            , paramSetMaxDouble1)
        .add_property("flags", &Parameter::getFlags, &Parameter::setFlags)
        .add_property("type" , &Parameter::getType)
        );

      class_<Parameter::EnumValue>("Parameter::EnumValue",init<std::string,int>())
        .def_readonly("label",&Parameter::EnumValue::_label)
        .def_readonly("value",&Parameter::EnumValue::_value)
        ;

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
        .value("UserFile"          ,Parameter::UserFile)
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

    {
      scope  layoutScope
        ( class_<LayoutDescription>("LayoutDescription", no_init)
        .def("addTab"      , layoutAddTab)
        .def("addRule"     , &LayoutDescription::addRule)
        .def("addTitle"    , &LayoutDescription::addTitle)
        .def("addSection"  , layoutAddSection2)
        .def("addSection"  , layoutAddSection3)
        .def("addParameter", layoutAddParameter3)
        .def("addParameter", layoutAddParameter4)
        .def("addParameter", layoutAddParameter5)
        .def("addParameter", layoutAddParameter6)
        );
    }

    typedef  std::map <const std::string,Parameter*>  ParametersMap;
    typedef  std::pair<const std::string,Parameter*>  ParametersPair;

    PairWrapper<ParametersPair>::wrap ( "ParametersPair", "key", "value" );
    MapWrapper <ParametersMap >::wrap ( "ParametersMap" );

  // Configuration overloaded function members.
    typedef  LayoutDescription& (Configuration::* LayoutVoidSign )();

    LayoutVoidSign  CfgGetLayout0 = (LayoutVoidSign)&Configuration::getLayout;

    {
      scope  confScope
        ( class_<Configuration>("Configuration", no_init)
        .def("get"                , &Configuration::get                , return_value_policy<reference_existing_object>())
        .def("getParameter"       , cfgGetParameter1                   , return_value_policy<reference_existing_object>())
        .def("getParameter"       , cfgGetParameter2                   , return_value_policy<reference_existing_object>())
        .def("addParameter"       , cfgAddParameter2                   , return_value_policy<reference_existing_object>())
        .def("addParameter"       , cfgAddParameter3                   , return_value_policy<reference_existing_object>())
        .def("readFromFile"       , &Configuration::readFromFile       )
        .def("pushDefaultPriority", &Configuration::pushDefaultPriority)
        .def("popDefaultPriority" , &Configuration::popDefaultPriority )
        .def("getDefaultPriority" , &Configuration::getDefaultPriority )
        .def("getParameters"      , &Configuration::getParameters      , return_value_policy<reference_existing_object>())
        .def("getLayout"          , CfgGetLayout0                      , return_value_policy<reference_existing_object>())
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

    def("hasParameter"      , hasParameter       );
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

    enum_<ParameterWidget::Flags>("ParameterWidgetFlags")
      .value("UseSpinBox",ParameterWidget::UseSpinBox)
      .value("IsFileName",ParameterWidget::IsFileName)
      .value("IsPathName",ParameterWidget::IsPathName)
      ;

    // {
    //   scope  parameterWidgetScope
    //     ( class_<ParameterWidget>("ParameterWidget", no_init)
    //     .def("getParameter", &ParameterWidget::getParameter, return_value_policy<reference_existing_object>())
    //     .def("hasFlags"    , &ParameterWidget::hasFlags    )
    //     .def("unsetFlags"  , &ParameterWidget::unsetFlags    )
    //     .add_property("flags", &ParameterWidget::getFlags, &ParameterWidget::setFlags)
    //     );

    //   implicitly_convertible<ParameterWidget::Flags,int>();

    //   enum_<ParameterWidget::Flags>("Flags")
    //     .value("UseSpinBox",ParameterWidget::UseSpinBox)
    //     .value("IsFileName",ParameterWidget::IsFileName)
    //     .value("IsPathName",ParameterWidget::IsPathName)
    //     ;
    // }

  } // End of Configuration BOOST_PYTHON_MODULE.



} // End of Cfg namespace.
