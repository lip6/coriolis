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
    RoutingPad* currentRp     = NULL;

    forEach ( Occurrence, ioccurrence, hyperNet.getLeafPlugOccurrences() ) {
      nbRoutingPads++;

      currentRp = RoutingPad::create( this, *ioccurrence, RoutingPad::BiggestArea );
      if (flags & Cell::Flags::WarnOnUnplacedInstances)
        currentRp->isPlacedOccurrence ( RoutingPad::ShowWarning );

      if (nbRoutingPads == 1) {
      //Net* net =
        currentRp->getNet();
      //cerr << "_createRoutingPads on " << net->getName() << " buildRing:" << buildRing << endl;
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


  void DeepNet::_toJson( JsonWriter* writer ) const
  {
    Inherit::_toJson( writer );

    jsonWrite( writer, "_netOccurrence", &_netOccurrence );
  }


// -------------------------------------------------------------------
// Class  :  "JsonDeepNet".

  JsonDeepNet::JsonDeepNet ( unsigned long flags )
    : JsonNet(flags)
  {
    ltrace(51) << "JsonDeepNet::JsonDeepNet()" << endl;

    add( "_netOccurrence", typeid(Occurrence) );
  }


  JsonDeepNet::~JsonDeepNet ()
  { }


  string  JsonDeepNet::getTypeName () const
  { return "DeepNet"; }


  JsonDeepNet* JsonDeepNet::clone ( unsigned long flags ) const
  { return new JsonDeepNet ( flags ); }


  void JsonDeepNet::toData(JsonStack& stack)
  {
    ltracein(51);

    _stack = &stack;

    check( stack, "JsonDeepNet::toData" );
    presetId( stack );

    HyperNet hyperNet ( get<Occurrence>(stack,"_netOccurrence") );

    _net = DeepNet::create( hyperNet );
    _net->setGlobal   ( get<bool>(stack,"_isGlobal"   ) );
    _net->setExternal ( get<bool>(stack,"_isExternal" ) );
    _net->setAutomatic( get<bool>(stack,"_isAutomatic") );
    _net->setType     ( Net::Type     (get<string>(stack,"_type")) );
    _net->setDirection( Net::Direction(get<string>(stack,"_direction")) );
    
    setName( ".Net" );
    update( stack, _net );

    ltraceout(51);
  }


} // End of Hurricane namespace.
