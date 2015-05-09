// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetlistModel.cpp"                       |
// +-----------------------------------------------------------------+


#include <QFont>
#include <QApplication>
#include "hurricane/Name.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/NetInformations.h"
#include "hurricane/viewer/NetlistModel.h"


namespace Hurricane {


  NetlistModel::NetlistModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _cell(NULL)
    , _netlist(NULL)
  { }


  NetlistModel::~NetlistModel ()
  {
    if ( _netlist ) delete _netlist; 
  }


  QVariant  NetlistModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont nameFont  = Graphics::getFixedFont ( QFont::Bold );
    static QFont valueFont = Graphics::getFixedFont ( QFont::Normal, true );

    if ( role == Qt::FontRole ) {
      if ( index.row() == 0 ) return QVariant();
      switch (index.column()) {
        case 0:  return nameFont;
        default: return valueFont;
      }
      return QVariant();
    }

    if ( not index.isValid() ) return QVariant ();

    if ( role == Qt::DisplayRole ) {
      int row = index.row ();
      return _netlist->getRow(row)->getColumn(index.column());
    }
    return QVariant();
  }


  QVariant  NetlistModel::headerData ( int             section
                                     , Qt::Orientation orientation
                                     , int             role ) const
  {
    if ( orientation == Qt::Vertical ) return QVariant();

    static QFont headerFont = Graphics::getFixedFont ( QFont::Bold, false, false, +0 );

    if ( role == Qt::FontRole    ) return headerFont;
    if ( role != Qt::DisplayRole ) return QVariant();

    if ( not _netlist ) {
      if ( section == 0 ) return QVariant("Net");
      return QVariant();
    }

    if ( section < _netlist->getColumnCount() )
      return _netlist->getColumnName(section);

    return QVariant();
  }


  int  NetlistModel::rowCount ( const QModelIndex& parent ) const
  {
    return (_netlist) ? _netlist->size() : 0;
  }


  int  NetlistModel::columnCount ( const QModelIndex& parent ) const
  {
    return (_netlist) ? _netlist->getColumnCount() : 1;
  }


  const Net* NetlistModel::getNet ( int row )
  {
    if ( !_netlist || ( row >= (int)_netlist->size() ) ) return NULL;

    return _netlist->getRow(row)->getNet();
  }


} // End of Hurricane namespace.
