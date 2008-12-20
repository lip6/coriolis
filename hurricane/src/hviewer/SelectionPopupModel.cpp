
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
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
// |  C++ Module  :       "./SelectionPopupModel.cpp"                |
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
#include  "hurricane/viewer/SelectionPopupModel.h"


namespace Hurricane {


  SelectionPopupModel::SelectionPopupModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _occurrences(NULL)
  { }


  SelectionPopupModel::~SelectionPopupModel ()
  {
    clear ();
  }


  void  SelectionPopupModel::add ( Occurrence occurrence, bool showChange )
  {
    if ( !_occurrences ) _occurrences = new vector<Occurrence> ();

    _occurrences->push_back ( occurrence );
    if ( showChange ) emit layoutChanged ();
  }


  void  SelectionPopupModel::clear ()
  {
    if ( _occurrences ) {
      delete _occurrences;
      _occurrences = NULL;
    }
    emit layoutChanged ();
  }


  void  SelectionPopupModel::updateLayout ()
  {
    emit layoutChanged ();
  }


  QVariant  SelectionPopupModel::data ( const QModelIndex& index, int role ) const
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
            name = getString ( (*_occurrences)[row].getPath().getName() ) + "::"
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


  QVariant  SelectionPopupModel::headerData ( int             section
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


  int  SelectionPopupModel::rowCount ( const QModelIndex& parent ) const
  {
    if ( !_occurrences ) return 1;
    return _occurrences->size();
  }


  int  SelectionPopupModel::columnCount ( const QModelIndex& parent ) const
  {
    return 1;
  }


  Occurrence  SelectionPopupModel::getOccurrence ( int row )
  {
    if ( !_occurrences || (row >= (int)_occurrences->size()) )
      return Occurrence();

    return (*_occurrences)[row];
  }


} // End of Hurricane namespace.
