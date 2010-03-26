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
// |  C++ Header  :       "./NetlistModel.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_NETLIST_MODEL__
#define  __HURRICANE_NETLIST_MODEL__

#include  <vector>

#include  <QFont>
#include  <QApplication>
#include  <QAbstractTableModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Name.h"
#include  "hurricane/Net.h"
#include  "hurricane/Cell.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/NetInformations.h"


namespace Hurricane {


  class Net;
  class Cell;


  class NetlistModel : public QAbstractTableModel {
      Q_OBJECT;

    public:
                                            NetlistModel ( QObject* parent=NULL );
                                           ~NetlistModel ();
      template<typename InformationType>
             void                           setCell      ( Cell* cell );
             int                            rowCount     ( const QModelIndex& parent=QModelIndex() ) const;
             int                            columnCount  ( const QModelIndex& parent=QModelIndex() ) const;
             QVariant                       data         ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
             QVariant                       headerData   ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      inline Cell*                          getCell      ();
             const Net*                     getNet       ( int row );

    private:
             Cell*                          _cell;
             AbstractNetInformationsVector* _netlist;
  };


// Inline Functions.

  inline Cell* NetlistModel::getCell () { return _cell; }


// Template Functions.

  template<typename InformationType>
  void  NetlistModel::setCell ( Cell* cell )
  {
    if ( _cell != cell ) {
      if ( _cell )
        delete _netlist;

      _cell    = cell;
      _netlist = new NetInformationsVector<InformationType>();

      if ( _cell ) {
        forEach ( Net*, net, _cell->getNets() ) {
          _netlist->addNet ( *net );
        }
      }

      emit layoutChanged ();
    }
  }


} // End of Hurricane namespace.


#endif // __NETLIST_MODEL_H__ 
