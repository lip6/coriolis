// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./DisplayFilterWidget.h"                  |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_DISPLAY_FILTER_WIDGET_H
#define  HURRICANE_DISPLAY_FILTER_WIDGET_H


#include  <QWidget>

class QSpinBox;
class QCheckBox;
class QRadioButton;

#include  "hurricane/viewer/CellWidget.h"


namespace Hurricane {


  class DisplayFilterWidget : public QWidget {
      Q_OBJECT;

    public:
                  DisplayFilterWidget  ( QWidget* parent=NULL );
      void        setCellWidget        ( CellWidget* );
    signals:                           
      void        queryFilterChanged   ();
      void        dbuModeChanged       ( unsigned int mode, DbU::UnitPower );
    public slots:                      
      void        changeQueryFilter    ();
      void        changeDbuMode        ( unsigned int mode, DbU::UnitPower );
      void        startLevelChanged    ( int level );
      void        stopLevelChanged     ( int level );
      void        setDoMasterCells     ( int state );
      void        setDoComponents      ( int state );
      void        setDoTerminalCells   ( int state );
      void        setRubberSteiner     ();
      void        setRubberCentric     ();
      void        setRubberBarycentric ();
      void        setSymbolicMode      ();
      void        setGridMode          ();
      void        setNanoMode          ();
      void        setMicroMode         ();
    protected:
      void        blockAllSignals      ( bool state );

    protected:
      CellWidget*    _cellWidget;
      QSpinBox*      _startSpinBox;
      QSpinBox*      _stopSpinBox;
      QCheckBox*     _doMasterCells;
      QCheckBox*     _doTerminalCells;
      QCheckBox*     _doComponents;
      QRadioButton*  _steiner;
      QRadioButton*  _centric;
      QRadioButton*  _barycentric;
      QRadioButton*  _symbolicMode;
      QRadioButton*  _gridMode;
      QRadioButton*  _nanoMode;
      QRadioButton*  _microMode;
      UpdateState    _updateState;
  };


} // Hurricane namespace.

#endif  // HURRICANE_DISPLAY_FILTER_WIDGET_H 
