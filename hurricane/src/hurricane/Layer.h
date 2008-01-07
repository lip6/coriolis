// ****************************************************************************************************
// File: Layer.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_LAYER
#define HURRICANE_LAYER

#include "DBo.h"
#include "Layers.h"
#include "Unit.h"
#include "BasicLayers.h"

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

	public: Technology* GetTechnology() const {return _technology;};
	public: const Name& GetName() const {return _name;};
	public: const Mask& GetMask() const {return _mask;};
	public: const Mask& GetExtractMask() const {return _extractMask;};
	public: const Unit& GetMinimalSize() const {return _minimalSize;};
	public: const Unit& GetMinimalSpacing() const {return _minimalSpacing;};
    public: Unit GetPitch() const {return (_pitch==0?(_minimalSize + _minimalSpacing):_pitch);};
	public: virtual BasicLayers GetBasicLayers() const = 0;

// Predicates
// **********

	public: bool Contains(const Layer* layer) const;
	public: bool Intersect(const Layer* layer) const;

// Updators
// ********

	public: void SetName(const Name& name);
	public: void SetMinimalSize(const Unit& minimalSize);
	public: void SetMinimalSpacing(const Unit& minimalSpacing);
	public: void SetPitch(const Unit& pitch);

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: virtual BasicLayer* _GetSymbolicBasicLayer() = 0;
	public: Layer* _GetNextOfTechnologyLayerMap() const {return _nextOfTechnologyLayerMap;};

	public: void _SetMask(const Mask& mask) {_mask = mask;};
	public: void _SetExtractMask(const Mask& extractMask) {_extractMask = extractMask;};
	public: void _SetNextOfTechnologyLayerMap(Layer* layer) {_nextOfTechnologyLayerMap = layer;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Layer)

#endif // HURRICANE_LAYER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
