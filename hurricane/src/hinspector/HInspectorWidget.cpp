#include <QVBoxLayout>

#include "RecordModel.h"
#include "HInspectorWidget.h"

HInspectorWidget::HInspectorWidget(QWidget* parent):
    QWidget(parent),
    recordModels(),
    slotsTableView(NULL)
{

    slotsTableView = new QTableView;
    slotsTableView->setAlternatingRowColors(true);
    slotsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(slotsTableView);
    setLayout(mainLayout);

    setWindowTitle(tr("Inspector"));
    resize(1000, 500);
}

HInspectorWidget::~HInspectorWidget() {
    for (RecordModels::iterator rmit = recordModels.begin();
            rmit != recordModels.end();
            ++rmit) {
        delete rmit->second;
    }
}

void HInspectorWidget::setRecord(Record* record) {
    RecordModel* recordModel = NULL;

    RecordModels::iterator rmit = recordModels.find(record);
    if (rmit != recordModels.end()) {
        recordModel = rmit->second;
    } else {
        recordModel = new RecordModel(record);
        recordModels[record] = recordModel;
    }
    slotsTableView->setModel(recordModel);
}
