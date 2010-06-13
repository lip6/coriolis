
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
#include <QComboBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>

#include "hurricane/viewer/Graphics.h"
#include "nimbus/Configuration.h"
#include "metis/Configuration.h"
#include "metis/MetisEngine.h"
#include "mauka/Configuration.h"
#include "mauka/ConfigurationWidget.h"


namespace Mauka {

  using Hurricane::Graphics;
  using Metis::MetisEngine;


// -------------------------------------------------------------------
// Class  :  "Mauka::ConfigurationWidget".


  ConfigurationWidget::ConfigurationWidget ( QWidget* parent )
    : QWidget(parent)
    , _nimbusConfiguration               (NULL)
    , _metisConfiguration                (NULL)
    , _maukaConfiguration                (NULL)
    , _partOrKWayHMetisState             (new QCheckBox()) 
    , _standardAnnealingState            (new QCheckBox())
    , _ignorePinsState                   (new QCheckBox())
    , _plotBinsState                     (new QCheckBox())
    , _pinsPlacementState                (new QCheckBox())
    , _hmetisCustomOptionsState          (new QCheckBox())
    , _hmetisNRunsEdit                   (new QLineEdit())
    , _hmetisCTypeCombo                  (new QComboBox())
    , _hmetisRTypeCombo                  (new QComboBox())
    , _hmetisVCycleCombo                 (new QComboBox())
    , _hmetisReconstCombo                (new QComboBox())
    , _numberOfInstancesStopCriterionEdit(new QLineEdit())
    , _globalConnectionsWeightRatioEdit  (new QLineEdit())
    , _ubFactorEdit                      (new QLineEdit())
    , _searchRatioEdit                   (new QLineEdit())
    , _annealingNetMultEdit              (new QLineEdit())
    , _annealingBinMultEdit              (new QLineEdit())
    , _annealingRowMultEdit              (new QLineEdit())
    , _aspectRatioEdit                   (new QLineEdit())
    , _marginEdit                        (new QLineEdit())
  {
    setAttribute ( Qt::WA_QuitOnClose, false );

    QVBoxLayout* vLayout = new QVBoxLayout ();
    QGridLayout* gLayout = new QGridLayout ();

  // Metis configuration.
    const int metisRow = 0;

    QLabel* label = new QLabel();
    label->setFont ( Graphics::getNormalFont(true) );
    label->setText ( "hMETIS - Partionner" );
    gLayout->addWidget ( label, metisRow+0, 0, 1, 4, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Recursive 2-parts (vs. K-Way)" );
    _partOrKWayHMetisState->setEnabled ( false );
    gLayout->addWidget ( label                 , metisRow+1, 0, Qt::AlignRight );
    gLayout->addWidget ( _partOrKWayHMetisState, metisRow+1, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Partition Size Stop" );
    _numberOfInstancesStopCriterionEdit->setValidator ( new QIntValidator(this) );
    _numberOfInstancesStopCriterionEdit->setEnabled   ( false );
    gLayout->addWidget ( label                              , metisRow+2, 0, Qt::AlignRight );
    gLayout->addWidget ( _numberOfInstancesStopCriterionEdit, metisRow+2, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Global Connections Weight" );
    _globalConnectionsWeightRatioEdit->setValidator ( new QIntValidator(this) );
    _globalConnectionsWeightRatioEdit->setEnabled   ( false );
    gLayout->addWidget ( label                            , metisRow+3, 0, Qt::AlignRight );
    gLayout->addWidget ( _globalConnectionsWeightRatioEdit, metisRow+3, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "UB Factor" );
    _ubFactorEdit->setValidator ( new QIntValidator(this) );
    _ubFactorEdit->setEnabled   ( false );
    gLayout->addWidget ( label        , metisRow+4, 0, Qt::AlignRight );
    gLayout->addWidget ( _ubFactorEdit, metisRow+4, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Tune hMETIS parameters" );
    _hmetisCustomOptionsState->setEnabled ( false );
    gLayout->addWidget ( label                    , metisRow+1, 2, Qt::AlignRight );
    gLayout->addWidget ( _hmetisCustomOptionsState, metisRow+1, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "# of tried bisections" );
    _hmetisNRunsEdit->setValidator ( new QIntValidator(this) );
    _hmetisNRunsEdit->setEnabled   ( false );
    gLayout->addWidget ( label           , metisRow+2, 2, Qt::AlignRight );
    gLayout->addWidget ( _hmetisNRunsEdit, metisRow+2, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "CType" );
    _hmetisCTypeCombo->setEnabled ( false );
    _hmetisCTypeCombo->addItem ( "Hybrid First Choice", Metis::Configuration::CTypeHFC );
    _hmetisCTypeCombo->addItem ( "First Choice"       , Metis::Configuration::CTypeFC );
    _hmetisCTypeCombo->addItem ( "Greedy First Choice", Metis::Configuration::CTypeGFC );
    _hmetisCTypeCombo->addItem ( "Hyper Edge"         , Metis::Configuration::CTypeHyperEdge );
    _hmetisCTypeCombo->addItem ( "Edge"               , Metis::Configuration::CTypeEdge );
    gLayout->addWidget ( label            , metisRow+3, 2, Qt::AlignRight );
    gLayout->addWidget ( _hmetisCTypeCombo, metisRow+3, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "RType" );
    _hmetisRTypeCombo->setEnabled ( false );
    _hmetisRTypeCombo->addItem ( "Fiduccia-Mattheyses", Metis::Configuration::RTypeFM );
    _hmetisRTypeCombo->addItem ( "One Way FM"         , Metis::Configuration::RTypeOneWayFM );
    _hmetisRTypeCombo->addItem ( "Early Exit FM"      , Metis::Configuration::RTypeEarlyExitFM );
    gLayout->addWidget ( label            , metisRow+4, 2, Qt::AlignRight );
    gLayout->addWidget ( _hmetisRTypeCombo, metisRow+4, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "V-Cycle" );
    _hmetisVCycleCombo->setEnabled ( false );
    _hmetisVCycleCombo->addItem ( "No V-Cycle Refinement"        , Metis::Configuration::VCycleDisable );
    _hmetisVCycleCombo->addItem ( "On Each Final Bisections"     , Metis::Configuration::VCycleFinal );
    _hmetisVCycleCombo->addItem ( "On Best Itermediate Solutions", Metis::Configuration::VCycleBestIntermed );
    _hmetisVCycleCombo->addItem ( "On All Intermediate Solutions", Metis::Configuration::VCycleAllIntermed );
    gLayout->addWidget ( label             , metisRow+5, 2, Qt::AlignRight );
    gLayout->addWidget ( _hmetisVCycleCombo, metisRow+5, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Reconst" );
    _hmetisReconstCombo->setEnabled ( false );
    _hmetisReconstCombo->addItem ( "Ignore cut hyperedges"        , Metis::Configuration::ReconstRemoveCutHE );
    _hmetisReconstCombo->addItem ( "Keep parts of cuts hyperedges", Metis::Configuration::ReconstKeepCutHE );
    gLayout->addWidget ( label              , metisRow+6, 2, Qt::AlignRight );
    gLayout->addWidget ( _hmetisReconstCombo, metisRow+6, 3, Qt::AlignLeft );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    gLayout->addWidget ( separator, metisRow+7, 0, 1, 4 );

  // Nimbus configuration.
    const int nimbusRow = metisRow+8;

    label = new QLabel();
    label->setFont ( Graphics::getNormalFont(true) );
    label->setText ( "Mauka - Placer" );
    gLayout->addWidget ( label, nimbusRow+0, 0, 1, 4, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Pins Placement" );
    _pinsPlacementState->setEnabled ( false );
    gLayout->addWidget ( label              , nimbusRow+1, 0, Qt::AlignRight );
    gLayout->addWidget ( _pinsPlacementState, nimbusRow+1, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Aspect Ratio: X/Y (%)" );
    _aspectRatioEdit->setValidator ( new QDoubleValidator(this) );
    _aspectRatioEdit->setEnabled   ( false );
    gLayout->addWidget ( label           , nimbusRow+1, 2, Qt::AlignRight );
    gLayout->addWidget ( _aspectRatioEdit, nimbusRow+1, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Space Margin (%)" );
    _marginEdit->setValidator ( new QDoubleValidator(this) );
    _marginEdit->setEnabled   ( false );
    gLayout->addWidget ( label      , nimbusRow+2, 2, Qt::AlignRight );
    gLayout->addWidget ( _marginEdit, nimbusRow+2, 3, Qt::AlignLeft );

    separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    gLayout->addWidget ( separator, nimbusRow+3, 0, 1, 4 );

  // Mauka configuration.
    const int maukaRow = nimbusRow+4;

    label = new QLabel();
    label->setText ( "Standard Annealing" );
    _standardAnnealingState->setEnabled ( false );
    gLayout->addWidget ( label                  , maukaRow+0, 0, Qt::AlignRight );
    gLayout->addWidget ( _standardAnnealingState, maukaRow+0, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Ignore Pins" );
    _ignorePinsState->setEnabled ( false );
    gLayout->addWidget ( label           , maukaRow+1, 0, Qt::AlignRight );
    gLayout->addWidget ( _ignorePinsState, maukaRow+1, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Plot Bins" );
    _plotBinsState->setEnabled ( false );
    gLayout->addWidget ( label         , maukaRow+2, 0, Qt::AlignRight );
    gLayout->addWidget ( _plotBinsState, maukaRow+2, 1, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Search Ratio (%)" );
    _searchRatioEdit->setValidator ( new QDoubleValidator(this) );
    _searchRatioEdit->setEnabled   ( false );
    gLayout->addWidget ( label           , maukaRow+0, 2, Qt::AlignRight );
    gLayout->addWidget ( _searchRatioEdit, maukaRow+0, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Annealing Net Mult (%)" );
    _annealingNetMultEdit->setValidator ( new QDoubleValidator(this) );
    _annealingNetMultEdit->setEnabled   ( false );
    gLayout->addWidget ( label                , maukaRow+1, 2, Qt::AlignRight );
    gLayout->addWidget ( _annealingNetMultEdit, maukaRow+1, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Annealing Bin Mult (%)" );
    _annealingBinMultEdit->setValidator ( new QDoubleValidator(this) );
    _annealingBinMultEdit->setEnabled   ( false );
    gLayout->addWidget ( label                , maukaRow+2, 2, Qt::AlignRight );
    gLayout->addWidget ( _annealingBinMultEdit, maukaRow+2, 3, Qt::AlignLeft );

    label = new QLabel();
    label->setText ( "Annealing Row Mult (%)" );
    _annealingRowMultEdit->setValidator ( new QDoubleValidator(this) );
    _annealingRowMultEdit->setEnabled   ( false );
    gLayout->addWidget ( label                , maukaRow+3, 2, Qt::AlignRight );
    gLayout->addWidget ( _annealingRowMultEdit, maukaRow+3, 3, Qt::AlignLeft );

    separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    gLayout->addWidget ( separator, maukaRow+4, 0, 1, 4 );

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
    connect ( _hmetisCustomOptionsState, SIGNAL(stateChanged(int)), this, SLOT(_enableHMetisOptions(int)) );
  }


  void  ConfigurationWidget::setConfiguration ( Nimbus::Configuration* nimbusConfig
                                              , Metis::Configuration*  metisConfig
                                              , Configuration*         maukaConfig )
  {
    _nimbusConfiguration = nimbusConfig;
    _metisConfiguration  = metisConfig;
    _maukaConfiguration  = maukaConfig;

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

      if ( MetisEngine::isHMetisCapable() ) {
        _partOrKWayHMetisState   ->setChecked (  metisConfig->getPartOrKWayHMetis() );
        _hmetisCustomOptionsState->setChecked ( (metisConfig->getHMetisOption(Metis::Configuration::CustomOptions) == 1) );

        _hmetisCTypeCombo  ->setCurrentIndex ( _hmetisCTypeToIndex () );
        _hmetisRTypeCombo  ->setCurrentIndex ( _hmetisRTypeToIndex () );
        _hmetisVCycleCombo ->setCurrentIndex ( _hmetisVCycleToIndex() );
        _hmetisReconstCombo->setCurrentIndex ( _hmetisReconstToIndex() );

        _numberOfInstancesStopCriterionEdit->setText ( QString("%1").arg(metisConfig->getNumberOfInstancesStopCriterion()) );
        _globalConnectionsWeightRatioEdit  ->setText ( QString("%1").arg(metisConfig->getGlobalConnectionsWeightRatio()) );
        _ubFactorEdit                      ->setText ( QString("%1").arg(metisConfig->getUbFactor()) );
        _hmetisNRunsEdit                   ->setText ( QString("%1").arg(metisConfig->getHMetisOption(Metis::Configuration::HMetisNRuns)) );

        _partOrKWayHMetisState             ->setEnabled ( true );
        _numberOfInstancesStopCriterionEdit->setEnabled ( true );
        _globalConnectionsWeightRatioEdit  ->setEnabled ( true );
        _ubFactorEdit                      ->setEnabled ( true );
        _hmetisCustomOptionsState          ->setEnabled ( true );

        _enableHMetisOptions ( _hmetisCustomOptionsState->checkState() );
      }
    } else {
      _searchRatioEdit       ->setEnabled ( false );
      _annealingNetMultEdit  ->setEnabled ( false );
      _annealingBinMultEdit  ->setEnabled ( false );
      _annealingRowMultEdit  ->setEnabled ( false );
      _marginEdit            ->setEnabled ( false );
      _standardAnnealingState->setEnabled ( false );
      _ignorePinsState       ->setEnabled ( false );
      _plotBinsState         ->setEnabled ( false );
      _pinsPlacementState    ->setEnabled ( false );

      _partOrKWayHMetisState             ->setEnabled ( false );
      _numberOfInstancesStopCriterionEdit->setEnabled ( false );
      _globalConnectionsWeightRatioEdit  ->setEnabled ( false );
      _ubFactorEdit                      ->setEnabled ( false );
      _hmetisCustomOptionsState          ->setEnabled ( false );
      _enableHMetisOptions ( Qt::Unchecked );

      _nimbusConfiguration = NULL;
      _metisConfiguration  = NULL;
      _maukaConfiguration  = NULL;
    }
  }


  void  ConfigurationWidget::_enableHMetisOptions ( int state )
  {
    bool enable = (state != Qt::Unchecked);

    _hmetisNRunsEdit   ->setEnabled ( enable );
    _hmetisCTypeCombo  ->setEnabled ( enable );
    _hmetisRTypeCombo  ->setEnabled ( enable );
    _hmetisVCycleCombo ->setEnabled ( enable );
    _hmetisReconstCombo->setEnabled ( enable );
  }


  void  ConfigurationWidget::_applySettings ()
  {
    if ( _maukaConfiguration == NULL ) return;

    _updatePartOrKWayHMetis               ();
    _updateNumberOfInstancesStopCriterion ();
    _updateGlobalConnectionsWeightRatio   ();
    _updateUbFactor                       ();
    _updateHMetisCustomOptions            ();
    _updateHMetisCType                    ();
    _updateHMetisRType                    ();
    _updateHMetisVCycle                   ();
    _updateHMetisReconst                  ();

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

    if ( MetisEngine::isHMetisCapable() ) {
      _updatePartOrKWayHMetis               ();
      _updateNumberOfInstancesStopCriterion ();
      _updateGlobalConnectionsWeightRatio   ();
      _updateUbFactor                       ();
      _updateHMetisCustomOptions            ();
      _updateHMetisNRuns                    ();
      _updateHMetisCType                    ();
      _updateHMetisRType                    ();
      _updateHMetisVCycle                   ();
      _updateHMetisReconst                  ();
    }
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


  void  ConfigurationWidget::_updatePartOrKWayHMetis ()
  {
    if ( _metisConfiguration == NULL ) return;

    _metisConfiguration->setPartOrKWayHMetis ( _partOrKWayHMetisState->isChecked() );
  }


  void  ConfigurationWidget::_updateHMetisCustomOptions ()
  {
    if ( _metisConfiguration == NULL ) return;

    _metisConfiguration->setHMetisOption ( Metis::Configuration::CustomOptions
                                         , (_hmetisCustomOptionsState->isChecked() ? 1 : 0) );
  }


  void  ConfigurationWidget::_updateNumberOfInstancesStopCriterion ()
  {
    if ( _metisConfiguration == NULL ) return;

    bool          success = false;
    unsigned int  limit   = _numberOfInstancesStopCriterionEdit->displayText().toUInt ( &success );

    if ( success ) {
      _metisConfiguration->setNumberOfInstancesStopCriterion ( limit );
      success = (_metisConfiguration->getNumberOfInstancesStopCriterion() == limit);
    }

    if ( not success )
      _numberOfInstancesStopCriterionEdit->setText
        ( QString("%1").arg(_metisConfiguration->getNumberOfInstancesStopCriterion()) );
  }


  void  ConfigurationWidget::_updateGlobalConnectionsWeightRatio ()
  {
    if ( _metisConfiguration == NULL ) return;

    bool  success = false;
    int   weight  = _globalConnectionsWeightRatioEdit->displayText().toInt ( &success );

    if ( success ) {
      _metisConfiguration->setGlobalConnectionsWeightRatio ( weight );
      success = (_metisConfiguration->getGlobalConnectionsWeightRatio() == weight);
    }

    if ( not success )
      _globalConnectionsWeightRatioEdit->setText
        ( QString("%1").arg(_metisConfiguration->getGlobalConnectionsWeightRatio()) );
  }


  void  ConfigurationWidget::_updateUbFactor ()
  {
    if ( _metisConfiguration == NULL ) return;

    bool  success = false;
    int   limit   = _ubFactorEdit->displayText().toUInt ( &success );

    if ( success ) {
      _metisConfiguration->setUbFactor ( limit );
      success = (_metisConfiguration->getUbFactor() == limit);
    }

    if ( not success )
      _ubFactorEdit->setText ( QString("%1").arg(_metisConfiguration->getUbFactor()) );
  }


  void  ConfigurationWidget::_updateHMetisNRuns ()
  {
    if ( _metisConfiguration == NULL ) return;

    bool  success = false;
    int   value   = _hmetisNRunsEdit->displayText().toInt ( &success );

    if ( success ) {
      _metisConfiguration->setHMetisOption ( Metis::Configuration::HMetisNRuns, value );
      success = (_metisConfiguration->getHMetisOption(Metis::Configuration::HMetisNRuns) == value);
    }

    if ( not success )
      _numberOfInstancesStopCriterionEdit->setText
        ( QString("%1").arg(_metisConfiguration->getHMetisOption(Metis::Configuration::HMetisNRuns)) );
  }


  int  ConfigurationWidget::_hmetisCTypeToIndex ()
  {
    if ( _metisConfiguration != NULL ) {
      int value = _metisConfiguration->getHMetisOption ( Metis::Configuration::HMetisCType );

      for ( int i=0 ; i<_hmetisCTypeCombo->count() ; ++i ) {
        if ( value == _hmetisCTypeCombo->itemData(i).toInt() ) {
          return i;
        }
      }
    }

    return 0;
  }


  void  ConfigurationWidget::_updateHMetisCType ()
  {
    if ( _metisConfiguration == NULL ) return;

    int value = _hmetisCTypeCombo->itemData ( _hmetisCTypeCombo->currentIndex() ).toInt();

    _metisConfiguration->setHMetisOption ( Metis::Configuration::HMetisCType, value );
  }


  int  ConfigurationWidget::_hmetisRTypeToIndex ()
  {
    if ( _metisConfiguration != NULL ) {
      int value = _metisConfiguration->getHMetisOption ( Metis::Configuration::HMetisRType );

      for ( int i=0 ; i<_hmetisRTypeCombo->count() ; ++i ) {
        if ( value == _hmetisRTypeCombo->itemData(i).toInt() ) {
          return i;
        }
      }
    }

    return 0;
  }


  void  ConfigurationWidget::_updateHMetisRType ()
  {
    if ( _metisConfiguration == NULL ) return;

    int value = _hmetisRTypeCombo->itemData ( _hmetisRTypeCombo->currentIndex() ).toInt();

    _metisConfiguration->setHMetisOption ( Metis::Configuration::HMetisRType, value );
  }


  int  ConfigurationWidget::_hmetisVCycleToIndex ()
  {
    if ( _metisConfiguration != NULL ) {
      int value = _metisConfiguration->getHMetisOption ( Metis::Configuration::HMetisVCycle );

      for ( int i=0 ; i<_hmetisVCycleCombo->count() ; ++i ) {
        if ( value == _hmetisVCycleCombo->itemData(i).toInt() ) {
          return i;
        }
      }
    }

    return 0;
  }


  void  ConfigurationWidget::_updateHMetisVCycle ()
  {
    if ( _metisConfiguration == NULL ) return;

    int value = _hmetisVCycleCombo->itemData ( _hmetisVCycleCombo->currentIndex() ).toInt();

    _metisConfiguration->setHMetisOption ( Metis::Configuration::HMetisVCycle, value );
  }


  int  ConfigurationWidget::_hmetisReconstToIndex ()
  {
    if ( _metisConfiguration != NULL ) {
      int value = _metisConfiguration->getHMetisOption ( Metis::Configuration::HMetisReconst );

      for ( int i=0 ; i<_hmetisReconstCombo->count() ; ++i ) {
        if ( value == _hmetisReconstCombo->itemData(i).toInt() ) {
          return i;
        }
      }
    }

    return 0;
  }


  void  ConfigurationWidget::_updateHMetisReconst ()
  {
    if ( _metisConfiguration == NULL ) return;

    int value = _hmetisReconstCombo->itemData ( _hmetisReconstCombo->currentIndex() ).toInt();

    _metisConfiguration->setHMetisOption ( Metis::Configuration::HMetisReconst, value );
  }


}  // End of Mauka namespace.
