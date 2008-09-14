
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
#include  <QFrame>
#include  <QLabel>
#include  <QCheckBox>
#include  <QPushButton>
#include  <QLineEdit>
#include  <QHeaderView>
#include  <QKeyEvent>
#include  <QGroupBox>
#include  <QHBoxLayout>
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
    , _baseModel(NULL)
    , _sortModel(NULL)
    , _view(NULL)
    , _cumulative(NULL)
    , _rowHeight(20)
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    _filterPatternLineEdit = new QLineEdit(this);
    QLabel* filterPatternLabel = new QLabel(tr("&Filter pattern:"), this);
    filterPatternLabel->setBuddy(_filterPatternLineEdit);

    QHBoxLayout* hLayout1 = new QHBoxLayout ();
    hLayout1->addWidget ( filterPatternLabel );
    hLayout1->addWidget ( _filterPatternLineEdit );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );

    _cumulative = new QCheckBox ();
    _cumulative->setText    ( tr("Cumulative Selection") );
    _cumulative->setChecked ( false );

    QPushButton* clear = new QPushButton ();
    clear->setText ( tr("Clear") );

    QHBoxLayout* hLayout2 = new QHBoxLayout ();
    hLayout2->addWidget  ( _cumulative );
    hLayout2->addStretch ();
    hLayout2->addWidget  ( clear );

    _baseModel = new HSelectionModel ( this );

    _sortModel = new QSortFilterProxyModel ( this );
    _sortModel->setSourceModel       ( _baseModel );
    _sortModel->setDynamicSortFilter ( true );
    _sortModel->setFilterKeyColumn   ( 0 );

    _view = new QTableView(this);
    _view->setShowGrid(false);
    _view->setAlternatingRowColors(true);
    _view->setSelectionBehavior(QAbstractItemView::SelectRows);
    _view->setSortingEnabled(true);
    _view->setModel ( _sortModel );
    _view->horizontalHeader()->setStretchLastSection ( true );

    QHeaderView* horizontalHeader = _view->horizontalHeader ();
    horizontalHeader->setStretchLastSection ( true );
    horizontalHeader->setMinimumSectionSize ( 200 );

    QHeaderView* verticalHeader = _view->verticalHeader ();
    verticalHeader->setVisible ( false );

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->addWidget ( _view );
    vLayout->addLayout ( hLayout1 );
    vLayout->addWidget ( separator );
    vLayout->addLayout ( hLayout2 );

    setLayout ( vLayout );

    connect ( _filterPatternLineEdit, SIGNAL(textChanged(const QString &))
            , this                  , SLOT(textFilterChanged())
            );
    connect ( _baseModel , SIGNAL(layoutChanged()), this, SLOT(forceRowHeight()) );
    connect ( _cumulative, SIGNAL(toggled(bool))  , this, SIGNAL(cumulativeToggled(bool)) );
    connect ( clear      , SIGNAL(clicked())      , this, SIGNAL(selectionCleared()) );
    connect ( clear      , SIGNAL(clicked())      , _baseModel, SLOT(clear()) );

    setWindowTitle ( tr("Selection<None>") );
    resize ( 500, 300 );
  }


  void  HSelection::hideEvent ( QHideEvent* event )
  {
    emit showSelected(false);
  }


  void  HSelection::forceRowHeight ()
  {
    for (  int rows=_sortModel->rowCount()-1; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
  }


  void  HSelection::keyPressEvent ( QKeyEvent* event )
  {
    if      ( event->key() == Qt::Key_I ) { runInspector    ( _view->currentIndex() ); }
    else if ( event->key() == Qt::Key_T ) { toggleSelection ( _view->currentIndex() ); }
    else event->ignore();
  }


  void  HSelection::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
    forceRowHeight ();
  }


  bool  HSelection::isCumulative () const
  {
    return _cumulative->isChecked();
  }


  void  HSelection::toggleSelection ( const QModelIndex& index )
  {
    Occurrence occurrence = _baseModel->toggleSelection ( index );
    if ( occurrence.isValid() )
      emit occurrenceToggled ( occurrence, false );
  }


  void  HSelection::toggleSelection ( Occurrence occurrence )
  {
    _baseModel->toggleSelection ( occurrence );
  }


  void  HSelection::setSelection ( const set<Selector*>& selection, Cell* cell )
  {
    _baseModel->setSelection ( selection );
     
    string windowTitle = "Selection";
    if ( cell ) windowTitle += getString(cell);
    else        windowTitle += "<None>";
    setWindowTitle ( tr(windowTitle.c_str()) );

    _view->selectRow ( 0 );
    _view->resizeColumnToContents ( 0 );
  }


  void  HSelection::runInspector ( const QModelIndex& index  )
  {
    if ( index.isValid() ) {
      Occurrence occurrence = _baseModel->getOccurrence ( _sortModel->mapToSource(index).row() );

      HInspectorWidget* inspector = new HInspectorWidget ();

      inspector->setRootRecord ( getRecord(occurrence) );
      inspector->show ();
    }
  }


} // End of Hurricane namespace.
