// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ControllerWidget.cpp"                   |
// +-----------------------------------------------------------------+


#include <QSettings>
#include <QAction>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <hurricane/configuration/Configuration.h>
#include <hurricane/configuration/ConfigurationWidget.h>
#include <hurricane/DataBase.h>
#include <hurricane/Cell.h>
#include <hurricane/viewer/Graphics.h>
#include <hurricane/viewer/CellWidget.h>
#include <hurricane/viewer/GraphicsWidget.h>
#include <hurricane/viewer/PaletteWidget.h>
#include <hurricane/viewer/DisplayFilterWidget.h>
#include <hurricane/viewer/NetlistWidget.h>
#include <hurricane/viewer/HierarchyWidget.h>
#include <hurricane/viewer/SelectionWidget.h>
#include <hurricane/viewer/InspectorWidget.h>
#include <hurricane/viewer/ControllerWidget.h>


namespace Hurricane {

  using Cfg::Configuration;


// -------------------------------------------------------------------
// Class  :  "Hurricane::ControllerTab".


  ControllerTab::ControllerTab ( QWidget* parent )
    : QWidget(parent)
    , _cellWidget(NULL)
  { }


  void  ControllerTab::setCellWidget ( CellWidget* cellWidget )
  {
    _cellWidget = cellWidget;
  }


  void  ControllerTab::setCell ( Cell* )
  { }


  void  ControllerTab::updateTab ()
  { }


  void  ControllerTab::cellPreModificate ()
  { }


  void  ControllerTab::cellPostModificate ()
  { }


  void  ControllerTab::graphicsUpdated ()
  { }


  void  ControllerTab::readQtSettings ( size_t viewerId )
  { }
  

  void  ControllerTab::saveQtSettings ( size_t viewerId ) const
  { }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabGraphics".


  TabGraphics::TabGraphics ( QWidget* parent )
    : ControllerTab(parent)
    , _graphics    (new GraphicsWidget())
  {
    _graphics->setObjectName ( "controller.tabGraphics.graphics" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 0, 0, 0, 0 );
    wLayout->addWidget ( _graphics );
    setLayout ( wLayout );
  }


  void  TabGraphics::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      _graphics->setCellWidget ( cellWidget );
    }
  }


  void  TabGraphics::graphicsUpdated ()
  { _graphics->rereadGraphics(); }


  void  TabGraphics::readQtSettings ( size_t viewerId )
  { _graphics->readQtSettings( viewerId ); }
  

  void  TabGraphics::saveQtSettings ( size_t viewerId ) const
  {  _graphics->saveQtSettings( viewerId );}


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabDisplayFilter".


  TabDisplayFilter::TabDisplayFilter ( QWidget* parent )
    : ControllerTab (parent)
    , _displayFilter(new DisplayFilterWidget())
  {
    _displayFilter->setObjectName ( "controller.tabDisplayFilter.displayFilter" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 0, 0, 0, 0 );
    wLayout->addWidget ( _displayFilter );
    setLayout ( wLayout );
  }


  void  TabDisplayFilter::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      _displayFilter->setCellWidget ( cellWidget );
    }
  }


  void  TabDisplayFilter::readQtSettings ( size_t viewerId )
  { _displayFilter->readQtSettings( viewerId ); }
  

  void  TabDisplayFilter::saveQtSettings ( size_t viewerId ) const
  {  _displayFilter->saveQtSettings( viewerId );}


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabPalette".


  TabPalette::TabPalette ( QWidget* parent )
    : ControllerTab(parent)
    , _palette     (new PaletteWidget())
  {
  //_palette->setOneColumn  ();
    _palette->setObjectName ( "controller.tabPalette.palette" );
  //_palette->setSectionVisible ( "Viewer", false );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 0, 0, 0, 0 );
    wLayout->addWidget ( _palette );
    setLayout ( wLayout );
  }


  void  TabPalette::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() )
      getCellWidget()->detachFromPalette ();

    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      if ( getCellWidget() ) {
        getCellWidget()->bindToPalette ( _palette );
      }
    }
  }


  void  TabPalette::graphicsUpdated ()
  { _palette->rereadGraphics(); }


  void  TabPalette::readQtSettings ( size_t viewerId )
  { _palette->readQtSettings( viewerId ); }
  

  void  TabPalette::saveQtSettings ( size_t viewerId ) const
  {  _palette->saveQtSettings( viewerId );}


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabNetlist".


  TabNetlist::TabNetlist ( QWidget* parent )
    : ControllerTab         (parent)
    , _netlistBrowser       (new NetlistWidget())
    , _syncNetlist          (new QCheckBox())
    , _syncSelection        (new QCheckBox())
    , _cwCumulativeSelection(false)
  {
    _netlistBrowser->setObjectName ( "controller.tabNetlist.netlistBrowser" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 10, 0, 10, 0 );
    wLayout->setSpacing ( 0 );

    _syncNetlist->setText    ( tr("Sync Netlist") );
    _syncNetlist->setChecked ( false );
    _syncNetlist->setFont    ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect ( _syncNetlist, SIGNAL(toggled(bool)), this, SLOT(setSyncNetlist(bool)) );

    _syncSelection->setText    ( tr("Sync Selection") );
    _syncSelection->setChecked ( false );
    _syncSelection->setFont    ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect ( _syncSelection, SIGNAL(toggled(bool)), this, SLOT(setSyncSelection(bool)) );

    QHBoxLayout* commands = new QHBoxLayout ();
    commands->setContentsMargins ( 0, 0, 0, 0 );
    commands->addStretch ();
    commands->addWidget  ( _syncNetlist );
    commands->addStretch ();
    commands->addWidget  ( _syncSelection );
    commands->addStretch ();
    wLayout->addLayout   ( commands );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    wLayout->addWidget ( separator );
    wLayout->addWidget ( _netlistBrowser );

    setLayout ( wLayout );
  }


  void  TabNetlist::setSyncNetlist ( bool state )
  {
    if (state and getCellWidget()) {
      _netlistBrowser->setCell<SimpleNetInformations>( getCellWidget()->getCell() );
    } else {
      _netlistBrowser->setCell<SimpleNetInformations>( NULL );
    }
  }


  void  TabNetlist::setSyncSelection ( bool state )
  {
    if (state and getCellWidget() and _syncNetlist->isChecked()) {
      _cwCumulativeSelection = getCellWidget()->cumulativeSelection();
      if (not _cwCumulativeSelection) {
        getCellWidget()->openRefreshSession ();
        getCellWidget()->unselectAll ();
        getCellWidget()->closeRefreshSession ();
      }
      getCellWidget()->setShowSelection( true );
      connect( _netlistBrowser, SIGNAL(netSelected  (Occurrence)), getCellWidget(), SLOT(select  (Occurrence)) );
      connect( _netlistBrowser, SIGNAL(netUnselected(Occurrence)), getCellWidget(), SLOT(unselect(Occurrence)) );
      _netlistBrowser->updateSelecteds();
    } else {
      getCellWidget()->setShowSelection( false );
      getCellWidget()->setCumulativeSelection( _cwCumulativeSelection );
      _netlistBrowser->disconnect( getCellWidget(), SLOT(select  (Occurrence)) );
      _netlistBrowser->disconnect( getCellWidget(), SLOT(unselect(Occurrence)) );
    }
  }


  void  TabNetlist::setCell ( Cell* cell )
  {
    setSyncNetlist ( _syncNetlist->isChecked() );
  }


  void  TabNetlist::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      _netlistBrowser->setCellWidget<SimpleNetInformations> ( cellWidget );
      if ( getCellWidget() ) {
        connect ( getCellWidget(), SIGNAL(cellChanged(Cell*)), this, SLOT(setCell(Cell*)) );
      }
      setSyncNetlist ( _syncNetlist->isChecked() );
    }
  }


  void  TabNetlist::cellPreModificate ()
  {
    _netlistBrowser->setCell<SimpleNetInformations>( NULL );
  }


  void  TabNetlist::cellPostModificate ()
  {
    setSyncNetlist( _syncNetlist->isChecked() );
  }



  void  TabNetlist::readQtSettings ( size_t viewerId )
  { _netlistBrowser->readQtSettings( viewerId ); }
  

  void  TabNetlist::saveQtSettings ( size_t viewerId ) const
  {  _netlistBrowser->saveQtSettings( viewerId );}

// -------------------------------------------------------------------
// Class  :  "Hurricane::TabNetlist".


  TabHierarchy::TabHierarchy ( QWidget* parent )
    : ControllerTab         (parent)
    , _hierarchyBrowser     (new HierarchyWidget())
  {
    _hierarchyBrowser->setObjectName ( "controller.tabHierarchy.hierarchyBrowser" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 10, 0, 10, 0 );
    wLayout->setSpacing ( 0 );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    wLayout->addWidget ( separator );
    wLayout->addWidget ( _hierarchyBrowser );

    setLayout ( wLayout );
  }


  void  TabHierarchy::setCell ( Cell* cell )
  { }


  void  TabHierarchy::setCellWidget ( CellWidget* cellWidget )
  {
    if (getCellWidget() != cellWidget) {
      ControllerTab::setCellWidget( cellWidget );
      _hierarchyBrowser->setCellWidget( cellWidget );
      if (getCellWidget()) {
        connect( getCellWidget(), SIGNAL(cellChanged(Cell*)), this, SLOT(setCell(Cell*)) );
      }
    }
  }


  void  TabHierarchy::cellPreModificate ()
  { }


  void  TabHierarchy::cellPostModificate ()
  {
    CellWidget* cw      = getCellWidget();
    Cell*       topCell = cw->getTopCell();
    if (not topCell) topCell = cw->getCell();
    _hierarchyBrowser->setCell( topCell );

    if (not cw->getTopPath().isEmpty())
      _hierarchyBrowser->rexpand( cw->getTopPath() );
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabSelection".


  TabSelection::TabSelection ( QWidget* parent )
    : ControllerTab(parent)
    , _selection   (new SelectionWidget())
  {
    _selection->setObjectName ( "controller.tabSelection.selection" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 10, 10, 10, 0 );
    wLayout->addWidget ( _selection );
    setLayout ( wLayout );
  }


  void  TabSelection::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      _selection->setCellWidget ( cellWidget );
    }
  }


  void  TabSelection::updateTab ()
  {
//     if ( _cellWidget && _cellWidget->getCell() )
//       _selection->setSelection ( _cellWidget->getSelectorSet(), _cellWidget->getCell() );
//     else
//       _selection->setSelection ( set<Selector*>() );
  }


  void  TabSelection::cellPreModificate ()
  {
    _selection->clear ();
  }


  void  TabSelection::cellPostModificate ()
  {
  //updateTab ();
    if ( getCellWidget() && getCellWidget()->getCell() ) {
      _selection->setSelection ( getCellWidget()->getSelectorSet() );
    } else
      _selection->clear ();
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabInspector".


  TabInspector::TabInspector ( QWidget* parent )
    : ControllerTab       (parent)
    , _inspectorWidget    (new InspectorWidget())
    , _bookmarks          (new QComboBox())
    , _selectionOccurrence()
    , _updateFromSelection(true)
  {
    _inspectorWidget->setObjectName ( "controller.tabInspector.inspectorWidget" );

    connect ( _bookmarks, SIGNAL(currentIndexChanged(int)), this, SLOT(bookmarkChanged(int)) );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 10, 10, 10, 0 );
  //wLayout->setSpacing ( 0 );

    _bookmarks->addItem ( tr("<Disabled>" ) );
    _bookmarks->addItem ( tr("<DataBase>" ) );
    _bookmarks->addItem ( tr("<Cell None>") );
    _bookmarks->addItem ( tr("<Selection>") );

    QHBoxLayout* commands = new QHBoxLayout ();
    commands->setContentsMargins ( 0, 0, 0, 0 );
    commands->addStretch ();
    commands->addWidget  ( _bookmarks );
    commands->addStretch ();
    wLayout->addLayout   ( commands );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    wLayout->addWidget ( separator );
    wLayout->addWidget ( _inspectorWidget );

    setLayout ( wLayout );
  }


  void  TabInspector::setCell ( Cell* cell )
  {
    _bookmarks->setItemText ( 2, getString(cell).c_str() );
    if ( _bookmarks->currentIndex() == 2 )
      emit bookmarkChanged ( 2 );
  }


  void  TabInspector::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget( cellWidget );
      if ( getCellWidget() ) {
        connect ( getCellWidget(), SIGNAL(cellChanged(Cell*)), this, SLOT(setCell(Cell*)) );
      }
    }
  }


  void  TabInspector::bookmarkChanged ( int index )
  {
    switch ( index ) {
      case 0: _inspectorWidget->setRootRecord ( NULL ); break;
      case 1: _inspectorWidget->setRootRecord ( getRecord(DataBase::getDB()) ); break;
      case 2:
        if ( getCellWidget() && getCellWidget()->getCell() )
          _inspectorWidget->setRootRecord ( getRecord(getCellWidget()->getCell()) );
        break;
      case 3:
        if ( getCellWidget() && getCellWidget()->getCell() )
          _inspectorWidget->setRootRecord ( getRecord(_selectionOccurrence) );
        break;
    }
  }


  void  TabInspector::updateTab ()
  {
    if ( _updateFromSelection && (_bookmarks->currentIndex() == 3) ) {
      _inspectorWidget->setRootRecord ( getRecord(_selectionOccurrence) );
    }
    _updateFromSelection = false;
  }


  void  TabInspector::setSelectionOccurrence ( Occurrence& occurrence )
  {
    _updateFromSelection = true;
    _selectionOccurrence = occurrence;
  }


  void  TabInspector::cellPreModificate ()
  {
    _selectionOccurrence = Occurrence();
    if ( _bookmarks->currentIndex() > 1 )
      _inspectorWidget->setRootRecord ( NULL );
  }


  void  TabInspector::cellPostModificate ()
  {
    if ( ( _bookmarks->currentIndex() == 2 ) && getCellWidget() && getCellWidget()->getCell() )
      _inspectorWidget->setRootRecord ( getRecord(getCellWidget()->getCell()) );
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabSettings".


  TabSettings::TabSettings ( QWidget* parent )
    : ControllerTab (parent)
    , _timestamp    (0)
    , _configuration(Configuration::get()->buildWidget(ConfigurationWidget::Embedded))
  {
    setContentsMargins ( 5, 0, 5, 5 );

    _configuration->setObjectName ( "controller.tabSettings.settings" );

    QVBoxLayout* vLayout  = new QVBoxLayout ();
    vLayout->setContentsMargins ( 0, 0, 0, 0 );
    vLayout->addWidget ( _configuration );
    setLayout ( vLayout );
  }


  void  TabSettings::setCellWidget ( CellWidget* )
  { }


  void  TabSettings::showEvent ( QShowEvent* )
  {
    Configuration* conf = Configuration::get();

    if (_timestamp < conf->getLayout().getTimestamp()) {
      _timestamp = conf->getLayout().getTimestamp();

      QLayoutItem* child = NULL;
      while ( (child = layout()->takeAt(0)) != 0 ) {
        layout()->removeWidget( child->widget() );
        delete child->widget();
      }
      
      _configuration = conf->buildWidget( ConfigurationWidget::Embedded );
      layout()->addWidget ( _configuration );
    }
  }


  // TabSettings::TabSettings ( QWidget* parent )
  //   : ControllerTab(parent)
  //   , _settings(new QTabWidget())
  // { 
  //   setContentsMargins ( 5, 0, 5, 5 );

  //   _settings->setObjectName ( "controller.tabSettings.settings" );

  //   QVBoxLayout* vLayout  = new QVBoxLayout ();
  //   vLayout->setContentsMargins ( 0, 0, 0, 0 );
  //   vLayout->addWidget ( _settings );
  //   setLayout ( vLayout );
  // }


// -------------------------------------------------------------------
// Class  :  "Hurricane::ControllerWidget".


  ControllerWidget::ControllerWidget ( QWidget* parent )
    : QTabWidget       (parent)
    , _observer        (this)
    , _cellWidget      (NULL)
    , _tabGraphics     (new TabGraphics())
    , _tabPalette      (new TabPalette())
    , _tabDisplayFilter(new TabDisplayFilter())
    , _tabNetlist      (new TabNetlist())
    , _tabHierarchy    (new TabHierarchy())
    , _tabSelection    (new TabSelection())
    , _tabInspector    (new TabInspector())
    , _tabSettings     (new TabSettings())
  {
    setObjectName ( "controller" );
    setAttribute  ( Qt::WA_QuitOnClose, false );
    setWindowTitle( tr("Controller") );

  //connect ( _netlistBrowser, SIGNAL(destroyed()), this, SLOT(netlistBrowserDestroyed()) );

    _tabGraphics     ->setObjectName( "controller.graphics"      );
    _tabPalette      ->setObjectName( "controller.palette"       );
    _tabDisplayFilter->setObjectName( "controller.displayFilter" );
    _tabNetlist      ->setObjectName( "controller.tabNetlist"    );
    _tabHierarchy    ->setObjectName( "controller.tabHierarchy"  );
    _tabSelection    ->setObjectName( "controller.tabSelection"  );
    _tabInspector    ->setObjectName( "controller.tabInspector"  );
    _tabSettings     ->setObjectName( "controller.tabSettings"   );

    addTab( _tabGraphics      , "Look"        );
    addTab( _tabDisplayFilter , "Filter"      );
    addTab( _tabPalette       , "Layers&&Gos" );
    addTab( _tabHierarchy     , "Hierarchy"   );
    addTab( _tabNetlist       , "Netlist"     );
    addTab( _tabSelection     , "Selection"   );
    addTab( _tabInspector     , "Inspector"   );
    addTab( _tabSettings      , "Settings"    );

    QAction* toggleShow = new QAction ( tr("Controller"), this );
    toggleShow->setObjectName( "controller.action.hideShow" );
    toggleShow->setShortcut  ( QKeySequence(tr("CTRL+I")) );
    addAction( toggleShow );

    connect( toggleShow, SIGNAL(triggered())        , this, SLOT(toggleShow()) );
    connect( this      , SIGNAL(currentChanged(int)), this, SLOT(updateTab(int)) );
    connect( _tabSelection->getSelection(), SIGNAL(inspect(Occurrence&))
           , _tabInspector                , SLOT  (setSelectionOccurrence(Occurrence&)) );

    Graphics::getGraphics()->addObserver( &_observer );
                                        
    resize( Graphics::toHighDpi(1000), Graphics::toHighDpi(700) );
  }


  void  ControllerWidget::toggleShow ()
  { setVisible( !isVisible() ); }


  void  ControllerWidget::setCellWidget ( CellWidget* cellWidget )
  {
    _cellWidget = cellWidget;
    if ( _cellWidget ) {
      for ( int i=0 ; i<count() ; ++i )
        (static_cast<ControllerTab*>(widget(i)))->setCellWidget ( _cellWidget );

      connect ( _cellWidget, SIGNAL(cellChanged(Cell*))   , this, SLOT(cellChanged(Cell*))   );
      connect ( _cellWidget, SIGNAL(cellPreModificated()) , this, SLOT(cellPreModificate())  );
      connect ( _cellWidget, SIGNAL(cellPostModificated()), this, SLOT(cellPostModificate()) );
    }
  }


  void  ControllerWidget::cellChanged ( Cell* cell )
  { }


  void  ControllerWidget::updateTab ( int index )
  {
    (static_cast<ControllerTab*>(widget(index)))->updateTab ();
  }


  void  ControllerWidget::cellPreModificate ()
  {
    for ( int i=0 ; i<count() ; ++i )
      (static_cast<ControllerTab*>(widget(i)))->cellPreModificate ();
  }


  void  ControllerWidget::cellPostModificate ()
  {
    for ( int i=0 ; i<count() ; ++i )
      (static_cast<ControllerTab*>(widget(i)))->cellPostModificate ();
  }


  void  ControllerWidget::graphicsUpdated ()
  {
    for ( int i=0 ; i<count() ; ++i )
      (static_cast<ControllerTab*>(widget(i)))->graphicsUpdated ();
  }


  void  ControllerWidget::insertTabAfter ( const QString& ref, QWidget* tab, const QString& label )
  {
    for ( int itab=0 ; true; ++itab ) {
      QWidget* refTab = widget( itab );
      if (not refTab) break;
      if (refTab->objectName() != ref) continue;
      insertTab( itab, tab, label );
      return;
    }
    addTab( tab, label );
  }
  

  void  ControllerWidget::readQtSettings ( size_t viewerId )
  {
    QSettings settings;
    QString sizeKey = QString( "CellViewer/%1/controller/geometry" ).arg( viewerId );
    if (not settings.contains(sizeKey)) return;
    settings.beginGroup( QString("CellViewer/%1/controller").arg(viewerId) );
    restoreGeometry( settings.value( "geometry" ).toByteArray() );
    _tabGraphics     ->readQtSettings( viewerId );
    _tabPalette      ->readQtSettings( viewerId );
    _tabDisplayFilter->readQtSettings( viewerId );
    _tabNetlist      ->readQtSettings( viewerId );
    _tabHierarchy    ->readQtSettings( viewerId );
    _tabSelection    ->readQtSettings( viewerId );
    _tabInspector    ->readQtSettings( viewerId );
    _tabSettings     ->readQtSettings( viewerId );
    settings.endGroup();
  }
  

  void  ControllerWidget::saveQtSettings ( size_t viewerId ) const
  {
    QSettings settings;
    settings.beginGroup( QString("CellViewer/%1/controller").arg(viewerId) );
    settings.setValue( "geometry", saveGeometry() );
    _tabGraphics     ->saveQtSettings( viewerId );
    _tabPalette      ->saveQtSettings( viewerId );
    _tabDisplayFilter->saveQtSettings( viewerId );
    _tabNetlist      ->saveQtSettings( viewerId );
    _tabHierarchy    ->saveQtSettings( viewerId );
    _tabSelection    ->saveQtSettings( viewerId );
    _tabInspector    ->saveQtSettings( viewerId );
    _tabSettings     ->saveQtSettings( viewerId );
    settings.endGroup();
  }


// -------------------------------------------------------------------
// Class  :  "ControllerWidget::GraphicsObserver".

  void  ControllerWidget::notify ( ControllerWidget* controller, unsigned int flags )
  {
    cdebug.log(19) << "ControllerWidget::notify()" << endl;
    if (flags & Graphics::ChangedDisplayStyles) {
      controller->graphicsUpdated();
    }
  }

}  // End of Hurricane namespace.
