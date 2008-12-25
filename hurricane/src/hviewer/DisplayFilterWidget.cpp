
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
    , _queryFilter    (Query::DoAll)
    , _signalEmitter  (false)
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
    wLayout->addStretch ();

    setLayout ( wLayout );

    connect ( _startSpinBox, SIGNAL(valueChanged(int)), this, SLOT(startLevelChanged(int)) );
    connect ( _stopSpinBox , SIGNAL(valueChanged(int)), this, SLOT(stopLevelChanged (int)) );
  }


  void  DisplayFilterWidget::setCellWidget ( CellWidget* cw )
  {
    if ( !cw ) {
      if ( _cellWidget )
        disconnect ( _cellWidget, SLOT(refresh()) );
      _cellWidget = NULL;
      return;
    }

    _cellWidget = cw;
    connect ( this        , SIGNAL(filterChanged())  , _cellWidget, SLOT(refresh()) );
    connect ( _cellWidget , SIGNAL(settingsChanged()), this       , SLOT(syncFromCellWidget()) );
    connect ( _steiner    , SIGNAL(clicked())        , this       , SLOT(setRubberSteiner()) );
    connect ( _centric    , SIGNAL(clicked())        , this       , SLOT(setRubberCentric()) );
    connect ( _barycentric, SIGNAL(clicked())        , this       , SLOT(setRubberBarycentric()) );

    _signalEmitter = false;
    syncFromCellWidget ();
  }


  void  DisplayFilterWidget::syncFromCellWidget ()
  {
    if ( !_cellWidget    ) return;
    if (  _signalEmitter ) {
      _signalEmitter = false;
      return;
    }

    _doMasterCells  ->setChecked ( _cellWidget->getQueryFilter() & Query::DoMasterCells   );
    _doTerminalCells->setChecked ( _cellWidget->getQueryFilter() & Query::DoTerminalCells );
    _doComponents   ->setChecked ( _cellWidget->getQueryFilter() & Query::DoComponents    );

    switch ( _cellWidget->getRubberShape() ) {
      case CellWidget::Steiner:     _steiner->setChecked(true); break;
      case CellWidget::Centric:     _centric->setChecked(true); break;
      case CellWidget::Barycentric: _barycentric->setChecked(true); break;
    }
    _signalEmitter = false;
  }


  void  DisplayFilterWidget::startLevelChanged ( int level )
  {
    if ( _cellWidget ) {
      _cellWidget->setStartLevel ( level );
      if ( _stopSpinBox->value() < level ) {
        _stopSpinBox->setValue ( level );
        return;
      }
      _signalEmitter = true;
      emit filterChanged();
    }
  }


  void  DisplayFilterWidget::stopLevelChanged ( int level )
  {
    if ( _cellWidget ) {
      _cellWidget->setStopLevel ( level );
      if ( _startSpinBox->value() > level ) {
        _startSpinBox->setValue ( level );
        return;
      }
      _signalEmitter = true;
      emit filterChanged();
    }
  }


  void  DisplayFilterWidget::setDoMasterCells ( int state )
  {
    if ( state != Qt::Unchecked ) _queryFilter |=  Query::DoMasterCells;
    else                          _queryFilter &= ~Query::DoMasterCells;

    _cellWidget->setQueryFilter ( _queryFilter );

    _signalEmitter = true;
    emit filterChanged();
  }


  void  DisplayFilterWidget::setDoTerminalCells ( int state )
  {
    if ( state != Qt::Unchecked ) _queryFilter |=  Query::DoTerminalCells;
    else                          _queryFilter &= ~Query::DoTerminalCells;

    _cellWidget->setQueryFilter ( _queryFilter );

    _signalEmitter = true;
    emit filterChanged();
  }


  void  DisplayFilterWidget::setDoComponents ( int state )
  {
    if ( state != Qt::Unchecked ) _queryFilter |=  Query::DoComponents;
    else                          _queryFilter &= ~Query::DoComponents;

    _cellWidget->setQueryFilter ( _queryFilter );

    _signalEmitter = true;
    emit filterChanged();
  }


  void  DisplayFilterWidget::setRubberSteiner ()
  {
    if ( _cellWidget ) {
      if ( _cellWidget->getRubberShape() != CellWidget::Steiner )
        _cellWidget->setRubberShape ( CellWidget::Steiner );
    //emit filterChanged();
    }
  }


  void  DisplayFilterWidget::setRubberCentric ()
  {
    if ( _cellWidget ) {
      if ( _cellWidget->getRubberShape() != CellWidget::Centric )
        _cellWidget->setRubberShape ( CellWidget::Centric );
    //emit filterChanged();
    }
  }


  void  DisplayFilterWidget::setRubberBarycentric ()
  {
    if ( _cellWidget ) {
      if ( _cellWidget->getRubberShape() != CellWidget::Barycentric )
        _cellWidget->setRubberShape ( CellWidget::Barycentric );
    //emit filterChanged();
    }
  }


}
