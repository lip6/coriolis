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
#include <QAction>
#include <QModelIndex>
#include "hurricane/Commons.h"
#include "hurricane/viewer/Graphics.h"
#include "tramontana/EquipotentialsModel.h"
#include "tramontana/EquipotentialsWidget.h"


namespace Tramontana {

  using std::cerr;
  using std::endl;
  using Hurricane::Bug;
  using Hurricane::Graphics;


  EquipotentialsWidget::EquipotentialsWidget ( QWidget* parent )
    : QWidget       (parent)
    , _cellWidget   (NULL)
    , _cell         (NULL)
    , _baseModel    (new EquipotentialsModel(this))
    , _sortModel    (new QSortFilterProxyModel(this))
    , _view         (new QTableView(this))
    , _rowHeight    (20)
    , _selecteds    ()
    , _forceReselect(false)
  {
    setAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_QuitOnClose, false );
    setContextMenuPolicy( Qt::ActionsContextMenu );

    _rowHeight = QFontMetrics( Graphics::getFixedFont() ).height() + 4;

    _sortModel->setSourceModel      ( _baseModel );
    _sortModel->setDynamicSortFilter( true );
    _sortModel->setFilterKeyColumn  ( 0 );

    _view->setShowGrid            ( false );
    _view->setAlternatingRowColors( true );
    _view->setSelectionBehavior   ( QAbstractItemView::SelectRows );
    _view->setSortingEnabled      ( true );
    _view->setModel               ( _sortModel );

    QHeaderView* horizontalHeader = _view->horizontalHeader();
    horizontalHeader->setDefaultAlignment  ( Qt::AlignHCenter );
    horizontalHeader->setMinimumSectionSize( (Graphics::isHighDpi()) ? 300 : 150 );
    horizontalHeader->setStretchLastSection( true );

    QHeaderView* verticalHeader = _view->verticalHeader();
    verticalHeader->setVisible( false );
    verticalHeader->setDefaultSectionSize( _rowHeight );

    // verticalHeader->setStyleSheet( "QHeaderView::section {"
    //                                  "padding-bottom: 0px;"
    //                                  "padding-top:    0px;"
    //                                  "padding-left:   0px;"
    //                                  "padding-right:  1px;"
    //                                  "margin:         0px;"
    //                                "}"
    //                              );

    _filterPatternLineEdit = new QLineEdit( this );
    QLabel* filterPatternLabel = new QLabel( tr("&Filter pattern:"), this );
    filterPatternLabel->setBuddy( _filterPatternLineEdit );

    QGridLayout* gLayout = new QGridLayout();
    gLayout->addWidget( _view                 , 1, 0, 1, 2 );
    gLayout->addWidget( filterPatternLabel    , 2, 0 );
    gLayout->addWidget( _filterPatternLineEdit, 2, 1 );

    setLayout( gLayout );

    QAction* fitAction = new QAction( tr("&Fit to Equi"), this );
    fitAction->setShortcut ( QKeySequence(tr("CTRL+F")) );
    fitAction->setStatusTip( tr("Fit the view to the Equipotentials's bounding box") );
    addAction( fitAction );

    connect( _filterPatternLineEdit , SIGNAL(textChanged(const QString &))
           , this                   , SLOT  (textFilterChanged()) );                       
    connect( _view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&))
           , this                   , SLOT  (updateSelecteds (const QItemSelection&,const QItemSelection&)) );
  //connect( fitAction, SIGNAL(triggered()), this, SLOT(fitToEqui()) );

    resize( 300, 300 );
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
      equi = _baseModel->getEqui( _sortModel->mapToSource(iList[i]).row() );
      if ( equi )
        _selecteds.insert( equi );
    }

    if (_forceReselect) {
      _selecteds.forceInserteds();
      _forceReselect = false;
    }

    SelectedEquiSet::iterator remove;
    SelectedEquiSet::iterator isel  = _selecteds.begin ();
    while ( isel != _selecteds.end() ) {
      switch ( isel->getAccesses() ) {
        case 1:  break;
        case 64:
          emit equipotentialSelect ( isel->getEqui()->getComponents() );
          break;
        case 0:
          emit equipotentialUnselect ( isel->getEqui()->getComponents() );
          remove = isel;
          ++isel;
          _selecteds.erase( remove );
          continue;
        default:
          cerr << Bug( "EquipotentialsWidget::updateSelecteds(): invalid code %d"
                     , isel->getAccesses()) << endl;
      }
      ++isel;
    }

    if (_cellWidget) _cellWidget->closeRefreshSession ();
  }


  void  EquipotentialsWidget::textFilterChanged ()
  {
    _sortModel->setFilterRegExp( _filterPatternLineEdit->text() );
  //updateSelecteds ();
  }


  // void  EquipotentialsWidget::fitToNet ()
  // {
  //   const Equipotential* equi = _baseModel->getEqui( _sortModel->mapToSource(_view->currentIndex()).row() );
  //   if (equi) emit netFitted ( equi );
  // }


  void  EquipotentialsWidget::setCellWidget ( CellWidget* cw )
  {
    if (_cellWidget) {
      disconnect( this, 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    if (_cellWidget) {
      setCell( _cellWidget->getCell() );
    //connect( this, SIGNAL(netFitted(const Equi*)), _cellWidget, SLOT(fitToEqui (const Equi*)) );
    } else
      setCell( nullptr );
  }


  void  EquipotentialsWidget::setCell ( Cell* cell )
  {
    _cell = cell;
    _view->setVisible( false );
    _view->selectionModel()->clear();
    _baseModel->setCell( cell );
     
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
    }
    _view->setVisible( true );
  }


}
