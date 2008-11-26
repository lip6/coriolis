
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
#include  <QGridLayout>
#include  <QVBoxLayout>

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/CellWidget.h"
//#include  "hurricane/viewer/DisplayFilterWidget.h"
#include  "hurricane/viewer/HDisplayFilter.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "DisplayFilterWidget".


  DisplayFilterWidget::DisplayFilterWidget ( QWidget* parent )
    : QWidget(parent)
    , _cellWidget(NULL)
    , _startSpinBox(new QSpinBox())
    , _stopSpinBox(new QSpinBox())
    , _queryFilter(Query::DoAll)
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

    QCheckBox* filterBox = new QCheckBox ();
    filterBox->setFont    ( Graphics::getNormalFont() );
    filterBox->setText    ( tr("Process Master Cells") );
    filterBox->setChecked ( true );

    gLayout->addWidget ( filterBox, 3, 0, 1, 2 );
    connect ( filterBox, SIGNAL(stateChanged(int)), this, SLOT(setDoMasterCells(int)) );

    filterBox = new QCheckBox ();
    filterBox->setFont    ( Graphics::getNormalFont() );
    filterBox->setText    ( tr("Process Terminal Cells") );
    filterBox->setChecked ( true );

    gLayout->addWidget ( filterBox, 4, 0, 1, 2 );
    connect ( filterBox, SIGNAL(stateChanged(int)), this, SLOT(setDoTerminalCells(int)) );

    filterBox = new QCheckBox ();
    filterBox->setFont    ( Graphics::getNormalFont() );
    filterBox->setText    ( tr("Process Components") );
    filterBox->setChecked ( true );

    gLayout->addWidget ( filterBox, 5, 0, 1, 2 );
    connect ( filterBox, SIGNAL(stateChanged(int)), this, SLOT(setDoComponents(int)) );

    groupBox->setLayout ( gLayout );
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
        disconnect ( _cellWidget, SLOT(redraw()) );
      _cellWidget = NULL;
      return;
    }
    _cellWidget = cw;
    connect ( this, SIGNAL(filterChanged()), _cellWidget, SLOT(redraw()) );
  }


  void  DisplayFilterWidget::startLevelChanged ( int level )
  {
    if ( _cellWidget ) {
      _cellWidget->setStartLevel ( level );
      if ( _stopSpinBox->value() < level ) {
        _stopSpinBox->setValue ( level );
        return;
      }
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
      emit filterChanged();
    }
  }


  void  DisplayFilterWidget::setDoMasterCells ( int state )
  {
    if ( state != Qt::Unchecked ) _queryFilter |=  Query::DoMasterCells;
    else                          _queryFilter &= ~Query::DoMasterCells;

    _cellWidget->setQueryFilter ( _queryFilter );

    emit filterChanged();
  }


  void  DisplayFilterWidget::setDoTerminalCells ( int state )
  {
    if ( state != Qt::Unchecked ) _queryFilter |=  Query::DoTerminalCells;
    else                          _queryFilter &= ~Query::DoTerminalCells;

    _cellWidget->setQueryFilter ( _queryFilter );

    emit filterChanged();
  }


  void  DisplayFilterWidget::setDoComponents ( int state )
  {
    if ( state != Qt::Unchecked ) _queryFilter |=  Query::DoComponents;
    else                          _queryFilter &= ~Query::DoComponents;

    _cellWidget->setQueryFilter ( _queryFilter );

    emit filterChanged();
  }


}
