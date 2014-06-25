// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./kite/GraphicKiteEngine.h"                    |
// +-----------------------------------------------------------------+



#ifndef  KITE_GRAPHIC_KITE_ENGINE_H
#define  KITE_GRAPHIC_KITE_ENGINE_H

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
#include "kite/KiteEngine.h"


namespace Kite {

  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
    

// -------------------------------------------------------------------
// Class  :  "Kite::GraphicKiteEngine".

  class GraphicKiteEngine : public GraphicTool {
      Q_OBJECT;

    public:
      enum Flags { NoFlags=0x0000, CreateEngine=0x0001 };

    public:
              KiteEngine*        createEngine       ();
              KiteEngine*        getForFramework    ( unsigned int flags );
      static  void               initKatabaticAc    ( CellWidget* );
      static  void               drawKatabaticAc    ( CellWidget*
                                                    , const Go*
                                                    , const BasicLayer*
                                                    , const Box&
                                                    , const Transformation&
                                                    );
      static  void               initKatabaticGCell ( CellWidget* );
      static  void               drawKatabaticGCell ( CellWidget*
                                                    , const Go*
                                                    , const BasicLayer*
                                                    , const Box&
                                                    , const Transformation&
                                                    );
      static  GraphicKiteEngine* grab               ();
      virtual const Name&        getName            () const;
              Cell*              getCell            ();
      virtual size_t             release            ();
      virtual void               addToMenu          ( CellViewer* );
              void               postEvent          ();
                               
    protected:                 
      static  size_t             _references;
      static  GraphicKiteEngine* _singleton;
              CellViewer*        _viewer;
    protected:
                                 GraphicKiteEngine      ();
      virtual                   ~GraphicKiteEngine      ();
              void               _route                 ();
              void               _loadGlobalSolution    ();
              void               _saveGlobalSolution    ();
              void               _globalRoute           ();
              void               _loadGlobalRouting     ();
              void               _balanceGlobalDensity  ();
              void               _runNegociatePreRouted ();
              void               _runNegociate          ();
              void               _detailRoute           ();
              void               _finalize              ();
              void               _save                  ();
              void               _dumpMeasures          ();
  };


}  // Kite namespace.

#endif  // KITE_GRAPHIC_KITE_ENGINE_H
