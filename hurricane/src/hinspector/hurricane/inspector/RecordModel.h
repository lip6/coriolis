
#ifndef  __RECORD_MODEL_H__
#define  __RECORD_MODEL_H__

#include  <QAbstractTableModel>

#include  "hurricane/Commons.h"


namespace Hurricane {


  class RecordModel : public QAbstractTableModel {
      Q_OBJECT;

    public:
                       RecordModel   ( Slot* slot, QObject* parent=NULL );
                      ~RecordModel   ();
             bool      setSlot       ( Slot* slot, size_t depth );
             int       rowCount      ( const QModelIndex& parent=QModelIndex() ) const;
             int       columnCount   ( const QModelIndex& parent=QModelIndex() ) const;
             QVariant  data          ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
             QVariant  headerData    ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      inline Record*   getRecord     ();
      inline Slot*     getSlot       ();

    private:
      Slot*     _slot;
      Record*   _record;
      size_t    _depth;
  };


  // Inline Functions.
  inline Record* RecordModel::getRecord     () { return _record; }
  inline Slot*   RecordModel::getSlot       () { return _slot; }


} // End of Hurricane namespace.


#endif // __RECORD_MODEL_H__ 
