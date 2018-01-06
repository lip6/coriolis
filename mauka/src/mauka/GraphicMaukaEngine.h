// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./mauka/GraphicMaukaEngine.h"                  |
// +-----------------------------------------------------------------+


#ifndef  MAUKA_GRAPHIC_MAUKA_ENGINE_H
#define  MAUKA_GRAPHIC_MAUKA_ENGINE_H

#include  <QObject>

namespace Hurricane {
  class Go;
  class BasicLayer;
  class Transformation;
  class CellWidget;
  class CellViewer;
}

#include  "crlcore/GraphicToolEngine.h"
#include  "mauka/MaukaEngine.h"


namespace Mauka {

  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
    

// -------------------------------------------------------------------
// Class  :  "Mauka::GraphicMaukaEngine".


  class GraphicMaukaEngine : public GraphicTool {
      Q_OBJECT;

    public:
      enum Flags { NoFlags=0x0000, CreateEngine=0x0001 };
    public:
              MaukaEngine*        createEngine         ();
              MaukaEngine*        getForFramework      ( unsigned int flags );
      static  void                initMaukaContainer   ( CellWidget* );
      static  void                drawMaukaContainer   ( CellWidget*
                                                       , const Go*
                                                       , const BasicLayer*
                                                       , const Box&
                                                       , const Transformation&
                                                       );
      static  GraphicMaukaEngine* grab                 ();
      virtual const Name&         getName              () const;
              Cell*               getCell              ();
      virtual size_t              release              ();
      virtual void                addToMenu            ( CellViewer* );
              void                refreshViewer        ();
    protected:                 
      static  size_t              _references;
      static  GraphicMaukaEngine* _singleton;
              CellViewer*         _viewer;
    protected:
                                  GraphicMaukaEngine    ();
      virtual                    ~GraphicMaukaEngine    ();
               void               _doQuadriPart         ();
               void               _doSimulatedAnnealing ();
               void               _place                ();
               void               _save                 ();
  };


} // Mauka namespace.

#endif  // MAUKA_GRAPHIC_MAUKA_ENGINE_H
