// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyLayoutDescription.cpp"                |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyLayoutDescription.h"
#include "hurricane/configuration/Configuration.h"


namespace  Cfg {

using namespace Hurricane;
using namespace Isobar3;

extern "C" {


// Thin wrappers for overloadeds member functions.


  void  addTab ( LayoutDescription* self, std::string tabName, std::string id )
  { self->addTab( tabName, id ); }

  void  addSection2 ( LayoutDescription* self, std::string tabName, std::string section )
  { self->addSection( tabName, section ); };


  void  addSection3 ( LayoutDescription* self
                    , std::string        tabName
                    , std::string        section
                    , int                column
                    )
  { self->addSection( tabName, section, column ); };

  void  addParameter3 ( LayoutDescription* self
                      , std::string        tabName
                      , std::string        id
                      , std::string        label
                      )
  { self->addParameter( tabName, id, label ); };


  void  addParameter4 ( LayoutDescription* self
                      , std::string        tabName
                      , std::string        id
                      , std::string        label
                      , int                column
                      )
  { self->addParameter( tabName, id, label, column ); };

  void  addParameter5 ( LayoutDescription* self
                      , std::string        tabName
                      , std::string        id
                      , std::string        label
                      , int                column
                      , int                span
                      )
  { self->addParameter( tabName, id, label, column, span ); };


  void  addParameter6 ( LayoutDescription* self
                      , std::string        tabName
                      , std::string        id
                      , std::string        label
                      , int                column
                      , int                span
                      , unsigned int       flags
                      )
  { self->addParameter( tabName, id, label, column, span, flags ); };


// Python methods.

  static PyObject* PyLayoutDescription_addTab ( PyVoidPointer* self, PyObject* args )
  { return callMethod("LayoutDescription.addTab",&addTab,self,args); }

  static PyObject* PyLayoutDescription_addRule ( PyVoidPointer* self, PyObject* args )
  { return callMethod("LayoutDescription.addRule",&LayoutDescription::addRule,self,args); }

  static PyObject* PyLayoutDescription_addTitle ( PyVoidPointer* self, PyObject* args )
  { return callMethod("LayoutDescription.addTitle",&LayoutDescription::addTitle,self,args); }

  static PyObject* PyLayoutDescription_addSection ( PyVoidPointer* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("LayoutDescription.addSection",&addSection3,self,args);
    if (not rvalue) rvalue = callMethod("LayoutDescription.addSection",&addSection2,self,args);
    return rvalue;
  }

  static PyObject* PyLayoutDescription_addParameter ( PyVoidPointer* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("LayoutDescription.addParameter",&addParameter6,self,args);
    if (not rvalue) rvalue = callMethod("LayoutDescription.addParameter",&addParameter5,self,args);
    if (not rvalue) rvalue = callMethod("LayoutDescription.addParameter",&addParameter4,self,args);
    if (not rvalue) rvalue = callMethod("LayoutDescription.addParameter",&addParameter3,self,args);
    return rvalue;
  }


  // ---------------------------------------------------------------
  // PyLayoutDescription Method table.

  PyMethodDef PyLayoutDescription_Methods[] =
    { { "addTab"              , (PyCFunction)PyLayoutDescription_addTab      , METH_VARARGS
                              , "Add a new Tab." }
    , { "addRule"             , (PyCFunction)PyLayoutDescription_addRule     , METH_VARARGS
                              , "Add a new ruler (separation)." }
    , { "addTitle"            , (PyCFunction)PyLayoutDescription_addTitle    , METH_VARARGS
                              , "Set the title." }
    , { "addSection"          , (PyCFunction)PyLayoutDescription_addSection  , METH_VARARGS
                              , "Add a new section." }
    , { "addParameter"        , (PyCFunction)PyLayoutDescription_addParameter, METH_VARARGS
                              , "Add a parameter." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


}  // extern "C".

}  // Cfg namespace.
