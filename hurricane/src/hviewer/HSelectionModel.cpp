
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


#include  <QFont>

#include  "hurricane/Path.h"
#include  "hurricane/Entity.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/Selector.h"
#include  "hurricane/viewer/HSelectionModel.h"


namespace Hurricane {


  HSelectionModel::HSelectionModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _selection()
  { }


  HSelectionModel::~HSelectionModel ()
  { }


  void  HSelectionModel::setSelection ( const set<Selector*>& selection )
  {
    _selection.clear ();

    set<Selector*>::const_iterator iselector = selection.begin();
    for ( ; iselector != selection.end() ; iselector++ )
      _selection.push_back ( (*iselector)->getOccurrence() );

    emit layoutChanged ();
  }


  void  HSelectionModel::addToSelection ( Selector* selector )
  {
    _selection.push_back ( selector->getOccurrence() );

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

    if ( role == Qt::FontRole ) {
      switch (index.column()) {
        case 0:  return occurrenceFont;
        default: return entityFont;
      }
    }

    if ( role == Qt::DisplayRole ) {
      int row = index.row ();
      if ( row < (int)_selection.size() ) {
        switch ( index.column() ) {
          case 0: return getString(_selection[row].getPath()).c_str();
          case 1: return getString(_selection[row].getEntity()).c_str();
        }
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

    return _selection[row];
  }


} // End of Hurricane namespace.
