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
// |  C++ Module  :       "./PyConfiguration.cpp"                    |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyConfiguration.h"
#include "hurricane/configuration/PyParameter.h"


namespace  Cfg {

using namespace Hurricane;
using namespace Isobar3;
  

extern "C" {

  static LayoutDescription* getLayout ( const Configuration* cf )
  { return const_cast<LayoutDescription*>( &(cf->getLayout()) ); }

  static Parameter* getParameter1 ( const Configuration* cf, std::string id )
  { return cf->getParameter( id ); }

  static Parameter* getParameter2 ( const Configuration* cf, std::string id, Parameter::Type type )
  { return cf->getParameter( id, type ); }

  static Parameter* addParameter4 ( Configuration*      cf
                                  , std::string         id
                                  , Parameter::Type     type
                                  , std::string         value
                                  , Parameter::Priority priority )
  {
    return cf->addParameter( id, type, value, priority );
  }

  static Parameter* addParameter3 ( Configuration*  cf
                                  , std::string     id
                                  , Parameter::Type type
                                  , std::string     value )
  {
    Parameter* p = cf->addParameter( id, type, value );
    return p;
  }


// Getter & Setters, for members/attributes.
  
  static PyObject* PyConfiguration_getFlags ( PyVoidPointer* self, void* closure )
  { return callMethod("Configuration.getFlags",&Configuration::getFlags,self,NULL); }

  static PyObject* PyConfiguration_setFlags ( PyVoidPointer* self, PyObject* value, void* closure )
  {
    callMethod("Configuration.setFlags",&Configuration::setFlags,self,PyTuple_Pack(1,value));
    return 0;
  }


// Object methods.
  
  static PyObject* PyConfiguration_get ( PyVoidPointer*, PyObject* args )
  { return callFunction("Configuration.get",&Configuration::get,args); }


  static PyObject* PyConfiguration_pushDefaultPriority ( PyVoidPointer*, PyObject* args )
  { return callFunction("Configuration.pushDefaultPriority",&Configuration::pushDefaultPriority,args); }


  static PyObject* PyConfiguration_popDefaultPriority ( PyVoidPointer* )
  { return callFunction("Configuration.popDefaultPriority",&Configuration::popDefaultPriority,NULL); }


  static PyObject* PyConfiguration_getDefaultPriority ( PyVoidPointer* )
  { return callFunction("Configuration.getDefaultPriority",&Configuration::getDefaultPriority,NULL); }


  static PyObject* PyConfiguration_getParameter ( PyVoidPointer* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Configuration.getParameter",&getParameter2,self,args);
    if (not rvalue) rvalue = callMethod("Configuration.getParameter",&getParameter1,self,args);
    return rvalue;
  }


  static PyObject* PyConfiguration_addParameter ( PyVoidPointer* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Configuration.addParameter",&addParameter4,self,args);
    if (not rvalue) rvalue = callMethod("Configuration.addParameter",&addParameter3,self,args);
    return rvalue;
  }


  static PyObject* PyConfiguration_getParameters ( PyVoidPointer* self )
  { return callMethod("Configuration.getParameters",&Configuration::getParameters,self,NULL); }


  static PyObject* PyConfiguration_getLayout ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Configuration.getLayout",&getLayout,self,args); }


  // ---------------------------------------------------------------
  // PyConfiguration Attribute Method table.

  PyMethodDef PyConfiguration_Methods[] =
    { { "get"                 , (PyCFunction)PyConfiguration_get                 , METH_NOARGS |METH_STATIC
                              , "Get the Configuration database." }
    , { "pushDefaultPriority" , (PyCFunction)PyConfiguration_pushDefaultPriority , METH_VARARGS|METH_STATIC
                              , "Push and set a new priority level." }
    , { "popDefaultPriority"  , (PyCFunction)PyConfiguration_popDefaultPriority  , METH_NOARGS |METH_STATIC
                              , "Pop and set the previous priority level." }
    , { "getDefaultPriority"  , (PyCFunction)PyConfiguration_getDefaultPriority  , METH_NOARGS |METH_STATIC
                              , "Get the current priority level." }
    , { "getParameter"        , (PyCFunction)PyConfiguration_getParameter        , METH_VARARGS
                              , "Return the named parameter (None if not found)." }
    , { "addParameter"        , (PyCFunction)PyConfiguration_addParameter        , METH_VARARGS
                              , "Create a new parameter." }
    , { "getParameters"       , (PyCFunction)PyConfiguration_getParameters       , METH_NOARGS
                              , "Return a dictionnary of all the parameters." }
    , { "getLayout"           , (PyCFunction)PyConfiguration_getLayout           , METH_NOARGS
                              , "Return the LayoutDescription object." }
  //, { "hasMenu"             , (PyCFunction)PyConfiguration_hasMenu             , METH_VARARGS
  //                          , "Return true if the menu at \"path\" exists." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyConfiguration Attributes/Properties table.

  PyGetSetDef PyConfiguration_Getsets[] =
    { { (char*)"flags", (getter)PyConfiguration_getFlags, (setter)PyConfiguration_setFlags, (char*)"flags attribute", NULL }
    , {NULL, NULL, NULL, NULL}   /* sentinel */
    };


}  // extern "C".

}  // Cfg namespace.
