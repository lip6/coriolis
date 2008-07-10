
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RecordModel.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>
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
    if ( _depth ) delete _record;
    delete _slot;
  }


  bool  RecordModel::setSlot ( Slot* slot, size_t depth )
  {
    Record* record = slot->getDataRecord ();
    if ( !record ) {
      delete slot;
      return false;
    }

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
