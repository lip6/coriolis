
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicKatabaticEngine.h"               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __KATABATIC_GRAPHIC_KATABATIC_ENGINE__
#define  __KATABATIC_GRAPHIC_KATABATIC_ENGINE__

#include  <QObject>

namespace Hurricane {
  class Go;
  class BasicLayer;
  class Transformation;
  class CellWidget;
  class CellViewer;
}

#include  "crlcore/GraphicToolEngine.h"
namespace CRL {
  class RoutingGauge;
}

#include  "katabatic/KatabaticEngine.h"


namespace Katabatic {

  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::RoutingGauge;
  using CRL::GraphicTool;
    

// -------------------------------------------------------------------
// Class  :  "CRL::GraphicKatabaticEngine".


  class GraphicKatabaticEngine : public GraphicTool {
      Q_OBJECT;

    public:
      static  KatabaticEngine*        create             ( const RoutingGauge*, Cell* );
      static   void                   initKatabaticAc    ( CellWidget* );
      static   void                   drawKatabaticAc    ( CellWidget*
                                                         , const Go*
                                                         , const BasicLayer*
                                                         , const Box&
                                                         , const Transformation&
                                                         );
      static   void                   initKatabaticGCell ( CellWidget* );
      static   void                   drawKatabaticGCell ( CellWidget*
                                                         , const Go*
                                                         , const BasicLayer*
                                                         , const Box&
                                                         , const Transformation&
                                                         );
      static  GraphicKatabaticEngine* grab               ();
      virtual const Name&             getName            () const;
      virtual size_t                  release            ();
      virtual  void                   addToMenu          ( CellViewer* );
    public slots:                                       
               void                   run                ();
               void                   closeRoute         ();
                               
    protected:                 
      static  size_t                  _references;
      static  GraphicKatabaticEngine* _singleton;
              CellViewer*             _viewer;
    protected:
                                      GraphicKatabaticEngine ();
      virtual                        ~GraphicKatabaticEngine ();
  };



} // End of Katabatic namespace.


#endif  // __KATABATIC_GRAPHIC_KATABATIC_ENGINE__
