// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./UnicornGui.cpp"                              |
// +-----------------------------------------------------------------+


#include <Python.h>
#include <QAction>
#include <QMenu>
#include "hurricane/Warning.h"
#include "hurricane/viewer/Script.h"
#include "hurricane/viewer/CellViewer.h"
#include "hurricane/viewer/PyCellViewer.h"
#include "crlcore/Utilities.h"
#include "crlcore/Catalog.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/GraphicToolEngine.h"
#include "crlcore/AcmSigda.h"
#include "crlcore/Ispd04Bookshelf.h"
#include "crlcore/Ispd05Bookshelf.h"
#include "crlcore/Iccad04Lefdef.h"
#include "crlcore/DefImport.h"
#include "crlcore/DefExport.h"
#include "crlcore/GdsDriver.h"
#include "unicorn/ImportCell.h"
#include "unicorn/OpenCellDialog.h"
#include "unicorn/SaveCellDialog.h"
#include "unicorn/ImportCellDialog.h"
#include "unicorn/ExportCellDialog.h"
#include "unicorn/UnicornGui.h"


namespace Unicorn {

  using Hurricane::dbo_ptr;
  using Hurricane::Warning;
  using Hurricane::PyCellViewer_Link;
  using CRL::System;
  using CRL::Catalog;
  using CRL::AllianceFramework;
  using CRL::AcmSigda;
  using CRL::Ispd04;
  using CRL::Ispd05;
  using CRL::Iccad04Lefdef;
  using CRL::DefImport;
  using CRL::DefExport;
  using CRL::GdsDriver;


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
    : CellViewer      (parent)
    , _tools          ()
    , _importCell     ()
    , _importDialog   (new ImportCellDialog(this))
    , _exportDialog   (new ExportCellDialog(this))
  {
    addMenu  ( "placeAndRoute"           , "P&&R"          , CellViewer::TopMenu );
    addMenu  ( "placeAndRoute.stepByStep", "&Step by Step" );
    addToMenu( "placeAndRoute.========" );

    _runUnicornInit();

    _importCell.setDialog( _importDialog );
    _importCell.addImporter( "ACM/SIGDA (aka MCNC, .bench)", std::bind( &AcmSigda::load     , placeholders::_1 ) );
    _importCell.addImporter( "ISPD'04 (Bookshelf)"         , std::bind( &Ispd04::load       , placeholders::_1 ) );
    _importCell.addImporter( "ISPD'05 (Bookshelf)"         , std::bind( &Ispd05::load       , placeholders::_1 ) );
    _importCell.addImporter( "ICCAD'04 (LEF/DEF)"          , std::bind( &Iccad04Lefdef::load, placeholders::_1, 0 ) );
    _importCell.addImporter( "Alliance compliant DEF"      , std::bind( &DefImport::load    , placeholders::_1, DefImport::FitAbOnCells) );
  }


  UnicornGui::~UnicornGui ()
  { }


  void  UnicornGui::_runUnicornInit ()
  {
    Utilities::Path pythonSitePackages = System::getPath("pythonSitePackages");
    Utilities::Path systemConfDir      = pythonSitePackages / "unicorn";
    Utilities::Path systemConfFile     = systemConfDir      / "unicornInit.py";

    if (systemConfFile.exists()) {
      Isobar::Script::addPath( systemConfDir.string() );

      dbo_ptr<Isobar::Script> script = Isobar::Script::create( systemConfFile.stem().string() );
      script->addKwArgument( "editor"          , (PyObject*)PyCellViewer_Link(this) );
      script->runFunction  ( "unicornConfigure", getCell() );

      Isobar::Script::removePath( systemConfDir.string() );
    } else {
      cerr << Warning("Unicorn system configuration file:\n  <%s> not found."
                     ,systemConfFile.string().c_str()) << endl;
    }
  }


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

    QAction* importAction = findChild<QAction*>("viewer.menuBar.file.importCell");
    if ( importAction ) {
      connect ( importAction, SIGNAL(triggered()), this, SLOT(importCell()) );
    }

    QAction* exportAction = findChild<QAction*>("viewer.menuBar.file.exportCell");
    if ( exportAction ) {
      connect ( exportAction, SIGNAL(triggered()), this, SLOT(exportCell()) );
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


  void  UnicornGui::importCell ()
  {
    QString cellName;
    bool    newViewer;
    int     format;

    if ( _importDialog->runDialog( cellName, format, newViewer ) ) {
      Cell* cell = _importCell.load( cellName.toStdString(), format ); 

      if (cell) {
        UnicornGui* viewer = this;
        if (newViewer) {
          viewer = UnicornGui::create();
          viewer->show();
        }
        viewer->setCell( cell );
      }
    }
  }


  void  UnicornGui::exportCell ()
  {
    Cell* cell = getCell();
    if ( cell == NULL ) return;

    QString cellName= getString(cell->getName()).c_str();
    int     format;

    if ( _exportDialog->runDialog ( cellName, format ) ) {
      renameCell ( cellName.toStdString().c_str() );
      switch ( format ) {
        // case ImportCellDialog::AcmSigda:
        //   break;
        // case ImportCellDialog::Ispd04:
        //   break;
        // case ImportCellDialog::Iccad04:
        //   break;
        case ExportCellDialog::AllianceDef:
          DefExport::drive ( cell, DefExport::WithLEF );
          break;
        case ExportCellDialog::AsciiGds:
          GdsDriver gdsDriver ( cell );
          gdsDriver.save( getString(cell->getName())+".agds" );
          break;
      }
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


  string  UnicornGui::_getString () const
  {
    ostringstream s;
    s << "<UnicornGui ";
    Cell* cell = getCell();
    if (cell) s << getString(cell->getName());
    else      s << "No_Cell_Loaded";
    s << ">";
    return s.str();
  }


} // End of Unicorn namespace.


//INSPECTOR_P_SUPPORT(Unicorn::UnicornGui)
