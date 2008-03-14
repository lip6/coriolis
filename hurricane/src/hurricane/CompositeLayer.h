// ****************************************************************************************************
// File: CompositeLayer.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_COMPOSITE_LAYER
#define HURRICANE_COMPOSITE_LAYER

#include "Layer.h"
#include "CompositeLayers.h"

namespace Hurricane {



// ****************************************************************************************************
// CompositeLayer declaration
// ****************************************************************************************************

class CompositeLayer : public Layer {
// ********************************

// Types
// *****

    public: typedef Layer Inherit;

    public: class Type {
    // ***************

        public: enum Code {UNDEFINED=0, METAL=1, VIA=2};

        private: Code _code;

        public: Type(const Code& code = UNDEFINED);
        public: Type(const Type& type);

        public: Type& operator=(const Type& type);

        public: operator const Code&() const {return _code;};

        public: const Code& getCode() const {return _code;};

        public: string _GetTypeName() const { return _TName("CompositeLayer::Type"); };
        public: string _GetString() const;
        public: Record* _GetRecord() const;

    };

    public: typedef list<BasicLayer*> BasicLayerList;

    public: typedef map<const BasicLayer*, Unit> SizeMap;

// Attributes
// **********

    private: Type _type;
    private: BasicLayerList _basicLayerList;
    private: SizeMap _contactSizeMap;
    private: SizeMap _segmentSizeMap;
    private: SizeMap _segmentExtentionMap;
    private: SizeMap _padSizeMap;
    private: Unit _maximalContactSize;
    private: Unit _maximalSegmentSize;
    private: Unit _maximalSegmentExtention;
    private: Unit _maximalPadSize;
    private: BasicLayer* _symbolicBasicLayer;

// Constructors
// ************

    protected: CompositeLayer(Technology* technology, const Name& name, const Type& type, const Unit& minimalSize = 0, const Unit& minimalSpacing = 0);

    public: static CompositeLayer* create(Technology* technology, const Name& name, const Type& type, const Unit& minimalSize = 0, const Unit& minimalSpacing = 0);

// Accessors
// *********

    public: const Type& getType() const {return _type;};
    public: virtual BasicLayers getBasicLayers() const;
    public: Unit getContactSize(const BasicLayer* basicLayer) const;
    public: Unit getSegmentSize(const BasicLayer* basicLayer) const;
    public: Unit getSegmentExtention(const BasicLayer* basicLayer) const;
    public: Unit getPadSize(const BasicLayer* basicLayer) const;
    public: const Unit& getMaximalContactSize() const {return _maximalContactSize;};
    public: const Unit& getMaximalSegmentSize() const {return _maximalSegmentSize;};
    public: const Unit& getMaximalSegmentExtention() const {return _maximalSegmentExtention;};
    public: const Unit& getMaximalPadSize() const {return _maximalPadSize;};

// Updators
// ********

    public: void add(BasicLayer* basicLayer, const Unit& contactSize, const Unit& segmentSize, const Unit& segmentExtention, const Unit& padSize);
    public: void remove(BasicLayer* basicLayer);

// Others
// ******

    public: virtual string _GetTypeName() const {return _TName("CompositeLayer");};
    public: virtual string _GetString() const;
    public: virtual Record* _GetRecord() const;
    public: virtual BasicLayer* _getSymbolicBasicLayer() {return _symbolicBasicLayer;};
    public: BasicLayerList& _getBasicLayerList() {return _basicLayerList;};

    public: void _updateSymbolicBasicLayer(const Layer::Mask& visibleBasicLayersMask);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::CompositeLayer)



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************


bool Scan(const string& s, H::CompositeLayer::Type& type);

#endif // HURRICANE_COMPOSITE_LAYER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
