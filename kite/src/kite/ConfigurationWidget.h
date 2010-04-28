
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
// |  C++ Header  :       "./ConfigurationWidget.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __CONFIGURATION_WIDGET__
#define  __CONFIGURATION_WIDGET__

#include  <QWidget>
class QLineEdit;
class QSpinBox;

namespace Kite {

  class Configuration;


// -------------------------------------------------------------------
// Class  :  "Kite::ConfigurationWidget".


  class ConfigurationWidget : public QWidget {
      Q_OBJECT;
    public:
            ConfigurationWidget        ( QWidget* parent=NULL );
    public slots:
      void  setConfiguration           ( Configuration* );
      void  _updateSaturateRatio       ();
      void  _updateGlobalThreshold     ();
      void  _updateEdgeCapacity        ();
      void  _updateExpandStep          ();
      void  _updateEventsLimit         ();
      void  _updateRipupCost           (int);
      void  _updateBorderRipupLimit    (int);
      void  _updateStrapRipupLimit     (int);
      void  _updateLocalRipupLimit     (int);
      void  _updateGlobalRipupLimit    (int);
      void  _updateLongGlobalRipupLimit(int);
    private:
      Configuration* _configuration;
      QLineEdit*     _saturateRatioEdit;
      QLineEdit*     _globalThresholdEdit;
      QLineEdit*     _edgeCapacityEdit;
      QLineEdit*     _expandStepEdit;
      QLineEdit*     _eventsLimitEdit;
      QSpinBox*      _ripupCostSb;
      QSpinBox*      _borderRipupLimitSb;
      QSpinBox*      _strapRipupLimitSb;
      QSpinBox*      _localRipupLimitSb;
      QSpinBox*      _globalRipupLimitSb;
      QSpinBox*      _longGlobalRipupLimitSb;
  };



} // End of Kite namespace.


#endif  // __CONFIGURATION_WIDGET__
