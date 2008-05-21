#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QKeyEvent>
#include <QGroupBox>
#include <QVBoxLayout>

#include "hurricane/inspector/RecordModel.h"
#include "hurricane/inspector/HInspectorWidget.h"

HInspectorWidget::HInspectorWidget(QWidget* parent):
    QWidget(parent),
    filterProxyModels(),
    filterProxyModelsHistory(),
    recordsHistoryComboBox(NULL),
    slotsView(NULL)
{
    //recordsHistoryComboBox = new QComboBox(this);
    
    slotsView = new QTableView(this);
    slotsView->setAlternatingRowColors(true);
    slotsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    slotsView->setSortingEnabled(true);

    QGridLayout* inspectorLayout = new QGridLayout();
    //inspectorLayout->addWidget(recordsHistoryComboBox, 0, 0, 1, 2);
    inspectorLayout->addWidget(slotsView, 1, 0, 1, 2);

    filterPatternLineEdit = new QLineEdit(this);
    QLabel* filterPatternLabel = new QLabel(tr("&Filter pattern:"), this);
    filterPatternLabel->setBuddy(filterPatternLineEdit);

    inspectorLayout->addWidget(filterPatternLabel, 2, 0);
    inspectorLayout->addWidget(filterPatternLineEdit, 2, 1);

    QGroupBox* inspectorGroupBox = new QGroupBox(tr("Hurricane inspector"), this);
    inspectorGroupBox->setLayout(inspectorLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(inspectorGroupBox);
    setLayout(mainLayout);

    //connect(recordsHistoryComboBox, SIGNAL(currentIndexChanged(int)),
    //        this, SLOT(recordChanged(int)));

    connect(filterPatternLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(textFilterChanged()));

    setWindowTitle(tr("Inspector"));
    resize(1000, 500);
}

void HInspectorWidget::setRecord(Record* record) {
    filterProxyModelsHistory.clear();
    //recordsHistoryComboBox->clear();
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
        sortModel->setDynamicSortFilter(true);
        sortModel->setSourceModel(recordModel);
        sortModel->setFilterKeyColumn(1);
        filterProxyModels[record] = sortModel;
    }
    slotsView->setModel(sortModel);
    slotsView->resizeColumnsToContents();
    filterProxyModelsHistory.push_back(sortModel);
    //recordsHistoryComboBox->addItem(QString(record->getName().c_str()));
    //recordsHistoryComboBox->setCurrentIndex(recordsHistoryComboBox->count()-1);
}

void HInspectorWidget::keyPressEvent(QKeyEvent *event) {
    if (event->matches(QKeySequence::MoveToNextChar)) {
        QModelIndex index = slotsView->currentIndex();
        if (index.isValid()) {
            QSortFilterProxyModel* sortModel =
                static_cast<QSortFilterProxyModel*>(slotsView->model());
            QModelIndex modelIndex = sortModel->mapToSource(index);
            RecordModel* recordModel =
                static_cast<RecordModel*>(sortModel->sourceModel());
            Record* record = recordModel->getRecord();
            unsigned row = modelIndex.row();
            Slot* slot = record->getSlot(row);
            if (slot) {
                Record* newRecord = slot->getDataRecord();
                if (newRecord) {
                    internalSetRecord(newRecord);
                }
            }
        }
    } else if (event->matches(QKeySequence::MoveToPreviousChar)) {
        if (!filterProxyModelsHistory.empty()) {
            filterProxyModelsHistory.pop_back();
            if (!filterProxyModelsHistory.empty()) {
                QSortFilterProxyModel* proxyModel = filterProxyModelsHistory.back();
                slotsView->setModel(proxyModel);
            }
            //unsigned count = recordsHistoryComboBox->count();
            //if (count > 0) {
            //    recordsHistoryComboBox->removeItem(count-1);
            //}
        }
    } else {
        event->ignore();
    }
}

void HInspectorWidget::recordChanged(int index) {
    if (index >= 0 && index < filterProxyModelsHistory.size()) {
        QSortFilterProxyModel* proxyModel = filterProxyModelsHistory[index];
        slotsView->setModel(proxyModel);
        //if (recordsHistoryComboBox->count() > index) {
        //    for (int i = recordsHistoryComboBox->count() - 1; i <= index; i--) {
        //        recordsHistoryComboBox->removeItem(i);
        //    }
        //}
    }
}

void HInspectorWidget::textFilterChanged() {
    QRegExp regExp(filterPatternLineEdit->text());
    QSortFilterProxyModel* sortModel =
        static_cast<QSortFilterProxyModel*>(slotsView->model());
    sortModel->setFilterRegExp(regExp);
}
