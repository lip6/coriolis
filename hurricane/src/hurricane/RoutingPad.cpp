// ****************************************************************************************************
// 
// This file is part of the Coriolis Project.
// Copyright (c) 2001-2006  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: RoutingPad.cpp
// Authors: H.Clement, M.Sroka
// ****************************************************************************************************

#include "RoutingPad.h"
#include "Net.h"
#include "NetExternalComponents.h"
#include "CompositeLayer.h"
#include "Plug.h"
#include "Pin.h"
#include "Segment.h"
#include "Horizontal.h"
#include "Vertical.h"
#include "Cell.h"
#include "Instance.h"
#include "Error.h"

namespace Hurricane {

// ****************************************************************************************************
// RoutingPad implementation
// ****************************************************************************************************

RoutingPad::RoutingPad(Net* net, const Point& p, Occurrence occurrence )
// **********************************************************************************
:  Inherit(net),
    _x(p.getX()),
    _y(p.getY()),
    _occurrence(occurrence)
{
}


RoutingPad* RoutingPad::create(Net* net, Occurrence occurrence)
// ***********************************************************
{
    if (!net)
        throw Error ("Can't create RoutingPad : NULL net");
    if (!occurrence.isValid())
        throw Error ("Can't create RoutingPag : Invalid occurrence");

    //TODO Gerer une contruction avec un composant externe, mais ce n'est pas prioritaire
    Plug*    plug    = NULL;
    Pin*     pin     = NULL;
    Contact* contact = NULL;
    Point position;

    if ( (plug = dynamic_cast<Plug*>(occurrence.getEntity()) ) ) {
      position = occurrence.getPath().getTransformation().getPoint( plug->getPosition() );
    } else if ( (pin = dynamic_cast<Pin*>(occurrence.getEntity()) ) ) {
      position = occurrence.getPath().getTransformation().getPoint( pin->getPosition() );
    } else if ( (contact = dynamic_cast<Contact*>(occurrence.getEntity()) ) ) {
      position = occurrence.getPath().getTransformation().getPoint( contact->getPosition() );
    }

    if ( !plug && !pin && !contact )
      throw Error ("Can't create RoutingPad : plug or pin occurrence *required*");

    RoutingPad* routingPad = new RoutingPad(net, position, occurrence);

    routingPad->_postCreate();

    return routingPad;
}

void  RoutingPad::_postCreate()
// ****************************
{
  Inherit::_postCreate();

  if (!_occurrence.getPath().isEmpty())
    _occurrence.getMasterCell()->_addSlaveEntity(_occurrence.getEntity(),this);
}

Unit RoutingPad::getX() const
// ***********************
{
    return _x;
}

Unit RoutingPad::getY() const
// ***********************
{
    return _y;
}

Box RoutingPad::getBoundingBox() const
// ********************************
{
  Component* component = _getEntityAsComponent();
  if ( component ) {
    return _occurrence.getPath().getTransformation().getBox ( component->getBoundingBox() );
  }

  return Box(getPosition());
}

Box RoutingPad::getBoundingBox(const BasicLayer* basicLayer) const
// ******************************************************
{
  Component* component = _getEntityAsComponent();
  if ( component )
    return _occurrence.getPath().getTransformation().getBox ( component->getBoundingBox(basicLayer) );

  return Box(getPosition());
}

Layer* RoutingPad::getLayer() const
// ******************************************************
{
  Component* component = _getEntityAsComponent();
  if ( component ) return component->getLayer ();

  return NULL;
}

Point RoutingPad::getSourcePosition() const
// ****************************************
{
  Segment* segment = _getEntityAsSegment();
  if ( segment )
    return _occurrence.getPath().getTransformation().getPoint ( segment->getSourcePosition() );

  return getPosition();
}

Point RoutingPad::getTargetPosition() const
// ****************************************
{
  Segment* segment = _getEntityAsSegment();
  if ( segment )
    return _occurrence.getPath().getTransformation().getPoint ( segment->getTargetPosition() );

  return getPosition();
}

Unit RoutingPad::getSourceX() const
// ********************************
{
  return getSourcePosition().getX();
}

Unit RoutingPad::getSourceY() const
// ********************************
{
  return getSourcePosition().getY();
}

Unit RoutingPad::getTargetX() const
// ********************************
{
  return getTargetPosition().getX();
}

Unit RoutingPad::getTargetY() const
// ********************************
{
  return getTargetPosition().getY();
}

Point RoutingPad::getCenter() const
// ****************************************
{
  Segment* segment = _getEntityAsSegment();
  if ( segment )
    return _occurrence.getPath().getTransformation().getPoint ( segment->getCenter() );

  return getPosition();
}


void RoutingPad::translate(const Unit& dx, const Unit& dy)
// ****************************************************
{
    if ((dx != 0) || (dy != 0)) {
        invalidate(true);
        _x += dx;
        _y += dy;
    }
}

void RoutingPad::setX(const Unit& x)
// ******************************
{
    setPosition(x, getY());
}

void RoutingPad::setY(const Unit& y)
// ******************************
{
    setPosition(getX(), y);
}

void RoutingPad::setPosition(const Unit& x, const Unit& y)
// ****************************************************
{
    setOffset(x, y);
}

void RoutingPad::setPosition(const Point& position)
// *********************************************
{
    setPosition(position.getX(), position.getY());
}

void RoutingPad::setOffset(const Unit& x, const Unit& y)
// ****************************************************
{
    invalidate(true);
    _x = x;
    _y = y;
}

void RoutingPad::_preDestroy()
// ***********************
{
// trace << "entering RoutingPad::preDestroy: " << this << endl;
// trace_in();


  if (!_occurrence.getPath().isEmpty())
    _occurrence.getMasterCell()->_removeSlaveEntity(_occurrence.getEntity(),this);
  Inherit::_preDestroy();

// trace << "exiting RoutingPad::preDestroy:" << endl;
// trace_out();
}

string RoutingPad::_getString() const
// *******************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " [" + getValueString(getX()));
    s.insert(s.length() - 1, " " + getValueString(getY()));
    s.insert(s.length() - 1, "] ");
    s.insert(s.length() - 1, getString(_occurrence));
    return s;
}

Record* RoutingPad::_getRecord() const
// **************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("X", &_x));
        record->add(getSlot("Y", &_y));
        record->add(getSlot("Occurrence",_occurrence));
    }
    return record;
}

Component*  RoutingPad::_getEntityAsComponent () const
// ***************************************************
{
  if ( _occurrence.isValid() )
    return dynamic_cast<Component*>( _occurrence.getEntity() );

  return NULL;
}

Segment*  RoutingPad::_getEntityAsSegment () const
// ***********************************************
{
  if ( _occurrence.isValid() )
    return dynamic_cast<Segment*>( _occurrence.getEntity() );

  return NULL;
}

void RoutingPad::setExternalComponent(Component* component)
// ********************************************************
{
    if (isMaterialized()) invalidate(false);

    Occurrence plugOccurrence = getPlugOccurrence();
    Plug* plug= static_cast<Plug*>(plugOccurrence.getEntity());
    if (plug->getMasterNet() != component->getNet())
        throw Error("Cannot Set External Component to Routing Pad : Inconsistant Net");

    _occurrence.getMasterCell()->_removeSlaveEntity(_occurrence.getEntity(),this);
    _occurrence = Occurrence(component,Path(plugOccurrence.getPath(),plug->getInstance()));

    Point position = _occurrence.getPath().getTransformation().getPoint ( component->getPosition() );

    Horizontal* horizontal = dynamic_cast<Horizontal*>(component);
    if ( horizontal ) {
      setX ( 0 );
      setY ( position.getY() );
    } else {
      Vertical* vertical = dynamic_cast<Vertical*>(component);
      if ( vertical ) {
        setX ( position.getX() );
        setY ( 0 );
      } else
        setPosition ( position );
    }

    _occurrence.getMasterCell()->_addSlaveEntity(_occurrence.getEntity(),this);

    if (!isMaterialized()) {
      materialize();
    }
}


Occurrence RoutingPad::getPlugOccurrence()
// ***************************************
{
    if (is_a<Plug*>(_occurrence.getEntity()))
        return _occurrence;
    Component* component= static_cast<Component*>(_occurrence.getEntity());
    Net* net=component->getNet();
    Path path=_occurrence.getPath();
    if (path.isEmpty())
        throw Error("Empty Path => not in an instance");
    Instance* instance=path.getTailInstance();
    Plug* plug=instance->getPlug(net);
    return Occurrence(plug,path.getHeadPath());
}


void RoutingPad::restorePlugOccurrence()
// *************************************
{
    if (isMaterialized()) unmaterialize();

    _occurrence=getPlugOccurrence();
    setPosition ( _occurrence.getPath().getTransformation().getPoint
        ( dynamic_cast<Component*>(_occurrence.getEntity())->getPosition() ) );
}

RoutingPad* createRoutingPad ( Net* net, Occurrence plugOccurrence )
// *****************************************************************
{
    Component*      bestComponent  = NULL;
    Plug* plug = static_cast<Plug*>(plugOccurrence.getEntity());

    for_each_component ( component, getExternalComponents(plug->getMasterNet()) ) {
      if ( !bestComponent ) { bestComponent = component; continue; }
      if ( /*    IsOnTop(component->getLayer(),bestComponent->getLayer())
           ||*/ ( getArea(component) > getArea(bestComponent) ) ) {
        bestComponent = component;
      }
      end_for
    }

    if ( !bestComponent ) {
      string message = "CreateRoutingPad(): Cannot find external component of ";
      message += getString(plug->getMasterNet()) + " in ";
      message += getString(plug->getInstance())  + ".\n" ;
      throw Error ( message );
    }

    RoutingPad* rp = RoutingPad::create ( net, plugOccurrence );
    rp->setExternalComponent ( bestComponent );

    return rp;
}

RoutingPad* createRoutingPad ( Pin* pin )
// **************************************
{
  Occurrence  pinOccurrence ( pin, Path() );

# if 0
  for_each_routing_pad ( routingPad, pin->getNet()->getRoutingPads() ) {
    if ( routingPad->getOccurrence() == pinOccurrence )
      return routingPad;
    end_for
  }
# endif

  return RoutingPad::create ( pin->getNet(), pinOccurrence );
}


} // End of Hurricane namespace.
