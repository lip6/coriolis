
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
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicSolsticeEngine.h"              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __SOLSTICE_GRAPHIC_SOLSTICE_ENGINE__
#define  __SOLSTICE_GRAPHIC_SOLSTICE_ENGINE__


#include  <QObject>



#include  "crlcore/GraphicToolEngine.h"
// #include "solstice/TabRoutingError.h"

namespace Hurricane {
  class Go;
  class BasicLayer;
  class Transformation;
  class CellWidget;
  class CellViewer;
}



namespace Solstice {
  
  
  using Hurricane::Cell;
  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
  using CRL::Name;
  
  class SolsticeEngine;
  class TabRoutingErrorList;
  
  
  // -------------------------------------------------------------------
  // Class  :  "Solstice::GraphicSolsticeEngine".
  
  
  class GraphicSolsticeEngine : public GraphicTool {
    Q_OBJECT;
    
  public:
    
    /**/   static   void                   initSolsticeRoutingError ( CellWidget* );
    /**/   static   void                   drawSolsticeRoutingError ( CellWidget*
								      , const Go*
								      , const BasicLayer*
								      , const Box&
								      , const Transformation&
								      );


    /**/            SolsticeEngine*        createEngine             ();
    /**/    static  GraphicSolsticeEngine* grab                     ();
    /**/    virtual const Name&            getName                  () const;
    /**/            Cell*                  getCell                  ();
    /**/    virtual size_t                 release                  ();
    /**/    virtual void                   addToMenu                ( CellViewer* );


								  
  public slots:
    /**/    void                  test             ();
    
    
    
  private:
    static  size_t                _references;
    static  GraphicSolsticeEngine*_singleton;
    /**/    CellViewer*           _viewer;
    /**/    TabRoutingErrorList*      _tabRoutingErrorList;
  protected:
    /**/                          GraphicSolsticeEngine ();
    virtual                      ~GraphicSolsticeEngine ();
    
  }; //end of class GraphicSolsticeEngine

}  // End of Solstice namespace.



#endif  // _SOLSTICE__GRAPHIC_SOLSTICE_ENGINE__
