
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
// |  C++ Header  :  "./hurricane/TransistorLayers.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __HURRICANE_TRANSISTOR_LAYERS_H__
#define __HURRICANE_TRANSISTOR_LAYERS_H__


#include  "hurricane/Collection.h"


namespace Hurricane {


  class TransistorLayer;


  typedef GenericCollection<TransistorLayer*> TransistorLayers;
  typedef GenericLocator<TransistorLayer*>    TransistorLayerLocator;
  typedef GenericFilter<TransistorLayer*>     TransistorLayerFilter;

#define  for_each_transistor_layer(transistorLayer, transistorLayers) { \
   TransistorLayerLocator _locator = transistorLayers.getLocator();     \
   while ( _locator.isValid() ) {                                       \
       TransistorLayer* transistorLayer = _locator.getElement();        \
       _locator.progress();

}


#endif  // __HURRICANE_TRANSISTOR_LAYERS__
