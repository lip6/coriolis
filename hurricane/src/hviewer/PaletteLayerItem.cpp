
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
// |  C++ Module  :       "./PaletteLayerItem.cpp"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <QCheckBox>
# include  <QHBoxLayout>

# include  "hurricane/BasicLayer.h"

# include  "hurricane/viewer/Graphics.h"
# include  "hurricane/viewer/PaletteLayerItem.h"


namespace Hurricane {


  PaletteLayerItem::PaletteLayerItem ( BasicLayer* basicLayer, bool visible )
    : PaletteItem()
    , _basicLayer(basicLayer)
    , _visible   (new QCheckBox())
    , _selectable(new QCheckBox())
    , _sample    (new DrawingStyleSample(this))
  {
    QHBoxLayout* layout = new QHBoxLayout ();
    layout->setContentsMargins ( 0, 0, 0, 0 );

    layout->addWidget ( _sample );

    _visible->setChecked ( visible );
    _visible->setText    ( getString(getName()).c_str() );
    _visible->setFont    ( Graphics::getFixedFont() );

    _selectable->setFixedWidth ( 23 );
    _selectable->setChecked ( true );
    _selectable->setStyleSheet ( "QCheckBox { background-color: red;"
                                 "            padding:          5px }" );

    layout->addWidget  ( _selectable );
  //layout->addSpacing ( -15 );
    layout->addWidget  ( _visible    );
    layout->addStretch ();

    setLayout ( layout );

    connect ( _visible   , SIGNAL(clicked     ()), this  , SIGNAL(visibleToggled   ()) );
    connect ( _selectable, SIGNAL(clicked     ()), this  , SIGNAL(selectableToggled()) );
  }


  PaletteLayerItem* PaletteLayerItem::create ( BasicLayer* basicLayer, bool visible )
  {
    PaletteLayerItem* item = new PaletteLayerItem ( basicLayer, visible );
    return item;
  }


  const Name& PaletteLayerItem::getName () const
  {
    return _basicLayer->getName();
  }


  void  PaletteLayerItem::changeStyle ()
  {
    _sample->redraw ();
  }


  bool  PaletteLayerItem::isItemVisible () const
  {
    return _visible->isChecked ();
  }


  void  PaletteLayerItem::setItemVisible ( bool state )
  {
    _visible->setChecked ( state );
  }


  bool  PaletteLayerItem::isItemSelectable () const
  {
    return _selectable->isChecked ();
  }


  void  PaletteLayerItem::setItemSelectable ( bool state )
  {
    _selectable->setChecked ( state );
  }


} // End of Hurricane namespace.
