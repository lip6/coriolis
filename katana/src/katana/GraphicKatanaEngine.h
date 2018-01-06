// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/GraphicKatanaEngine.h"                |
// +-----------------------------------------------------------------+



#ifndef  KATANA_GRAPHIC_KATANA_ENGINE_H
#define  KATANA_GRAPHIC_KATANA_ENGINE_H

#include <functional>
#include <QObject>

namespace Hurricane {
  class Go;
  class BasicLayer;
  class Transformation;
  class CellWidget;
  class CellViewer;
}

#include "crlcore/GraphicToolEngine.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
    

// -------------------------------------------------------------------
// Class  :  "Katana::GraphicKatanaEngine".

  class GraphicKatanaEngine : public GraphicTool {
      Q_OBJECT;

    public:
      enum FunctionFlags { NoFlags=0x0000, CreateEngine=0x0001 };
    public:
      static  void                   initGCell       ( CellWidget* );
      static  void                   drawGCell       ( CellWidget*
                                                     , const Go*
                                                     , const BasicLayer*
                                                     , const Box&
                                                     , const Transformation&
                                                     );
      static  void                   initEdge        ( CellWidget* );
      static  void                   drawEdge        ( CellWidget*
                                                     , const Go*
                                                     , const BasicLayer*
                                                     , const Box&
                                                     , const Transformation&
                                                     );
              KatanaEngine*        createEngine      ();
              KatanaEngine*        getForFramework   ( uint32_t flags );
      static  GraphicKatanaEngine* grab              ();
      virtual const Name&          getName           () const;
              Cell*                getCell           ();
      virtual size_t               release           ();
      virtual void                 addToMenu         ( CellViewer* );
              void                 postEvent         ();
    protected:                 
      static  size_t               _references;
      static  GraphicKatanaEngine* _singleton;
              CellViewer*          _viewer;
    protected:
                                   GraphicKatanaEngine    ();
      virtual                     ~GraphicKatanaEngine    ();
              void                 _wipeoutRouting        ();
              void                 _route                 ();
              void                 _loadGlobalSolution    ();
              void                 _saveGlobalSolution    ();
              void                 _globalRoute           ();
              void                 _loadGlobalRouting     ();
              void                 _balanceGlobalDensity  ();
              void                 _runNegociatePreRouted ();
              void                 _runNegociate          ();
              void                 _detailRoute           ();
              void                 _finalize              ();
              void                 _save                  ();
              void                 _runTest               ();
              void                 _dumpMeasures          ();
  };


}  // Katana namespace.

#endif  // KATANA_GRAPHIC_KATANA_ENGINE_H
