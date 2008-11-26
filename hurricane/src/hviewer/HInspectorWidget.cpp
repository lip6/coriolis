
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
// |  C++ Module  :       "./InspectorWidget.cpp"                    |
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
//#include "hurricane/viewer/InspectorWidget.h"
#include "hurricane/viewer/HInspectorWidget.h"
#include "hurricane/Slot.h"


namespace Hurricane {


  InspectorWidget::History::History ()
    : _depth(0)
    , _slots()
    , _comboBox(NULL)
  {
  }


  InspectorWidget::History::~History ()
  {
    clear ( true );
  }


  void  InspectorWidget::History::push ( Slot* slot )
  {
    if ( _depth < _slots.size()-1 ) {
      while ( _depth < _slots.size()-1 ) pop ();
    }

    _depth++;
    _slots.push_back ( slot->getClone() );
    _comboBox->addItem ( QString("%1: %2").arg(_depth).arg(_slots[_slots.size()-1]->getDataString().c_str()));
    _comboBox->setCurrentIndex ( _depth );

  }


  void  InspectorWidget::History::pop ()
  {
    if ( _slots.size() > 1 ) {
      delete _slots.back ();
      _slots.pop_back ();
      _comboBox->removeItem ( _slots.size() );
      if ( _depth == _slots.size() )
        _comboBox->setCurrentIndex ( --_depth );
    }
  }


  void  InspectorWidget::History::back ()
  {
    if ( _depth == 0 ) return;

    _comboBox->setCurrentIndex ( --_depth );
  }


  void  InspectorWidget::History::goTo ( int depth )
  {
    if ( ( depth < 0 ) || ( depth >= (int)_slots.size() ) ) return;

    _depth = depth;
  }


  size_t  InspectorWidget::History::getDepth () const
  {
    return _depth;
  }


  Slot* InspectorWidget::History::getSlot () const
  {
    if ( !_slots[_depth] ) return NULL;
    return _slots[_depth]->getClone();
  }


  void  InspectorWidget::History::clear ( bool inDelete )
  {
    if ( !_slots.empty() ) {
      _comboBox->clear ();

    // Delete the rootRecord as it's the only one not deleted
    // automatically through RecordModel (case of depth 0).
      if ( _slots[0] )
        delete _slots[0]->getDataRecord();

      for ( size_t i=0 ; i < _slots.size() ; i++ )
        delete _slots[i];

      _slots.clear ();
      _depth = 0;
    }
  }


  void  InspectorWidget::History::setComboBox ( QComboBox* comboBox )
  {
    assert ( comboBox  != NULL );

    _comboBox = comboBox;
  }


  void  InspectorWidget::History::setRootRecord ( Record* rootRecord )
  {
    assert ( _comboBox  != NULL );

    clear ();

    if ( rootRecord ) {
      Slot* rootSlot = ::getSlot ( "<TopLevelSlot>", rootRecord );
      _slots.push_back ( rootSlot );
      _comboBox->addItem ( QString("%1: %2").arg(_depth).arg(_slots[_slots.size()-1]->getDataString().c_str()));
    } else {
      _slots.push_back ( NULL );
      _comboBox->addItem ( "<Inspector Disabled>" );
    }
  }



  InspectorWidget::InspectorWidget ( QWidget* parent )
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


  InspectorWidget::~InspectorWidget ()
  {
  //cerr << "InspectorWidget::~InspectorWidget()" << endl;
  //cerr << "Records: " << Record::getAllocateds()  << endl;
  //cerr << "Slots:   " << Slot::getAllocateds()  << endl;
  }


  void  InspectorWidget::forceRowHeight ()
  {
    for (  int rows=_sortModel->rowCount()-1; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
  }


  void  InspectorWidget::setRootRecord ( Record* record )
  {
    _history.setRootRecord ( record );

    if ( !_baseModel ) {
      _baseModel = new RecordModel ( this );
      _sortModel = new QSortFilterProxyModel ( this );
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


  bool  InspectorWidget::setSlot ()
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


  void  InspectorWidget::pushSlot ( Slot* slot )
  {
    _history.push ( slot );
    if ( !setSlot() )
      _history.pop ();
  }


  void  InspectorWidget::popSlot ()
  {
    _history.pop ();
    setSlot ();
  }


  void  InspectorWidget::back ()
  {
    _history.back ();
    setSlot ();
  }


  void  InspectorWidget::keyPressEvent ( QKeyEvent *event )
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


  void  InspectorWidget::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
    forceRowHeight ();
  }


  void  InspectorWidget::historyChanged ( int depth )
  {
    if ( depth < 0 ) return;

    _history.goTo ( depth );
    setSlot ();
  }


  void  InspectorWidget::forkInspector ( const QModelIndex& index )
  {
    if ( index.isValid() ) {
      Slot*   slot   = _baseModel->getRecord()->getSlot(_sortModel->mapToSource(index).row());
      Record* record = slot->getDataRecord();

      if ( record ) {
        InspectorWidget* fork = new InspectorWidget ();
        fork->setRootRecord ( record );
        fork->show ();
      }
    }
  }


} // End of Hurricane namespace.
