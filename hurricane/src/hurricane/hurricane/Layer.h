// ****************************************************************************************************
// File: Layer.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_LAYER
#define HURRICANE_LAYER

#include "hurricane/DBo.h"
#include "hurricane/Layers.h"
#include "hurricane/Unit.h"
#include "hurricane/BasicLayers.h"

namespace Hurricane {

class Technology;



// ****************************************************************************************************
// Layer declaration
// ****************************************************************************************************

class Layer : public DBo {
// *********************

// Types
// *****

    public: typedef DBo Inherit;

    public: typedef unsigned long long Mask;

// Attributes
// **********

    private: Technology* _technology;
    private: Name _name;
    private: Mask _mask;
    private: Mask _extractMask;
    private: Unit _minimalSize;
    private: Unit _minimalSpacing;
    private: Unit _pitch;
    private: Layer* _nextOfTechnologyLayerMap;

// Constructors
// ************

    protected: Layer(Technology* technology, const Name& name, const Unit& minimalSize = 0, const Unit& minimalSpacing = 0, const Unit& pitch = 0);

// Accessors
// *********

    public: Technology* getTechnology() const {return _technology;};
    public: const Name& getName() const {return _name;};
    public: const Mask& getMask() const {return _mask;};
    public: const Mask& getExtractMask() const {return _extractMask;};
    public: const Unit& getMinimalSize() const {return _minimalSize;};
    public: const Unit& getMinimalSpacing() const {return _minimalSpacing;};
    public: Unit getPitch() const {return (_pitch==0?(_minimalSize + _minimalSpacing):_pitch);};
    public: virtual BasicLayers getBasicLayers() const = 0;
    public: virtual Layer* getConnectorLayer() const {return NULL;}; 
    public: virtual Layer* getObstructionLayer() const {return NULL;}; 

// Predicates
// **********

    public: bool contains(const Layer* layer) const;
    public: bool intersect(const Layer* layer) const;

// Updators
// ********

    public: void setName(const Name& name);
    public: void setMinimalSize(const Unit& minimalSize);
    public: void setMinimalSpacing(const Unit& minimalSpacing);
    public: void setPitch(const Unit& pitch);

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: virtual BasicLayer* _getSymbolicBasicLayer() = 0;
    public: Layer* _getNextOfTechnologyLayerMap() const {return _nextOfTechnologyLayerMap;};

    public: void _setMask(const Mask& mask) {_mask = mask;};
    public: void _setExtractMask(const Mask& extractMask) {_extractMask = extractMask;};
    public: void _setNextOfTechnologyLayerMap(Layer* layer) {_nextOfTechnologyLayerMap = layer;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Layer)

#endif // HURRICANE_LAYER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
