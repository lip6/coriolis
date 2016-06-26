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
      typedef ToolEngine  Super;
    public:
      static        AnabaticEngine* create            ( Cell* );
      static        AnabaticEngine* get               ( const Cell* );
      static  const Name&           staticGetName     ();
      virtual const Name&           getName           () const;
      virtual       Configuration*  getConfiguration  ();
      inline        CellViewer*     getViewer         () const;
      inline        void            setViewer         ( CellViewer* );
      inline  const Matrix*         getMatrix         () const;
      inline  const vector<GCell*>& getGCells         () const;
      inline  const vector<Edge*>&  getOvEdges        () const;
      inline        GCell*          getSouthWestGCell () const;
      inline        GCell*          getGCellUnder     ( DbU::Unit x, DbU::Unit y ) const;
      inline        GCell*          getGCellUnder     ( Point ) const;
                    int             getCapacity       ( Interval, Flags ) const;
                    size_t          getNetsFromEdge   ( const Edge*, NetSet& );
      inline        void            addOv             ( Edge* );
      inline        void            removeOv          ( Edge* );
    // Dijkstra related functions.
      inline        int             getStamp          () const;
      inline        int             incStamp          ();
    // Global routing related functions.
                    void            globalRoute       ();
    // Misc. functions.
      inline const  Flags&          flags             () const;
      inline        Flags&          flags             ();
                    void            reset             ();
                    void            startMeasures     ();
                    void            stopMeasures      ();
                    void            printMeasures     ( const string& ) const;
      inline        void            _add              ( GCell* );
      inline        void            _remove           ( GCell* );
      inline        void            _updateLookup     ( GCell* );
      inline        bool            _inDestroy        () const;
    // Inspector support.                             
      virtual       Record*         _getRecord        () const;
      virtual       string          _getString        () const;
      virtual       string          _getTypeName      () const;
    protected:                                        
                                     AnabaticEngine   ( Cell* );
      virtual                       ~AnabaticEngine   ();
      virtual       void             _postCreate      ();
      virtual       void             _preDestroy      ();
                    void             _clear           ();
    private:                                          
                                     AnabaticEngine   ( const AnabaticEngine& );
                    AnabaticEngine&  operator=        ( const AnabaticEngine& );
    private:
      static Name           _toolName;
             Timer          _timer;
             Configuration* _configuration;
             Matrix         _matrix;
             vector<GCell*> _gcells;
             vector<Edge*>  _ovEdges;
             CellViewer*    _viewer;
             Flags          _flags;
             int            _stamp;
  };


  inline CellViewer*           AnabaticEngine::getViewer         () const { return _viewer; }
  inline void                  AnabaticEngine::setViewer         ( CellViewer* viewer ) { _viewer=viewer; }
  inline const Matrix*         AnabaticEngine::getMatrix         () const { return &_matrix; }
  inline const vector<GCell*>& AnabaticEngine::getGCells         () const { return _gcells; }
  inline const vector<Edge*>&  AnabaticEngine::getOvEdges        () const { return _ovEdges; }
  inline GCell*                AnabaticEngine::getSouthWestGCell () const { return _gcells[0]; }
  inline GCell*                AnabaticEngine::getGCellUnder     ( DbU::Unit x, DbU::Unit y ) const { return _matrix.getUnder(x,y); }
  inline GCell*                AnabaticEngine::getGCellUnder     ( Point p ) const { return _matrix.getUnder(p); }
  inline const Flags&          AnabaticEngine::flags             () const { return _flags; }
  inline Flags&                AnabaticEngine::flags             () { return _flags; }
  inline void                  AnabaticEngine::_updateLookup     ( GCell* gcell ) { _matrix.updateLookup(gcell); }
  inline bool                  AnabaticEngine::_inDestroy        () const { return _flags & Flags::Destroy; }
  
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
