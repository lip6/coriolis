//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>
//
// Authors-Tag 


#ifndef __NIMBUS_GCELL_H__
#define __NIMBUS_GCELL_H__

#include "nimbus/GCells.h"
#include "nimbus/Fence.h"

namespace Nimbus {

  class VFence;
  class HFence;
  class NimbusEngine;


struct _gcellmapcmp
{
    bool operator()(const DbU::Unit u1, const DbU::Unit u2) const
    {
        return u1 < u2;
    }
};

typedef map<DbU::Unit, GCell*, _gcellmapcmp> RBMap;
typedef map<DbU::Unit, RBMap*, _gcellmapcmp> RBPavement;
typedef pair <GCell*, GCell*> RBPair;
typedef list <GCell*> RBList;


class GCell : public DBo {

    public: 
        typedef DBo Inherit;

        class GCellSet : public IntrusiveSet<GCell> {
            public:
                typedef IntrusiveSet<GCell> Inherit;
                GCellSet();
                virtual unsigned _getHashValue(GCell* gcell) const;
                virtual GCell* _getNextElement(GCell* gcell) const; 
                virtual void _setNextElement(GCell* gcell, GCell* nextGCell) const;
        };

        class SubFenceSet : public IntrusiveSet<Fence> {
            public:
                typedef IntrusiveSet<Fence> Inherit;
                SubFenceSet();
                virtual unsigned _getHashValue(Fence* fence) const;
                virtual Fence* _getNextElement(Fence* fence) const;
                virtual void _setNextElement(Fence* fence, Fence* nextFence) const;
        };
                

    private:
        NimbusEngine* _nimbus;
        Box _box;
        unsigned _step;
        Interval _Xinterval;
        Interval _Yinterval;
        GCell* _container;
        GCell* _nextOfGCellGCellSet;
        GCellSet _subGCells;
        SubFenceSet _subFences;
        RBPavement _pavement;

        //SubFenceSet _surroundingFences;
        HFence* _upfence;
        HFence* _downfence;
        VFence* _leftfence;
        VFence* _rightfence;

        bool _isAPlacementLeaf;
        bool _isARoutingLeaf;

        //map<H::Net*, set<Component*>> _componentNetMap;
        // getComponents(Net* net); // setComponent(Net* net, Component* component)

    public:

  /*!
   *  \fn  GCell::GCell ( unsigned step, const Box& box, GCell* container = NULL )
   *  \param nimbus    proxy to nimbus
   *  \param step      the hierarchy level of the routbox
   *  \param box       hurricane box of this routbox
   *  \param container the parent container of the box
   *  creates a new routbox at level step, dimensions and position specified by box,
   *  under container (no consistency check)
   */
    private:
        GCell (NimbusEngine* nimbus, unsigned step, const Box& box, GCell* container = NULL);
    public:
        static GCell* create(NimbusEngine* nimbus, unsigned step, const Box& box, GCell* container = NULL);
        
    protected: virtual void _preDestroy();

    public: 
        Cell* getCell () const;
        /* Box methods */
        DbU::Unit getXMin() const { return _box.getXMin(); };
        DbU::Unit getYMin() const { return _box.getYMin(); };
        DbU::Unit getXMax() const { return _box.getXMax(); };
        DbU::Unit getYMax() const { return _box.getYMax(); };
        
        DbU::Unit getXCenter() const { return _box.getXCenter(); };
        DbU::Unit getYCenter() const { return _box.getYCenter(); };
        Point getCenter() const { return _box.getCenter(); };
        
        DbU::Unit getWidth() const { return _box.getWidth(); };
	DbU::Unit getHalfWidth() const {return _box.getHalfWidth();};
        DbU::Unit getHeight() const { return _box.getHeight(); };
	DbU::Unit getHalfHeight() const {return _box.getHalfHeight();};
        
	Box getUnion(const Box& box) const { return _box.getUnion(box); };
        
	Box getIntersection(const Box& box) const { return _box.getIntersection(box); };
	DbU::Unit manhattanDistance(const Point& pt) const { return _box.manhattanDistance(pt); };
	DbU::Unit manhattanDistance(const Box& box) const { return _box.manhattanDistance(box); };
                
        bool isEmpty() const { return _box.isEmpty(); };
	bool isFlat() const { return _box.isFlat(); };
	bool isPonctual() const { return _box.isPonctual(); };

	bool contains(const DbU::Unit& x, const DbU::Unit& y) const { return _box.contains(x,y); };
        bool contains(const Point& point) const { return _box.contains(point); };
	bool contains(const Box& box) const { return _box.contains(box); };
        
	bool intersect(const Box& box) const { return _box.intersect(box); };

	bool isConstrainedBy(const Box& box) const { return _box.isConstrainedBy(box); };

        const Box& getBox() const { return _box; };
        
        unsigned getStep() const    { return _step; };
        unsigned getDepth() const   { return _step; };
        GCell* getContainer() const { return _container; };
        bool isASubGCellOf(GCell* gcell) const;
        bool isAStrictSubGCellOf(GCell* gcell) const;
        void makeSub ();
  /*!
   *  \fn  bool GCell::hasSubGCells ()
   *  The name is quite straightforward, isn't it ?
   */

        GCell* _getNextOfGCellGCellSet() const {return _nextOfGCellGCellSet;};
        void _setNextOfGCellGCellSet(GCell* gcell) {_nextOfGCellGCellSet = gcell;};
        
        bool hasSubGCells ()      const;
        bool horizontalIsCrossed (DbU::Unit& Y) const;
        bool verticalIsCrossed (DbU::Unit& X) const;
        bool strictContains(const Point& point) const;
        bool strictContains(const DbU::Unit& x, const DbU::Unit& y) const;

        void subHSplit (DbU::Unit& Y);
        void subVSplit (DbU::Unit& X);
        RBPair hSplitMe (DbU::Unit& coord);
        RBPair vSplitMe (DbU::Unit& coord);

        GCells getSubGCells () const { return _subGCells.getElements(); };
        Fences getSurroundingFences () const;// { return _surroundingFences.getElements(); };
        Fences getSubFences() const { return _subFences.getElements(); };
        Fences getInnerFences() const { return _subFences.getElements(); };
        Fence* getOppositeFence(const Fence* fence) const;
        //void addSurroundingFence (Fence* fence) { _surroundingFences._Insert(fence); };
        void addSubGCell (GCell* gcell);
        void removeSubGCell (GCell* gcell);
        void addInnerFence (Fence* fence);

        GCell* getPlacementLeaf();
        bool isAPlacementLeaf() const { return _isAPlacementLeaf; };
        void setAsPlacementLeaf();
        void _setNotPlacementLeaf() { _isAPlacementLeaf = false; };
        void setSubGCellsAsPlacementLeaves();
        void recFlushSubPlacementLeaves(GCell* gcell);
        GCell* getRoutingLeaf();
        bool isARoutingLeaf() const { return _isARoutingLeaf; };
        void setAsRoutingLeaf();
        void _setNotRoutingLeaf() { _isARoutingLeaf = false; };
        void setSubGCellsAsRoutingLeaves();
        void recFlushSubRoutingLeaves(GCell* gcell);

        double testMargin();
        
        // Resizing
    public:
        void setXMin (DbU::Unit x);
        void setYMin (DbU::Unit y);
        void setXMax (DbU::Unit x);
        void setYMax (DbU::Unit y);

            // Pavement
    private:
        void addToPavement (GCell* gcell);
        void removeFromPavement (GCell* gcell);
    public:
        void dumpPavement () const;

  /*!
   *  \fn  GCell* GCell::getRightOf ()
   *  \fn  GCell* GCell::getLeftOf ()
   *  \fn  GCell* GCell::getDownOf ()
   *  \fn  GCell* GCell::getUpOf ()
   *  \fn  GCell* GCell::getUpperRight ()
   *  \fn  GCell* GCell::getUpperLeft ()
   *  \fn  GCell* GCell::getBottomRight ()
   *  \fn  GCell* GCell::getBottomLeft ()
   *  Navigate through the subroutboxes
   */
    public:
        void setUpFence (HFence* hfence)    { _upfence    = hfence; };
        void setDownFence (HFence* hfence)  { _downfence  = hfence; };
        void setLeftFence (VFence* vfence)  { _leftfence  = vfence; };
        void setRightFence (VFence* vfence) { _rightfence = vfence; };

        HFence* getUpFence()    const { return _upfence; };
        HFence* getDownFence()  const { return _downfence; };
        VFence* getLeftFence()  const { return _leftfence; };
        VFence* getRightFence() const { return _rightfence; };

        GCell* getRightOfMe () const;
        GCell* getLeftOfMe ()  const;
        GCell* getDownOfMe ()  const;
        GCell* getUpOfMe ()    const;

        GCell* computeRightOfMe () const;
        GCell* computeLeftOfMe ()  const;
        GCell* computeDownOfMe ()  const;
        GCell* computeUpOfMe ()    const;

        GCell* getSubRightOf (GCell* gcell) const;
        GCell* getSubLeftOf (GCell* gcell)  const;
        GCell* getSubDownOf (GCell* gcell)  const;
        GCell* getSubUpOf (GCell* gcell)    const;

        GCell* _computeSubRightOf (const GCell* gcell) const;
        GCell* _computeSubLeftOf (const GCell* gcell)  const;
        GCell* _computeSubDownOf (const GCell* gcell)  const;
        GCell* _computeSubUpOf (const GCell* gcell)    const;

        GCell* getSubUpperRight ()  const;
        GCell* getSubUpperLeft ()   const;
        GCell* getSubLowerRight ()  const { return getSubBottomRight(); };
        GCell* getSubBottomRight () const;
        GCell* getSubLowerLeft ()   const { return getSubBottomLeft(); };
        GCell* getSubBottomLeft ()  const;

        GCell* _recGetRightOfMe () const;
        GCell* _recGetLeftOfMe () const;
        GCell* _recGetDownOfMe () const;
        GCell* _recGetUpOfMe () const;

        virtual string _getTypeName() const {return _TName("Nimbus::GCell");}
        virtual string _getString() const;
        virtual Record* _getRecord() const;
}; // class GCell
} // namespace Nimbus


INSPECTOR_P_SUPPORT(Nimbus::GCell);


#endif
