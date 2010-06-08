//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>
//
// Authors-Tag 


#ifndef __NIMBUS_NIMBUS_H__
#define __NIMBUS_NIMBUS_H__

#include "hurricane/Layers.h"
#include "crlcore/ToolEngine.h"
#include "nimbus/Configuration.h"
#include "nimbus/GCell.h"
#include "nimbus/GCells.h"
#include "nimbus/Grid.h"
#include "nimbus/Fence.h"
#include "nimbus/HFence.h"
#include "nimbus/VFence.h"
#include "nimbus/FrontLine.h"
#include "nimbus/Splitter.h"
#include "nimbus/SplitterContact.h"
#include "nimbus/StepProperty.h"
#include "nimbus/FenceProperty.h"

namespace Hurricane {
  class Library;
}

namespace Nimbus {

using namespace Hurricane;

class NimbusEngine : public ToolEngine {

    typedef ToolEngine Inherit;

// Attributes
// **********
  private:
    static const Name     _toolName;
           Configuration* _configuration;
           unsigned int   _depth;
           unsigned int   _step;
           Grid*          _grid;

           GCellSet       _placementBoxSet;
           GCellSet       _routingBoxSet;
           FenceSet       _routingFenceSet;

           set<Layer*>    _routingLayerSet;


// Constructors
// ************
    private: NimbusEngine (Cell* cell, const Box& workzone);

    public: static NimbusEngine* create (Cell* cell, const Library* library, const Box& workZone=Box());
            
  // Accessors
  // *********
  public:
    virtual const Name&   getName         () const;
    static  const Name&   staticGetName   ();
    static  NimbusEngine* get             ( Cell* );
    inline  bool          doPinsPlacement () const;
    inline  double        getAspectRatio  () const;
    inline  double        getMargin       () const;
    inline  const Box&    getWorkZone     () const;

    public: Grid* getGrid() { return _grid; };
    public: unsigned getDepth() { return _depth; };
    public: unsigned getSteps() { cerr<<"getSteps is deprecated"<<endl; return _depth; };
    public: unsigned getStep()  { return _step; };

    public: GCells getLeaves() const;
    public: GCells getLeaves(unsigned step) const;
    public: Fences getFences(unsigned step);
    public: Layers getRoutingLayers() const;

    public: GCell* getCommonAncester(GCell* gc1, GCell* gc2) const;

    public: GCells getPlacementLeaves() const;
    public: GCells getRoutingLeaves() const;
    public: Fences   getRoutingFences() const;
    public: GCells getColumnGCells ( unsigned indexColumn ) const;
    public: GCells getRowGCells    ( unsigned indexRow )    const;

    public: GCell* getRoutingLeafContaining(Point& p);
    public: GCell* getPlacementLeafContaining(Point& p);
    //private: GCell* getLeafContaining(Point& p, GCellSet& gcset);

    //public: DbU::Unit getStandardCellHeight() const;

    public: bool testMargin ();
    public: bool testMargin ( double margin );


// Modifiers
// *********

    public: void invalidateSplitterContactsOppositeCounts(Net* net);
    public: void invalidateSplitterContactsOppositeCounts();
            
    public: void setPlacementLeaf(GCell* gcell);
    public: void setRoutingLeaf(GCell* gcell);
    public: void setSubGCellsAsPlacementLeaves(GCell* gcell);
    public: void setSubGCellsAsRoutingLeaves(GCell* gcell);
    private: void setLeaf(GCell* gcell, GCellSet& gcset);
    public: void flushPlacementLeaves();
    public: void flushRoutingLeaves();
    public: void addToRoutingFences(Fence* fence);
    public: void flushRoutingFences();
    public: void addToRoutingLayers(Layer* layer);
    public: void _removePlacementLeaf(GCell* gcell);
    public: void _removeRoutingLeaf(GCell* gcell);
    public: void regroup(bool placed = true, bool fixed = false);
    public: void placementLeavesUp();
    public: void placementLeavesDown();

    public: void hideFences();
    public: void showPlacementLeaves();
    public: void showRoutingLeaves();

    // compute the X coordinates which divides a gcell into nbSplits sub-gcells
    private: void computeXSplit (GCell* gcell, DbU::Unit* XSplit, unsigned nbSplits);
    // compute the Y coordinates which divides a gcell into nbSplits sub-gcells
    private: void computeYSplit (GCell* gcell, DbU::Unit* YSplit, unsigned nbSplits);

    // Recalls of Nimbus::Grid functions so we are able to create a specific partionning with Python
    public: void horizontalLongSplit (unsigned step, DbU::Unit& Y) const;
    public: void verticalLongSplit   (unsigned step, DbU::Unit& X) const;

    // To increment Depth
    public: void incrementDepth () { _depth++; };
    public: void createFences ( unsigned depth );

    public: void placePins();
    public: void progress(int nbSplits = 2);
    public: void progress(int nbXSplits, int nbYSplits);
    public: void destroyLevel(unsigned depth);
    private: void recDestroy (unsigned depth, GCell* gcell);

    public: void balance();

    private: void _recInit (unsigned step, GCell* rgc);

    protected: virtual void _postCreate ();
    protected: virtual void _preDestroy ();
    public: virtual string _getTypeName() const {return _TName("NimbusEngine");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
};


  inline  bool          NimbusEngine::doPinsPlacement () const { return _configuration->doPinsPlacement(); }
  inline  double        NimbusEngine::getAspectRatio  () const { return _configuration->getAspectRatio(); }
  inline  double        NimbusEngine::getMargin       () const { return _configuration->getMargin(); }
  inline  const Box&    NimbusEngine::getWorkZone     () const { return _configuration->getWorkZone(); }


} // namespace Nimbus


#endif // __NIMBUS_NIMBUS_H__
