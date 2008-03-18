// ****************************************************************************************************
// File: Technology.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Technology.h"
#include "DataBase.h"
#include "Layer.h"
#include "BasicLayer.h"
#include "CompositeLayer.h"
#include "Error.h"



namespace Hurricane {



// ****************************************************************************************************
// Technology_BasicLayers declaration
// ****************************************************************************************************

class Technology_BasicLayers : public Collection<BasicLayer*> {
// **********************************************************

// Types
// *****

    public: typedef Collection<BasicLayer*> Inherit;

    public: class Locator : public Hurricane::Locator<BasicLayer*> {
    // ***********************************************************

        public: typedef Hurricane::Locator<BasicLayer*> Inherit;

        private: const Technology* _technology;
        private: Layer::Mask _mask;
        private: BasicLayerLocator _basicLayerLocator;

        public: Locator();
        public: Locator(const Technology* technology, const Layer::Mask& mask);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual BasicLayer* getElement() const;
        public: virtual Hurricane::Locator<BasicLayer*>* getClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Technology* _technology;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Technology_BasicLayers();
    public: Technology_BasicLayers(const Technology* technology, const Layer::Mask& mask);
    public: Technology_BasicLayers(const Technology_BasicLayers& basicLayers);

// Operators
// *********

    public: Technology_BasicLayers& operator=(const Technology_BasicLayers& basicLayers);

// Accessors
// *********

    public: virtual Collection<BasicLayer*>* getClone() const;
    public: virtual Hurricane::Locator<BasicLayer*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};

// ****************************************************************************************************
// Technology implementation
// ****************************************************************************************************

Technology::Technology(DataBase* dataBase, const Name& name)
// *********************************************************
:    Inherit(),
    _dataBase(dataBase),
    _name(name),
    _layerMap(),
    _layerList()
{
    if (!_dataBase)
        throw Error("Can't create " + _TName("Technology") + " : null data base");

    if (_dataBase->getTechnology())
        throw Error("Can't create " + _TName("Technology") + " : already exists");

    if (_name.IsEmpty())
        throw Error("Can't create " + _TName("Technology") + " : empty name");
}

Technology* Technology::create(DataBase* dataBase, const Name& name)
// *****************************************************************
{
    Technology* technology = new Technology(dataBase, name);

    technology->_postCreate();

    return technology;
}

BasicLayer* Technology::getBasicLayer(const Name& name) const
// **********************************************************
{
    Layer* layer = getLayer(name);
    return (layer && is_a<BasicLayer*>(layer)) ? (BasicLayer*)layer : NULL;
}

CompositeLayer* Technology::getCompositeLayer(const Name& name) const
// ******************************************************************
{
    Layer* layer = getLayer(name);
    return (layer && is_a<CompositeLayer*>(layer)) ? (CompositeLayer*)layer : NULL;
}

BasicLayers Technology::getBasicLayers() const
// *******************************************
{
    // return getLayers().getSubSet<BasicLayer*>();
    return SubTypeCollection<Layer*, BasicLayer*>(getLayers());
}

BasicLayers Technology::getBasicLayers(const Layer::Mask& mask) const
// ******************************************************************
{
    return Technology_BasicLayers(this, mask);
}

CompositeLayers Technology::getCompositeLayers() const
// ***************************************************
{
    // return getLayers().getSubSet<CompositeLayer*>();
    return SubTypeCollection<Layer*, CompositeLayer*>(getLayers());
}

void Technology::SetName(const Name& name)
// ***************************************
{
    if (name != _name) {
        if (name.IsEmpty())
            throw Error("Can't change technology name : empty name");

        _name = name;
    }
}

void Technology::_postCreate()
// ***************************
{
    Inherit::_postCreate();

    _dataBase->_SetTechnology(this);
}

void Technology::_preDestroy()
// **************************
{
    Inherit::_preDestroy();

    for_each_layer(layer, getLayers()) layer->destroy(); end_for;

    _dataBase->_SetTechnology(NULL);
}

string Technology::_getString() const
// **********************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_name));
    return s;
}

Record* Technology::_getRecord() const
// *****************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("DataBase", _dataBase));
        record->Add(getSlot("Name", &_name));
        record->Add(getSlot("Layers", &_layerList));
    }
    return record;
}

// ****************************************************************************************************
// Technology::LayerMap implementation
// ****************************************************************************************************

Technology::LayerMap::LayerMap()
// *****************************
:    Inherit()
{
}

Name Technology::LayerMap::_getKey(Layer* layer) const
// ***************************************************
{
    return layer->getName();
}

unsigned Technology::LayerMap::_getHashValue(Name name) const
// **********************************************************
{
    return ( (unsigned int)( (unsigned long)name._getSharedName() ) ) / 8;
}

Layer* Technology::LayerMap::_getNextElement(Layer* layer) const
// *************************************************************
{
    return layer->_getNextOfTechnologyLayerMap();
}

void Technology::LayerMap::_setNextElement(Layer* layer, Layer* nextLayer) const
// *****************************************************************************
{
    layer->_setNextOfTechnologyLayerMap(nextLayer);
};



// ****************************************************************************************************
// Technology_BasicLayers implementation
// ****************************************************************************************************

Technology_BasicLayers::Technology_BasicLayers()
// *********************************************
:     Inherit(),
    _technology(NULL),
    _mask(0)
{
}

Technology_BasicLayers::Technology_BasicLayers(const Technology* technology, const Layer::Mask& mask)
// **************************************************************************************************
:     Inherit(),
    _technology(technology),
    _mask(mask)
{
}

Technology_BasicLayers::Technology_BasicLayers(const Technology_BasicLayers& basicLayers)
// **************************************************************************************
:     Inherit(),
    _technology(basicLayers._technology),
    _mask(basicLayers._mask)
{
}

Technology_BasicLayers& Technology_BasicLayers::operator=(const Technology_BasicLayers& basicLayers)
// *************************************************************************************************
{
    _technology = basicLayers._technology;
    _mask = basicLayers._mask;
    return *this;
}

Collection<BasicLayer*>* Technology_BasicLayers::getClone() const
// **************************************************************
{
    return new Technology_BasicLayers(*this);
}

Locator<BasicLayer*>* Technology_BasicLayers::getLocator() const
// *************************************************************
{
    return new Locator(_technology, _mask);
}

string Technology_BasicLayers::_getString() const
// **********************************************
{
    string s = "<" + _TName("Technology::BasicLayers");
    if (_technology) {
        s += " " + getString(_technology);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Technology_BasicLayers::Locator implementation
// ****************************************************************************************************

Technology_BasicLayers::Locator::Locator()
// ***************************************
:    Inherit(),
    _technology(NULL),
    _mask(0),
    _basicLayerLocator()
{
}

Technology_BasicLayers::Locator::Locator(const Technology* technology, const Layer::Mask& mask)
// ********************************************************************************************
:    Inherit(),
    _technology(technology),
    _mask(mask),
    _basicLayerLocator()
{
    if (_technology) {
        _basicLayerLocator = _technology->getBasicLayers().getLocator();
        while (_basicLayerLocator.IsValid() && !(_basicLayerLocator.getElement()->getMask() & _mask))
            _basicLayerLocator.Progress();
    }
}

Technology_BasicLayers::Locator::Locator(const Locator& locator)
// *************************************************************
:    Inherit(),
    _technology(locator._technology),
    _mask(locator._mask),
    _basicLayerLocator(locator._basicLayerLocator)
{
}

Technology_BasicLayers::Locator& Technology_BasicLayers::Locator::operator=(const Locator& locator)
// ************************************************************************************************
{
    _technology = locator._technology;
    _mask = locator._mask;
    _basicLayerLocator = locator._basicLayerLocator;
    return *this;
}

BasicLayer* Technology_BasicLayers::Locator::getElement() const
// ************************************************************
{
    return _basicLayerLocator.getElement();
}

Locator<BasicLayer*>* Technology_BasicLayers::Locator::getClone() const
// ********************************************************************
{
    return new Locator(*this);
}

bool Technology_BasicLayers::Locator::IsValid() const
// **************************************************
{
    return _basicLayerLocator.IsValid();
}

void Technology_BasicLayers::Locator::Progress()
// *********************************************
{
    do {
        _basicLayerLocator.Progress();
    }
    while (_basicLayerLocator.IsValid() && !(_basicLayerLocator.getElement()->getMask() & _mask));
}

string Technology_BasicLayers::Locator::_getString() const
// *******************************************************
{
    string s = "<" + _TName("Technology::BasicLayers::Locator");
    if (_technology) {
        s += " " + getString(_technology);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
