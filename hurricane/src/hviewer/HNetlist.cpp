
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
// |  C++ Module  :       "./HNetlist.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>

#include <QFontMetrics>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QKeyEvent>
#include <QGroupBox>
#include <QVBoxLayout>

#include "hurricane/Commons.h"
#include "hurricane/Net.h"

#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/HNetlistModel.h"
#include "hurricane/viewer/HNetlist.h"


namespace Hurricane {



  HNetlist::HNetlist ( QWidget* parent )
      : QWidget(parent)
      , _netlistModel(NULL)
      , _sortModel(NULL)
      , _netlistView(NULL)
      , _rowHeight(20)
      , _cellWidget(NULL)
  {
    setAttribute ( Qt::WA_DeleteOnClose );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    _netlistModel = new HNetlistModel ( this );

    _sortModel = new QSortFilterProxyModel ( this );
    _sortModel->setSourceModel       ( _netlistModel );
    _sortModel->setDynamicSortFilter ( true );
    _sortModel->setFilterKeyColumn   ( 0 );

    _netlistView = new QTableView(this);
    _netlistView->setShowGrid(false);
    _netlistView->setAlternatingRowColors(true);
    _netlistView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _netlistView->setSortingEnabled(true);
    _netlistView->setModel ( _sortModel );
    _netlistView->horizontalHeader()->setStretchLastSection ( true );
    _netlistView->resizeColumnToContents ( 0 );

    QHeaderView* horizontalHeader = _netlistView->horizontalHeader ();
    horizontalHeader->setStretchLastSection ( true );
    horizontalHeader->setMinimumSectionSize ( 200 );

    QHeaderView* verticalHeader = _netlistView->verticalHeader ();
    verticalHeader->setVisible ( false );

    _filterPatternLineEdit = new QLineEdit(this);
    QLabel* filterPatternLabel = new QLabel(tr("&Filter pattern:"), this);
    filterPatternLabel->setBuddy(_filterPatternLineEdit);

    QGridLayout* inspectorLayout = new QGridLayout();
    inspectorLayout->addWidget(_netlistView          , 1, 0, 1, 2);
    inspectorLayout->addWidget(filterPatternLabel    , 2, 0);
    inspectorLayout->addWidget(_filterPatternLineEdit, 2, 1);

    setLayout ( inspectorLayout );

    connect ( _filterPatternLineEdit, SIGNAL(textChanged(const QString &))
            , this                  , SLOT(textFilterChanged())
            );
    connect ( _netlistView          , SIGNAL(activated(const QModelIndex&))
            , this                  , SLOT(selectNet(const QModelIndex&))
            );

    setWindowTitle(tr("Netlist"));
    resize(500, 300);

  }


  void  HNetlist::selectNet ( const QModelIndex& index )
  {
    const Net* net = _netlistModel->getNet ( _sortModel->mapToSource(index).row() );
    
    if ( _cellWidget && net ) {
      _cellWidget->unselectAll ();
      _cellWidget->select ( net );
    }
  }


  void  HNetlist::keyPressEvent ( QKeyEvent* event )
  {
    cerr << "keyPressEvent" << endl;

    if ( event->key() == Qt::Key_Left ) {
      cerr << "Key Left Pressed." << endl;
    } else {
      event->ignore();
    }
  }


  void  HNetlist::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
  }


} // End of Hurricane namespace.
