
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
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
#include "hurricane/viewer/InspectorWidget.h"
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


  void  InspectorWidget::History::push ( Slot* slot, Record* record )
  {
    if ( _depth < _slots.size()-1 ) {
      while ( _depth < _slots.size()-1 ) pop ();
    }

    _depth++;
    _slots.push_back ( slot->getClone() );
    _comboBox->addItem ( QString("%1: %2").arg(_depth).arg(_slots[_slots.size()-1]->getDataString().c_str()));
    _comboBox->setCurrentIndex ( _depth );

  //cerr << "After History::push()" << endl;
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
    if ( ( depth < 0 ) or ( (size_t)depth >= _slots.size() ) ) return;
    if ( (size_t)depth != _depth ) {
      _depth = (size_t)depth;
      _comboBox->setCurrentIndex ( _depth );
    }
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
    : QWidget         (parent)
    , _baseModel      (NULL)
    , _sortModel      (NULL)
    , _historyComboBox(NULL)
    , _view           (NULL)
    , _rowHeight      (20)
    , _history        ()
    , _rootOccurrence ()
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    _view = new QTableView(this);
    _view->setShowGrid(false);
    _view->setAlternatingRowColors(true);
    _view->setSelectionBehavior(QAbstractItemView::SelectRows);
    _view->setSortingEnabled(true);
    _view->installEventFilter(this);

    QHeaderView* horizontalHeader = _view->horizontalHeader ();
    horizontalHeader->setStretchLastSection ( true );
    horizontalHeader->setMinimumSectionSize ( 200 );

    QHeaderView* verticalHeader = _view->verticalHeader ();
    verticalHeader->setVisible ( false );
    verticalHeader->setDefaultSectionSize ( _rowHeight );

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


  void  InspectorWidget::setRootOccurrence ( Occurrence& occurrence )
  {
    _rootOccurrence = occurrence;
    _setRootRecord ( getRecord(occurrence) );
  }


  void  InspectorWidget::setRootRecord ( Record* record )
  {
    _rootOccurrence = Occurrence();
    _setRootRecord ( record );
  }


  void  InspectorWidget::_setRootRecord ( Record* record )
  {
  //cerr << "InspectorWidget::_setRootRecord()." << endl;
  //if ( _baseModel ) _baseModel->setSlot ( NULL, 0 );

    if ( record == NULL ) _rootOccurrence = Occurrence ();

    if ( _baseModel == NULL ) {
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
              , this            , SLOT  (historyChanged(int)) );
    }

    _history.setRootRecord ( record );
  }


  bool  InspectorWidget::setSlot ( Record* record )
  {
    bool change = true;

    if ( (_history.getSlot() != NULL) and (record == NULL) )
      record = _history.getSlot()->getDataRecord();

  //cerr << "  Effective setSlot() ." << endl;
    change = _baseModel->setSlot ( _history.getSlot(), record, _history.getDepth() );

    return change;
  }


  void  InspectorWidget::pushSlot ( Slot* slot, Record* record )
  {
  //cerr << "InspectorWidget::pushSlot()" << endl;

    if ( slot   == NULL ) return;
    if ( record == NULL ) {
      record = slot->getDataRecord ();
      if ( record == NULL ) return;
    }

    _history.push ( slot, record );
    setSlot ( record );
  }


  void  InspectorWidget::popSlot ()
  {
    _history.pop ();
  //setSlot ();
  }


  void  InspectorWidget::back ()
  {
    _history.back ();
  //setSlot ();
  }


  bool  InspectorWidget::eventFilter ( QObject* object, QEvent* event )
  {
    if ( event->type() == QEvent::KeyPress ) {
      QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

      if ( keyEvent->key() == Qt::Key_Right ) {
        QModelIndex index = _view->currentIndex();
        if ( index.isValid() ) {
        //cerr << "Key Right: do to sub-slot." << endl;
          Slot* slot = _baseModel->getRecord()->getSlot(_sortModel->mapToSource(index).row());

          if ( slot != NULL )
            pushSlot ( slot, slot->getDataRecord() );
        }
      } else if ( keyEvent->key() == Qt::Key_Left ) {
        back ();
      } else if ( keyEvent->key() == Qt::Key_O ) {
        forkInspector ( _view->currentIndex() );
      }
    }

    return QObject::eventFilter ( object, event );
  }


  void  InspectorWidget::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
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
