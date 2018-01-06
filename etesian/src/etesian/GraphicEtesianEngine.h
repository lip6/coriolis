// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./etesian/GraphicEtesianEngine.h"              |
// +-----------------------------------------------------------------+



#ifndef  ETESIAN_GRAPHIC_ETESIAN_ENGINE_H
#define  ETESIAN_GRAPHIC_ETESIAN_ENGINE_H

#include <QObject>

namespace Hurricane {
  class Go;
  class BasicLayer;
  class Transformation;
  class CellWidget;
  class CellViewer;
}

#include "crlcore/GraphicToolEngine.h"
#include "etesian/EtesianEngine.h"


namespace Etesian {

  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
    

// -------------------------------------------------------------------
// Class  :  "Etesian::GraphicEtesianEngine".

  class GraphicEtesianEngine : public GraphicTool {
      Q_OBJECT;

    public:
      enum Flags { NoFlags=0x0000, CreateEngine=0x0001 };

    public:
              EtesianEngine*        createEngine    ();
              EtesianEngine*        getForFramework ( unsigned int flags );
      static  GraphicEtesianEngine* grab            ();
      virtual const Name&           getName         () const;
              Cell*                 getCell         ();
      virtual size_t                release         ();
      virtual void                  addToMenu       ( CellViewer* );
              void                  postEvent       ();
                               
    protected:                 
      static  size_t                _references;
      static  GraphicEtesianEngine* _singleton;
              CellViewer*           _viewer;
    protected:
                                    GraphicEtesianEngine ();
      virtual                      ~GraphicEtesianEngine ();
              void                  _place               ();
  };


}  // Etesian namespace.

#endif  // ETESIAN_GRAPHIC_ETESIAN_ENGINE_H
