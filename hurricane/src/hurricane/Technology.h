// ****************************************************************************************************
// File: Technology.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_TECHNOLOGY
#define HURRICANE_TECHNOLOGY

#include "DBo.h"
#include "Layer.h"
#include "BasicLayers.h"
#include "CompositeLayers.h"
#include "IntrusiveMap.h"

namespace Hurricane {

class DataBase;
class BasicLayer;
class CompositeLayer;



// ****************************************************************************************************
// Technology declaration
// ****************************************************************************************************

class Technology : public DBo {
// **************************

// Types
// *****

    public: typedef DBo Inherit;

    public: class LayerMap : public IntrusiveMap<Name, Layer> {
    // ******************************************************

        public: typedef IntrusiveMap<Name, Layer> Inherit;
    
        public: LayerMap();

        public: virtual Name _getKey(Layer* layer) const;
        public: virtual unsigned _getHashValue(Name name) const;
        public: virtual Layer* _getNextElement(Layer* layer) const;
        public: virtual void _setNextElement(Layer* layer, Layer* nextLayer) const;

    };

    public: typedef list<Layer*> LayerList;

// Attributes
// **********

    private: DataBase* _dataBase;
    private: Name _name;
    private: LayerMap _layerMap;
    private: LayerList _layerList;

// Constructors
// ************

    protected: Technology(DataBase* dataBase, const Name& name);

    public: static Technology* create(DataBase* dataBase, const Name& name);

// Accessors
// *********

    public: DataBase* getDataBase() const {return _dataBase;};
    public: const Name& getName() const {return _name;};
    public: Layer* getLayer(const Name& name) const {return _layerMap.getElement(name);};
    public: BasicLayer* getBasicLayer(const Name& name) const;
    public: CompositeLayer* getCompositeLayer(const Name& name) const;
    public: Layers getLayers() const {return getCollection(_layerList);};
    public: BasicLayers getBasicLayers() const;
    public: BasicLayers getBasicLayers(const Layer::Mask& mask) const;
    public: CompositeLayers getCompositeLayers() const;

// Updators
// ********

    public: void setName(const Name& name);

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("Technology");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: LayerMap& _getLayerMap() {return _layerMap;};
    public: LayerList& _getLayerList() {return _layerList;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Technology)

#endif // HURRICANE_TECHNOLOGY

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
