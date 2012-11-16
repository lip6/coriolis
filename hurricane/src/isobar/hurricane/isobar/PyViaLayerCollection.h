
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
// |  C++ Header  :  "./hurricane/isobar/PyViaLayerCollection.h"     |
// +-----------------------------------------------------------------+


#ifndef __PY_VIALAYER_COLLECTION__
#define __PY_VIALAYER_COLLECTION__

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/ViaLayers.h"


namespace Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyViaLayerCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::ViaLayers* _object;
  } PyViaLayerCollection;


  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::ViaLayer*>* _object;
      PyViaLayerCollection* _collection;
  } PyViaLayerCollectionLocator;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeViaLayerCollection;
  extern PyTypeObject PyTypeViaLayerCollectionLocator;

  extern void PyViaLayerCollection_LinkPyType();


}  // extern "C".

}  // Isobar namespace.
 
#endif
