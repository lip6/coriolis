
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyBanner.cpp"                                |
// +-----------------------------------------------------------------+


#include "crlcore/PyBanner.h"
#include <string>
#include <sstream>


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::string;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::__cs;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(Banner,banner,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |               "PyBanner" Python Module Code Part                |
// +=================================================================+


  static PyObject* PyBanner_new ( PyTypeObject* type, PyObject* args, PyObject* kwArgs )
  {
    trace << "PyBanner_new()" << endl;

    Banner*   banner   = NULL;
    PyBanner* pyBanner = (PyBanner*)type->tp_alloc(type,0);

    HTRY
    if ( pyBanner ) {
      char* name         = "";
      char* version      = "";
      char* purpose      = "";
      char* date         = "";
      char* authors      = "";
      char* contributors = "";

      static char* keywords[] = { "name", "version", "purpose", "date", "authors", "contributors", NULL };

      if (PyArg_ParseTupleAndKeywords( args
                                     , kwArgs
                                     , "|ssssss:Banner.new"
                                     , keywords
                                     , &name
                                     , &version
                                     , &purpose
                                     , &date
                                     , &authors
                                     , &contributors)) {
        banner = new Banner(name, version, purpose, date, authors, contributors);
      } else {
        PyErr_SetString ( ConstructorError, "CRL.Banner(): Invalid number/bad type of parameters." );
        return NULL;
      }
      pyBanner->_object = banner;
    }
    HCATCH

    return (PyObject*)pyBanner;
  }


  static PyObject* PyBanner_Repr ( PyBanner* self )
  {
    if ( self->ACCESS_OBJECT == NULL )
      return PyString_FromString("<PyObject unbound>");

    string representation;

    HTRY
    METHOD_HEAD("Banner.__repr__()")
    representation = getPrint(*banner);
    HCATCH

    return PyString_FromString(representation.c_str());
  }

  
  // Standart Accessors (Attributes).
  DirectGetStringAttribute(PyBanner_getName        ,getName        ,PyBanner,Banner)
  DirectGetStringAttribute(PyBanner_getVersion     ,getVersion     ,PyBanner,Banner)
  DirectGetStringAttribute(PyBanner_getPurpose     ,getPurpose     ,PyBanner,Banner)
  DirectGetStringAttribute(PyBanner_getDate        ,getDate        ,PyBanner,Banner)
  DirectGetStringAttribute(PyBanner_getAuthors     ,getAuthors     ,PyBanner,Banner)
  DirectGetStringAttribute(PyBanner_getContributors,getContributors,PyBanner,Banner)
  DirectGetUIntAttribute  (PyBanner_getScreenWidth ,getScreenWidth ,PyBanner,Banner)

  // Standart Mutators (Attributes).
  DirectSetCStringAttribute(PyBanner_setName        ,setName        ,"Banner.setName"        ,PyBanner,Banner)
  DirectSetCStringAttribute(PyBanner_setVersion     ,setVersion     ,"Banner.setVersion"     ,PyBanner,Banner)
  DirectSetCStringAttribute(PyBanner_setPurpose     ,setPurpose     ,"Banner.setPurpose"     ,PyBanner,Banner)
  DirectSetCStringAttribute(PyBanner_setDate        ,setDate        ,"Banner.setDate"        ,PyBanner,Banner)
  DirectSetCStringAttribute(PyBanner_setContributors,setContributors,"Banner.setContributors",PyBanner,Banner)
  DirectSetCStringAttribute(PyBanner_setAuthors     ,setAuthors     ,"Banner.setAuthors"     ,PyBanner,Banner)
  DirectSetLongAttribute   (PyBanner_setScreenWidth ,setScreenWidth ,"Banner.setScreenWidth" ,PyBanner,Banner)


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyBanner_destroy,PyBanner)


  PyMethodDef PyBanner_Methods[] =
    { { "getName"               , (PyCFunction)PyBanner_getName        , METH_NOARGS
                                , "Returns the banner's tool name." }
    , { "getVersion"            , (PyCFunction)PyBanner_getVersion     , METH_NOARGS
                                , "Returns the banner's version number." }
    , { "getPurpose"            , (PyCFunction)PyBanner_getPurpose     , METH_NOARGS
                                , "Returns the banner's purpose." }
    , { "getDate"               , (PyCFunction)PyBanner_getDate        , METH_NOARGS
                                , "Returns the banner's date." }
    , { "getAuthors"            , (PyCFunction)PyBanner_getAuthors     , METH_NOARGS
                                , "Returns the banner's authors." }
    , { "getContributors"       , (PyCFunction)PyBanner_getContributors, METH_NOARGS
                                , "Returns the banner's contributors." }
    , { "getScreenWidth"        , (PyCFunction)PyBanner_getScreenWidth , METH_NOARGS
                                , "Returns the terminal width used to display (justify) the banner." }
    , { "setName"               , (PyCFunction)PyBanner_setName        , METH_VARARGS
                                , "Sets the banner's tool name." }
    , { "setVersion"            , (PyCFunction)PyBanner_setVersion     , METH_VARARGS
                                , "Sets the banner's version number." }
    , { "setPurpose"            , (PyCFunction)PyBanner_setPurpose     , METH_VARARGS
                                , "Sets the banner's purpose." }
    , { "setDate"               , (PyCFunction)PyBanner_setDate        , METH_VARARGS
                                , "Sets the banner's date." }
    , { "setAuthors"            , (PyCFunction)PyBanner_setAuthors     , METH_VARARGS
                                , "Sets the banner's authors." }
    , { "setContributors"       , (PyCFunction)PyBanner_setContributors, METH_VARARGS
                                , "Sets the banner's contributors." }
    , { "setScreenWidth"        , (PyCFunction)PyBanner_setScreenWidth , METH_VARARGS
                                , "Sets the terminal width used to display (justify) the banner." }
  //, { "destroy"               , (PyCFunction)PyBanner_destroy               , METH_VARARGS
  //                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  PythonOnlyDeleteMethod(Banner)
  DirectHashMethod(PyBanner_Hash, PyBanner)

  extern void  PyBanner_LinkPyType() {
    trace << "PyBanner_LinkType()" << endl;

    PyTypeBanner.tp_new     =             PyBanner_new;
    PyTypeBanner.tp_dealloc = (destructor)PyBanner_DeAlloc;
    PyTypeBanner.tp_repr    = (reprfunc)  PyBanner_Repr;
    PyTypeBanner.tp_str     = (reprfunc)  PyBanner_Repr;
    PyTypeBanner.tp_hash    = (hashfunc)  PyBanner_Hash;
    PyTypeBanner.tp_methods =             PyBanner_Methods;
  }


#else  // End of Python Module Code Part.

// +=================================================================+
// |                "PyBanner" Shared Library Code Part              |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,Banner)

  // Link/Creation Method.
  LinkCreateMethod(Banner)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
