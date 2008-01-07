// ****************************************************************************************************
// File: CompositeLayer.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "CompositeLayer.h"
#include "Technology.h"
#include "BasicLayer.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// CompositeLayer implementation
// ****************************************************************************************************

CompositeLayer::CompositeLayer(Technology* technology, const Name& name, const Type& type, const Unit& minimalSize, const Unit& minimalSpacing)
// ****************************************************************************************************
:	Inherit(technology, name, minimalSize, minimalSpacing),
	_type(type),
	_basicLayerList(),
	_contactSizeMap(),
	_segmentSizeMap(),
	_segmentExtentionMap(),
	_padSizeMap(),
	_maximalContactSize(0),
	_maximalSegmentSize(0),
	_maximalSegmentExtention(0),
	_maximalPadSize(0),
	_symbolicBasicLayer(NULL)
{
}

CompositeLayer* CompositeLayer::Create(Technology* technology, const Name& name, const Type& type, const Unit& minimalSize, const Unit& minimalSpacing)
// ****************************************************************************************************
{
	CompositeLayer* compositeLayer =
		new CompositeLayer(technology, name, type, minimalSize, minimalSpacing);

	compositeLayer->_PostCreate();

	return compositeLayer;
}

BasicLayers CompositeLayer::GetBasicLayers() const
// ***********************************************
{
	return GetCollection(_basicLayerList);
}

Unit CompositeLayer::GetContactSize(const BasicLayer* basicLayer) const
// **************************************************************
{
	SizeMap::const_iterator it = _contactSizeMap.find(basicLayer);
	return ((it == _contactSizeMap.end()) ? 0 : (*it).second);
}

Unit CompositeLayer::GetSegmentSize(const BasicLayer* basicLayer) const
// **************************************************************
{
	SizeMap::const_iterator it = _segmentSizeMap.find(basicLayer);
	return ((it == _segmentSizeMap.end()) ? 0 : (*it).second);
}

Unit CompositeLayer::GetSegmentExtention(const BasicLayer* basicLayer) const
// *******************************************************************
{
	SizeMap::const_iterator it = _segmentExtentionMap.find(basicLayer);
	return ((it == _segmentExtentionMap.end()) ? 0 : (*it).second);
}

Unit CompositeLayer::GetPadSize(const BasicLayer* basicLayer) const
// **********************************************************
{
	SizeMap::const_iterator it = _padSizeMap.find(basicLayer);
	return ((it == _padSizeMap.end()) ? 0 : (*it).second);
}

void CompositeLayer::Add(BasicLayer* basicLayer, const Unit& contactSize, const Unit& segmentSize, const Unit& segmentExtention, const Unit& padSize)
// ****************************************************************************************************
{
	if (!basicLayer)
		throw Error("Can't add basic layer : null basic layer");

	if (Contains(basicLayer))
		throw Error("Can't add basic layer : already done");

	_basicLayerList.push_back(basicLayer);

	_SetMask(GetMask() | basicLayer->GetMask());
	_SetExtractMask(GetExtractMask() | basicLayer->GetExtractMask());

	if (contactSize != 0) _contactSizeMap[basicLayer] = contactSize;
	if (segmentSize != 0) _segmentSizeMap[basicLayer] = segmentSize;
	if (segmentExtention != 0) _segmentExtentionMap[basicLayer] = segmentExtention;
	if (padSize != 0) _padSizeMap[basicLayer] = padSize;

	_maximalContactSize = max(contactSize, _maximalContactSize);
	_maximalSegmentSize = max(segmentSize, _maximalSegmentSize);
	_maximalSegmentExtention = max(segmentExtention, _maximalSegmentExtention);
	_maximalPadSize = max(padSize, _maximalPadSize);
}

void CompositeLayer::Remove(BasicLayer* basicLayer)
// ************************************************
{
	if (!basicLayer)
		throw Error("Can't remove basic layer : null basic layer");

	if (!Contains(basicLayer))
		throw Error("Can't remove basic layer : not contained");

	_basicLayerList.remove(basicLayer);

	_contactSizeMap.erase(basicLayer);
	_segmentSizeMap.erase(basicLayer);
	_segmentExtentionMap.erase(basicLayer);
	_padSizeMap.erase(basicLayer);

	_maximalContactSize = 0;
	_maximalSegmentSize = 0;
	_maximalSegmentExtention = 0;
	_maximalPadSize = 0;

	Mask mask = 0;
	Mask extractMask = 0;

	for_each_basic_layer(basicLayer, GetBasicLayers()) {
		mask |= basicLayer->GetMask();
		extractMask |= basicLayer->GetExtractMask();
		_maximalContactSize = max(GetContactSize(basicLayer), _maximalContactSize);
		_maximalSegmentSize = max(GetSegmentSize(basicLayer), _maximalSegmentSize);
		_maximalSegmentExtention = max(GetSegmentExtention(basicLayer), _maximalSegmentExtention);
		_maximalPadSize = max(GetPadSize(basicLayer), _maximalPadSize);
		end_for;
	}

	_SetMask(mask);
	_SetExtractMask(extractMask);
}

string CompositeLayer::_GetString() const
// **************************************
{
	string s = Inherit::_GetString();
	/*
	s.insert(s.length() - 1, " " + GetString(_type));
	s.insert(s.length() - 1, " {");
	string separator = "";
	for_each_basic_layer(basicLayer, GetBasicLayers()) {
		s.insert(s.length() - 1, separator + GetString(basicLayer->GetName()));
		separator = "|";
		end_for;
	}
	s.insert(s.length() - 1, "}");
	*/
	return s;
}

Record* CompositeLayer::_GetRecord() const
// ***************************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Type", &_type));
		record->Add(GetSlot("BasicLayers", &_basicLayerList));
		record->Add(GetSlot("ContactSizes", &_contactSizeMap));
		record->Add(GetSlot("SegmentSizes", &_segmentSizeMap));
		record->Add(GetSlot("SegmentExtentions", &_segmentExtentionMap));
		record->Add(GetSlot("PadSizes", &_padSizeMap));
		record->Add(GetSlot("MaximalContactSize", &_maximalContactSize));
		record->Add(GetSlot("MaximalSegmentSize", &_maximalSegmentSize));
		record->Add(GetSlot("MaximalSegmentExtention", &_maximalSegmentExtention));
		record->Add(GetSlot("MaximalPadSize", &_maximalPadSize));
	}
	return record;
}

void CompositeLayer::_UpdateSymbolicBasicLayer(const Layer::Mask& visibleBasicLayersMask)
// **************************************************************************************
{
	_symbolicBasicLayer = NULL;
	BasicLayer* symbolicBasicLayer = NULL;
	for_each_basic_layer(basicLayer, GetBasicLayers()) {
		if (basicLayer->GetMask() & visibleBasicLayersMask) {
			symbolicBasicLayer = basicLayer;
			if (basicLayer->GetType() == BasicLayer::Type::CONTACT)
				_symbolicBasicLayer = basicLayer;
		}
		end_for;
	}
	if (!_symbolicBasicLayer) _symbolicBasicLayer = symbolicBasicLayer;
}

// ****************************************************************************************************
// CompositeLayer::Type implementation
// ****************************************************************************************************

CompositeLayer::Type::Type(const Code& code)
// *****************************************
:	_code(code)
{
}

CompositeLayer::Type::Type(const Type& type)
// *****************************************
:	_code(type._code)
{
}

CompositeLayer::Type& CompositeLayer::Type::operator=(const Type& type)
// ********************************************************************
{
	_code = type._code;
	return *this;
}

string CompositeLayer::Type::_GetString() const
// ********************************************
{
	switch (_code) {
		case UNDEFINED : return "UNDEFINED";
		case METAL : return "METAL";
		case VIA : return "VIA";
	}
	return "ABNORMAL";
}

Record* CompositeLayer::Type::_GetRecord() const
// *********************************************
{
	Record* record = new Record(GetString(this));
	record->Add(GetSlot("Code", (int)_code));
	return record;
}




} // End of Hurricane namespace.

// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

bool Scan(const string& s, H::CompositeLayer::Type& type)
// ***************************************************
{
	if (s == "UNDEFINED") {
		type = H::CompositeLayer::Type::UNDEFINED;
		return true;
	}
	if (s == "METAL") {
		type = H::CompositeLayer::Type::METAL;
		return true;
	}
	if (s == "VIA") {
		type = H::CompositeLayer::Type::VIA;
		return true;
	}
	return false;
}


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
