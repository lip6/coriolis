#ifndef HINSPECTORWIDGET_H
#define HINSPECTORWIDGET_H

#include "hurricane/Commons.h"
using namespace Hurricane;

#include <QWidget>
class QSortFilterProxyModel;
class QTableView;
class QLineEdit;
class QComboBox;

class HInspectorWidget : public QWidget {
    Q_OBJECT

    public:
        typedef map<Record*, QSortFilterProxyModel*> FilterProxyModels;
        typedef vector<QSortFilterProxyModel*> FilterProxyModelsHistory;
        HInspectorWidget(QWidget* parent=0);
        void setRecord(Record* record);

    private slots:
        void recordChanged(size_t index);
        void textFilterChanged();

    protected:
        void keyPressEvent(QKeyEvent * event);

    private:
        void internalSetRecord(Record* record);
        FilterProxyModels filterProxyModels;
        FilterProxyModelsHistory filterProxyModelsHistory;
        QComboBox* recordsHistoryComboBox;
        QTableView* slotsView;
        QLineEdit* filterPatternLineEdit;
};


#endif // HINSPECTORWIDGET_H
