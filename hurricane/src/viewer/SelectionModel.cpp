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
// |  C++ Module  :       "./SelectionModel.cpp"                     |
// +-----------------------------------------------------------------+


#include  <algorithm>
#include  <QFont>
#include  <QFontMetrics>
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
    return false;
  }


  void  SelectionModel::clear ()
  {
    beginResetModel();
    _selection.clear();
    endResetModel();
  }


  void  SelectionModel::setSelection ( const SelectorSet& selection )
  {
    beginResetModel();
    if ( not isCumulative() ) _selection.clear ();

    SelectorSet::const_iterator       iselector = selection.begin();
    vector<OccurrenceItem>::iterator  iitem;
    for ( ; iselector != selection.end() ; iselector++ ) {
      if ( isCumulative() ) {
        iitem = find( _selection.begin(), _selection.end(), (*iselector)->getOccurrence() );
        if ( iitem != _selection.end() ) {
          (*iitem).setFlags ( OccurrenceItem::Selected );
          continue;
        }
      }
      _selection.push_back ( OccurrenceItem((*iselector)->getOccurrence()) );
    }

    endResetModel();
  }


  void  SelectionModel::setSelection ( Occurrence occurrence )
  {
    bool modificated = false;
    if ( not isCumulative() ) _selection.clear ();

    size_t i = 0;
    for ( ; i<_selection.size() ; i++ ) {
      if ( _selection[i]._occurrence == occurrence ) break;
    }

    if ( i >= _selection.size() ) {
      modificated = true;
      beginResetModel ();
      _selection.push_back ( OccurrenceItem(occurrence) );
    }
    else _selection[i].setFlags ( OccurrenceItem::Selected );
    
    if ( modificated ) endResetModel ();
  }


  Occurrence  SelectionModel::toggleSelection ( const QModelIndex& oindex )
  {
    if ( oindex.isValid() && ( oindex.row() < (int)_selection.size() ) ) {
      _selection[oindex.row()].toggle();
      emit dataChanged ( index(oindex.row(),0), index(oindex.row(),1) );

      return _selection[oindex.row()]._occurrence;
    }

    return Occurrence ();
  }


  Occurrence  SelectionModel::getOccurrence ( const QModelIndex& index )
  {
    if ( index.isValid() && ( index.row() < (int)_selection.size() ) ) {
      return _selection[index.row()]._occurrence;
    }

    return Occurrence ();
  }


  void  SelectionModel::toggleSelection ( Occurrence occurrence )
  {
    bool   found = false;
    size_t i     = 0;
    for ( ; i<_selection.size() ; i++ ) {
      if ( (not found) and (_selection[i]._occurrence == occurrence) ) {
        found = true;
        break;
      }
    }

    if ( not found ) {
      _selection.push_back ( OccurrenceItem(occurrence) );
    }
    _selection[i].toggle ();
    
  //emit dataChanged ( index(i,0), index(i,1) );
  }


  // void  SelectionModel::toggleSelection ( Occurrence occurrence )
  // {
  //   bool   found = false;
  //   size_t i     = 0;
  //   for ( ; i<_selection.size() ; i++ ) {
  //     if ( (not found) and (_selection[i]._occurrence == occurrence) ) {
  //       found = true;
  //       if ( isCumulative() ) break;
  //     }
  //     if (  found and ( i < _selection.size()-1 ) )
  //       _selection[i] = _selection[i+1];
  //   }

  //   if ( not found ) {
  //     _selection.push_back ( OccurrenceItem(occurrence) );
  //   } else {
  //     if ( isCumulative() ) _selection[i].toggle ();
  //     else                  _selection.pop_back ();
  //   }
    
  //   reset ();
  // }


  QVariant  SelectionModel::data ( const QModelIndex& index, int role ) const
  {
    static QBrush       unselectForeground = QBrush ( QColor(255,0,0) );
    static QFont        occurrenceFont     = Graphics::getFixedFont ( QFont::Normal );
    static QFont        unselectFont       = Graphics::getFixedFont ( QFont::Normal, true );
    static QFont        selectFont         = Graphics::getFixedFont ( QFont::Bold, false );
    static QFontMetrics entityMetrics      = QFontMetrics(selectFont);

    if ( !index.isValid() ) return QVariant ();

    if ( role == Qt::SizeHintRole ) {
      if ( index.row() == 0 ) return QVariant();
      switch (index.column()) {
        case 0:  return 200;
        default:
          if ( index.row() > (int)_selection.size() ) return 0;
          return entityMetrics.width(getString(_selection[index.row()]._occurrence.getEntity()).c_str());
      }
    }

    int row = index.row ();
    if ( row >= (int)_selection.size() ) return QVariant ();

    if ( role == Qt::FontRole ) {
      switch (index.column()) {
        case 0:  return occurrenceFont;
        case 1:
          if ( _selection[row]._flags & OccurrenceItem::Selected )
            return selectFont;
        default:
          return unselectFont;
      }
    }

    if ( role == Qt::ForegroundRole ) {
      if ( _selection[row]._flags & OccurrenceItem::Selected ) return QVariant();
      return unselectForeground;
    }

    if ( role == Qt::DisplayRole ) {
      switch ( index.column() ) {
        case 0: return getString(_selection[row]._occurrence.getPath().getName()).c_str();
        case 1: return getString(_selection[row]._occurrence.getEntity()).c_str();
      }
    }
    return QVariant();
  }


  QVariant  SelectionModel::headerData ( int              section
                                        , Qt::Orientation orientation
                                        , int             role ) const
  {
    if ( orientation == Qt::Vertical )
      return QVariant();

    static QFont headerFont = Graphics::getFixedFont ( QFont::Bold, false, false, +2 );

    if ( role == Qt::FontRole    ) return headerFont;
    if ( role != Qt::DisplayRole ) return QVariant();

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
