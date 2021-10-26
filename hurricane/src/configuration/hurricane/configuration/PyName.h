// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/configuration/PyName.h"            |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/Name.h"


inline PyObject* cToPy ( Hurricane::Name name )
{ return PyUnicode_FromString( getString(name).c_str() ); }


inline bool  pyToC ( PyObject* pyArg, const Hurricane::Name* arg )
{
//std::cerr << "pyToC<Hurricane::Name>() called." << std::endl;
  if (not PyUnicode_Check(pyArg)) return false;
  PyObject* pyBytes = PyUnicode_AsASCIIString( pyArg );
  *(const_cast<Hurricane::Name*>(arg)) = Hurricane::Name( PyBytes_AsString( pyBytes ));
  Py_DECREF( pyBytes );
  return true;
}


inline bool  pyToC ( PyObject* pyArg, Hurricane::Name* arg )
{ return pyToC( pyArg, const_cast<const Hurricane::Name*>(arg) ); }
