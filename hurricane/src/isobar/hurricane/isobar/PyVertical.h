
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
// $Id: PyVertical.h,v 1.6 2007/01/30 14:47:25 cobell Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Sophie BELLOEIL               |
// |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Header  :       "./PyVertical.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x





#ifndef __PYVERTICAL__
#define __PYVERTICAL__


#include "hurricane/isobar/PySegment.h"

#include "hurricane/Vertical.h"


namespace  Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyVertical".

  typedef struct {
      PySegment  _baseObject;
  } PyVertical;




// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeVertical;
  extern  PyMethodDef   PyVertical_Methods[];

  extern  PyObject* PyVertical_create     ( PyObject* module, PyObject* args );
  extern  PyObject* PyVertical_Link       ( Hurricane::Vertical* object );
  extern  void      PyVertical_LinkPyType ();



#define IsPyVertical(v) ((v)->ob_type == &PyTypeVertical)
#define PYVERTICAL(v)   ((PyVertical*)(v))
#define PYVERTICAL_O(v) (PYVERTICAL(v)->_baseObject._baseObject._baseObject._object)


}  // End of extern "C".




}  // End of Isobar namespace.
 



#endif
