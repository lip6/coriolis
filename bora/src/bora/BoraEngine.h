// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/BoraEngine.h"                           |
// +-----------------------------------------------------------------+


#ifndef  BORA_BORA_ENGINE_H
#define  BORA_BORA_ENGINE_H

#include <iostream>
#include "hurricane/viewer/CellViewer.h"
namespace Hurricane {
  class Cell;
  class CellWidget;
}
#include "crlcore/ToolEngine.h"


namespace Bora {

  using Hurricane::Name;
  using Hurricane::DbU;
  using Hurricane::Cell;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  class SlicingPlotWidget;
  class BoxSet;


  class BoraEngine : public CRL::ToolEngine {
    public:
      typedef CRL::ToolEngine  Super;
    public:
      static  const Name&            staticGetName           ();
      static        BoraEngine*      get                     ( const Cell* );
      static        BoraEngine*      create                  ( Cell* );
      virtual const Name&            getName                 () const;
      inline        CellViewer*      getViewer               () const;
                    void             setViewer               ( CellViewer* );
                    void             updateSlicingTree       ();
                    void             updatePlacement         ( size_t index );
                    void             updatePlacement         ( DbU::Unit width, DbU::Unit height );
                    void             updatePlacement         ( BoxSet* );
                    void             setToleranceRatio       ( DbU::Unit, DbU::Unit );
                    void             setToleranceBand        ( DbU::Unit, DbU::Unit );
                    void             showPareto              ();
      inline        CellWidget*      getCellWidget           ();
      inline        void             emitCellPreModificated  ();
      inline        void             emitCellPostModificated ();
                    void             resetPlacement          ();
                    DbU::Unit        getToleranceRatioH      () const;
                    DbU::Unit        getToleranceRatioW      () const;
                    DbU::Unit        getToleranceBandH       () const;
                    DbU::Unit        getToleranceBandW       () const;
    protected:                                               
                                     BoraEngine              ( Cell* );
      virtual                       ~BoraEngine              ();
      virtual void                   _postCreate             ();
      virtual void                   _preDestroy             ();
      virtual void                   _runBoraInit            ();

    private:
      static  Hurricane::Name        _toolName;
              Hurricane::CellViewer* _viewer;
  };


  inline Hurricane::CellWidget* BoraEngine::getCellWidget           () { return (_viewer) ? _viewer->getCellWidget() : NULL; }
  inline void                   BoraEngine::emitCellPreModificated  () { if (_viewer) _viewer->emitCellPreModificated() ; }
  inline void                   BoraEngine::emitCellPostModificated () { if (_viewer) _viewer->emitCellPostModificated(); }
  inline Hurricane::CellViewer* BoraEngine::getViewer               () const { return _viewer; }


}  // Bora namespace.

#endif  // BORA_BORA_ENGINE_H
