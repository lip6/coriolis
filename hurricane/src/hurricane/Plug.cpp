// ****************************************************************************************************
// File: Plug.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Plug.h"
#include "Net.h"
#include "Cell.h"
#include "Instance.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Plug_IsConnectedFilter : public Filter<Plug*> {
// ************************************************

	public: Plug_IsConnectedFilter() {};

	public: Plug_IsConnectedFilter(const Plug_IsConnectedFilter& filter) {};

	public: Plug_IsConnectedFilter& operator=(const Plug_IsConnectedFilter& filter) {return *this;};

	public: virtual Filter<Plug*>* GetClone() const {return new Plug_IsConnectedFilter(*this);};

	public: virtual bool Accept(Plug* plug) const {return plug->IsConnected();};

	public: virtual string _GetString() const {return "<" + _TName("Plug::IsConnectedFilter>");};

};



// ****************************************************************************************************
// Plug implementation
// ****************************************************************************************************

Plug::Plug(Instance* instance, Net* masterNet)
// *******************************************
:	Inherit(NULL, true),
	_instance(instance),
	_masterNet(masterNet),
	_nextOfInstancePlugMap(NULL)
{
	if (!_instance)
		throw Error("Can't create " + _TName("Plug") + " : null instance");

	if (!_masterNet)
		throw Error("Can't create " + _TName("Plug") + " : null master net");

	if (_masterNet->GetCell() != _instance->GetMasterCell())
		throw Error("Can't create " + _TName("Plug") + " : incompatible master net");

	if (!_masterNet->IsExternal())
		throw Error("Can't create " + _TName("Plug") + " : not an external master net");

	if (_instance->GetPlug(_masterNet))
		throw Error("Can't create " + _TName("Plug") + " : already exists");
}

void Plug::Delete()
// ****************
{
	throw Error("Abnormal deletion of " + _TName("Plug"));
}

Cell* Plug::GetCell() const
// ************************
{
	return _instance->GetCell();
}

Unit Plug::GetX() const
// ********************
{
	return _instance->GetTransformation().GetX(_masterNet->GetPosition());
}

Unit Plug::GetY() const
// ********************
{
	return _instance->GetTransformation().GetY(_masterNet->GetPosition());
}

Point Plug::GetPosition() const
// ****************************
{
	return _instance->GetTransformation().GetPoint(_masterNet->GetPosition());
}

Box Plug::GetBoundingBox() const
// *****************************
{
	return _instance->GetTransformation().GetBox(_masterNet->GetPosition());
}

Box Plug::GetBoundingBox(BasicLayer* basicLayer) const
// ***************************************************
{
	return Box();
}

PlugFilter Plug::GetIsConnectedFilter()
// ************************************
{
	return Plug_IsConnectedFilter();
}

PlugFilter Plug::GetIsUnconnectedFilter()
// **************************************
{
	return !Plug_IsConnectedFilter();
}

void Plug::Materialize()
// *********************
{
}

void Plug::Unmaterialize()
// ***********************
{
}

void Plug::SetNet(Net* net)
// ************************
{
	if (net != GetNet()) {

		if (net && (GetCell() != net->GetCell()))
			throw Error("Can't change net of plug : net : " + GetString(net) + "does not belong to the cell : " + GetString(GetCell()));

		if (!GetBodyHook()->GetSlaveHooks().IsEmpty())
			throw Error("Can't change net of plug : not empty slave hooks");

		_SetNet(net);
	}
}

Plug* Plug::_Create(Instance* instance, Net* masterNet)
// ****************************************************
{
	Plug* plug = new Plug(instance, masterNet);

	plug->_PostCreate();

	return plug;
}

void Plug::_PostCreate()
// *********************
{
	_instance->_GetPlugMap()._Insert(this);

	Inherit::_PostCreate();
}

void Plug::_Delete()
// *****************
{
	_PreDelete();

	delete this;
}

void Plug::_PreDelete()
// ********************
{
// trace << "entering Plug::_PreDelete: " << this << endl;
// trace_in();

	Inherit::_PreDelete();

	_instance->_GetPlugMap()._Remove(this);

// trace << "exiting Plug::_PreDelete:" << endl;
// trace_out();
}

string Plug::_GetString() const
// ****************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetName());
	return s;
}

Record* Plug::_GetRecord() const
// ***********************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Instance", _instance));
		record->Add(GetSlot("MasterNet", _masterNet));
	}
	return record;
}

string Plug::GetName() const
// *************************
{
    return GetString(_instance->GetName())
         + "."
         + GetString(_masterNet->GetName());
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
