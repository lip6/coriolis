#include "hurricane/inspector/RecordModel.h"

RecordModel::RecordModel(Record* r, QObject* parent):
    QAbstractTableModel(parent),
    record(r)
{}

QVariant RecordModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    unsigned row = index.row();
    Slot* slot = record->getSlot(row);
    if (slot) {
        switch (index.column()) {
            case 0:
                return QVariant(getString(slot->getName()).c_str());
            case 1:
                return QVariant(slot->getDataString().c_str());
        }
    } 
    return QVariant();
}

int RecordModel::rowCount(const QModelIndex &parent) const {
    return record->_getSlotList().size();
}

int RecordModel::columnCount(const QModelIndex &parent) const {
    return 2;
}
