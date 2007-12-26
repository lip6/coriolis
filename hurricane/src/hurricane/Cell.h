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

		public: virtual Name _GetKey(Instance* instance) const;
		public: virtual unsigned _GetHashValue(Name name) const;
		public: virtual Instance* _GetNextElement(Instance* instance) const;
		public: virtual void _SetNextElement(Instance* instance, Instance* nextInstance) const;

	};

	public: class SlaveInstanceSet : public IntrusiveSet<Instance> {
	// ***********************************************************

		public: typedef IntrusiveSet<Instance> Inherit;
	
		public: SlaveInstanceSet();

		public: virtual unsigned _GetHashValue(Instance* slaveInstance) const;
		public: virtual Instance* _GetNextElement(Instance* slaveInstance) const;
		public: virtual void _SetNextElement(Instance* slaveInstance, Instance* nextSlaveInstance) const;

	};

	public: class NetMap : public IntrusiveMap<Name, Net> {
	// **************************************************

		public: typedef IntrusiveMap<Name, Net> Inherit;
	
		public: NetMap();

		public: virtual Name _GetKey(Net* net) const;
		public: virtual unsigned _GetHashValue(Name name) const;
		public: virtual Net* _GetNextElement(Net* net) const;
		public: virtual void _SetNextElement(Net* net, Net* nextNet) const;

	};

	class PinMap : public IntrusiveMap<Name, Pin> {
	// *******************************************

		public: typedef IntrusiveMap<Name, Pin> Inherit;

		public: PinMap();

		public: virtual Name _GetKey(Pin* pin) const;
		public: virtual unsigned _GetHashValue(Name name) const;
		public: virtual Pin* _GetNextElement(Pin* pin) const;
		public: virtual void _SetNextElement(Pin* pin, Pin* nextPin) const;

	};

	public: class SliceMap : public IntrusiveMap<const Layer*, Slice> {
	// **************************************************************

		public: typedef IntrusiveMap<const Layer*, Slice> Inherit;
	
		public: SliceMap();

		public: virtual const Layer* _GetKey(Slice* slice) const;
		public: virtual unsigned _GetHashValue(const Layer* layer) const;
		public: virtual Slice* _GetNextElement(Slice* slice) const;
		public: virtual void _SetNextElement(Slice* slice, Slice* nextSlice) const;

	};

	public: class MarkerSet : public IntrusiveSet<Marker> {
	// **************************************************

		public: typedef IntrusiveSet<Marker> Inherit;
	
		public: MarkerSet();

		public: virtual unsigned _GetHashValue(Marker* marker) const;
		public: virtual Marker* _GetNextElement(Marker* marker) const;
		public: virtual void _SetNextElement(Marker* marker, Marker* nextMarker) const;

	};

	//public: class ViewSet : public IntrusiveSet<View> {
	//// **********************************************

	//	public: typedef IntrusiveSet<View> Inherit;
	//
	//	public: ViewSet();

	//	public: virtual unsigned _GetHashValue(View* view) const;
	//	public: virtual View* _GetNextElement(View* view) const;
	//	public: virtual void _SetNextElement(View* view, View* nextView) const;

	//};

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
	//private: ViewSet _viewSet;
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

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("Cell");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: InstanceMap& _GetInstanceMap() {return _instanceMap;};
	public: QuadTree* _GetQuadTree() {return &_quadTree;};
	public: SlaveInstanceSet& _GetSlaveInstanceSet() {return _slaveInstanceSet;};
	public: NetMap& _GetNetMap() {return _netMap;};
	public: PinMap& _GetPinMap() {return _pinMap;};
	public: SliceMap& _GetSliceMap() {return _sliceMap;};
	public: MarkerSet& _GetMarkerSet() {return _markerSet;};
	//public: ViewSet& _GetViewSet() {return _viewSet;};
	public: Cell* _GetNextOfLibraryCellMap() const {return _nextOfLibraryCellMap;};
	public: Cell* _GetNextOfSymbolCellSet() const {return _nextOfSymbolCellSet;};

	public: void _SetNextOfLibraryCellMap(Cell* cell) {_nextOfLibraryCellMap = cell;};
	public: void _SetNextOfSymbolCellSet(Cell* cell) {_nextOfSymbolCellSet = cell;};

	public: void _Fit(const Box& box);
	public: void _Unfit(const Box& box);

    public: void _AddSlaveEntity(Entity* entity, Entity* slaveEntity);
    public: void _RemoveSlaveEntity(Entity* entity, Entity* slaveEntity);
    public: void _GetSlaveEntities(SlaveEntityMap::iterator& begin, SlaveEntityMap::iterator& end);
    public: void _GetSlaveEntities(Entity* entity, SlaveEntityMap::iterator& begin, SlaveEntityMap::iterator& end);

	//public: bool _IsDrawable(View* view) const;
	//public: bool _ContentIsDrawable(View* view) const;
	//public: void _DrawPhantoms(View* view, const Box& updateArea, const Transformation& transformation);
	//public: void _DrawBoundaries(View* view, const Box& updateArea, const Transformation& transformation);
	//public: void _DrawContent(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);
	//public: void _DrawRubbers(View* view, const Box& updateArea, const Transformation& transformation);
	//public: void _DrawMarkers(View* view, const Box& updateArea, const Transformation& transformation);
        //public: void _DrawDisplaySlots(View* view,  const Box& area, const Box& updateArea, const Transformation& transformation);

# endif

// Constructors
// ************

	public: static Cell* Create(Library* library, const Name& name);

// Accessors
// *********

	public: virtual Cell* GetCell() const {return (Cell*)this;};
	public: virtual Box GetBoundingBox() const;
	public: Library* GetLibrary() const {return _library;};
	public: const Name& GetName() const {return _name;};
	public: Instance* GetInstance(const Name& name) const {return _instanceMap.GetElement(name);};
	public: Instances GetInstances() const {return _instanceMap.GetElements();};
	public: Instances GetPlacedInstances() const;
	public: Instances GetFixedInstances() const;
	public: Instances GetUnplacedInstances() const;
	public: Instances GetNotUnplacedInstances() const;
	public: Instances GetInstancesUnder(const Box& area) const;
	public: Instances GetPlacedInstancesUnder(const Box& area) const;
	public: Instances GetFixedInstancesUnder(const Box& area) const;
	public: Instances GetUnplacedInstancesUnder(const Box& area) const;
	public: Instances GetNotUnplacedInstancesUnder(const Box& area) const;
	public: Instances GetSlaveInstances() const; // {return _slaveInstanceSet.GetElements();}; NOON!!
	public: Instances GetTerminalInstances() const;
	public: Instances GetTerminalInstancesUnder(const Box& area) const;
	public: Instances GetNonTerminalInstances() const;
	public: Instances GetNonTerminalInstancesUnder(const Box& area) const;
	public: Instances GetLeafInstances() const;
	public: Instances GetLeafInstancesUnder(const Box& area) const;
	public: Instances GetNonLeafInstances() const;
	public: Instances GetNonLeafInstancesUnder(const Box& area) const;
	public: Net* GetNet(const Name& name) const {return _netMap.GetElement(name);};
	public: Nets GetNets() const {return _netMap.GetElements();};
	public: Nets GetGlobalNets() const;
	public: Nets GetExternalNets() const;
	public: Nets GetInternalNets() const;
	public: Nets GetClockNets() const;
	public: Nets GetSupplyNets() const;
	public: Nets GetPowerNets() const;
	public: Nets GetGroundNets() const;
	public: Pin* GetPin(const Name& name) const {return _pinMap.GetElement(name);};
	public: Pins GetPins() const {return _pinMap.GetElements();};
	public: Slice* GetSlice(const Layer* layer) const {return _sliceMap.GetElement(layer);};
	public: Slices GetSlices(const Layer::Mask& mask = ~0) const;
//	public: Views GetViews() const {return _viewSet.GetElements();};
//	public: MainViews GetMainViews() const;
//	public: MainViews GetImpactedMainViews() const;
	public: Rubbers GetRubbers() const;
	public: Rubbers GetRubbersUnder(const Box& area) const;
	public: Markers GetMarkers() const {return _markerSet.GetElements();};
	public: Markers GetMarkersUnder(const Box& area) const;
    public: References GetReferences() const;
	public: Components GetComponents(const Layer::Mask& mask = ~0) const;
	public: Components GetComponentsUnder(const Box& area, const Layer::Mask& mask = ~0) const;
	public: Occurrences GetOccurrences(unsigned searchDepth = (unsigned)-1) const;
	public: Occurrences GetOccurrencesUnder(const Box& area, unsigned searchDepth = (unsigned)-1) const;
	public: Occurrences GetTerminalInstanceOccurrences() const;
	public: Occurrences GetTerminalInstanceOccurrencesUnder(const Box& area) const;
	public: Occurrences GetLeafInstanceOccurrences() const;
	public: Occurrences GetLeafInstanceOccurrencesUnder(const Box& area) const;
	public: Occurrences GetComponentOccurrences(const Layer::Mask& mask = ~0) const;
	public: Occurrences GetComponentOccurrencesUnder(const Box& area, const Layer::Mask& mask = ~0) const;
    public: Occurrences GetHyperNetRootNetOccurrences() const;
    public: Cells GetSubCells() const;
    public: Pathes GetRecursiveSlavePathes() const;
	public: const Box& GetAbutmentBox() const {return _abutmentBox;};

// Predicates
// **********

	public: bool IsCalledBy(Cell* cell) const;
	public: bool IsTerminal() const {return _isTerminal;};
	public: bool IsFlattenLeaf() const {return _isFlattenLeaf;};
	public: bool IsLeaf() const;
	public: bool IsPad() const {return _isPad;};

// Updators
// ********

	public: void SetName(const Name& name);
	public: void SetAbutmentBox(const Box& abutmentBox);
	public: void SetTerminal(bool isTerminal) {_isTerminal = isTerminal;};
	public: void SetFlattenLeaf(bool isFlattenLeaf) {_isFlattenLeaf = isFlattenLeaf;};
	public: void SetPad(bool isPad) {_isPad = isPad;};
	public: void FlattenNets(bool buildRings=true);
	public: void Materialize();
	public: void Unmaterialize();

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Cell)

#endif // HURRICANE_CELL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
