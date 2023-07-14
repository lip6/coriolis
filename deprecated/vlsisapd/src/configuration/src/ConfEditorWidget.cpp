
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2010-2011, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ConfEditorWidget.cpp"                   |
// +-----------------------------------------------------------------+


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>
#include  <QApplication>
#include  "vlsisapd/configuration/Configuration.h"
#include  "vlsisapd/configuration/ConfigurationWidget.h"
#include  "vlsisapd/configuration/ConfEditorWidget.h"


namespace Cfg {

  using namespace std;


  ConfEditorWidget::ConfEditorWidget ( QWidget* parent )
    : QMainWindow             (parent)
    , _configurationWidget    (Configuration::get()->buildWidget(ConfigurationWidget::Embedded))
    , _fileMenu               (NULL)
    , _saveAction             (NULL)
    , _quitAction             (NULL)
  {
    setObjectName("confEditor");

    createMenus ();

    setCentralWidget ( _configurationWidget );
  }


  void  ConfEditorWidget::createActions ()
  {
    if ( _quitAction != NULL ) return;

    _quitAction = new QAction  ( tr("&Quit"), this );
    _quitAction->setObjectName ( "confEditor.menuBar.file.quit" );
    _quitAction->setStatusTip  ( tr("Quit the VLSISAPD Configuration Editor") );
    _quitAction->setShortcut   ( QKeySequence(tr("CTRL+Q")) );
    connect ( _quitAction, SIGNAL(triggered()), this, SLOT(close()) );

    _saveAction = new QAction  ( tr("&Save"), this );
    _saveAction->setObjectName ( "confEditor.menuBar.file.save" );
    _saveAction->setStatusTip  ( tr("Save Settings in ./coriolis2.configuration.xml") );
    _saveAction->setShortcut   ( QKeySequence(tr("CTRL+S")) );
    connect ( _saveAction, SIGNAL(triggered()), this, SLOT(save()) );
  }


  void  ConfEditorWidget::createMenus ()
  {
    if ( _fileMenu   != NULL ) return;
    if ( _quitAction == NULL ) createActions ();

    menuBar()->setObjectName ( tr("confEditor.menuBar") );

    _fileMenu = menuBar()->addMenu ( tr("File") );
    _fileMenu->addAction ( _saveAction );
    _fileMenu->addAction ( _quitAction );
  }


  void  ConfEditorWidget::save ()
  {
    string   dotConfigFile = "./.coriolis2.configuration.xml";
    ofstream file          ( dotConfigFile.c_str() );

    cout << "Saving configuration file: <" << dotConfigFile << ">."<< endl;

  //Configuration::get()->writeToStream ( file, 0, ";misc;kite;;mauka;;" );
  //Configuration::get()->writeToStream ( file, Configuration::DriveValues|Configuration::DriveLayout, "misc" );
    Configuration::get()->writeToStream ( file, 0 );
  //Configuration::get()->writeToStream ( file, Configuration::DriveValues|Configuration::DriveLayout );
    file.close ();
  }


}  // End of Cfg namespace.
