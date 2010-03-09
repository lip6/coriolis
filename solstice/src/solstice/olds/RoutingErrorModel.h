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
// |  C++ Header  :       "./RoutingErrorModel.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __SOLSTICE_ROUTINGERROR_MODEL__
#define __SOLSTICE_ROUTINGERROR_MODEL__

#include  <vector>

#include  <QFont>
#include  <QApplication>
#include  <QAbstractTableModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Name.h"
#include  "hurricane/Net.h"
#include  "hurricane/Cell.h"
#include  "hurricane/viewer/Graphics.h"
#include  "solstice/RoutingErrorInformations.h"
#include  "solstice/SolsticeEngine.h"

namespace {


  using namespace Hurricane;


} // End of anonymous namespace.

namespace Solstice {
 

  class RoutingErrorModel : public QAbstractTableModel {
      Q_OBJECT;

    public:
                                            RoutingErrorModel ( QObject* parent=NULL );
                                           ~RoutingErrorModel ();
             void                           setCell      ( Cell* cell );
             int                            rowCount     ( const QModelIndex& parent=QModelIndex() ) const;
             int                            columnCount  ( const QModelIndex& parent=QModelIndex() ) const;
             QVariant                       data         ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
             QVariant                       headerData   ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      inline Cell*                          getCell      ();
             const RoutingError*                     getRoutingError      ( int row );

    private:
             Cell*                          _cell;
             RoutingErrorInformationsVector* _routingError;
  };


// Inline Functions.

  inline Cell* RoutingErrorModel::getCell () { return _cell; }

} // End of Solstice namespace.


#endif // _SOLSTICE_ROUTINGERROR_MODEL__ 
