
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
// $Id: PyPin.h,v 1.9 2006/11/29 10:40:25 d2 Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :          Damien.Dupuis@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :            "./PyPin.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef __PYPIN__
#define __PYPIN__

#include "hurricane/isobar/PyContact.h"
#include "hurricane/Pin.h"


namespace  Isobar {

using namespace Hurricane;
    
extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyPin".

  typedef struct {
      PyContact _baseObject;
  } PyPin;

  
// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypePin;
  extern  PyMethodDef   PyPin_Methods[];

  extern  PyObject* PyPin_create     ( PyObject* module, PyObject* args );
  extern  PyObject* PyPin_Link       (Hurricane::Pin* object );
  extern  void      PinLoadConstants ( PyObject* dictionnary );
  extern  void      PyPin_LinkPyType ();


# define IsPyPin(v)    ( (v)->ob_type == &PyTypePin )
# define PYPIN(v)      ( (PyPin*)(v) )
# define PYPIN_O(v)    ( PYPIN(v)->_baseObject._baseObject._baseObject._object )

}  // End of extern "C".

}  // End of Isobar namespace.

# endif
