// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
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
    , _cellWidget(NULL)
    , _selection ()
  { }


  SelectionModel::~SelectionModel ()
  { }


  bool  SelectionModel::isCumulative () const
  {
    SelectionWidget* widget = qobject_cast<SelectionWidget*>(QObject::parent());
    if (widget)
      return widget->cumulativeSelection();
    return false;
  }


  void  SelectionModel::setCellWidget ( CellWidget* w )
  {
    if (_cellWidget) {
      disconnect( _cellWidget, 0, this       , 0 );
      disconnect( this       , 0, _cellWidget, 0 );
    }

    _cellWidget = w;
    if (not _cellWidget) return;

    connect( _cellWidget, SIGNAL(unlinkSelector(Selector*)), this, SLOT(unlink(Selector*)) );
    connect( _cellWidget, SIGNAL(cellPreModificated())     , this, SLOT(clear()) );
  }


  void  SelectionModel::clear ()
  {
    beginResetModel();
    for ( Selector* selector : _selection ) {
      selector->resetFlags( _cellWidget, Selector::InModel|Selector::Selected );
    }
    _selection.clear();
    endResetModel();
  }


  void  SelectionModel::unlink ( Selector* selector )
  {
    beginResetModel();
    for ( auto it=_selection.begin() ; it != _selection.end() ; ++it ) {
      if ((*it) == selector) {
        _selection.erase( it );
        break;
      }
    }
    endResetModel();
  }


  void  SelectionModel::setSelection ( const SelectorSet& selection )
  {
    if (not _cellWidget) return;
    
    if (not isCumulative()) clear ();
    beginResetModel();

    for ( Selector* selector : selection ) {
      if (not selector->isInModel(_cellWidget)) {
        _selection.push_back( selector );
      }
      selector->setFlags( _cellWidget, Selector::InModel|Selector::Selected );
    }

    endResetModel();
  }


  void  SelectionModel::setSelection ( Selector* selector )
  {
    if (not _cellWidget) return;
    
    bool modificated = false;
    if (not isCumulative()) {
      modificated = true;
      beginResetModel ();
      _selection.clear ();
    }

    if (not selector->isInModel(_cellWidget)) {
      if (not modificated) {
        modificated = true;
        beginResetModel ();
      }
      _selection.push_back( selector );
    }
    selector->setFlags( _cellWidget, Selector::InModel|Selector::Selected );
    
    if (modificated) endResetModel();
  }


  Selector* SelectionModel::toggleSelection ( const QModelIndex& oindex )
  {
    if ( oindex.isValid() and (oindex.row() < (int)_selection.size()) ) {
      _selection[oindex.row()]->toggle( _cellWidget );
      emit dataChanged( index(oindex.row(),0), index(oindex.row(),1) );

      return _selection[ oindex.row() ];
    }

    return NULL;
  }


  Selector* SelectionModel::getSelector ( const QModelIndex& index )
  {
    if ( index.isValid() and (index.row() < (int)_selection.size()) ) {
      return _selection[ index.row() ];
    }

    return NULL;
  }


  void  SelectionModel::toggleSelection ( Selector* selector )
  {
    if (not _cellWidget) return;
    
    selector->toggle( _cellWidget );
    
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
    static QBrush       unselectForeground = QBrush( QColor(255,0,0) );
    static QFont        occurrenceFont     = Graphics::getFixedFont( QFont::Normal );
    static QFont        unselectFont       = Graphics::getFixedFont( QFont::Normal, true );
    static QFont        selectFont         = Graphics::getFixedFont( QFont::Bold, false );
    static QFontMetrics entityMetrics      = QFontMetrics( selectFont );

    if (not index.isValid()) return QVariant();

    if (role == Qt::SizeHintRole) {
      switch ( index.column() ) {
        case 0:  return 200;
        default:
          if (index.row() > (int)_selection.size()) return 0;
          return entityMetrics.width(getString( _selection[index.row()]->getEntity() ).c_str());
      }
    }

    int row = index.row ();
    if (row >= (int)_selection.size()) return QVariant ();

    if (role == Qt::FontRole) {
      switch (index.column()) {
        case 0:  return occurrenceFont;
        case 1:
          if (_selection[row]->isSelected(_cellWidget))
            return selectFont;
        default:
          return unselectFont;
      }
    }

    if ( role == Qt::ForegroundRole ) {
      if (_selection[row]->isSelected(_cellWidget)) return QVariant();
      return unselectForeground;
    }

    if (role == Qt::DisplayRole) {
      switch ( index.column() ) {
        case 0: return getString(_selection[row]->getPath().getName()).c_str();
        case 1: return getString(_selection[row]->getEntity()).c_str();
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

    static QFont headerFont = Graphics::getFixedFont ( QFont::Bold, false, false, +0 );

    if ( role == Qt::FontRole    ) return headerFont;
    if ( role != Qt::DisplayRole ) return QVariant();

    switch ( section ) {
      case 0: return "Path";
      case 1: return "Entity";
    }

    return QVariant();
  }


  int  SelectionModel::rowCount ( const QModelIndex& parent ) const
  { return _selection.size(); }


  int  SelectionModel::columnCount ( const QModelIndex& parent ) const
  {
    return 2;
  }


  const Selector* SelectionModel::getSelector ( int row )
  {
    if (row >= (int)_selection.size()) return NULL;
    return _selection[ row ];
  }


} // End of Hurricane namespace.
