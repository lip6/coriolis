
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
// |  C++ Module  :       "./HSelectionPopupModel.cpp"               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QFont>

#include  "hurricane/Path.h"
#include  "hurricane/Entity.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/Selector.h"
#include  "hurricane/viewer/HSelectionPopupModel.h"


namespace Hurricane {


  HSelectionPopupModel::HSelectionPopupModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _occurrences(NULL)
  { }


  HSelectionPopupModel::~HSelectionPopupModel ()
  {
    clear ();
  }


  void  HSelectionPopupModel::add ( Occurrence occurrence, bool showChange )
  {
    if ( !_occurrences ) _occurrences = new vector<Occurrence> ();

    _occurrences->push_back ( occurrence );
    if ( showChange ) emit layoutChanged ();
  }


  void  HSelectionPopupModel::clear ()
  {
    if ( _occurrences ) {
      delete _occurrences;
      _occurrences = NULL;
    }
    emit layoutChanged ();
  }


  void  HSelectionPopupModel::updateLayout ()
  {
    emit layoutChanged ();
  }


  QVariant  HSelectionPopupModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont occurrenceFont = Graphics::getFixedFont ( QFont::Normal );
    static QFont entityFont     = Graphics::getFixedFont ( QFont::Bold, false );

    if ( !index.isValid() ) return QVariant ();

    if ( role == Qt::SizeHintRole ) {
      switch (index.column()) {
        default: return -1;
      }
    }

    if ( role == Qt::FontRole ) {
      switch (index.column()) {
        default: return entityFont;
      }
    }

    if ( role == Qt::DisplayRole ) {
      int    row = index.row ();
      string name;
      if ( _occurrences ) {
        if ( row < (int)_occurrences->size() ) {
          if ( index.column() == 0 ) {
            name = getString ( (*_occurrences)[row].getPath() )
                 + getString ( (*_occurrences)[row].getEntity() );
            return name.c_str();
          }
        }
      } else {
        if ( row < 1 ) {
          if ( index.column() == 0 ) {
            return QVariant ( tr("Nothing Selectable") );
          }
        }
      }
    }

    return QVariant();
  }


  QVariant  HSelectionPopupModel::headerData ( int             section
                                             , Qt::Orientation orientation
                                             , int             role ) const
  {
    if ( ( orientation == Qt::Vertical ) || (role != Qt::DisplayRole) )
      return QVariant();

    if ( section == 0 ) {
      return "Path+Entity";
    }

    return QVariant();
  }


  int  HSelectionPopupModel::rowCount ( const QModelIndex& parent ) const
  {
    if ( !_occurrences ) return 1;
    return _occurrences->size();
  }


  int  HSelectionPopupModel::columnCount ( const QModelIndex& parent ) const
  {
    return 1;
  }


  Occurrence  HSelectionPopupModel::getOccurrence ( int row )
  {
    if ( !_occurrences || (row >= (int)_occurrences->size()) )
      return Occurrence();

    return (*_occurrences)[row];
  }


} // End of Hurricane namespace.
