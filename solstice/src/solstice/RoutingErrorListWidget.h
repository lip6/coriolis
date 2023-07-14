
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
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./solstice/RoutingErrorListWidget.h.h"    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_ROUTINGERRORLIST_WIDGET__
#define  __HURRICANE_ROUTINGERRORLIST_WIDGET__


#include  <set>

#include  <QWidget>
#include  <QTableView>
#include  <QSortFilterProxyModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Bug.h"
#include  "hurricane/viewer/CellWidget.h"
#include  "solstice/RoutingErrorListModel.h"
#include  "hurricane/viewer/CellWidget.h"


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;

namespace Hurricane {

  class Cell;

}

namespace Solstice {


  using std::set;
  class RoutingError;



// -------------------------------------------------------------------
// Class  :  "SelectedRoutingError".


  class SelectedRoutingError {
    public:
      inline            SelectedRoutingError   ();
      inline            SelectedRoutingError   ( const RoutingError*, size_t access=0 );
      inline const RoutingError* getRoutingError        () const;
      inline size_t     getAccesses   () const;
      inline void       incAccesses   () const;
      inline void       setInserted   () const;
      inline void       resetAccesses () const;
    private:
              const RoutingError* _routingError;
      mutable size_t     _accesses;
  };


  inline            SelectedRoutingError::SelectedRoutingError   () : _routingError(NULL), _accesses(0) { }
  inline            SelectedRoutingError::SelectedRoutingError   ( const RoutingError* routingError, size_t accesses ) : _routingError(routingError), _accesses(accesses) { }
  inline const RoutingError* SelectedRoutingError::getRoutingError        () const { return _routingError; }
  inline void       SelectedRoutingError::setInserted   () const { _accesses = 64; }
  inline size_t     SelectedRoutingError::getAccesses   () const { return _accesses; }
  inline void       SelectedRoutingError::incAccesses   () const { ++_accesses; }
  inline void       SelectedRoutingError::resetAccesses () const { _accesses = 0; }


  struct SelectedRoutingErrorCompare {
      inline bool operator() ( const SelectedRoutingError& lhs, const SelectedRoutingError& rhs ) const;
  };


  inline bool SelectedRoutingErrorCompare::operator() ( const SelectedRoutingError& lhs, const SelectedRoutingError& rhs ) const
  {
    return lhs.getRoutingError() < rhs.getRoutingError();
  }


// -------------------------------------------------------------------
// Class  :  "SelectedRoutingErrorSet".


  class SelectedRoutingErrorSet : public set<SelectedRoutingError,SelectedRoutingErrorCompare>{
    public:
      void  insert         ( const RoutingError* );
      void  forceInserteds ();
      void  resetAccesses  ();
  };


  inline void  SelectedRoutingErrorSet::insert ( const RoutingError* routingError )
  {
    iterator  iselected = find(SelectedRoutingError(routingError));
    if ( iselected != end() ) {
      iselected->incAccesses ();
    }
    else
      set<SelectedRoutingError,SelectedRoutingErrorCompare>::insert ( SelectedRoutingError(routingError,64) );
  }


  inline void  SelectedRoutingErrorSet::forceInserteds ()
  {
    for ( iterator iselected=begin() ; iselected != end() ; ++iselected )
      iselected->setInserted ();
  }


  inline void  SelectedRoutingErrorSet::resetAccesses ()
  {
    for ( iterator iselected=begin() ; iselected != end() ; ++iselected )
      iselected->resetAccesses ();
  }


// -------------------------------------------------------------------
// Class  :  "RoutingErrorListWidget".


  class RoutingErrorListWidget : public QWidget {
      Q_OBJECT;

    public:
                                     RoutingErrorListWidget        ( QWidget* parent=NULL );
      inline  Cell*                  getCell              ();
      template<typename InformationType>                  
              void                   setCellWidget        ( CellWidget* );
      template<typename InformationType>                  
              void                   setCell              ( Cell* );
              void                   goTo                 ( int );
              void                   updateSelecteds      ();
    signals:
    /**/      void                   occurrenceSelected   ( const Occurrence );
    /**/      void                   occurrenceUnselected ( const Occurrence );
    /**/      void                   routingErrorFitted   ( const RoutingError* );
    public slots:                                         
              void                   forceRowHeight       ();
    private slots:            
    /**/      void                   fitToError           ();  
              void                   textFilterChanged    ();
              void                   updateSelecteds      ( const QItemSelection& , const QItemSelection& );

    private:
              CellWidget*             _cellWidget;
              Cell*                   _cell;
              RoutingErrorListModel*  _baseModel;
              QSortFilterProxyModel*  _sortModel;
              QTableView*             _view;
              QLineEdit*              _filterPatternLineEdit;
              int                     _rowHeight;
              SelectedRoutingErrorSet _selecteds;
              bool                    _forceReselect;
  };


  template<typename InformationType>
  void  RoutingErrorListWidget::setCellWidget ( CellWidget* cw )
  {
    if ( _cellWidget ) {
      disconnect ( this, 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    if ( _cellWidget ) {
      setCell<InformationType> ( _cellWidget->getCell() );
      /*TODO*/
      //connect ( this, SIGNAL(routingErrorFitted(const RoutingError*)), _cellWidget, SLOT(fitToRoutingError (const RoutingError*)) );
    } else
      setCell<InformationType> ( NULL );
  }


  template<typename InformationType>
  void  RoutingErrorListWidget::setCell ( Cell* cell )
  {
    _cell = cell;
    _view->selectionModel()->clear ();
    _baseModel->setCell<InformationType> ( cell );
     
    string windowTitle = "RoutingErrorList" + getString(cell);
    setWindowTitle ( tr(windowTitle.c_str()) );

    int rows = _sortModel->rowCount ();
    for ( rows-- ; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
    _view->selectRow ( 0 );
    _view->resizeColumnToContents ( 0 );
  }


  inline  Cell* RoutingErrorListWidget::getCell () { return _cell; }


} // End of Hurricane namespace.


#endif // __HURRICANE_ROUTINGERRORLIST_WIDGET_H__
