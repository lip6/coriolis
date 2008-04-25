#ifndef RECORDMODEL_H
#define RECORDMODEL_H

#include <QAbstractTableModel>

#include "Commons.h"
using namespace Hurricane;

class RecordModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        RecordModel(Record* record, QObject* parent=0);
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        Record* getRecord() { return record; }
    private:
        Record* record;
};

#endif // RECORDMODEL_H 
