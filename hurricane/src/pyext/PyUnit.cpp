
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




#include "hurricane/isobar/PyUnit.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


// x=================================================================x
// |                "PyUnit" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyUnit" General Methods                    |
  // x-------------------------------------------------------------x


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyUnit_getUnit ()"

  extern PyObject* PyUnit_getUnit ( PyObject* module, PyObject* args )
  {
    trace << "PyUnit_getUnit ()" << endl;
    
    PyObject* arg0;
    Unit      result = 0;

    HTRY
    
    __cs.Init ( "Unit.getUnit" );
    if ( ! PyArg_ParseTuple(args,"|O&:Unit.getUnit",Converter,&arg0) )
      return ( NULL );
    
    if      ( __cs.getObjectIds() == INT_ARG   ) { result = getUnit ( PyInt_AsLong     ( arg0 ) ); }
    else if ( __cs.getObjectIds() == FLOAT_ARG ) { result = getUnit ( PyFloat_AsDouble ( arg0 ) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Unit.getUnit constructor." );
      return ( NULL );
    }
    
    HCATCH
    
    return ( Py_BuildValue ( "i", result ) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyUnit_getValue ()"

  extern PyObject* PyUnit_getValue ( PyObject* module, PyObject* args )
  {
    trace << "PyUnit_getValue ()" << endl;
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "getValue", args,INT_ARG, &arg0 ) ) return ( NULL );

    return ( Py_BuildValue("d",getValue(PyInt_AsLong(arg0))) );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyUnit_getResolution ()"

  extern PyObject* PyUnit_getResolution ( PyObject* module )
  {
    trace << "PyUnit_getResolution ()" << endl;
    

    return ( Py_BuildValue("d",getResolution()) );
  }


#endif  // End of Python Module Code Part.
      

}  // End of extern "C".


}  // End of Isobar namespace.
 
