
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
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicMaukaEngine.h"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __MAUKA_GRAPHIC_MAUKA_ENGINE__
#define  __MAUKA_GRAPHIC_MAUKA_ENGINE__

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
              MaukaEngine*        createEngine         ();
              MaukaEngine*        getForFramework      ();
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
    public slots:                                      
               void               doQuadriPart         ();
               void               doSimulatedAnnealing ();
               void               place                ();
               void               save                 ();
                               
    protected:                 
      static  size_t              _references;
      static  GraphicMaukaEngine* _singleton;
              CellViewer*         _viewer;
    protected:
                                  GraphicMaukaEngine ();
      virtual                    ~GraphicMaukaEngine ();
  };


} // End of Mauka namespace.


#endif  // __MAUKA_GRAPHIC_MAUKA_ENGINE__
