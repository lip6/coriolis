// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/KatabaticEngine.h"                 |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_KATABATIC_ENGINE_H
#define  KATABATIC_KATABATIC_ENGINE_H

#include  <iostream>
#include  <string>
#include  <vector>
#include  <set>
#include  "hurricane/Timer.h"
#include  "hurricane/DbU.h"
#include  "hurricane/Torus.h"
#include  "hurricane/Layer.h"
#include  "hurricane/Net.h"

namespace Hurricane {
  class Name;
  class Cell;
  class Instance;
}

#include  "crlcore/ToolEngine.h"

namespace CRL {
  class RoutingGauge;
  class RoutingLayerGauge;
}

#include  "katabatic/Constants.h"
#include  "katabatic/Configuration.h"
#include  "katabatic/GCell.h"
#include  "katabatic/AutoSegments.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/ChipTools.h"


namespace Katabatic {

  using std::ostream;
  using std::string;
  using std::vector;
  using std::set;
  using Hurricane::Timer;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Torus;
  using Hurricane::Net;
  using Hurricane::Nets;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;
  using CRL::ToolEngine;

  class GCellGrid;


// -------------------------------------------------------------------
// Functors.

  struct NetCompareByName {
      inline bool operator() ( const Net* lhs, const Net* rhs ) const;
  };

  inline bool NetCompareByName::operator() ( const Net* lhs, const Net* rhs ) const
  { return lhs->getName() < rhs->getName(); }


// -------------------------------------------------------------------
// Class  :  "KatabaticEngine".

  class KatabaticEngine : public ToolEngine {
    public:
      typedef set<Net*,NetCompareByName>  NetSet;

    public:
    // Constructor.
      static  KatabaticEngine*      create                    ( Cell* );
    // Accessors.                                             
      static  KatabaticEngine*      get                       ( const Cell* );
      static  const Name&           staticGetName             ();
      inline  bool                  isGMetal                  ( const Layer* ) const;
      inline  bool                  isGContact                ( const Layer* ) const;
      inline  bool                  isChip                    () const;
      inline  bool                  isInDemoMode              () const;
      inline  bool                  doWarnOnGCellOverload     () const;
      inline  bool                  doDestroyBaseContact      () const;
      inline  bool                  doDestroyBaseSegment      () const;
      inline  bool                  doDestroyTool             () const;
      virtual const Name&           getName                   () const;
      inline  EngineState           getState                  () const;
      inline  unsigned int          getFlags                  ( unsigned int mask ) const;
      inline  Configuration*        getKatabaticConfiguration ();
      virtual Configuration*        getConfiguration          ();
      inline  CellGauge*            getCellGauge              () const;
      inline  RoutingGauge*         getRoutingGauge           () const;
      inline  RoutingLayerGauge*    getLayerGauge             ( size_t depth ) const;
      inline  const Layer*          getRoutingLayer           ( size_t depth ) const ;
      inline  Layer*                getContactLayer           ( size_t depth ) const ;
      inline  GCellGrid*            getGCellGrid              () const;
      inline  const NetSet&         getRoutingNets            () const;
      inline  DbU::Unit             getGlobalThreshold        () const;
      inline  float                 getSaturateRatio          () const;
      inline  size_t                getSaturateRp             () const;
      inline  DbU::Unit             getExtensionCap           () const;
      inline  const ChipTools&      getChipTools              () const;
              void                  xmlWriteGCellGrid         ( ostream& );
              void                  xmlWriteGCellGrid         ( const string& );
    // Modifiers.                                             
      inline  void                  setState                  ( EngineState state );
      inline  void                  setFlags                  ( unsigned int );
      inline  void                  unsetFlags                ( unsigned int );
      inline  void                  setGlobalThreshold        ( DbU::Unit );
      inline  void                  setSaturateRatio          ( float );
      inline  void                  setSaturateRp             ( size_t );
              void                  startMeasures             ();
              void                  stopMeasures              ();
              void                  printMeasures             ( const string& ) const;
              void                  refresh                   ( unsigned int flags=KbOpenSession );
      virtual void                  createDetailedGrid        ();
              void                  chipPrep                  ();
              void                  makePowerRails            ();
      virtual void                  loadGlobalRouting         ( unsigned int method, NetSet& );
              void                  slackenBorder             ( Box bb, Layer::Mask, unsigned int flags );
              void                  slackenBlockIos           ( Instance* core );
              bool                  moveUpNetTrunk            ( AutoSegment*, set<Net*>& globalNets, GCell::SetIndex& invalidateds );
              void                  moveULeft                 ( AutoSegment*, set<Net*>& globalNets, GCell::SetIndex& invalidateds );
              void                  moveURight                ( AutoSegment*, set<Net*>& globalNets, GCell::SetIndex& invalidateds );
              void                  balanceGlobalDensity      ();
              void                  layerAssign               ( unsigned int method );
              void                  updateNetTopology         ( Net* );
              void                  computeNetConstraints     ( Net* );
              void                  toOptimals                ( Net* );
      virtual void                  finalizeLayout            ();
    // Internal Modifiers.                                    
              void                  _computeNetOptimals       ( Net* );
              void                  _computeNetTerminals      ( Net* );
              bool                  _check                    ( const char* message=NULL ) const;
              void                  _check                    ( Net* ) const;
              void                  _gutKatabatic             ();
              void                  _link                     ( AutoContact* );
              void                  _link                     ( AutoSegment* );
              void                  _unlink                   ( AutoContact* );
              void                  _unlink                   ( AutoSegment* );
              AutoContact*          _lookup                   ( Contact* ) const;
              AutoSegment*          _lookup                   ( Segment* ) const;
              void                  _destroyAutoSegments      ();
              void                  _destroyAutoContacts      ();
              void                  _loadGrByNet              ();
              void                  _loadNetGlobalRouting     ( Net* );
              void                  _alignate                 ( Net* );
              void                  _balanceGlobalDensity     ( unsigned int depth );
              void                  _desaturate               ( unsigned int depth, set<Net*>&, unsigned long& total, unsigned long& globals );
              void                  _layerAssignByLength      ( unsigned long& total, unsigned long& global, set<Net*>& );
              void                  _layerAssignByLength      ( Net*, unsigned long& total, unsigned long& global, set<Net*>& );
              void                  _layerAssignByTrunk       ( unsigned long& total, unsigned long& global, set<Net*>& );
              void                  _layerAssignByTrunk       ( Net*, set<Net*>&, unsigned long& total, unsigned long& global );
              void                  _saveNet                  ( Net* );
              void                  _print                    () const;
              void                  _print                    ( Net* ) const;
      inline  const AutoContactLut& _getAutoContactLut        () const;
      inline  const AutoSegmentLut& _getAutoSegmentLut        () const;
    // Inspector Management.                                 
      virtual Record*               _getRecord                () const;
      virtual string                _getString                () const;
      virtual string                _getTypeName              () const;

    protected:
    // Attributes.
      static  Name            _toolName;
              Timer           _timer;
              EngineState     _state;
              unsigned int    _flags;
              Configuration*  _configuration;
              GCellGrid*      _gcellGrid;
              ChipTools       _chipTools;
              NetSet          _routingNets;
              AutoSegmentLut  _autoSegmentLut;
              AutoContactLut  _autoContactLut;

    protected:
    // Constructors & Destructors.
                               KatabaticEngine ( Cell* );
      virtual                 ~KatabaticEngine ();
      virtual void             _postCreate     ();
      virtual void             _preDestroy     ();
    private:
                               KatabaticEngine ( const KatabaticEngine& );
              KatabaticEngine& operator=       ( const KatabaticEngine& );
  };


// Inline Functions.
  inline bool                           KatabaticEngine::doDestroyBaseContact      () const { return _flags & EngineDestroyBaseContact; }
  inline bool                           KatabaticEngine::doDestroyBaseSegment      () const { return _flags & EngineDestroyBaseSegment; }
  inline bool                           KatabaticEngine::doDestroyTool             () const { return _state >= EngineGutted; }
  inline bool                           KatabaticEngine::doWarnOnGCellOverload     () const { return _flags & EngineWarnOnGCellOverload; }
  inline bool                           KatabaticEngine::isInDemoMode              () const { return _flags & EngineDemoMode; }
  inline Configuration*                 KatabaticEngine::getKatabaticConfiguration () { return _configuration; }
  inline bool                           KatabaticEngine::isGMetal                  ( const Layer* layer ) const { return _configuration->isGMetal(layer); }
  inline bool                           KatabaticEngine::isGContact                ( const Layer* layer ) const { return _configuration->isGContact(layer); }
  inline void                           KatabaticEngine::setFlags                  ( unsigned int flags ) { _flags |=  flags; }
  inline void                           KatabaticEngine::unsetFlags                ( unsigned int flags ) { _flags &= ~flags; }
  inline void                           KatabaticEngine::setSaturateRatio          ( float ratio ) { _configuration->setSaturateRatio(ratio); }
  inline void                           KatabaticEngine::setSaturateRp             ( size_t threshold ) { _configuration->setSaturateRp(threshold); }
  inline void                           KatabaticEngine::setGlobalThreshold        ( DbU::Unit threshold ) { _configuration->setGlobalThreshold(threshold); }
  inline unsigned int                   KatabaticEngine::getFlags                  ( unsigned int mask ) const { return _flags & mask; }
  inline EngineState                    KatabaticEngine::getState                  () const { return _state; }
  inline CellGauge*                     KatabaticEngine::getCellGauge              () const { return _configuration->getCellGauge(); }
  inline RoutingGauge*                  KatabaticEngine::getRoutingGauge           () const { return _configuration->getRoutingGauge(); }
  inline RoutingLayerGauge*             KatabaticEngine::getLayerGauge             ( size_t depth ) const { return _configuration->getLayerGauge(depth); }
  inline const Layer*                   KatabaticEngine::getRoutingLayer           ( size_t depth ) const { return _configuration->getRoutingLayer(depth); }
  inline Layer*                         KatabaticEngine::getContactLayer           ( size_t depth ) const { return _configuration->getContactLayer(depth); }
  inline GCellGrid*                     KatabaticEngine::getGCellGrid              () const { return _gcellGrid; }
  inline const KatabaticEngine::NetSet& KatabaticEngine::getRoutingNets            () const { return _routingNets; }
  inline DbU::Unit                      KatabaticEngine::getGlobalThreshold        () const { return _configuration->getGlobalThreshold(); }
  inline float                          KatabaticEngine::getSaturateRatio          () const { return _configuration->getSaturateRatio(); }
  inline size_t                         KatabaticEngine::getSaturateRp             () const { return _configuration->getSaturateRp(); }
  inline DbU::Unit                      KatabaticEngine::getExtensionCap           () const { return _configuration->getExtensionCap(); }
  inline const AutoContactLut&          KatabaticEngine::_getAutoContactLut        () const { return _autoContactLut; }
  inline const AutoSegmentLut&          KatabaticEngine::_getAutoSegmentLut        () const { return _autoSegmentLut; }
  inline void                           KatabaticEngine::setState                  ( EngineState state ) { _state = state; }
  inline bool                           KatabaticEngine::isChip                    () const { return _chipTools.isChip(); }
  inline const ChipTools&               KatabaticEngine::getChipTools              () const { return _chipTools; }


// -------------------------------------------------------------------
// Global Variables.


  extern const char* missingKTBT;
  extern const char* badMethod;


} // Katabatic namespace.

#endif  // KATABATIC_KATABATIC_ENGINE_H
