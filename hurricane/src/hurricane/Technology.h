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

		public: virtual Name _GetKey(Layer* layer) const;
		public: virtual unsigned _GetHashValue(Name name) const;
		public: virtual Layer* _GetNextElement(Layer* layer) const;
		public: virtual void _SetNextElement(Layer* layer, Layer* nextLayer) const;

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

	public: static Technology* Create(DataBase* dataBase, const Name& name);

// Accessors
// *********

	public: DataBase* GetDataBase() const {return _dataBase;};
	public: const Name& GetName() const {return _name;};
	public: Layer* GetLayer(const Name& name) const {return _layerMap.GetElement(name);};
	public: BasicLayer* GetBasicLayer(const Name& name) const;
	public: CompositeLayer* GetCompositeLayer(const Name& name) const;
	public: Layers GetLayers() const {return GetCollection(_layerList);};
	public: BasicLayers GetBasicLayers() const;
	public: BasicLayers GetBasicLayers(const Layer::Mask& mask) const;
	public: CompositeLayers GetCompositeLayers() const;

// Updators
// ********

	public: void SetName(const Name& name);

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("Technology");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: LayerMap& _GetLayerMap() {return _layerMap;};
	public: LayerList& _GetLayerList() {return _layerList;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Technology)

#endif // HURRICANE_TECHNOLOGY

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
