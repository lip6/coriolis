
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
// |  C++ Module  :       "./SelectionModel.cpp"                     |
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
#include  "hurricane/viewer/SelectionModel.h"
#include  "hurricane/viewer/SelectionWidget.h"


namespace Hurricane {


  SelectionModel::SelectionModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _selection()
  { }


  SelectionModel::~SelectionModel ()
  { }


  bool  SelectionModel::isCumulative () const
  {
    SelectionWidget* widget = qobject_cast<SelectionWidget*>(QObject::parent());
    if ( widget )
      return widget->cumulativeSelection();

    return true;
  }


  void  SelectionModel::clear ()
  {
    _selection.clear ();
    emit layoutChanged ();
  }


  void  SelectionModel::setSelection ( const SelectorSet& selection )
  {
    if ( !isCumulative() ) _selection.clear ();

    SelectorSet::const_iterator       iselector = selection.begin();
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


  Occurrence  SelectionModel::toggleSelection ( const QModelIndex& index )
  {
    if ( index.isValid() && ( index.row() < (int)_selection.size() ) ) {
      _selection[index.row()].toggle();
      emit layoutChanged ();

      return _selection[index.row()]._occurrence;
    }

    return Occurrence ();
  }


  void  SelectionModel::toggleSelection ( Occurrence occurrence )
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


  QVariant  SelectionModel::data ( const QModelIndex& index, int role ) const
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
        case 0: return getString(_selection[row]._occurrence.getPath().getName()).c_str();
        case 1: return getString(_selection[row]._occurrence.getEntity()).c_str();
      }
    }
    return QVariant();
  }


  QVariant  SelectionModel::headerData ( int             section
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


  int  SelectionModel::rowCount ( const QModelIndex& parent ) const
  {
    return _selection.size();
  }


  int  SelectionModel::columnCount ( const QModelIndex& parent ) const
  {
    return 2;
  }


  const Occurrence  SelectionModel::getOccurrence ( int row )
  {
    if ( row >= (int)_selection.size() ) return Occurrence();

    return _selection[row]._occurrence;
  }


} // End of Hurricane namespace.
