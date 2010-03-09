
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
// |            E q u i n o x   -   E x t r a c t o r                |
// |                                                                 |
// |  Author      :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./equinox/GraphicEquinoxEngine.h"         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __EQUINOX_GRAPHIC_EQUINOX_ENGINE__
#define  __EQUINOX_GRAPHIC_EQUINOX_ENGINE__

#include  <QObject>



#include  "crlcore/GraphicToolEngine.h"


namespace Hurricane {
  class Go;
  class BasicLayer;
  class Transformation;
  class CellWidget;
  class CellViewer;
}


namespace Equinox {



  using Hurricane::Cell;
  using Hurricane::Go;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::Transformation;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using CRL::GraphicTool;
  using CRL::Name;

  class EquinoxEngine;
  class Sweepline;


// -------------------------------------------------------------------
// Class  :  "Equinox::GraphicEquinoxEngine".


  class GraphicEquinoxEngine : public GraphicTool {

    Q_OBJECT;
    
  public:
    inline 				   
    /**/           EquinoxEngine*          createEngine              ();
    /**/   static  GraphicEquinoxEngine*   grab                      ();
    /**/   virtual const Name&             getName                   () const;
    /**/           Cell*                   getCell                   ();
    /**/   virtual size_t                  release                   ();
    /**/   virtual void                    addToMenu                 ( CellViewer* );
					   				    
  public slots:				   
    /**/           void                    runWithAlim               ();
    /**/           void                    runWithoutAlim            ();
    					   
  private:				   
    /**/   static  size_t                 _references;
    /**/   static  GraphicEquinoxEngine*  _singleton;
    /**/           CellViewer*            _viewer;
  protected:
    /**/                                   GraphicEquinoxEngine      ();
    /**/   virtual                        ~GraphicEquinoxEngine      ();
  };
  
  
  
} // End of Equinox namespace.


#endif  // __EQUINOX_GRAPHIC_EQUINOX_ENGINE__
