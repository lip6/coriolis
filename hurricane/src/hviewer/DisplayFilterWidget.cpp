
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
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./DisplayFilterWidget.cpp"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QLabel>
#include  <QCheckBox>
#include  <QSpinBox>
#include  <QGroupBox>
#include  <QButtonGroup>
#include  <QRadioButton>
#include  <QGridLayout>
#include  <QVBoxLayout>

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/DisplayFilterWidget.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "DisplayFilterWidget".


  DisplayFilterWidget::DisplayFilterWidget ( QWidget* parent )
    : QWidget         (parent)
    , _cellWidget     (NULL)
    , _startSpinBox   (new QSpinBox())
    , _stopSpinBox    (new QSpinBox())
    , _doMasterCells  (new QCheckBox())
    , _doTerminalCells(new QCheckBox())
    , _doComponents   (new QCheckBox())
    , _steiner        (new QRadioButton())
    , _centric        (new QRadioButton())
    , _barycentric    (new QRadioButton())
    , _symbolicMode   (new QRadioButton())
    , _realMode       (new QRadioButton())
    , _updateState    (ExternalEmit)
  {
    setAttribute   ( Qt::WA_QuitOnClose, false );
    setWindowTitle ( tr("Display Filter") );
    setFont        ( Graphics::getNormalFont(true) );

    QGroupBox*    groupBox = new QGroupBox    ( tr("Hierarchy Settings") );
    QGridLayout*  gLayout  = new QGridLayout  ();
    QVBoxLayout*  wLayout  = new QVBoxLayout  ();

    QLabel* label = new QLabel ();
    label->setText ( "Hierarchy Start Level" );
    label->setFont ( Graphics::getNormalFont() );

    _startSpinBox->setFont ( Graphics::getNormalFont() );

    gLayout->addWidget ( label        , 0, 0 );
    gLayout->addWidget ( _startSpinBox, 0, 1 );

    label = new QLabel ();
    label->setText ( "Hierarchy Stop Level" );
    label->setFont ( Graphics::getNormalFont() );

    _stopSpinBox->setFont  ( Graphics::getNormalFont() );
    _stopSpinBox->setValue ( 100 );

    gLayout->addWidget ( label       , 1, 0 );
    gLayout->addWidget ( _stopSpinBox, 1, 1 );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    gLayout->addWidget ( separator, 2, 0, 1, 2 );

    _doMasterCells->setFont    ( Graphics::getNormalFont() );
    _doMasterCells->setText    ( tr("Process Master Cells") );
    _doMasterCells->setChecked ( true );

    gLayout->addWidget ( _doMasterCells, 3, 0, 1, 2 );
    connect ( _doMasterCells, SIGNAL(stateChanged(int)), this, SLOT(setDoMasterCells(int)) );

    _doTerminalCells->setFont    ( Graphics::getNormalFont() );
    _doTerminalCells->setText    ( tr("Process Terminal Cells") );
    _doTerminalCells->setChecked ( true );

    gLayout->addWidget ( _doTerminalCells, 4, 0, 1, 2 );
    connect ( _doTerminalCells, SIGNAL(stateChanged(int)), this, SLOT(setDoTerminalCells(int)) );

    _doComponents->setFont    ( Graphics::getNormalFont() );
    _doComponents->setText    ( tr("Process Components") );
    _doComponents->setChecked ( true );

    gLayout->addWidget ( _doComponents, 5, 0, 1, 2 );
    connect ( _doComponents, SIGNAL(stateChanged(int)), this, SLOT(setDoComponents(int)) );

    groupBox->setLayout ( gLayout );
    wLayout->addWidget  ( groupBox );

    groupBox = new QGroupBox ( tr("Rubbers") );
    QHBoxLayout*  hLayout = new QHBoxLayout  ();
    QButtonGroup* group   = new QButtonGroup ();

    hLayout->setContentsMargins (  5,  0,  5,  0 );

    _centric->setText  ( tr("Centric") );
    _centric->setFont  ( Graphics::getNormalFont() );
    group->setId       ( _centric, 0 );
    group->addButton   ( _centric );
    hLayout->addWidget ( _centric );

    _barycentric->setText ( tr("Barycentric") );
    _barycentric->setFont ( Graphics::getNormalFont() );
    group->setId          ( _barycentric, 1 );
    group->addButton      ( _barycentric );
    hLayout->addWidget    ( _barycentric );

    _steiner->setText  ( tr("Steiner") );
    _steiner->setFont  ( Graphics::getNormalFont() );
    group->setId       ( _steiner, 2 );
    group->addButton   ( _steiner );
    hLayout->addWidget ( _steiner );

    groupBox->setLayout ( hLayout );
    wLayout->addWidget  ( groupBox );

    groupBox = new QGroupBox ( tr("Layout Mode") );
    hLayout  = new QHBoxLayout  ();
    group    = new QButtonGroup ();

    hLayout->setContentsMargins (  5,  0,  5,  0 );

    _symbolicMode->setText  ( tr("Symbolic (lambda)") );
    _symbolicMode->setFont  ( Graphics::getNormalFont() );
    group->setId       ( _symbolicMode, 0 );
    group->addButton   ( _symbolicMode );
    hLayout->addWidget ( _symbolicMode );

    _realMode->setText  ( tr("Real (foundry grid)") );
    _realMode->setFont  ( Graphics::getNormalFont() );
    group->setId       ( _realMode, 0 );
    group->addButton   ( _realMode );
    hLayout->addWidget ( _realMode );

    groupBox->setLayout ( hLayout );
    wLayout->addWidget  ( groupBox );
    wLayout->addStretch ();

    setLayout ( wLayout );

    connect ( _startSpinBox, SIGNAL(valueChanged(int)), this, SLOT(startLevelChanged(int)) );
    connect ( _stopSpinBox , SIGNAL(valueChanged(int)), this, SLOT(stopLevelChanged (int)) );
    connect ( _steiner     , SIGNAL(clicked())        , this, SLOT(setRubberSteiner()) );
    connect ( _centric     , SIGNAL(clicked())        , this, SLOT(setRubberCentric()) );
    connect ( _barycentric , SIGNAL(clicked())        , this, SLOT(setRubberBarycentric()) );
    connect ( _symbolicMode, SIGNAL(clicked())        , this, SLOT(setSymbolicMode()) );
    connect ( _realMode    , SIGNAL(clicked())        , this, SLOT(setRealMode()) );
  }


  void  DisplayFilterWidget::setCellWidget ( CellWidget* cw )
  {
    if ( _cellWidget ) {
      disconnect ( this        , SIGNAL(queryFilterChanged()), _cellWidget, SLOT(changeQueryFilter()) );
      disconnect ( _cellWidget , SIGNAL(queryFilterChanged()), this       , SLOT(changeQueryFilter()) );
      disconnect ( this        , SIGNAL(layoutModeChanged ()), _cellWidget, SLOT(changeLayoutMode ()) );
      disconnect ( _cellWidget , SIGNAL(layoutModeChanged ()), this       , SLOT(changeLayoutMode ()) );
    }

    _cellWidget = cw;
    if ( !_cellWidget ) return;

    connect ( this        , SIGNAL(queryFilterChanged()), _cellWidget, SLOT(changeQueryFilter()) );
    connect ( _cellWidget , SIGNAL(queryFilterChanged()), this       , SLOT(changeQueryFilter()) );
    connect ( this        , SIGNAL(layoutModeChanged ()), _cellWidget, SLOT(changeLayoutMode ()) );
    connect ( _cellWidget , SIGNAL(layoutModeChanged ()), this       , SLOT(changeLayoutMode ()) );

    _updateState = ExternalEmit;
    changeQueryFilter ();
    changeLayoutMode ();
  }


  void  DisplayFilterWidget::changeLayoutMode ()
  {
    if ( !_cellWidget ) return;

    if ( _updateState == InternalEmit ) {
      _updateState = InternalReceive;
      emit layoutModeChanged ();
    } else {
      if ( _updateState == ExternalEmit ) {
        blockAllSignals ( true );

        if ( _cellWidget->symbolicMode() )
          _symbolicMode->setChecked(true);
        else
          _realMode->setChecked(true);
         
        blockAllSignals ( false );
      }
      _updateState = ExternalEmit;
    }
  }


  void  DisplayFilterWidget::changeQueryFilter ()
  {
    if ( !_cellWidget ) return;

    if ( _updateState == InternalEmit ) {
      _updateState = InternalReceive;
      emit queryFilterChanged ();
    } else {
      if ( _updateState == ExternalEmit ) {
        blockAllSignals ( true );

        _startSpinBox->setValue ( _cellWidget->getStartLevel() );
        _stopSpinBox ->setValue ( _cellWidget->getStopLevel()  );

        _doMasterCells  ->setChecked ( _cellWidget->getQueryFilter().isSet(Query::DoMasterCells  ) );
        _doTerminalCells->setChecked ( _cellWidget->getQueryFilter().isSet(Query::DoTerminalCells) );
        _doComponents   ->setChecked ( _cellWidget->getQueryFilter().isSet(Query::DoComponents   ) );

        switch ( _cellWidget->getRubberShape() ) {
          case CellWidget::Steiner:     _steiner->setChecked(true); break;
          case CellWidget::Centric:     _centric->setChecked(true); break;
          case CellWidget::Barycentric: _barycentric->setChecked(true); break;
        }
         
        blockAllSignals ( false );
      }
      _updateState = ExternalEmit;
    }
  }


  void  DisplayFilterWidget::blockAllSignals ( bool state )
  {
    _startSpinBox   ->blockSignals ( state );
    _stopSpinBox    ->blockSignals ( state );
    _doMasterCells  ->blockSignals ( state );
    _doTerminalCells->blockSignals ( state );
    _doComponents   ->blockSignals ( state );
    _steiner        ->blockSignals ( state );
    _centric        ->blockSignals ( state );
    _barycentric    ->blockSignals ( state );
    _symbolicMode   ->blockSignals ( state );
    _realMode       ->blockSignals ( state );
  }


  void  DisplayFilterWidget::startLevelChanged ( int level )
  {
    if ( _cellWidget ) {
      if ( _stopSpinBox->value() < level ) {
        _stopSpinBox->blockSignals ( true );
        _stopSpinBox->setValue     ( level );
        _stopSpinBox->blockSignals ( false );
      }
      _updateState = InternalEmit;
      _cellWidget->setStartLevel ( level );
    }
  }


  void  DisplayFilterWidget::stopLevelChanged ( int level )
  {
    if ( _cellWidget ) {
      if ( _startSpinBox->value() > level ) {
        _startSpinBox->blockSignals ( true );
        _startSpinBox->setValue     ( level );
        _startSpinBox->blockSignals ( false );
      }
      _updateState = InternalEmit;
      _cellWidget->setStopLevel ( level );
    }
  }


  void  DisplayFilterWidget::setDoMasterCells ( int state )
  {
    if ( _cellWidget ) {
      _updateState = InternalEmit;
      Query::Mask  queryFilter = _cellWidget->getQueryFilter();
      
      if ( state != Qt::Unchecked ) queryFilter.set   ( Query::DoMasterCells );
      else                          queryFilter.unset ( Query::DoMasterCells );

      _cellWidget->setQueryFilter ( queryFilter );
    }
  }


  void  DisplayFilterWidget::setDoTerminalCells ( int state )
  {
    if ( _cellWidget ) {
      _updateState = InternalEmit;
      Query::Mask  queryFilter = _cellWidget->getQueryFilter();
      
      if ( state != Qt::Unchecked ) queryFilter.set   ( Query::DoTerminalCells );
      else                          queryFilter.unset ( Query::DoTerminalCells );

      _cellWidget->setQueryFilter ( queryFilter );
    }
  }


  void  DisplayFilterWidget::setDoComponents ( int state )
  {
    if ( _cellWidget ) {
      _updateState = InternalEmit;
      Query::Mask  queryFilter = _cellWidget->getQueryFilter();
      
      if ( state != Qt::Unchecked ) queryFilter.set   ( Query::DoComponents );
      else                          queryFilter.unset ( Query::DoComponents );

      _cellWidget->setQueryFilter ( queryFilter );
    }
  }


  void  DisplayFilterWidget::setRubberSteiner ()
  {
    if ( _cellWidget ) {
      if ( _cellWidget->getRubberShape() != CellWidget::Steiner ) {
        _updateState = InternalEmit;
        _cellWidget->setRubberShape ( CellWidget::Steiner );
      }
    }
  }


  void  DisplayFilterWidget::setRubberCentric ()
  {
    if ( _cellWidget ) {
      if ( _cellWidget->getRubberShape() != CellWidget::Centric ) {
        _updateState = InternalEmit;
        _cellWidget->setRubberShape ( CellWidget::Centric );
      }
    }
  }


  void  DisplayFilterWidget::setRubberBarycentric ()
  {
    if ( _cellWidget ) {
      if ( _cellWidget->getRubberShape() != CellWidget::Barycentric ) {
        _updateState = InternalEmit;
        _cellWidget->setRubberShape ( CellWidget::Barycentric );
      }
    }
  }


  void  DisplayFilterWidget::setSymbolicMode ()
  {
    if ( _cellWidget ) {
      if ( !_cellWidget->symbolicMode() ) {
        _updateState = InternalEmit;
        _cellWidget->setSymbolicMode ();
      }
    }
  }


  void  DisplayFilterWidget::setRealMode ()
  {
    if ( _cellWidget ) {
      if ( !_cellWidget->realMode() ) {
        _updateState = InternalEmit;
        _cellWidget->setRealMode ();
      }
    }
  }


}
