// ****************************************************************************************************
// File: Cell.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_CELL
#define HURRICANE_CELL

#include "Pathes.h"
#include "Entity.h"
#include "Cells.h"
#include "Instance.h"
#include "DeepNet.h"
#include "Pin.h"
#include "Pins.h"
#include "Slices.h"
#include "Rubbers.h"
#include "Markers.h"
#include "Marker.h"
#include "Reference.h"
#include "Components.h"
#include "Occurrences.h"
#include "Transformation.h"
#include "Layer.h"
#include "QuadTree.h"
#include "IntrusiveMap.h"
#include "IntrusiveSet.h"


namespace Hurricane {

class Library;
class BasicLayer;


typedef  multimap<Entity*,Entity*>  SlaveEntityMap;



// ****************************************************************************************************
// Cell declaration
// ****************************************************************************************************

class Cell : public Entity {
// ***********************

#   if !defined(__DOXYGEN_PROCESSOR__)

// Types
// *****

    public: typedef Entity Inherit;

    class InstanceMap : public IntrusiveMap<Name, Instance> {
    // ****************************************************

        public: typedef IntrusiveMap<Name, Instance> Inherit;

        public: InstanceMap();

        public: virtual Name _getKey(Instance* instance) const;
        public: virtual unsigned _getHashValue(Name name) const;
        public: virtual Instance* _getNextElement(Instance* instance) const;
        public: virtual void _setNextElement(Instance* instance, Instance* nextInstance) const;

    };

    public: class SlaveInstanceSet : public IntrusiveSet<Instance> {
    // ***********************************************************

        public: typedef IntrusiveSet<Instance> Inherit;
    
        public: SlaveInstanceSet();

        public: virtual unsigned _getHashValue(Instance* slaveInstance) const;
        public: virtual Instance* _getNextElement(Instance* slaveInstance) const;
        public: virtual void _setNextElement(Instance* slaveInstance, Instance* nextSlaveInstance) const;

    };

    public: class NetMap : public IntrusiveMap<Name, Net> {
    // **************************************************

        public: typedef IntrusiveMap<Name, Net> Inherit;
    
        public: NetMap();

        public: virtual Name _getKey(Net* net) const;
        public: virtual unsigned _getHashValue(Name name) const;
        public: virtual Net* _getNextElement(Net* net) const;
        public: virtual void _setNextElement(Net* net, Net* nextNet) const;

    };

    class PinMap : public IntrusiveMap<Name, Pin> {
    // *******************************************

        public: typedef IntrusiveMap<Name, Pin> Inherit;

        public: PinMap();

        public: virtual Name _getKey(Pin* pin) const;
        public: virtual unsigned _getHashValue(Name name) const;
        public: virtual Pin* _getNextElement(Pin* pin) const;
        public: virtual void _setNextElement(Pin* pin, Pin* nextPin) const;

    };

    public: class SliceMap : public IntrusiveMap<const Layer*, Slice> {
    // **************************************************************

        public: typedef IntrusiveMap<const Layer*, Slice> Inherit;
    
        public: SliceMap();

        public: virtual const Layer* _getKey(Slice* slice) const;
        public: virtual unsigned _getHashValue(const Layer* layer) const;
        public: virtual Slice* _getNextElement(Slice* slice) const;
        public: virtual void _setNextElement(Slice* slice, Slice* nextSlice) const;

    };

    public: class MarkerSet : public IntrusiveSet<Marker> {
    // **************************************************

        public: typedef IntrusiveSet<Marker> Inherit;
    
        public: MarkerSet();

        public: virtual unsigned _getHashValue(Marker* marker) const;
        public: virtual Marker* _getNextElement(Marker* marker) const;
        public: virtual void _setNextElement(Marker* marker, Marker* nextMarker) const;

    };

// Attributes
// **********

    private: Library* _library;
    private: Name _name;
    private: InstanceMap _instanceMap;
    private: QuadTree _quadTree;
    private: SlaveInstanceSet _slaveInstanceSet;
    private: NetMap _netMap;
    private: PinMap _pinMap;
    private: SliceMap _sliceMap;
    private: MarkerSet _markerSet;
    private: Box _abutmentBox;
    private: Box _boundingBox;
    private: bool _isTerminal;
    private: bool _isFlattenLeaf;
    private: bool _isPad;
    private: Cell* _nextOfLibraryCellMap;
    private: Cell* _nextOfSymbolCellSet;
    private: multimap<Entity*,Entity*> _slaveEntityMap;

// Constructors
// ************

    protected: Cell(Library* library, const Name& name);

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("Cell");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: InstanceMap& _getInstanceMap() {return _instanceMap;};
    public: QuadTree* _getQuadTree() {return &_quadTree;};
    public: SlaveInstanceSet& _getSlaveInstanceSet() {return _slaveInstanceSet;};
    public: NetMap& _getNetMap() {return _netMap;};
    public: PinMap& _getPinMap() {return _pinMap;};
    public: SliceMap& _getSliceMap() {return _sliceMap;};
    public: MarkerSet& _getMarkerSet() {return _markerSet;};
    public: Cell* _getNextOfLibraryCellMap() const {return _nextOfLibraryCellMap;};
    public: Cell* _getNextOfSymbolCellSet() const {return _nextOfSymbolCellSet;};

    public: void _setNextOfLibraryCellMap(Cell* cell) {_nextOfLibraryCellMap = cell;};
    public: void _setNextOfSymbolCellSet(Cell* cell) {_nextOfSymbolCellSet = cell;};

    public: void _fit(const Box& box);
    public: void _unfit(const Box& box);

    public: void _addSlaveEntity(Entity* entity, Entity* slaveEntity);
    public: void _removeSlaveEntity(Entity* entity, Entity* slaveEntity);
    public: void _getSlaveEntities(SlaveEntityMap::iterator& begin, SlaveEntityMap::iterator& end);
    public: void _getSlaveEntities(Entity* entity, SlaveEntityMap::iterator& begin, SlaveEntityMap::iterator& end);

#endif

// Constructors
// ************

    public: static Cell* create(Library* library, const Name& name);

// Accessors
// *********

    public: virtual Cell* getCell() const {return (Cell*)this;};
    public: virtual Box getBoundingBox() const;
    public: Library* getLibrary() const {return _library;};
    public: const Name& getName() const {return _name;};
    public: Instance* getInstance(const Name& name) const {return _instanceMap.getElement(name);};
    public: Instances getInstances() const {return _instanceMap.getElements();};
    public: Instances getPlacedInstances() const;
    public: Instances getFixedInstances() const;
    public: Instances getUnplacedInstances() const;
    public: Instances getNotUnplacedInstances() const;
    public: Instances getInstancesUnder(const Box& area) const;
    public: Instances getPlacedInstancesUnder(const Box& area) const;
    public: Instances getFixedInstancesUnder(const Box& area) const;
    public: Instances getUnplacedInstancesUnder(const Box& area) const;
    public: Instances getNotUnplacedInstancesUnder(const Box& area) const;
    public: Instances getSlaveInstances() const; // {return _slaveInstanceSet.getElements();}; NOON!!
    public: Instances getTerminalInstances() const;
    public: Instances getTerminalInstancesUnder(const Box& area) const;
    public: Instances getNonTerminalInstances() const;
    public: Instances getNonTerminalInstancesUnder(const Box& area) const;
    public: Instances getLeafInstances() const;
    public: Instances getLeafInstancesUnder(const Box& area) const;
    public: Instances getNonLeafInstances() const;
    public: Instances getNonLeafInstancesUnder(const Box& area) const;
    public: Net* getNet(const Name& name) const {return _netMap.getElement(name);};
    public: Nets getNets() const {return _netMap.getElements();};
    public: Nets getGlobalNets() const;
    public: Nets getExternalNets() const;
    public: Nets getInternalNets() const;
    public: Nets getClockNets() const;
    public: Nets getSupplyNets() const;
    public: Nets getPowerNets() const;
    public: Nets getGroundNets() const;
    public: Pin* getPin(const Name& name) const {return _pinMap.getElement(name);};
    public: Pins getPins() const {return _pinMap.getElements();};
    public: Slice* getSlice(const Layer* layer) const {return _sliceMap.getElement(layer);};
    public: Slices getSlices(const Layer::Mask& mask = ~0) const;
    public: Rubbers getRubbers() const;
    public: Rubbers getRubbersUnder(const Box& area) const;
    public: Markers getMarkers() const {return _markerSet.getElements();};
    public: Markers getMarkersUnder(const Box& area) const;
    public: References getReferences() const;
    public: Components getComponents(const Layer::Mask& mask = ~0) const;
    public: Components getComponentsUnder(const Box& area, const Layer::Mask& mask = ~0) const;
    public: Occurrences getOccurrences(unsigned searchDepth = (unsigned)-1) const;
    public: Occurrences getOccurrencesUnder(const Box& area, unsigned searchDepth = (unsigned)-1) const;
    public: Occurrences getTerminalInstanceOccurrences() const;
    public: Occurrences getTerminalInstanceOccurrencesUnder(const Box& area) const;
    public: Occurrences getLeafInstanceOccurrences() const;
    public: Occurrences getLeafInstanceOccurrencesUnder(const Box& area) const;
    public: Occurrences getComponentOccurrences(const Layer::Mask& mask = ~0) const;
    public: Occurrences getComponentOccurrencesUnder(const Box& area, const Layer::Mask& mask = ~0) const;
    public: Occurrences getHyperNetRootNetOccurrences() const;
    public: Cells getSubCells() const;
    public: Pathes getRecursiveSlavePathes() const;
    public: const Box& getAbutmentBox() const {return _abutmentBox;};

// Predicates
// **********

    public: bool isCalledBy(Cell* cell) const;
    public: bool isTerminal() const {return _isTerminal;};
    public: bool isFlattenLeaf() const {return _isFlattenLeaf;};
    public: bool isLeaf() const;
    public: bool isPad() const {return _isPad;};

// Updators
// ********

    public: void setName(const Name& name);
    public: void setAbutmentBox(const Box& abutmentBox);
    public: void setTerminal(bool isTerminal) {_isTerminal = isTerminal;};
    public: void setFlattenLeaf(bool isFlattenLeaf) {_isFlattenLeaf = isFlattenLeaf;};
    public: void setPad(bool isPad) {_isPad = isPad;};
    public: void flattenNets(bool buildRings=true);
    public: void materialize();
    public: void unmaterialize();

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Cell)

#endif // HURRICANE_CELL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
