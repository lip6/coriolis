
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
// |  C++ Module  :       "./ViewerPaletteEntry.cpp"                 |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>


# include  <QCheckBox>
# include  <QHBoxLayout>

# include  "hurricane/BasicLayer.h"

# include  "hurricane/viewer/Graphics.h"
# include  "hurricane/viewer/ViewerPaletteEntry.h"
# include  "hurricane/viewer/HPalette.h"


namespace Hurricane {


  ViewerPaletteEntry::ViewerPaletteEntry ( HPalette* entry, const Name& name )
    : HPaletteEntry(entry)
    , _name(name)
  {
  }


  ViewerPaletteEntry* ViewerPaletteEntry::create ( HPalette* palette, const Name& name )
  {
    ViewerPaletteEntry* entry = new ViewerPaletteEntry ( palette, name );

    entry->_postCreate ();

    return entry;
  }


  void  ViewerPaletteEntry::_postCreate ()
  {
    QHBoxLayout* layout = new QHBoxLayout ();
    layout->setContentsMargins ( 0, 0, 0, 0 );

    layout->addWidget ( new HPaletteSample(this) );

    _checkBox = new QCheckBox ( this );
    _checkBox->setChecked ( true );
    _checkBox->setText    ( getString(getName()).c_str() );
    layout->addWidget ( _checkBox );

    setLayout ( layout );

    connect ( _checkBox, SIGNAL(clicked()), this, SLOT(toggle()) );
  }


  bool ViewerPaletteEntry::isGroup () const
  {
    return false;
  }


  bool ViewerPaletteEntry::isBasicLayer () const
  {
    return false;
  }


  const Name& ViewerPaletteEntry::getName () const
  {
    return _name;
  }


  BasicLayer* ViewerPaletteEntry::getBasicLayer ()
  {
    return NULL;
  }


  bool  ViewerPaletteEntry::isChecked () const
  {
    return _checkBox->isChecked ();
  }


  void  ViewerPaletteEntry::setChecked ( bool state )
  {
    _checkBox->setChecked ( state );
  }


  void  ViewerPaletteEntry::toggle ()
  {
    _palette->redrawCellWidget();
  }


} // End of Hurricane namespace.
