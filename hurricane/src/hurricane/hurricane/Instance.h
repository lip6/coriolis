// ****************************************************************************************************
// File: Instance.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_INSTANCE
#define HURRICANE_INSTANCE

#include "hurricane/Go.h"
#include "hurricane/Plug.h"
#include "hurricane/SharedPath.h"
#include "hurricane/IntrusiveMap.h"

namespace Hurricane {

class Net;
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

        public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("Instance::PlacementStatus"); };
        public: string _getString() const;
        public: Record* _getRecord() const;

    };

    public: class PlugMap : public IntrusiveMap<const Net*, Plug> {
    // **********************************************************

        public: typedef IntrusiveMap<const Net*, Plug> Inherit;
    
        public: PlugMap();

        public: virtual const Net* _getKey(Plug* plug) const;
        public: virtual unsigned _getHashValue(const Net* masterNet) const;
        public: virtual Plug* _getNextElement(Plug* plug) const;
        public: virtual void _setNextElement(Plug* plug, Plug* nextPlug) const;

    };

    public: class SharedPathMap : public IntrusiveMap<const SharedPath*, SharedPath> {
    // *****************************************************************************

        public: typedef IntrusiveMap<const SharedPath*, SharedPath> Inherit;
    
        public: SharedPathMap();

        public: virtual const SharedPath* _getKey(SharedPath* sharedPath) const;
        public: virtual unsigned _getHashValue(const SharedPath* tailSharedPath) const;
        public: virtual SharedPath* _getNextElement(SharedPath* sharedPath) const;
        public: virtual void _setNextElement(SharedPath* sharedPath, SharedPath* nextSharedPath) const;

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

    public: static Instance* create(Cell* cell, const Name& name, Cell* masterCell, bool secureFlag = true);
    public: static Instance* create(Cell* cell, const Name& name, Cell* masterCell, const Transformation& transformation, const PlacementStatus& placementstatus, bool secureFlag = true);

// Accessors
// *********

    public: virtual Cell* getCell() const {return _cell;};
    public: virtual Box getBoundingBox() const;
    public: const Name& getName() const {return _name;};
    public: Cell* getMasterCell() const {return _masterCell;};
    public: const Transformation& getTransformation() const {return _transformation;};
    public: const PlacementStatus& getPlacementStatus()  const {return _placementStatus;};
    public: Plug* getPlug(const Net* masterNet) const {return _plugMap.getElement(masterNet);};
    public: Plugs getPlugs() const {return _plugMap.getElements();};
    public: Plugs getConnectedPlugs() const;
    public: Plugs getUnconnectedPlugs() const;
    public: Path getPath(const Path& tailPath = Path()) const;
    public: Box getAbutmentBox() const;

// Predicates
// **********

    public: bool isUnplaced() const {return _placementStatus == PlacementStatus::UNPLACED;};
    public: bool isPlaced() const {return _placementStatus == PlacementStatus::PLACED;};
    public: bool isFixed() const {return _placementStatus == PlacementStatus::FIXED;};
    public: bool isTerminal() const;
    public: bool isLeaf() const;

// Filters
// *******

    public: static InstanceFilter getIsUnderFilter(const Box& area);
    public: static InstanceFilter getIsTerminalFilter();
    public: static InstanceFilter getIsLeafFilter();
    public: static InstanceFilter getIsUnplacedFilter();
    public: static InstanceFilter getIsPlacedFilter();
    public: static InstanceFilter getIsFixedFilter();
    public: static InstanceFilter getIsNotUnplacedFilter();

// Updators
// ********

    public: virtual void materialize();
    public: virtual void unmaterialize();
    public: virtual void invalidate(bool propagateFlag = true);
    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);

    public: void setName(const Name& name);
    public: void setTransformation(const Transformation& transformation);
    public: void setPlacementStatus(const PlacementStatus& placementstatus);
    public: void setMasterCell(Cell* masterCell, bool secureFlag = true);

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("Instance");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: PlugMap& _getPlugMap() {return _plugMap;};
    public: SharedPath* _getSharedPath(const SharedPath* tailSharedPath) const {return _sharedPathMap.getElement(tailSharedPath);}
    public: SharedPathes _getSharedPathes() const {return _sharedPathMap.getElements();};
    public: SharedPathMap& _getSharedPathMap() {return _sharedPathMap;};
    public: Instance* _getNextOfCellInstanceMap() const {return _nextOfCellInstanceMap;};
    public: Instance* _getNextOfCellSlaveInstanceSet() const {return _nextOfCellSlaveInstanceSet;};

    public: void _setNextOfCellInstanceMap(Instance* instance) {_nextOfCellInstanceMap = instance;};
    public: void _setNextOfCellSlaveInstanceSet(Instance* instance) {_nextOfCellSlaveInstanceSet = instance;};

};


} // End of Hurricane namespace.


// -------------------------------------------------------------------
// Inspector Support for  : Instance::PlacementStatus::Code*".

template<>
inline std::string  getString<const Hurricane::Instance::PlacementStatus::Code*>
                             ( const Hurricane::Instance::PlacementStatus::Code* object )
                             {
                               switch ( *object ) {
                                 case Hurricane::Instance::PlacementStatus::UNPLACED: return "PLACED";
                                 case Hurricane::Instance::PlacementStatus::PLACED:   return "PLACED";
                                 case Hurricane::Instance::PlacementStatus::FIXED:    return "FIXED";
                               }
                               return "ABNORMAL";
                             }

template<>
inline Hurricane::Record* getRecord<const Hurricane::Instance::PlacementStatus::Code*>
                                   ( const Hurricane::Instance::PlacementStatus::Code* object )
                                   {
                                     Hurricane::Record* record = new Hurricane::Record(getString(object));
                                     record->add(getSlot("Code", (unsigned int*)object));
                                     return record;
                                   }


INSPECTOR_P_SUPPORT(Hurricane::Instance);
INSPECTOR_P_SUPPORT(Hurricane::Instance::PlacementStatus);
INSPECTOR_P_SUPPORT(Hurricane::Instance::PlugMap);
INSPECTOR_P_SUPPORT(Hurricane::Instance::SharedPathMap);
IOSTREAM_POINTER_SUPPORT(Hurricane::Instance::PlacementStatus::Code);
IOSTREAM_VALUE_SUPPORT(Hurricane::Instance::PlacementStatus::Code);


#endif // HURRICANE_INSTANCE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
