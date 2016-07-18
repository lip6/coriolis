// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
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


#ifndef  ANABATIC_ANABATIC_ENGINE_H
#define  ANABATIC_ANABATIC_ENGINE_H

#include <string>
#include <vector>
#include <set>

#include "hurricane/Timer.h"
namespace Hurricane {
  class Name;
  class Net;
  class Cell;
  class Instance;
  class CellViewer;
}

#include "crlcore/ToolEngine.h"
#include "anabatic/Configuration.h"
#include "anabatic/Matrix.h"
#include "anabatic/GCell.h"
#include "anabatic/AutoContact.h"
#include "anabatic/AutoSegments.h"


namespace Anabatic {

  using std::string;
  using std::vector;
  using Hurricane::Timer;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::Cell;
  using Hurricane::CellViewer;
  using CRL::ToolEngine;


  typedef  std::set<Net*,Entity::CompareById>  NetSet;


  class AnabaticEngine : public ToolEngine {
    public:
      enum DensityMode { AverageHVDensity=1  // Average between all densities.
                       , AverageHDensity =2  // Average between all H densities.
                       , AverageVDensity =3  // Average between all V densities.
                       , MaxHVDensity    =4  // Maximum between average H and average V.
                       , MaxVDensity     =5  // Maximum of V densities.
                       , MaxHDensity     =6  // Maximum of H densities.
                       , MaxDensity      =7  // Maximum of H & V densities.
                       };
    public:
      typedef ToolEngine  Super;
    public:
      static        AnabaticEngine* create                ( Cell* );
      static        AnabaticEngine* get                   ( const Cell* );
      static  const Name&           staticGetName         ();
      virtual const Name&           getName               () const;
      virtual       Configuration*  getConfiguration      ();
      inline        unsigned int    getDensityMode        () const;
      inline        CellViewer*     getViewer             () const;
      inline        void            setViewer             ( CellViewer* );
      inline        EngineState     getState              () const;
      inline  const Matrix*         getMatrix             () const;
      inline  const vector<GCell*>& getGCells             () const;
      inline  const vector<Edge*>&  getOvEdges            () const;
      inline        GCell*          getSouthWestGCell     () const;
      inline        GCell*          getGCellUnder         ( DbU::Unit x, DbU::Unit y ) const;
      inline        GCell*          getGCellUnder         ( Point ) const;
                    int             getCapacity           ( Interval, Flags ) const;
                    size_t          getNetsFromEdge       ( const Edge*, NetSet& );
      inline        void            setState              ( EngineState state );
      inline        void            setDensityMode        ( unsigned int );
      inline        void            addOv                 ( Edge* );
      inline        void            removeOv              ( Edge* );
    // Dijkstra related functions.                        
      inline        int             getStamp              () const;
      inline        int             incStamp              ();
    // Global routing related functions.                  
                    void            globalRoute           ();
                    void            cleanupGlobal         ();
    // Detailed routing related functions.                
      inline        bool            isInDemoMode          () const;
      inline        bool            doWarnOnGCellOverload () const;
      inline        bool            doDestroyBaseContact  () const;
      inline        bool            doDestroyBaseSegment  () const;
      inline        bool            doDestroyTool         () const;
      inline        DbU::Unit       getGlobalThreshold    () const;
      inline        float           getSaturateRatio      () const;
      inline        size_t          getSaturateRp         () const;
      inline        DbU::Unit       getExtensionCap       () const;
                    void            updateDensity         ();
      inline        void            setGlobalThreshold    ( DbU::Unit );
      inline        void            setSaturateRatio      ( float );
      inline        void            setSaturateRp         ( size_t );
                    void            loadGlobalRouting     ( unsigned int method );
                    void            computeNetConstraints ( Net* );
                    void            toOptimals            ( Net* );
                    void            updateNetTopology     ( Net* );
                    void            finalizeLayout        ();
      inline const  AutoContactLut& _getAutoContactLut    () const;
      inline const  AutoSegmentLut& _getAutoSegmentLut    () const;
                    void            _link                 ( AutoContact* );
                    void            _link                 ( AutoSegment* );
      void            _unlink               ( AutoContact* );
                    void            _unlink               ( AutoSegment* );
                    AutoContact*    _lookup               ( Contact* ) const;
                    AutoSegment*    _lookup               ( Segment* ) const;
                    void            _loadGrByNet          ();
                    void            _loadNetGlobalRouting ( Net* );
                    void            _computeNetOptimals   ( Net* );
                    void            _computeNetTerminals  ( Net* );
                    void            _alignate             ( Net* );
                    void            _saveNet              ( Net* );
                    void            _destroyAutoContacts  ();
                    void            _destroyAutoSegments  ();
    // Misc. functions.                                   
      inline const  Flags&          flags                 () const;
      inline        Flags&          flags                 ();
                    void            reset                 ();
                    void            startMeasures         ();
                    void            stopMeasures          ();
                    void            printMeasures         ( const string& ) const;
      inline        void            _add                  ( GCell* );
      inline        void            _remove               ( GCell* );
      inline        void            _updateLookup         ( GCell* );
      inline        bool            _inDestroy            () const;
    // Inspector support.                                 
      virtual       Record*         _getRecord            () const;
      virtual       string          _getString            () const;
      virtual       string          _getTypeName          () const;
    protected:                                            
                                     AnabaticEngine       ( Cell* );
      virtual                       ~AnabaticEngine       ();
      virtual       void             _postCreate          ();
      virtual       void             _preDestroy          ();
                    void             _gutAnabatic         ();
    private:                                              
                                     AnabaticEngine       ( const AnabaticEngine& );
                    AnabaticEngine&  operator=            ( const AnabaticEngine& );
    private:
      static Name            _toolName;
             Timer           _timer;
             Configuration*  _configuration;
             EngineState     _state;
             Matrix          _matrix;
             vector<GCell*>  _gcells;
             vector<Edge*>   _ovEdges;
             CellViewer*     _viewer;
             Flags           _flags;
             int             _stamp;
             unsigned int    _densityMode;
             AutoSegmentLut  _autoSegmentLut;
             AutoContactLut  _autoContactLut;
  };


  inline       EngineState     AnabaticEngine::getState              () const { return _state; }
  inline       void            AnabaticEngine::setState              ( EngineState state ) { _state = state; }
  inline       CellViewer*     AnabaticEngine::getViewer             () const { return _viewer; }
  inline       void            AnabaticEngine::setViewer             ( CellViewer* viewer ) { _viewer=viewer; }
  inline const Matrix*         AnabaticEngine::getMatrix             () const { return &_matrix; }
  inline const vector<GCell*>& AnabaticEngine::getGCells             () const { return _gcells; }
  inline const vector<Edge*>&  AnabaticEngine::getOvEdges            () const { return _ovEdges; }
  inline       GCell*          AnabaticEngine::getSouthWestGCell     () const { return _gcells[0]; }
  inline       GCell*          AnabaticEngine::getGCellUnder         ( DbU::Unit x, DbU::Unit y ) const { return _matrix.getUnder(x,y); }
  inline       GCell*          AnabaticEngine::getGCellUnder         ( Point p ) const { return _matrix.getUnder(p); }
  inline       unsigned int    AnabaticEngine::getDensityMode        () const { return _densityMode; }
  inline       void            AnabaticEngine::setDensityMode        ( unsigned int mode ) { _densityMode=mode; }
  inline const AutoContactLut& AnabaticEngine::_getAutoContactLut    () const { return _autoContactLut; }
  inline const AutoSegmentLut& AnabaticEngine::_getAutoSegmentLut    () const { return _autoSegmentLut; }
  inline const Flags&          AnabaticEngine::flags                 () const { return _flags; }
  inline       Flags&          AnabaticEngine::flags                 () { return _flags; }
  inline       bool            AnabaticEngine::doDestroyBaseContact  () const { return _flags & Flags::DestroyBaseContact; }
  inline       bool            AnabaticEngine::doDestroyBaseSegment  () const { return _flags & Flags::DestroyBaseSegment; }
  inline       bool            AnabaticEngine::doDestroyTool         () const { return _state >= EngineGutted; }
  inline       bool            AnabaticEngine::doWarnOnGCellOverload () const { return _flags & Flags::WarnOnGCellOverload; }
  inline       bool            AnabaticEngine::isInDemoMode          () const { return _flags & Flags::DemoMode; }
  inline       DbU::Unit       AnabaticEngine::getGlobalThreshold    () const { return _configuration->getGlobalThreshold(); }
  inline       float           AnabaticEngine::getSaturateRatio      () const { return _configuration->getSaturateRatio(); }
  inline       size_t          AnabaticEngine::getSaturateRp         () const { return _configuration->getSaturateRp(); }
  inline       void            AnabaticEngine::setSaturateRatio      ( float ratio ) { _configuration->setSaturateRatio(ratio); }
  inline       void            AnabaticEngine::setSaturateRp         ( size_t threshold ) { _configuration->setSaturateRp(threshold); }
  inline       void            AnabaticEngine::setGlobalThreshold    ( DbU::Unit threshold ) { _configuration->setGlobalThreshold(threshold); }
  inline       void            AnabaticEngine::_updateLookup         ( GCell* gcell ) { _matrix.updateLookup(gcell); }
  inline       bool            AnabaticEngine::_inDestroy            () const { return _flags & Flags::DestroyMask; }
  
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

  inline int  AnabaticEngine::getStamp () const { return _stamp; }
  inline int  AnabaticEngine::incStamp () { return ++_stamp; }

  inline void  AnabaticEngine::addOv ( Edge* edge ) { _ovEdges.push_back(edge); }

  inline void  AnabaticEngine::removeOv ( Edge* edge )
  { 
    for ( auto iedge = _ovEdges.begin() ; iedge != _ovEdges.end() ; ++iedge )
      if (*iedge == edge) { _ovEdges.erase(iedge); break; }
  }

}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::AnabaticEngine);

#endif  // ANABATIC_ANABATIC_ENGINE_H
