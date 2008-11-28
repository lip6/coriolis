
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
// |  C++ Header  :       "./NetlistWidget.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_NETLIST_WIDGET__
#define  __HURRICANE_NETLIST_WIDGET__


#include  <set>

#include  <QWidget>
#include  <QTableView>
#include  <QSortFilterProxyModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Bug.h"
#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/NetlistModel.h"


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


  class SelectedNet {
    public:
      inline            SelectedNet   ();
      inline            SelectedNet   ( const Net*, size_t access=0 );
      inline const Net* getNet        () const;
      inline size_t     getAccesses   () const;
      inline void       incAccesses   () const;
      inline void       resetAccesses () const;
    private:
              const Net* _net;
      mutable size_t     _accesses;
  };


  inline            SelectedNet::SelectedNet   () : _net(NULL), _accesses(0) { }
  inline            SelectedNet::SelectedNet   ( const Net* net, size_t accesses ) : _net(net), _accesses(accesses) { }
  inline const Net* SelectedNet::getNet        () const { return _net; }
  inline size_t     SelectedNet::getAccesses   () const { return _accesses; }
  inline void       SelectedNet::incAccesses   () const { ++_accesses; }
  inline void       SelectedNet::resetAccesses () const { _accesses = 0; }


  struct SelectedNetCompare {
      inline bool operator() ( const SelectedNet& lhs, const SelectedNet& rhs );
  };


  inline bool SelectedNetCompare::operator() ( const SelectedNet& lhs, const SelectedNet& rhs )
  {
    return lhs.getNet()->getName() < rhs.getNet()->getName();
  }


  class SelectedNetSet : public set<SelectedNet,SelectedNetCompare>{
    public:
      void  insert        ( const Net* );
      void  resetAccesses ();
  };


  inline void  SelectedNetSet::insert ( const Net* net )
  {
    iterator  iselected = find(SelectedNet(net));
    if ( iselected != end() )
      iselected->incAccesses ();
    else
      set<SelectedNet,SelectedNetCompare>::insert ( SelectedNet(net,64) );
  }


  inline void  SelectedNetSet::resetAccesses ()
  {
    for ( iterator iselected=begin() ; iselected != end() ; ++iselected )
      iselected->resetAccesses ();
  }


  class NetlistWidget : public QWidget {
      Q_OBJECT;

    public:
                                     NetlistWidget     ( QWidget* parent=NULL );
      inline  Cell*                  getCell           ();
      template<typename InformationType>
              void                   setCell           ( Cell* );
              void                   goTo              ( int );
    signals:
              void                   netSelected       ( const Net* );
              void                   netUnselected     ( const Net* );
    public slots:
              void                   forceRowHeight    ();
    private slots:
              void                   textFilterChanged ();
    //        void                   selectNet         ( const QModelIndex& );
    //        void                   selectCurrent     ( const QModelIndex&    , const QModelIndex& );
              void                   updateSelecteds   ( const QItemSelection& , const QItemSelection& );
    protected:
              void                   keyPressEvent     ( QKeyEvent* );

    private:
              Cell*                  _cell;
              NetlistModel*          _baseModel;
              QSortFilterProxyModel* _sortModel;
              QTableView*            _view;
              QLineEdit*             _filterPatternLineEdit;
              int                    _rowHeight;
              SelectedNetSet         _selecteds;
  };


  template<typename InformationType>
  void  NetlistWidget::setCell ( Cell* cell )
  {
    _cell = cell;
    _baseModel->setCell<InformationType> ( cell );
     
    string windowTitle = "Netlist" + getString(cell);
    setWindowTitle ( tr(windowTitle.c_str()) );

    int rows = _sortModel->rowCount ();
    for ( rows-- ; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
    _view->selectRow ( 0 );
    _view->resizeColumnToContents ( 0 );
  }


  inline  Cell* NetlistWidget::getCell () { return _cell; }


} // End of Hurricane namespace.


#endif // __HURRICANE_NETLIST_WIDGET_H__
