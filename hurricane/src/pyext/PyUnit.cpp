
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id: PyUnit.cpp,v 1.11 2006/05/03 14:00:05 jpc Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyUnit.cpp"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "PyUnit.h"


namespace Isobar {


USING_NAMESPACE_HURRICANE

extern "C" {


// x=================================================================x
// |                "PyUnit" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyUnit" General Methods                    |
  // x-------------------------------------------------------------x


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyUnit_GetUnit ()"

  extern PyObject* PyUnit_GetUnit ( PyObject* module, PyObject* args )
  {
    trace << "PyUnit_GetUnit ()" << endl;
    
    PyObject* arg0;
    Unit      result = 0;

    HTRY
    
    __cs.Init ( "Unit.GetUnit" );
    if ( ! PyArg_ParseTuple(args,"|O&:Unit.GetUnit",Converter,&arg0) )
      return ( NULL );
    
    if      ( __cs.GetObjectIds() == INT_ARG   ) { result = GetUnit ( PyInt_AsLong     ( arg0 ) ); }
    else if ( __cs.GetObjectIds() == FLOAT_ARG ) { result = GetUnit ( PyFloat_AsDouble ( arg0 ) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Unit.GetUnit constructor." );
      return ( NULL );
    }
    
    HCATCH
    
    return ( Py_BuildValue ( "i", result ) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyUnit_GetValue ()"

  extern PyObject* PyUnit_GetValue ( PyObject* module, PyObject* args )
  {
    trace << "PyUnit_GetValue ()" << endl;
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "GetValue", args,INT_ARG, &arg0 ) ) return ( NULL );

    return ( Py_BuildValue("d",GetValue(PyInt_AsLong(arg0))) );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyUnit_GetResolution ()"

  extern PyObject* PyUnit_GetResolution ( PyObject* module )
  {
    trace << "PyUnit_GetResolution ()" << endl;
    

    return ( Py_BuildValue("d",GetResolution()) );
  }


#endif  // End of Python Module Code Part.
      

}  // End of extern "C".


}  // End of Isobar namespace.
 
