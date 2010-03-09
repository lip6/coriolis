
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
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
// |  C++ Module  :       "./PaletteItem.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <QPainter>

# include  "hurricane/viewer/Graphics.h"
# include  "hurricane/viewer/PaletteItem.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::DrawingStyleSample".


  DrawingStyleSample::DrawingStyleSample ( PaletteItem* entry )
    : QWidget()
    , _sample(QSize(20,20))
    , _entry(entry)
  {
    setAttribute  ( Qt::WA_StaticContents );
    setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    setFixedSize  ( 20, 20 );

    redraw ();
  }


  void  DrawingStyleSample::redraw ()
  {
    QPainter  painter ( &_sample );

    painter.setPen        ( Qt::NoPen );
    painter.setBackground ( Graphics::getBrush("background") );
    painter.eraseRect     ( 0, 0, 20, 20 );
    painter.setPen        ( Graphics::getPen  (_entry->getName()) );
    painter.setBrush      ( Graphics::getBrush(_entry->getName()) );
    painter.drawRect      ( 2, 2, 16, 16 );

    update ();
  }


  void  DrawingStyleSample::paintEvent ( QPaintEvent* )
  {
    QPainter painter ( this );
    painter.drawPixmap ( 0, 0, _sample );
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
