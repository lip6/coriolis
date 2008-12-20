
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


namespace Hurricane {


  class CellWidget;


  class DisplayFilterWidget : public QWidget {
      Q_OBJECT;

    public:
                  DisplayFilterWidget ( QWidget* parent=NULL );
      void        setCellWidget       ( CellWidget* );
                                      
    signals:                          
      void        filterChanged       ();
                                      
    public slots:                     
      void        startLevelChanged   ( int level );
      void        stopLevelChanged    ( int level );
      void        setDoMasterCells    ( int state );
      void        setDoComponents     ( int state );
      void        setDoTerminalCells  ( int state );

    protected:
      CellWidget* _cellWidget;
      QSpinBox*   _startSpinBox;
      QSpinBox*   _stopSpinBox;
      QCheckBox*  _doMasterCells;
      QCheckBox*  _doTerminalCells;
      QCheckBox*  _doComponents;
      int         _queryFilter;
  };


} // End of Hurricane namespace.


#endif
