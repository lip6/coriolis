
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
// $Id: PyHyperNet.h,v 1.2 2007/05/10 11:15:56 d2 Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                      Damien DUPUIS               |
// |  E-mail      :              Damien.Dupuis@lip6.fr               |
// | =============================================================== |
// |  C++ Header  :       "./PyHyperNet.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef __PYHYPERNET__
#define __PYHYPERNET__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/HyperNet.h"


namespace  Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyHyperNet".

  typedef struct {
      PyObject_HEAD
      Hurricane::HyperNet* _object;
  } PyHyperNet;




// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeHyperNet;
  extern PyMethodDef  PyHyperNet_Methods[];

  extern PyObject* PyHyperNet_create ( PyObject* module, PyObject* args );
  extern void      PyHyperNet_LinkPyType();


#define IsPyHyperNet(v)    ( (v)->ob_type == &PyTypeHyperNet )
#define PYHYPERNET(v)      ( (PyHyperNet*)(v) )
#define PYHYPERNET_O(v)    ( PYHYPERNET(v)->_object )


}  // End of extern "C".




}  // End of Isobar namespace.
 



# endif
