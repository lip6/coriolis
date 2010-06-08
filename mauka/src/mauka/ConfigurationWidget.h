
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

namespace Nimbus {
  class Configuration;
}

namespace Mauka {

  class Configuration;


// -------------------------------------------------------------------
// Class  :  "Kite::ConfigurationWidget".


  class ConfigurationWidget : public QWidget {
      Q_OBJECT;
    public:
            ConfigurationWidget           ( QWidget* parent=NULL );
    public slots:
      void  setConfiguration              ( Nimbus::Configuration*, Configuration* );
      void  _applySettings                ();
      void  _updateStandardAnnealingState ();
      void  _updateIgnorePinsState        ();
      void  _updatePlotBinsState          ();
      void  _updatePinsPlacementState     ();
      void  _updateSearchRatio            ();
      void  _updateAnnealingNetMult       ();
      void  _updateAnnealingBinMult       ();
      void  _updateAnnealingRowMult       ();
      void  _updateAspectRatio            ();
      void  _updateMargin                 ();
    private:
      Nimbus::Configuration* _nimbusConfiguration;
      Configuration*         _maukaConfiguration;
      QCheckBox*             _standardAnnealingState;
      QCheckBox*             _ignorePinsState;
      QCheckBox*             _plotBinsState;
      QCheckBox*             _pinsPlacementState;
      QLineEdit*             _searchRatioEdit;
      QLineEdit*             _annealingNetMultEdit;
      QLineEdit*             _annealingBinMultEdit;
      QLineEdit*             _annealingRowMultEdit;
      QLineEdit*             _aspectRatioEdit;
      QLineEdit*             _marginEdit;
  };



} // End of Mauka namespace.


#endif  // __MAUKA_CONFIGURATION_WIDGET__
