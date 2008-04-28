#include <QLineEdit>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QKeyEvent>
#include <QGroupBox>
#include <QVBoxLayout>

#include "RecordModel.h"
#include "HInspectorWidget.h"

HInspectorWidget::HInspectorWidget(QWidget* parent):
    QWidget(parent),
    filterProxyModels(),
    filterProxyModelsHistory(),
    slotsView(NULL)
{
    slotsView = new QTableView(this);
    slotsView->setAlternatingRowColors(true);
    slotsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    slotsView->setSortingEnabled(true);

    QVBoxLayout* inspectorLayout = new QVBoxLayout();
    inspectorLayout->addWidget(slotsView);

    filterPatternLineEdit = new QLineEdit(this);
    inspectorLayout->addWidget(filterPatternLineEdit);




    QGroupBox* inspectorGroupBox = new QGroupBox(tr("Hurricane inspector"), this);
    inspectorGroupBox->setLayout(inspectorLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(inspectorGroupBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Inspector"));
    resize(1000, 500);
}

void HInspectorWidget::setRecord(Record* record) {
    filterProxyModelsHistory.clear();
    internalSetRecord(record);
}

void HInspectorWidget::internalSetRecord(Record* record) {
    QSortFilterProxyModel* sortModel = NULL;

    FilterProxyModels::iterator fpmit = filterProxyModels.find(record);
    if (fpmit != filterProxyModels.end()) {
        sortModel = fpmit->second;
    } else {
        RecordModel* recordModel = new RecordModel(record, this);
        sortModel = new QSortFilterProxyModel(this);
        sortModel->setSourceModel(recordModel);
        filterProxyModels[record] = sortModel;
    }
    slotsView->setModel(sortModel);
    slotsView->resizeColumnsToContents();
    filterProxyModelsHistory.push_back(sortModel);
}

void HInspectorWidget::keyPressEvent(QKeyEvent *event) {
    if (event->matches(QKeySequence::MoveToNextChar)) {
        QModelIndex index = slotsView->currentIndex();
        if (index.isValid()) {
            unsigned row = index.row();
            QSortFilterProxyModel* sortModel =
                static_cast<QSortFilterProxyModel*>(slotsView->model());
            RecordModel* recordModel =
                static_cast<RecordModel*>(sortModel->sourceModel());
            Record* record = recordModel->getRecord();
            Slot* slot = record->getSlot(row);
            if (slot) {
                Record* newRecord = slot->getDataRecord();
                if (newRecord) {
                    internalSetRecord(newRecord);
                }
            }
        }
    //} else if (event->matches(QKeySequence::MoveToPreviousChar)) {
    //    if (!recordModelsHistory.empty()) {
    //        recordModelsHistory.pop_back();
    //        if (!recordModelsHistory.empty()) {
    //            QSortFilterProxyModel* proxyModel = recordModelsHistory.back();
    //            slotsView->setModel(proxyModel);
    //        }
    //    }
    } else {
        event->ignore();
    }
}
