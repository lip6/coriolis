// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
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
#include "hurricane/DebugSession.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/isobar/Script.h"
#include "hurricane/viewer/CellViewer.h"
#include "hurricane/isobar/PyCellViewer.h"
#include "crlcore/Utilities.h"
#include "crlcore/Catalog.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/LibraryManager.h"
#include "crlcore/GraphicToolEngine.h"
#include "crlcore/AcmSigda.h"
//#include "crlcore/Ispd04Bookshelf.h"
//#include "crlcore/Ispd05Bookshelf.h"
#include "crlcore/Gds.h"
#include "crlcore/Blif.h"
#include "crlcore/Iccad04Lefdef.h"
#include "crlcore/LefImport.h"
#include "crlcore/DefImport.h"
#include "crlcore/DefExport.h"
#include "unicorn/ImportCell.h"
#include "unicorn/OpenCellDialog.h"
#include "unicorn/SaveCellDialog.h"
#include "unicorn/ImportCellDialog.h"
#include "unicorn/ExportCellDialog.h"
#include "unicorn/UnicornGui.h"


namespace Unicorn {

  using Hurricane::dbo_ptr;
  using Hurricane::DebugSession;
  using Hurricane::Warning;
  using Hurricane::PyCellViewer_Link;
  using Hurricane::jsonCellParse;
  using Hurricane::DataBase;
  using Hurricane::Library;
  using CRL::System;
  using CRL::Catalog;
  using CRL::AllianceFramework;
  using CRL::LibraryManager;
  using CRL::AcmSigda;
//using CRL::Ispd04;
//using CRL::Ispd05;
  using CRL::Gds;
  using CRL::Blif;
  using CRL::Iccad04Lefdef;
  using CRL::LefImport;
  using CRL::DefImport;
  using CRL::DefExport;


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
    : CellViewer     (parent)
    , _tools         ()
    , _importCell    ()
    , _importDialog  (new ImportCellDialog(this))
    , _exportDialog  (new ExportCellDialog(this))
    , _libraryManager(new LibraryManager  ())
  {
    addMenu  ( "placeAndRoute"           , "P&&R"          , CellViewer::TopMenu );
    addMenu  ( "placeAndRoute.stepByStep", "&Step by Step" );
    addToMenu( "placeAndRoute.========" );

    _runUnicornInit();

    _importCell.setDialog( _importDialog );
    _importCell.addImporter<Cell*>   ( "JSON (experimental)"         , std::bind( &Cell::fromJson     , placeholders::_1 ) );
    _importCell.addImporter<Cell*>   ( "BLIF (Yosys/ABC)"            , std::bind( &Blif::load         , placeholders::_1, false ) );
    _importCell.addImporter<Cell*>   ( "ACM/SIGDA (aka MCNC, .bench)", std::bind( &AcmSigda::load     , placeholders::_1 ) );
    /* Disabled because this is never the one you want
    _importCell.addImporter<Cell*>   ( "ISPD'04 (Bookshelf)"         , std::bind( &Ispd04::load       , placeholders::_1 ) );
    _importCell.addImporter<Cell*>   ( "ISPD'05 (Bookshelf)"         , std::bind( &Ispd05::load       , placeholders::_1 ) );
    */
    _importCell.addImporter<Cell*>   ( "ICCAD'04 (LEF/DEF)"          , std::bind( &Iccad04Lefdef::load, placeholders::_1, 0 ) );
    _importCell.addImporter<Cell*>   ( "Alliance compliant DEF"      , std::bind( &DefImport::load    , placeholders::_1, DefImport::FitAbOnCells) );
    _importCell.addImporter<Library*>( "Cadence LEF"                 , std::bind( &LefImport::load    , placeholders::_1 ) );

    _libraryManager->setCellViewer( this );
  }


  UnicornGui::~UnicornGui ()
  { }


  void  UnicornGui::_runUnicornInit ()
  {
    Utilities::Path pythonSitePackages = System::getPath("pythonSitePackages");
    Utilities::Path confFile           = "coriolis/unicorn/initHook.py";
    Utilities::Path systemConfFile     = pythonSitePackages / confFile;

    if (systemConfFile.exists()) {
    //Isobar::Script::addPath( systemConfDir.toString() );

      dbo_ptr<Isobar::Script> script = Isobar::Script::create( confFile.toPyModPath() );
      script->addKwArgument( "editor"          , (PyObject*)PyCellViewer_Link(this) );
      script->runFunction  ( "unicornConfigure", getCell() );

    //Isobar::Script::removePath( systemConfDir.toString() );
    } else {
      cerr << Warning("Unicorn system configuration file:\n  <%s> not found."
                     ,systemConfFile.toString().c_str()) << endl;
    }
  }


  UnicornGui* UnicornGui::create ( QWidget* parent )
  {
    UnicornGui* unicorn = new UnicornGui ( parent );

    unicorn->_postCreate ();

    return unicorn;
  }


  UnicornGui* UnicornGui::vcreate () const
  { return UnicornGui::create(); }


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

    QAction* action = addToMenu( "tools.libraryManager"
                               , tr("Library Manager")
                               , tr("Browse through Views, Cells & Libraries")
                               , QKeySequence(tr("CTRL+M"))
                               , QIcon()
                               , "tools.script"
                               );
    connect( action, SIGNAL(triggered()), _libraryManager, SLOT(toggleShow()) );
    connect( this  , SIGNAL(cellLoadedFromDisk(Cell*)), _libraryManager, SLOT(updateLibrary(Cell*)) );
  }


  void  UnicornGui::_preDestroy ()
  {
    set<GraphicTool*>::iterator itool = _tools.begin();
    for ( ; itool != _tools.end() ; itool++ )
      (*itool)->release ();
  }


  Cell* UnicornGui::getCellFromDb ( const char* name )
  {
    Cell* cell = DataBase::getDB()->getCell( name );
    if (cell) return cell;

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
        emit cellLoadedFromDisk ( cell );
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
      if (_importCell.load( cellName.toStdString(), format )) {
        Cell* cell = _importCell.getLoaded<Cell*>();
        if (cell) {
          UnicornGui* viewer = this;
          if (newViewer) {
            viewer = UnicornGui::create();
            viewer->show();
          }
          viewer->setCell( cell );
          emit cellLoadedFromDisk ( cell );
        }
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
        case ExportCellDialog::Gds:
          Gds::save( cell );
          break;
        case ExportCellDialog::Json:
          { //DebugSession::open( 19, 20 );
            cerr << "cdebug (" << cdebug.getMinLevel() << "," << cdebug.getMaxLevel() 
                 << ") level:" << cdebug.getLevel() << " enabled:" << cdebug.enabled() << endl;

            JsonWriter writer ( cellName.toStdString()+".json.bz2" );
            jsonWrite( &writer, cell );
          //DebugSession::close();
          }
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
