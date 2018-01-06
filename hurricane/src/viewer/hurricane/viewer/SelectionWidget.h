
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
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
// |  C++ Header  :       "./SelectionWidget.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_SELECTION_WIDGET__
#define  __HURRICANE_SELECTION_WIDGET__


#include  <QWidget>
#include  <QTableView>
#include  <QSortFilterProxyModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/viewer/SelectionModel.h"
#include  "hurricane/viewer/CellWidget.h"


class QCloseEvent;
class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;
class QCheckBox;


namespace Hurricane {


  class Selector;


  class SelectionWidget : public QWidget {
      Q_OBJECT;

    public:
                                     SelectionWidget        ( QWidget* parent=NULL );
              void                   inspect                ( const QModelIndex& index  );
              bool                   cumulativeSelection    () const;
    signals:                                                
              void                   selectionModeChanged   ();
              void                   selectionToggled       ( Occurrence );
              void                   inspect                ( Record* );
              void                   inspect                ( Occurrence& );
    public slots:                                           
              void                   setCellWidget          ( CellWidget* );
              void                   clear                  ();
              void                   changeSelectionMode    ();
              void                   setShowSelection       ( bool );
              void                   setCumulativeSelection ( bool );
              void                   selectCurrent          ( const QModelIndex& current, const QModelIndex& );
              void                   setSelection           ( const SelectorSet& selection );
              void                   setSelection           ( Occurrence );
              void                   toggleSelection        ();
              void                   toggleSelection        ( Occurrence );
              void                   toggleSelection        ( const QModelIndex& );
              void                   inspect                ();
    private slots:                                          
              void                   textFilterChanged      ();
            //void                   dataChanged            ( const QModelIndex&, const QModelIndex& );
    protected:                                              
              void                   blockAllSignals        ( bool );

    private:
              CellWidget*            _cellWidget;
              SelectionModel*        _baseModel;
              QSortFilterProxyModel* _sortModel;
              QTableView*            _view;
              QLineEdit*             _filterPatternLineEdit;
              QCheckBox*             _cumulative;
              QCheckBox*             _showSelection;
              int                    _rowHeight;
              UpdateState            _updateState;
  };


} // End of Hurricane namespace.


#endif // __HURRICANE_SELECTION_WIDGET__
