// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   S m u r f  -  A   D e m o   T o o l E n g i n e               |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./smurf/GraphicSmurfEngine.h"                  |
// +-----------------------------------------------------------------+



#ifndef  SMURF_GRAPHIC_SMURF_ENGINE_H
#define  SMURF_GRAPHIC_SMURF_ENGINE_H

#include <QObject>

namespace Hurricane {
  class Go;
  class BasicLayer;
  class Transformation;
  class CellWidget;
  class CellViewer;
}

#include "crlcore/GraphicToolEngine.h"
#include "smurf/SmurfEngine.h"


namespace Smurf {

  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
    

// -------------------------------------------------------------------
// Class  :  "Smurf::GraphicSmurfEngine".

  class GraphicSmurfEngine : public GraphicTool {
      Q_OBJECT;

    public:
              SmurfEngine*        createEngine       ();
              SmurfEngine*        getForFramework    ();
      static  GraphicSmurfEngine* grab               ();
      virtual const Name&           getName            () const;
              Cell*                 getCell            ();
      virtual size_t                release            ();
      virtual void                  addToMenu          ( CellViewer* );
              void                  postEvent          ();
    public slots:                                     
               void                 place              ();
                               
    protected:                 
      static  size_t                _references;
      static  GraphicSmurfEngine* _singleton;
              CellViewer*           _viewer;
    protected:
                                    GraphicSmurfEngine  ();
      virtual                      ~GraphicSmurfEngine  ();
  };


}  // Smurf namespace.

#endif  // SMURF_GRAPHIC_SMURF_ENGINE_H
