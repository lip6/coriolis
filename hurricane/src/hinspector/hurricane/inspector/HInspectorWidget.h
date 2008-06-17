
#ifndef  __HINSPECTOR_WIDGET_H__
#define  __HINSPECTOR_WIDGET_H__


#include  "hurricane/Commons.h"

using namespace Hurricane;

#include  <QWidget>

class QSortFilterProxyModel;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;

class RecordModel;


class HInspectorWidget : public QWidget {
    Q_OBJECT;

  public:
    typedef vector<Slot*>             SlotsVector;

  public:
                                      HInspectorWidget  ( Record* rootRecord, QWidget* parent=NULL );
                                     ~HInspectorWidget  ();
            void                      setRootRecord     ( Record*  record );
  private slots:
            void                      textFilterChanged ();
  protected:
            void                      keyPressEvent     ( QKeyEvent * event );
  private:
            void                      clearHistory      ();
            void                      pushSlot          ( Slot* slot );
            void                      popSlot           ();
            bool                      setSlot           ( Slot* slot );

  private:
            SlotsVector               _slotsHistory;
            RecordModel*              _recordModel;
            QSortFilterProxyModel*    _sortModel;
            QComboBox*                _recordsHistoryComboBox;
            QTableView*               _slotsView;
            QLineEdit*                _filterPatternLineEdit;
            int                       _rowHeight;
            Record*                   _rootRecord;
};


#endif // __HINSPECTOR_WIDGET_H__
