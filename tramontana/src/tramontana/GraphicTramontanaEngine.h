// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header   :  "./tramontana/GraphicTramontanaEngine.h"       |
// +-----------------------------------------------------------------+


#pragma  once
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
#include "tramontana/TramontanaEngine.h"


namespace Tramontana {

  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
    

// -------------------------------------------------------------------
// Class  :  "Tramontana::GraphicTramontanaEngine".

  class GraphicTramontanaEngine : public GraphicTool {
      Q_OBJECT;

    public:
      enum FunctionFlags { NoFlags=0x0000, CreateEngine=0x0001 };
    public:
#if THIS_IS_DISABLED
      static  void                       initGCell       ( CellWidget* );
      static  void                       drawGCell       ( CellWidget*
                                                         , const Go*
                                                         , const BasicLayer*
                                                         , const Box&
                                                         , const Transformation&
                                                         );
#endif
              TramontanaEngine*        createEngine      ();
              TramontanaEngine*        getForFramework   ( uint32_t flags );
      static  GraphicTramontanaEngine* grab              ();
      virtual const Name&              getName           () const;
              Cell*                    getCell           ();
      virtual size_t                   release           ();
      virtual void                     addToMenu         ( CellViewer* );
              void                     postEvent         ();
    protected:                 
      static  size_t                   _references;
      static  GraphicTramontanaEngine* _singleton;
              CellViewer*              _viewer;
    protected:
                                       GraphicTramontanaEngine ();
      virtual                         ~GraphicTramontanaEngine ();
              void                     _extract                ();
  };


}  // Tramontana namespace.
