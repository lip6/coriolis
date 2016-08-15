// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2015-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./HierarchyWidget.cpp"                    |
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
#include <QTreeView>
#include "hurricane/Commons.h"
#include "hurricane/Warning.h"
#include "hurricane/Net.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/HierarchyModel.h"
#include "hurricane/viewer/HierarchyWidget.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "HierarchyWidget".


  HierarchyWidget::HierarchyWidget ( QWidget* parent )
    : QWidget               (parent)
    , _cellWidget           (NULL)
    , _cell                 (NULL)
    , _baseModel            (new HierarchyModel(this))
    , _view                 (new QTreeView(this))
    , _filterPatternLineEdit(new QLineEdit(this))
    , _rowHeight            (20)
  {
    setAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_QuitOnClose, false );
    setContextMenuPolicy( Qt::ActionsContextMenu );

    _rowHeight = QFontMetrics( Graphics::getFixedFont() ).height() + 4;

  //_view->setShowGrid            ( false );
    _view->setAlternatingRowColors( true );
    _view->setSelectionBehavior   ( QAbstractItemView::SelectRows );
    _view->setSelectionMode       ( QAbstractItemView::SingleSelection );
    _view->setSortingEnabled      ( true );
    _view->setModel               ( _baseModel );
  //_view->resizeColumnToContents ( 1 );

    QHeaderView* horizontalHeader = _view->header();
    horizontalHeader->setDefaultAlignment  ( Qt::AlignHCenter );
    horizontalHeader->setMinimumSectionSize( (Graphics::isHighDpi()) ? 600 : 300 );
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    horizontalHeader->setResizeMode        ( 0, QHeaderView::Interactive );
    horizontalHeader->setResizeMode        ( 1, QHeaderView::Interactive );
// #else
//     horizontalHeader->setSectionResizeMode ( 0, QHeaderView::Interactive );
//     horizontalHeader->setSectionResizeMode ( 1, QHeaderView::Interactive );
#endif
    horizontalHeader->setStretchLastSection( true );

    QLabel* filterPatternLabel = new QLabel(tr("&Filter pattern:"), this);
    filterPatternLabel->setBuddy( _filterPatternLineEdit );

    QGridLayout* gLayout = new QGridLayout();
    gLayout->addWidget( _view                , 1, 0, 1, 2 );
    gLayout->addWidget( filterPatternLabel   , 2, 0 );
    gLayout->addWidget(_filterPatternLineEdit, 2, 1);

    setLayout( gLayout );

    connect ( _view, SIGNAL(collapsed   (const QModelIndex&))
            , this , SLOT  (collapseHook(const QModelIndex& )) );                       
    connect ( _filterPatternLineEdit , SIGNAL(returnPressed    ())
            , this                   , SLOT  (textFilterChanged()) );                       
    connect ( _view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&))
            , this                   , SLOT  (followSelected  (const QItemSelection&,const QItemSelection&)) );

    TreeKeyFilter* filter = new TreeKeyFilter( this );
    _view->installEventFilter( filter );

    resize(300, 300);
  }


  void  HierarchyWidget::setCell ( Cell* cell )
  {
    if (_cell == cell) return;

    _cell = cell;
    _view->setVisible( false );
    _view->selectionModel()->clear();
    _baseModel->setCell( cell );
     
    string windowTitle = "Hierarchy" + getString(cell);
    setWindowTitle( tr(windowTitle.c_str()) );

    _view->selectionModel()->select( _baseModel->index(0,0,_view->rootIndex())
                                   , QItemSelectionModel::Select|QItemSelectionModel::Rows );
    _view->resizeColumnToContents( 0 );
    _view->setVisible( true );
  }


  bool  HierarchyWidget::isRSelected ( const QModelIndex& index ) const
  {
    HierarchyInfos* infos = const_cast<HierarchyInfos*>(_baseModel->infosFromIndex(index));
    for ( size_t row=0 ; row < infos->getInstances().size() ; ++row ) {
      QModelIndex childIndex  = _baseModel->index( row, 0, index );
      if (_view->selectionModel()->isSelected(childIndex)) 
        return true;

      if (not infos->isCollapsed() and isRSelected(childIndex)) return true;
    }

    return false;
  }


  void  HierarchyWidget::rexpand ( Path path )
  {
    if (path.isEmpty()) return;

    QModelIndex index = _baseModel->index( 0, 0, QModelIndex() );
    do {
      Instance* head = path.getHeadInstance();
      path = path.getTailPath();

      _view->expand( index ); 
      HierarchyInfos* parentInfos = const_cast<HierarchyInfos*>( _baseModel->infosFromIndex(index) );
      HierarchyInfos* childInfos  = NULL;

      for ( HierarchyInfos* child : parentInfos->getInstances() ) {
        if (child->getInstance() == head) {
          childInfos = child;
          break;
        }
      }
      if (not childInfos) {
        cerr << Warning("HierarchyWidget::rexpand(): discrepency between Path and Hierarchy.") << endl;
        return;
      }

      index = _baseModel->index( childInfos->getRowInParent(), 0, index );
    } while (not path.isEmpty());

    _view->selectionModel()->clear();
    _view->selectionModel()->select( index, QItemSelectionModel::Select|QItemSelectionModel::Rows );
    _view->setCurrentIndex( index );
  }


  void  HierarchyWidget::rexpand ( const QModelIndex& index )
  {
    HierarchyInfos* infos = const_cast<HierarchyInfos*>(_baseModel->infosFromIndex(index));
    if (infos) {
      if (not infos->isCollapsed()) {
        _view->expand( index );
        for ( HierarchyInfos* child : infos->getInstances() ) {
          rexpand( _baseModel->index( child->getRowInParent(), 0, index ) );
        }
      }
    }
  }


  void  HierarchyWidget::collapseHook ( const QModelIndex& index )
  {
    if (isRSelected(index)) {
      _view->selectionModel()->clear();
      _view->selectionModel()->select( index, QItemSelectionModel::Select|QItemSelectionModel::Rows );
    }
    
    HierarchyInfos* infos = const_cast<HierarchyInfos*>(_baseModel->infosFromIndex(index));
    infos->collapse();
  }


  void  HierarchyWidget::textFilterChanged ()
  {
    QModelIndexList selecteds = _view->selectionModel()->selectedIndexes();

    if (not selecteds.isEmpty()) {
      QModelIndex     index = selecteds[0];
      HierarchyInfos* infos = const_cast<HierarchyInfos*>(_baseModel->infosFromIndex(index));
      infos->setFilterPattern( _filterPatternLineEdit->text() );
      _view->reset();
      rexpand( _view->rootIndex() );
      _view->selectionModel()->select( index, QItemSelectionModel::Select|QItemSelectionModel::Rows );
      _view->setCurrentIndex( index );
    }
  }


  void  HierarchyWidget::followSelected ( const QItemSelection& selecteds, const QItemSelection& deselecteds )
  {
    if (selecteds.indexes().isEmpty()) {
    //cerr << Warning("HierarchyWidget::followSelected(): Selection should never be empty.") << endl;
      return;
    }

    QModelIndex     index = selecteds.indexes()[0];
    HierarchyInfos* infos = const_cast<HierarchyInfos*>(_baseModel->infosFromIndex(index));
    _filterPatternLineEdit->setText( infos->getFilterPattern() );
  }


  void  HierarchyWidget::goTo ( int delta )
  {
    if (delta == 0) return;

  //QModelIndex newIndex =
    _baseModel->index( _view->currentIndex().row()+delta, 0, QModelIndex() );

  //if (newIndex.isValid())
  //  _view->selectRow( newIndex.row() );
  }


// -------------------------------------------------------------------
// Class  :  "TreeKeyFilter".


  TreeKeyFilter::TreeKeyFilter ( QObject* parent )
    : QObject(parent)
  { }


  bool  TreeKeyFilter::eventFilter ( QObject* object, QEvent* event )
  {
    if (event->type() == QEvent::KeyPress) {
      HierarchyWidget* owner = qobject_cast<HierarchyWidget*>( parent() );
      if (not owner) return false;

      QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

      if (keyEvent->key() == Qt::Key_Return) {
        QModelIndexList selecteds = owner->_view->selectionModel()->selectedIndexes();

        if (not selecteds.isEmpty()) {
          QModelIndex     index = selecteds[0];
          HierarchyInfos* infos = const_cast<HierarchyInfos*>(owner->_baseModel->infosFromIndex(index));
          if (infos) {
            if (owner->getCellWidget()) {
              owner->getCellWidget()->setCell( infos->getMasterCell(), infos->getPath() );
            }
          }
        }
      }
    }

    return QObject::eventFilter( object, event );
  }


} // Hurricane namespace.
