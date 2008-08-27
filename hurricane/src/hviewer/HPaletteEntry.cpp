
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
// |  C++ Module  :       "./HPaletteEntry.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <QPainter>

# include  "hurricane/viewer/Graphics.h"
# include  "hurricane/viewer/HPaletteEntry.h"


namespace Hurricane {


  HPaletteSample::HPaletteSample ( HPaletteEntry* entry )
    : QWidget()
    , _sample(QSize(20,20))
    , _entry(entry)
  {
    setAttribute  ( Qt::WA_StaticContents );
    setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    setFixedSize  ( 20, 20 );

    redraw ();
  }


  void  HPaletteSample::redraw ()
  {
    QPainter  painter ( &_sample );

    painter.setPen        ( Qt::NoPen );
    painter.setBackground ( Graphics::getBrush("background") );
    painter.eraseRect     ( 0, 0, 20, 20 );
    painter.setPen        ( Graphics::getPen  (_entry->getName()) );
    painter.setBrush      ( Graphics::getBrush(_entry->getName()) );
    painter.drawRect      ( 2, 2, 16, 16 );
  }


  void  HPaletteSample::paintEvent ( QPaintEvent* )
  {
    QPainter painter ( this );
    painter.drawPixmap ( 0, 0, _sample );
  }


  HPaletteEntry::HPaletteEntry ( HPalette* palette )
    : QWidget()
    , _palette(palette)
  {
  }


} // End of Hurricane namespace.
