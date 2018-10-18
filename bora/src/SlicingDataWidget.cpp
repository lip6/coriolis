// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :          Jean-Paul Chaput, Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./SlicingDataWidget.cpp"                       |
// +-----------------------------------------------------------------+


#include <iostream>
#include <QFontMetrics>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/CellViewer.h"
#include "bora/SlicingDataModel.h"
#include "bora/SlicingDataWidget.h"
#include "bora/GraphicBoraEngine.h"


namespace Bora {

  using namespace std;
  using Hurricane::Graphics;
  using Analog::AnalogCellExtension;


  SlicingDataWidget::SlicingDataWidget ( QWidget* parent )
    : QWidget   (parent)
    , _viewer   (NULL)
    , _baseModel(NULL)
    , _sortModel(NULL)
    , _view     (NULL)
    , _rowHeight(20)
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    _view = new QTableView                ( this  );
    _view->setShowGrid                    ( false );
    _view->setAlternatingRowColors        ( true  );
    _view->setSelectionBehavior           ( QAbstractItemView::SelectRows );
    _view->setSelectionMode               ( QAbstractItemView::SingleSelection );
    _view->setSortingEnabled              ( true  );
    _view->installEventFilter             ( this  );
    _view->viewport()->installEventFilter ( this  );

    QHeaderView* horizontalHeader = _view->horizontalHeader ();
  //horizontalHeader->setStretchLastSection ( true );
    horizontalHeader->setMinimumSectionSize ( 20 );
  //horizontalHeader->setResizeMode         ( QHeaderView::ResizeToContents );
  //horizontalHeader->setDefaultSectionSize ( 100 );
    horizontalHeader->setSortIndicatorShown(true);
    horizontalHeader->setStretchLastSection(true);
      
    QHeaderView* verticalHeader = _view->verticalHeader ();
    verticalHeader->setVisible ( false );
    verticalHeader->setDefaultSectionSize ( _rowHeight );

    _baseModel = new SlicingDataModel( this );
    _sortModel = new QSortFilterProxyModel( this );
    _sortModel->setSourceModel      ( _baseModel );
    _sortModel->setDynamicSortFilter( true );
  //_sortModel->setFilterKeyColumn  ( 1 );

    _view->setModel( _sortModel );
  //_view->horizontalHeader()->setStretchLastSection( true );
  //_view->resizeColumnToContents( 0 );
  //_view->model()->setCurrentIndex()

    QGridLayout* slicingDataLayout = new QGridLayout();
    slicingDataLayout->addWidget(_view, 0, 0, 1, 1);

    setLayout( slicingDataLayout );

    setWindowTitle( tr("SlicingTree Datas") );
    resize( 500, 300 );

    connect( _view->horizontalHeader () , SIGNAL(sectionClicked(int)), this, SLOT(columnPressed(int)) );
    connect( _view                      , SIGNAL(clicked(const QModelIndex &)), this, SLOT(cellClicked(const QModelIndex &)) );
    
  }


  SlicingDataWidget::~SlicingDataWidget ()
  { }


  void  SlicingDataWidget::setViewer ( CellViewer* viewer )
  {
    if (_viewer) {
      disconnect( _viewer, 0, this   , 0 );
      disconnect( this   , 0, _viewer, 0 );
    }

    _viewer = viewer;
    _baseModel->setCell(_viewer->getCellWidget()->getCell());
  }


  void  SlicingDataWidget::columnPressed ( int column )
  {
    _view->sortByColumn(column);
  }


  void  SlicingDataWidget::cellClicked (const QModelIndex& index)
  {
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _viewer->getCellWidget()->getCell() );
    if ( index.isValid() ) {
      cdebug_log(540,0) << "SlicingDatasWidget::cellClicked(): index=("
                        << index.column() << "," << index.row() << ")" << endl;

      BoxSet* boxSet = _baseModel->getBoxSet((_view->model()->data(index, Qt::UserRole)).toInt());
      emit updatePlacement( boxSet );
      emit updateSelectedPoint( DbU::getPhysical(slicingtree->getWidth() , DbU::Micro)
                              , DbU::getPhysical(slicingtree->getHeight(), DbU::Micro) 
                              );
    }
  }


  BoxSet* SlicingDataWidget::getBoxSet ( int index )
  { return _baseModel->getBoxSet( index ); }


  void  SlicingDataWidget::updateSelectedRow ( double x, double y )
  {
    QAbstractItemModel* model = _view->model();
    int row = 0;
    for( int irow = 0; irow < model->rowCount(); ++irow ) {
      QModelIndex idxH = model->index( irow, 1 );
      QModelIndex idxW = model->index( irow, 2 );
      if ( (_view->model()->data(idxH).toDouble() == y) && (_view->model()->data(idxW).toDouble() == x) ){ row = irow; break; }
    }
    _view->selectRow(row);
  }


  bool SlicingDataWidget::eventFilter ( QObject* object, QEvent* event )
  {
    if ( event->type() == QEvent::KeyPress ) {
      QModelIndex index = _view->currentIndex();
      if ( index.isValid() ) {
        BoxSet* boxSet = _baseModel->getBoxSet((_view->model()->data(index, Qt::UserRole)).toInt());
        emit updatePlacement( boxSet );
      }
    }
    return QObject::eventFilter( object, event );
  }


  void SlicingDataWidget::updateContents()
  {
    _baseModel->updateContents();
  }


}  // Bora namespace.
