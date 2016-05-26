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

namespace Hurricane {
  class Name;
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
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::Cell;
  using Hurricane::CellViewer;
  using CRL::ToolEngine;


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
      inline        GCell*          getSouthWestGCell () const;
      inline        GCell*          getGCellUnder     ( DbU::Unit x, DbU::Unit y ) const;
      inline        GCell*          getGCellUnder     ( Point ) const;
                    int             getCapacity       ( Interval, Flags ) const;
      inline        void            _add              ( GCell* );
      inline        void            _remove           ( GCell* );
      inline        void            _updateLookup     ( GCell* );
                    void            _runTest          ();
    // Inspector support.                             
      virtual       Record*         _getRecord        () const;
      virtual       string          _getString        () const;
      virtual       string          _getTypeName      () const;
    protected:                                        
                                     AnabaticEngine   ( Cell* );
      virtual                       ~AnabaticEngine   ();
      virtual       void             _postCreate      ();
      virtual       void             _preDestroy      ();
    private:                                          
                                     AnabaticEngine   ( const AnabaticEngine& );
                    AnabaticEngine&  operator=        ( const AnabaticEngine& );
    private:
      static Name           _toolName;
             Configuration* _configuration;
             Matrix         _matrix;
             GCellSet       _gcells;
             CellViewer*    _viewer;
  };


  inline CellViewer* AnabaticEngine::getViewer         () const { return _viewer; }
  inline void        AnabaticEngine::setViewer         ( CellViewer* viewer ) { _viewer=viewer; }
  inline GCell*      AnabaticEngine::getSouthWestGCell () const { return *(_gcells.begin()); }
  inline GCell*      AnabaticEngine::getGCellUnder     ( DbU::Unit x, DbU::Unit y ) const { return _matrix.getUnder(x,y); }
  inline GCell*      AnabaticEngine::getGCellUnder     ( Point p ) const { return _matrix.getUnder(p); }
  inline void        AnabaticEngine::_add              ( GCell* gcell ) { _gcells.insert(gcell); }
  inline void        AnabaticEngine::_remove           ( GCell* gcell ) { _gcells.erase(gcell); }
  inline void        AnabaticEngine::_updateLookup     ( GCell* gcell ) { _matrix.updateLookup(gcell); }
  

}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::AnabaticEngine);

#endif  // ANABATIC_ANABATIC_ENGINE_H
