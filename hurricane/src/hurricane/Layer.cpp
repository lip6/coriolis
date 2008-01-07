// ****************************************************************************************************
// File: Layer.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Layer.h"
#include "Technology.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Layer implementation
// ****************************************************************************************************

Layer::Layer(Technology* technology, const Name& name, const Unit& minimalSize, const Unit& minimalSpacing, const Unit& pitch)
// ****************************************************************************************************
:	Inherit(),
	_technology(technology),
	_name(name),
	_mask(0),
	_extractMask(0),
	_minimalSize(minimalSize),
	_minimalSpacing(minimalSpacing),
    _pitch(pitch),
	_nextOfTechnologyLayerMap(NULL)
{
	if (!_technology)
		throw Error("Can't create " + _TName("Layer") + " : null technology");

	if (_name.IsEmpty())
		throw Error("Can't create " + _TName("Layer") + " : empty name");

	if (_technology->GetLayer(_name))
		throw Error("Can't create " + _TName("Layer") + " " + GetString(_name) + " : already exists");
}

bool Layer::Contains(const Layer* layer) const
// *******************************************
{
	return (layer && ((_mask & layer->GetMask()) == layer->GetMask()));
}

bool Layer::Intersect(const Layer* layer) const
// ********************************************
{
	return ((_mask & layer->GetMask()) != 0);
}

void Layer::SetName(const Name& name)
// **********************************
{
	if (name != _name) {
		if (name.IsEmpty())
			throw Error("Can't change layer name : empty name");

		if (_technology->GetLayer(name))
			throw Error("Can't change layer name : already exists");

		_technology->_GetLayerMap()._Remove(this);
		_name = name;
		_technology->_GetLayerMap()._Insert(this);
	}
}

void Layer::SetMinimalSize(const Unit& minimalSize)
// ************************************************
{
	_minimalSize = minimalSize;
}

void Layer::SetMinimalSpacing(const Unit& minimalSpacing)
// ******************************************************
{
	_minimalSpacing = minimalSpacing;
}

void Layer::SetPitch(const Unit& pitch)
// ************************************
{
	_pitch = pitch;
}

void Layer::_PostCreate()
// **********************
{
	_technology->_GetLayerMap()._Insert(this);
	_technology->_GetLayerList().push_back(this);

	Inherit::_PostCreate();
}

void Layer::_PreDelete()
// *********************
{
	Inherit::_PreDelete();

	_technology->_GetLayerList().remove(this);
	_technology->_GetLayerMap()._Remove(this);
}

string Layer::_GetString() const
// *****************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_name));
	return s;
}

Record* Layer::_GetRecord() const
// ************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Technology", _technology));
		record->Add(GetSlot("Name", &_name));
		record->Add(GetSlot("Mask", &_mask));
		record->Add(GetSlot("ExtractMask", &_extractMask));
		record->Add(GetSlot("MinimalSize", &_minimalSize));
		record->Add(GetSlot("MinimalSpacing", &_minimalSpacing));
		record->Add(GetSlot("Pitch", &_pitch));
	}
	return record;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
