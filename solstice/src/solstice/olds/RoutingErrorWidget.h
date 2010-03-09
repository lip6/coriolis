
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
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./solstice/RoutingErrorWidget.h"          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __SOLSTICE_ROUTINGERROR_WIDGET__
#define  __SOLSTICE_ROUTINGERROR_WIDGET__


#include  <set>

#include  <QWidget>
#include  <QTableView>
#include  <QSortFilterProxyModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Bug.h"
#include  "hurricane/viewer/CellWidget.h"

#include  "solstice/RoutingErrorModel.h"

class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;


namespace Solstice {


  using std::set;

  // -------------------------------------------------------------------
  // Class  :  "SelectedRoutingError".
  
  
  class SelectedRoutingError {
  public:
    inline                     SelectedRoutingError   ();
    inline                     SelectedRoutingError   ( const RoutingError*, 
							size_t access=0 );
    inline const RoutingError* getRoutingError        () const;
    inline size_t              getAccesses            () const;
    inline void                incAccesses            () const;
    inline void                setInserted            () const;
    inline void                resetAccesses          () const;
  private:
    const   RoutingError* _routingError;
    mutable size_t        _accesses;
  };
  

  inline            SelectedRoutingError::SelectedRoutingError   () : _routingError(NULL), _accesses(0) { }
  inline            SelectedRoutingError::SelectedRoutingError   ( const RoutingError* RoutingError, size_t accesses ) : _routingError(RoutingError), _accesses(accesses) { }
  inline const RoutingError* SelectedRoutingError::getRoutingError        () const { return _routingError; }
  inline void       SelectedRoutingError::setInserted   () const { _accesses = 64; }
  inline size_t     SelectedRoutingError::getAccesses   () const { return _accesses; }
  inline void       SelectedRoutingError::incAccesses   () const { ++_accesses; }
  inline void       SelectedRoutingError::resetAccesses () const { _accesses = 0; }


  struct SelectedRoutingErrorCompare {
      inline bool operator() ( const SelectedRoutingError& lhs, const SelectedRoutingError& rhs );
  };


  inline bool SelectedRoutingErrorCompare::operator() ( const SelectedRoutingError& lhs, const SelectedRoutingError& rhs )
  {
    return lhs.getRoutingError()->getName() < rhs.getRoutingError()->getName();
  }


// -------------------------------------------------------------------
// Class  :  "SelectedRoutingErrorSet".


  class SelectedRoutingErrorSet : public set<SelectedRoutingError,SelectedRoutingErrorCompare>{
    public:
      void  insert         ( const RoutingError* );
      void  forceInserteds ();
      void  resetAccesses  ();
  };


  inline void  SelectedRoutingErrorSet::insert ( const RoutingError* RoutingError )
  {
    iterator  iselected = find(SelectedRoutingError(RoutingError));
    if ( iselected != end() )
      iselected->incAccesses ();
    else
      set<SelectedRoutingError,SelectedRoutingErrorCompare>::insert ( SelectedRoutingError(RoutingError,64) );
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
// Class  :  "RoutingErrorWidget".


  class RoutingErrorWidget : public QWidget {
      Q_OBJECT;

    public:
                                     RoutingErrorWidget            ( QWidget* parent=NULL );
      inline  Cell*                  getCell                       ();
    /**/      void                   setCellWidget                 ( CellWidget* );
    /**/      void                   setCell                       ( Cell* );
    /**/      void                   goTo                          ( int );
    /**/      void                   updateSelecteds               ();
    signals:
    /**/      void                   routingErrorSelected          ( const RoutingError* );
    /**/      void                   routingErrorUnselected        ( const RoutingError* );
    /**/      void                   routingErrorFitted            ( const RoutingError* );
    public slots:                                         
    /**/      void                   forceRowHeight                ();
    private slots:                                                  
    /**/      void                   textFilterChanged             ();
    /**/      void                   updateSelecteds               ( const QItemSelection& , const QItemSelection& );
    /**/      void                   fitToRoutingError             ();

    private:
    /**/      CellWidget*               _cellWidget;
    /**/      Cell*                     _cell;
    /**/      RoutingErrorModel*        _baseModel;
    /**/      QSortFilterProxyModel*    _sortModel;
    /**/      QTableView*               _view;
    /**/      QLineEdit*                _filterPatternLineEdit;
    /**/      int                       _rowHeight;
    /**/      SelectedRoutingErrorSet   _selecteds;
    /**/      bool                      _forceReselect;
  };

  inline  Cell* RoutingErrorWidget::getCell () { return _cell; }


} // End of Hurricane namespace.


#endif // __SOLSTICE_ROUTINGERROR_WIDGET_H__
