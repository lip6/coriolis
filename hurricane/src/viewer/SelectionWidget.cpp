// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./SelectionWidget.cpp"                    |
// +-----------------------------------------------------------------+


#include <QFontMetrics>
#include <QFrame>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QHeaderView>
#include <QKeyEvent>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>
#include "hurricane/Commons.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/SelectionModel.h"
#include "hurricane/viewer/SelectionWidget.h"
#include "hurricane/viewer/CellWidget.h"


namespace Hurricane {



  SelectionWidget::SelectionWidget ( QWidget* parent )
    : QWidget               (parent)
    , _cellWidget           (NULL)
    , _baseModel            (new SelectionModel(this))
    , _sortModel            (new QSortFilterProxyModel(this))
    , _view                 (new QTableView(this))
    , _filterPatternLineEdit(new QLineEdit(this))
    , _cumulative           (new QCheckBox())
    , _showSelection        (new QCheckBox())
    , _rowHeight            (20)
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );
    setContextMenuPolicy ( Qt::ActionsContextMenu );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    QLabel* filterPatternLabel = new QLabel(tr("&Filter pattern:"), this);
    filterPatternLabel->setBuddy(_filterPatternLineEdit);

    QHBoxLayout* hLayout1 = new QHBoxLayout ();
    hLayout1->addWidget ( filterPatternLabel );
    hLayout1->addWidget ( _filterPatternLineEdit );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );

    _cumulative->setText    ( tr("Cumulative Selection") );
    _cumulative->setChecked ( false );

    _showSelection->setText    ( tr("Show Selection") );
    _showSelection->setChecked ( false );

    QPushButton* clear = new QPushButton ();
    clear->setText ( tr("Clear") );

    QHBoxLayout* hLayout2 = new QHBoxLayout ();
    hLayout2->addWidget  ( _cumulative );
    hLayout2->addStretch ();
    hLayout2->addWidget  ( _showSelection );
    hLayout2->addStretch ();
    hLayout2->addWidget  ( clear );

    _sortModel->setSourceModel       ( _baseModel );
    _sortModel->setDynamicSortFilter ( true );
    _sortModel->setFilterKeyColumn   ( 1 );

    _view->setShowGrid(false);
    _view->setAlternatingRowColors(true);
    _view->setSelectionBehavior(QAbstractItemView::SelectRows);
  //_view->setTextElideMode(Qt::ElideNone);
    _view->setSortingEnabled(true);
    _view->setModel ( _sortModel );
    _view->installEventFilter(this);

    QHeaderView* horizontalHeader = _view->horizontalHeader ();
    horizontalHeader->setStretchLastSection ( true );
  //horizontalHeader->setResizeMode ( QHeaderView::ResizeToContents );
    horizontalHeader->setMinimumSectionSize ( 200 );

    QHeaderView* verticalHeader = _view->verticalHeader ();
    verticalHeader->setVisible ( false );
    verticalHeader->setDefaultSectionSize ( _rowHeight );

    QVBoxLayout* vLayout = new QVBoxLayout ();
  //vLayout->setSpacing ( 0 );
    vLayout->addLayout ( hLayout2 );
    vLayout->addWidget ( separator );
    vLayout->addWidget ( _view );
    vLayout->addLayout ( hLayout1 );

    setLayout ( vLayout );

    connect ( _filterPatternLineEdit, SIGNAL(textChanged(const QString &))
            , this                  , SLOT  (textFilterChanged()) );

    connect ( _showSelection, SIGNAL(toggled(bool))  , this      , SLOT(setShowSelection(bool)) );
    connect ( _cumulative   , SIGNAL(toggled(bool))  , this      , SLOT(setCumulativeSelection(bool)) );
    connect ( clear         , SIGNAL(clicked())      , _baseModel, SLOT(clear()) );
    connect ( clear         , SIGNAL(clicked())      , this      , SLOT(clear()) );

    connect ( _view->selectionModel(), SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&))
            , this                   , SLOT  (selectCurrent (const QModelIndex&,const QModelIndex&)) );
  //connect ( _baseModel             , SIGNAL(dataChanged   (const QModelIndex&,const QModelIndex&))
  //        , this                   , SLOT  (dataChanged   (const QModelIndex&,const QModelIndex&)) );

    QAction* toggleAction = new QAction  ( tr("&Toggle Selection"), this );
    toggleAction->setShortcut   ( QKeySequence(tr("T")) );
    toggleAction->setStatusTip  ( tr("Toggle the selection state of this Occurrence") );
    addAction ( toggleAction );

    QAction* inspectAction = new QAction  ( tr("&Inspect"), this );
    inspectAction->setShortcut   ( QKeySequence(tr("I")) );
    inspectAction->setStatusTip  ( tr("Load this Occurrence in the Inspector") );
    addAction ( inspectAction );

    connect ( toggleAction , SIGNAL(triggered()), this, SLOT(toggleSelection ()) );
    connect ( inspectAction, SIGNAL(triggered()), this, SLOT(inspect()) );

    setWindowTitle ( tr("Selection<None>") );
    resize ( 500, 300 );
  }


  void  SelectionWidget::setCellWidget ( CellWidget* cw )
  {
    if (_cellWidget) {
      disconnect( _cellWidget, 0, this       , 0 );
      disconnect( this       , 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    _baseModel->setCellWidget( cw );
    if (not _cellWidget) return;

    connect( _cellWidget, SIGNAL(selectionModeChanged()), this, SLOT(selectionModeChanged()) );
    connect( _cellWidget, SIGNAL(selectionChanged(const SelectorSet&))
           , this       , SLOT  (setSelection    (const SelectorSet&)) );
    connect( _cellWidget, SIGNAL(selectionToggled(Selector*)), this, SLOT(toggleSelection(Selector*)) );

    selectionModeChanged();
  }


  void  SelectionWidget::blockAllSignals ( bool state )
  {
    _showSelection->blockSignals ( state );
    _cumulative   ->blockSignals ( state );
    _baseModel    ->blockSignals ( state );
  }


  void  SelectionWidget::setShowSelection ( bool state )
  {
    if (not _cellWidget) return;

    blockAllSignals( true );
    _cellWidget   ->setShowSelection ( state );
    _showSelection->setChecked( state );
    _cumulative   ->setChecked( _cellWidget->getState()->cumulativeSelection() );
    blockAllSignals( false );
  }


  void  SelectionWidget::selectionModeChanged ()
  {
    setShowSelection( _cellWidget->showSelection() );
  }


  void  SelectionWidget::setCumulativeSelection ( bool state )
  {
    _cellWidget->setCumulativeSelection ( state );
  }


  bool  SelectionWidget::cumulativeSelection () const
  {
    return _cumulative->isChecked ();
  }


  void  SelectionWidget::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
  }


  void  SelectionWidget::toggleSelection ()
  {
    toggleSelection ( _view->currentIndex() );
  }


  void  SelectionWidget::toggleSelection ( const QModelIndex& index )
  {
    if (not index.isValid()) return;

    Selector* selector = _baseModel->toggleSelection( _sortModel->mapToSource(index) );
    if (selector) {
      _cellWidget->blockSignals( true );
      _cellWidget->toggleSelection ( selector->getOccurrence() );
      _cellWidget->blockSignals( false );
    }
  }


  void  SelectionWidget::toggleSelection ( Selector* selector )
  {
    _baseModel->toggleSelection( selector );
  }


  void  SelectionWidget::setSelection ( const SelectorSet& selection )
  {
    _cellWidget->openRefreshSession ();
    _baseModel->setSelection ( selection );
     
    string windowTitle = "Selection";
    Cell*  cell = (_cellWidget) ? _cellWidget->getCell() : NULL;
    if ( cell ) windowTitle += getString(cell);
    else        windowTitle += "<None>";
    setWindowTitle ( tr(windowTitle.c_str()) );

    _view->selectRow ( 0 );
    _view->resizeColumnToContents ( 0 );
    _cellWidget->closeRefreshSession ();
  }


  void  SelectionWidget::setSelection ( Selector* selector )
  { _baseModel->setSelection( selector ); }


  void  SelectionWidget::clear ()
  { 
    _baseModel->clear();
    _view->selectionModel()->clearSelection();
    if (_cellWidget)
      _cellWidget->unselectAll();
  }


  void  SelectionWidget::selectCurrent ( const QModelIndex& current, const QModelIndex& )
  {
    inspect ( current );
  }


  void  SelectionWidget::inspect ()
  {
    inspect ( _view->currentIndex() );
  }


  void  SelectionWidget::inspect ( const QModelIndex& index  )
  {
    if (not index.isValid()) {
      emit inspect ( NULL );
      return;
    }

    QModelIndex sourceIndex = _sortModel->mapToSource(index);
    const Selector* selector = _baseModel->getSelector( _sortModel->mapToSource(index).row() );
    if (not selector) {
      emit inspect ( NULL );
      return;
    }

    Occurrence occurrence = selector->getOccurrence();
    emit inspect ( occurrence );
  }


} // End of Hurricane namespace.
