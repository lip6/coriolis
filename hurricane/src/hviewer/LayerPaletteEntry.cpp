
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
// |  C++ Module  :       "./LayerPaletteEntry.cpp"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <QCheckBox>
# include  <QHBoxLayout>

# include  "hurricane/BasicLayer.h"

# include  "hurricane/viewer/Graphics.h"
# include  "hurricane/viewer/LayerPaletteEntry.h"
# include  "hurricane/viewer/HPalette.h"


namespace Hurricane {


  LayerPaletteEntry::LayerPaletteEntry ( HPalette* entry, BasicLayer* basicLayer )
    : HPaletteEntry(entry)
    , _basicLayer(basicLayer)
  {
  }


  LayerPaletteEntry* LayerPaletteEntry::create ( HPalette* palette, BasicLayer* basicLayer )
  {
    LayerPaletteEntry* entry = new LayerPaletteEntry ( palette, basicLayer );

    entry->_postCreate ();

    return entry;
  }


  void  LayerPaletteEntry::_postCreate ()
  {
    QHBoxLayout* layout = new QHBoxLayout ();
    layout->setContentsMargins ( 0, 0, 0, 0 );

    layout->addWidget ( new HPaletteSample(this) );

    _checkBox = new QCheckBox ( this );
    _checkBox->setChecked ( true );
    _checkBox->setText    ( getString(getName()).c_str() );
    _checkBox->setFont    ( Graphics::getFixedFont() );
    layout->addWidget ( _checkBox );

    setLayout ( layout );

    connect ( _checkBox, SIGNAL(clicked()), this, SLOT(toggle()) );
  }


  bool LayerPaletteEntry::isGroup () const
  {
    return false;
  }


  bool LayerPaletteEntry::isBasicLayer () const
  {
    return true;
  }


  const Name& LayerPaletteEntry::getName () const
  {
    return _basicLayer->getName();
  }


  BasicLayer* LayerPaletteEntry::getBasicLayer ()
  {
    return _basicLayer;
  }


  bool  LayerPaletteEntry::isChecked () const
  {
    return _checkBox->isChecked ();
  }


  void  LayerPaletteEntry::setChecked ( bool state )
  {
    _checkBox->setChecked ( state );
  }


  void  LayerPaletteEntry::toggle ()
  {
    _palette->redrawCellWidget();
  }


} // End of Hurricane namespace.
