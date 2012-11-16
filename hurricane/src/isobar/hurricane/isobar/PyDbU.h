
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
//
// +-----------------------------------------------------------------+ 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyDbU.h"                    |
// +-----------------------------------------------------------------+


#ifndef  __PYUNIT__
#define  __PYUNIT__


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/DbU.h"


namespace  Isobar {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyDbU".
//
// This is nothing but a hollow shell to provide a "DbU" name space
// at Python level. DbU are just treaded as long long.

  typedef struct {
      PyObject_HEAD
  } PyDbU;



// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeDbU;
  extern  PyMethodDef   PyDbU_Methods[];

  extern  void          PyDbU_LinkPyType    ();
  extern  PyObject*     PyDbU_fromDb            ( PyObject* module, PyObject* args );
  extern  PyObject*     PyDbU_fromGrid          ( PyObject* module, PyObject* args );
  extern  PyObject*     PyDbU_fromLambda        ( PyObject* module, PyObject* args );
  extern  PyObject*     PyDbU_toDb              ( PyObject* module, PyObject* args );
  extern  PyObject*     PyDbU_toGrid            ( PyObject* module, PyObject* args );
  extern  PyObject*     PyDbU_toLambda          ( PyObject* module, PyObject* args );
  extern  PyObject*     PyDbU_toPhysical        ( PyObject* module, PyObject* args );
  extern  PyObject*     PyDbU_getResolution     ( PyObject* module );
  extern  PyObject*     PyDbU_getOnPhysicalGrid ( PyObject* module, PyObject* args );

  extern  void          DbULoadConstants        ( PyObject* dictionnary );
  extern  void          PyDbU_postModuleInit    ();


}  // extern "C".


}  // Isobar namespace.

#endif
