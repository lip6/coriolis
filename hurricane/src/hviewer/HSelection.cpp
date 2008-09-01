
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
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./HSelection.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QFontMetrics>
#include  <QLabel>
#include  <QLineEdit>
#include  <QHeaderView>
#include  <QKeyEvent>
#include  <QGroupBox>
#include  <QVBoxLayout>

#include "hurricane/Commons.h"
#include "hurricane/Cell.h"

#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/HSelectionModel.h"
#include "hurricane/viewer/HSelection.h"
#include "hurricane/viewer/HInspectorWidget.h"


namespace Hurricane {



  HSelection::HSelection ( QWidget* parent )
    : QWidget(parent)
    , _selectionModel(NULL)
    , _sortModel(NULL)
    , _selectionView(NULL)
    , _rowHeight(20)
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    _selectionModel = new HSelectionModel ( this );

    _sortModel = new QSortFilterProxyModel ( this );
    _sortModel->setSourceModel       ( _selectionModel );
    _sortModel->setDynamicSortFilter ( true );
    _sortModel->setFilterKeyColumn   ( 0 );

    _selectionView = new QTableView(this);
    _selectionView->setShowGrid(false);
    _selectionView->setAlternatingRowColors(true);
    _selectionView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _selectionView->setSortingEnabled(true);
    _selectionView->setModel ( _sortModel );
    _selectionView->horizontalHeader()->setStretchLastSection ( true );

    QHeaderView* horizontalHeader = _selectionView->horizontalHeader ();
    horizontalHeader->setStretchLastSection ( true );
    horizontalHeader->setMinimumSectionSize ( 200 );

    QHeaderView* verticalHeader = _selectionView->verticalHeader ();
    verticalHeader->setVisible ( false );

    _filterPatternLineEdit = new QLineEdit(this);
    QLabel* filterPatternLabel = new QLabel(tr("&Filter pattern:"), this);
    filterPatternLabel->setBuddy(_filterPatternLineEdit);

    QGridLayout* inspectorLayout = new QGridLayout();
    inspectorLayout->addWidget(_selectionView        , 1, 0, 1, 2);
    inspectorLayout->addWidget(filterPatternLabel    , 2, 0);
    inspectorLayout->addWidget(_filterPatternLineEdit, 2, 1);

    setLayout ( inspectorLayout );

    connect ( _filterPatternLineEdit, SIGNAL(textChanged(const QString &))
            , this                  , SLOT(textFilterChanged())
            );

    setWindowTitle(tr("Selection<None>"));
    resize(500, 300);
  }


  void  HSelection::keyPressEvent ( QKeyEvent* event )
  {
    if ( event->key() == Qt::Key_I ) { runInspector(_selectionView->currentIndex()); }
    else event->ignore();
  }


  void  HSelection::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
  }


  void  HSelection::addToSelection ( Selector* selector )
  {
    _selectionModel->addToSelection ( selector );
    int rows = _sortModel->rowCount () - 1;
    _selectionView->setRowHeight ( rows, _rowHeight );
  }


  void  HSelection::setSelection ( const set<Selector*>& selection, Cell* cell )
  {
    _selectionModel->setSelection ( selection );
     
    string windowTitle = "Selection";
    if ( cell ) windowTitle += getString(cell);
    else        windowTitle += "<None>";
    setWindowTitle ( tr(windowTitle.c_str()) );

    int rows = _sortModel->rowCount ();
    for ( rows-- ; rows >= 0 ; rows-- )
      _selectionView->setRowHeight ( rows, _rowHeight );
    _selectionView->selectRow ( 0 );
    _selectionView->resizeColumnToContents ( 0 );
  }


  void  HSelection::runInspector ( const QModelIndex& index  )
  {
    if ( index.isValid() ) {
      Occurrence occurrence = _selectionModel->getOccurrence ( _sortModel->mapToSource(index).row() );

      HInspectorWidget* inspector = new HInspectorWidget ();

      inspector->setRootRecord ( getRecord(occurrence) );
      inspector->show ();
    }
  }


} // End of Hurricane namespace.
