
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
// |  C++ Header  :       "./SelectionPopup.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_SELECTION_POPUP_WIDGET_H__
#define  __HURRICANE_SELECTION_POPUP_WIDGET_H__


#include  <QWidget>
#include  <QTableView>

#include  "hurricane/Commons.h"
#include  "hurricane/Occurrence.h"


class QModelIndex;
class QTableView;
class QComboBox;
class QHeaderView;


namespace Hurricane {


  class SelectionPopupModel;


  class SelectionPopup : public QWidget {
      Q_OBJECT;

    public:
                                     SelectionPopup    ( QWidget* parent=NULL );
              void                   updateLayout       ();
              void                   popup              ();
    signals:
              void                   occurrenceSelected ( Occurrence occurrence, bool fromPopup );
    public slots:
              void                   add                ( Occurrence occurrence, bool showChange=false );
              void                   clear              ();
              void                   forceRowHeight     ();
    protected:
      virtual void                   keyPressEvent      ( QKeyEvent * event );
      virtual void                   mouseMoveEvent     ( QMouseEvent* event );
      virtual void                   mouseReleaseEvent  ( QMouseEvent* event );

    private:
              SelectionPopupModel*  _model;
              QTableView*            _view;
              int                    _rowHeight;
  };


} // End of Hurricane namespace.


#endif // __HURRICANE_SELECTION_POPUP_WIDGET_H__
