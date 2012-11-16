
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyGraphicToolEngine.cpp"                     |
// +-----------------------------------------------------------------+


#include "crlcore/PyGraphicToolEngine.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"


namespace  CRL {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(GraphicTool,gtool,function)


// +=================================================================+
// |            "PyGraphicTool" Python Module Code Part              |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  GetNameMethod(GraphicTool, gtool)

  // Standart destroy (Attribute).


  PyMethodDef PyGraphicTool_Methods[] =
    { { "getName"             , (PyCFunction)PyGraphicTool_getName, METH_NOARGS
                              , "Returns the name of the GraphicTool (class attribute)." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyGraphicTool Type Methods.


  PythonOnlyDeleteMethod(GraphicTool)
  PyTypeObjectLinkPyType(GraphicTool)


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PyGraphicTool" Shared Library Code Part              |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(GraphicTool)

  PyTypeRootObjectDefinitions(GraphicTool)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace. 
