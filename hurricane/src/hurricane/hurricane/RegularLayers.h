
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/RegularLayers.h"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __HURRICANE_REGULAR_LAYERS_H__
#define __HURRICANE_REGULAR_LAYERS_H__


#include  "hurricane/Collection.h"


namespace Hurricane {


  class RegularLayer;


  typedef GenericCollection<RegularLayer*> RegularLayers;
  typedef GenericLocator<RegularLayer*>    RegularLayerLocator;
  typedef GenericFilter<RegularLayer*>     RegularLayerFilter;

#define  for_each_regular_layer(regularLayer, regularLayers) {  \
   RegularLayerLocator _locator = regularLayers.getLocator();   \
   while ( _locator.isValid() ) {                               \
       RegularLayer* regularLayer = _locator.getElement();      \
       _locator.progress();

}


#endif // __HURRICANE_REGULAR_LAYERS__
