// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/SelectionPopup.h"      |
// +-----------------------------------------------------------------+


#pragma  once
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
  class CellWidget;


  class SelectionPopup : public QWidget {
      Q_OBJECT;
    public:
                    SelectionPopup    ( QWidget* parent=NULL );
      inline  void  setCellWidget     ( CellWidget* );
              void  updateLayout      ();
              void  showPopup         ();
              void  clearFilter       ();
              void  setFilter         ( OccurrenceFilter );
    signals:
              void  selectionToggled  ( Occurrence );
    public slots:
              void  loadOccurrences   ( Occurrences, bool showChange=false );
              void  clear             ();
    protected:
      virtual void  keyPressEvent     ( QKeyEvent * );
      virtual void  mouseMoveEvent    ( QMouseEvent* );
      virtual void  mousePressEvent   ( QMouseEvent* );
    private:
      CellWidget*           _cellWidget;
      SelectionPopupModel*  _model;
      QTableView*           _view;
      int                   _rowHeight;
      int                   _charWidth;
  };


  inline  void  SelectionPopup::setCellWidget ( CellWidget* cellWidget ) { _cellWidget=cellWidget; }


} // Hurricane namespace.
