// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
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


#include  <QAction>
#include  <QMenu>

#include  "hurricane/Warning.h"
#include  "hurricane/viewer/CellWidget.h"

#include  "crlcore/Catalog.h"
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/GraphicToolEngine.h"
#include  "crlcore/AcmSigda.h"
#include  "crlcore/Ispd04Bookshelf.h"
#include  "crlcore/Ispd05Bookshelf.h"
#include  "crlcore/Iccad04Lefdef.h"
#include  "crlcore/DefImport.h"
#include  "crlcore/DefExport.h"

#include  "unicorn/OpenCellDialog.h"
#include  "unicorn/SaveCellDialog.h"
#include  "unicorn/ImportCellDialog.h"
#include  "unicorn/ExportCellDialog.h"
#include  "unicorn/UnicornGui.h"


namespace Unicorn {


  using Hurricane::Warning;
  using CRL::Catalog;
  using CRL::AllianceFramework;
  using CRL::AcmSigda;
  using CRL::Ispd04;
  using CRL::Ispd05;
  using CRL::Iccad04Lefdef;
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
    : CellViewer   (parent)
    , _tools       ()
    , _importDialog(new ImportCellDialog(this))
    , _exportDialog(new ExportCellDialog(this))
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

    if ( _importDialog->runDialog ( cellName, format, newViewer ) ) {
      Cell* cell = NULL; 

      switch ( format ) {
        case ImportCellDialog::AcmSigda:
          cell = AcmSigda::load ( cellName.toStdString() );
          break;
        case ImportCellDialog::Ispd04:
          cell = Ispd04::load ( cellName.toStdString() );
          break;
        case ImportCellDialog::Ispd05:
          cell = Ispd05::load ( cellName.toStdString() );
          cerr << "Cell " << " successfully loaded." << endl;
          break;
        case ImportCellDialog::Iccad04:
          cell = Iccad04Lefdef::load ( cellName.toStdString() , 0 );
          break;
        case ImportCellDialog::AllianceDef:
          cell = DefImport::load ( cellName.toStdString().c_str() , DefImport::FitAbOnCells );
          break;
      }

      if ( cell ) {
        UnicornGui* viewer = this;
        if ( newViewer ) {
          viewer = UnicornGui::create ();
          viewer->show ();
        }
        cerr << "Loading " << cell->getName() << " into the viewer." << endl;
        viewer->setCell ( cell );
      } else
        cerr << "[ERROR] Cell not found: " << cellName.toStdString() << endl;
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
