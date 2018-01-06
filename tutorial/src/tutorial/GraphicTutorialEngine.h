// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        T o o l E n g i n e   T u t o r i a l                    |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./tutorial/GraphicTutorialEngine.h"            |
// +-----------------------------------------------------------------+



#ifndef  TUTORIAL_GRAPHIC_TUTORIAL_ENGINE_H
#define  TUTORIAL_GRAPHIC_TUTORIAL_ENGINE_H

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
#include "tutorial/TutorialEngine.h"


namespace Tutorial {

  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
    

// -------------------------------------------------------------------
// Class  :  "Tutorial::GraphicTutorialEngine".

  class GraphicTutorialEngine : public GraphicTool {
      Q_OBJECT;

    public:
              TutorialEngine*        createEngine          ();
              TutorialEngine*        getForFramework       ();
      static  GraphicTutorialEngine* grab                  ();
      virtual const Name&            getName               () const;
              Cell*                  getCell               ();
      virtual size_t                 release               ();
      virtual void                   addToMenu             ( CellViewer* );
              void                   postEvent             ();
    protected:
                                     GraphicTutorialEngine ();
      virtual                       ~GraphicTutorialEngine ();
              void                   _runDemoPart1         ();
              void                   _runDemoPart2         ();
    protected:                 
      static  size_t                 _references;
      static  GraphicTutorialEngine* _singleton;
              CellViewer*            _viewer;
  };


}  // Tutorial namespace.

#endif  // TUTORIAL_GRAPHIC_TUTORIAL_ENGINE_H
