
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
// |  C++ Module  :       "./AreaCommand.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include <QMouseEvent>
# include <QKeyEvent>

# include <hurricane/viewer/CellWidget.h>
# include <hurricane/viewer/AreaCommand.h>


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "AreaCommand".


  AreaCommand::AreaCommand ()
    : Command()
    , _startPoint()
    , _stopPoint()
    , _drawingEnabled(false)
  { }


  AreaCommand::~AreaCommand ()
  { }



  bool  AreaCommand::mouseMoveEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( !_drawingEnabled ) return false;

    setStopPoint ( event->pos() );
    widget->update ();

    return true;
  }


  void  AreaCommand::draw ( CellWidget* widget )
  {
    if ( !_drawingEnabled ) return;

    widget->drawScreenRect ( _startPoint, _stopPoint );
    drawCorner ( widget, true  );
    drawCorner ( widget, false );
  }


  void  AreaCommand::drawCorner ( CellWidget* widget, bool bottomLeft )
  {
    QRect  zoomRect = QRect(_startPoint,_stopPoint).normalized();
    QPoint base     = (bottomLeft) ? zoomRect.bottomLeft() : zoomRect.topRight();

    if ( bottomLeft ) base.rx() += 2;
    else              base.ry() += 2;

    _cornerPoints[0] = base;
    _cornerPoints[1] = base;
    _cornerPoints[2] = base;

    _cornerPoints[0].ry() += (bottomLeft) ? -10 :  10;
    _cornerPoints[2].rx() += (bottomLeft) ?  10 : -10;

    widget->drawScreenPolyline ( _cornerPoints, 3, 4 );
  }


} // End of Hurricane namespace.
