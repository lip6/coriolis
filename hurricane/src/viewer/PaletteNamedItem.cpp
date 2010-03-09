
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
// |  C++ Module  :       "./PaletteNamedItem.cpp"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QCheckBox>
#include  <QHBoxLayout>

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/PaletteNamedItem.h"


namespace Hurricane {


  PaletteNamedItem::PaletteNamedItem ( const Name& name, bool visible )
    : PaletteItem()
    , _visible   (new QCheckBox(this))
    , _name      (name)
  {
    QHBoxLayout* layout = new QHBoxLayout ();
    layout->setContentsMargins ( 0, 0, 0, 0 );

    _visible->setChecked ( visible );
    _visible->setText    ( getString(getName()).c_str() );
    _visible->setFont    ( Graphics::getFixedFont() );
    layout->addWidget ( _visible );

    setLayout ( layout );

    connect ( _visible, SIGNAL(clicked()), this, SIGNAL(visibleToggled()) );
  }


  PaletteNamedItem* PaletteNamedItem::create ( const Name& name, bool visible )
  {
    PaletteNamedItem* item = new PaletteNamedItem ( name, visible );
    return item;
  }


  const Name& PaletteNamedItem::getName () const
  {
    return _name;
  }


  bool  PaletteNamedItem::isItemVisible () const
  {
    return _visible->isChecked ();
  }


  void  PaletteNamedItem::setItemVisible ( bool state )
  {
    _visible->setChecked ( state );
  }


} // End of Hurricane namespace.
