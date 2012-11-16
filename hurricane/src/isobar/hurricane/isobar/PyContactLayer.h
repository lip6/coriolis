
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
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyContactLayer.h"           |
// +-----------------------------------------------------------------+


# ifndef  __PYCONTACTLAYER__
# define  __PYCONTACTLAYER__

#include "hurricane/isobar/PyLayer.h"
#include "hurricane/ContactLayer.h"

namespace  Isobar {

using namespace Hurricane;

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyContactLayer".

  typedef struct {
      PyLayer  _baseObject;
  } PyContactLayer;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeContactLayer;
  extern  PyMethodDef   PyContactLayer_Methods[];

  extern  PyObject* PyContactLayer_Link       ( Hurricane::ContactLayer* object );
  extern  void      PyContactLayer_LinkPyType ();

# define IsPyContactLayer(v)    ( (v)->ob_type == &PyTypeContactLayer )
# define PYCONTACTLAYER(v)      ( (PyContactLayer*)(v) )
# define PYCONTACTLAYER_O(v)    ( dynamic_cast<ContactLayer*>(PYCONTACTLAYER(v)->_baseObject._object) )


}  // extern "C".

}  // Isobar namespace.
 
# endif
