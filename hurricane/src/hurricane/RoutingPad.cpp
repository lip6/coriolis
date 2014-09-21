// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2014, All Rights Reserved
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
// |  Authors     :          Hugo Clement & Marek Sroka              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./RoutingPad.cpp"                              |
// +-----------------------------------------------------------------+


#include <sstream>
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Layer.h"
#include "hurricane/Plug.h"
#include "hurricane/Pin.h"
#include "hurricane/Segment.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Warning.h"
#include "hurricane/Error.h"
#include "hurricane/RoutingPad.h"


namespace Hurricane {

  using  std::ostringstream;


  RoutingPad::RoutingPad ( Net* net, Occurrence occurrence )
    :  Inherit   (net)
    , _occurrence(occurrence)
  { }


  RoutingPad* RoutingPad::create ( Net* net, Occurrence occurrence, unsigned int flags )
  {
    if ( not net ) throw Error ("Can't create RoutingPad : NULL net");
    if ( not occurrence.isValid() ) throw Error ("Can't create RoutingPag : Invalid occurrence");

    Plug*    plug    = NULL;
    Pin*     pin     = NULL;
    Contact* contact = NULL;

    if ( (plug = dynamic_cast<Plug*>(occurrence.getEntity()) ) == NULL) {
      if ( (pin = dynamic_cast<Pin*>(occurrence.getEntity()) ) == NULL) {
        contact = dynamic_cast<Contact*>(occurrence.getEntity());
      }
    }

    if ( (not plug) and (not pin) and (not contact) )
      throw Error ("Can't create RoutingPad : Plug, Pin, or Contact Occurrence *required*");

    RoutingPad* routingPad = new RoutingPad( net, occurrence );
    routingPad->_postCreate();

    if ( plug and (flags & ComponentSelection) )
      routingPad->setOnBestComponent( flags );
    if (not plug)
      routingPad->isPlacedOccurrence( flags );

    return routingPad;
  }


  void  RoutingPad::_postCreate ()
  {
    Inherit::_postCreate();

    if ( not _occurrence.getPath().isEmpty() )
      _occurrence.getMasterCell()->_addSlaveEntity(_occurrence.getEntity(),this);
  }


  RoutingPad* RoutingPad::create ( Pin* pin )
  {
    Occurrence  pinOccurrence ( pin, Path() );

#if 0
    forEach ( RoutingPad*, irp, pin->getNet()->getRoutingPads() ) {
      if ( (*irp)->getOccurrence() == pinOccurrence )
        return (*irp);
    }
#endif

    return RoutingPad::create ( pin->getNet(), pinOccurrence );
  }


  bool  RoutingPad::isPlacedOccurrence ( unsigned int flags ) const
  {
    vector<Instance*> unplaceds;
    forEach( Instance*, iinstance, _occurrence.getPath().getInstances() ) {
      if (iinstance->getPlacementStatus() == Instance::PlacementStatus::UNPLACED) {
        unplaceds.push_back( *iinstance );
      }
    }

    if (not unplaceds.empty() and (flags & ShowWarning)) {
      ostringstream message;
      message << "There are unplaced(s) instances in " << this << ":";
      for ( size_t i=0 ; i<unplaceds.size() ; ++i ) {
        message << "\n          * Instance <" << unplaceds[i]->getName() << ":"
                << unplaceds[i]->getMasterCell()->getName() << "> in Cell <"
                << unplaceds[i]->getCell()->getName() << ">";
          ;
      }
      cerr << Warning( message.str() ) << endl;;
    }

    return not unplaceds.empty();
  }


  DbU::Unit RoutingPad::getX       () const { return getPosition().getX(); }
  DbU::Unit RoutingPad::getY       () const { return getPosition().getY(); }
  DbU::Unit RoutingPad::getSourceX () const { return getSourcePosition().getX(); }
  DbU::Unit RoutingPad::getSourceY () const { return getSourcePosition().getY(); }
  DbU::Unit RoutingPad::getTargetX () const { return getTargetPosition().getX(); }
  DbU::Unit RoutingPad::getTargetY () const { return getTargetPosition().getY(); }


  Box RoutingPad::getBoundingBox () const
  {
    Component* component = _getEntityAsComponent();
    if ( component )
      return _occurrence.getPath().getTransformation().getBox ( component->getBoundingBox() );

    return Box(getPosition());
  }


  Box RoutingPad::getBoundingBox ( const BasicLayer* basicLayer ) const
  {
    Component* component = _getEntityAsComponent();
    if ( component )
      return _occurrence.getPath().getTransformation().getBox ( component->getBoundingBox(basicLayer) );

    return Box(getPosition());
  }


  const Layer* RoutingPad::getLayer () const
  {
    Component* component = _getEntityAsComponent();
    if (component) return component->getLayer ();

    return NULL;
  }


  Point RoutingPad::getPosition () const
  {
    Component* component = _getEntityAsComponent();
    if (component)
      return _occurrence.getPath().getTransformation().getPoint( component->getCenter() );

    return Point();
  }


  Point RoutingPad::getSourcePosition () const
  {
    Segment* segment = _getEntityAsSegment();
    if ( segment ) {
      return _occurrence.getPath().getTransformation().getPoint ( segment->getSourcePosition() );
    }

    return getPosition();
  }


  Point RoutingPad::getTargetPosition() const
  {
    Segment* segment = _getEntityAsSegment();
    if ( segment )
      return _occurrence.getPath().getTransformation().getPoint ( segment->getTargetPosition() );

    return getPosition();
  }


  Point RoutingPad::getCenter() const
  {
    Segment* segment = _getEntityAsSegment();
    if ( segment )
      return _occurrence.getPath().getTransformation().getPoint ( segment->getCenter() );
    
    return getPosition();
  }


  void RoutingPad::translate(const DbU::Unit& dx, const DbU::Unit& dy)
  {
  // Does nothing. The position is fixed and relative to the instance path.
  }


  void RoutingPad::_preDestroy ()
  {
  // trace << "entering RoutingPad::preDestroy: " << this << endl;
  // trace_in();

    if ( not _occurrence.getPath().isEmpty() )
      _occurrence.getMasterCell()->_removeSlaveEntity(_occurrence.getEntity(),this);
    Inherit::_preDestroy();

  // trace << "exiting RoutingPad::preDestroy:" << endl;
  // trace_out();
  }


  string RoutingPad::_getString () const
  {
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " [" + DbU::getValueString(getX()));
    s.insert(s.length() - 1, " " + DbU::getValueString(getY()));
    s.insert(s.length() - 1, "] ");
    s.insert(s.length() - 1, getString(_occurrence));
    return s;
  }


  Record* RoutingPad::_getRecord () const
  {
    Record* record = Inherit::_getRecord();
    if ( record ) {
      record->add(getSlot("_occurrence",_occurrence));
    }
    return record;
  }


  Component* RoutingPad::_getEntityAsComponent () const
  {
    if ( _occurrence.isValid() )
      return dynamic_cast<Component*>( _occurrence.getEntity() );

    return NULL;
  }


  Segment* RoutingPad::_getEntityAsSegment () const
  {
    if ( _occurrence.isValid() )
      return dynamic_cast<Segment*>( _occurrence.getEntity() );
    
    return NULL;
  }


  void RoutingPad::setExternalComponent ( Component* component )
  {
    if ( isMaterialized() ) invalidate(false);

    Occurrence plugOccurrence = getPlugOccurrence();
    Plug*      plug           = static_cast<Plug*>(plugOccurrence.getEntity());
    if ( plug->getMasterNet() != component->getNet() )
      throw Error("Cannot Set External Component to Routing Pad : Inconsistant Net");

    _occurrence.getMasterCell()->_removeSlaveEntity(_occurrence.getEntity(),this);
    _occurrence = Occurrence(component,Path(plugOccurrence.getPath(),plug->getInstance()));

    _occurrence.getMasterCell()->_addSlaveEntity(_occurrence.getEntity(),this);

    if (!isMaterialized()) materialize();
  }


  Occurrence RoutingPad::getPlugOccurrence ()
  {
    if (dynamic_cast<Plug*>(_occurrence.getEntity()))
      return _occurrence;

    Component* component = static_cast<Component*>(_occurrence.getEntity());
    Net*       net       = component->getNet();
    Path       path      = _occurrence.getPath();

    if ( path.isEmpty() )
      throw Error("Empty Path => not in an instance");

    Instance* instance = path.getTailInstance();
    Plug*     plug     = instance->getPlug(net);

    return Occurrence(plug,path.getHeadPath());
  }


  void RoutingPad::restorePlugOccurrence ()
  {
    if (isMaterialized()) unmaterialize();

    _occurrence=getPlugOccurrence();
  }


  Component* RoutingPad::setOnBestComponent ( unsigned int flags )
  {
    restorePlugOccurrence ();

    Component* bestComponent = NULL;
    Plug*      plug          = static_cast<Plug*>(_occurrence.getEntity());

    forEach ( Component*, icomponent, NetExternalComponents::get(plug->getMasterNet()) ) {
      if ( not bestComponent ) { bestComponent = *icomponent; continue; }

      switch ( flags & ComponentSelection ) {
        case LowestLayer:
          if ( icomponent->getLayer()->below(bestComponent->getLayer()) )
            bestComponent = *icomponent;
          break;
        case HighestLayer:
          if ( icomponent->getLayer()->above(bestComponent->getLayer()) )
            bestComponent = *icomponent;
          break;
        case BiggestArea:
        default:
          {
            double compArea = getArea(*icomponent);
            double bestArea = getArea(bestComponent);

            if (compArea == bestArea) {
              if (icomponent->getId() < bestComponent->getId())
                bestComponent = *icomponent;
            } else {
              if (compArea > bestArea)
                bestComponent = *icomponent;
            }
          }
          break;
      }
    }

    if ( not bestComponent )
      throw Error ( "RoutingPad::_getBestComponent(): No external components for\n"
                   "  %s of %s."
                  ,getString(plug->getMasterNet()).c_str()
                  ,getString(plug->getInstance ()).c_str() );

    setExternalComponent ( bestComponent );
    if (flags & ShowWarning) isPlacedOccurrence( flags );

    return bestComponent;
  }


}   // End of Hurricane namespace.
