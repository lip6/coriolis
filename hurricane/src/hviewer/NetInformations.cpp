
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
// |  C++ Module  :       "./NetInformations.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>
#include  <QObject>

#include  "hurricane/Name.h"
#include  "hurricane/Net.h"
#include  "hurricane/viewer/NetInformations.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "NetInformations"


  int  NetInformations::getColumnCount ()
  { return 1; }


  QVariant  NetInformations::getColumnName ( int column )
  {
    switch ( column ) {
      case 0: return QVariant(QObject::tr("Net"));
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  QVariant  NetInformations::getColumn ( int column )
  {
    switch ( column ) {
      case 0: return QVariant(getString(getName()).c_str());
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  NetInformations::NetInformations ( const Net* net )
    : _net(net)
  { }


  NetInformations::~NetInformations ()
  { }


// -------------------------------------------------------------------
// Class  :  "SimpleNetInformations"


  int  SimpleNetInformations::getColumnCount ()
  { return 2; }


  QVariant  SimpleNetInformations::getColumnName ( int column )
  {
    switch ( column ) {
      case 0: return QVariant(QObject::tr("Net"));
      case 1: return QVariant(QObject::tr("Plugs"));
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  QVariant  SimpleNetInformations::getColumn ( int column )
  {
    switch ( column ) {
      case 0: return QVariant(getString(getName()).c_str());
      case 1: return QVariant((unsigned int)getPlugsCount());
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  SimpleNetInformations::SimpleNetInformations ( const Net* net )
    : NetInformations(net)
    , _plugsCount(_net->getPlugs().getSize())
  { }


  SimpleNetInformations::~SimpleNetInformations ()
  { }


// -------------------------------------------------------------------
// Class  :  "AbstractNetInformationsVector"


  AbstractNetInformationsVector::~AbstractNetInformationsVector  ()
  { }


} // End of Hurricane namespace.
