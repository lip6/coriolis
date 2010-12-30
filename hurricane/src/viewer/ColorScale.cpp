
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
// |  C++ Module  :       "./ColorScale.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/viewer/ColorScale.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::ColorScale"


  ColorScale::ColorScale ( const Name& name )
    : _name(name)
  {
    for ( size_t i=0 ; i<256 ; i++ ) {
      _red  [i] = 0;
      _green[i] = 0;
      _blue [i] = 0;
      _color[i] = NULL;
      _brush[i] = NULL;
    }
  }


  ColorScale::~ColorScale ()
  {
    if ( _brush[0] ) {
      for ( size_t i=0 ; i<256 ; i++ ) {
        delete _brush[i];
        delete _color[i];
      }
    }
  }


  void  ColorScale::qtAllocate ()
  {
    if ( !_brush[0] ) {
      for ( size_t i=0 ; i<256 ; i++ ) {
        _color[i] = new QColor ( _red[i], _green[i], _blue[i] );
        _brush[i] = new QBrush ( *_color[i] );
      }
    }
  }


  QBrush  ColorScale::getBrush ( size_t i, const DisplayStyle::HSVr& darkening ) const
  {
    assert ( _brush[i] != NULL );

    QBrush brush ( *_brush[i] );
    brush.setColor ( DisplayStyle::darken(*_color[i],darkening) );
    return brush;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::FireColorScale"


  FireColorScale::FireColorScale ()
    : ColorScale("Fire")
  {
    for ( size_t i=0 ; i<128 ; i++ ) {
      _red  [i] = i * 2;
      _green[i] = 0;
      _blue [i] = 0;
    }
    for ( size_t i=128 ; i<224 ; i++ ) {
      _red  [i] = 255;
      _green[i] = ((i-128)*8)/3;
      _blue [i] = 0;
    }
    for ( size_t i=224 ; i<256 ; i++ ) {
      _red  [i] = 255;
      _green[i] = 255;
      _blue [i] = (i-224)*8;
    }
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::RainbowColorScale"


  RainbowColorScale::RainbowColorScale ()
    : ColorScale("Rainbow")
  {
    size_t slice = 51;
    for ( size_t i=0 ; i<256 ; i++ ) {
      switch ( i / slice ) {
        case 0:  // Red --> Yellow.
          _red  [i] = 255;
          _green[i] = ( 255 * i ) / slice;
          _blue [i] = 0;
          break;
        case 1:  // Yellow --> Green.
          _red  [i] = 510 - ( 255 * i ) / slice;
          _green[i] = 255;
          _blue [i] = 0;
          break;
        case 2:  // Green --> Cyan.
          _red  [i] = 0;
          _green[i] = 255;
          _blue [i] = ( 255 * i ) / slice - 510;
          break;
        case 3:  // Cyan --> Blue.
          _red  [i] = 0;
          _green[i] = 1020 - ( 255 * i ) / slice;
          _blue [i] = 255;
          break;
        default:  // Blue --> Magenta.
          _red  [i] = ( 255 * i ) / slice - 1020;
          _green[i] = 0;
          _blue [i] = 255;
          break;
      }
    }
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TemperatureColorScale"


  TemperatureColorScale::TemperatureColorScale ()
    : ColorScale("Temperature")
  {
    for ( size_t i=0 ; i<153 ; i++ ) {
      _red  [i] = 0;
      _green[i] = 0;
      _blue [i] = (size_t)(0.83*(float)(i))+128;
    }
    for ( size_t i=153 ; i<204 ; i++ ) {
      _red  [i] = (size_t)(2.5*(float)(i)-382.5);
      _green[i] = 0;
      _blue [i] = 255;
    }
    for ( size_t i=204 ; i<256 ; i++ ) {
      _red  [i] = (size_t)(2.5*(float)(i)-382.5);
      _green[i] = 0;
      _blue [i] = (size_t)(-5*(int)(i)+1275);
    }
  }


} // End of Hurricane namespace.
