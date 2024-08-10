// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
// |  E-mail       :            Jean-Paul.Chaput@lip6.fr             |
// | =============================================================== |
// |  C++ Header   :  "./EquipotentialsWidget.cpp"                   |
// +-----------------------------------------------------------------+


#include <QFontMetrics>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QKeyEvent>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QSplitter>
#include <QAction>
#include <QModelIndex>
#include "hurricane/Commons.h"
#include "hurricane/viewer/Graphics.h"
#include "tramontana/EquipotentialsWidget.h"


namespace Tramontana {

  using std::cerr;
  using std::endl;
  using Hurricane::Bug;
  using Hurricane::Graphics;


// -------------------------------------------------------------------
// Class  :  "BuriedFilterProxymodel".


  EquiFilterProxyModel::EquiFilterProxyModel ( QObject* parent )
    : Super  (parent)
    , _filter(NoFilter)
  { }


  void  EquiFilterProxyModel::setFilter ( uint32_t filter )
  { _filter = filter; invalidateFilter(); }

  
  bool  EquiFilterProxyModel::filterAcceptsRow ( int row, const QModelIndex& index ) const
  {
    EquipotentialsModel* model = dynamic_cast<EquipotentialsModel*>( sourceModel() );
    if (not model) return true;
    
    const Equipotential* equi = model->getEqui( row );
    if (not (_filter & ShowBuried) and equi->isBuried()) return false;
    return true;
  }


// -------------------------------------------------------------------
// Class  :  "EquipotentialsWidget".


  EquipotentialsWidget::EquipotentialsWidget ( QWidget* parent )
    : QWidget               (parent)
    , _cellWidget           (nullptr)
    , _cell                 (nullptr)
    , _baseModel            (new EquipotentialsModel(this))
    , _sortModel            (new QSortFilterProxyModel(this))
    , _filterModel          (new EquiFilterProxyModel(this))
    , _openModel            (new OpenCircuitModel(this))
    , _view                 (new QTableView(this))
    , _openCircuits         (new QTreeView(this))
    , _filterPatternLineEdit(new QLineEdit(this))
    , _equiDisplay          (new EquipotentialWidget(this))
    , _rowHeight            (20)
    , _selecteds            ()
    , _forceReselect        (false)
    , _flags                (InternalEmit)
  {
    setAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_QuitOnClose, false );
    setContextMenuPolicy( Qt::ActionsContextMenu );

    _rowHeight = QFontMetrics( Graphics::getFixedFont() ).height() + 4;

    _filterModel->setSourceModel    ( _baseModel );
  //_filterModel->setFilter         ( EquiFilterProxyModel::ShowBuried );
    _sortModel->setSourceModel      ( _filterModel );
    _sortModel->setDynamicSortFilter( true );
    _sortModel->setFilterKeyColumn  ( 0 );

    _view->setShowGrid            ( false );
    _view->setAlternatingRowColors( true );
    _view->setSelectionBehavior   ( QAbstractItemView::SelectRows );
    _view->setSortingEnabled      ( true );
    _view->setModel               ( _sortModel );

    _openCircuits->setSelectionBehavior( QAbstractItemView::SelectRows );
    _openCircuits->setSelectionMode    ( QAbstractItemView::SingleSelection );
    _openCircuits->setModel            (  _openModel );

    QHeaderView* horizontalHeader = _view->horizontalHeader();
    horizontalHeader->setDefaultAlignment  ( Qt::AlignHCenter );
    horizontalHeader->setMinimumSectionSize( (Graphics::isHighDpi()) ? 150 : 75 );
    horizontalHeader->setStretchLastSection( true );

    QHeaderView* verticalHeader = _view->verticalHeader();
    verticalHeader->setVisible( false );
    verticalHeader->setDefaultSectionSize( _rowHeight );

    QLabel* filterPatternLabel = new QLabel( tr("&Filter pattern:"), this );
    filterPatternLabel->setBuddy( _filterPatternLineEdit );

    QWidget*     equiTable = new QWidget();
    QGridLayout* gLayout   = new QGridLayout();
    gLayout->addWidget( _view                 , 1, 0, 1, 2 );
    gLayout->addWidget( filterPatternLabel    , 2, 0 );
    gLayout->addWidget( _filterPatternLineEdit, 2, 1 );
    equiTable->setLayout( gLayout );

    QSplitter* splitter = new QSplitter ();
    splitter->setOrientation( Qt::Vertical );
    splitter->addWidget( equiTable );
    splitter->addWidget( _equiDisplay );
    splitter->addWidget( _openCircuits );

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget( splitter );
    setLayout( vLayout );

    QAction* fitAction = new QAction( tr("&Fit to Equi"), this );
    fitAction->setShortcut ( QKeySequence(tr("CTRL+F")) );
    fitAction->setStatusTip( tr("Fit the view to the Equipotentials's bounding box") );
    addAction( fitAction );

    connect( _filterPatternLineEdit , SIGNAL(textChanged(const QString &))
           , this                   , SLOT  (textFilterChanged()) );                       
    connect( _view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&))
           , this                   , SLOT  (updateSelecteds (const QItemSelection&,const QItemSelection&)) );
    connect( _openCircuits->selectionModel(), SIGNAL(selectionChanged   (const QItemSelection&,const QItemSelection&))
           , this                           , SLOT  (updateSelectedsOpen(const QItemSelection&,const QItemSelection&)) );
    connect( fitAction, SIGNAL(triggered()), this, SLOT(fitToEqui()) );

    resize( 300, 300 );
  }


  QModelIndex  EquipotentialsWidget::mapToSource ( QModelIndex viewIndex ) const
  { return _filterModel->mapToSource( _sortModel->mapToSource( viewIndex )); }


  void  EquipotentialsWidget::changeSelectionMode ()
  {
    if (not _cellWidget) return;

    if (isInternalEmit()) {
      setInternalReceive();
      emit selectionModeChanged();
    } else {
      if (isExternalEmit()) {
        blockAllSignals( true );
        // _showSelection->setChecked( _cellWidget->getState()->showSelection() );
        blockAllSignals( false );
      }
    }
    setExternalEmit();
  }


  void  EquipotentialsWidget::blockAllSignals ( bool state )
  {
    // _showSelection->blockSignals( state );
    // _baseModel    ->blockSignals( state );
  }


  void  EquipotentialsWidget::setShowBuried ( bool state )
  {
    _filterModel->setFilter( (state) ? EquiFilterProxyModel::ShowBuried
                                     : EquiFilterProxyModel::NoFilter );
  }


  void  EquipotentialsWidget::goTo ( int delta )
  {
    if ( delta == 0 ) return;

    QModelIndex newIndex = _sortModel->index( _view->currentIndex().row()+delta, 0, QModelIndex() );
    if (newIndex.isValid())
      _view->selectRow( newIndex.row() );
  }


  void  EquipotentialsWidget::updateSelecteds ()
  {
    _forceReselect = true;

    QItemSelection  dummy;
    updateSelecteds( dummy, dummy );
  }


  void  EquipotentialsWidget::updateSelecteds ( const QItemSelection& , const QItemSelection& )
  {
    if (_cellWidget) _cellWidget->openRefreshSession ();

    _selecteds.resetAccesses ();

    const Equipotential* equi  = nullptr;
    QModelIndexList      iList = _view->selectionModel()->selectedRows();
    for ( int i=0 ; i<iList.size() ; i++ ) {
      equi = _baseModel->getEqui( mapToSource(iList[i]).row() );
      if (equi) {
        _selecteds.insert( equi );
        if (i == 0) _equiDisplay->setEquipotential( equi );
      }
    }

    if (_forceReselect) {
      _selecteds.forceInserteds();
      _forceReselect = false;
    }

    SelectedEquiSet::iterator isel = _selecteds.begin ();
    while ( isel != _selecteds.end() ) {
      SelectedEquiSet::iterator remove = isel++;
      if (remove->getAccesses() == 0) {
        emit equipotentialUnselect ( remove->getEqui()->getFlatComponents() );
        _selecteds.erase( remove );
      }
    }
    isel = _selecteds.begin ();
    while ( isel != _selecteds.end() ) {
      if (isel->getAccesses() == 64) {
        emit equipotentialSelect ( isel->getEqui()->getFlatComponents() );
      }
      ++isel;
    }
    isel = _selecteds.begin ();

    if (_cellWidget) _cellWidget->closeRefreshSession ();
  }


  void  EquipotentialsWidget::textFilterChanged ()
  {
    _sortModel->setFilterRegExp( _filterPatternLineEdit->text() );
  //updateSelecteds ();
  }


  void  EquipotentialsWidget::fitToEqui ()
  {
    const Equipotential* equi = _baseModel->getEqui( mapToSource(_view->currentIndex()).row() );
    if (equi) emit reframe ( equi->getBoundingBox() );
  }


  void  EquipotentialsWidget::setCellWidget ( CellWidget* cw )
  {
    if (_cellWidget) {
      disconnect( this, 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    if (_cellWidget) {
      setCell( _cellWidget->getCell() );
      connect( this, SIGNAL( reframe(const Box&) ), _cellWidget, SLOT( reframe(const Box&) ));
      _equiDisplay->setCellWidget( cw );
    } else {
      setCell( nullptr );
      _equiDisplay->setCellWidget( nullptr );
    }
  }


  void  EquipotentialsWidget::setCell ( Cell* cell )
  {
    _cell = cell;
    _view->setVisible( false );
    _view->selectionModel()->clear();
    _baseModel->setCell( cell );

    TramontanaEngine* tramontana = nullptr;
    if (cell) tramontana = TramontanaEngine::get( cell );
    _openModel->setTramontana( tramontana );
     
    string windowTitle = "Equis" + getString(cell);
    setWindowTitle( tr(windowTitle.c_str()) );

    QHeaderView* header = _view->horizontalHeader();

    _view->selectRow( 0 );
    for ( int i=0 ; i<_baseModel->columnCount() ; ++i ) {
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
      header->setSectionResizeMode( i, QHeaderView::Interactive );
#else
      header->setResizeMode( i, QHeaderView::Interactive );
#endif
      _view->resizeColumnToContents( i );
      _view->sortByColumn( 1, Qt::AscendingOrder );
      _view->sortByColumn( 0, Qt::DescendingOrder );
    }
    _view->setVisible( true );
    _openCircuits->resizeColumnToContents( 0 );
  }


  void  EquipotentialsWidget::updateSelectedsOpen ( const QItemSelection& , const QItemSelection& )
  {
    if (not _cellWidget) return;

    _cellWidget->openRefreshSession();
    _cellWidget->unselectAll();
    _cellWidget->setShowSelection( true );

    QModelIndexList iList = _openCircuits->selectionModel()->selectedRows();
    if (not iList.empty()) {
      for ( int i=0 ; i<iList.size() ; i++ ) {
        const OpenCircuitAbstractItem* item = reinterpret_cast<OpenCircuitAbstractItem*>( iList[i].internalPointer() );
        if (item->getType() == OpenCircuitAbstractItem::TypeNet) {
          const Net* net = item->getOpenCircuit().first;
          emit netSelect ( Occurrence( net ));
          emit reframe   ( item->getBoundingBox() );
        } else if (item->getType() == OpenCircuitAbstractItem::TypeEqui) {
          emit equipotentialSelect ( item->getEquipotential()->getFlatComponents() );
          emit reframe ( item->getBoundingBox() );
        }
      }
    }

    _cellWidget->closeRefreshSession();
  }


}
