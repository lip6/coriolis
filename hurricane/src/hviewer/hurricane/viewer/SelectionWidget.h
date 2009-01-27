
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


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;
class QCheckBox;


namespace Hurricane {


  class Selector;
  class QCloseEvent;


  class SelectionWidget : public QWidget {
      Q_OBJECT;

    public:
                                     SelectionWidget      ( QWidget* parent=NULL );
              void                   inspect              ( const QModelIndex& index  );
              bool                   isCumulative         () const;
    signals:                                              
              void                   showSelectionToggled ( bool );
              void                   occurrenceToggled    ( Occurrence, bool );
              void                   cumulativeToggled    ( bool );
              void                   selectionCleared     ();
              void                   inspect              ( Record* );
              void                   inspect              ( Occurrence& );
    public slots:                                         
              void                   clear                ();
              void                   setShowSelection     ( bool );
              void                   selectCurrent        ( const QModelIndex& current, const QModelIndex& );
              void                   setSelection         ( const SelectorSet& selection, Cell* cell=NULL );
              void                   toggleSelection      ( Occurrence occurrence );
              void                   toggleSelection      ( const QModelIndex& index );
              void                   forceRowHeight       ();
    private slots:                                        
              void                   textFilterChanged    ();
    protected:                                            
      virtual void                   keyPressEvent        ( QKeyEvent * event );
      virtual void                   hideEvent            ( QHideEvent* event );

    private:
              SelectionModel*        _baseModel;
              QSortFilterProxyModel* _sortModel;
              QTableView*            _view;
              QLineEdit*             _filterPatternLineEdit;
              QCheckBox*             _cumulative;
              QCheckBox*             _showSelection;
              int                    _rowHeight;
  };


} // End of Hurricane namespace.


#endif // __HURRICANE_SELECTION_WIDGET__
