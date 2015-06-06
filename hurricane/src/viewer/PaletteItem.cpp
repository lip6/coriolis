// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PaletteItem.cpp"                        |
// +-----------------------------------------------------------------+


#include  <QPainter>
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/PaletteItem.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::DrawingStyleSample".


  int  DrawingStyleSample::_side   = 20;
  int  DrawingStyleSample::_border = 2;


  DrawingStyleSample::DrawingStyleSample ( PaletteItem* entry )
    : QWidget()
    , _sample(NULL)
    , _entry(entry)
  {
    if (Graphics::isHighDpi()) {
      _side   = 40;
      _border =  4;
    }
    _sample = new QPixmap( _side, _side );

    setAttribute ( Qt::WA_StaticContents );
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    setFixedSize ( _side, _side );

    redraw();
  }


  DrawingStyleSample::~DrawingStyleSample ()
  { delete _sample; }


  void  DrawingStyleSample::redraw ()
  {
    QPainter  painter ( _sample );

    painter.setPen       ( Qt::NoPen );
    painter.setBackground( Graphics::getBrush("background") );
    painter.eraseRect    ( 0, 0, _side, _side );
    painter.setPen       ( Graphics::getPen  (_entry->getName()) );
    painter.setBrush     ( Graphics::getBrush(_entry->getName()) );
    painter.drawRect     ( _border, _border, _side-2*_border, _side-2*_border );

    update ();
  }


  void  DrawingStyleSample::paintEvent ( QPaintEvent* )
  {
    QPainter painter( this );
    painter.drawPixmap( 0, 0, *_sample );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::PaletteItem".


  PaletteItem::PaletteItem ()
    : QWidget()
  { }


  bool  PaletteItem::isItemSelectable () const
  {
    return true;
  }


    void  PaletteItem::setItemSelectable ( bool )
  { }


  void  PaletteItem::changeStyle ()
  { }


} // End of Hurricane namespace.
