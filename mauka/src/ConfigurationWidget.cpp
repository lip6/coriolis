
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
// |              M a u k a  -  P l a c e r                          |
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
#include <QCheckBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>

#include "hurricane/viewer/Graphics.h"
#include "nimbus/Configuration.h"
#include "mauka/Configuration.h"
#include "mauka/ConfigurationWidget.h"


namespace Mauka {

  using Hurricane::Graphics;


// -------------------------------------------------------------------
// Class  :  "Mauka::ConfigurationWidget".


  ConfigurationWidget::ConfigurationWidget ( QWidget* parent )
    : QWidget(parent)
    , _nimbusConfiguration   (NULL)
    , _maukaConfiguration    (NULL)
    , _standardAnnealingState(new QCheckBox())
    , _ignorePinsState       (new QCheckBox())
    , _plotBinsState         (new QCheckBox())
    , _pinsPlacementState    (new QCheckBox())
    , _searchRatioEdit       (new QLineEdit())
    , _annealingNetMultEdit  (new QLineEdit())
    , _annealingBinMultEdit  (new QLineEdit())
    , _annealingRowMultEdit  (new QLineEdit())
    , _aspectRatioEdit       (new QLineEdit())
    , _marginEdit            (new QLineEdit())
  {
    setAttribute ( Qt::WA_QuitOnClose, false );

    QVBoxLayout* vLayout = new QVBoxLayout ();
    QGridLayout* gLayout = new QGridLayout ();

    QLabel* label = new QLabel();
    label->setFont ( Graphics::getNormalFont(true) );
    label->setText ( "Mauka - Placer" );
    gLayout->addWidget ( label, 0, 0, 1, 4, Qt::AlignLeft );

  // Nimbus configuration.
    label = new QLabel();
    label->setText ( "Pins Placement" );
    _pinsPlacementState->setEnabled ( false );
    gLayout->addWidget ( label              , 1, 0, Qt::AlignRight );
    gLayout->addWidget ( _pinsPlacementState, 1, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Aspect Ratio: X/Y (%)" );
    _aspectRatioEdit->setValidator ( new QDoubleValidator(this) );
    _aspectRatioEdit->setEnabled   ( false );
    gLayout->addWidget ( label           , 1, 2, Qt::AlignRight );
    gLayout->addWidget ( _aspectRatioEdit, 1, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Space Margin (%)" );
    _marginEdit->setValidator ( new QDoubleValidator(this) );
    _marginEdit->setEnabled   ( false );
    gLayout->addWidget ( label      , 2, 2, Qt::AlignRight );
    gLayout->addWidget ( _marginEdit, 2, 3, Qt::AlignLeft );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    gLayout->addWidget ( separator, 3, 0, 1, 4 );

  // Mauka configuration.
    label = new QLabel();
    label->setText ( "Standard Annealing" );
    _standardAnnealingState->setEnabled ( false );
    gLayout->addWidget ( label                  , 4, 0, Qt::AlignRight );
    gLayout->addWidget ( _standardAnnealingState, 4, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Ignore Pins" );
    _ignorePinsState->setEnabled ( false );
    gLayout->addWidget ( label           , 5, 0, Qt::AlignRight );
    gLayout->addWidget ( _ignorePinsState, 5, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Plot Bins" );
    _plotBinsState->setEnabled ( false );
    gLayout->addWidget ( label         , 6, 0, Qt::AlignRight );
    gLayout->addWidget ( _plotBinsState, 6, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Search Ratio (%)" );
    _searchRatioEdit->setValidator ( new QDoubleValidator(this) );
    _searchRatioEdit->setEnabled   ( false );
    gLayout->addWidget ( label           , 4, 2, Qt::AlignRight );
    gLayout->addWidget ( _searchRatioEdit, 4, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Annealing Net Mult (%)" );
    _annealingNetMultEdit->setValidator ( new QDoubleValidator(this) );
    _annealingNetMultEdit->setEnabled   ( false );
    gLayout->addWidget ( label                , 5, 2, Qt::AlignRight );
    gLayout->addWidget ( _annealingNetMultEdit, 5, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Annealing Bin Mult (%)" );
    _annealingBinMultEdit->setValidator ( new QDoubleValidator(this) );
    _annealingBinMultEdit->setEnabled   ( false );
    gLayout->addWidget ( label                , 6, 2, Qt::AlignRight );
    gLayout->addWidget ( _annealingBinMultEdit, 6, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Annealing Row Mult (%)" );
    _annealingRowMultEdit->setValidator ( new QDoubleValidator(this) );
    _annealingRowMultEdit->setEnabled   ( false );
    gLayout->addWidget ( label                , 7, 2, Qt::AlignRight );
    gLayout->addWidget ( _annealingRowMultEdit, 7, 3, Qt::AlignLeft );

    separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    gLayout->addWidget ( separator, 8, 0, 1, 4 );

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
  }


  void  ConfigurationWidget::setConfiguration ( Nimbus::Configuration* nimbusConfig
                                              , Configuration*         maukaConfig )
  {
    if ( maukaConfig != NULL ) {
      _searchRatioEdit     ->setText ( QString("%1").arg(maukaConfig ->getSearchRatio()) );
      _annealingNetMultEdit->setText ( QString("%1").arg(maukaConfig ->getAnnealingNetMult()) );
      _annealingBinMultEdit->setText ( QString("%1").arg(maukaConfig ->getAnnealingBinMult()) );
      _annealingRowMultEdit->setText ( QString("%1").arg(maukaConfig ->getAnnealingRowMult()) );
      _aspectRatioEdit     ->setText ( QString("%1").arg(nimbusConfig->getAspectRatio()) );
      _marginEdit          ->setText ( QString("%1").arg(nimbusConfig->getMargin()) );

      _standardAnnealingState->setChecked ( maukaConfig ->useStandardSimulatedAnnealing() );
      _ignorePinsState       ->setChecked ( maukaConfig ->doIgnorePins() );
      _plotBinsState         ->setChecked ( maukaConfig ->doPlotBins() );
      _pinsPlacementState    ->setChecked ( nimbusConfig->doPinsPlacement() );

      _searchRatioEdit       ->setEnabled ( true );
      _annealingNetMultEdit  ->setEnabled ( true );
      _annealingBinMultEdit  ->setEnabled ( true );
      _annealingRowMultEdit  ->setEnabled ( true );
      _aspectRatioEdit       ->setEnabled ( true );
      _marginEdit            ->setEnabled ( true );
      _standardAnnealingState->setEnabled ( true );
      _ignorePinsState       ->setEnabled ( true );
      _plotBinsState         ->setEnabled ( true );
      _pinsPlacementState    ->setEnabled ( true );
    } else {
      _searchRatioEdit       ->setEnabled ( false );
      _annealingNetMultEdit  ->setEnabled ( false );
      _annealingBinMultEdit  ->setEnabled ( false );
      _annealingRowMultEdit  ->setEnabled ( false );
      _marginEdit            ->setEnabled ( false );
      _standardAnnealingState->setEnabled ( false );
      _standardAnnealingState->setEnabled ( false );
      _ignorePinsState       ->setEnabled ( false );
      _plotBinsState         ->setEnabled ( false );
      _pinsPlacementState    ->setEnabled ( false );

      _nimbusConfiguration = NULL;
      _maukaConfiguration  = NULL;
    }
    _nimbusConfiguration = nimbusConfig;
    _maukaConfiguration  = maukaConfig;
  }


  void  ConfigurationWidget::_applySettings ()
  {
    if ( _maukaConfiguration == NULL ) return;

    _updateStandardAnnealingState ();
    _updateIgnorePinsState        ();
    _updatePlotBinsState          ();
    _updatePinsPlacementState     ();
    _updateSearchRatio            ();
    _updateAnnealingNetMult       ();
    _updateAnnealingBinMult       ();
    _updateAnnealingRowMult       ();
    _updateAspectRatio            ();
    _updateMargin                 ();
  }


  void  ConfigurationWidget::_updateSearchRatio ()
  {
    if ( _maukaConfiguration == NULL ) return;

    bool  success = false;
    float ratio   = _searchRatioEdit->displayText().toFloat ( &success );

    if ( success ) {
      _maukaConfiguration->setSearchRatio ( ratio );
      success = (_maukaConfiguration->getSearchRatio() == ratio);
    }

    if ( not success )
      _searchRatioEdit->setText ( QString("%1").arg(_maukaConfiguration->getSearchRatio()) );
  }


  void  ConfigurationWidget::_updateAnnealingNetMult ()
  {
    if ( _maukaConfiguration == NULL ) return;

    bool  success = false;
    float ratio   = _annealingNetMultEdit->displayText().toFloat ( &success );

    if ( success ) {
      _maukaConfiguration->setAnnealingNetMult ( ratio );
      success = (_maukaConfiguration->getAnnealingNetMult() == ratio);
    }

    if ( not success )
      _annealingNetMultEdit->setText ( QString("%1").arg(_maukaConfiguration->getAnnealingNetMult()) );
  }


  void  ConfigurationWidget::_updateAnnealingBinMult ()
  {
    if ( _maukaConfiguration == NULL ) return;

    bool  success = false;
    float ratio   = _annealingBinMultEdit->displayText().toFloat ( &success );

    if ( success ) {
      _maukaConfiguration->setAnnealingBinMult ( ratio );
      success = (_maukaConfiguration->getAnnealingBinMult() == ratio);
    }

    if ( not success )
      _annealingBinMultEdit->setText ( QString("%1").arg(_maukaConfiguration->getAnnealingBinMult()) );
  }


  void  ConfigurationWidget::_updateAnnealingRowMult ()
  {
    if ( _maukaConfiguration == NULL ) return;

    bool  success = false;
    float ratio   = _annealingRowMultEdit->displayText().toFloat ( &success );

    if ( success ) {
      _maukaConfiguration->setAnnealingRowMult ( ratio );
      success = (_maukaConfiguration->getAnnealingRowMult() == ratio);
    }

    if ( not success )
      _annealingRowMultEdit->setText ( QString("%1").arg(_maukaConfiguration->getAnnealingRowMult()) );
  }


  void  ConfigurationWidget::_updateAspectRatio ()
  {
    if ( _nimbusConfiguration == NULL ) return;

    bool  success = false;
    float ratio   = _aspectRatioEdit->displayText().toFloat ( &success );

    if ( success ) {
      _nimbusConfiguration->setAspectRatio ( ratio );
      success = (_nimbusConfiguration->getAspectRatio() == ratio);
    }

    if ( not success )
      _aspectRatioEdit->setText ( QString("%1").arg(_nimbusConfiguration->getAspectRatio()) );
  }


  void  ConfigurationWidget::_updateMargin ()
  {
    if ( _nimbusConfiguration == NULL ) return;

    bool  success = false;
    float ratio   = _marginEdit->displayText().toFloat ( &success );

    if ( success ) {
      _nimbusConfiguration->setMargin ( ratio );
      success = (_nimbusConfiguration->getMargin() == ratio);
    }

    if ( not success )
      _marginEdit->setText ( QString("%1").arg(_nimbusConfiguration->getMargin()) );
  }


  void  ConfigurationWidget::_updateStandardAnnealingState ()
  {
    if ( _maukaConfiguration == NULL ) return;

    _maukaConfiguration->setStandardSimulatedAnnealing ( _standardAnnealingState->isChecked() );
  }


  void  ConfigurationWidget::_updateIgnorePinsState ()
  {
    if ( _maukaConfiguration == NULL ) return;

    _maukaConfiguration->setIgnorePins ( _ignorePinsState->isChecked() );
  }


  void  ConfigurationWidget::_updatePlotBinsState ()
  {
    if ( _maukaConfiguration == NULL ) return;

    _maukaConfiguration->setPlotBins ( _plotBinsState->isChecked() );
  }


  void  ConfigurationWidget::_updatePinsPlacementState ()
  {
    if ( _nimbusConfiguration == NULL ) return;

    _nimbusConfiguration->setPinsPlacement ( _pinsPlacementState->isChecked() );
  }


}  // End of Mauka namespace.
