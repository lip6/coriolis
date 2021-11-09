// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/SelectionWidget.h"     |
// +-----------------------------------------------------------------+


#pragma  once
#include <QWidget>
#include <QTableView>
#include <QSortFilterProxyModel>
#include "hurricane/Commons.h"
#include "hurricane/Occurrence.h"
#include "hurricane/viewer/SelectionModel.h"
#include "hurricane/viewer/CellWidget.h"

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
              void                   selectionToggled       ( Selector* );
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
              void                   setSelection           ( Selector* );
              void                   toggleSelection        ();
              void                   toggleSelection        ( Selector* );
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


} // Hurricane namespace.
