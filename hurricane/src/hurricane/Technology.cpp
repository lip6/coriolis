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

        public: virtual BasicLayer* GetElement() const;
        public: virtual Hurricane::Locator<BasicLayer*>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

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

    public: virtual Collection<BasicLayer*>* GetClone() const;
    public: virtual Hurricane::Locator<BasicLayer*>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

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

    if (_dataBase->GetTechnology())
        throw Error("Can't create " + _TName("Technology") + " : already exists");

    if (_name.IsEmpty())
        throw Error("Can't create " + _TName("Technology") + " : empty name");
}

Technology* Technology::Create(DataBase* dataBase, const Name& name)
// *****************************************************************
{
    Technology* technology = new Technology(dataBase, name);

    technology->_PostCreate();

    return technology;
}

BasicLayer* Technology::GetBasicLayer(const Name& name) const
// **********************************************************
{
    Layer* layer = GetLayer(name);
    return (layer && is_a<BasicLayer*>(layer)) ? (BasicLayer*)layer : NULL;
}

CompositeLayer* Technology::GetCompositeLayer(const Name& name) const
// ******************************************************************
{
    Layer* layer = GetLayer(name);
    return (layer && is_a<CompositeLayer*>(layer)) ? (CompositeLayer*)layer : NULL;
}

BasicLayers Technology::GetBasicLayers() const
// *******************************************
{
    // return GetLayers().GetSubSet<BasicLayer*>();
    return SubTypeCollection<Layer*, BasicLayer*>(GetLayers());
}

BasicLayers Technology::GetBasicLayers(const Layer::Mask& mask) const
// ******************************************************************
{
    return Technology_BasicLayers(this, mask);
}

CompositeLayers Technology::GetCompositeLayers() const
// ***************************************************
{
    // return GetLayers().GetSubSet<CompositeLayer*>();
    return SubTypeCollection<Layer*, CompositeLayer*>(GetLayers());
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

void Technology::_PostCreate()
// ***************************
{
    Inherit::_PostCreate();

    _dataBase->_SetTechnology(this);
}

void Technology::_PreDelete()
// **************************
{
    Inherit::_PreDelete();

    for_each_layer(layer, GetLayers()) layer->Delete(); end_for;

    _dataBase->_SetTechnology(NULL);
}

string Technology::_GetString() const
// **********************************
{
    string s = Inherit::_GetString();
    s.insert(s.length() - 1, " " + GetString(_name));
    return s;
}

Record* Technology::_GetRecord() const
// *****************************
{
    Record* record = Inherit::_GetRecord();
    if (record) {
        record->Add(GetSlot("DataBase", _dataBase));
        record->Add(GetSlot("Name", &_name));
        record->Add(GetSlot("Layers", &_layerList));
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

Name Technology::LayerMap::_GetKey(Layer* layer) const
// ***************************************************
{
    return layer->getName();
}

unsigned Technology::LayerMap::_GetHashValue(Name name) const
// **********************************************************
{
    return ( (unsigned int)( (unsigned long)name._GetSharedName() ) ) / 8;
}

Layer* Technology::LayerMap::_GetNextElement(Layer* layer) const
// *************************************************************
{
    return layer->_getNextOfTechnologyLayerMap();
}

void Technology::LayerMap::_SetNextElement(Layer* layer, Layer* nextLayer) const
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

Collection<BasicLayer*>* Technology_BasicLayers::GetClone() const
// **************************************************************
{
    return new Technology_BasicLayers(*this);
}

Locator<BasicLayer*>* Technology_BasicLayers::GetLocator() const
// *************************************************************
{
    return new Locator(_technology, _mask);
}

string Technology_BasicLayers::_GetString() const
// **********************************************
{
    string s = "<" + _TName("Technology::BasicLayers");
    if (_technology) {
        s += " " + GetString(_technology);
        s += " " + GetString(_mask);
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
        _basicLayerLocator = _technology->GetBasicLayers().GetLocator();
        while (_basicLayerLocator.IsValid() && !(_basicLayerLocator.GetElement()->getMask() & _mask))
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

BasicLayer* Technology_BasicLayers::Locator::GetElement() const
// ************************************************************
{
    return _basicLayerLocator.GetElement();
}

Locator<BasicLayer*>* Technology_BasicLayers::Locator::GetClone() const
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
    while (_basicLayerLocator.IsValid() && !(_basicLayerLocator.GetElement()->getMask() & _mask));
}

string Technology_BasicLayers::Locator::_GetString() const
// *******************************************************
{
    string s = "<" + _TName("Technology::BasicLayers::Locator");
    if (_technology) {
        s += " " + GetString(_technology);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
