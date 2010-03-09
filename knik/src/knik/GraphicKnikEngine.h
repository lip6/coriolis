
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |               K n i k - Global Router                           |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicKnikEngine.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KNIK_GRAPHIC_KNIK_ENGINE__
#define  __KNIK_GRAPHIC_KNIK_ENGINE__

#include  <QObject>

namespace Hurricane {
  class Go;
  class BasicLayer;
  class Transformation;
  class CellWidget;
  class CellViewer;
}

#include  "crlcore/GraphicToolEngine.h"
#include  "knik/KnikEngine.h"

namespace Knik {


  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
    

// -------------------------------------------------------------------
// Class  :  "CRL::GraphicKnikEngine".


  class GraphicKnikEngine : public GraphicTool {
      Q_OBJECT;

    public:
              KnikEngine*        createEngine     ( unsigned int mode );
      static  void               initKnikEdges    ( CellWidget* );
      static  void               initKnikVertex   ( CellWidget* );
      static  void               drawKnikEdges    ( CellWidget*, const Go*, const BasicLayer*, const Box&, const Transformation& );
      static  void               drawKnikVertex   ( CellWidget*, const Go*, const BasicLayer*, const Box&, const Transformation& );
      static  GraphicKnikEngine* grab             ();
      virtual const Name&        getName          () const;
              Cell*              getCell          ();
      virtual size_t             release          ();
      virtual void               addToMenu        ( CellViewer* );
      inline  void               setCongestion    ( unsigned int );
      inline  void               setPreCongestion ( unsigned int );
      inline  void               setBenchMode     ( bool );
      inline  void               setUseSegments   ( bool );
      inline  void               setEdgeCost      ( float );
    public slots:
              void               run              ();
              void               flattenNets      ();
              void               route            ();
              bool               analyse          ();
              void               unroute          ();
              void               reroute          ();
              void               saveSolution     ();
              void               loadSolution     ();

    private:
      enum CreateMode { BuildSolution = 1
                      , LoadSolution
                      };

    private:
      static size_t              _references;
      static GraphicKnikEngine*  _singleton;
             CellViewer*         _viewer;
             unsigned int        _congestion;
             unsigned int        _preCongestion;
             bool                _benchMode;
             bool                _useSegments;
             float               _edgeCost;
    protected:
                                 GraphicKnikEngine ();
      virtual                   ~GraphicKnikEngine ();
  };


// Inline Functions.
  inline  void  GraphicKnikEngine::setCongestion    ( unsigned int congestion ) { _congestion = congestion; }
  inline  void  GraphicKnikEngine::setPreCongestion ( unsigned int preCongestion ) { _preCongestion = preCongestion; }
  inline  void  GraphicKnikEngine::setBenchMode     ( bool state ) { _benchMode = state; }
  inline  void  GraphicKnikEngine::setUseSegments   ( bool state ) { _useSegments = state; }
  inline  void  GraphicKnikEngine::setEdgeCost      ( float cost ) { _edgeCost = cost; }


} // End of Knik namespace.


#endif  // __KNIK_GRAPHIC_KNIK_ENGINE__
