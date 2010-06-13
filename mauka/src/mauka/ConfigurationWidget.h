
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
// |  C++ Header  :       "./ConfigurationWidget.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __MAUKA_CONFIGURATION_WIDGET__
#define  __MAUKA_CONFIGURATION_WIDGET__

#include  <QWidget>
class QLineEdit;
class QCheckBox;
class QComboBox;

namespace Nimbus {
  class Configuration;
}

namespace Metis {
  class Configuration;
}

namespace Mauka {

  class Configuration;


// -------------------------------------------------------------------
// Class  :  "Kite::ConfigurationWidget".


  class ConfigurationWidget : public QWidget {
      Q_OBJECT;
    public:
            ConfigurationWidget                   ( QWidget* parent=NULL );
    public slots:                                 
      void  setConfiguration                      ( Nimbus::Configuration*
                                                  , Metis::Configuration*
                                                  , Configuration*
                                                  );
      void  _applySettings                        ();
    // Metis relateds.
      void  _updatePartOrKWayHMetis               ();
      void  _updateNumberOfInstancesStopCriterion ();
      void  _updateGlobalConnectionsWeightRatio   ();
      void  _updateUbFactor                       ();
      void  _updateHMetisCustomOptions            ();
      void  _updateHMetisNRuns                    ();
      void  _updateHMetisCType                    ();
      void  _updateHMetisRType                    ();
      void  _updateHMetisVCycle                   ();
      void  _updateHMetisReconst                  ();
    // Nimbus/Mauka relateds.
      void  _updateStandardAnnealingState         ();
      void  _updateIgnorePinsState                ();
      void  _updatePlotBinsState                  ();
      void  _updatePinsPlacementState             ();
      void  _updateSearchRatio                    ();
      void  _updateAnnealingNetMult               ();
      void  _updateAnnealingBinMult               ();
      void  _updateAnnealingRowMult               ();
      void  _updateAspectRatio                    ();
      void  _updateMargin                         ();
    private slots:
      void  _enableHMetisOptions                  ( int );
    private:
      int   _hmetisCTypeToIndex                   ();
      int   _hmetisRTypeToIndex                   ();
      int   _hmetisVCycleToIndex                  ();
      int   _hmetisReconstToIndex                 ();
    private:
      Nimbus::Configuration* _nimbusConfiguration;
      Metis::Configuration*  _metisConfiguration;
      Configuration*         _maukaConfiguration;
      QCheckBox*             _partOrKWayHMetisState;
      QCheckBox*             _standardAnnealingState;
      QCheckBox*             _ignorePinsState;
      QCheckBox*             _plotBinsState;
      QCheckBox*             _pinsPlacementState;
      QCheckBox*             _hmetisCustomOptionsState;
      QLineEdit*             _hmetisNRunsEdit;
      QComboBox*             _hmetisCTypeCombo;
      QComboBox*             _hmetisRTypeCombo;
      QComboBox*             _hmetisVCycleCombo;
      QComboBox*             _hmetisReconstCombo;
      QLineEdit*             _numberOfInstancesStopCriterionEdit;
      QLineEdit*             _globalConnectionsWeightRatioEdit;
      QLineEdit*             _ubFactorEdit;
      QLineEdit*             _searchRatioEdit;
      QLineEdit*             _annealingNetMultEdit;
      QLineEdit*             _annealingBinMultEdit;
      QLineEdit*             _annealingRowMultEdit;
      QLineEdit*             _aspectRatioEdit;
      QLineEdit*             _marginEdit;
  };



} // End of Mauka namespace.


#endif  // __MAUKA_CONFIGURATION_WIDGET__
