
#include  <QFont>
#include  <QApplication>

#include  "hurricane/Name.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/RecordModel.h"


namespace Hurricane {


  RecordModel::RecordModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _slot(NULL)
    , _record(NULL)
    , _depth(0)
  { }


  RecordModel::~RecordModel ()
  {
    delete _record;
    delete _slot;
  }


  bool  RecordModel::setSlot ( Slot* slot, size_t depth )
  {
    Record* record = slot->getDataRecord ();
    if ( !record ) {
      delete slot;
      return false;
    }

    emit layoutAboutToBeChanged ();

    if ( _depth ) delete _record;
    if ( _slot  ) delete _slot;

    _slot   = slot;
    _record = record;
    _depth  = depth;

    emit layoutChanged ();

    return true;
  }


  QVariant  RecordModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont nameFont  = Graphics::getFixedFont ( QFont::Bold );
    static QFont valueFont = Graphics::getFixedFont ( QFont::Normal, true );

    if ( !index.isValid() ) return QVariant ();

    if ( role == Qt::SizeHintRole ) {
      switch (index.column()) {
        case 0: return 200;
        case 1: return -1;
      }
    }

    if ( role == Qt::FontRole ) {
      switch (index.column()) {
        case 0: return nameFont;
        case 1: return valueFont;
      }
    }

    if ( role == Qt::DisplayRole ) {
      int   row  = index.row ();
      Slot* slot = _record->getSlot ( row );
      if ( slot ) {
        switch ( index.column() ) {
          case 0: return QVariant(slot->getName      ().c_str());
          case 1: return QVariant(slot->getDataString().c_str());
        }
      } 
    }
    return QVariant();
  }


  QVariant  RecordModel::headerData ( int             section
                                    , Qt::Orientation orientation
                                    , int             role ) const
  {
    if ( ( orientation == Qt::Vertical ) || ( section > 1 ) || (role != Qt::DisplayRole) )
      return QVariant();

    if ( section == 0 )
      return QVariant ( tr("Object Attribute") );

    return QVariant ( tr("Value") );
  }


  int  RecordModel::rowCount ( const QModelIndex& parent ) const
  {

    return (_record) ? _record->_getSlotList().size() : 0;
  }


  int  RecordModel::columnCount ( const QModelIndex& parent ) const
  {
    return 2;
  }


} // End of Hurricane namespace.
