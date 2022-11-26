// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/AnabaticEngine.h"                   |
// +-----------------------------------------------------------------+


#pragma  once
#include <memory>
#include <string>
#include <vector>
#include <set>
#include "hurricane/NetRoutingProperty.h"
namespace Hurricane {
  class Instance;
  class CellViewer;
}
#include "crlcore/ToolEngine.h"
#include "anabatic/Configuration.h"
#include "anabatic/Matrix.h"
#include "anabatic/GCell.h"
#include "anabatic/AutoContact.h"
#include "anabatic/AutoSegments.h"
#include "anabatic/ChipTools.h"


namespace Anabatic {

  using std::string;
  using std::vector;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::Cell;
  using Hurricane::CellViewer;
  using Hurricane::NetRoutingState;
  using CRL::ToolEngine;

  class NetBuilder;
  class AnabaticEngine;


// -------------------------------------------------------------------
// Class  :  "Anabatic::RawGCellsUnder".

  class RawGCellsUnder {
    public:
      class Element {
        public:
          inline         Element ( GCell*, Edge* );
          inline  GCell* gcell   () const;
          inline  Edge*  edge    () const;
        private:
          GCell* _gcell;
          Edge*  _edge;
      };
    public:
                                    RawGCellsUnder ( const AnabaticEngine*, Segment* );
                                    RawGCellsUnder ( const AnabaticEngine*, Point source, Point target );
                   void             commonCtor     ( const AnabaticEngine*, Point source, Point target );
      inline       bool             empty          () const;
      inline       size_t           size           () const;
      inline       GCell*           gcellAt        ( size_t ) const;
      inline       GCell*           gcellRAt       ( size_t ) const;
      inline       Edge*            edgeAt         ( size_t ) const;
      inline const vector<Element>& getElements    () const;
    private:
                      RawGCellsUnder ( const RawGCellsUnder& );
      RawGCellsUnder& operator=      ( const RawGCellsUnder& );
    private:
      vector<Element>  _elements;
  };


  inline         RawGCellsUnder::Element::Element ( GCell* gcell, Edge* edge ) : _gcell(gcell), _edge(edge) { } 
  inline  GCell* RawGCellsUnder::Element::gcell   () const { return _gcell; }
  inline  Edge*  RawGCellsUnder::Element::edge    () const { return _edge; }

  inline       bool    RawGCellsUnder::empty       () const { return _elements.empty(); }
  inline       size_t  RawGCellsUnder::size        () const { return _elements.size(); }
  inline const vector<RawGCellsUnder::Element>&
                       RawGCellsUnder::getElements () const { return _elements; }
  inline       Edge*   RawGCellsUnder::edgeAt      ( size_t i ) const { return (i<size()) ? _elements[i].edge () : NULL; }
  inline       GCell*  RawGCellsUnder::gcellAt     ( size_t i ) const { return (i<size()) ? _elements[i].gcell() : NULL; }
  inline       GCell*  RawGCellsUnder::gcellRAt    ( size_t i ) const { return (i<size()) ? _elements[size()-1-i].gcell() : NULL; }

  typedef  std::shared_ptr<RawGCellsUnder>  GCellsUnder;


// -------------------------------------------------------------------
// Class  :  "Anabatic::NetData".

  class NetData {
    public:
                                    NetData            ( Net*, AnabaticEngine* );
      inline       bool             isGlobalEstimated  () const;
      inline       bool             isGlobalRouted     () const;
      inline       bool             isGlobalFixed      () const;
      inline       bool             isMixedPreRoute    () const;
      inline       bool             isFixed            () const;
      inline       bool             isExcluded         () const;
      inline       bool             isNoMoveUp         ( Segment* ) const;
      inline       Net*             getNet             () const;
      inline       NetRoutingState* getNetRoutingState () const;
      inline const Box&             getSearchArea      () const;
      inline       DbU::Unit        getHalfPerimeter   () const;
      inline       size_t           getRpCount         () const;
      inline       size_t           getDiodeRpCount    () const;
      inline       DbU::Unit        getSparsity        () const;
      inline       void             setNetRoutingState ( NetRoutingState* );
      inline       void             setSearchArea      ( Box );
      inline       void             setGlobalEstimated ( bool );
      inline       void             setGlobalRouted    ( bool );
      inline       void             setGlobalFixed     ( bool );
      inline       void             setExcluded        ( bool );
      inline       void             setRpCount         ( size_t );
      inline       void             setNoMoveUp        ( Segment* );
    private:                                     
                              NetData            ( const NetData& );
             NetData&         operator=          ( const NetData& );
      inline void             _update            ();
    private:
      Net*                                 _net;
      NetRoutingState*                     _state;
      Box                                  _searchArea;
      size_t                               _rpCount;
      size_t                               _diodeCount;
      DbU::Unit                            _sparsity;
      Flags                                _flags;
      std::set<Segment*,DBo::CompareById>  _noMoveUp;
  };


  inline bool             NetData::isGlobalEstimated  () const { return _flags & Flags::GlobalEstimated; }
  inline bool             NetData::isGlobalRouted     () const { return _flags & Flags::GlobalRouted; }
  inline bool             NetData::isGlobalFixed      () const { return _flags & Flags::GlobalFixed; }
  inline bool             NetData::isMixedPreRoute    () const { return (_state) ? _state->isMixedPreRoute() : false; }
  inline bool             NetData::isFixed            () const { return (_state) ? _state->isFixed        () : false; }
  inline bool             NetData::isExcluded         () const { return _flags & Flags::ExcludeRoute; }
  inline bool             NetData::isNoMoveUp         ( Segment* segment ) const { return (_noMoveUp.find(segment) != _noMoveUp.end()); }
  inline Net*             NetData::getNet             () const { return _net; }
  inline NetRoutingState* NetData::getNetRoutingState () const { return _state; }
  inline const Box&       NetData::getSearchArea      () const { return _searchArea; }
  inline DbU::Unit        NetData::getHalfPerimeter   () const { return (_searchArea.isEmpty()) ? 0.0 : (_searchArea.getWidth()+_searchArea.getHeight()); }
  inline size_t           NetData::getRpCount         () const { return _rpCount; }
  inline size_t           NetData::getDiodeRpCount    () const { return _diodeCount; }
  inline void             NetData::setNetRoutingState ( NetRoutingState* state ) { _state=state; }
  inline DbU::Unit        NetData::getSparsity        () const { return _sparsity; }
  inline void             NetData::setGlobalEstimated ( bool state ) { _flags.set(Flags::GlobalEstimated,state); }
  inline void             NetData::setGlobalRouted    ( bool state ) { _flags.set(Flags::GlobalRouted   ,state); }
  inline void             NetData::setGlobalFixed     ( bool state ) { _flags.set(Flags::GlobalFixed    ,state); }
  inline void             NetData::setExcluded        ( bool state ) { _flags.set(Flags::ExcludeRoute   ,state); }
  inline void             NetData::setRpCount         ( size_t count ) { _rpCount=count; _update(); }
  inline void             NetData::setNoMoveUp        ( Segment* segment ) { _noMoveUp.insert(segment); }


  inline void  NetData::_update ()
  { if (_rpCount) _sparsity=getHalfPerimeter()/_rpCount; else _sparsity=0; }


  class SparsityOrder {
    public:
      inline bool operator() ( const NetData* lhs, const NetData* rhs ) const
      {
        if (lhs->isMixedPreRoute() != rhs->isMixedPreRoute()) return lhs->isMixedPreRoute();
        if ((lhs->getRpCount() > 10) or (rhs->getRpCount() > 10)) {
          if (lhs->getRpCount() != rhs->getRpCount())
            return lhs->getRpCount() > rhs->getRpCount();
        }
        
        if (lhs->getSparsity() != rhs->getSparsity()) return lhs->getSparsity() < rhs->getSparsity();
        return lhs->getNet()->getId() < rhs->getNet()->getId();
      }
  };


// -------------------------------------------------------------------
// Class  :  "Anabatic::AnabaticEngine".

  typedef  std::set<Net*,Entity::CompareById>  NetSet;
  typedef  std::map<uint64_t,NetData*>         NetDatas;


  class AnabaticEngine : public ToolEngine {
    public:
      static const uint32_t  DigitalMode      = (1 <<  0);
      static const uint32_t  AnalogMode       = (1 <<  1);
      static const uint32_t  MixedMode        = (1 <<  2);
      static const uint32_t  AverageHVDensity = 1;  // Average between all densities.
      static const uint32_t  AverageHDensity  = 2;  // Average between all H densities.
      static const uint32_t  AverageVDensity  = 3;  // Average between all V densities.
      static const uint32_t  MaxHVDensity     = 4;  // Maximum between average H and average V.
      static const uint32_t  MaxVDensity      = 5;  // Maximum of V densities.
      static const uint32_t  MaxHDensity      = 6;  // Maximum of H densities.
      static const uint32_t  MaxDensity       = 7;  // Maximum of H & V densities.
    public:
      typedef ToolEngine  Super;
    public:
      static        AnabaticEngine*   create                  ( Cell* );
      static        AnabaticEngine*   get                     ( const Cell* );
      inline        bool              isCanonizeDisabled      () const;
      inline        bool              isDigitalMode           () const;
      inline        bool              isAnalogMode            () const;
      inline        bool              isMixedMode             () const;
      inline        bool              isChannelStyle          () const;
      inline        bool              isHybridStyle           () const;
      static  const Name&             staticGetName           ();
      virtual const Name&             getName                 () const;
      virtual       Configuration*    getConfiguration        ();
      virtual const Configuration*    getConfiguration        () const;
      inline        std::string       getNetBuilderStyle      () const;
      inline        StyleFlags        getRoutingStyle         () const;
      inline        uint64_t          getDensityMode          () const;
      inline        CellViewer*       getViewer               () const;
      inline        void              setViewer               ( CellViewer* );
      inline        EngineState       getState                () const;
      inline  const Matrix*           getMatrix               () const;
      inline  const vector<GCell*>&   getGCells               () const;
      inline  const vector<Edge*>&    getOvEdges              () const;
      inline        GCell*            getSouthWestGCell       () const;
      inline        GCell*            getGCellUnder           ( DbU::Unit x, DbU::Unit y ) const;
      inline        GCell*            getGCellUnder           ( Point ) const;
      inline        GCellsUnder       getGCellsUnder          ( Segment* ) const;
      inline        GCellsUnder       getGCellsUnder          ( Point source, Point target ) const;
      inline        Edges             getEdgesUnderPath       ( GCell* source, GCell* target, Flags pathFlags=Flags::NorthPath ) const;
                    Interval          getUSide                ( Flags direction ) const;
                    int               getCapacity             ( Interval, Flags ) const;
                    size_t            getNetsFromEdge         ( const Edge*, NetSet& );
      virtual       void              openSession             ();
      inline        void              setState                ( EngineState state );
      inline        void              setDensityMode          ( uint64_t );
      inline        void              disableCanonize         ();
      inline        void              enableCanonize          ();
      inline        void              addOv                   ( Edge* );
      inline        void              removeOv                ( Edge* );
      inline const  NetDatas&         getNetDatas             () const;
                    NetData*          getNetData              ( Net*, Flags flags=Flags::NoFlags );
                    void              setupNetDatas           ();
                    void              exclude                 ( const Name& netName );
                    void              exclude                 ( Net* );
                    void              updateMatrix            ();
                    bool              checkPlacement          () const;
    // Dijkstra related functions.                            
      inline        int               getStamp                () const;
      inline        int               incStamp                ();
                    Contact*          breakAt                 ( Segment*, GCell* );
                    void              ripup                   ( Segment*, Flags );
                    bool              unify                   ( Contact* );
    // Global routing related functions.                      
                    void              globalRoute             ();
                    void              cleanupGlobal           ();
                    void              relaxOverConstraineds   ();
    // Detailed routing related functions.                    
      inline        bool              isInDemoMode            () const;
      inline        bool              isChip                  () const;
      inline        bool              doWarnOnGCellOverload   () const;
      inline        bool              doDestroyBaseContact    () const;
      inline        bool              doDestroyBaseSegment    () const;
      inline        bool              doDestroyTool           () const;
      inline        DbU::Unit         getAntennaGateMaxWL     () const;
      inline        DbU::Unit         getAntennaDiodeMaxWL    () const;
      inline        DbU::Unit         getGlobalThreshold      () const;
      inline        float             getSaturateRatio        () const;
      inline        size_t            getSaturateRp           () const;
      inline        DbU::Unit         getExtensionCap         () const;
      inline        Cell*             getDiodeCell            () const;
      inline        Net*              getBlockageNet          () const;
      inline const  ChipTools&        getChipTools            () const;
      inline const  vector<NetData*>& getNetOrdering          () const;
                    void              invalidateRoutingPads   ();
                    void              updateDensity           ();
                    size_t            checkGCellDensities     ();
                    void              setupNetBuilder         ();
      inline        void              setRoutingMode          ( uint32_t );
      inline        void              resetRoutingMode        ( uint32_t );
      inline        void              setGlobalThreshold      ( DbU::Unit );
      inline        void              setSaturateRatio        ( float );
      inline        void              setSaturateRp           ( size_t );
      inline        void              setBlockageNet          ( Net* );
                    void              chipPrep                ();
                    void              computeEdgeCapacities   ( int maxHCap, int maxVCap, int termSatThreshold, int maxTermSat );
                    void              antennaProtect          ( Net*, uint32_t& failed, uint32_t& total );
                    void              antennaProtect          ();
                    void              setupSpecialNets        ();
                    size_t            setupPreRouteds         ();
                    void              loadGlobalRouting       ( uint32_t method );
                    void              computeNetConstraints   ( Net* );
                    void              toOptimals              ( Net* );
                    void              updateNetTopology       ( Net* );
                    bool              moveUpNetTrunk          ( AutoSegment*, set<Net*>& globalNets, GCell::Set& invalidateds );
                    void              layerAssign             ( uint32_t method );
                    void              finalizeLayout          ();
                    void              exportExternalNets      ();
      inline const  AutoContactLut&   _getAutoContactLut      () const;
      inline const  AutoSegmentLut&   _getAutoSegmentLut      () const;
                    void              _link                   ( AutoContact* );
                    void              _link                   ( AutoSegment* );
                    void              _unlink                 ( AutoContact* );
                    void              _unlink                 ( AutoSegment* );
                    AutoContact*      _lookup                 ( Contact* ) const;
                    AutoSegment*      _lookup                 ( Segment* ) const;
                    EdgeCapacity*     _createCapacity         ( Flags, Interval );
                    size_t            _unrefCapacity          ( EdgeCapacity* );
                    void              _loadGrByNet            ();
                    void              _computeNetOptimals     ( Net* );
                    void              _computeNetTerminals    ( Net* );
                    void              _alignate               ( Net* );
                    void              _desaturate             ( unsigned int depth, set<Net*>&, unsigned long& total, unsigned long& globals );
                    void              _layerAssignByLength    ( unsigned long& total, unsigned long& global, set<Net*>& );
                    void              _layerAssignByLength    ( Net*, unsigned long& total, unsigned long& global, set<Net*>& );
                    void              _layerAssignByTrunk     ( unsigned long& total, unsigned long& global, set<Net*>& );
                    void              _layerAssignByTrunk     ( Net*, set<Net*>&, unsigned long& total, unsigned long& global );
                    void              _layerAssignNoGlobalM2V ( unsigned long& total, unsigned long& global, set<Net*>& );
                    void              _layerAssignNoGlobalM2V ( Net*, set<Net*>&, unsigned long& total, unsigned long& global );
                    void              _saveNet                ( Net* );
                    void              _destroyAutoContacts    ();
                    void              _destroyAutoSegments    ();
                    void              _check                  ( Net* net ) const;
                    bool              _check                  ( const char* message ) const;
                    void              printMeasures           ( const string& tag ) const;
    // Misc. functions.                                       
      inline const  Flags&            flags                   () const;
      inline        Flags&            flags                   ();
                    void              reset                   ();
      inline        void              _add                    ( GCell* );
      inline        void              _remove                 ( GCell* );
      inline        void              _updateLookup           ( GCell* );
      inline        void              _updateGContacts        ( Flags flags=Flags::Horizontal|Flags::Vertical );
      inline        void              _resizeMatrix           ();
      inline        bool              _inDestroy              () const;
    // Inspector support.                                     
      virtual       Record*           _getRecord              () const;
      virtual       string            _getString              () const;
      virtual       string            _getTypeName            () const;
    protected:                                                
                                      AnabaticEngine          ( Cell* );
      virtual                        ~AnabaticEngine          ();
      virtual       void              _postCreate             ();
      virtual       void              _preDestroy             ();
                    void              _gutAnabatic            ();
      virtual       Configuration*    _createConfiguration    ();
    private:                                                   
                                      AnabaticEngine          ( const AnabaticEngine& );
                    AnabaticEngine&   operator=               ( const AnabaticEngine& );
    private:
      static Name                _toolName;
    protected:
             Configuration*      _configuration;
    private:
             ChipTools           _chipTools;
             EngineState         _state;
             Matrix              _matrix;
             vector<GCell*>      _gcells;
             vector<Edge*>       _ovEdges;
             vector<NetData*>    _netOrdering;
             NetDatas            _netDatas;
             CellViewer*         _viewer;
             Flags               _flags;
             int                 _stamp;
             uint32_t            _routingMode;
             uint64_t            _densityMode;
             AutoSegmentLut      _autoSegmentLut;
             AutoContactLut      _autoContactLut;
             EdgeCapacityLut     _edgeCapacitiesLut;
             Net*                _blockageNet;
             Cell*               _diodeCell;
  };


  inline       bool              AnabaticEngine::isDigitalMode            () const { return (_routingMode & DigitalMode); };
  inline       bool              AnabaticEngine::isAnalogMode             () const { return (_routingMode & AnalogMode); };
  inline       bool              AnabaticEngine::isMixedMode              () const { return (_routingMode & MixedMode); };
  inline       bool              AnabaticEngine::isChannelStyle           () const { return (_configuration->getRoutingStyle() & StyleFlags::Channel); };
  inline       bool              AnabaticEngine::isHybridStyle            () const { return (_configuration->getRoutingStyle() & StyleFlags::Hybrid); };
  inline       void              AnabaticEngine::setRoutingMode           ( uint32_t mode ) { _routingMode |=  mode;  };
  inline       void              AnabaticEngine::resetRoutingMode         ( uint32_t mode ) { _routingMode &= ~mode;  };
  inline       EngineState       AnabaticEngine::getState                 () const { return _state; }
  inline       void              AnabaticEngine::setState                 ( EngineState state ) { _state = state; }
  inline       CellViewer*       AnabaticEngine::getViewer                () const { return _viewer; }
  inline       void              AnabaticEngine::setViewer                ( CellViewer* viewer ) { _viewer=viewer; }
  inline const Matrix*           AnabaticEngine::getMatrix                () const { return &_matrix; }
  inline const vector<GCell*>&   AnabaticEngine::getGCells                () const { return _gcells; }
  inline const vector<Edge*>&    AnabaticEngine::getOvEdges               () const { return _ovEdges; }
  inline       GCell*            AnabaticEngine::getSouthWestGCell        () const { return _gcells[0]; }
  inline       GCell*            AnabaticEngine::getGCellUnder            ( DbU::Unit x, DbU::Unit y ) const { return _matrix.getUnder(x,y); }
  inline       GCell*            AnabaticEngine::getGCellUnder            ( Point p ) const { return _matrix.getUnder(p); }
  inline       GCellsUnder       AnabaticEngine::getGCellsUnder           ( Segment* s ) const { return std::shared_ptr<RawGCellsUnder>( new RawGCellsUnder(this,s) ); }
  inline       GCellsUnder       AnabaticEngine::getGCellsUnder           ( Point source, Point target ) const { return std::shared_ptr<RawGCellsUnder>( new RawGCellsUnder(this,source,target) ); }
  inline       Edges             AnabaticEngine::getEdgesUnderPath        ( GCell* source, GCell* target, Flags pathFlags ) const { return new Path_Edges(source,target,pathFlags); }
  inline       uint64_t          AnabaticEngine::getDensityMode           () const { return _densityMode; }
  inline       void              AnabaticEngine::setDensityMode           ( uint64_t mode ) { _densityMode=mode; }
  inline       void              AnabaticEngine::setBlockageNet           ( Net* net ) { _blockageNet = net; }
  inline const AutoContactLut&   AnabaticEngine::_getAutoContactLut       () const { return _autoContactLut; }
  inline const AutoSegmentLut&   AnabaticEngine::_getAutoSegmentLut       () const { return _autoSegmentLut; }
  inline const Flags&            AnabaticEngine::flags                    () const { return _flags; }
  inline       Flags&            AnabaticEngine::flags                    () { return _flags; }
  inline       bool              AnabaticEngine::doDestroyBaseContact     () const { return _flags & Flags::DestroyBaseContact; }
  inline       bool              AnabaticEngine::doDestroyBaseSegment     () const { return _flags & Flags::DestroyBaseSegment; }
  inline       bool              AnabaticEngine::doDestroyTool            () const { return _state >= EngineGutted; }
  inline       bool              AnabaticEngine::doWarnOnGCellOverload    () const { return _flags & Flags::WarnOnGCellOverload; }
  inline       bool              AnabaticEngine::isCanonizeDisabled       () const { return _flags & Flags::DisableCanonize; }
  inline       bool              AnabaticEngine::isInDemoMode             () const { return _flags & Flags::DemoMode; }
  inline       bool              AnabaticEngine::isChip                   () const { return _chipTools.isChip(); }
  inline       std::string       AnabaticEngine::getNetBuilderStyle       () const { return _configuration->getNetBuilderStyle(); }
  inline       StyleFlags        AnabaticEngine::getRoutingStyle          () const { return _configuration->getRoutingStyle(); }
  inline       DbU::Unit         AnabaticEngine::getAntennaGateMaxWL      () const { return _configuration->getAntennaGateMaxWL(); }
  inline       DbU::Unit         AnabaticEngine::getAntennaDiodeMaxWL     () const { return _configuration->getAntennaDiodeMaxWL(); }
  inline       DbU::Unit         AnabaticEngine::getGlobalThreshold       () const { return _configuration->getGlobalThreshold(); }
  inline       float             AnabaticEngine::getSaturateRatio         () const { return _configuration->getSaturateRatio(); }
  inline       size_t            AnabaticEngine::getSaturateRp            () const { return _configuration->getSaturateRp(); }
  inline       void              AnabaticEngine::setSaturateRatio         ( float ratio ) { _configuration->setSaturateRatio(ratio); }
  inline       void              AnabaticEngine::setSaturateRp            ( size_t threshold ) { _configuration->setSaturateRp(threshold); }
  inline       Cell*             AnabaticEngine::getDiodeCell             () const { return _diodeCell; }
  inline       Net*              AnabaticEngine::getBlockageNet           () const { return _blockageNet; }
  inline const ChipTools&        AnabaticEngine::getChipTools             () const { return _chipTools; }
  inline const vector<NetData*>& AnabaticEngine::getNetOrdering           () const { return _netOrdering; }
  inline       void              AnabaticEngine::setGlobalThreshold       ( DbU::Unit threshold ) { _configuration->setGlobalThreshold(threshold); }
  inline const NetDatas&         AnabaticEngine::getNetDatas              () const { return _netDatas; }
  inline       void              AnabaticEngine::_updateLookup            ( GCell* gcell ) { _matrix.updateLookup(gcell); }
  inline       void              AnabaticEngine::_resizeMatrix            () { _matrix.resize( getCell(), getGCells() ); }
  inline       void              AnabaticEngine::_updateGContacts         ( Flags flags ) { for ( GCell* gcell : getGCells() ) gcell->updateGContacts(flags); }
  inline       bool              AnabaticEngine::_inDestroy               () const { return _flags & Flags::DestroyMask; }
  inline       void              AnabaticEngine::disableCanonize          () { _flags |= Flags::DisableCanonize; }
  inline       void              AnabaticEngine::enableCanonize           () { _flags.reset( Flags::DisableCanonize ); }
  
  inline void  AnabaticEngine::_add ( GCell* gcell )
  {
    _gcells.push_back( gcell );
  //std::sort( _gcells.begin(), _gcells.end(), Entity::CompareById() );
  }

  inline void  AnabaticEngine::_remove ( GCell* gcell )
  {
    for ( auto igcell = _gcells.begin() ; igcell != _gcells.end() ; ++igcell )
      if (*igcell == gcell) {
        if (_inDestroy()) (*igcell) = NULL;
        else              _gcells.erase(igcell);
        break;
      }
  }

  inline       int    AnabaticEngine::getStamp () const { return _stamp; }
  inline       int    AnabaticEngine::incStamp () { return ++_stamp; }

  inline void  AnabaticEngine::addOv ( Edge* edge ) {
    _ovEdges.push_back(edge);
  }

  inline void  AnabaticEngine::removeOv ( Edge* edge )
  {
    for ( auto iedge = _ovEdges.begin() ; iedge != _ovEdges.end() ; ++iedge ) {
      if (*iedge == edge) { _ovEdges.erase(iedge); return; }
    }
  }


  extern const char* badMethod;

}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::AnabaticEngine);
