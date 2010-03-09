
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
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingErrorModel.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QFont>
#include  <QApplication>

#include  "hurricane/Name.h"
#include  "hurricane/Net.h"
#include  "hurricane/Cell.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/NetInformations.h"

#include  "solstice/RoutingErrorModel.h"
#include  "solstice/RoutingError.h"
#include  "solstice/ShortCircuitError.h"

namespace Solstice {


  RoutingErrorModel::RoutingErrorModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _cell(NULL)
    , _routingError(NULL)
  { }


  RoutingErrorModel::~RoutingErrorModel ()
  {
    if ( _routingError ) delete _routingError; 
  }


  QVariant  RoutingErrorModel::data ( const QModelIndex& index, int role ) const
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
      return _routingError->getRow(row)->getColumn(index.column());
    }
    return QVariant();
  }


  QVariant  RoutingErrorModel::headerData ( int             section
                                     , Qt::Orientation orientation
                                     , int             role ) const
  {
    if ( ( orientation == Qt::Vertical ) || (role != Qt::DisplayRole) )
      return QVariant();

    if ( !_routingError ) {
      if ( section == 0 ) return QVariant("RoutingError");
      return QVariant();
    }

    if ( section < _routingError->getColumnCount() )
      return _routingError->getColumnName(section);

    return QVariant();
  }


  int  RoutingErrorModel::rowCount ( const QModelIndex& parent ) const
  {
    return (_routingError) ? _routingError->size() : 0;
  }


  int  RoutingErrorModel::columnCount ( const QModelIndex& parent ) const
  {
    return (_routingError) ? _routingError->getColumnCount() : 1;
  }


  const RoutingError* RoutingErrorModel::getRoutingError ( int row )
  {
    if ( !_routingError || ( row >= (int)_routingError->size() ) ) return NULL;

    return _routingError->getRow(row)->getRoutingError();
  }


   void  RoutingErrorModel::setCell ( Cell* cell )
   {
     if ( _cell != cell ) {
       if ( _cell )
         delete _routingError;
  
       _cell    = cell;
       _routingError = new RoutingErrorInformationsVector();
       if ( _cell ) {
  	SolsticeEngine* solstice = SolsticeEngine::get(_cell);
  	if (solstice) {
	  forEach(RoutingError*, sce ,getCollection(solstice->getRoutingErrorSet()))
	    {
	      _routingError->addRoutingError((*sce));
	    }
  	}
       }
  
       emit layoutChanged ();
     }
   }

} // End of Solstice namespace.
