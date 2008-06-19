
#ifndef  __HINSPECTOR_WIDGET_H__
#define  __HINSPECTOR_WIDGET_H__


#include  <QWidget>

#include  "hurricane/Commons.h"


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;


namespace Hurricane {


  class RecordModel;


  class HInspectorWidget : public QWidget {

    public:
      typedef vector<Slot*>             SlotsVector;

    private:
      class History {
        public:
                       History       ();
                      ~History       ();
          void         push          ( Slot* slot );
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
                                        HInspectorWidget  ( QWidget* parent=NULL );
                                       ~HInspectorWidget  ();
              void                      setRootRecord     ( Record*  record );
    private slots:
              void                      textFilterChanged ();
              void                      historyChanged    ( int depth );
              void                      forkInspector     ( const QModelIndex& index );
    protected:
              void                      keyPressEvent     ( QKeyEvent * event );
    private:
              void                      pushSlot          ( Slot* slot );
              void                      popSlot           ();
              void                      back              ();
              bool                      setSlot           ();

    private:
              RecordModel*              _recordModel;
              QSortFilterProxyModel*    _sortModel;
              QComboBox*                _historyComboBox;
              QTableView*               _slotsView;
              QLineEdit*                _filterPatternLineEdit;
              int                       _rowHeight;
              History                   _history;
  };


} // End of Hurricane namespace.


#endif // __HINSPECTOR_WIDGET_H__
