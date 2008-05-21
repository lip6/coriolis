// ****************************************************************************************************
// File: BasicLayer.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/BasicLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/CompositeLayer.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// BasicLayer_BasicLayers declaration
// ****************************************************************************************************

class BasicLayer_BasicLayers : public Collection<BasicLayer*> {
// **********************************************************

// Types
// *****

    public: typedef Collection<BasicLayer*> Inherit;

    public: class Locator : public Hurricane::Locator<BasicLayer*> {
    // ***********************************************************

        public: typedef Hurricane::Locator<BasicLayer*> Inherit;

        private: const BasicLayer* _basicLayer;

        public: Locator(const BasicLayer* basicLayer = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual BasicLayer* getElement() const;
        public: virtual Hurricane::Locator<BasicLayer*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Atributes
// *********

    private: const BasicLayer* _basicLayer;

// Constructors
// ************

    public: BasicLayer_BasicLayers(const BasicLayer* basicLayer = NULL);
    public: BasicLayer_BasicLayers(const BasicLayer_BasicLayers& basicLayers);

// Operators
// *********

    public: BasicLayer_BasicLayers& operator=(const BasicLayer_BasicLayers& basicLayers);

// Accessors
// *********

    public: virtual Collection<BasicLayer*>* getClone() const;
    public: virtual Hurricane::Locator<BasicLayer*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// BasicLayer implementation
// ****************************************************************************************************

BasicLayer::BasicLayer(Technology* technology, const Name& name, const Type& type, unsigned extractNumber, const Unit& minimalSize, const Unit& minimalSpacing)
// ****************************************************************************************************
:    Inherit(technology, name, minimalSize, minimalSpacing),
    _type(type),
    _extractNumber(extractNumber),
    _realName("<not associated>")
{
}

BasicLayer* BasicLayer::create(Technology* technology, const Name& name, const Type& type, unsigned extractNumber, const Unit& minimalSize, const Unit& minimalSpacing)
// ****************************************************************************************************
{
    BasicLayer* basicLayer =
        new BasicLayer(technology, name, type, extractNumber, minimalSize, minimalSpacing);

    basicLayer->_postCreate();

    return basicLayer;
}

BasicLayers BasicLayer::getBasicLayers() const
// *******************************************
{
    return BasicLayer_BasicLayers(this);
}

void BasicLayer::_postCreate()
// ***************************
{
    Mask basicLayersMask = 0;
    for_each_basic_layer(basicLayer, getTechnology()->getBasicLayers()) {
        basicLayersMask |= basicLayer->getMask();
        end_for;
    }

    Mask mask = 1;
    while (mask && (mask & basicLayersMask)) mask = mask<<1;

    if (!mask)
        throw Error("Can't create " + _TName("BasicLayer") + " : mask capacity overflow");

    _setMask(mask);

    if (_extractNumber) {
        Mask extractMask = (1 << _extractNumber);

        if (!extractMask)
            throw Error("Can't create " + _TName("BasicLayer") + " : extract mask capacity overflow");

        _setExtractMask(extractMask);
    }

    Inherit::_postCreate();
}

void BasicLayer::_preDestroy()
// **************************
{
    Inherit::_preDestroy();

    CompositeLayers compositeLayers = getTechnology()->getCompositeLayers();
    for_each_composite_layer(compositeLayer, compositeLayers) {
        if (compositeLayer->contains(this)) compositeLayer->remove(this);
        end_for;
    }
}

string BasicLayer::_getString() const
// **********************************
{
    string s = Inherit::_getString();
    // s.insert(s.length() - 1, " " + getString(_type));
    return s;
}

Record* BasicLayer::_getRecord() const
// *****************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Type", &_type));
        record->add(getSlot("RealName", &_realName));
    }
    return record;
}

// ****************************************************************************************************
// BasicLayer_BasicLayers implementation
// ****************************************************************************************************

BasicLayer_BasicLayers::BasicLayer_BasicLayers(const BasicLayer* basicLayer)
// *************************************************************************
:     Inherit(),
    _basicLayer(basicLayer)
{
}

BasicLayer_BasicLayers::BasicLayer_BasicLayers(const BasicLayer_BasicLayers& basicLayers)
// **************************************************************************************
:     Inherit(),
    _basicLayer(basicLayers._basicLayer)
{
}

BasicLayer_BasicLayers& BasicLayer_BasicLayers::operator=(const BasicLayer_BasicLayers& basicLayers)
// *************************************************************************************************
{
    _basicLayer = basicLayers._basicLayer;
    return *this;
}

Collection<BasicLayer*>* BasicLayer_BasicLayers::getClone() const
// **************************************************************
{
    return new BasicLayer_BasicLayers(*this);
}

Locator<BasicLayer*>* BasicLayer_BasicLayers::getLocator() const
// *************************************************************
{
    return new Locator(_basicLayer);
}

string BasicLayer_BasicLayers::_getString() const
// **********************************************
{
    string s = "<" + _TName("BasicLayer::BasicLayers");
    if (_basicLayer) s += " " + getString(_basicLayer);
    s += ">";
    return s;
}



// ****************************************************************************************************
// BasicLayer_BasicLayers::Locator implementation
// ****************************************************************************************************

BasicLayer_BasicLayers::Locator::Locator(const BasicLayer* basicLayer)
// *******************************************************************
:    Inherit(),
    _basicLayer(basicLayer)
{
}

BasicLayer_BasicLayers::Locator::Locator(const Locator& locator)
// *************************************************************
:    Inherit(),
    _basicLayer(locator._basicLayer)
{
}

BasicLayer_BasicLayers::Locator& BasicLayer_BasicLayers::Locator::operator=(const Locator& locator)
// ************************************************************************************************
{
    _basicLayer = locator._basicLayer;
    return *this;
}

BasicLayer* BasicLayer_BasicLayers::Locator::getElement() const
// ************************************************************
{
    return (BasicLayer*)_basicLayer;
}

Locator<BasicLayer*>* BasicLayer_BasicLayers::Locator::getClone() const
// ********************************************************************
{
    return new Locator(*this);
}

bool BasicLayer_BasicLayers::Locator::isValid() const
// **************************************************
{
    return (_basicLayer != NULL);
}

void BasicLayer_BasicLayers::Locator::progress()
// *********************************************
{
    _basicLayer = NULL;
}

string BasicLayer_BasicLayers::Locator::_getString() const
// *******************************************************
{
    string s = "<" + _TName("BasicLayer::BasicLayers::Locator");
    if (_basicLayer) s += " " + getString(_basicLayer);
    s += ">";
    return s;
}



// ****************************************************************************************************
// BasicLayer::Type implementation
// ****************************************************************************************************

BasicLayer::Type::Type(const Code& code)
// *************************************
:    _code(code)
{
}

BasicLayer::Type::Type(const Type& type)
// *************************************
:    _code(type._code)
{
}

BasicLayer::Type& BasicLayer::Type::operator=(const Type& type)
// ************************************************************
{
    _code = type._code;
    return *this;
}

string BasicLayer::Type::_getString() const
// ****************************************
{
    return getString(_code);
}

Record* BasicLayer::Type::_getRecord() const
// ***********************************
{
    Record* record = new Record(getString(this));
    record->add(getSlot("Code", &_code));
    return record;
}

} // End of Hurricane namespace.

bool Scan(const string& s, Hurricane::BasicLayer::Type& type)
// **************************************************
{
    if (s == "UNDEFINED") {
        type = Hurricane::BasicLayer::Type::UNDEFINED;
        return true;
    }
    if (s == "CONDUCTING") {
        type = Hurricane::BasicLayer::Type::CONDUCTING;
        return true;
    }
    if (s == "CONTACT") {
        type = Hurricane::BasicLayer::Type::CONTACT;
        return true;
    }
    return false;
}

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
