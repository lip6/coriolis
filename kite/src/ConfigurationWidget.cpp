
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
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ConfigurationWidget.cpp"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>

#include "hurricane/viewer/Graphics.h"
#include "kite/Configuration.h"
#include "kite/ConfigurationWidget.h"


namespace Kite {

  using Hurricane::Graphics;


// -------------------------------------------------------------------
// Class  :  "Kite::ConfigurationWidget".


  ConfigurationWidget::ConfigurationWidget ( QWidget* parent )
    : QWidget(parent)
    , _configuration         (NULL)
    , _saturateRatioEdit     (new QLineEdit())
    , _globalThresholdEdit   (new QLineEdit())
    , _edgeCapacityEdit      (new QLineEdit())
    , _expandStepEdit        (new QLineEdit())
    , _eventsLimitEdit       (new QLineEdit())
    , _ripupCostSb           (new QSpinBox())
    , _borderRipupLimitSb    (new QSpinBox())
    , _strapRipupLimitSb     (new QSpinBox())
    , _localRipupLimitSb     (new QSpinBox())
    , _globalRipupLimitSb    (new QSpinBox())
    , _longGlobalRipupLimitSb(new QSpinBox())
  {
    setAttribute ( Qt::WA_QuitOnClose, false );

    QVBoxLayout* vLayout = new QVBoxLayout ();
    QGridLayout* gLayout = new QGridLayout ();

    QLabel* label = new QLabel();
    label->setFont ( Graphics::getNormalFont(true) );
    label->setText ( "Katabatic - Routing Database" );
    gLayout->addWidget ( label, 0, 0, 1, 4, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Saturate Ratio (%)" );
    _saturateRatioEdit->setValidator ( new QDoubleValidator(this) );
    _saturateRatioEdit->setEnabled   ( false );
    gLayout->addWidget ( label             , 1, 0, Qt::AlignRight );
    gLayout->addWidget ( _saturateRatioEdit, 1, 1, Qt::AlignLeft );

    label = new QLabel();
    _globalThresholdEdit->setValidator ( new QIntValidator(this) );
    _globalThresholdEdit->setEnabled   ( false );
    label->setText ( "Global Length Threshold" );
    gLayout->addWidget ( label               , 2, 0, Qt::AlignRight );
    gLayout->addWidget ( _globalThresholdEdit, 2, 1, Qt::AlignLeft );

    const int kiteRow = 3;

    label = new QLabel();
    label->setFont ( Graphics::getNormalFont(true) );
    label->setText ( "Kite - Detailed Router" );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );

    gLayout->addWidget ( separator, kiteRow+0, 0, 1, 4 );
    gLayout->addWidget ( label    , kiteRow+1, 0, 1, 4, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Edge Capacity (%)" );
    _edgeCapacityEdit->setValidator ( new QDoubleValidator(this) );
    _edgeCapacityEdit->setEnabled   ( false );
    gLayout->addWidget ( label            , kiteRow+2, 0, Qt::AlignRight );
    gLayout->addWidget ( _edgeCapacityEdit, kiteRow+2, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Expand Step (%)" );
    _expandStepEdit->setValidator ( new QDoubleValidator(this) );
    _expandStepEdit->setEnabled   ( false );
    gLayout->addWidget ( label          , kiteRow+3, 0, Qt::AlignRight );
    gLayout->addWidget ( _expandStepEdit, kiteRow+3, 1, Qt::AlignLeft );

    label = new QLabel();
    _eventsLimitEdit->setValidator ( new QIntValidator(this) );
    _eventsLimitEdit->setEnabled   ( false );
    label->setText ( "Events Limit" );
    gLayout->addWidget ( label           , kiteRow+4, 0, Qt::AlignRight );
    gLayout->addWidget ( _eventsLimitEdit, kiteRow+4, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Ripup Cost" );
    _ripupCostSb->setEnabled   ( false );
    gLayout->addWidget ( label       , kiteRow+2, 2, Qt::AlignRight );
    gLayout->addWidget ( _ripupCostSb, kiteRow+2, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setFont ( Graphics::getNormalFont(true) );
    label->setText ( "Ripup Limit" );
    gLayout->addWidget ( label, kiteRow+3, 2, Qt::AlignLeft );

    QSizePolicy spinBoxPolicy (QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    label = new QLabel();
    label->setText ( "Borders" );
    _borderRipupLimitSb->setEnabled    ( false );
  //_borderRipupLimitSb->setSizePolicy ( spinBoxPolicy );
    gLayout->addWidget ( label              , kiteRow+4, 2, Qt::AlignRight );
    gLayout->addWidget ( _borderRipupLimitSb, kiteRow+4, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Straps" );
    _strapRipupLimitSb->setEnabled   ( false );
    gLayout->addWidget ( label             , kiteRow+5, 2, Qt::AlignRight );
    gLayout->addWidget ( _strapRipupLimitSb, kiteRow+5, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Locals" );
    _localRipupLimitSb->setEnabled   ( false );
    gLayout->addWidget ( label             , kiteRow+6, 2, Qt::AlignRight );
    gLayout->addWidget ( _localRipupLimitSb, kiteRow+6, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Globals" );
    _globalRipupLimitSb->setEnabled   ( false );
    gLayout->addWidget ( label              , kiteRow+7, 2, Qt::AlignRight );
    gLayout->addWidget ( _globalRipupLimitSb, kiteRow+7, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Long Globals" );
    _longGlobalRipupLimitSb->setEnabled   ( false );
    gLayout->addWidget ( label                  , kiteRow+8, 2, Qt::AlignRight );
    gLayout->addWidget ( _longGlobalRipupLimitSb, kiteRow+8, 3, Qt::AlignLeft );

    separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    gLayout->addWidget ( separator, kiteRow+9, 0, 1, 4 );

    vLayout->addLayout ( gLayout );

    QHBoxLayout* hLayout = new QHBoxLayout ();
    hLayout->addStretch ();

    QPushButton* apply = new QPushButton ();
    apply->setText ( tr("Apply") );
    hLayout->addWidget ( apply );
    hLayout->addStretch ();

    vLayout->addLayout ( hLayout );
    vLayout->addStretch ();

    setLayout ( vLayout );

    connect ( apply, SIGNAL(clicked()), this, SLOT(_applySettings()) );
    // connect ( _saturateRatioEdit     , SIGNAL(returnPressed())  , this, SLOT(_updateSaturateRatio()) );
    // connect ( _globalThresholdEdit   , SIGNAL(returnPressed())  , this, SLOT(_updateGlobalThreshold()) );
    // connect ( _edgeCapacityEdit      , SIGNAL(returnPressed())  , this, SLOT(_updateEdgeCapacity()) );
    // connect ( _expandStepEdit        , SIGNAL(returnPressed())  , this, SLOT(_updateExpandStep()) );
    // connect ( _eventsLimitEdit       , SIGNAL(returnPressed())  , this, SLOT(_updateEventsLimit()) );
    // connect ( _ripupCostSb           , SIGNAL(valueChanged(int)), this, SLOT(_updateRipupCost(int)) );
    // connect ( _borderRipupLimitSb    , SIGNAL(valueChanged(int)), this, SLOT(_updateBorderRipupLimit(int)) );
    // connect ( _strapRipupLimitSb     , SIGNAL(valueChanged(int)), this, SLOT(_updateStrapRipupLimit(int)) );
    // connect ( _localRipupLimitSb     , SIGNAL(valueChanged(int)), this, SLOT(_updateLocalRipupLimit(int)) );
    // connect ( _globalRipupLimitSb    , SIGNAL(valueChanged(int)), this, SLOT(_updateGlobalRipupLimit(int)) );
    // connect ( _longGlobalRipupLimitSb, SIGNAL(valueChanged(int)), this, SLOT(_updateLongGlobalRipupLimit(int)) );
  }


  void  ConfigurationWidget::setConfiguration ( Configuration* config )
  {
    if ( config != NULL ) {
      _saturateRatioEdit  ->setText ( QString("%1").arg(config->getSaturateRatio()) );
      _globalThresholdEdit->setText ( QString("%1").arg((long)DbU::getLambda(config->getGlobalThreshold())) );

      _edgeCapacityEdit->setText ( QString("%1").arg(config->getEdgeCapacityPercent()) );
      _expandStepEdit  ->setText ( QString("%1").arg(config->getExpandStep()) );
      _eventsLimitEdit ->setText ( QString("%1").arg(config->getEventsLimit()) );

      _ripupCostSb           ->setValue ( config->getRipupCost() );
      _borderRipupLimitSb    ->setValue ( config->getRipupLimit(Configuration::BorderRipupLimit) );
      _strapRipupLimitSb     ->setValue ( config->getRipupLimit(Configuration::StrapRipupLimit) );
      _localRipupLimitSb     ->setValue ( config->getRipupLimit(Configuration::LocalRipupLimit) );
      _globalRipupLimitSb    ->setValue ( config->getRipupLimit(Configuration::GlobalRipupLimit) );
      _longGlobalRipupLimitSb->setValue ( config->getRipupLimit(Configuration::LongGlobalRipupLimit) );

      _saturateRatioEdit     ->setEnabled ( true );
      _globalThresholdEdit   ->setEnabled ( true );
      _edgeCapacityEdit      ->setEnabled ( true );
      _expandStepEdit        ->setEnabled ( true );
      _eventsLimitEdit       ->setEnabled ( true );
      _ripupCostSb           ->setEnabled ( true );
      _borderRipupLimitSb    ->setEnabled ( true );
      _strapRipupLimitSb     ->setEnabled ( true );
      _localRipupLimitSb     ->setEnabled ( true );
      _globalRipupLimitSb    ->setEnabled ( true );
      _longGlobalRipupLimitSb->setEnabled ( true );
    } else {
      _saturateRatioEdit     ->setEnabled ( false );
      _globalThresholdEdit   ->setEnabled ( false );
      _edgeCapacityEdit      ->setEnabled ( false );
      _expandStepEdit        ->setEnabled ( false );
      _eventsLimitEdit       ->setEnabled ( false );
      _ripupCostSb           ->setEnabled ( false );
      _borderRipupLimitSb    ->setEnabled ( false );
      _strapRipupLimitSb     ->setEnabled ( false );
      _localRipupLimitSb     ->setEnabled ( false );
      _globalRipupLimitSb    ->setEnabled ( false );
      _longGlobalRipupLimitSb->setEnabled ( false );
    }
    _configuration = config;
  }


  void  ConfigurationWidget::_applySettings ()
  {
    if ( _configuration == NULL ) return;

    _updateSaturateRatio       ();
    _updateGlobalThreshold     ();
    _updateEdgeCapacity        ();
    _updateExpandStep          ();
    _updateEventsLimit         ();
    _updateRipupCost           (_ripupCostSb           ->value());
    _updateBorderRipupLimit    (_borderRipupLimitSb    ->value());
    _updateStrapRipupLimit     (_strapRipupLimitSb     ->value());
    _updateLocalRipupLimit     (_localRipupLimitSb     ->value());
    _updateGlobalRipupLimit    (_globalRipupLimitSb    ->value());
    _updateLongGlobalRipupLimit(_longGlobalRipupLimitSb->value());
  }


  void  ConfigurationWidget::_updateSaturateRatio ()
  {
    if ( _configuration == NULL ) return;

    bool  success = false;
    float ratio   = _saturateRatioEdit->displayText().toFloat ( &success );
    if ( not success or (ratio >= 1.0) or (ratio < 0.0) ) {
      _saturateRatioEdit->setText ( QString("%1").arg(_configuration->getSaturateRatio()) );
    } else {
      _configuration->setSaturateRatio ( ratio );
    }
  }


  void  ConfigurationWidget::_updateGlobalThreshold ()
  {
    if ( _configuration == NULL ) return;

    bool      success = false;
    DbU::Unit length  = DbU::lambda ( _globalThresholdEdit->displayText().toDouble(&success) );
    if ( not success or (length < 0) ) {
      _globalThresholdEdit->setText ( QString("%1").arg((long)DbU::getLambda(_configuration->getGlobalThreshold())) );
    } else {
      _configuration->setGlobalThreshold ( length );
    }
  }


  void  ConfigurationWidget::_updateEdgeCapacity ()
  {
    if ( _configuration == NULL ) return;

    bool  success = false;
    float ratio   = _edgeCapacityEdit->displayText().toFloat ( &success );
    if ( not success or (ratio >= 1.0) or (ratio < 0.0) ) {
      _edgeCapacityEdit->setText ( QString("%1").arg(_configuration->getEdgeCapacityPercent()) );
    } else {
      _configuration->setEdgeCapacityPercent ( ratio );
    }
  }


  void  ConfigurationWidget::_updateExpandStep ()
  {
    if ( _configuration == NULL ) return;

    bool  success = false;
    float ratio   = _expandStepEdit->displayText().toFloat ( &success );
    if ( not success or (ratio >= 1.0) or (ratio < 0.0) ) {
      _expandStepEdit->setText ( QString("%1").arg(_configuration->getExpandStep()) );
    } else {
      _configuration->setExpandStep ( ratio );
    }
  }


  void  ConfigurationWidget::_updateEventsLimit ()
  {
    if ( _configuration == NULL ) return;

    bool  success = false;
    long  limit   = _eventsLimitEdit->displayText().toLong ( &success );
    if ( not success or (limit < 0) ) {
      _eventsLimitEdit->setText ( QString("%1").arg(_configuration->getEventsLimit()) );
    } else {
      _configuration->setEventsLimit ( (unsigned long)limit );
    }
  }


  void  ConfigurationWidget::_updateRipupCost ( int cost )
  {
    if ( _configuration == NULL ) return;

  //std::cerr << "_vptr:" << (void*)(*((int*)_configuration)) << std::endl;
    if ( cost < 0 ) {
      _ripupCostSb->setValue ( (int)_configuration->getRipupCost() );
    } else {
      _configuration->setRipupCost ( (unsigned int)cost );
    }
  }


  void  ConfigurationWidget::_updateBorderRipupLimit ( int limit )
  {
    if ( _configuration == NULL ) return;

    if ( limit < 0 ) {
      _borderRipupLimitSb->setValue ( (int)_configuration->getRipupLimit(Configuration::BorderRipupLimit) );
    } else {
      _configuration->setRipupLimit ( Configuration::BorderRipupLimit, (unsigned int)limit );
    }
  }


  void  ConfigurationWidget::_updateStrapRipupLimit ( int limit )
  {
    if ( _configuration == NULL ) return;

    if ( limit < 0 ) {
      _strapRipupLimitSb->setValue ( (int)_configuration->getRipupLimit(Configuration::StrapRipupLimit) );
    } else {
      _configuration->setRipupLimit ( Configuration::StrapRipupLimit, (unsigned int)limit );
    }
  }


  void  ConfigurationWidget::_updateLocalRipupLimit ( int limit )
  {
    if ( _configuration == NULL ) return;

    if ( limit < 0 ) {
      _localRipupLimitSb->setValue ( (int)_configuration->getRipupLimit(Configuration::LocalRipupLimit) );
    } else {
      _configuration->setRipupLimit ( Configuration::LocalRipupLimit, (unsigned int)limit );
    }
  }


  void  ConfigurationWidget::_updateGlobalRipupLimit ( int limit )
  {
    if ( _configuration == NULL ) return;

    if ( limit < 0 ) {
      _globalRipupLimitSb->setValue ( (int)_configuration->getRipupLimit(Configuration::GlobalRipupLimit) );
    } else {
      _configuration->setRipupLimit ( Configuration::GlobalRipupLimit, (unsigned int)limit );
    }
  }


  void  ConfigurationWidget::_updateLongGlobalRipupLimit ( int limit )
  {
    if ( _configuration == NULL ) return;

    if ( limit < 0 ) {
      _longGlobalRipupLimitSb->setValue ( (int)_configuration->getRipupLimit(Configuration::LongGlobalRipupLimit) );
    } else {
      _configuration->setRipupLimit ( Configuration::LongGlobalRipupLimit, (unsigned int)limit );
    }
  }


}  // End of Kite namespace.
