// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/ViewsWidget.h"                       |
// +-----------------------------------------------------------------+


#ifndef  CRL_VIEWS_WIDGET_H
#define  CRL_VIEWS_WIDGET_H

#include <QWidget>
#include <QTableView>
#include <QItemDelegate>
#include <QSortFilterProxyModel>
#include "crlcore/AllianceLibrary.h"
namespace Hurricane {
  class Cell;
}

class QLabel;
class QModelIndex;
class QTableView;
class QHeaderView;


namespace CRL {

  using Hurricane::Cell;
  class ViewsModel;
  class CellDatas;


// -------------------------------------------------------------------
// Class  :  "ViewsWidget".

  class ViewsWidget : public QWidget {
      Q_OBJECT;
    public:
                         ViewsWidget  ( QWidget* parent=NULL );
      inline ViewsModel* getModel     () const;
    public slots:
             void        setCellDatas ( CellDatas* );
    private:
      ViewsModel*  _baseModel;
      QTableView*  _view;
      int          _rowHeight;
      QLabel*      _note;
  };


  inline ViewsModel* ViewsWidget::getModel () const { return _baseModel; }


}  // CRL namepsace.

#endif  // CRL_VIEWS_WIDGET_H
