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
:    Inherit(technology, name, minimalSize, minimalSpacing),
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

CompositeLayer* CompositeLayer::create(Technology* technology, const Name& name, const Type& type, const Unit& minimalSize, const Unit& minimalSpacing)
// ****************************************************************************************************
{
    CompositeLayer* compositeLayer =
        new CompositeLayer(technology, name, type, minimalSize, minimalSpacing);

    compositeLayer->_postCreate();

    return compositeLayer;
}

BasicLayers CompositeLayer::getBasicLayers() const
// ***********************************************
{
    return getCollection(_basicLayerList);
}

Unit CompositeLayer::getContactSize(const BasicLayer* basicLayer) const
// **************************************************************
{
    SizeMap::const_iterator it = _contactSizeMap.find(basicLayer);
    return ((it == _contactSizeMap.end()) ? 0 : (*it).second);
}

Unit CompositeLayer::getSegmentSize(const BasicLayer* basicLayer) const
// **************************************************************
{
    SizeMap::const_iterator it = _segmentSizeMap.find(basicLayer);
    return ((it == _segmentSizeMap.end()) ? 0 : (*it).second);
}

Unit CompositeLayer::getSegmentExtention(const BasicLayer* basicLayer) const
// *******************************************************************
{
    SizeMap::const_iterator it = _segmentExtentionMap.find(basicLayer);
    return ((it == _segmentExtentionMap.end()) ? 0 : (*it).second);
}

Unit CompositeLayer::getPadSize(const BasicLayer* basicLayer) const
// **********************************************************
{
    SizeMap::const_iterator it = _padSizeMap.find(basicLayer);
    return ((it == _padSizeMap.end()) ? 0 : (*it).second);
}

void CompositeLayer::add(BasicLayer* basicLayer, const Unit& contactSize, const Unit& segmentSize, const Unit& segmentExtention, const Unit& padSize)
// ****************************************************************************************************
{
    if (!basicLayer)
        throw Error("Can't add basic layer : null basic layer");

    if (contains(basicLayer))
        throw Error("Can't add basic layer : already done");

    _basicLayerList.push_back(basicLayer);

    _setMask(getMask() | basicLayer->getMask());
    _setExtractMask(getExtractMask() | basicLayer->getExtractMask());

    if (contactSize != 0) _contactSizeMap[basicLayer] = contactSize;
    if (segmentSize != 0) _segmentSizeMap[basicLayer] = segmentSize;
    if (segmentExtention != 0) _segmentExtentionMap[basicLayer] = segmentExtention;
    if (padSize != 0) _padSizeMap[basicLayer] = padSize;

    _maximalContactSize = max(contactSize, _maximalContactSize);
    _maximalSegmentSize = max(segmentSize, _maximalSegmentSize);
    _maximalSegmentExtention = max(segmentExtention, _maximalSegmentExtention);
    _maximalPadSize = max(padSize, _maximalPadSize);
}

void CompositeLayer::setContactSize(BasicLayer* basicLayer, const Unit& contactSize)
// *********************************************************************************
{
    if (!basicLayer)
        throw Error("Can't add basic layer : null basic layer");

    if (!contains(basicLayer))
        throw Error("Basic layer not part of composite layer");

    if (contactSize != 0) _contactSizeMap[basicLayer] = contactSize;

    _maximalContactSize = max(contactSize, _maximalContactSize);
}

void CompositeLayer::setSegmentSize(BasicLayer* basicLayer, const Unit& segmentSize)
// *********************************************************************************
{
    if (!basicLayer)
        throw Error("Can't add basic layer : null basic layer");

    if (!contains(basicLayer))
        throw Error("Basic layer not part of composite layer");

    if (segmentSize != 0) _segmentSizeMap[basicLayer] = segmentSize;

    _maximalSegmentSize = max(segmentSize, _maximalSegmentSize);
}

void CompositeLayer::setSegmentExtention(BasicLayer* basicLayer, const Unit& segmentExtention)
// *********************************************************************************
{
    if (!basicLayer)
        throw Error("Can't add basic layer : null basic layer");

    if (!contains(basicLayer))
        throw Error("Basic layer not part of composite layer");

    if (segmentExtention != 0) _segmentExtentionMap[basicLayer] = segmentExtention;

    _maximalSegmentExtention = max(segmentExtention, _maximalSegmentExtention);
}

void CompositeLayer::remove(BasicLayer* basicLayer)
// ************************************************
{
    if (!basicLayer)
        throw Error("Can't remove basic layer : null basic layer");

    if (!contains(basicLayer))
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

    for_each_basic_layer(basicLayer, getBasicLayers()) {
        mask |= basicLayer->getMask();
        extractMask |= basicLayer->getExtractMask();
        _maximalContactSize = max(getContactSize(basicLayer), _maximalContactSize);
        _maximalSegmentSize = max(getSegmentSize(basicLayer), _maximalSegmentSize);
        _maximalSegmentExtention = max(getSegmentExtention(basicLayer), _maximalSegmentExtention);
        _maximalPadSize = max(getPadSize(basicLayer), _maximalPadSize);
        end_for;
    }

    _setMask(mask);
    _setExtractMask(extractMask);
}

string CompositeLayer::_getString() const
// **************************************
{
    string s = Inherit::_getString();
    /*
    s.insert(s.length() - 1, " " + getString(_type));
    s.insert(s.length() - 1, " {");
    string separator = "";
    for_each_basic_layer(basicLayer, getBasicLayers()) {
        s.insert(s.length() - 1, separator + getString(basicLayer->getName()));
        separator = "|";
        end_for;
    }
    s.insert(s.length() - 1, "}");
    */
    return s;
}

Record* CompositeLayer::_getRecord() const
// ***************************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Type", &_type));
        record->add(getSlot("BasicLayers", &_basicLayerList));
        record->add(getSlot("ContactSizes", &_contactSizeMap));
        record->add(getSlot("SegmentSizes", &_segmentSizeMap));
        record->add(getSlot("SegmentExtentions", &_segmentExtentionMap));
        record->add(getSlot("PadSizes", &_padSizeMap));
        record->add(getSlot("MaximalContactSize", &_maximalContactSize));
        record->add(getSlot("MaximalSegmentSize", &_maximalSegmentSize));
        record->add(getSlot("MaximalSegmentExtention", &_maximalSegmentExtention));
        record->add(getSlot("MaximalPadSize", &_maximalPadSize));
    }
    return record;
}

void CompositeLayer::_updateSymbolicBasicLayer(const Layer::Mask& visibleBasicLayersMask)
// **************************************************************************************
{
    _symbolicBasicLayer = NULL;
    BasicLayer* symbolicBasicLayer = NULL;
    for_each_basic_layer(basicLayer, getBasicLayers()) {
        if (basicLayer->getMask() & visibleBasicLayersMask) {
            symbolicBasicLayer = basicLayer;
            if (basicLayer->getType() == BasicLayer::Type::CONTACT)
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
:    _code(code)
{
}

CompositeLayer::Type::Type(const Type& type)
// *****************************************
:    _code(type._code)
{
}

CompositeLayer::Type& CompositeLayer::Type::operator=(const Type& type)
// ********************************************************************
{
    _code = type._code;
    return *this;
}

string CompositeLayer::Type::_getString() const
// ********************************************
{
    switch (_code) {
        case UNDEFINED : return "UNDEFINED";
        case METAL : return "METAL";
        case VIA : return "VIA";
    }
    return "ABNORMAL";
}

Record* CompositeLayer::Type::_getRecord() const
// *********************************************
{
    Record* record = new Record(getString(this));
    record->add(getSlot("Code", (int)_code));
    return record;
}




} // End of Hurricane namespace.

// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

bool Scan(const string& s, Hurricane::CompositeLayer::Type& type)
// ***************************************************
{
    if (s == "UNDEFINED") {
        type = Hurricane::CompositeLayer::Type::UNDEFINED;
        return true;
    }
    if (s == "METAL") {
        type = Hurricane::CompositeLayer::Type::METAL;
        return true;
    }
    if (s == "VIA") {
        type = Hurricane::CompositeLayer::Type::VIA;
        return true;
    }
    return false;
}


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
