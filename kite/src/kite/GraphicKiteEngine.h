
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
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicKiteEngine.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __KITE_GRAPHIC_KITE_ENGINE__
#define  __KITE_GRAPHIC_KITE_ENGINE__

#include  <QObject>

namespace Hurricane {
  class Go;
  class BasicLayer;
  class Transformation;
  class CellWidget;
  class CellViewer;
}

#include  "crlcore/GraphicToolEngine.h"

#include  "kite/KiteEngine.h"


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
              KiteEngine*        createEngine       ();
              KiteEngine*        getForFramework    ();
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
    public slots:                                  
               void              loadGlobalSolution ();
               void              saveGlobalSolution ();
               void              runGlobal          ();
               void              runDetailed        ();
               void              finalize           ();
               void              save               ();
                               
    protected:                 
      static  size_t             _references;
      static  GraphicKiteEngine* _singleton;
              CellViewer*        _viewer;
    protected:
                                 GraphicKiteEngine  ();
      virtual                   ~GraphicKiteEngine  ();
  };


} // End of Kite namespace.


#endif  // __KITE_GRAPHIC_KITE_ENGINE__
