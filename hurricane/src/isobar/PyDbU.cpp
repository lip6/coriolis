
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
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyDbU.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyDbU.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


// x=================================================================x
// |                 "PyDbU" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                  "PyDbU" General Methods                    |
  // x-------------------------------------------------------------x


  // ---------------------------------------------------------------
  // Module Method  :  "PyDbU_db ()"

  extern PyObject* PyDbU_db ( PyObject* module, PyObject* args )
  {
    trace << "PyDbU_db ()" << endl;
    
    PyObject*  arg0;
    DbU::Unit  result = 0;

    HTRY
    
    __cs.Init ( "DbU.db" );
    if ( ! PyArg_ParseTuple(args,"|O&:DbU.db",Converter,&arg0) )
      return ( NULL );
    
    if ( __cs.getObjectIds() == INT_ARG   ) { result = DbU::db ( PyInt_AsLong ( arg0 ) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters or bad type for DbU.db converter." );
      return ( NULL );
    }
    
    HCATCH
    
    return ( Py_BuildValue ( "i", result ) );
  }


  // ---------------------------------------------------------------
  // Module Method  :  "PyDbU_grid ()"

  extern PyObject* PyDbU_grid ( PyObject* module, PyObject* args )
  {
    trace << "PyDbU_grid ()" << endl;
    
    PyObject*  arg0;
    DbU::Unit  result = 0;

    HTRY
    
    __cs.Init ( "DbU.grid" );
    if ( ! PyArg_ParseTuple(args,"|O&:DbU.grid",Converter,&arg0) )
      return ( NULL );
    
    if ( __cs.getObjectIds() == FLOAT_ARG   ) { result = DbU::grid ( PyFloat_AsDouble ( arg0 ) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters or bad type for DbU.grid converter." );
      return ( NULL );
    }
    
    HCATCH
    
    return ( Py_BuildValue ( "i", result ) );
  }


  // ---------------------------------------------------------------
  // Module Method  :  "PyDbU_lambda ()"

  extern PyObject* PyDbU_lambda ( PyObject* module, PyObject* args )
  {
    trace << "PyDbU_symbolic ()" << endl;
    
    PyObject*  arg0;
    DbU::Unit  result = 0;

    HTRY
    
    __cs.Init ( "DbU.symbolic" );
    if ( ! PyArg_ParseTuple(args,"|O&:DbU.symbolic",Converter,&arg0) )
      return ( NULL );
    
    if ( __cs.getObjectIds() == FLOAT_ARG   ) { result = DbU::lambda ( PyFloat_AsDouble ( arg0 ) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters or bad type for DbU.symbolic converter." );
      return ( NULL );
    }
    
    HCATCH
    
    return ( Py_BuildValue ( "i", result ) );
  }


  // ---------------------------------------------------------------
  // Module Method  :  "PyDbU_getDb ()"

  extern PyObject* PyDbU_getDb ( PyObject* module, PyObject* args )
  {
    trace << "PyDbU_getDb ()" << endl;
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "Dbu.getDb", args,INT_ARG, &arg0 ) ) return ( NULL );

    return ( Py_BuildValue("i",DbU::getDb(PyInt_AsLong(arg0))) );
  }


  // ---------------------------------------------------------------
  // Module Method  :  "PyDbU_getGrid ()"

  extern PyObject* PyDbU_getGrid ( PyObject* module, PyObject* args )
  {
    trace << "PyDbU_getGrid ()" << endl;
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "Dbu.getGrid", args,INT_ARG, &arg0 ) ) return ( NULL );

    return ( Py_BuildValue("d",DbU::getGrid(PyInt_AsLong(arg0))) );
  }


  // ---------------------------------------------------------------
  // Module Method  :  "PyDbU_getLambda ()"

  extern PyObject* PyDbU_getLambda ( PyObject* module, PyObject* args )
  {
    trace << "PyDbU_getLambda ()" << endl;
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "Dbu.getLambda", args,INT_ARG, &arg0 ) ) return ( NULL );

    return ( Py_BuildValue("d",DbU::getLambda(PyInt_AsLong(arg0))) );
  }


  // ---------------------------------------------------------------
  // Module Method  :  "PyDbU_getResolution ()"

  extern PyObject* PyDbU_getResolution ( PyObject* module )
  {
    trace << "PyDbU_getResolution ()" << endl;
    
    return ( Py_BuildValue("d",DbU::getResolution()) );
  }


#endif  // End of Python Module Code Part.
      

}  // End of extern "C".


}  // End of Isobar namespace.
 
