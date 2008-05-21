
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
// $Id: PyInstanceLocator.h,v 1.4 2006/05/03 14:00:03 jpc Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                     Sophie BELLOEIL              |
// |  E-mail      :        Sophie.Belloeil@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :        "./PyInstanceLocator.h"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x





#ifndef __PYINSTANCELOCATOR__
#define __PYINSTANCELOCATOR__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/Locator.h"
#include "hurricane/Instance.h"


namespace  Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyInstanceLocator".

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Instance*>* _object;
  } PyInstanceLocator;




// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject  PyTypeInstanceLocator;
  extern PyMethodDef   PyInstanceLocator_Methods[];

  extern void      PyInstanceLocator_LinkPyType ();


#define IsPyInstanceLocator(v)    ( (v)->ob_type == &PyTypeInstanceLocator )
#define PYINSTANCELOCATOR(v)      ( (PyInstanceLocator*)(v) )
#define PYINSTANCELOCATOR_O(v)    ( PYINSTANCELOCATOR(v)->_object )


}  // End of extern "C".




}  // End of Isobar namespace.
 



#endif
