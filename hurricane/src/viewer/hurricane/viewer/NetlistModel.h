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
// |  E-mail      :       Jean-Paul.Chaput@lip6.fr                   |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/NetlistModel.h"        |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_NETLIST_MODEL_H
#define  HURRICANE_NETLIST_MODEL_H

#include <vector>
#include <QFont>
#include <QApplication>
#include <QAbstractTableModel>
#include "hurricane/Commons.h"
#include "hurricane/Name.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/NetInformations.h"


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
    if (_cell != cell) {
      emit layoutAboutToBeChanged ();

      if (_cell) delete _netlist;

      _cell    = cell;
      _netlist = new NetInformationsVector<InformationType>();

      if (_cell) {
        for ( Net* net : _cell->getNets() ) _netlist->addNet( net );
      }

      emit layoutChanged ();
    }
  }


} // Hurricane namespace.

#endif // HURRICANE_NETLIST_MODEL_H
