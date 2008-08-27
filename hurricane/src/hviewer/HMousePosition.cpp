
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
// |  C++ Module  :       "./HMousePosition.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <QHBoxLayout>

# include  "hurricane/viewer/DynamicLabel.h"
# include  "hurricane/viewer/HMousePosition.h"


namespace Hurricane {


  HMousePosition::HMousePosition ( QWidget* parent )
    : QWidget(parent)
    , _xPosition(NULL)
    , _yPosition(NULL)
  {
    _xPosition = new DynamicLabel ();
    _xPosition->setStaticText  ( "X:" );
    _xPosition->setDynamicText ( "N/A" );
    
    _yPosition = new DynamicLabel ();
    _yPosition->setStaticText  ( "Y:" );
    _yPosition->setDynamicText ( "N/A" );

    QHBoxLayout* layout = new QHBoxLayout ();
    layout->addWidget ( _xPosition );
    layout->addWidget ( _yPosition );
    layout->setContentsMargins ( 0, 0, 0, 0 );

    setLayout ( layout );
  }


  void  HMousePosition::setPosition ( const Point& position )
  {
    _xPosition->setDynamicText ( position.getX() );
    _yPosition->setDynamicText ( position.getY() );
  }


} // End of Hurricane namespace.
