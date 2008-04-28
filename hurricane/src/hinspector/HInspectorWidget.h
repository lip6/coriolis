#ifndef HINSPECTORWIDGET_H
#define HINSPECTORWIDGET_H

#include "Commons.h"
using namespace Hurricane;

#include <QWidget>
class QSortFilterProxyModel;
class QTableView;
class QLineEdit;

class HInspectorWidget : public QWidget {
    Q_OBJECT

    public:
        typedef map<Record*, QSortFilterProxyModel*> FilterProxyModels;
        typedef list<QSortFilterProxyModel*> FilterProxyModelsHistory;
        HInspectorWidget(QWidget* parent=0);
        void setRecord(Record* record);

    protected:
        void keyPressEvent(QKeyEvent * event);

    private:
        void internalSetRecord(Record* record);
        FilterProxyModels filterProxyModels;
        FilterProxyModelsHistory filterProxyModelsHistory;
        QTableView* slotsView;
        QLineEdit* filterPatternLineEdit;
};


#endif // HINSPECTORWIDGET_H
