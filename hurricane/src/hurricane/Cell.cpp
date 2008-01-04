// ****************************************************************************************************
// File: Cell.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Cell.h"
#include "DataBase.h"
#include "Library.h"
#include "Instance.h"
#include "Net.h"
#include "Pin.h"
#include "RoutingPad.h"
#include "Layer.h"
#include "Slice.h"
#include "Rubber.h"
#include "Marker.h"
#include "Component.h"
#include "UpdateSession.h"
#include "Error.h"

namespace Hurricane {

// ****************************************************************************************************
// Cell implementation
// ****************************************************************************************************

Cell::Cell(Library* library, const Name& name)
// *******************************************
:    Inherit(),
    _library(library),
    _name(name),
    _instanceMap(),
    _quadTree(),
    _slaveInstanceSet(),
    _netMap(),
    _sliceMap(),
    _markerSet(),
    //_viewSet(),
    _abutmentBox(),
    _boundingBox(),
    _isTerminal(true),
    _isPad(false),
    _nextOfLibraryCellMap(NULL),
    _nextOfSymbolCellSet(NULL),
    _slaveEntityMap()
{
    if (!_library)
        throw Error("Can't create " + _TName("Cell") + " : null library");

    if (name.IsEmpty())
        throw Error("Can't create " + _TName("Cell") + " : empty name");

    if (_library->GetCell(_name))
        throw Error("Can't create " + _TName("Cell") + " : already exists");
}

Cell* Cell::Create(Library* library, const Name& name)
// ***************************************************
{
    Cell* cell = new Cell(library, name);

    cell->_PostCreate();

    return cell;
}

Box Cell::GetBoundingBox() const
// *****************************
{
    if (_boundingBox.isEmpty()) {
        Box& boundingBox = (Box&)_boundingBox;
        boundingBox = _abutmentBox;
        boundingBox.merge(_quadTree.GetBoundingBox());
        for_each_slice(slice, GetSlices()) {
            boundingBox.merge(slice->GetBoundingBox());
            end_for;
        }
    }
    
    return _boundingBox;
}

bool Cell::IsLeaf() const
// **********************
{
    return _instanceMap.IsEmpty();
}

bool Cell::IsCalledBy(Cell* cell) const
// ************************************
{
    for_each_instance(instance, cell->GetInstances()) {
        Cell* masterCell = instance->GetMasterCell();
        if (masterCell == this) return true;
        if (IsCalledBy(masterCell)) return true;
        end_for;
    }
    return false;
}

void Cell::SetName(const Name& name)
// *********************************
{
    if (name != _name) {
        if (name.IsEmpty())
            throw Error("Can't change " + _TName("Cell") + " name : empty name");

        if (_library->GetCell(name))
            throw Error("Can't change " + _TName("Cell") + " name : already exists");

        _library->_GetCellMap()._Remove(this);
        _name = name;
        _library->_GetCellMap()._Insert(this);
    }
}

void Cell::SetAbutmentBox(const Box& abutmentBox)
// **********************************************
{
    if (abutmentBox != _abutmentBox) {
        if (!_abutmentBox.isEmpty() &&
             (abutmentBox.isEmpty() || !abutmentBox.contains(_abutmentBox)))
            _Unfit(_abutmentBox);
        _abutmentBox = abutmentBox;
        _Fit(_abutmentBox);
    }
}

void Cell::FlattenNets(bool buildRings)
// ************************************
{
  OpenUpdateSession ();

  for_each_occurrence ( occurrence, GetHyperNetRootNetOccurrences() ) {
    HyperNet  hyperNet ( occurrence );
    if ( !occurrence.GetPath().IsEmpty() ) {
      DeepNet* deepNet = DeepNet::Create ( hyperNet );
      if (deepNet) deepNet->_CreateRoutingPads ( buildRings );
    } else {
      RoutingPad* previousRP = NULL;
      RoutingPad* currentRP  = NULL;
      Net* net = static_cast<Net*>(occurrence.GetEntity());

      for_each_component ( component, net->GetComponents() ) {
        Plug* primaryPlug = dynamic_cast<Plug*>( component );
        if ( primaryPlug ) {
          if ( !primaryPlug->GetBodyHook()->GetSlaveHooks().IsEmpty() ) {
            cerr << "[ERROR] " << primaryPlug << "\n"
                 << "        has attached components, not managed yet." << endl;
          } else {
            primaryPlug->GetBodyHook()->Detach ();
          }
        }
        end_for
      }

      for_each_occurrence ( plugOccurrence, hyperNet.GetLeafPlugOccurrences() ) {
        currentRP = CreateRoutingPad ( net, plugOccurrence );
        currentRP->Materialize ();
        if ( buildRings ) {
          if ( previousRP ) {
            currentRP->GetBodyHook()->Attach ( previousRP->GetBodyHook() );
          }
          Plug* plug = static_cast<Plug*>( plugOccurrence.GetEntity() );
          if ( plugOccurrence.GetPath().IsEmpty() ) {
            plug->GetBodyHook()->Attach ( currentRP->GetBodyHook() );
            plug->GetBodyHook()->Detach ();
          }
          previousRP = currentRP;
        }

        end_for
      }

      for_each_component ( component, net->GetComponents() ) {
        Pin* pin = dynamic_cast<Pin*>( component );
        if ( pin ) {
          currentRP = CreateRoutingPad ( pin );
          if ( buildRings ) {
            if ( previousRP ) {
              currentRP->GetBodyHook()->Attach ( previousRP->GetBodyHook() );
            }
            pin->GetBodyHook()->Attach ( currentRP->GetBodyHook() );
            pin->GetBodyHook()->Detach ();
          }
          previousRP = currentRP;
        }
        
        end_for
      }
    }
    end_for
  }

  CloseUpdateSession ();
}

void Cell::Materialize()
// *********************
{
    for_each_instance(instance, GetInstances()) instance->Materialize(); end_for;
    for_each_net(net, GetNets()) net->Materialize(); end_for;
    for_each_marker(marker, GetMarkers()) marker->Materialize(); end_for;
}

void Cell::Unmaterialize()
// ***********************
{
    for_each_instance(instance, GetInstances()) instance->Unmaterialize(); end_for;
    for_each_net(net, GetNets()) net->Unmaterialize(); end_for;
    for_each_marker(marker, GetMarkers()) marker->Unmaterialize(); end_for;
}

void Cell::_PostCreate()
// *********************
{
    _library->_GetCellMap()._Insert(this);

    Inherit::_PostCreate();
}

void Cell::_PreDelete()
// ********************
{
    Inherit::_PreDelete();

    while(_slaveEntityMap.size()) {
      _slaveEntityMap.begin()->second->Delete();
    }

    //for_each_view(view, GetViews()) view->SetCell(NULL); end_for;
    for_each_marker(marker, GetMarkers()) marker->Delete(); end_for;
    for_each_instance(slaveInstance, GetSlaveInstances()) slaveInstance->Delete(); end_for;
    for_each_instance(instance, GetInstances()) instance->Delete(); end_for;
    for_each_net(net, GetNets()) net->Delete(); end_for;
    for_each_slice(slice, GetSlices()) slice->_Delete(); end_for;

    _library->_GetCellMap()._Remove(this);
}

string Cell::_GetString() const
// ****************************
{
    string s = Inherit::_GetString();
    s.insert(s.length() - 1, " " + GetString(_name));
    return s;
}

Record* Cell::_GetRecord() const
// ***********************
{
    Record* record = Inherit::_GetRecord();
    if (record) {
        record->Add(GetSlot("Library", _library));
        record->Add(GetSlot("Name", &_name));
        record->Add(GetSlot("Instances", &_instanceMap));
        record->Add(GetSlot("QuadTree", &_quadTree));
        record->Add(GetSlot("SlaveInstances", &_slaveInstanceSet));
        record->Add(GetSlot("Nets", &_netMap));
        record->Add(GetSlot("Pins", &_pinMap));
        record->Add(GetSlot("Slices", &_sliceMap));
        record->Add(GetSlot("Markers", &_markerSet));
        //record->Add(GetSlot("Views", &_viewSet));
        record->Add(GetSlot("AbutmentBox", &_abutmentBox));
        record->Add(GetSlot("BoundingBox", &_boundingBox));
        record->Add(GetSlot("IsTerminal", &_isTerminal));
        record->Add(GetSlot("IsFlattenLeaf", &_isFlattenLeaf));
        //record->Add(GetSlot("Symbol", _symbol));
    }
    return record;
}

void Cell::_Fit(const Box& box)
// ****************************
{
    if (box.isEmpty()) return;
    if (_boundingBox.isEmpty()) return;
    if (_boundingBox.contains(box)) return;
    _boundingBox.merge(box);
    for_each_instance(instance, GetSlaveInstances()) {
        instance->GetCell()->_Fit(instance->GetTransformation().GetBox(box));
        end_for;
    }
}

void Cell::_Unfit(const Box& box)
// ******************************
{
    if (box.isEmpty()) return;
    if (_boundingBox.isEmpty()) return;
    if (!_boundingBox.isConstrainedBy(box)) return;
    _boundingBox.makeEmpty();
    for_each_instance(instance, GetSlaveInstances()) {
        instance->GetCell()->_Unfit(instance->GetTransformation().GetBox(box));
        end_for;
    }
}

void Cell::_AddSlaveEntity(Entity* entity, Entity* slaveEntity)
// ************************************************************************
{
  assert(entity->GetCell() == this);

  _slaveEntityMap.insert(pair<Entity*,Entity*>(entity,slaveEntity));
}

void Cell::_RemoveSlaveEntity(Entity* entity, Entity* slaveEntity)
// ***************************************************************************
{
  assert(entity->GetCell() == this);

  pair<SlaveEntityMap::iterator,SlaveEntityMap::iterator>
    bounds = _slaveEntityMap.equal_range(entity);
  multimap<Entity*,Entity*>::iterator it = bounds.first;
  for(; it != bounds.second ; it++ ) {
    if (it->second == slaveEntity) {
      _slaveEntityMap.erase(it);
      break;
    }
  }
}

void Cell::_GetSlaveEntities(SlaveEntityMap::iterator& begin, SlaveEntityMap::iterator& end)
// *********************************************************************************************************
{
  begin = _slaveEntityMap.begin();
  end   = _slaveEntityMap.end();
}

void Cell::_GetSlaveEntities(Entity* entity, SlaveEntityMap::iterator& begin, SlaveEntityMap::iterator& end)
// *********************************************************************************************************
{
  begin = _slaveEntityMap.lower_bound(entity);
  end   = _slaveEntityMap.upper_bound(entity);
}

//bool Cell::_IsDrawable(View* view) const
//// *************************************
//{
//    return true;
//    //if (view->GetCell() == this) return true;
//
//    //if (is_a<MapView*>(view)) return true;
//
//    //return (1 < (double)view->GetScreenSize(_boundingBox.GetHeight()));
////    return (100 < ((double)view->GetScreenSize(_boundingBox.GetWidth()) *
////                        (double)view->GetScreenSize(_boundingBox.GetHeight())));
//}
//
//bool Cell::_ContentIsDrawable(View* view) const
//// ********************************************
//{
//    if (IsTerminal()) return false;
//
//        return true;
//
//    //if (view->GetCell() == this) return true;
//
//    //if (is_a<MapView*>(view)) return false;
//
//    //return (40 < (double)view->GetScreenSize(_boundingBox.GetHeight()));
////    return (400 < ((double)view->GetScreenSize(_boundingBox.GetWidth()) *
////                        (double)view->GetScreenSize(_boundingBox.GetHeight())));
//}
//
//void Cell::_DrawPhantoms(View* view, const Box& updateArea, const Transformation& transformation)
//// **********************************************************************************************
//{
////    if (_IsDrawable(view)) { // To avoid irregular display of instances phantoms
////        if (!_ContentIsDrawable(view))
////            view->FillRectangle(transformation.GetBox(GetAbutmentBox()));
////        else {
////            for_each_instance(instance, GetInstancesUnder(updateArea)) {
////                instance->_DrawPhantoms(view, updateArea, transformation);
////                end_for;
////            }
////        }
////    }
//}
//
//void Cell::_DrawBoundaries(View* view, const Box& updateArea, const Transformation& transformation)
//// ************************************************************************************************
//{
// //       if (_IsDrawable(view)) { // To avoid irregular display of instances phantoms
// //           view->DrawRectangle(transformation.GetBox(GetAbutmentBox()));
// //           if (_ContentIsDrawable(view)) {
// //           for_each_instance(instance, GetInstancesUnder(updateArea)) {
// //               instance->_DrawBoundaries(view, updateArea, transformation);
// //               end_for;
// //           }
// //       }
// //   }
//}
//
//void Cell::_DrawContent(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// ****************************************************************************************************
//{
////    if (_IsDrawable(view)) {
////        if (_ContentIsDrawable(view)) {
////            view->CheckForDisplayInterruption();
////            for_each_instance(instance, GetInstancesUnder(updateArea)) {
////                instance->_Draw(view, basicLayer, updateArea, transformation);
////                end_for;
////            }
////            for_each_slice(slice, GetSlices()) {
////                slice->_Draw(view, basicLayer, updateArea, transformation);
////                end_for;
////            }
////        }
////    }
//}
//
//void Cell::_DrawRubbers(View* view, const Box& updateArea, const Transformation& transformation)
//// *********************************************************************************************
//{
////    if (_IsDrawable(view)) {
////        if (_ContentIsDrawable(view)) {
////            for_each_instance(instance, GetInstancesUnder(updateArea)) {
////                instance->_DrawRubbers(view, updateArea, transformation);
////                end_for;
////            }
////            for_each_rubber(rubber, GetRubbersUnder(updateArea)) {
////                rubber->_Draw(view, NULL, updateArea, transformation);
////                end_for;
////            }
////        }
////    }
//}
//
//void Cell::_DrawMarkers(View* view, const Box& updateArea, const Transformation& transformation)
//// *********************************************************************************************
//{
////    if (_IsDrawable(view)) {
////        if (_ContentIsDrawable(view)) {
////            for_each_instance(instance, GetInstancesUnder(updateArea)) {
////                instance->_DrawMarkers(view, updateArea, transformation);
////                end_for;
////            }
////            for_each_marker(marker, GetMarkersUnder(updateArea)) {
////                marker->_Draw(view, NULL, updateArea, transformation);
////                end_for;
////            }
////        }
////    }
//}
//
//void Cell::_DrawDisplaySlots(View* view, const Box& area,  const Box& updateArea, const Transformation& transformation)
//// ********************************************************************************************************************
//{
////    if (_IsDrawable(view)) {
////        if (_ContentIsDrawable(view)) {
////            for_each_instance(instance, GetInstancesUnder(updateArea)) {
////                instance->_DrawDisplaySlots(view, area, updateArea, transformation);
////                end_for;
////            }
////            for_each_display_slot(displaySlot, GetDisplaySlots(this)) {
////                view->_DrawDisplaySlot(displaySlot, area, updateArea, transformation);
////                end_for;
////            }
////        }
////    }
//}
//
// ****************************************************************************************************
// Cell::InstanceMap implementation
// ****************************************************************************************************

Cell::InstanceMap::InstanceMap()
// *****************************
:    Inherit()
{
}

Name Cell::InstanceMap::_GetKey(Instance* instance) const
// ******************************************************
{
    return instance->GetName();
}

unsigned Cell::InstanceMap::_GetHashValue(Name name) const
// *******************************************************
{
    return ( (unsigned int)( (unsigned long)name._GetSharedName() ) ) / 8;
}

Instance* Cell::InstanceMap::_GetNextElement(Instance* instance) const
// *******************************************************************
{
    return instance->_GetNextOfCellInstanceMap();
}

void Cell::InstanceMap::_SetNextElement(Instance* instance, Instance* nextInstance) const
// **************************************************************************************
{
    instance->_SetNextOfCellInstanceMap(nextInstance);
}



// ****************************************************************************************************
// Cell::SlaveInstanceSet implementation
// ****************************************************************************************************

Cell::SlaveInstanceSet::SlaveInstanceSet()
// ***************************************
:    Inherit()
{
}

unsigned Cell::SlaveInstanceSet::_GetHashValue(Instance* slaveInstance) const
// **************************************************************************
{
    return ( (unsigned int)( (unsigned long)slaveInstance ) ) / 8;
}

Instance* Cell::SlaveInstanceSet::_GetNextElement(Instance* slaveInstance) const
// *****************************************************************************
{
    return slaveInstance->_GetNextOfCellSlaveInstanceSet();
}

void Cell::SlaveInstanceSet::_SetNextElement(Instance* slaveInstance, Instance* nextSlaveInstance) const
// ****************************************************************************************************
{
    slaveInstance->_SetNextOfCellSlaveInstanceSet(nextSlaveInstance);
}



// ****************************************************************************************************
// Cell::NetMap implementation
// ****************************************************************************************************

Cell::NetMap::NetMap()
// *******************
:    Inherit()
{
}

Name Cell::NetMap::_GetKey(Net* net) const
// ***************************************
{
    return net->GetName();
}

unsigned Cell::NetMap::_GetHashValue(Name name) const
// **************************************************
{
    return ( (unsigned int)( (unsigned long)name._GetSharedName() ) ) / 8;
}

Net* Cell::NetMap::_GetNextElement(Net* net) const
// ***********************************************
{
    return net->_GetNextOfCellNetMap();
}

void Cell::NetMap::_SetNextElement(Net* net, Net* nextNet) const
// *************************************************************
{
    net->_SetNextOfCellNetMap(nextNet);
}


// ****************************************************************************************************
// Cell::PinMap implementation
// ****************************************************************************************************

Cell::PinMap::PinMap()
// *******************
:    Inherit()
{
}

Name Cell::PinMap::_GetKey(Pin* pin) const
// ***************************************
{
    return pin->GetName();
}

unsigned Cell::PinMap::_GetHashValue(Name name) const
// **************************************************
{
    return ( (unsigned int)( (unsigned long)name._GetSharedName() ) ) / 8;
}

Pin* Cell::PinMap::_GetNextElement(Pin* pin) const
// ***********************************************
{
    return pin->_GetNextOfCellPinMap();
}

void Cell::PinMap::_SetNextElement(Pin* pin, Pin* nextPin) const
// *************************************************************
{
    pin->_SetNextOfCellPinMap(nextPin);
}


// ****************************************************************************************************
// Cell::SliceMap implementation
// ****************************************************************************************************

Cell::SliceMap::SliceMap()
// ***********************
:    Inherit()
{
}

const Layer* Cell::SliceMap::_GetKey(Slice* slice) const
// *****************************************************
{
    return slice->GetLayer();
}

unsigned Cell::SliceMap::_GetHashValue(const Layer* layer) const
// *************************************************************
{
    return ( (unsigned int)( (unsigned long)layer ) ) / 8;
}

Slice* Cell::SliceMap::_GetNextElement(Slice* slice) const
// *******************************************************
{
    return slice->_GetNextOfCellSliceMap();
}

void Cell::SliceMap::_SetNextElement(Slice* slice, Slice* nextSlice) const
// ***********************************************************************
{
    slice->_SetNextOfCellSliceMap(nextSlice);
};



// ****************************************************************************************************
// Cell::MarkerSet implementation
// ****************************************************************************************************

Cell::MarkerSet::MarkerSet()
// *************************
:    Inherit()
{
}

unsigned Cell::MarkerSet::_GetHashValue(Marker* marker) const
// **********************************************************
{
    return ( (unsigned int)( (unsigned long)marker ) ) / 8;
}

Marker* Cell::MarkerSet::_GetNextElement(Marker* marker) const
// ***********************************************************
{
    return marker->_GetNextOfCellMarkerSet();
}

void Cell::MarkerSet::_SetNextElement(Marker* marker, Marker* nextMarker) const
// ****************************************************************************
{
    marker->_SetNextOfCellMarkerSet(nextMarker);
}



//// ****************************************************************************************************
//// Cell::ViewSet implementation
//// ****************************************************************************************************
//
//Cell::ViewSet::ViewSet()
//// *********************
//:    Inherit()
//{
//}
//
//unsigned Cell::ViewSet::_GetHashValue(View* view) const
//// ****************************************************
//{
//    return ( (unsigned int)( (unsigned long)view ) ) / 8;
//}
//
//View* Cell::ViewSet::_GetNextElement(View* view) const
//// ***************************************************
//{
//    return view->_GetNextOfCellViewSet();
//}
//
//void Cell::ViewSet::_SetNextElement(View* view, View* nextView) const
//// ******************************************************************
//{
//    view->_SetNextOfCellViewSet(nextView);
//}
//
//

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
