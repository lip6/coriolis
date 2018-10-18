// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :          Jean-Paul Chaput, Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/SlicingDataWidget.h"                    |
// +-----------------------------------------------------------------+


#ifndef  BORA_SLICING_DATAS_WIDGET_H
#define  BORA_SLICING_DATAS_WIDGET_H

#include <QWidget>
#include <QModelIndex>
class QTableView;
class QSortFilterProxyModel;
namespace Hurricane {
  class CellViewer;
}
#include "bora/SlicingNode.h"


namespace Bora {

  using Hurricane::CellViewer;
  class SlicingDataModel;


  class SlicingDataWidget : public QWidget {
      Q_OBJECT;
    public:
              SlicingDataWidget   ( QWidget* parent=NULL );
             ~SlicingDataWidget   ();
      void    setViewer           ( CellViewer* );
      bool    eventFilter         ( QObject*, QEvent* );
      void    updateContents      ();
      BoxSet* getBoxSet           ( int index );
    public slots:                 
      void    columnPressed       ( int column );
      void    cellClicked         ( const QModelIndex& index );
      void    updateSelectedRow   ( double x, double y );
    signals:
      void    updatePlacement     ( BoxSet* boxSet );
      void    updateSelectedPoint ( double x, double y );
    private:
      CellViewer*            _viewer;
      SlicingDataModel*      _baseModel;
      QSortFilterProxyModel* _sortModel;
      QTableView*            _view;
      int                    _rowHeight;
  };


}  // Bora namespace.

#endif  // BORA_SLICING_DATAS_WIDGET_H
