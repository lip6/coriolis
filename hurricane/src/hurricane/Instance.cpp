// ****************************************************************************************************
// File: Instance.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6

#include "Instance.h"
#include "Cell.h"
#include "Net.h"
#include "Plug.h"
#include "SharedPath.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Instance_IsUnderFilter : public Filter<Instance*> {
// ****************************************************

	public: Box _area;

	public: Instance_IsUnderFilter(const Box& area)
	// ********************************************
	: _area(area)
	{
	};

	public: Instance_IsUnderFilter(const Instance_IsUnderFilter& filter)
	// *****************************************************************
	:	_area(filter._area)
	{
	};

	public: Instance_IsUnderFilter& operator=(const Instance_IsUnderFilter& filter)
	// ****************************************************************************
	{
		_area = filter._area;
		return *this;
	};

	public: virtual Filter<Instance*>* GetClone() const
	// ************************************************
	{
		return new Instance_IsUnderFilter(*this);
	};

	public: virtual bool Accept(Instance* instance) const
	// **************************************************
	{
		return _area.Intersect(instance->GetBoundingBox());
	};

	public: virtual string _GetString() const
	// **************************************
	{
		return "<" + _TName("Instance::IsUnderFilter") + " " + GetString(_area) + ">";
	};

};

class Instance_IsTerminalFilter : public Filter<Instance*> {
// *******************************************************

	public: Instance_IsTerminalFilter() {};

	public: Instance_IsTerminalFilter(const Instance_IsTerminalFilter& filter) {};

	public: Instance_IsTerminalFilter& operator=(const Instance_IsTerminalFilter& filter) {return *this;};

	public: virtual Filter<Instance*>* GetClone() const {return new Instance_IsTerminalFilter(*this);};

	public: virtual bool Accept(Instance* instance) const {return instance->IsTerminal();};

	public: virtual string _GetString() const {return "<" + _TName("Instance::IsTerminalFilter") + ">";};

};

class Instance_IsLeafFilter : public Filter<Instance*> {
// *******************************************************

	public: Instance_IsLeafFilter() {};

	public: Instance_IsLeafFilter(const Instance_IsLeafFilter& filter) {};

	public: Instance_IsLeafFilter& operator=(const Instance_IsLeafFilter& filter) {return *this;};

	public: virtual Filter<Instance*>* GetClone() const {return new Instance_IsLeafFilter(*this);};

	public: virtual bool Accept(Instance* instance) const {return instance->IsLeaf();};

	public: virtual string _GetString() const {return "<" + _TName("Instance::IsLeafFilter") + ">";};

};

class Instance_IsUnplacedFilter : public Filter<Instance*> {
// *******************************************************

	public: Instance_IsUnplacedFilter() {};

	public: Instance_IsUnplacedFilter(const Instance_IsUnplacedFilter& filter) {};

	public: Instance_IsUnplacedFilter& operator=(const Instance_IsUnplacedFilter& filter) {return *this;};

	public: virtual Filter<Instance*>* GetClone() const {return new Instance_IsUnplacedFilter(*this);};

	public: virtual bool Accept(Instance* instance) const {return instance->IsUnplaced();};

	public: virtual string _GetString() const {return "<" + _TName("Net::IsUnplacedFilter>");};

};

class Instance_IsPlacedFilter : public Filter<Instance*> {
// *****************************************************

	public: Instance_IsPlacedFilter() {};

	public: Instance_IsPlacedFilter(const Instance_IsPlacedFilter& filter) {};

	public: Instance_IsPlacedFilter& operator=(const Instance_IsPlacedFilter& filter) {return *this;};

	public: virtual Filter<Instance*>* GetClone() const {return new Instance_IsPlacedFilter(*this);};

	public: virtual bool Accept(Instance* instance) const {return instance->IsPlaced();};

	public: virtual string _GetString() const {return "<" + _TName("Net::IsPlacedFilter>");};

};

class Instance_IsFixedFilter : public Filter<Instance*> {
// *****************************************************

	public: Instance_IsFixedFilter() {};

	public: Instance_IsFixedFilter(const Instance_IsFixedFilter& filter) {};

	public: Instance_IsFixedFilter& operator=(const Instance_IsFixedFilter& filter) {return *this;};

	public: virtual Filter<Instance*>* GetClone() const {return new Instance_IsFixedFilter(*this);};

	public: virtual bool Accept(Instance* instance) const {return instance->IsFixed();};

	public: virtual string _GetString() const {return "<" + _TName("Net::IsFixedFilter>");};

};

// ****************************************************************************************************
// Instance implementation
// ****************************************************************************************************

Instance::Instance(Cell* cell, const Name& name, Cell* masterCell, const Transformation& transformation, const PlacementStatus& placementstatus, bool secureFlag)
// ****************************************************************************************************
:	Inherit(),
	_cell(cell),
	_name(name),
	_masterCell(masterCell),
	_transformation(transformation),
	_placementStatus(placementstatus),
	_plugMap(),
	_sharedPathMap(),
	_nextOfCellInstanceMap(NULL),
	_nextOfCellSlaveInstanceSet(NULL)
{
	if (!_cell)
		throw Error("Can't create " + _TName("Instance") + " : null cell");

	if (name.IsEmpty())
		throw Error("Can't create " + _TName("Instance") + " : empty name");

	if (_cell->GetInstance(_name))
		throw Error("Can't create " + _TName("Instance") + " : already exists");

	if (!_masterCell)
		throw Error("Can't create " + _TName("Instance") + " : null master cell");

	if (secureFlag && _cell->IsCalledBy(_masterCell))
		throw Error("Can't create " + _TName("Instance") + " : cyclic construction");
}

Instance* Instance::Create(Cell* cell, const Name& name, Cell* masterCell, bool secureFlag)
// ****************************************************************************************
{
	Instance* instance =
		new Instance(cell, name, masterCell, Transformation(), PlacementStatus(), secureFlag);

	instance->_PostCreate();

	return instance;
}

Instance* Instance::Create(Cell* cell, const Name& name, Cell* masterCell, const Transformation& transformation, const PlacementStatus& placementstatus, bool secureFlag)
// ****************************************************************************************************
{
	Instance* instance =
		new Instance(cell, name, masterCell, transformation, placementstatus, secureFlag);

	instance->_PostCreate();

	return instance;
}

Box Instance::GetBoundingBox() const
// *********************************
{
    return  _transformation.GetBox(_masterCell->GetBoundingBox());
}

Plugs Instance::GetConnectedPlugs() const
// **************************************
{
	return GetPlugs().GetSubSet(Plug::GetIsConnectedFilter());
}

Plugs Instance::GetUnconnectedPlugs() const
// ****************************************
{
	return GetPlugs().GetSubSet(Plug::GetIsUnconnectedFilter());
}

Path Instance::GetPath(const Path& tailPath) const
// ***********************************************
{
	return Path((Instance*)this, tailPath);
}

Box Instance::GetAbutmentBox() const
// *********************************
{
    return _transformation.GetBox(_masterCell->GetAbutmentBox());
}

bool Instance::IsTerminal() const
// ******************************
{
	return GetMasterCell()->IsTerminal();
}

bool Instance::IsLeaf() const
// **************************
{
	return GetMasterCell()->IsLeaf();
}

InstanceFilter Instance::GetIsUnderFilter(const Box& area)
// *******************************************************
{
	return Instance_IsUnderFilter(area);
}

InstanceFilter Instance::GetIsTerminalFilter()
// *******************************************
{
	return Instance_IsTerminalFilter();
}

InstanceFilter Instance::GetIsLeafFilter()
// *******************************************
{
	return Instance_IsLeafFilter();
}

InstanceFilter Instance::GetIsUnplacedFilter()
// *******************************************
{
    return Instance_IsUnplacedFilter();
}

InstanceFilter Instance::GetIsPlacedFilter()
// *****************************************
{
    return Instance_IsPlacedFilter();
}

InstanceFilter Instance::GetIsFixedFilter()
// ****************************************
{
    return Instance_IsFixedFilter();
}

InstanceFilter Instance::GetIsNotUnplacedFilter()
// **********************************************
{
    return !Instance_IsUnplacedFilter();
}

void Instance::Materialize()
// *************************
{
	if (!IsMaterialized()) {
		Box boundingBox = GetBoundingBox();
		if (!boundingBox.IsEmpty()) {
			QuadTree* quadTree = _cell->_GetQuadTree();
			quadTree->Insert(this);
			_cell->_Fit(quadTree->GetBoundingBox());
		}
	}
}

void Instance::Unmaterialize()
// ***************************
{
	if (IsMaterialized()) {
		_cell->_Unfit(GetBoundingBox());
		_cell->_GetQuadTree()->Remove(this);
	}
}

void Instance::Invalidate(bool propagateFlag)
// ******************************************
{
	Inherit::Invalidate(false);

	if (propagateFlag) {
		for_each_plug(plug, GetConnectedPlugs()) {
			plug->Invalidate(true);
			end_for;
		}
	}
}

void Instance::Translate(const Unit& dx, const Unit& dy)
// *****************************************************
{
	if ((dx != 0) || (dy !=0)) {
		Point translation = _transformation.GetTranslation();
		Unit x = translation.GetX() + dx;
		Unit y = translation.GetY() + dy;
		Transformation::Orientation orientation = _transformation.GetOrientation();
		SetTransformation(Transformation(x, y, orientation));
	}
}

void Instance::SetName(const Name& name)
// *************************************
{
	if (name != _name) {
		if (name.IsEmpty())
			throw Error("Can't change instance name : empty name");

		if (_cell->GetInstance(name))
			throw Error("Can't change instance name : already exists");

		_cell->_GetInstanceMap()._Remove(this);
		_name = name;
		_cell->_GetInstanceMap()._Insert(this);
	}
}

void Instance::SetTransformation(const Transformation& transformation)
// *******************************************************************
{
	if (transformation != _transformation) {
		Invalidate(true);
		_transformation = transformation;
	}
}

void Instance::SetPlacementStatus(const PlacementStatus& placementstatus)
// **********************************************************************
{
//	if (placementstatus != _placementStatus) {
//		Invalidate(true);
        _placementStatus = placementstatus;
//	}
}

void Instance::SetMasterCell(Cell* masterCell, bool secureFlag)
// ************************************************************
{
	if (masterCell != _masterCell) {

		if (!masterCell)
			throw Error("Can't set master : null master cell");

		if (secureFlag && _cell->IsCalledBy(masterCell))
			throw Error("Can't set master : cyclic construction");

		list<Plug*> connectedPlugList;
		list<Net*> masterNetList;
		for_each_plug(plug, GetConnectedPlugs()) {
			Net* masterNet = masterCell->GetNet(plug->GetMasterNet()->GetName());
			if (!masterNet || !masterNet->IsExternal())
				throw Error("Can't set master (bad master net matching)");
			connectedPlugList.push_back(plug);
			masterNetList.push_back(masterNet);
			end_for;
		}

		for_each_shared_path(sharedPath, _GetSharedPathes()) {
			if (!sharedPath->GetTailSharedPath())
				// if the tail is empty the SharedPath isn't impacted by the change
				delete sharedPath;
			end_for;
		}

		Invalidate(true);

		for_each_plug(plug, GetUnconnectedPlugs()) {
			plug->_Delete();
			end_for;
		}

		while (!connectedPlugList.empty() && !masterNetList.empty()) {
			Plug* plug = connectedPlugList.front();
			Net* masterNet = masterNetList.front();
			_plugMap._Remove(plug);
			plug->_SetMasterNet(masterNet);
			_plugMap._Insert(plug);
			connectedPlugList.pop_front();
			masterNetList.pop_front();
		}

		_masterCell->_GetSlaveInstanceSet()._Remove(this);
		_masterCell = masterCell;
		_masterCell->_GetSlaveInstanceSet()._Insert(this);

		for_each_net(externalNet, _masterCell->GetExternalNets()) {
			if (!GetPlug(externalNet)) Plug::_Create(this, externalNet);
			end_for;
		}
	}
}

void Instance::_PostCreate()
// *************************
{
	_cell->_GetInstanceMap()._Insert(this);
	_masterCell->_GetSlaveInstanceSet()._Insert(this);

	for_each_net(externalNet, _masterCell->GetExternalNets()) {
		Plug::_Create(this, externalNet);
		end_for;
	}

	Inherit::_PostCreate();
}

void Instance::_PreDelete()
// ************************
{
	for_each_shared_path(sharedPath, _GetSharedPathes()) delete sharedPath; end_for;

	Inherit::_PreDelete();

	for_each_plug(plug, GetPlugs()) plug->_Delete(); end_for;

	_masterCell->_GetSlaveInstanceSet()._Remove(this);
	_cell->_GetInstanceMap()._Remove(this);
}

string Instance::_GetString() const
// ********************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_name));
	s.insert(s.length() - 1, " " + GetString(_masterCell->GetName()));
	return s;
}

Record* Instance::_GetRecord() const
// ***************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Cell", _cell));
		record->Add(GetSlot("Name", &_name));
		record->Add(GetSlot("MasterCell", _masterCell));
		record->Add(GetSlot("Transformation", &_transformation));
		record->Add(GetSlot("PlacementStatus", _placementStatus));
        record->Add(GetSlot("XCenter", GetValue(GetAbutmentBox().GetXCenter())));
        record->Add(GetSlot("YCenter", GetValue(GetAbutmentBox().GetYCenter())));
		record->Add(GetSlot("Plugs", &_plugMap));
		record->Add(GetSlot("SharedPathes", &_sharedPathMap));
	}
	return record;
}

//void Instance::_DrawPhantoms(View* view, const Box& updateArea, const Transformation& transformation)
//// **************************************************************************************************
//{
//	Symbol* symbol = _masterCell->GetSymbol();
//	if (!symbol) {
//		Box masterArea = updateArea;
//		Transformation masterTransformation = _transformation;
//		_transformation.GetInvert().ApplyOn(masterArea);
//		transformation.ApplyOn(masterTransformation);
//		_masterCell->_DrawPhantoms(view, masterArea, masterTransformation);
//	}
//}
//
//void Instance::_DrawBoundaries(View* view, const Box& updateArea, const Transformation& transformation)
//// ****************************************************************************************************
//{
//	Box masterArea = updateArea;
//	Transformation masterTransformation = _transformation;
//	_transformation.GetInvert().ApplyOn(masterArea);
//	transformation.ApplyOn(masterTransformation);
//	Symbol* symbol = _masterCell->GetSymbol();
//	if (!symbol)
//		_masterCell->_DrawBoundaries(view, masterArea, masterTransformation);
//	else
//		_masterCell->GetSymbol()->_Draw(view, masterArea, masterTransformation);
//}
//
//void Instance::_DrawRubbers(View* view, const Box& updateArea, const Transformation& transformation)
//// *************************************************************************************************
//{
//	Box masterArea = updateArea;
//	Transformation masterTransformation = _transformation;
//	_transformation.GetInvert().ApplyOn(masterArea);
//	transformation.ApplyOn(masterTransformation);
//	_masterCell->_DrawRubbers(view, masterArea, masterTransformation);
//}
//
//void Instance::_DrawMarkers(View* view, const Box& updateArea, const Transformation& transformation)
//// *************************************************************************************************
//{
//	Box masterArea = updateArea;
//	Transformation masterTransformation = _transformation;
//	_transformation.GetInvert().ApplyOn(masterArea);
//	transformation.ApplyOn(masterTransformation);
//	_masterCell->_DrawMarkers(view, masterArea, masterTransformation);
//}
//
//void Instance::_DrawDisplaySlots(View* view, const Box& area, const Box& updateArea, const Transformation& transformation)
//// ***********************************************************************************************************************
//{
//	Box masterArea = updateArea;
//	Transformation masterTransformation = _transformation;
//	_transformation.GetInvert().ApplyOn(masterArea);
//	transformation.ApplyOn(masterTransformation);
//	_masterCell->_DrawDisplaySlots(view, area, masterArea, masterTransformation);
//}
//
//bool Instance::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
//// ****************************************************************************************
//{
//	Symbol* symbol = _masterCell->GetSymbol();
//	if (!symbol)
//		return (view->PhantomsAreVisible() || view->BoundariesAreVisible()) &&
//				 GetAbutmentBox().Intersect(Box(point).Inflate(aperture));
//	else {
//		Point masterPoint = point;
//		_transformation.GetInvert().ApplyOn(masterPoint);
//		return (view->BoundariesAreVisible() && symbol->_IsInterceptedBy(view, masterPoint, aperture));
//	}
//}
//
//void Instance::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// ****************************************************************************************************
//{
//	Symbol* symbol = _masterCell->GetSymbol();
//	if (!symbol) {
//		Box masterArea = updateArea;
//		Transformation masterTransformation = _transformation;
//		_transformation.GetInvert().ApplyOn(masterArea);
//		transformation.ApplyOn(masterTransformation);
//		_masterCell->_DrawContent(view, basicLayer, masterArea, masterTransformation);
//	}
//}
//
//void Instance::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
//// ***********************************************************************************************
//{
//	Symbol* symbol = _masterCell->GetSymbol();
//	if (!symbol) {
//		Box abutmentBox = transformation.GetBox(GetAbutmentBox());
//		view->FillRectangle(abutmentBox);
//		view->DrawRectangle(abutmentBox);
//        
//        if ( view->GetScale() > 1 )
//        {
//            if ( view->IsTextVisible() )
//            {
//                string text = GetString ( _name ) + " ("
//                            + GetString ( GetValue ( abutmentBox.GetXCenter () ) ) + ","
//                            + GetString ( GetValue ( abutmentBox.GetYCenter () ) ) + ")";
//                view->DrawString ( text, abutmentBox.GetXMin(), abutmentBox.GetYMax() ); 
//            }
//        }
//	}
//	else {
//		Box masterArea = updateArea;
//		Transformation masterTransformation = _transformation;
//		_transformation.GetInvert().ApplyOn(masterArea);
//		transformation.ApplyOn(masterTransformation);
//		symbol->_Highlight(view, masterArea, masterTransformation);
//	}
//}
//

// ****************************************************************************************************
// Instance::PlugMap implementation
// ****************************************************************************************************

Instance::PlugMap::PlugMap()
// *************************
:	Inherit()
{
}

const Net* Instance::PlugMap::_GetKey(Plug* plug) const
// ****************************************************
{
	return plug->GetMasterNet();
}

unsigned Instance::PlugMap::_GetHashValue(const Net* masterNet) const
// ******************************************************************
{
	return ( (unsigned int)( (unsigned long)masterNet ) ) / 8;
}

Plug* Instance::PlugMap::_GetNextElement(Plug* plug) const
// *******************************************************
{
	return plug->_GetNextOfInstancePlugMap();
}

void Instance::PlugMap::_SetNextElement(Plug* plug, Plug* nextPlug) const
// **********************************************************************
{
	plug->_SetNextOfInstancePlugMap(nextPlug);
}



// ****************************************************************************************************
// Instance::SharedPathMap implementation
// ****************************************************************************************************

Instance::SharedPathMap::SharedPathMap()
// *************************************
:	Inherit()
{
}

const SharedPath* Instance::SharedPathMap::_GetKey(SharedPath* sharedPath) const
// *****************************************************************************
{
	return sharedPath->GetTailSharedPath();
}

unsigned Instance::SharedPathMap::_GetHashValue(const SharedPath* tailSharedPath) const
// ************************************************************************************
{
	return ( (unsigned int)( (unsigned long)tailSharedPath ) ) / 8;
}

SharedPath* Instance::SharedPathMap::_GetNextElement(SharedPath* sharedPath) const
// *******************************************************************************
{
	return sharedPath->_GetNextOfInstanceSharedPathMap();
}

void Instance::SharedPathMap::_SetNextElement(SharedPath* sharedPath, SharedPath* nextSharedPath) const
// ****************************************************************************************************
{
	sharedPath->_SetNextOfInstanceSharedPathMap(nextSharedPath);
};

// ****************************************************************************************************
// Instance::PlacementStatus implementation
// ****************************************************************************************************

Instance::PlacementStatus::PlacementStatus(const Code& code)
// *********************************************************
:	_code(code)
{
}

Instance::PlacementStatus::PlacementStatus(const PlacementStatus& placementstatus)
// *******************************************************************************
:	_code(placementstatus._code)
{
}

Instance::PlacementStatus& Instance::PlacementStatus::operator=(const PlacementStatus& placementstatus)
// ****************************************************************************************************
{
    _code = placementstatus._code;
	return *this;
}

string Instance::PlacementStatus::_GetString() const
// *************************************************
{
  return GetString(&_code);
}

Record* Instance::PlacementStatus::_GetRecord() const
// ********************************************
{
	Record* record = new Record(GetString(this));
	record->Add(GetSlot("Code", &_code));
	return record;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
