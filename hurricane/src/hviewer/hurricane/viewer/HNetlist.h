
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
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
// |  C++ Header  :       "./HNetlist.h"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HNETLIST_WIDGET_H__
#define  __HNETLIST_WIDGET_H__


#include  <QWidget>
#include  <QTableView>
#include  <QSortFilterProxyModel>

#include  "hurricane/Commons.h"
#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/HNetlistModel.h"


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;


namespace Hurricane {


  class Cell;
  class CellWidget;


  class HNetlist : public QWidget {
      Q_OBJECT;

    public:
                                     HNetlist          ( QWidget* parent=NULL );
      template<typename InformationType>
              void                   setCell           ( Cell* cell );
      template<typename InformationType>
              void                   setCellWidget     ( CellWidget* cw );
              void                   runInspector      ( const QModelIndex& index  );
    public slots:
              void                   forceRowHeight    ();
    private slots:
              void                   textFilterChanged ();
              void                   selectNet         ( const QModelIndex& index );
    protected:
              void                   keyPressEvent     ( QKeyEvent * event );

    private:
              HNetlistModel*         _baseModel;
              QSortFilterProxyModel* _sortModel;
              QTableView*            _view;
              QLineEdit*             _filterPatternLineEdit;
              int                    _rowHeight;
              CellWidget*            _cellWidget;
  };


  template<typename InformationType>
  void  HNetlist::setCell ( Cell* cell )
  {
    _baseModel->setCell<InformationType> ( cell );
     
    string windowTitle = "Netlist" + getString(cell);
    setWindowTitle ( tr(windowTitle.c_str()) );

    int rows = _sortModel->rowCount ();
    for ( rows-- ; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
    _view->selectRow ( 0 );
    _view->resizeColumnToContents ( 0 );
  }


  template<typename InformationType>
  void  HNetlist::setCellWidget ( CellWidget* cw )
  {
    if ( _baseModel->getCell() != cw->getCell() )
      setCell<InformationType>( cw->getCell() );

    _cellWidget = cw;
  }


} // End of Hurricane namespace.


#endif // __HNETLIST_WIDGET_H__
