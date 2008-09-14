
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
// |  C++ Module  :       "./HSelectionModel.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <algorithm>

#include  <QFont>

#include  "hurricane/Path.h"
#include  "hurricane/Entity.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/Selector.h"
#include  "hurricane/viewer/HSelectionModel.h"
#include  "hurricane/viewer/HSelection.h"


namespace Hurricane {


  HSelectionModel::HSelectionModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _selection()
  { }


  HSelectionModel::~HSelectionModel ()
  { }


  bool  HSelectionModel::isCumulative () const
  {
    HSelection* widget = qobject_cast<HSelection*>(QObject::parent());
    if ( widget )
      return widget->isCumulative();

    return true;
  }


  void  HSelectionModel::clear ()
  {
    _selection.clear ();
    emit layoutChanged ();
  }


  void  HSelectionModel::setSelection ( const set<Selector*>& selection )
  {
    if ( !isCumulative() ) _selection.clear ();

    set<Selector*>::const_iterator    iselector = selection.begin();
    vector<OccurrenceItem>::iterator  iitem;
    for ( ; iselector != selection.end() ; iselector++ ) {
      if ( isCumulative() ) {
        iitem = find( _selection.begin(), _selection.end(), (*iselector)->getOccurrence() );
        if ( iitem != _selection.end() ) {
          (*iitem)._flags |= OccurrenceItem::Selected;
          continue;
        }
      }
      _selection.push_back ( OccurrenceItem((*iselector)->getOccurrence()) );
    }

    emit layoutChanged ();
  }


  Occurrence  HSelectionModel::toggleSelection ( const QModelIndex& index )
  {
    if ( index.isValid() && ( index.row() < (int)_selection.size() ) ) {
      _selection[index.row()].toggle();
      emit layoutChanged ();

      return _selection[index.row()]._occurrence;
    }

    return Occurrence ();
  }


  void  HSelectionModel::toggleSelection ( Occurrence occurrence )
  {
    bool   found = false;
    size_t i     = 0;
    for ( ; i<_selection.size() ; i++ ) {
      if ( !found && (_selection[i]._occurrence == occurrence) ) {
        found = true;
        if ( isCumulative() ) break;
      }
      if (  found && ( i < _selection.size()-1 ) )
        _selection[i] = _selection[i+1];
    }

    if ( !found )
      _selection.push_back ( OccurrenceItem(occurrence) );
    else {
      if ( isCumulative() ) _selection[i].toggle ();
      else                  _selection.pop_back ();
    }
    
    emit layoutChanged ();
  }


  QVariant  HSelectionModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont occurrenceFont = Graphics::getFixedFont ( QFont::Normal );
    static QFont entityFont     = Graphics::getFixedFont ( QFont::Bold, false );

    if ( !index.isValid() ) return QVariant ();

    if ( role == Qt::SizeHintRole ) {
      switch (index.column()) {
        case 0:  return 200;
        default: return -1;
      }
    }

    int row = index.row ();
    if ( row >= (int)_selection.size() ) return QVariant ();

    if ( role == Qt::FontRole ) {
      switch (index.column()) {
        case 0:  return occurrenceFont;
        case 1:
          if ( _selection[row]._flags & OccurrenceItem::Selected )
            return entityFont;
        default:
          return occurrenceFont;
      }
    }

    if ( role == Qt::DisplayRole ) {
      switch ( index.column() ) {
        case 0: return getString(_selection[row]._occurrence.getPath()).c_str();
        case 1: return getString(_selection[row]._occurrence.getEntity()).c_str();
      }
    }
    return QVariant();
  }


  QVariant  HSelectionModel::headerData ( int             section
                                        , Qt::Orientation orientation
                                        , int             role ) const
  {
    if ( ( orientation == Qt::Vertical ) || (role != Qt::DisplayRole) )
      return QVariant();

    switch ( section ) {
      case 0: return "Path";
      case 1: return "Entity";
    }

    return QVariant();
  }


  int  HSelectionModel::rowCount ( const QModelIndex& parent ) const
  {
    return _selection.size();
  }


  int  HSelectionModel::columnCount ( const QModelIndex& parent ) const
  {
    return 2;
  }


  const Occurrence  HSelectionModel::getOccurrence ( int row )
  {
    if ( row >= (int)_selection.size() ) return Occurrence();

    return _selection[row]._occurrence;
  }


} // End of Hurricane namespace.
