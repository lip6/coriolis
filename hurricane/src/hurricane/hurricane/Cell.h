// ****************************************************************************************************
// File: ./hurricane/Cell.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#ifndef HURRICANE_CELL_H
#define HURRICANE_CELL_H

#include <limits>
#include "hurricane/Observer.h"
#include "hurricane/Relation.h"
#include "hurricane/Pathes.h"
#include "hurricane/Entity.h"
#include "hurricane/Cells.h"
#include "hurricane/DeepNet.h"
#include "hurricane/Instance.h"
#include "hurricane/Pin.h"
#include "hurricane/Pins.h"
#include "hurricane/Slices.h"
#include "hurricane/ExtensionSlice.h"
#include "hurricane/Rubbers.h"
#include "hurricane/Markers.h"
#include "hurricane/Marker.h"
#include "hurricane/Reference.h"
#include "hurricane/Components.h"
#include "hurricane/Occurrences.h"
#include "hurricane/Transformation.h"
#include "hurricane/Layer.h"
#include "hurricane/QuadTree.h"
//#include "hurricane/IntrusiveMap.h"
#include "hurricane/IntrusiveSet.h"
#include "hurricane/MapCollection.h"
#include "hurricane/NetAlias.h"



namespace Hurricane {

class Library;
class BasicLayer;


typedef  multimap<Entity*,Entity*>  SlaveEntityMap;



// ****************************************************************************************************
// Cell declaration
// ****************************************************************************************************

class Cell : public Entity {
// *************************

// Types
// *****

    public: enum Flag { BuildRings              = 0x0001
                      , BuildClockRings         = 0x0002
                      , BuildSupplyRings        = 0x0004
                      , NoClockFlatten          = 0x0008
                      , WarnOnUnplacedInstances = 0x0010
                      // Flags set for Observers.
                      , CellAboutToChange       = 0x0001
                      , CellChanged             = 0x0002
                      // Cell states
                      , FlattenedNets           = 0x0001
                      , Placed                  = 0x0002
                      , Routed                  = 0x0004
                      };
    public: typedef Entity Inherit;
    public: typedef map<Name,ExtensionSlice*> ExtensionSliceMap;

    class UniquifyRelation : public Relation {
      public:
        static  UniquifyRelation*  create           ( Cell* );
        static  UniquifyRelation*  get              ( const Cell* );
        virtual Name               getName          () const;
        static  Name               staticGetName    ();
                Name               getUniqueName    ();
        virtual string             _getTypeName     () const;
        virtual Record*            _getRecord       () const;
      private:
        static const Name          _name;
                     unsigned int  _duplicates;
      private:
                                   UniquifyRelation ( Cell* );
      protected:
        virtual void               _preDestroy      ();
    };

    class ClonedSet : public Collection<Cell*> {
      public:
      // Sub-Class: Locator.
        class Locator : public Hurricane::Locator<Cell*> {
          public:
                                               Locator    ( const Cell* );
            inline                             Locator    ( const Locator& );
            virtual Cell*                      getElement () const;
            virtual Hurricane::Locator<Cell*>* getClone   () const;
            virtual bool                       isValid    () const;
            virtual void                       progress   ();
            virtual string                     _getString () const;
          protected:
            Hurricane::Locator<DBo*>* _dboLocator;
        };
  
      public:
        inline                                ClonedSet  ( const Cell* cell );
        inline                                ClonedSet  ( const ClonedSet& );
        virtual Hurricane::Collection<Cell*>* getClone   () const;
        virtual Hurricane::Locator<Cell*>*    getLocator () const;
        virtual string                        _getString () const;
      protected:
        const Cell* _cell;
    };

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
    private: ExtensionSliceMap _extensionSlices;
    private: MarkerSet _markerSet;
    private: Box _abutmentBox;
    private: Box _boundingBox;
    private: bool _isTerminal;
    private: bool _isFlattenLeaf;
    private: bool _isPad;
    private: Cell* _nextOfLibraryCellMap;
    private: Cell* _nextOfSymbolCellSet;
    private: SlaveEntityMap _slaveEntityMap;
    private: AliasNameSet _netAliasSet;
    private: Observable _observers;
    private: unsigned int _flags;

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
    public: ExtensionSliceMap& _getExtensionSliceMap() {return _extensionSlices;};
    public: MarkerSet& _getMarkerSet() {return _markerSet;};
    public: Cell* _getNextOfLibraryCellMap() const {return _nextOfLibraryCellMap;};
    public: Cell* _getNextOfSymbolCellSet() const {return _nextOfSymbolCellSet;};
    public: AliasNameSet& _getNetAliasSet() { return _netAliasSet; }

    public: void _setNextOfLibraryCellMap(Cell* cell) {_nextOfLibraryCellMap = cell;};
    public: void _setNextOfSymbolCellSet(Cell* cell) {_nextOfSymbolCellSet = cell;};

    public: void _addNetAlias(NetAliasName* alias) { _netAliasSet.insert(alias); }
    public: void _removeNetAlias(NetAliasName* alias) { _netAliasSet.erase(alias); }

    public: void _fit(const Box& box);
    public: void _unfit(const Box& box);

    public: void _addSlaveEntity(Entity* entity, Entity* slaveEntity);
    public: void _removeSlaveEntity(Entity* entity, Entity* slaveEntity);
    public: void _getSlaveEntities(SlaveEntityMap::iterator& begin, SlaveEntityMap::iterator& end);
    public: void _getSlaveEntities(Entity* entity, SlaveEntityMap::iterator& begin, SlaveEntityMap::iterator& end);
    public: void _insertSlice ( ExtensionSlice* );
    public: void _removeSlice ( ExtensionSlice* );

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
    public: Net* getNet(const Name& name) const;
    public: DeepNet* getDeepNet( Path, const Net* ) const;
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
    public: const ExtensionSliceMap& getExtensionSliceMap() const { return _extensionSlices; };
    public: ExtensionSlice* getExtensionSlice(const Name& name) const;
    public: ExtensionSlices getExtensionSlices(ExtensionSlice::Mask mask=~0) const;
    public: Rubbers getRubbers() const;
    public: Rubbers getRubbersUnder(const Box& area) const;
    public: Markers getMarkers() const {return _markerSet.getElements();};
    public: Markers getMarkersUnder(const Box& area) const;
    public: References getReferences() const;
    public: Components getComponents(const Layer::Mask& mask = ~0) const;
    public: Components getComponentsUnder(const Box& area, const Layer::Mask& mask = ~0) const;
    public: Occurrences getOccurrences(unsigned searchDepth = std::numeric_limits<unsigned int>::max()) const;
    public: Occurrences getOccurrencesUnder(const Box& area, unsigned searchDepth = std::numeric_limits<unsigned int>::max()) const;
    public: Occurrences getTerminalInstanceOccurrences() const;
    public: Occurrences getTerminalInstanceOccurrencesUnder(const Box& area) const;
    public: Occurrences getLeafInstanceOccurrences() const;
    public: Occurrences getLeafInstanceOccurrencesUnder(const Box& area) const;
    public: Occurrences getNonLeafInstanceOccurrences() const;
    public: Occurrences getComponentOccurrences(const Layer::Mask& mask = ~0) const;
    public: Occurrences getComponentOccurrencesUnder(const Box& area, const Layer::Mask& mask = ~0) const;
    public: Occurrences getHyperNetRootNetOccurrences() const;
    public: ExtensionSlice::Mask getExtensionSliceMask ( const Name& name ) const;
    public: Gos getExtensionGos ( const Name& name ) const;
    public: Gos getExtensionGos ( ExtensionSlice::Mask mask = ~0 ) const;
    public: Gos getExtensionGosUnder ( const Box& area, const Name& name ) const;
    public: Gos getExtensionGosUnder ( const Box& area, ExtensionSlice::Mask mask = ~0 ) const;
    public: Cells getSubCells() const;
    public: Cells getClonedCells() const;
    public: Cell* getCloneMaster() const;
    public: Pathes getRecursiveSlavePathes() const;
    public: const Box& getAbutmentBox() const {return _abutmentBox;};

// Predicates
// **********

    public: bool isCalledBy(Cell* cell) const;
    public: bool isTerminal() const {return _isTerminal;};
    public: bool isFlattenLeaf() const {return _isFlattenLeaf;};
    public: bool isLeaf() const;
    public: bool isPad() const {return _isPad;};
    public: bool isFlattenedNets() const {return _flags & FlattenedNets;};
    public: bool isPlaced() const {return _flags & Placed;};
    public: bool isRouted() const {return _flags & Routed;};
    public: bool isNetAlias(const Name& name) const;

// Updators
// ********

    public: void setName(const Name& name);
    public: void setAbutmentBox(const Box& abutmentBox);
    public: void setTerminal(bool isTerminal) {_isTerminal = isTerminal;};
    public: void setFlattenLeaf(bool isFlattenLeaf) {_isFlattenLeaf = isFlattenLeaf;};
    public: void setPad(bool isPad) {_isPad = isPad;};
    public: void flattenNets(unsigned int flags=BuildRings);
    public: void setFlags(unsigned int flags) { _flags |= flags; }
    public: void resetFlags(unsigned int flags) { _flags &= ~flags; }
    public: void materialize();
    public: void unmaterialize();
    public: Cell* getClone();
    public: void uniquify(unsigned int depth=std::numeric_limits<unsigned int>::max());
    public: void addObserver(BaseObserver*);
    public: void removeObserver(BaseObserver*);
    public: void notify(unsigned flags);

};


inline  Cell::ClonedSet::Locator::Locator ( const Locator& other )
  : Hurricane::Locator<Cell*>()
  , _dboLocator(other._dboLocator)
{ }

inline  Cell::ClonedSet::ClonedSet ( const Cell* cell )
  : Hurricane::Collection<Cell*>()
  , _cell(cell)
{ }

inline  Cell::ClonedSet::ClonedSet ( const ClonedSet& other )
  : Hurricane::Collection<Cell*>()
  , _cell(other._cell)
{ }


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Cell);
INSPECTOR_P_SUPPORT(Hurricane::Cell::InstanceMap);
INSPECTOR_P_SUPPORT(Hurricane::Cell::SlaveInstanceSet);
INSPECTOR_P_SUPPORT(Hurricane::Cell::NetMap);
INSPECTOR_P_SUPPORT(Hurricane::Cell::PinMap);
INSPECTOR_P_SUPPORT(Hurricane::Cell::SliceMap);
INSPECTOR_P_SUPPORT(Hurricane::Cell::MarkerSet);


#endif // HURRICANE_CELL_H


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
