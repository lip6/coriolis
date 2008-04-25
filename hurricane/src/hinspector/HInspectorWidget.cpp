#include <QKeyEvent>
#include <QVBoxLayout>

#include "RecordModel.h"
#include "HInspectorWidget.h"

HInspectorWidget::HInspectorWidget(QWidget* parent):
    QWidget(parent),
    recordModels(),
    recordModelsHistory(),
    slotsTableView(NULL)
{
    slotsTableView = new QTableView;
    slotsTableView->setAlternatingRowColors(true);
    slotsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(slotsTableView);
    setLayout(mainLayout);

    //setFocusPolicy(Qt::StrongFocus);

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
    recordModelsHistory.clear();
    internalSetRecord(record);
}

void HInspectorWidget::internalSetRecord(Record* record) {
    RecordModel* recordModel = NULL;

    RecordModels::iterator rmit = recordModels.find(record);
    if (rmit != recordModels.end()) {
        recordModel = rmit->second;
    } else {
        recordModel = new RecordModel(record);
        recordModels[record] = recordModel;
    }
    slotsTableView->setModel(recordModel);
    recordModelsHistory.push_back(recordModel);
}

void HInspectorWidget::keyPressEvent(QKeyEvent *event) {
    if (event->matches(QKeySequence::MoveToNextChar)) {
        QModelIndex index = slotsTableView->currentIndex();
        if (index.isValid()) {
            unsigned row = index.row();
            RecordModel* recordModel = static_cast<RecordModel*>(slotsTableView->model());
            Record* record = recordModel->getRecord();
            Slot* slot = record->getSlot(row);
            if (slot) {
                Record* newRecord = slot->getDataRecord();
                if (newRecord) {
                    internalSetRecord(newRecord);
                }
            }
        }
    } else if (event->matches(QKeySequence::MoveToPreviousChar)) {
        if (!recordModelsHistory.empty()) {
            recordModelsHistory.pop_back();
            if (!recordModelsHistory.empty()) {
                RecordModel* recordModel = recordModelsHistory.back();
                slotsTableView->setModel(recordModel);
            }
        }
    } else {
        event->ignore();
    }
}
