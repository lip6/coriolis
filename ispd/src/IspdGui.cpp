
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
// |    I s p d   G l o b a l   r o u t i n g  -  M a i n   G U I    |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :               Damien.Dupuis@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./IspdGui.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>

#include  "hurricane/Warning.h"
#include  "hurricane/viewer/CellWidget.h"

#include  "crlcore/Catalog.h"
#include  "crlcore/GraphicToolEngine.h"

#include  "IspdGui.h"


namespace Ispd {


  using Hurricane::Warning;
  using CRL::Catalog;


// -------------------------------------------------------------------
// Class  :  "IspdGui".


  IspdGui::IspdGui ( QWidget* parent )
    : CellViewer(parent)
    , _banner ( "Ispd"
              , "1.0a"
              , "Coriolis Ispd Main GUI"
              , "2008"
              , "Damien Dupuis"
              , ""
              )
    , _tools()
  { }


  IspdGui::~IspdGui ()
  { }


  IspdGui* IspdGui::create ( QWidget* parent )
  {
    IspdGui* ispdgui = new IspdGui ( parent );

    ispdgui->_postCreate ();

    return ispdgui;
  }


  void  IspdGui::destroy ()
  {
    _preDestroy ();
    delete this;
  }


  void  IspdGui::_postCreate ()
  {
    setApplicationName ( tr("ispd") );

    //QAction* openAction = findChild<QAction*>("viewer.menuBar.file.openCell");
    //if ( openAction ) {
    //  connect ( openAction, SIGNAL(triggered()), this, SLOT(openCell()) );
    //}
  }


  void  IspdGui::_preDestroy ()
  {
    set<GraphicTool*>::iterator itool = _tools.begin();
    for ( ; itool != _tools.end() ; itool++ )
      (*itool)->release ();
  }


  void  IspdGui::registerTool ( GraphicTool* tool )
  {
    assert ( tool != NULL );

    if ( _tools.find(tool) != _tools.end() ) {
      cerr << Warning ( "Tool %s already registered in Ispd (ignored)."
                     , getString(tool->getName()).c_str() ) << endl;
      return;
    }

    _tools.insert ( tool );

    const GraphicTool::DrawGoMap&          drawGos = tool->getDrawGos ();
    GraphicTool::DrawGoMap::const_iterator idrawGo = drawGos.begin();

    for ( ; idrawGo != drawGos.end() ; idrawGo++ )
      getCellWidget()->addDrawExtensionGo ( idrawGo->first
                                          , idrawGo->second.getInit()
                                          , idrawGo->second.getDraw()
                                          );

    tool->addToMenu ( this );
  }

} // End of Ispd namespace.
