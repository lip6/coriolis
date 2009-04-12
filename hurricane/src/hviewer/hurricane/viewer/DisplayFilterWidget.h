
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
// |  C++ Header  :       "./DisplayFilterWidget.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_DISPLAY_FILTER_WIDGET__
#define  __HURRICANE_DISPLAY_FILTER_WIDGET__


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
      void        layoutModeChanged    ();
    public slots:                      
      void        changeQueryFilter    ();
      void        changeLayoutMode     ();
      void        startLevelChanged    ( int level );
      void        stopLevelChanged     ( int level );
      void        setDoMasterCells     ( int state );
      void        setDoComponents      ( int state );
      void        setDoTerminalCells   ( int state );
      void        setRubberSteiner     ();
      void        setRubberCentric     ();
      void        setRubberBarycentric ();
      void        setSymbolicMode      ();
      void        setRealMode          ();
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
      QRadioButton*  _realMode;
      UpdateState    _updateState;
  };


} // End of Hurricane namespace.


#endif
