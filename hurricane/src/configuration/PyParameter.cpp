// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyParameter.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyParameter.h"
#include "hurricane/configuration/Configuration.h"


namespace  Cfg {

using namespace Hurricane;
using namespace Isobar3;

extern "C" {


// Thin wrappers for overloadeds member functions.

  static bool  checkValueInt ( const Parameter* p, int value )
  { return p->checkValue( value ); }

  static bool  checkValueDouble ( const Parameter* p, double value )
  { return p->checkValue( value ); }

  static bool  setString1 ( Parameter* self, std::string value )
  { return self->setString(value); }

  static bool  setString2 ( Parameter* self, std::string value, unsigned int flags )
  { return self->setString(value,Configuration::getDefaultPriority(),flags); }

  static bool  setString3 ( Parameter* self, std::string value, unsigned int flags, Parameter::Priority pri )
  { return self->setString(value,pri,flags); }

  static bool  setBool1        ( Parameter* self, bool   value ) { return self->setBool(value); }
  static bool  setInt1         ( Parameter* self, int    value ) { return self->setInt(value); }
  static bool  setDouble1      ( Parameter* self, double value ) { return self->setDouble(value); }
  static bool  setPercentage1  ( Parameter* self, double value ) { return self->setPercentage(value); }
  static bool  setPercentage1i ( Parameter* self, int    value ) { return self->setPercentage((double)value); }

  static bool  setBool2        ( Parameter* self, bool   value, Parameter::Priority pri ) { return self->setBool(value,pri); }
  static bool  setInt2         ( Parameter* self, int    value, Parameter::Priority pri ) { return self->setInt(value,pri); }
  static bool  setDouble2      ( Parameter* self, double value, Parameter::Priority pri ) { return self->setDouble(value,pri); }
  static bool  setPercentage2  ( Parameter* self, double value, Parameter::Priority pri ) { return self->setPercentage(value,pri); }
  static bool  setPercentage2i ( Parameter* self, int    value, Parameter::Priority pri ) { return self->setPercentage((double)value,pri); }

  static void  setMinInt    ( Parameter* self, int    min ) { self->setMin(min); }
  static void  setMinDouble ( Parameter* self, double min ) { self->setMin(min); }
  static void  setMaxInt    ( Parameter* self, int    max ) { self->setMax(max); }
  static void  setMaxDouble ( Parameter* self, double max ) { self->setMax(max); }


// Python getters & setters.

  static PyObject* PyParameter_getFlags ( PyObject* self, void* closure )
  { return callMethod("Parameter.getFlags",&Parameter::getFlags,self,NULL); }

  static PyObject* PyParameter_setFlags ( PyObject* self, PyObject* value, void* closure )
  {
    callMethod("Parameter.setFlags",&Parameter::setFlags,self,PyTuple_Pack(1,value));
    return 0;
  }

  static PyObject* PyParameter_getType ( PyObject* self, void* closure )
  { return callMethod("Parameter.getType",&Parameter::getType,self,NULL); }


// Python methods.

  static PyObject* PyParameter_isFile ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.isFile",&Parameter::isFile,self,args); }

  static PyObject* PyParameter_isPath ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.isPath",&Parameter::isPath,self,args); }

  static PyObject* PyParameter_hasMin ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.hasMin",&Parameter::hasMin,self,args); }

  static PyObject* PyParameter_hasMax ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.hasMax",&Parameter::hasMax,self,args); }

  static PyObject* PyParameter_hasNeedRestart ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.hasNeedRestart",&Parameter::hasNeedRestart,self,args); }

  static PyObject* PyParameter_hasMustExist ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.hasMustExist",&Parameter::hasMustExist,self,args); }

  static PyObject* PyParameter_hasFlags ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.hasFlags",&Parameter::hasFlags,self,args); }
  
  static PyObject* PyParameter_getId ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.getId",&Parameter::getId,self,args); }

  static PyObject* PyParameter_getMinInt ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.getMinInt",&Parameter::getMinInt,self,args); }

  static PyObject* PyParameter_getMaxInt ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.getMaxInt",&Parameter::getMaxInt,self,args); }

  static PyObject* PyParameter_getMinDouble ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.getMinDouble",&Parameter::getMinDouble,self,args); }

  static PyObject* PyParameter_getMaxDouble ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.getMaxDouble",&Parameter::getMaxDouble,self,args); }

  static PyObject* PyParameter_checkValue ( PyObject* self, PyObject* args )
  {
    PyObject* rvalue = callMethod("Parameter.checkValue",&checkValueInt,self,args);
    if (not rvalue) 
      rvalue = callMethod("Parameter.checkValue",&checkValueDouble,self,args);
    return rvalue;
  }
  
  static PyObject* PyParameter_asBool ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.asBool",&Parameter::asBool,self,args); }
  
  static PyObject* PyParameter_asInt ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.asInt",&Parameter::asInt,self,args); }
  
  static PyObject* PyParameter_asDouble ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.asDouble",&Parameter::asDouble,self,args); }
  
  static PyObject* PyParameter_asPercentage ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.asPercentage",&Parameter::asPercentage,self,args); }
  
  static PyObject* PyParameter_asString ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.asString",&Parameter::asString,self,args); }

  static PyObject* PyParameter_asPercentageString ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.asPercentageString",&Parameter::asPercentageString,self,args); }

  static PyObject* PyParameter_addValue ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.addValue",&Parameter::addValue,self,args); }

  static PyObject* PyParameter_addSlave ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.addSlave",&Parameter::addSlave,self,args); }

  static PyObject* PyParameter_setPriority ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.setPriority",&Parameter::setPriority,self,args); }

  static PyObject* PyParameter_setString ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Parameter.setString",&setString3,self,args);
    if (not rvalue) rvalue = callMethod("Parameter.setString",&setString2,self,args);
    if (not rvalue) rvalue = callMethod("Parameter.setString",&setString1,self,args);
    return rvalue;
  }

  static PyObject* PyParameter_setBool ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Parameter.setBool",&setBool2,self,args);
    if (not rvalue) rvalue = callMethod("Parameter.setBool",&setBool1,self,args);
    return rvalue;
  }

  static PyObject* PyParameter_setInt ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Parameter.setInt",&setInt2,self,args);
    if (not rvalue) rvalue = callMethod("Parameter.setInt",&setInt1,self,args);
    return rvalue;
  }

  static PyObject* PyParameter_setDouble ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Parameter.setDouble",&setDouble2,self,args);
    if (not rvalue) rvalue = callMethod("Parameter.setDouble",&setDouble1,self,args);
    return rvalue;
  }

  static PyObject* PyParameter_setPercentage ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Parameter.setPercentage",&setPercentage2 ,self,args);
    if (not rvalue) rvalue = callMethod("Parameter.setPercentage",&setPercentage2i,self,args);
    if (not rvalue) rvalue = callMethod("Parameter.setPercentage",&setPercentage1 ,self,args);
    if (not rvalue) rvalue = callMethod("Parameter.setPercentage",&setPercentage1i,self,args);
    return rvalue;
  }

  static PyObject* PyParameter_setMin ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Parameter.setMin",&setMinDouble,self,args);
    if (not rvalue) rvalue = callMethod("Parameter.setMin",&setMinInt   ,self,args);
    return rvalue;
  }

  static PyObject* PyParameter_setMax ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Parameter.setMax",&setMaxDouble,self,args);
    if (not rvalue) rvalue = callMethod("Parameter.setMax",&setMaxInt   ,self,args);
    return rvalue;
  }

  static PyObject* PyParameter_getSlaves ( PyObject* self, PyObject* args )
  { return callMethod("Parameter.getSlaves",&Parameter::getSlaves,self,args); }


  // ---------------------------------------------------------------
  // PyParameter Method table.

  PyMethodDef PyParameter_Methods[] =
    { { "isFile"              , (PyCFunction)PyParameter_isFile             , METH_NOARGS
                              , "Tells if this parameter (string) holds a file name." }
    , { "isPath"              , (PyCFunction)PyParameter_isPath             , METH_NOARGS
                              , "Tells if this parameter (string) holds a path." }
    , { "hasMin"              , (PyCFunction)PyParameter_hasMin             , METH_NOARGS
                              , "Tells if this parameter (int) has a lower bound." }
    , { "hasMax"              , (PyCFunction)PyParameter_hasMax             , METH_NOARGS
                              , "Tells if this parameter (int) has a higher bound." }
    , { "hasNeedRestart"      , (PyCFunction)PyParameter_hasNeedRestart     , METH_NOARGS
                              , "Tells if this parameter needs a restart to be taken into account." }
    , { "hasMustExist"        , (PyCFunction)PyParameter_hasMustExist       , METH_NOARGS
                              , "Tells if the file/path must exists." }
    , { "hasFlags"            , (PyCFunction)PyParameter_hasFlags           , METH_VARARGS
                              , "Tells if the file/path must exists." }
    , { "getId"               , (PyCFunction)PyParameter_getId              , METH_VARARGS
                              , "Return the string identifier of the parameter (unique)." }
    , { "getMinInt"           , (PyCFunction)PyParameter_getMinInt          , METH_VARARGS
                              , "Return the integer lower bound (if set)." }
    , { "getMaxInt"           , (PyCFunction)PyParameter_getMaxInt          , METH_VARARGS
                              , "Return the integer upper bound (if set)." }
    , { "getMinDouble"        , (PyCFunction)PyParameter_getMinDouble       , METH_VARARGS
                              , "Return the float/double lower bound (if set)." }
    , { "getMaxDouble"        , (PyCFunction)PyParameter_getMaxDouble       , METH_VARARGS
                              , "Return the float/double upper bound (if set)." }
    , { "checkValue"          , (PyCFunction)PyParameter_checkValue         , METH_VARARGS
                              , "Check if the parameter value is within the defined bounds." }
    , { "asBool"              , (PyCFunction)PyParameter_asBool             , METH_VARARGS
                              , "Return the parameter value, as a boolean." }
    , { "asInt"               , (PyCFunction)PyParameter_asInt              , METH_VARARGS
                              , "Return the parameter value, as an integer." }
    , { "asDouble"            , (PyCFunction)PyParameter_asDouble           , METH_VARARGS
                              , "Return the parameter value, as a float/double." }
    , { "asPercentage"        , (PyCFunction)PyParameter_asPercentage       , METH_VARARGS
                              , "Return the parameter value, as a percentage (float)." }
    , { "asString"            , (PyCFunction)PyParameter_asString           , METH_VARARGS
                              , "Return the parameter value, as a string." }
    , { "asPercentageString"  , (PyCFunction)PyParameter_asPercentageString , METH_VARARGS
                              , "Return the parameter value expressed in percentage (for double)." }
    , { "addValue"            , (PyCFunction)PyParameter_addValue           , METH_VARARGS
                              , "Add a new value to parameter of enumerated type." }
    , { "addSlave"            , (PyCFunction)PyParameter_addSlave           , METH_VARARGS
                              , "Add a new slave to this parameter." }
    , { "setPriority"         , (PyCFunction)PyParameter_setPriority        , METH_VARARGS
                              , "Set the modification threshold (priority) of the parameter." }
    , { "setString"           , (PyCFunction)PyParameter_setString          , METH_VARARGS
                              , "Set the parameter value as a string." }
    , { "setBool"             , (PyCFunction)PyParameter_setBool            , METH_VARARGS
                              , "Set the parameter value as a bool." }
    , { "setInt"              , (PyCFunction)PyParameter_setInt             , METH_VARARGS
                              , "Set the parameter value as a integer." }
    , { "setDouble"           , (PyCFunction)PyParameter_setDouble          , METH_VARARGS
                              , "Set the parameter value as a float/double." }
    , { "setPercentage"       , (PyCFunction)PyParameter_setPercentage      , METH_VARARGS
                              , "Set the parameter value as a float/double, expressed in percentage." }
    , { "setMin"              , (PyCFunction)PyParameter_setMin             , METH_VARARGS
                              , "Set the parameter lower bound (either float or integer)." }
    , { "setMax"              , (PyCFunction)PyParameter_setMax             , METH_VARARGS
                              , "Set the parameter upper bound (either float or integer)." }
    , { "getSlaves"           , (PyCFunction)PyParameter_getSlaves          , METH_VARARGS
                              , "Returns the list of slaved parameters to this one (by ids)." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };

  PyMethodDef PyParameterPriority_Methods[] = { {NULL, NULL, 0, NULL}   /* sentinel */ };
  PyMethodDef PyParameterType_Methods    [] = { {NULL, NULL, 0, NULL}   /* sentinel */ };
  PyMethodDef PyParameterFlags_Methods   [] = { {NULL, NULL, 0, NULL}   /* sentinel */ };


  // ---------------------------------------------------------------
  // PyParameter Attriutes/Properties table.

  PyGetSetDef PyParameter_Getsets[] =
    { { (char*)"flags", (getter)PyParameter_getFlags, (setter)PyParameter_setFlags, (char*)"flags attribute", NULL }
    , { (char*)"type" , (getter)PyParameter_getType , NULL                        , (char*)"type attribute" , NULL }
    , {NULL, NULL, NULL, NULL}   /* sentinel */
    };


}  // extern "C".

}  // Cfg namespace.
