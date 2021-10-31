
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
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/NetlistWidget.h"       |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_NETLIST_WIDGET_H
#define  HURRICANE_NETLIST_WIDGET_H

#include <set>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QItemDelegate>
#include <QSortFilterProxyModel>
#include "hurricane/Commons.h"
#include "hurricane/Bug.h"
#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/NetlistModel.h"
#include "hurricane/viewer/CellWidget.h"


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;


namespace Hurricane {


  using std::set;
  class Net;
  class Cell;


// -------------------------------------------------------------------
// Class  :  "SelectedNet".


  class SelectedNet {
    public:
      inline            SelectedNet   ();
      inline            SelectedNet   ( const Net*, size_t access=0 );
      inline const Net* getNet        () const;
      inline size_t     getAccesses   () const;
      inline void       incAccesses   () const;
      inline void       setInserted   () const;
      inline void       resetAccesses () const;
    private:
              const Net* _net;
      mutable size_t     _accesses;
  };


  inline            SelectedNet::SelectedNet   () : _net(NULL), _accesses(0) { }
  inline            SelectedNet::SelectedNet   ( const Net* net, size_t accesses ) : _net(net), _accesses(accesses) { }
  inline const Net* SelectedNet::getNet        () const { return _net; }
  inline void       SelectedNet::setInserted   () const { _accesses = 64; }
  inline size_t     SelectedNet::getAccesses   () const { return _accesses; }
  inline void       SelectedNet::incAccesses   () const { ++_accesses; }
  inline void       SelectedNet::resetAccesses () const { _accesses = 0; }


  struct SelectedNetCompare {
      inline bool operator() ( const SelectedNet& lhs, const SelectedNet& rhs ) const;
  };


  inline bool SelectedNetCompare::operator() ( const SelectedNet& lhs, const SelectedNet& rhs ) const
  {
    return lhs.getNet()->getName() < rhs.getNet()->getName();
  }


// -------------------------------------------------------------------
// Class  :  "SelectedNetSet".


  class SelectedNetSet : public set<SelectedNet,SelectedNetCompare>{
    public:
      void  insert         ( const Net* );
      void  forceInserteds ();
      void  resetAccesses  ();
  };


  inline void  SelectedNetSet::insert ( const Net* net )
  {
    iterator  iselected = find(SelectedNet(net));
    if ( iselected != end() )
      iselected->incAccesses ();
    else
      set<SelectedNet,SelectedNetCompare>::insert ( SelectedNet(net,64) );
  }


  inline void  SelectedNetSet::forceInserteds ()
  {
    for ( iterator iselected=begin() ; iselected != end() ; ++iselected )
      iselected->setInserted ();
  }


  inline void  SelectedNetSet::resetAccesses ()
  {
    for ( iterator iselected=begin() ; iselected != end() ; ++iselected )
      iselected->resetAccesses ();
  }


// -------------------------------------------------------------------
// Class  :  "NetlistWidget".


  class NetlistWidget : public QWidget {
      Q_OBJECT;

    public:
                                     NetlistWidget        ( QWidget* parent=NULL );
      inline  Cell*                  getCell              ();
      template<typename InformationType>                  
              void                   setCellWidget        ( CellWidget* );
      template<typename InformationType>                  
              void                   setCell              ( Cell* );
              void                   goTo                 ( int );
              void                   updateSelecteds      ();
    signals:
              void                   netSelected          ( Occurrence );
              void                   netUnselected        ( Occurrence );
              void                   netFitted            ( const Net* );
    private slots:                                        
              void                   textFilterChanged    ();
              void                   updateSelecteds      ( const QItemSelection& , const QItemSelection& );
              void                   fitToNet             ();

    private:
              CellWidget*            _cellWidget;
              Cell*                  _cell;
              NetlistModel*          _baseModel;
              QSortFilterProxyModel* _sortModel;
              QTableView*            _view;
              QLineEdit*             _filterPatternLineEdit;
              int                    _rowHeight;
              SelectedNetSet         _selecteds;
              bool                   _forceReselect;
  };


  template<typename InformationType>
  void  NetlistWidget::setCellWidget ( CellWidget* cw )
  {
    if ( _cellWidget ) {
      disconnect ( this, 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    if ( _cellWidget ) {
      setCell<InformationType> ( _cellWidget->getCell() );
      connect ( this, SIGNAL(netFitted(const Net*)), _cellWidget, SLOT(fitToNet (const Net*)) );
    } else
      setCell<InformationType> ( NULL );
  }


  template<typename InformationType>
  void  NetlistWidget::setCell ( Cell* cell )
  {
    _cell = cell;
    _view->setVisible( false );
    _view->selectionModel()->clear();
    _baseModel->setCell<InformationType>( cell );
     
    string windowTitle = "Netlist" + getString(cell);
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


  inline  Cell* NetlistWidget::getCell () { return _cell; }


} // Hurricane namespace.

#endif // HURRICANE_NETLIST_WIDGET_H
