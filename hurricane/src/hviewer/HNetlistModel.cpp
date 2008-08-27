
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
// |  C++ Module  :       "./HNetlistModel.cpp"                      |
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
#include  "hurricane/viewer/HNetlistModel.h"


namespace Hurricane {


  HNetlistModel::HNetlistModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _cell(NULL)
    , _netlist(NULL)
  { }


  HNetlistModel::~HNetlistModel ()
  {
    if ( _netlist ) delete _netlist; 
  }


  QVariant  HNetlistModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont nameFont  = Graphics::getFixedFont ( QFont::Bold );
    static QFont valueFont = Graphics::getFixedFont ( QFont::Normal, true );

    if ( !index.isValid() ) return QVariant ();

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

    if ( role == Qt::DisplayRole ) {
      int row = index.row ();
      return _netlist->getRow(row)->getColumn(index.column());
    }
    return QVariant();
  }


  QVariant  HNetlistModel::headerData ( int             section
                                      , Qt::Orientation orientation
                                      , int             role ) const
  {
    if ( !_netlist ) return QVariant();

    if (   ( orientation == Qt::Vertical )
        || ( section >= _netlist->getColumnCount() )
        || (role != Qt::DisplayRole) )
      return QVariant();

    return _netlist->getColumnName(section);
  }


  int  HNetlistModel::rowCount ( const QModelIndex& parent ) const
  {
    return (_netlist) ? _netlist->size() : 0;
  }


  int  HNetlistModel::columnCount ( const QModelIndex& parent ) const
  {
    return (_netlist) ? _netlist->getColumnCount() : 0;
  }


  const Net* HNetlistModel::getNet ( int row )
  {
    if ( !_netlist || ( row >= (int)_netlist->size() ) ) return NULL;

    return _netlist->getRow(row)->getNet();
  }


} // End of Hurricane namespace.
