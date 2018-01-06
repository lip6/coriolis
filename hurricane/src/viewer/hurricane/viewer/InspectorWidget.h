
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./hurricane/viewer/InspectorWidget.h"         |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_INSPECTOR_WIDGET_H
#define  HURRICANE_INSPECTOR_WIDGET_H


#include <QWidget>
#include "hurricane/Commons.h"
#include "hurricane/Occurrence.h"


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;


namespace Hurricane {


  class RecordModel;


  class InspectorWidget : public QWidget {

    public:
      typedef vector<Slot*> SlotsVector;

    private:
      class History {
        public:
                       History       ();
                      ~History       ();
          void         push          ( Slot*, Record* );
          void         pop           ();
          void         back          ();
          void         goTo          ( int depth );
          void         clear         ( bool inDelete=false );
          void         setComboBox   ( QComboBox* comboBox );
          void         setRootRecord ( Record* rootRecord );
          size_t       getDepth      () const;
          Slot*        getSlot       () const;
        private:
          size_t       _depth;
          SlotsVector  _slots;
          QComboBox*   _comboBox;
        private:
                       History       ( const History& );
          History&     operator=     ( const History& );
      };

      Q_OBJECT;

    public:
                                        InspectorWidget   ( QWidget* parent=NULL );
                                       ~InspectorWidget   ();
              void                      setRootRecord     ( Record* );
              void                      setRootOccurrence ( Occurrence& );
    private slots:
              void                      textFilterChanged ();
              void                      historyChanged    ( int depth );
              void                      forkInspector     ( const QModelIndex& );
    protected:
              bool                      eventFilter       ( QObject*, QEvent* );
    private:
              void                      _setRootRecord    ( Record* );
              void                      pushSlot          ( Slot*, Record* record=NULL );
              void                      popSlot           ();
              void                      back              ();
              bool                      setSlot           ( Record* record=NULL );

    private:
              RecordModel*              _baseModel;
              QSortFilterProxyModel*    _sortModel;
              QComboBox*                _historyComboBox;
              QTableView*               _view;
              QLineEdit*                _filterPatternLineEdit;
              int                       _rowHeight;
              History                   _history;
              Occurrence                _rootOccurrence;
  };


} // End of Hurricane namespace.


#endif // __HURRICANE_INSPECTOR_WIDGET__
