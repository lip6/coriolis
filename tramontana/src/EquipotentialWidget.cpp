// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
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
// |  C++ Header   :  "./EquipotentialWidget.cpp"                    |
// +-----------------------------------------------------------------+


#include <QFontMetrics>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QSplitter>
#include <QAction>
#include <QModelIndex>
#include "hurricane/Commons.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/NetsetWidget.h"
#include "tramontana/ShortCircuit.h"
#include "tramontana/EquipotentialsModel.h"
#include "tramontana/EquipotentialsWidget.h"


namespace Tramontana {

  using std::cerr;
  using std::endl;
  using Hurricane::Bug;
  using Hurricane::Graphics;
  using Hurricane::SimpleNetInformations;


// -------------------------------------------------------------------
// Class  :  "EquipotentialWidget".


  EquipotentialWidget::EquipotentialWidget ( QWidget* parent )
    : QWidget       (parent)
    , _cellWidget   (nullptr)
    , _equipotential(nullptr)
    , _equiName     (new QLabel())
    , _netset       (new NetsetWidget())
    , _shortWidget  (new QTreeView())
    , _shortModel   ()
  {
    setAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_QuitOnClose, false );
    setContextMenuPolicy( Qt::ActionsContextMenu );

    _equiName->setText( "No equipotential selected." );
    _shortWidget->setSelectionBehavior( QAbstractItemView::SelectRows );
    _shortWidget->setSelectionMode    ( QAbstractItemView::SingleSelection );
    _shortWidget->setModel( &_shortModel );

    QSplitter* splitter = new QSplitter ();
    splitter->setOrientation( Qt::Vertical );
    splitter->addWidget( _equiName );
    splitter->addWidget( _netset );
    splitter->addWidget( _shortWidget );

    QGridLayout* gLayout = new QGridLayout();
    gLayout->addWidget( splitter, 0, 0, 1, 2 );

    setLayout( gLayout );

    connect( _shortWidget->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&))
           , this                          , SLOT  (updateSelecteds (const QItemSelection&,const QItemSelection&)) );
  }


  void  EquipotentialWidget::setCellWidget ( CellWidget* cw )
  {
    if (_cellWidget) {
      disconnect( this   , 0, _cellWidget, 0 );
      disconnect( _netset, 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    if (_cellWidget) {
      connect( this   , SIGNAL(reframe  (const Box&))          , _cellWidget, SLOT(reframe  (const Box&)) );
      connect( this   , SIGNAL(select   (Occurrence))          , _cellWidget, SLOT(select   (Occurrence)) );
      connect( this   , SIGNAL(selectSet(const OccurrenceSet&)), _cellWidget, SLOT(selectSet(const OccurrenceSet&)) );
      connect( _netset, SIGNAL(netSelected  (Occurrence))      , _cellWidget, SLOT(select   (Occurrence)) );
      connect( _netset, SIGNAL(netUnselected(Occurrence))      , _cellWidget, SLOT(unselect (Occurrence)) );
    }
    _netset->setCellWidget( cw );
  }


  void  EquipotentialWidget::setEquipotential ( const Equipotential* equi )
  {
    if (_equipotential == equi) return;

    QString name = "No equipotential selected.";
    _netset->clear();
    _shortModel.clear();
    _equipotential = equi;
    if (equi) {
      name = getString( _equipotential->getName() ).c_str();
      for ( auto& netDatas : _equipotential->getNets() )
        _netset->add<SimpleNetInformations>( netDatas.first );
    }
    _netset->resizeColumnToContents( 0 );
    _shortModel.setEqui( _equipotential );
    _shortWidget->resizeColumnToContents( 0 );
    _equiName->setText( name );
  }


  void  EquipotentialWidget::updateSelecteds ( const QItemSelection& , const QItemSelection& )
  {
    if (not _cellWidget) return;

    _cellWidget->openRefreshSession();
    _cellWidget->unselectAll();
    _cellWidget->setShowSelection( true );

    QModelIndexList iList = _shortWidget->selectionModel()->selectedRows();
    if (not iList.empty()) {
      for ( int i=0 ; i<iList.size() ; i++ ) {
        const ShortCircuitAbstractItem* item = reinterpret_cast<ShortCircuitAbstractItem*>( iList[i].internalPointer() );
        if (item->getType() == ShortCircuitAbstractItem::CompA) {
          emit select ( item->getShortCircuit()->getOccurrenceA() );
          emit reframe ( item->getBoundingBox() );
        } else if (item->getType() == ShortCircuitAbstractItem::CompB) {
          emit select ( item->getShortCircuit()->getOccurrenceB() );
          emit reframe ( item->getBoundingBox() );
        } else if (item->getType() == ShortCircuitAbstractItem::Overlap) {
          OccurrenceSet selecteds;
          selecteds.insert( item->getShortCircuit()->getOccurrenceA() );
          selecteds.insert( item->getShortCircuit()->getOccurrenceB() );
          selecteds.insert( item->getShortCircuit()->getOccurrenceO() );
          emit selectSet ( selecteds );
          Box shortingBox = item->getBoundingBox();
          shortingBox.inflate( shortingBox.getWidth(), shortingBox.getHeight() );
          emit reframe ( shortingBox );
        }
      }
    }

    _cellWidget->closeRefreshSession();
  }
  


}  // Tramontana namespace.
