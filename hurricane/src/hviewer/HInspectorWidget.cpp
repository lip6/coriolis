
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
// |  C++ Module  :       "./HInspectorWidget.cpp"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <QFontMetrics>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QKeyEvent>
#include <QGroupBox>
#include <QVBoxLayout>

#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/RecordModel.h"
#include "hurricane/viewer/HInspectorWidget.h"
#include "hurricane/Slot.h"


namespace Hurricane {


  HInspectorWidget::History::History ()
    : _depth(0)
    , _slots()
    , _comboBox(NULL)
  {
  }


  HInspectorWidget::History::~History ()
  {
    clear ( true );
  }


  void  HInspectorWidget::History::push ( Slot* slot )
  {
    if ( _depth < _slots.size()-1 ) {
      while ( _depth < _slots.size()-1 ) pop ();
    }

    _depth++;
    _slots.push_back ( slot->getClone() );
    _comboBox->addItem ( QString("%1: %2").arg(_depth).arg(_slots[_slots.size()-1]->getDataString().c_str()));
    _comboBox->setCurrentIndex ( _depth );

  }


  void  HInspectorWidget::History::pop ()
  {
    if ( _slots.size() > 1 ) {
      delete _slots.back ();
      _slots.pop_back ();
      _comboBox->removeItem ( _slots.size() );
      if ( _depth == _slots.size() )
        _comboBox->setCurrentIndex ( --_depth );
    }
  }


  void  HInspectorWidget::History::back ()
  {
    if ( _depth == 0 ) return;

    _comboBox->setCurrentIndex ( --_depth );
  }


  void  HInspectorWidget::History::goTo ( int depth )
  {
    if ( ( depth < 0 ) || ( depth >= (int)_slots.size() ) ) return;

    _depth = depth;
  }


  size_t  HInspectorWidget::History::getDepth () const
  {
    return _depth;
  }


  Slot* HInspectorWidget::History::getSlot () const
  {
    return _slots[_depth]->getClone();
  }


  void  HInspectorWidget::History::clear ( bool inDelete )
  {
    if ( !_slots.empty() ) {
      _comboBox->clear ();

    // Delete the rootRecord as it's the only one not deleted
    // automatically through RecordModel (case of depth 0).
      delete _slots[0]->getDataRecord();

      for ( size_t i=0 ; i < _slots.size() ; i++ )
        delete _slots[i];

      _slots.clear ();
      _depth = 0;
    }
  }


  void  HInspectorWidget::History::setComboBox ( QComboBox* comboBox )
  {
    assert ( comboBox  != NULL );

    _comboBox = comboBox;
  }


  void  HInspectorWidget::History::setRootRecord ( Record* rootRecord )
  {
    assert ( _comboBox  != NULL );
    assert ( rootRecord != NULL );

    clear ();

    Slot* rootSlot = ::getSlot ( "<TopLevelSlot>", rootRecord );
    _slots.push_back ( rootSlot );
    _comboBox->addItem ( QString("%1: %2").arg(_depth).arg(_slots[_slots.size()-1]->getDataString().c_str()));
  }



  HInspectorWidget::HInspectorWidget ( QWidget* parent )
      : QWidget(parent)
      , _baseModel(NULL)
      , _sortModel(NULL)
      , _historyComboBox(NULL)
      , _view(NULL)
      , _rowHeight(20)
      , _history()
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    _view = new QTableView(this);
    _view->setShowGrid(false);
    _view->setAlternatingRowColors(true);
    _view->setSelectionBehavior(QAbstractItemView::SelectRows);
    _view->setSortingEnabled(true);

    QHeaderView* horizontalHeader = _view->horizontalHeader ();
    horizontalHeader->setStretchLastSection ( true );
    horizontalHeader->setMinimumSectionSize ( 200 );

    QHeaderView* verticalHeader = _view->verticalHeader ();
    verticalHeader->setVisible ( false );

    _historyComboBox = new QComboBox ( this );
    _history.setComboBox ( _historyComboBox );

    _filterPatternLineEdit = new QLineEdit(this);
    QLabel* filterPatternLabel = new QLabel(tr("&Filter pattern:"), this);
    filterPatternLabel->setBuddy(_filterPatternLineEdit);

    QGridLayout* inspectorLayout = new QGridLayout();
    inspectorLayout->addWidget(_historyComboBox      , 0, 0, 1, 2);
    inspectorLayout->addWidget(_view            , 1, 0, 1, 2);
    inspectorLayout->addWidget(filterPatternLabel    , 2, 0);
    inspectorLayout->addWidget(_filterPatternLineEdit, 2, 1);

    setLayout ( inspectorLayout );

    connect ( _filterPatternLineEdit, SIGNAL(textChanged(const QString &))
            , this                  , SLOT(textFilterChanged())
            );

    setWindowTitle(tr("Inspector"));
    resize(500, 300);
  }


  HInspectorWidget::~HInspectorWidget ()
  {
  //cerr << "HInspectorWidget::~HInspectorWidget()" << endl;
  //cerr << "Records: " << Record::getAllocateds()  << endl;
  //cerr << "Slots:   " << Slot::getAllocateds()  << endl;
  }


  void  HInspectorWidget::forceRowHeight ()
  {
    for (  int rows=_sortModel->rowCount()-1; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
  }


  void  HInspectorWidget::setRootRecord ( Record* record )
  {
    _history.setRootRecord ( record );

    if ( !_baseModel ) {
      _baseModel = new RecordModel ( this );
      _sortModel   = new QSortFilterProxyModel ( this );
      _sortModel->setSourceModel       ( _baseModel );
      _sortModel->setDynamicSortFilter ( true );
      _sortModel->setFilterKeyColumn   ( 1 );

      _view->setModel ( _sortModel );
      _view->horizontalHeader()->setStretchLastSection ( true );
      _view->resizeColumnToContents ( 0 );

    // Only after creating the RecordModel can we connect the ComboBox.
      connect ( _historyComboBox, SIGNAL(currentIndexChanged(int))
              , this            , SLOT(historyChanged(int)) );
      connect ( _baseModel, SIGNAL(layoutChanged()), this, SLOT(forceRowHeight()) );
    }

    setSlot ();
  }


  bool  HInspectorWidget::setSlot ()
  {
    bool change = true;

    change = _baseModel->setSlot ( _history.getSlot(), _history.getDepth() );
//     if ( change ) {
//       int rows = _sortModel->rowCount ();
//       for ( rows-- ; rows >= 0 ; rows-- )
//         _view->setRowHeight ( rows, _rowHeight );
//       _view->selectRow ( 0 );
//     }

    return change;
  }


  void  HInspectorWidget::pushSlot ( Slot* slot )
  {
    _history.push ( slot );
    if ( !setSlot() )
      _history.pop ();
  }


  void  HInspectorWidget::popSlot ()
  {
    _history.pop ();
    setSlot ();
  }


  void  HInspectorWidget::back ()
  {
    _history.back ();
    setSlot ();
  }


  void  HInspectorWidget::keyPressEvent ( QKeyEvent *event )
  {
    if ( event->key() == Qt::Key_Right ) {
      QModelIndex index = _view->currentIndex();
      if ( index.isValid() ) {
        Slot* slot = _baseModel->getRecord()->getSlot(_sortModel->mapToSource(index).row());

        if ( slot )
          pushSlot ( slot );
      }
    } else if ( event->key() == Qt::Key_Left ) {
      back ();
    } else if ( event->key() == Qt::Key_O ) {
      forkInspector ( _view->currentIndex() );
    } else {
      event->ignore();
    }
  }


  void  HInspectorWidget::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
    forceRowHeight ();
  }


  void  HInspectorWidget::historyChanged ( int depth )
  {
    if ( depth < 0 ) return;

    _history.goTo ( depth );
    setSlot ();
  }


  void  HInspectorWidget::forkInspector ( const QModelIndex& index )
  {
    if ( index.isValid() ) {
      Slot*   slot   = _baseModel->getRecord()->getSlot(_sortModel->mapToSource(index).row());
      Record* record = slot->getDataRecord();

      if ( record ) {
        HInspectorWidget* fork = new HInspectorWidget ();
        fork->setRootRecord ( record );
        fork->show ();
      }
    }
  }


} // End of Hurricane namespace.
