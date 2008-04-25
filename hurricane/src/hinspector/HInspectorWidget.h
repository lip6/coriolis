#ifndef HINSPECTORWIDGET_H
#define HINSPECTORWIDGET_H

#include "Commons.h"
using namespace Hurricane;

#include <QWidget>
#include <QTableView>

class RecordModel;

class HInspectorWidget : public QWidget {
    Q_OBJECT

    public:
        typedef map<Record*, RecordModel*> RecordModels;
        typedef list<RecordModel*> RecordModelsHistory;
        HInspectorWidget(QWidget* parent=0);
        ~HInspectorWidget();
        void setRecord(Record* record);

    protected:
        void keyPressEvent(QKeyEvent * event);

    private:
        void internalSetRecord(Record* record);
        RecordModels recordModels;
        RecordModelsHistory recordModelsHistory;
        QTableView* slotsTableView;
};


#endif // HINSPECTORWIDGET_H
