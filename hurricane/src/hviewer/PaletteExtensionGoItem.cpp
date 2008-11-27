
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
// |  C++ Module  :       "./PaletteExtensionGoItem.cpp"             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QCheckBox>
#include  <QHBoxLayout>

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/PaletteExtensionGoItem.h"


namespace Hurricane {


  PaletteExtensionGoItem::PaletteExtensionGoItem ( const Name& name )
    : PaletteItem()
    , _name(name)
  { }


  PaletteExtensionGoItem* PaletteExtensionGoItem::create ( const Name& name )
  {
    PaletteExtensionGoItem* item = new PaletteExtensionGoItem ( name );
    item->_postCreate ();
    return item;
  }


  void  PaletteExtensionGoItem::_postCreate ()
  {
    QHBoxLayout* layout = new QHBoxLayout ();
    layout->setContentsMargins ( 0, 0, 0, 0 );

    _checkBox = new QCheckBox ( this );
    _checkBox->setChecked ( false );
    _checkBox->setText    ( getString(getName()).c_str() );
    _checkBox->setFont    ( Graphics::getFixedFont() );
    layout->addWidget ( _checkBox );

    setLayout ( layout );

    connect ( _checkBox, SIGNAL(clicked()), this, SIGNAL(toggled()) );
  }


  const Name& PaletteExtensionGoItem::getName () const
  {
    return _name;
  }


  bool  PaletteExtensionGoItem::isChecked () const
  {
    return _checkBox->isChecked ();
  }


  void  PaletteExtensionGoItem::setChecked ( bool state )
  {
    _checkBox->setChecked ( state );
  }


} // End of Hurricane namespace.
