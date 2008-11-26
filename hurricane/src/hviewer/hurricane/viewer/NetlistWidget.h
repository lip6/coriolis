
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
// |  C++ Header  :       "./NetlistWidget.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_NETLIST_WIDGET__
#define  __HURRICANE_NETLIST_WIDGET__


#include  <QWidget>
#include  <QTableView>
#include  <QSortFilterProxyModel>

#include  "hurricane/Commons.h"
#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/NetlistModel.h"


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;


namespace Hurricane {


  class Net;
  class Cell;


  class NetlistWidget : public QWidget {
      Q_OBJECT;

    public:
                                     NetlistWidget     ( QWidget* parent=NULL );
      inline  Cell*                  getCell           ();
      template<typename InformationType>
              void                   setCell           ( Cell* );
              void                   goTo              ( int );
    signals:
              void                   netSelected       ( const Net* );
    public slots:
              void                   forceRowHeight    ();
    private slots:
              void                   textFilterChanged ();
              void                   selectNet         ( const QModelIndex& );
              void                   selectCurrent     ( const QModelIndex& , const QModelIndex& );                                                           
    protected:
              void                   keyPressEvent     ( QKeyEvent* );

    private:
              Cell*                  _cell;
              NetlistModel*          _baseModel;
              QSortFilterProxyModel* _sortModel;
              QTableView*            _view;
              QLineEdit*             _filterPatternLineEdit;
              int                    _rowHeight;
  };


  template<typename InformationType>
  void  NetlistWidget::setCell ( Cell* cell )
  {
    _cell = cell;
    _baseModel->setCell<InformationType> ( cell );
     
    string windowTitle = "Netlist" + getString(cell);
    setWindowTitle ( tr(windowTitle.c_str()) );

    int rows = _sortModel->rowCount ();
    for ( rows-- ; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
    _view->selectRow ( 0 );
    _view->resizeColumnToContents ( 0 );
  }


  inline  Cell* NetlistWidget::getCell () { return _cell; }


} // End of Hurricane namespace.


#endif // __HURRICANE_NETLIST_WIDGET_H__
