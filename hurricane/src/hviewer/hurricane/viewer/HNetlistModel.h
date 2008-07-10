
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
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
// |  C++ Header  :       "./HNetlistModel.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>
#ifndef  __NETLIST_MODEL_H__
#define  __NETLIST_MODEL_H__

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
#include  "hurricane/viewer/HNetlistModel.h"


namespace Hurricane {


  class Net;
  class Cell;


  class HNetlistModel : public QAbstractTableModel {
      Q_OBJECT;

    public:
                                            HNetlistModel ( QObject* parent=NULL );
                                           ~HNetlistModel ();
      template<typename InformationType>
             void                           setCell       ( Cell* cell );
             int                            rowCount      ( const QModelIndex& parent=QModelIndex() ) const;
             int                            columnCount   ( const QModelIndex& parent=QModelIndex() ) const;
             QVariant                       data          ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
             QVariant                       headerData    ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      inline Cell*                          getCell       ();
             const Net*                     getNet        ( int row );

    private:
             Cell*                          _cell;
             AbstractNetInformationsVector* _netlist;
  };


// Inline Functions.

  inline Cell* HNetlistModel::getCell () { return _cell; }


// Template Functions.

  template<typename InformationType>
  void  HNetlistModel::setCell ( Cell* cell )
  {
    if ( _cell != cell ) {
      if ( _cell )
        delete _netlist;

      _cell    = cell;
      _netlist = new NetInformationsVector<InformationType>();

      for_each_net ( net, _cell->getNets() ) {
        _netlist->addNet ( net );
        end_for;
      }

      emit layoutChanged ();
    }
  }


} // End of Hurricane namespace.


#endif // __NETLIST_MODEL_H__ 
