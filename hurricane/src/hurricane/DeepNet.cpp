// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
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
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DeepNet.cpp"                                 |
// +-----------------------------------------------------------------+


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


// -------------------------------------------------------------------
// Class  :  "DeepNet".


  DeepNet::DeepNet ( Occurrence& netOccurrence )
    : Net(netOccurrence.getOwnerCell()
         ,netOccurrence.getName()
         )
    , _netOccurrence(netOccurrence)
  {
  //trace << "DeepNet::DeepNet() " << getCell() << " " << this << endl;
  }


  DeepNet* DeepNet::create ( HyperNet& hyperNet )
  {
    if (not hyperNet.isValid())
      throw Error ( "Can't create " + _TName("DeepNet") + ": occurence is invalid." );

    Occurrence  rootNetOccurrence = getHyperNetRootNetOccurrence( hyperNet.getNetOccurrence() );

    if (rootNetOccurrence.getMasterCell()->isFlattenLeaf()) return NULL;
    if (rootNetOccurrence.getPath().isEmpty())              return NULL;

    DeepNet* deepNet = new DeepNet( rootNetOccurrence );
    deepNet->_postCreate();
    
    return deepNet;
  }


  size_t  DeepNet::_createRoutingPads ( unsigned int flags )
  {
    size_t      nbRoutingPads = 0;
    HyperNet    hyperNet      ( _netOccurrence );
    RoutingPad* previousRp    = NULL;
    RoutingPad* currentRp     = NULL;
    bool        buildRing     = false;

    forEach ( Occurrence, ioccurrence, hyperNet.getLeafPlugOccurrences() ) {
      nbRoutingPads++;

      currentRp = RoutingPad::create( this, *ioccurrence, RoutingPad::BiggestArea );
      if (flags & Cell::WarnOnUnplacedInstances)
        currentRp->isPlacedOccurrence ( RoutingPad::ShowWarning );

      if (nbRoutingPads == 1) {
        Net* net = currentRp->getNet();

        if (net->isGlobal()) {
          if      ( (flags & Cell::BuildClockRings ) and net->isClock () ) buildRing = true;
          else if ( (flags & Cell::BuildSupplyRings) and net->isSupply() ) buildRing = true;
        } else {
          buildRing = flags & Cell::BuildRings;
        }

      //cerr << "_createRoutingPads on " << net->getName() << " buildRing:" << buildRing << endl;
      }

      if (buildRing) {
        if (previousRp) {
          currentRp->getBodyHook()->attach( previousRp->getBodyHook() );
        }
        previousRp = currentRp;
      }
    }

    return nbRoutingPads;
  }


Net* getDeepNet ( HyperNet& hypernet )
{
  Occurrence  rootNetOccurrence = getHyperNetRootNetOccurrence( hypernet.getNetOccurrence() );

//if (  rootNetOccurrence.getMasterCell()->IsFlattenLeaf() ) return NULL;
//if (  rootNetOccurrence.getPath().isEmpty() )              return NULL;

  return rootNetOccurrence.getOwnerCell()->getNet(rootNetOccurrence.getName());
}


  Record* DeepNet::_getRecord () const
  {
    Record* record = Net::_getRecord();
    if (record) {
      record->add( getSlot("_netOccurrence", &_netOccurrence) );
    }
    return record;
  }


} // End of Hurricane namespace.
