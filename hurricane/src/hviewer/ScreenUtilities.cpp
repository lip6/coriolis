
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
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
// |  C++ Module  :       "./ScreenUtilisties.cpp"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>


# include  <QBitmap>

# include  "hurricane/BasicLayer.h"

# include  "hurricane/viewer/ScreenUtilities.h"




namespace {


  inline bool charToBits ( uchar& b, char c, bool low=true )
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

    b &= (low) ? 0xF0  : 0x0F;
    b += (low) ? value : (value<<4);

    return isValid;
  } 


  bool  getPattern ( uchar bits[], const std::string& pattern )
  {
    bool isValid = true;

    for ( size_t i=0 ; i<pattern.size() ; i++ ) {
      if ( i > 15 ) { isValid = false; break; }
      isValid &= charToBits ( bits[i/2], pattern[i], i%2 );
    }
    return isValid;
  }


}




namespace Hurricane {


QBrush  getBrush ( const string& pattern, int red, int green, int blue )
{
  if ( pattern != "FFFFFFFFFFFFFFFF" ) {
    uchar  bits[8];
    
    if ( !getPattern(bits,pattern) )
      cerr << "[WARNING] Invalid stipple pattern: \"0x" << pattern << "\"." << endl;
    
    return QBrush ( QColor(red,green,blue), QBitmap::fromData(QSize(8,8),bits,QImage::Format_Mono) );
  }

  return QBrush ( QColor(red,green,blue), Qt::SolidPattern );
}


} // End of Hurricane namespace.
