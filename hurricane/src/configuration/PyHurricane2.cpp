// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universite 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyHurricane.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyHurricane2.h"


namespace Isobar2 {


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyHurricane" Shared Library Code Part              |
// +=================================================================+

  extern "C" {

    void  _deAlloc ( PyVoidPointer* self )
    {
      PyTypeManager* manager = PyTypeManager::get( self->ob_type );
      manager->_deAlloc( self );
    }
    
  }

  
  std::map< std::type_index, PyTypeManager* >  PyTypeManager::_managerByCppTypes;
  std::map< PyTypeObject*  , PyTypeManager* >  PyTypeManager::_managerByPyTypes;


  PyTypeManager::~PyTypeManager () { }


  void  PyTypeManager::_setTypeNames ( std::string className, std::string nspace )
  {
    size_t cppScope = className.find_last_of( "::" );
    if (cppScope != std::string::npos) className = className.substr( cppScope+1 );

    _cppTypeName = className;
    _pyTypeName  = "Py"+_cppTypeName;
    if (not nspace.empty()) {
      _cppTypeName.insert( 0, nspace+"." );
      _pyTypeName .insert( 0, nspace+"." );
    }
  }


# else // PyHurricane Shared Library Code Part.

// +=================================================================+
// |              "PyHurricane" Python Module Code Part              |
// +=================================================================+


#endif // End of Python Module Code Part.

}  // Isobar namespace.
