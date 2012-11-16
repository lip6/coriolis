
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ScreenUtilisties.cpp"                        |
// +-----------------------------------------------------------------+


#include  <cmath>
#include  <QBitmap>
#include  "hurricane/BasicLayer.h"
#include  "hurricane/viewer/ScreenUtilities.h"


namespace {


  inline bool charToBits ( uchar& b, char c, bool isMSB=true )
  {
    bool   isValid = true;
    uchar  value   = 0xF;

    if ( (c >= '0') && (c <= '9') ) value = (uchar)( c - '0' );
    else {
      if ( (c >= 'a') && (c <= 'f') ) value = (uchar)( c - 'a' + 10 );
      else {
        if ( (c >= 'A') && (c <= 'F') ) value = (uchar)( c - 'A' + 10 );
        else
          isValid = false;
      }
    }

    b &= (isMSB) ? 0x0F       : 0xF0;
    b |= (isMSB) ? (value<<4) : value;

    return isValid;
  } 


  bool isSolidFill ( const std::string& pattern )
  {
    for( size_t i=0 ; i<pattern.size() ; ++i ) {
      if (pattern[i] != 'F') return false;
    }
    return true;
  }


  bool  patternToBits ( uchar **bits, const std::string& pattern, int& size )
  {
    bool isValid = true;

    size = (int)sqrt((double)pattern.size()*4.0);
    if ( pow((double)size,2.0) != (double)pattern.size()*4.0 )
      isValid = false;

    *bits = new uchar[pattern.size()/2];

    for ( size_t i=0 ; i<pattern.size() ; ++i ) {
      isValid &= charToBits ( (*bits)[i/2], pattern[i], i%2 );
    }
    return isValid;
  }


}




namespace Hurricane {


QBrush* getBrush ( const string& pattern, int red, int green, int blue )
{
  if ( not isSolidFill(pattern) ) {
    uchar* bits = NULL;
    int    size = 0;
    
    if ( not patternToBits(&bits,pattern,size) )
      cerr << "[WARNING] Invalid bitmap pattern: \"0x" << pattern << "\"." << endl;
    
    QBrush* brush = new QBrush ( QColor(red,green,blue), QBitmap::fromData(QSize(size,size),bits,QImage::Format_Mono) );
    if (bits != NULL) delete bits;

    return brush;
  }

  return new QBrush ( QColor(red,green,blue), Qt::SolidPattern );
}


} // End of Hurricane namespace.
