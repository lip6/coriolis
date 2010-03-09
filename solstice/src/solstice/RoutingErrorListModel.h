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
// |  C++ Header  :       "./RoutingErrorListModel.h"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_ROUTINGERRORLIST_MODEL__
#define  __HURRICANE_ROUTINGERRORLIST_MODEL__

#include  <vector>

#include  <QFont>
#include  <QApplication>
#include  <QAbstractTableModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Name.h"

#include  "hurricane/Cell.h"
#include  "hurricane/viewer/Graphics.h"

#include  "solstice/RoutingError.h"
#include  "solstice/SolsticeEngine.h"
#include  "solstice/RoutingErrorInformations.h"

namespace Hurricane {

  class Cell;


}

namespace Solstice {




  class RoutingErrorListModel : public QAbstractTableModel {
      Q_OBJECT;

    public:
                                            RoutingErrorListModel ( QObject* parent=NULL );
                                           ~RoutingErrorListModel ();
      template<typename InformationType>
             void                           setCell      ( Cell* cell );
             int                            rowCount     ( const QModelIndex& parent=QModelIndex() ) const;
             int                            columnCount  ( const QModelIndex& parent=QModelIndex() ) const;
             QVariant                       data         ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
             QVariant                       headerData   ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      inline Cell*                          getCell      ();
             const RoutingError*                     getRoutingError       ( int row );

    private:
             Cell*                          _cell;
             AbstractRoutingErrorInformationsVector* _routingErrorList;
  };


// Inline Functions.

  inline Cell* RoutingErrorListModel::getCell () { return _cell; }


// Template Functions.

  template<typename InformationType>
  void  RoutingErrorListModel::setCell ( Cell* cell )
  {
    if ( _cell != cell ) {
      if ( _cell )
        delete _routingErrorList;

      _cell    = cell;
      _routingErrorList = new RoutingErrorInformationsVector<InformationType>();

      if ( _cell ) {
	SolsticeEngine* solstice = SolsticeEngine::get(_cell);
	forEach(RoutingError*, routingError ,getCollection(solstice->getRoutingErrorSet()))
	  {
	    _routingErrorList->addRoutingError ( *routingError );
	  }
      }
      
      emit layoutChanged ();
    }
  }


} // End of Hurricane namespace.


#endif // __ROUTINGERRORLIST_MODEL_H__ 
