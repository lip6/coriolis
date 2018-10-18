// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :          Jean-Paul Chaput, Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/GraphicBoraEngine.h"                    |
// +-----------------------------------------------------------------+


#ifndef  BORA_GRAPHIC_BORA_ENGINE_H
#define  BORA_GRAPHIC_BORA_ENGINE_H

#include <QObject>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "hurricane/DbU.h"
namespace Hurricane {
  class Go;
  class BasicLayer;
  class Transformation;
  class CellWidget;
  class CellViewer;
}
#include "crlcore/GraphicToolEngine.h"
#include "bora/BoraEngine.h"


namespace Bora {

  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::Cell;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;

  class SlicingPlotWidget;
  class SlicingDataWidget;
  class SlicingWidget;


// -------------------------------------------------------------------
// Class  :  "Bora::GraphicBoraEngine".


  class GraphicBoraEngine : public GraphicTool {
      Q_OBJECT;
    public:
      enum Flags { NoFlags=0x0000, CreateEngine=0x0001 };
    public:
              BoraEngine*         createEngine        ();
              BoraEngine*         getForFramework     ( unsigned int flags );
      static  GraphicBoraEngine*  grab                ();
      virtual const Name&         getName             () const;
              Cell*               getCell             ();
      virtual size_t              release             ();
      virtual void                addToMenu           ( CellViewer* );
              void                updateSlicingTree   ();
              void                setToleranceRatio   ();
              void                setToleranceBand    ();
              void                showPareto          ();
      inline  SlicingWidget*      getSlicingWidget    ();                                    
    public slots:
              void                updatePlacementST   ( BoxSet* );
              void                updatePlacementST   ( size_t index  );
    protected:                                      
      static  size_t              _references;
      static  GraphicBoraEngine*  _singleton;
              CellViewer*         _viewer;
              SlicingWidget*      _slicingWidget;
    protected:                                      
                                  GraphicBoraEngine ();
      virtual                    ~GraphicBoraEngine ();
  };


  inline SlicingWidget* GraphicBoraEngine::getSlicingWidget () { return _slicingWidget; }

  
}  // Bora namespace.

#endif  // BORA_GRAPHIC_BORA_ENGINE_H
