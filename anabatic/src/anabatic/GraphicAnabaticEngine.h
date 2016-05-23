// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/GraphicAnabaticEngine.h"            |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_GRAPHIC_ANABATIC_ENGINE_H
#define  ANABATIC_GRAPHIC_ANABATIC_ENGINE_H

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
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
    

// -------------------------------------------------------------------
// Class  :  "Anabatic::GraphicAnabaticEngine".

  class GraphicAnabaticEngine : public GraphicTool {
      Q_OBJECT;

    public:
      enum Flags { NoFlags=0x0000, CreateEngine=0x0001 };
    public:
              AnabaticEngine*        createEngine    ();
              AnabaticEngine*        getForFramework ( unsigned int flags );
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
      static  GraphicAnabaticEngine* grab            ();
      virtual const Name&            getName         () const;
              Cell*                  getCell         ();
      virtual size_t                 release         ();
      virtual void                   addToMenu       ( CellViewer* );
                               
    protected:                 
      static  size_t                 _references;
      static  GraphicAnabaticEngine* _singleton;
              CellViewer*            _viewer;
    protected:
                                     GraphicAnabaticEngine ();
      virtual                       ~GraphicAnabaticEngine ();
              void                  _runTest               ();
  };


}  // Anabatic namespace.

#endif  // ANABATIC_GRAPHIC_ANABATIC_ENGINE_H
