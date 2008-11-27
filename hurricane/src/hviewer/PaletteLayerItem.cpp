
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


  PaletteLayerItem::PaletteLayerItem ( BasicLayer* basicLayer, bool checked )
    : PaletteItem()
    , _basicLayer(basicLayer)
  {
    QHBoxLayout* layout = new QHBoxLayout ();
    layout->setContentsMargins ( 0, 0, 0, 0 );

    layout->addWidget ( new DrawingStyleSample(this) );

    _checkBox = new QCheckBox ( this );
    _checkBox->setChecked ( checked );
    _checkBox->setText    ( getString(getName()).c_str() );
    _checkBox->setFont    ( Graphics::getFixedFont() );
    layout->addWidget ( _checkBox );

    setLayout ( layout );

    connect ( _checkBox, SIGNAL(clicked()), this, SIGNAL(toggled()) );
  }


  PaletteLayerItem* PaletteLayerItem::create ( BasicLayer* basicLayer, bool checked )
  {
    PaletteLayerItem* item = new PaletteLayerItem ( basicLayer, checked );
    return item;
  }


  const Name& PaletteLayerItem::getName () const
  {
    return _basicLayer->getName();
  }


  bool  PaletteLayerItem::isChecked () const
  {
    return _checkBox->isChecked ();
  }


  void  PaletteLayerItem::setChecked ( bool state )
  {
    _checkBox->setChecked ( state );
  }


} // End of Hurricane namespace.
