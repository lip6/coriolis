// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/SelectionPopup.h"      |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_SELECTION_POPUP_WIDGET_H
#define  HURRICANE_SELECTION_POPUP_WIDGET_H

#include <QWidget>
#include <QTableView>
#include "hurricane/Commons.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Occurrences.h"


class QModelIndex;
class QTableView;
class QComboBox;
class QHeaderView;


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::SelectionPopup".


  class SelectionPopupModel;


  class SelectionPopup : public QWidget {
      Q_OBJECT;

    public:
                    SelectionPopup    ( QWidget* parent=NULL );
              void  updateLayout      ();
              void  popup             ();
              void  clearFilter       ();
              void  setFilter         ( OccurrenceFilter );
    signals:
              void  selectionToggled  ( Occurrence );
    public slots:
              void  loadOccurrences   ( Occurrences, bool showChange=false );
              void  clear             ();
              void  forceRowHeight    ();
    protected:
      virtual void  keyPressEvent     ( QKeyEvent * );
      virtual void  mouseMoveEvent    ( QMouseEvent* );
      virtual void  mouseReleaseEvent ( QMouseEvent* );

    private:
      SelectionPopupModel*  _model;
      QTableView*           _view;
      int                   _rowHeight;
      int                   _charWidth;
  };


} // Hurricane namespace.

#endif // HURRICANE_SELECTION_POPUP_WIDGET_H
