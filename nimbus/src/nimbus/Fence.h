//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#ifndef __NIMBUS_FENCE_H__
#define __NIMBUS_FENCE_H__

#include "hurricane/IntrusiveSet.h"
#include "hurricane/ExtensionGo.h"
#include "crlcore/RoutingGauge.h"
#include "nimbus/Fences.h"
#include "nimbus/GCells.h"
#include "nimbus/Splitter.h"
#include "nimbus/Grid.h"


namespace Nimbus {

  using namespace CRL;

  class FrontLine;

typedef set<Splitter*> SplitterSet;
typedef set<Component*> ComponentSet;

class Fence : public ExtensionGo {

    public:
        typedef ExtensionGo Inherit;

        class SubFenceSet : public IntrusiveSet<Fence> {
            public:
                typedef IntrusiveSet<Fence> Inherit;
                SubFenceSet();
                virtual unsigned _getHashValue(Fence* fence) const;
                virtual Fence* _getNextElement(Fence* fence) const;
                virtual void _setNextElement(Fence* fence, Fence* nextFence) const;
        };

    protected:
        static const Name _extensionName;

    private:
        SplitterSet _splitters;
        ComponentSet _components;

        unsigned _occupancy;
        unsigned _capacity;

        FrontLine* _frontLine;

        Fence* _nextOfFenceSubFenceSet;
        Fence* _nextOfGCellSubFenceSet;
        Fence* _nextOfGridFenceSet;

        bool _isARoutingFence;

        bool _visible;

    protected:
        unsigned _step;
        DbU::Unit _size;
        Grid* _grid;
        SubFenceSet _subFences;

        Fence (Grid* grid, DbU::Unit size = 0, unsigned capacity = 0, unsigned occupancy = 0, bool visible = false);
        ~Fence();

    public:
        void destroy();
        virtual void _preDestroy();
        virtual void _postCreate();

        static  const Name& staticGetName  () { return _extensionName; };
        virtual const Name& getName        () const { return _extensionName; };
        
        void setStep(unsigned step) { _step = step; };
        unsigned getStep()      { return _step; };
        unsigned getCapacity()  { return _capacity; };
        unsigned getOccupancy() { return _occupancy; };
        DbU::Unit getSize()          { return _size; };
        DbU::Unit getLength()        { return _size; };
        DbU::Unit getDistance() const;
        Grid* getGrid()     { return _grid; };
        NimbusEngine* getNimbus () { return _grid->getNimbus(); }
        Cell* getCell() const;

        virtual GCell* getGCell1()     const { return NULL; };
        virtual GCell* getGCell2()     const { return NULL; };
        virtual GCell* getRightGCell() const { return NULL; };
        virtual GCell* getLeftGCell()  const { return NULL; };
        virtual GCell* getUpGCell()    const { return NULL; };
        virtual GCell* getDownGCell()  const { return NULL; };

        Fence* getParentFence() const;
	FrontLine* getFrontLine();
        void setFrontLine(FrontLine* frontLine);

        GCell* getOppositeGCell (GCell* gcell) const;

        GCells getGCells() const;
        Fences getSubFences() const { return _subFences.getElements(); };
        void addSubFence (Fence* fence);
        void removeSubFence (Fence* fence);

        Fence* _getNextOfFenceSubFenceSet() const {return _nextOfFenceSubFenceSet;};
        void _setNextOfFenceSubFenceSet(Fence* fence) {_nextOfFenceSubFenceSet = fence;};

        Fence* _getNextOfGCellSubFenceSet() const {return _nextOfGCellSubFenceSet;};
        void _setNextOfGCellSubFenceSet(Fence* fence) {_nextOfGCellSubFenceSet = fence;};

        Fence* _getNextOfGridFenceSet() const {return _nextOfGridFenceSet;};
        void _setNextOfGridFenceSet(Fence* fence) {_nextOfGridFenceSet = fence;};

        void setAsRoutingFence();
        bool isARoutingFence() const { return _isARoutingFence; };
        void _NotRoutingFence() { _isARoutingFence = false; };

        void setSize (DbU::Unit& size) { _size = size; };
        void setCapacity (unsigned capacity)   { _capacity = capacity; };
        void setOccupancy (unsigned occupancy) { _occupancy = occupancy; };
        virtual void computeCapacity () = 0;

        void attachSplitter (Splitter* splitter);
        void detachSplitter (Splitter* splitter);
        void attachComponent (Component* component);
        void detachComponent (Component* component);
        Splitter* getOffendingSplitter() { return NULL; };
        Splitter* getNetSplitter(Net* net) const;
        Splitters getSplitters() const;
        
        bool isOverloaded() { return _occupancy > _capacity; };

        virtual DbU::Unit getX()    const { return 0; };
        virtual DbU::Unit getY()    const { return 0; };
        virtual DbU::Unit getXMin() const { return getX(); };
        virtual DbU::Unit getXMax() const { return getX(); };
        virtual DbU::Unit getYMin() const { return getY(); };
        virtual DbU::Unit getYMax() const { return getY(); };
        virtual Point getP1()  const { return Point (getX(), getY()); };
        virtual Point getP2()  const { return Point (getX(), getY()); };
        virtual DbU::Unit getXCenter() const { return 0;};
        virtual DbU::Unit getYCenter() const { return 0;};
        virtual Point getCenter() const { return Point (getXCenter(), getYCenter());};
        virtual Box getBoundingBox() const;

        virtual bool isHorizontal() = 0;
        virtual bool isVertical()   = 0;

        virtual void _moveTo (DbU::Unit target);
        virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy) {};

        bool isVisible() const  { return _visible; };
        void setVisible()       { _visible = true; };
        void setInvisible()     { _visible = false; };
        void ToggleVisibility() { _visible = not _visible; };


        virtual Record* _getRecord() const;
        virtual string  _getString() const;
        virtual string _getTypeName() const {return _TName("Fence");};
};

}


INSPECTOR_P_SUPPORT(Nimbus::Fence);


#endif
