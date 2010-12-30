
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DeepNet.cpp"                                 |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/DeepNet.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Plug.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Pin.h"
#include "hurricane/Contact.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Pad.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Error.h"


namespace Hurricane {

namespace {


  using namespace std;
  using namespace Hurricane;




# if !defined(__DOXYGEN_PROCESSOR__)


# endif


} // End of local namespace.


// x-----------------------------------------------------------------x
// |                 "::DeepNet" Class Definitions                   |
// x-----------------------------------------------------------------x


#   if !defined(__DOXYGEN_PROCESSOR__)


// -------------------------------------------------------------------
// Constructor  :  "DeepNet::DeepNet ()".

DeepNet::DeepNet ( Occurrence& netOccurrence )
  : Net(netOccurrence.getOwnerCell()
       ,netOccurrence.getName()
       )
  , _netOccurrence(netOccurrence)
{
}




// -------------------------------------------------------------------
// Inspector Management  :  "DeepNet::_getRecord ()".

Record* DeepNet::_getRecord () const
{
  Record* record = Net::_getRecord();
  if (record) {
        record->add(getSlot("_netOccurrence", &_netOccurrence));
  }
  return record;
}


# endif


// -------------------------------------------------------------------
// Constructor  :  "DeepNet::create ()".

DeepNet* DeepNet::create ( HyperNet& hyperNet )
{
  if ( !hyperNet.isValid() )
    throw Error ( "Can't create " + _TName("DeepNet") + ": occurence is invalid." );

  Occurrence  rootNetOccurrence = getHyperNetRootNetOccurrence ( hyperNet.getNetOccurrence() );

  if ( rootNetOccurrence.getMasterCell()->isFlattenLeaf() ) return NULL;
  if ( rootNetOccurrence.getPath().isEmpty() )              return NULL;

  DeepNet* deepNet = new DeepNet ( rootNetOccurrence );
  deepNet->_postCreate ();

  return deepNet;
}




// -------------------------------------------------------------------
// Internal Modifier  :  "DeepNet::_createRoutingPads ()".

size_t  DeepNet::_createRoutingPads ( bool buildRings )
{
  size_t  nbRoutingPads = 0;
  HyperNet  hyperNet ( _netOccurrence );

  RoutingPad* previousRP = NULL;
  RoutingPad* currentRP  = NULL;
  for_each_occurrence ( plugOccurrence, hyperNet.getLeafPlugOccurrences() ) {
    nbRoutingPads++;

    currentRP = RoutingPad::create ( this, plugOccurrence, RoutingPad::BiggestArea );
    if ( buildRings ) {
      if ( previousRP ) {
        currentRP->getBodyHook()->attach ( previousRP->getBodyHook() );
      }
      previousRP = currentRP;
    }

    end_for
  }

  return nbRoutingPads;
}


// -------------------------------------------------------------------
// 

Net* getDeepNet(HyperNet& hypernet)
// ********************************
{
    Occurrence  rootNetOccurrence = getHyperNetRootNetOccurrence ( hypernet.getNetOccurrence() );

  //if (  rootNetOccurrence.getMasterCell()->IsFlattenLeaf() ) return NULL;
  //if (  rootNetOccurrence.getPath().isEmpty() )              return NULL;

    return  rootNetOccurrence.getOwnerCell()->getNet(rootNetOccurrence.getName());

}


} // End of Hurricane namespace.
