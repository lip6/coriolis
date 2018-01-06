// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PaletteLayerItem.cpp"                   |
// +-----------------------------------------------------------------+


#include  <sstream>
#include  <iomanip>
#include  <QCheckBox>
#include  <QHBoxLayout>
#include  "hurricane/BasicLayer.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/PaletteLayerItem.h"


namespace Hurricane {


  using std::ostringstream;
  using std::setw;


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

    ostringstream label;
    label << setw(12) << left << getName();
    _visible->setChecked ( visible );
    _visible->setText    ( label.str().c_str() );
    _visible->setFont    ( Graphics::getFixedFont() );

    if (not Graphics::isHighDpi())
      _selectable->setFixedWidth ( 23 );
    _selectable->setChecked    ( true );
    _selectable->setStyleSheet ( "QCheckBox { background-color: red;"
                                 "            padding:          2px 2px 6px 6px; }" );
  //_selectable->setStyleSheet ( "QCheckBox::indicator {"
  //                             "  border:  2px solid red;"
  //                             "}"
  //                           );

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
