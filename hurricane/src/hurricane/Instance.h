// ****************************************************************************************************
// File: Instance.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6

#ifndef HURRICANE_INSTANCE
#define HURRICANE_INSTANCE

#include "Go.h"
#include "Instances.h"
#include "Transformation.h"
#include "Plug.h"
#include "Path.h"
#include "SharedPath.h"
#include "IntrusiveMap.h"

namespace Hurricane {

class Net;
//class View;
class BasicLayer;

// ****************************************************************************************************
// Instance declaration
// ****************************************************************************************************

class Instance : public Go {
// ***********************

// Types
// *****

	public: typedef Go Inherit;

    public: class PlacementStatus {
	// **************************

		public: enum Code {UNPLACED=0, PLACED=1, FIXED=2};

		private: Code _code;

		public: PlacementStatus(const Code& code = UNPLACED);
		public: PlacementStatus(const PlacementStatus& placementstatus);

		public: PlacementStatus& operator=(const PlacementStatus& placementstatus);

		public: operator const Code&() const {return _code;};

		public: const Code& GetCode() const {return _code;};

        public: string _GetTypeName() const { return _TName("Instance::PlacementStatus"); };
		public: string _GetString() const;
		public: Record* _GetRecord() const;

	};

	public: class PlugMap : public IntrusiveMap<const Net*, Plug> {
	// **********************************************************

		public: typedef IntrusiveMap<const Net*, Plug> Inherit;
	
		public: PlugMap();

		public: virtual const Net* _GetKey(Plug* plug) const;
		public: virtual unsigned _GetHashValue(const Net* masterNet) const;
		public: virtual Plug* _GetNextElement(Plug* plug) const;
		public: virtual void _SetNextElement(Plug* plug, Plug* nextPlug) const;

	};

	public: class SharedPathMap : public IntrusiveMap<const SharedPath*, SharedPath> {
	// *****************************************************************************

		public: typedef IntrusiveMap<const SharedPath*, SharedPath> Inherit;
	
		public: SharedPathMap();

		public: virtual const SharedPath* _GetKey(SharedPath* sharedPath) const;
		public: virtual unsigned _GetHashValue(const SharedPath* tailSharedPath) const;
		public: virtual SharedPath* _GetNextElement(SharedPath* sharedPath) const;
		public: virtual void _SetNextElement(SharedPath* sharedPath, SharedPath* nextSharedPath) const;

	};

// Attributes
// **********

	private: Cell* _cell;
	private: Name _name;
	private: Cell* _masterCell;
	private: Transformation _transformation;
    private: PlacementStatus _placementStatus;
	private: PlugMap _plugMap;
	private: SharedPathMap _sharedPathMap;
	private: Instance* _nextOfCellInstanceMap;
	private: Instance* _nextOfCellSlaveInstanceSet;

// Constructors
// ************

	protected: Instance(Cell* cell, const Name& name, Cell* masterCell, const Transformation& transformation, const PlacementStatus& placementstatus, bool secureFlag);

	public: static Instance* Create(Cell* cell, const Name& name, Cell* masterCell, bool secureFlag = true);
	public: static Instance* Create(Cell* cell, const Name& name, Cell* masterCell, const Transformation& transformation, const PlacementStatus& placementstatus, bool secureFlag = true);

// Accessors
// *********

	public: virtual Cell* GetCell() const {return _cell;};
	public: virtual Box GetBoundingBox() const;
	public: const Name& GetName() const {return _name;};
	public: Cell* GetMasterCell() const {return _masterCell;};
	public: const Transformation& GetTransformation() const {return _transformation;};
	public: const PlacementStatus& GetPlacementStatus()  const {return _placementStatus;};
	public: Plug* GetPlug(const Net* masterNet) const {return _plugMap.GetElement(masterNet);};
	public: Plugs GetPlugs() const {return _plugMap.GetElements();};
	public: Plugs GetConnectedPlugs() const;
	public: Plugs GetUnconnectedPlugs() const;
	public: Path GetPath(const Path& tailPath = Path()) const;
	public: Box GetAbutmentBox() const;

// Predicates
// **********

    public: bool IsUnplaced() const {return _placementStatus == PlacementStatus::UNPLACED;};
    public: bool IsPlaced() const {return _placementStatus == PlacementStatus::PLACED;};
	public: bool IsFixed() const {return _placementStatus == PlacementStatus::FIXED;};
	public: bool IsTerminal() const;
	public: bool IsLeaf() const;

// Filters
// *******

	public: static InstanceFilter GetIsUnderFilter(const Box& area);
	public: static InstanceFilter GetIsTerminalFilter();
	public: static InstanceFilter GetIsLeafFilter();
	public: static InstanceFilter GetIsUnplacedFilter();
	public: static InstanceFilter GetIsPlacedFilter();
	public: static InstanceFilter GetIsFixedFilter();
	public: static InstanceFilter GetIsNotUnplacedFilter();

// Updators
// ********

	public: virtual void Materialize();
	public: virtual void Unmaterialize();
	public: virtual void Invalidate(bool propagateFlag = true);
	public: virtual void Translate(const Unit& dx, const Unit& dy);

	public: void SetName(const Name& name);
	public: void SetTransformation(const Transformation& transformation);
	public: void SetPlacementStatus(const PlacementStatus& placementstatus);
	public: void SetMasterCell(Cell* masterCell, bool secureFlag = true);

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("Instance");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: PlugMap& _GetPlugMap() {return _plugMap;};
	public: SharedPath* _GetSharedPath(const SharedPath* tailSharedPath) const {return _sharedPathMap.GetElement(tailSharedPath);}
	public: SharedPathes _GetSharedPathes() const {return _sharedPathMap.GetElements();};
	public: SharedPathMap& _GetSharedPathMap() {return _sharedPathMap;};
	public: Instance* _GetNextOfCellInstanceMap() const {return _nextOfCellInstanceMap;};
	public: Instance* _GetNextOfCellSlaveInstanceSet() const {return _nextOfCellSlaveInstanceSet;};

	public: void _SetNextOfCellInstanceMap(Instance* instance) {_nextOfCellInstanceMap = instance;};
	public: void _SetNextOfCellSlaveInstanceSet(Instance* instance) {_nextOfCellSlaveInstanceSet = instance;};

	//public: void _DrawPhantoms(View* view, const Box& updateArea, const Transformation& transformation);
	//public: void _DrawBoundaries(View* view, const Box& updateArea, const Transformation& transformation);
	//public: void _DrawRubbers(View* view, const Box& updateArea, const Transformation& transformation);
	//public: void _DrawMarkers(View* view, const Box& updateArea, const Transformation& transformation);
        //public: void _DrawDisplaySlots(View* view, const Box& area, const Box& updateArea, const Transformation& transformation);

	//public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
	//public: virtual void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);
	//public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);

};




// -------------------------------------------------------------------
// Class  :  "Proxy...<const Instance::PlacementStatus::Code*>".

template<>
  inline string  ProxyTypeName<Instance::PlacementStatus::Code>
                              ( const Instance::PlacementStatus::Code* object )
                              { return "<PointerSlotAdapter<Instance::PlacementStatus::Code>>"; }

template<>
  inline string  ProxyString  <Instance::PlacementStatus::Code>
                              ( const Instance::PlacementStatus::Code* object )
                              {
                                switch ( *object ) {
                                  case Instance::PlacementStatus::UNPLACED: return "PLACED";
                                  case Instance::PlacementStatus::PLACED:   return "PLACED";
                                  case Instance::PlacementStatus::FIXED:    return "FIXED";
                                }
                                return "ABNORMAL";
                              }

template<>
  inline Record* ProxyRecord  <Instance::PlacementStatus::Code>
                              ( const Instance::PlacementStatus::Code* object )
                              {
                                Record* record = new Record(GetString(object));
                                record->Add(GetSlot("Code", (unsigned int*)object));
                                return record;
                              }


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Instance)

#endif // HURRICANE_INSTANCE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
