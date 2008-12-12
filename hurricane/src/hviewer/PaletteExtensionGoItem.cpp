
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
    , _visible(NULL)
    , _selectable(NULL)
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

    _visible = new QCheckBox ( this );
    _visible->setChecked ( false );
    _visible->setText    ( getString(getName()).c_str() );
    _visible->setFont    ( Graphics::getFixedFont() );

    _selectable = new QCheckBox ( this );
    _selectable->setFixedWidth ( 23 );
    _selectable->setChecked    ( true );
    _selectable->setStyleSheet ( "QCheckBox { background-color: red;"
                                 "            padding:          5px }" );

    layout->addWidget ( _selectable );
    layout->addWidget ( _visible );
    setLayout ( layout );

    connect ( _visible   , SIGNAL(clicked()), this, SIGNAL(visibleToggled   ()) );
    connect ( _selectable, SIGNAL(clicked()), this, SIGNAL(selectableToggled()) );
  }


  const Name& PaletteExtensionGoItem::getName () const
  {
    return _name;
  }


  bool  PaletteExtensionGoItem::isItemVisible () const
  {
    return _visible->isChecked ();
  }


  void  PaletteExtensionGoItem::setItemVisible ( bool state )
  {
    _visible->setChecked ( state );
  }


  bool  PaletteExtensionGoItem::isItemSelectable () const
  {
    return _selectable->isChecked ();
  }


  void  PaletteExtensionGoItem::setItemSelectable ( bool state )
  {
    _visible->setChecked ( state );
  }


} // End of Hurricane namespace.
