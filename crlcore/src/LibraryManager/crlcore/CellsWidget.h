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
// |  C++ Header  :  "./crlcore/CellsWidget.h"                       |
// +-----------------------------------------------------------------+


#ifndef  CRL_CELLS_WIDGET_H
#define  CRL_CELLS_WIDGET_H

#include <QWidget>
#include <QTableView>
#include <QItemDelegate>
#include <QSortFilterProxyModel>
#include "crlcore/AllianceLibrary.h"

class QModelIndex;
class QTableView;
class QHeaderView;


namespace CRL {

  class CellDatas;
  class CellsModel;


// -------------------------------------------------------------------
// Class  :  "CellsWidget".


  class CellsWidget : public QWidget {
      Q_OBJECT;
    public:
                         CellsWidget    ( QWidget* parent=NULL );
      inline CellsModel* getModel       () const;
    signals:              
             void        selectedCell   ( CellDatas* );
    public slots:         
             void        updateSelected ();
             void        updateLibrary  ();
             void        selectCurrent  ( const QModelIndex& index, const QModelIndex& );
             void        setLibrary     ( const AllianceLibrary* );
    private:
      CellsModel*  _baseModel;
      QTableView*  _view;
      int          _rowHeight;
      QModelIndex  _selected;
  };


  inline CellsModel* CellsWidget::getModel () const { return _baseModel; }


}  // CRL namespace.

#endif  // CRL_CELLS_WIDGET_H
