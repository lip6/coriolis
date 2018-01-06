
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/HierarchyWidget.h"     |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_HIERARCHY_WIDGET_H
#define  HURRICANE_HIERARCHY_WIDGET_H

#include <set>
#include <QWidget>
#include <QTreeView>
#include <QItemDelegate>
#include "hurricane/Commons.h"
#include "hurricane/Bug.h"
#include "hurricane/viewer/HierarchyModel.h"
#include "hurricane/viewer/CellWidget.h"


class QModelIndex;
class QItemSelection;
class QLineEdit;
class QComboBox;
class QHeaderView;


namespace Hurricane {

  using std::set;
  class Net;
  class Cell;


// -------------------------------------------------------------------
// Class  :  "TreeKeyFilter".


  class  TreeKeyFilter : public QObject {
      Q_OBJECT;

    public:
            TreeKeyFilter ( QObject* parent );
      bool  eventFilter   ( QObject*, QEvent* );
  };


// -------------------------------------------------------------------
// Class  :  "HierarchyWidget".


  class HierarchyWidget : public QWidget {
      Q_OBJECT;

      friend class TreeKeyFilter;
    public:
                          HierarchyWidget   ( QWidget* parent=NULL );
              bool        isRSelected       ( const QModelIndex& index ) const;
      inline  CellWidget* getCellWidget     ();
              void        rexpand           ( Path path );
              void        rexpand           ( const QModelIndex& index );
      inline  Cell*       getCell           ();
      inline  void        setCellWidget     ( CellWidget* );
              void        setCell           ( Cell* );
              void        goTo              ( int );
    private slots:                          
              void        collapseHook      ( const QModelIndex& );
              void        textFilterChanged ();
              void        followSelected    ( const QItemSelection&, const QItemSelection& );
    private:
              CellWidget*     _cellWidget;
              Cell*           _cell;
              HierarchyModel* _baseModel;
              QTreeView*      _view;
              QLineEdit*      _filterPatternLineEdit;
              int             _rowHeight;
  };


  inline Cell*       HierarchyWidget::getCell       () { return _cell; }
  inline CellWidget* HierarchyWidget::getCellWidget () { return _cellWidget; }


  inline void  HierarchyWidget::setCellWidget ( CellWidget* cw )
  {
    if (_cellWidget) {
      disconnect( this, 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    if (_cellWidget) {
      if (_cellWidget->getTopCell()) setCell( _cellWidget->getTopCell() );
      else                           setCell( _cellWidget->getCell() );
    } else
      setCell( NULL );
  }


} // Hurricane namespace.

#endif // HURRICANE_HIERARCHY_WIDGET_H
