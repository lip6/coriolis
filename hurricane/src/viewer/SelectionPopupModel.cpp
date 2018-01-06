// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./SelectionPopupModel.cpp"                |
// +-----------------------------------------------------------------+


#include <QFont>
#include "hurricane/Path.h"
#include "hurricane/Entity.h"
#include "hurricane/Occurrence.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/Selector.h"
#include "hurricane/viewer/SelectionPopupModel.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Occurrence_AcceptAll".


  class Occurrence_AcceptAll : public Filter<Occurrence> {
    public:
      virtual Filter<Occurrence>* getClone   () const;
      virtual bool                accept     ( Occurrence ) const;
      virtual string              _getString () const;
  };


  Filter<Occurrence>* Occurrence_AcceptAll::getClone   () const { return new Occurrence_AcceptAll(); }
  bool                Occurrence_AcceptAll::accept     ( Occurrence ) const { return true; }
  string              Occurrence_AcceptAll::_getString () const { return "<Occurrence_AcceptAll>"; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::SelectionPopupModel".


  SelectionPopupModel::SelectionPopupModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _filter            (new Occurrence_AcceptAll())
    , _occurrences       (NULL)
    , _charWidth         (50)
  { }


  SelectionPopupModel::~SelectionPopupModel ()
  { clear (); }


  void  SelectionPopupModel::clearFilter ()
  { _filter = new Occurrence_AcceptAll(); }


  void  SelectionPopupModel::setFilter ( OccurrenceFilter filter )
  { _filter = filter; }


  OccurrenceFilter  SelectionPopupModel::getFilter ()
  { return _filter; }


  int  SelectionPopupModel::charWidth () const
  { return _charWidth; }


  void  SelectionPopupModel::loadOccurrences ( Occurrences occurrences, bool showChange )
  {
    _charWidth = 50;
    if (not _occurrences) _occurrences = new vector<Occurrence> ();
    forEach ( Occurrence, ioccurrence, occurrences.getSubSet(getFilter()) ) {
      _occurrences->push_back( *ioccurrence );
      string name = getString( (*ioccurrence).getPath().getName() ) + "::"
                  + getString( (*ioccurrence).getEntity() );
      _charWidth = std::max( _charWidth, (int)name.size() );
    }
    if (showChange) emit layoutChanged ();
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
  { emit layoutChanged (); }


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
    if ( orientation == Qt::Vertical )
      return QVariant();

    static QFont headerFont = Graphics::getFixedFont ( QFont::Bold, false, false, +0 );

    if ( role == Qt::FontRole    ) return headerFont;
    if ( role != Qt::DisplayRole ) return QVariant();

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
