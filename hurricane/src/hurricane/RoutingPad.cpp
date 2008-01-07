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


RoutingPad* RoutingPad::Create(Net* net, Occurrence occurrence)
// ***********************************************************
{
    if (!net)
        throw Error ("Can't create RoutingPad : NULL net");
    if (!occurrence.IsValid())
        throw Error ("Can't create RoutingPag : Invalid occurrence");

    //TODO Gerer une contruction avec un composant externe, mais ce n'est pas prioritaire
    Plug*    plug    = NULL;
    Pin*     pin     = NULL;
    Contact* contact = NULL;
    Point position;

    if ( (plug = dynamic_cast<Plug*>(occurrence.GetEntity()) ) ) {
      position = occurrence.GetPath().GetTransformation().getPoint( plug->GetPosition() );
    } else if ( (pin = dynamic_cast<Pin*>(occurrence.GetEntity()) ) ) {
      position = occurrence.GetPath().GetTransformation().getPoint( pin->GetPosition() );
    } else if ( (contact = dynamic_cast<Contact*>(occurrence.GetEntity()) ) ) {
      position = occurrence.GetPath().GetTransformation().getPoint( contact->GetPosition() );
    }

    if ( !plug && !pin && !contact )
      throw Error ("Can't create RoutingPad : plug or pin occurrence *required*");

    RoutingPad* routingPad = new RoutingPad(net, position, occurrence);

    routingPad->_PostCreate();

    return routingPad;
}

void  RoutingPad::_PostCreate()
// ****************************
{
  Inherit::_PostCreate();

  if (!_occurrence.GetPath().IsEmpty())
    _occurrence.GetMasterCell()->_AddSlaveEntity(_occurrence.GetEntity(),this);
}

Unit RoutingPad::GetX() const
// ***********************
{
    return _x;
}

Unit RoutingPad::GetY() const
// ***********************
{
    return _y;
}

Box RoutingPad::GetBoundingBox() const
// ********************************
{
  Component* component = _GetEntityAsComponent();
  if ( component ) {
    return _occurrence.GetPath().GetTransformation().getBox ( component->GetBoundingBox() );
  }

  return Box(GetPosition());
}

Box RoutingPad::GetBoundingBox(const BasicLayer* basicLayer) const
// ******************************************************
{
  Component* component = _GetEntityAsComponent();
  if ( component )
    return _occurrence.GetPath().GetTransformation().getBox ( component->GetBoundingBox(basicLayer) );

  return Box(GetPosition());
}

Layer* RoutingPad::GetLayer() const
// ******************************************************
{
  Component* component = _GetEntityAsComponent();
  if ( component ) return component->GetLayer ();

  return NULL;
}

Point RoutingPad::GetSourcePosition() const
// ****************************************
{
  Segment* segment = _GetEntityAsSegment();
  if ( segment )
    return _occurrence.GetPath().GetTransformation().getPoint ( segment->GetSourcePosition() );

  return GetPosition();
}

Point RoutingPad::GetTargetPosition() const
// ****************************************
{
  Segment* segment = _GetEntityAsSegment();
  if ( segment )
    return _occurrence.GetPath().GetTransformation().getPoint ( segment->GetTargetPosition() );

  return GetPosition();
}

Unit RoutingPad::GetSourceX() const
// ********************************
{
  return GetSourcePosition().getX();
}

Unit RoutingPad::GetSourceY() const
// ********************************
{
  return GetSourcePosition().getY();
}

Unit RoutingPad::GetTargetX() const
// ********************************
{
  return GetTargetPosition().getX();
}

Unit RoutingPad::GetTargetY() const
// ********************************
{
  return GetTargetPosition().getY();
}

Point RoutingPad::GetCenter() const
// ****************************************
{
  Segment* segment = _GetEntityAsSegment();
  if ( segment )
    return _occurrence.GetPath().GetTransformation().getPoint ( segment->GetCenter() );

  return GetPosition();
}


void RoutingPad::Translate(const Unit& dx, const Unit& dy)
// ****************************************************
{
    if ((dx != 0) || (dy != 0)) {
        Invalidate(true);
        _x += dx;
        _y += dy;
    }
}

void RoutingPad::SetX(const Unit& x)
// ******************************
{
    SetPosition(x, GetY());
}

void RoutingPad::SetY(const Unit& y)
// ******************************
{
    SetPosition(GetX(), y);
}

void RoutingPad::SetPosition(const Unit& x, const Unit& y)
// ****************************************************
{
    SetOffset(x, y);
}

void RoutingPad::SetPosition(const Point& position)
// *********************************************
{
    SetPosition(position.getX(), position.getY());
}

void RoutingPad::SetOffset(const Unit& x, const Unit& y)
// ****************************************************
{
    Invalidate(true);
    _x = x;
    _y = y;
}

void RoutingPad::_PreDelete()
// ***********************
{
// trace << "entering RoutingPad::PreDelete: " << this << endl;
// trace_in();


  if (!_occurrence.GetPath().IsEmpty())
    _occurrence.GetMasterCell()->_RemoveSlaveEntity(_occurrence.GetEntity(),this);
  Inherit::_PreDelete();

// trace << "exiting RoutingPad::PreDelete:" << endl;
// trace_out();
}

string RoutingPad::_GetString() const
// *******************************
{
    string s = Inherit::_GetString();
    s.insert(s.length() - 1, " [" + GetValueString(GetX()));
    s.insert(s.length() - 1, " " + GetValueString(GetY()));
    s.insert(s.length() - 1, "] ");
    s.insert(s.length() - 1, GetString(_occurrence));
    return s;
}

Record* RoutingPad::_GetRecord() const
// **************************
{
    Record* record = Inherit::_GetRecord();
    if (record) {
        record->Add(GetSlot("X", &_x));
        record->Add(GetSlot("Y", &_y));
        record->Add(GetSlot("Occurrence",_occurrence));
    }
    return record;
}

Component*  RoutingPad::_GetEntityAsComponent () const
// ***************************************************
{
  if ( _occurrence.IsValid() )
    return dynamic_cast<Component*>( _occurrence.GetEntity() );

  return NULL;
}

Segment*  RoutingPad::_GetEntityAsSegment () const
// ***********************************************
{
  if ( _occurrence.IsValid() )
    return dynamic_cast<Segment*>( _occurrence.GetEntity() );

  return NULL;
}

//bool RoutingPad::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
//// ******************************************************************************************
//{
//    Layer* layer = GetLayer();
//    Box    boundingBox ( GetBoundingBox() );
//    Box    area        ( point );
//
//    area.Inflate ( aperture );
//
//    for_each_basic_layer(basicLayer, layer->GetBasicLayers()) {
//        if (view->IsVisible(basicLayer))
//            if (boundingBox.Intersect(area)) return true;
//        end_for;
//    }
//
//    return false;
//}
//
//
//void RoutingPad::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// ****************************************************************************************************
//{
//    Box         boundingBox ( GetBoundingBox() );
//    BasicLayer* layer = GetLayer()->_GetSymbolicBasicLayer();
//
//    if ( basicLayer == layer ) {
//      if ( 1 < view->GetScreenSize(boundingBox.GetWidth()) ) {
//        view->DrawRectangle ( boundingBox );
//      }
//      else
//        view->DrawLine ( GetSourcePosition(), GetTargetPosition() );
//    }
//}
//
//
//void RoutingPad::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
//// **********************************************************************************************
//{
//    BasicLayer* layer = GetLayer()->_GetSymbolicBasicLayer();
//    _Draw ( view, layer, updateArea, transformation );
//}


void RoutingPad::SetExternalComponent(Component* component)
// ********************************************************
{
    if (IsMaterialized()) Invalidate(false);

    Occurrence plugOccurrence = GetPlugOccurrence();
    Plug* plug= static_cast<Plug*>(plugOccurrence.GetEntity());
    if (plug->GetMasterNet() != component->GetNet())
        throw Error("Cannot Set External Component to Routing Pad : Inconsistant Net");

    _occurrence.GetMasterCell()->_RemoveSlaveEntity(_occurrence.GetEntity(),this);
    _occurrence = Occurrence(component,Path(plugOccurrence.GetPath(),plug->GetInstance()));

    Point position = _occurrence.GetPath().GetTransformation().getPoint ( component->GetPosition() );

    Horizontal* horizontal = dynamic_cast<Horizontal*>(component);
    if ( horizontal ) {
      SetX ( 0 );
      SetY ( position.getY() );
    } else {
      Vertical* vertical = dynamic_cast<Vertical*>(component);
      if ( vertical ) {
        SetX ( position.getX() );
        SetY ( 0 );
      } else
        SetPosition ( position );
    }

    _occurrence.GetMasterCell()->_AddSlaveEntity(_occurrence.GetEntity(),this);

    if (!IsMaterialized()) {
      Materialize();
    }
}


Occurrence RoutingPad::GetPlugOccurrence()
// ***************************************
{
    if (is_a<Plug*>(_occurrence.GetEntity()))
        return _occurrence;
    Component* component= static_cast<Component*>(_occurrence.GetEntity());
    Net* net=component->GetNet();
    Path path=_occurrence.GetPath();
    if (path.IsEmpty())
        throw Error("Empty Path => not in an instance");
    Instance* instance=path.GetTailInstance();
    Plug* plug=instance->GetPlug(net);
    return Occurrence(plug,path.GetHeadPath());
}


void RoutingPad::RestorePlugOccurrence()
// *************************************
{
    if (IsMaterialized()) Unmaterialize();

    _occurrence=GetPlugOccurrence();
    SetPosition ( _occurrence.GetPath().GetTransformation().getPoint
        ( dynamic_cast<Component*>(_occurrence.GetEntity())->GetPosition() ) );
}



#if 0
// ****************************************************************************************************
// RoutingPad::Builder declaration
// ****************************************************************************************************

RoutingPad::Builder::Builder(const string& token)
// *******************************************
:    Inherit(token),
    _layer(NULL),
    _x(0),
    _y(0),
    _width(0),
    _height(0)
{
}

void RoutingPad::Builder::Scan(InputFile& inputFile, char*& arguments)
// ****************************************************************
{
    Inherit::Scan(inputFile, arguments);

    unsigned layerId;
    unsigned n;


    if (r != 6)
        throw Error("Can't create RoutingPad : syntax error");

    arguments = &arguments[n];

    DBo* dbo = inputFile.GetDBo(layerId);
    if (!dbo || !is_a<Layer*>(dbo))
        throw Error("Can't create RoutingPad : bad layer");

    _layer = (Layer*)dbo;
}

DBo* RoutingPad::Builder::CreateDBo()
// *******************************
{
    return RoutingPad::Create(GetNet(), GetLayer(), getX(), getY(), GetWidth(), GetHeight());
}

RoutingPad::Builder ROUTINGPAD_BUILDER("RP");
#endif


RoutingPad* CreateRoutingPad ( Net* net, Occurrence plugOccurrence )
// *****************************************************************
{
    Component*      bestComponent  = NULL;
    Plug* plug = static_cast<Plug*>(plugOccurrence.GetEntity());

    for_each_component ( component, GetExternalComponents(plug->GetMasterNet()) ) {
      if ( !bestComponent ) { bestComponent = component; continue; }
      if ( /*    IsOnTop(component->GetLayer(),bestComponent->GetLayer())
           ||*/ ( GetArea(component) > GetArea(bestComponent) ) ) {
        bestComponent = component;
      }
      end_for
    }

    if ( !bestComponent ) {
      string message = "CreateRoutingPad(): Cannot find external component of ";
      message += GetString(plug->GetMasterNet()) + " in ";
      message += GetString(plug->GetInstance())  + ".\n" ;
      throw Error ( message );
    }

    RoutingPad* rp = RoutingPad::Create ( net, plugOccurrence );
    rp->SetExternalComponent ( bestComponent );

    return rp;
}

RoutingPad* CreateRoutingPad ( Pin* pin )
// **************************************
{
  Occurrence  pinOccurrence ( pin, Path() );

# if 0
  for_each_routing_pad ( routingPad, pin->GetNet()->GetRoutingPads() ) {
    if ( routingPad->GetOccurrence() == pinOccurrence )
      return routingPad;
    end_for
  }
# endif

  return RoutingPad::Create ( pin->GetNet(), pinOccurrence );
}


} // End of Hurricane namespace.
