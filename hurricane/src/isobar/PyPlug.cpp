
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
// $Id: PyPlug.cpp,v 1.21 2008/02/07 17:09:41 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Sophie BELLOEIL               |
// |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyPlug.cpp"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyPlug.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/isobar/PyNet.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Plug,plug,function)


// x=================================================================x
// |                "PyPlug" Python Module Code Part                 |
// x=================================================================x

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyPlug" Attribute Methods                   |
  // x-------------------------------------------------------------x


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyPlug_isConnected,isConnected,PyPlug,Plug)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPlug_getInstance ()"

  static PyObject* PyPlug_getInstance ( PyPlug *self )
  {
    trace << "PyPlug_getInstance ()" << endl;
    
    Instance* instance = NULL;

    HTRY
    METHOD_HEAD ( "Plug.getInstance()" )
    instance = plug->getInstance ( );
    HCATCH    

    return PyInstance_Link ( instance );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPlug_getMasterNet ()"

  static PyObject* PyPlug_getMasterNet ( PyPlug *self )
  {
    trace << "PyPlug_getMasterNet ()" << endl;
    
    Net* net = NULL;

    HTRY
    METHOD_HEAD ( "Plug.getMasterNet()" )
    net = plug->getMasterNet ( );
    HCATCH
    
    return PyNet_Link ( net );
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPlug_getNet ()"

  static PyObject* PyPlug_getNet ( PyPlug *self ) {
    trace << "PyPlug_getNet ()" << endl;
    
    Net* net = NULL;

    HTRY
    METHOD_HEAD ( "Plug.getNet()" )
    net = plug->getNet ( );
    HCATCH
    
    return PyNet_Link ( net );
  }


 
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPlug_setNet ()"

  static PyObject* PyPlug_setNet ( PyPlug *self, PyObject* args ) {
    trace << "PyPlug_setNet()" << endl;

    HTRY
    
    METHOD_HEAD ( "Plug.setNet()" )

    PyNet* net;
    if ( ! ParseOneArg ( "Plug.setNet", args, NET_ARG, (PyObject**)&net ) ) return ( NULL );

    plug->setNet ( PYNET_O(net) );

    HCATCH

    Py_RETURN_NONE;
  }


  


  // ---------------------------------------------------------------
  // PyPlug Attribute Method table.

  PyMethodDef PyPlug_Methods[] =
    { { "getInstance"    , (PyCFunction)PyPlug_getInstance   , METH_NOARGS , "Return the instance to which belongs the plug." }
    , { "getMasterNet"   , (PyCFunction)PyPlug_getMasterNet  , METH_NOARGS , "Returns the external net referenced by the plug in the master cell of its instance." }
    , { "getNet"         , (PyCFunction)PyPlug_getNet  , METH_NOARGS , "Returns the  net connected by the plug in the cell of its instance." }
    , { "isConnected"    , (PyCFunction)PyPlug_isConnected   , METH_NOARGS , "Returns true if the plug is connected, else false." }
    , { "setNet"         , (PyCFunction)PyPlug_setNet        , METH_VARARGS, "This method allows to connect or change the net of a plug."  }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyPlug" Object Methods                    |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(Plug)
  PyTypeObjectLinkPyType(Plug)


# else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyPlug" Shared Library Code Part                 |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(Plug)


  // ---------------------------------------------------------------
  // PyPlug Object Definitions.
 
  PyTypeInheritedObjectDefinitions(Plug, Component)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
