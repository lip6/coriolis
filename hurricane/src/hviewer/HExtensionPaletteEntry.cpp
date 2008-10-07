
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
// |  C++ Module  :       "./HExtensionPaletteEntry.cpp"             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QCheckBox>
#include  <QHBoxLayout>

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/HExtensionPaletteEntry.h"
#include  "hurricane/viewer/HPalette.h"


namespace Hurricane {


  HExtensionPaletteEntry::HExtensionPaletteEntry ( HPalette* entry, const Name& name )
    : HPaletteEntry(entry)
    , _name(name)
  {
  }


  HExtensionPaletteEntry* HExtensionPaletteEntry::create ( HPalette* palette, const Name& name )
  {
    HExtensionPaletteEntry* entry = new HExtensionPaletteEntry ( palette, name );

    entry->_postCreate ();

    return entry;
  }


  void  HExtensionPaletteEntry::_postCreate ()
  {
    QHBoxLayout* layout = new QHBoxLayout ();
    layout->setContentsMargins ( 0, 0, 0, 0 );

    _checkBox = new QCheckBox ( this );
    _checkBox->setChecked ( true );
    _checkBox->setText    ( getString(getName()).c_str() );
    _checkBox->setFont    ( Graphics::getFixedFont() );
    layout->addWidget ( _checkBox );

    setLayout ( layout );

    connect ( _checkBox, SIGNAL(clicked()), this, SLOT(toggle()) );
  }


  bool HExtensionPaletteEntry::isGroup () const
  {
    return false;
  }


  bool HExtensionPaletteEntry::isBasicLayer () const
  {
    return false;
  }


  bool HExtensionPaletteEntry::isExtension () const
  {
    return true;
  }


  const Name& HExtensionPaletteEntry::getName () const
  {
    return _name;
  }


  BasicLayer* HExtensionPaletteEntry::getBasicLayer ()
  {
    return NULL;
  }


  bool  HExtensionPaletteEntry::isChecked () const
  {
    return _checkBox->isChecked ();
  }


  void  HExtensionPaletteEntry::setChecked ( bool state )
  {
    _checkBox->setChecked ( state );
  }


  void  HExtensionPaletteEntry::toggle ()
  {
    _palette->redrawCellWidget();
  }


} // End of Hurricane namespace.
