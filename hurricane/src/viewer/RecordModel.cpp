// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RecordModel.cpp"                        |
// +-----------------------------------------------------------------+


#include  <QFont>
#include  <QApplication>

#include  "hurricane/Name.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/RecordModel.h"


namespace Hurricane {


  RecordModel::RecordModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _slot              (NULL)
    , _record            (NULL)
    , _depth             (0)
    , _cache             ()
  { }


  RecordModel::~RecordModel ()
  {
    if ( _depth ) delete _record;
    delete _slot;
  }


  bool  RecordModel::setSlot ( Slot* slot, Record* record, size_t depth )
  {
  //cerr << "    Slot change" << endl;

    emit layoutAboutToBeChanged ();

    vector< pair<QVariant,QVariant> >().swap ( _cache );

    if ( _slot  ) {
      delete _slot;
      if ( _depth ) delete _record;
    }

    _slot   = NULL;
    _record = NULL;
    _depth  = depth;

    if ( slot == NULL ) {
    //cerr << "    NULL Slot" << endl;

      emit layoutChanged ();
      return false;
    }

  // Now supplied by argument.
    if ( record == NULL ) {
      record = slot->getDataRecord ();
    //cerr << "    New record build" << endl;
      if ( record == NULL ) {
      //cerr << "    Slot " << slot->getDataString() << " has NULL Record" << endl;

        delete slot;
        emit layoutChanged ();
        return false;
      }
    }

  //cerr << "    New Slot [" << depth << "] " << slot->getDataString() << endl;

    _slot   = slot;
    _record = record;
    _depth  = depth;

    Record::SlotVector&          slotVector = _record->_getSlotVector();
    Record::SlotVector::iterator islot      = slotVector.begin();
    for ( ; islot != slotVector.end() ; islot++ ) {
      _cache.push_back ( make_pair(QVariant(getString((*islot)->getName()).c_str())
                                  ,QVariant((*islot)->getDataString().c_str())) );
    }

    emit layoutChanged ();

    return true;
  }


  QVariant  RecordModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont nameFont  = Graphics::getFixedFont ( QFont::Bold );
    static QFont valueFont = Graphics::getFixedFont ( QFont::Normal, true );

    if ( not index.isValid() ) return QVariant ();

    if ( role == Qt::SizeHintRole ) {
      switch (index.column()) {
        case 0: return 200;
        case 1: return -1;
      }
    }

    if ( role == Qt::FontRole ) {
    //if ( index.row() == 0 ) return QVariant();
      switch (index.column()) {
        case 0: return nameFont;
        case 1: return valueFont;
      }
    }

    if ( role == Qt::DisplayRole ) {
      int row = index.row ();
    //Slot* slot = _record->getSlot ( row );
      if ( row < (int)_cache.size() ) {
        switch ( index.column() ) {
          case 0: return _cache[row].first;
          case 1: return _cache[row].second;
        //case 0: return QVariant(slot->getName      ().c_str());
        //case 1: return QVariant(slot->getDataString().c_str());
        }
      } 
    }
    return QVariant();
  }


  QVariant  RecordModel::headerData ( int             section
                                    , Qt::Orientation orientation
                                    , int             role ) const
  {
    if ( ( orientation == Qt::Vertical ) or ( section > 1 ) )
      return QVariant();

    static QFont headerFont = Graphics::getFixedFont ( QFont::Bold, false, false, +2 );

    if ( role == Qt::FontRole    ) return headerFont;
    if ( role != Qt::DisplayRole ) return QVariant();

    if ( section == 0 )
      return QVariant ( tr("Object Attribute") );

    return QVariant ( tr("Value") );
  }


  int  RecordModel::rowCount ( const QModelIndex& parent ) const
  {
  //return (_record != NULL) ? _record->_getSlotList().size() : 0;
    return _cache.size();
  }


  int  RecordModel::columnCount ( const QModelIndex& parent ) const
  {
    return 2;
  }


} // End of Hurricane namespace.
