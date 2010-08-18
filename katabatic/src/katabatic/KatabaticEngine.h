
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./KatabaticEngine.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KATABATIC_KATABATIC_ENGINE__
#define  __KATABATIC_KATABATIC_ENGINE__

#include  <iostream>
#include  <string>
#include  <vector>
#include  <set>

#include  "hurricane/Timer.h"
#include  "hurricane/DbU.h"
#include  "hurricane/Net.h"

namespace Hurricane {
  class Name;
  class Layer;
  class Cell;
}

#include  "crlcore/ToolEngine.h"

namespace CRL {
  class RoutingGauge;
  class RoutingLayerGauge;
}

#include  "katabatic/Configuration.h"
#include  "katabatic/AutoContacts.h"
#include  "katabatic/AutoSegments.h"


namespace Katabatic {


  using std::ostream;
  using std::string;
  using std::vector;
  using std::set;
  using Hurricane::Timer;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Nets;
  using Hurricane::Cell;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;
  using CRL::ToolEngine;

  class GCellGrid;


// -------------------------------------------------------------------
// Enumerations


  enum LoadGRMethod { LoadGrByNet   = 1
                    , LoadGrByGCell = 2
                    };

  enum LayerAssignMethod { LayerAssignByLength = 1
                         , LayerAssignByTrunk  = 2
                         , NoNetLayerAssign    = 3
                         };

  enum EngineState { StateCreation = 1
                   , StateGlobalLoaded
                   , StateActive
                   , StateDriving
                   , StatePreDestroying
                   , StateGutted
                   };


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
      static  KatabaticEngine*     create                    ( Cell* );
    // Accessors.                                            
      static  KatabaticEngine*     get                       ( const Cell* );
      static  const Name&          staticGetName             ();
      inline  bool                 doDestroyBaseContact      () const;
      inline  bool                 doDestroyBaseSegment      () const;
      inline  bool                 doDestroyTool             () const;
      virtual const Name&          getName                   () const;
      inline  EngineState          getState                  () const;
      inline  bool                 getDemoMode               ();
      inline  bool                 getWarnGCellOverload      ();
      inline  Configuration*       getKatabaticConfiguration ();
      virtual Configuration*       getConfiguration          ();
      inline  RoutingGauge*        getRoutingGauge           () const;
      inline  RoutingLayerGauge*   getLayerGauge             ( size_t depth ) const;
      inline  const Layer*         getRoutingLayer           ( size_t depth ) const ;
      inline  Layer*               getContactLayer           ( size_t depth ) const ;
      inline  GCellGrid*           getGCellGrid              () const;
      inline  const NetSet&        getRoutingNets            () const;
      inline  DbU::Unit            getGlobalThreshold        () const;
      inline  float                getSaturateRatio          () const;
      inline  size_t               getSaturateRp             () const;
      inline  DbU::Unit            getExtensionCap           () const;
              void                 xmlWriteGCellGrid         ( ostream& );
              void                 xmlWriteGCellGrid         ( const string& );
      inline  bool                 isGMetal                  ( const Layer* ) const;
    // Modifiers.                                            
      inline  void                 setState                  ( EngineState state );
      inline  void                 setDemoMode               ( bool );
      inline  bool                 setDestroyBaseContact     ( bool );
      inline  bool                 setDestroyBaseSegment     ( bool );
      inline  void                 setWarnGCellOverload      ( bool );
      inline  void                 setGlobalThreshold        ( DbU::Unit );
      inline  void                 setSaturateRatio          ( float );
      inline  void                 setSaturateRp             ( size_t );
              void                 startMeasures             ();
              void                 stopMeasures              ();
              void                 printMeasures             ( const string& ) const;
              void                 refresh                   ( bool openSession=true );
              void                 makePowerRails            ();
      virtual void                 createDetailedGrid        ();
      virtual void                 loadGlobalRouting         ( unsigned int method, NetSet& );
              void                 layerAssign               ( unsigned int method );
    //        void                 computeNetConstraints     ();
    //        void                 computeNetOptimals        ();
      virtual void                 finalizeLayout            ();
    // Internal Modifiers.                                   
              void                 _computeNetConstraints    ( Net*, set<AutoSegment*>& faileds );
              void                 _computeNetOptimals       ( Net* );
              void                 _computeNetTerminals      ( Net* );
              bool                 _check                    ( const char* message=NULL ) const;
              void                 _check                    ( Net* ) const;
              void                 _gutKatabatic             ();
              void                 _link                     ( AutoContact* );
              void                 _link                     ( AutoSegment* );
              void                 _unlink                   ( AutoContact* );
              void                 _unlink                   ( AutoSegment* );
              AutoContact*         _lookup                   ( Contact* ) const;
              AutoSegment*         _lookup                   ( Segment* ) const;
              void                 _destroyAutoSegments      ();
              void                 _destroyAutoContacts      ();
              void                 _loadGrByNet              ();
              void                 _loadNetGlobalRouting     ( Net* );
              void                 _alignate                 ( Net* );
              void                 _canonize                 ( Net* );
              void                 _desaturate               ( unsigned int depth, set<Net*>&, unsigned long& total, unsigned long& globals );
              void                 _layerAssignByLength      ( unsigned long& total, unsigned long& global, set<Net*>& );
              void                 _layerAssignByLength      ( Net* , unsigned long& total, unsigned long& global, set<Net*>& );
              void                 _layerAssignByTrunk       ( unsigned long& total, unsigned long& global, set<Net*>& );
              void                 _layerAssignByTrunk       ( Net* , unsigned long& total, unsigned long& global, set<Net*>& );
              void                 _splitContactsOfNet       ( Net* );
              void                 _collapseNet              ( const Name& , unsigned int depth=1 );
              void                 _collapseNet              ( Net* , unsigned int depth=1 );
              void                 _collapseNets             ( Nets , unsigned int depth=1 );
              void                 _toOptimals               ( Net*, bool onlyNew=false );
              void                 _saveNet                  ( Net* );
              void                 _print                    () const;
              void                 _print                    ( Net* ) const;
      inline  AutoContactLut&      _getAutoContactLut        ();
      inline  AutoSegmentLut&      _getAutoSegmentLut        ();
    // Inspector Management.                                 
      virtual Record*              _getRecord                () const;
      virtual string               _getString                () const;
      virtual string               _getTypeName              () const;

    protected:
    // Attributes.
      static  Name            _toolName;
              Timer           _timer;
              EngineState     _state;
              bool            _destroyBaseContact;
              bool            _destroyBaseSegment;
              bool            _demoMode;
              bool            _warnGCellOverload;
              Configuration*  _configuration;
              GCellGrid*      _gcellGrid;
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
  inline bool                           KatabaticEngine::doDestroyBaseContact      () const { return _destroyBaseContact; }
  inline bool                           KatabaticEngine::doDestroyBaseSegment      () const { return _destroyBaseSegment; }
  inline bool                           KatabaticEngine::doDestroyTool             () const { return _state >= StateGutted; }
  inline bool                           KatabaticEngine::setDestroyBaseContact     ( bool state ) { bool p=_destroyBaseContact; _destroyBaseContact = state; return p; }
  inline bool                           KatabaticEngine::setDestroyBaseSegment     ( bool state ) { bool p=_destroyBaseSegment; _destroyBaseSegment = state; return p; }
  inline Configuration*                 KatabaticEngine::getKatabaticConfiguration () { return _configuration; }
  inline bool                           KatabaticEngine::isGMetal                  ( const Layer* layer ) const { return _configuration->isGMetal(layer); }
  inline void                           KatabaticEngine::setDemoMode               ( bool  mode ) { _demoMode = mode; }
  inline void                           KatabaticEngine::setWarnGCellOverload      ( bool  mode ) { _warnGCellOverload = mode; }
  inline void                           KatabaticEngine::setSaturateRatio          ( float ratio ) { _configuration->setSaturateRatio(ratio); }
  inline void                           KatabaticEngine::setSaturateRp             ( size_t threshold ) { _configuration->setSaturateRp(threshold); }
  inline void                           KatabaticEngine::setGlobalThreshold        ( DbU::Unit threshold ) { _configuration->setGlobalThreshold(threshold); }
  inline bool                           KatabaticEngine::getDemoMode               () { return _demoMode; }
  inline bool                           KatabaticEngine::getWarnGCellOverload      () { return _warnGCellOverload; }
  inline EngineState                    KatabaticEngine::getState                  () const { return _state; }
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
  inline AutoContactLut&                KatabaticEngine::_getAutoContactLut        () { return _autoContactLut; }
  inline AutoSegmentLut&                KatabaticEngine::_getAutoSegmentLut        () { return _autoSegmentLut; }
  inline void                           KatabaticEngine::setState                  ( EngineState state ) { _state = state; }


// -------------------------------------------------------------------
// Global Variables.


  extern const char* missingKTBT;
  extern const char* badMethod;


} // End of Katabatic namespace.


#endif  // __KATABATIC_KATABATIC_ENGINE__
