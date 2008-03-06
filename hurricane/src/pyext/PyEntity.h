
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
// $Id: PyEntity.h,v 1.3 2006/05/03 14:00:02 jpc Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Sophie BELLOEIL               |
// |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Header  :       "./PyEntity.h"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x





#ifndef __PYENTITY__
#define __PYENTITY__


#include  "PyHurricane.h"

#include  "Entity.h"


namespace  Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyEntity".

  typedef struct {
      PyObject_HEAD
      Hurricane::Entity* _object;
  } PyEntity;




// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".


  extern  PyObject* PyEntity_NEW        ( Hurricane::Entity* entity );
  extern  void      PyEntity_LinkPyType ();

  extern  PyTypeObject  PyTypeEntity;
  extern  PyMethodDef   PyEntity_Methods[];


#define IsPyEntity(v)    ( (v)->ob_type == &PyTypeEntity )
#define PYENTITY(v)      ( (PyEntity*)(v) )
#define PYENTITY_O(v)    ( PYENTITY(v)->_object )


}  // End of extern "C".


Hurricane::Entity*  EntityCast ( PyObject* derivedObject );




}  // End of Isobar namespace.
 



#endif
