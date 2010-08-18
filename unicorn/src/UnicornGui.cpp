
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./UnicornGui.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>

#include  "hurricane/Warning.h"
#include  "hurricane/viewer/CellWidget.h"

#include  "crlcore/Catalog.h"
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/GraphicToolEngine.h"

#include  "unicorn/OpenCellDialog.h"
#include  "unicorn/SaveCellDialog.h"
#include  "unicorn/UnicornGui.h"


namespace Unicorn {


  using Hurricane::Warning;
  using CRL::Catalog;
  using CRL::AllianceFramework;


// -------------------------------------------------------------------
// Class  :  "UnicornGui".


  Banner  UnicornGui::_banner ( "Unicorn"
                              , "1.0b"
                              , "Coriolis Main GUI"
                              , "2008"
                              , "Jean-Paul Chaput"
                              , ""
                              );


  UnicornGui::UnicornGui ( QWidget* parent )
    : CellViewer(parent)
    , _tools    ()
  { }


  UnicornGui::~UnicornGui ()
  { }


  UnicornGui* UnicornGui::create ( QWidget* parent )
  {
    UnicornGui* unicorn = new UnicornGui ( parent );

    unicorn->_postCreate ();

    return unicorn;
  }


  void  UnicornGui::destroy ()
  {
    _preDestroy ();
    delete this;
  }


  void  UnicornGui::_postCreate ()
  {
    setApplicationName ( tr("unicorn") );
    setWindowIcon      ( QIcon(":/images/diet-coke.png") );

    QAction* openAction = findChild<QAction*>("viewer.menuBar.file.openCell");
    if ( openAction ) {
      connect ( openAction, SIGNAL(triggered()), this, SLOT(openCell()) );
    }

    QAction* saveAction = findChild<QAction*>("viewer.menuBar.file.saveCell");
    if ( saveAction ) {
      saveAction->setVisible ( true );
      connect ( saveAction, SIGNAL(triggered()), this, SLOT(saveCell()) );
    }
  }


  void  UnicornGui::_preDestroy ()
  {
    set<GraphicTool*>::iterator itool = _tools.begin();
    for ( ; itool != _tools.end() ; itool++ )
      (*itool)->release ();
  }


  Cell* UnicornGui::getCellFromDb ( const char* name )
  {
    return AllianceFramework::get()->getCell ( name, Catalog::State::Views );
  }


  void  UnicornGui::registerTool ( GraphicTool* tool )
  {
    assert ( tool != NULL );

    if ( _tools.find(tool) != _tools.end() ) {
      cerr << Warning ( "Tool %s already registered in Unicorn (ignored)."
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


  void  UnicornGui::openCell ()
  {
    UnicornGui* viewer   = this;
    QString     cellName;
    bool        newViewer;

    if ( OpenCellDialog::runDialog ( this, cellName, newViewer ) ) {
      Cell* cell = getCellFromDb ( cellName.toStdString().c_str() );
      if ( cell ) {
        if ( newViewer ) {
          viewer = UnicornGui::create ();
          viewer->show ();
        }
        viewer->setCell ( cell );
      } else
        cerr << "[ERROR] Cell not found: " << cellName.toStdString() << endl;
    }
  }


  void  UnicornGui::saveCell ()
  {
    Cell* cell = getCell();
    if ( cell == NULL ) return;

    QString cellName = getString(cell->getName()).c_str();

    if ( SaveCellDialog::runDialog ( this, cellName ) ) {
      renameCell ( cellName.toStdString().c_str() );
      AllianceFramework::get()->saveCell ( cell, Catalog::State::Views );
    }
  }


} // End of Unicorn namespace.
