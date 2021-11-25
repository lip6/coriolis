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
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DeepNet.cpp"                                 |
// +-----------------------------------------------------------------+


#include "hurricane/Warning.h"
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
    cdebug_log(18,0) << "DeepNet::DeepNet() " << getCell() << " " << this << endl;
    _netOccurrence.put( Uplink::create(this) );
  }


  DeepNet* DeepNet::create ( HyperNet& hyperNet )
  {
    if (not hyperNet.isValid())
      throw Error ( "Can't create " + _TName("DeepNet") + ": occurence is invalid." );

    if (not isHyperNetRootNetOccurrence(hyperNet.getNetOccurrence())) {
      cerr << Warning( "DeepNet::create(): Occurrence \"%s\" is not a root one."
                     , hyperNet.getNetOccurrence().getCompactString().c_str()
                     ) << endl;
    }

    Occurrence  rootNetOccurrence = getHyperNetRootNetOccurrence( hyperNet.getNetOccurrence() );

    if (rootNetOccurrence.getMasterCell()->isTerminalNetlist()) return NULL;
    if (rootNetOccurrence.getPath().isEmpty())                  return NULL;
    
    DeepNet* deepNet = new DeepNet( rootNetOccurrence );
    deepNet->_postCreate();

    return deepNet;
  }


  void  DeepNet::_preDestroy ()
  {
    _netOccurrence.removeProperty( Uplink::staticGetName() );
    Inherit::_preDestroy();
  }

  

  size_t  DeepNet::_createRoutingPads ( unsigned int flags )
  {
    cdebug_log(18,1) << "DeepNet::_createRoutingPads(): " << this << endl;

    size_t        nbRoutingPads = 0;
    HyperNet      hyperNet      ( _netOccurrence );
    RoutingPad*   currentRp     = NULL;
    bool          createRp      = true;
  //unsigned int  rpFlags       = (flags & Cell::Flags::StayOnPlugs) ? 0 : RoutingPad::BiggestArea;

    for ( Occurrence occurrence : hyperNet.getComponentOccurrences() ) {
      cdebug_log(18,0) << "| occurrence=" << occurrence << endl;
      RoutingPad* rp = dynamic_cast<RoutingPad*>( occurrence.getEntity() );
      cdebug_log(18,0) << "| rp=" << rp << endl;
      if (rp and (rp->getCell() == getCell())) { createRp = false; break; }
      if (dynamic_cast<Segment*>(occurrence.getEntity())) { createRp = false; break; }
      cdebug_log(18,0) << "| dynamic_cast OK createRp=" << createRp << endl;
    }
    if (not createRp) {
      cdebug_log(18,0) << "DeepNet::_createRoutingPads(): No RoutingPad created" << endl;
      cdebug_tabw(18,-1);
      return 0;
    }

    for ( Occurrence occurrence : hyperNet.getTerminalNetlistPlugOccurrences() ) {
      nbRoutingPads++;

      currentRp = RoutingPad::create( this, occurrence, RoutingPad::BiggestArea );
      if (flags & Cell::Flags::WarnOnUnplacedInstances)
        currentRp->isPlacedOccurrence( RoutingPad::ShowWarning );

      if (nbRoutingPads == 1) {
        currentRp->getNet();
      }
    }

    cdebug_log(18,0) << "DeepNet::_createRoutingPads(): done on " << this << endl;
    cdebug_tabw(18,-1);
    return nbRoutingPads;
  }


  Net* getDeepNet ( HyperNet& hypernet )
  {
    Occurrence  rootNetOccurrence = getHyperNetRootNetOccurrence( hypernet.getNetOccurrence() );
  
  //if (  rootNetOccurrence.getMasterCell()->isTerminalNetlist() ) return NULL;
  //if (  rootNetOccurrence.getPath().isEmpty() )                  return NULL;
  
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

  Initializer<JsonDeepNet>  jsonDeepNetInit ( 0 );

  void  JsonDeepNet::initialize ()
  { JsonTypes::registerType( new JsonDeepNet (JsonWriter::RegisterMode) ); }


  JsonDeepNet::JsonDeepNet ( unsigned long flags )
    : JsonNet(flags)
  {
    cdebug_log(19,0) << "JsonDeepNet::JsonDeepNet()" << endl;

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
    cdebug_tabw(19,1);

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

    cdebug_tabw(19,-1);
  }



// -------------------------------------------------------------------
// Class  :  "Uplink"

  Name  DeepNet::Uplink::_name = "DeepNet::Uplink";


  DeepNet::Uplink* DeepNet::Uplink::create ( DeepNet* uplink )
  {
    Uplink *property = new Uplink( uplink );
    property->_postCreate (); 
    return property;
  }


  void  DeepNet::Uplink::onReleasedBy ( DBo* owner )
  { PrivateProperty::onReleasedBy( owner ); }


  Name  DeepNet::Uplink::getPropertyName ()
  { return _name; }


  Name  DeepNet::Uplink::getName () const
  { return getPropertyName(); }


  string  DeepNet::Uplink::_getTypeName () const
  { return _TName( "DeepNet::Uplink" ); }


  string  DeepNet::Uplink::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(_uplink) );

    return s;
  }


  Record* DeepNet::Uplink::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if (record) {
      record->add( getSlot( "_name"  , _name   ) );
      record->add( getSlot( "_uplink", _uplink ) );
    }
    return record;
  }


} // End of Hurricane namespace.
