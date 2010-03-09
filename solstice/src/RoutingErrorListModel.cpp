
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
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./RoutingErrorlistModel.cpp"              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#include  <QFont>
#include  <QApplication>

#include  "hurricane/Name.h"

#include  "hurricane/Cell.h"
#include  "hurricane/viewer/Graphics.h"
#include  "solstice/RoutingError.h"
#include  "solstice/RoutingErrorInformations.h"
#include  "solstice/RoutingErrorListModel.h"


namespace Solstice {


  RoutingErrorListModel::RoutingErrorListModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _cell(NULL)
    , _routingErrorList(NULL)
  { }


  RoutingErrorListModel::~RoutingErrorListModel ()
  {
    if ( _routingErrorList ) delete _routingErrorList; 
  }


  QVariant  RoutingErrorListModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont nameFont  = Graphics::getFixedFont ( QFont::Bold );
    static QFont valueFont = Graphics::getFixedFont ( QFont::Normal, true );

    if ( role == Qt::SizeHintRole ) {
      switch (index.column()) {
        case 0:  return 200;
        default: return -1;
      }
    }

    if ( role == Qt::FontRole ) {
      switch (index.column()) {
        case 0:  return nameFont;
        default: return valueFont;
      }
    }

    if ( !index.isValid() ) return QVariant ();

    if ( role == Qt::DisplayRole ) {
      int row = index.row ();
      return _routingErrorList->getRow(row)->getColumn(index.column());
    }
    return QVariant();
  }


  QVariant  RoutingErrorListModel::headerData ( int             section
                                     , Qt::Orientation orientation
                                     , int             role ) const
  {
    if ( ( orientation == Qt::Vertical ) || (role != Qt::DisplayRole) )
      return QVariant();

    if ( !_routingErrorList ) {
      if ( section == 0 ) return QVariant("RoutingError");
      return QVariant();
    }

    if ( section < _routingErrorList->getColumnCount() )
      return _routingErrorList->getColumnName(section);

    return QVariant();
  }


  int  RoutingErrorListModel::rowCount ( const QModelIndex& parent ) const
  {
    return (_routingErrorList) ? _routingErrorList->size() : 0;
  }


  int  RoutingErrorListModel::columnCount ( const QModelIndex& parent ) const
  {
    return (_routingErrorList) ? _routingErrorList->getColumnCount() : 1;
  }


  const RoutingError* RoutingErrorListModel::getRoutingError ( int row )
  {
    if ( !_routingErrorList || ( row >= (int)_routingErrorList->size() ) ) return NULL;

    return _routingErrorList->getRow(row)->getRoutingError();
  }


} // End of Hurricane namespace.
