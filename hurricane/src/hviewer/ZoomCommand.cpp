
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
// |  C++ Module  :       "./ZoomCommand.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include <QMouseEvent>
# include <QKeyEvent>

# include <hurricane/Warning.h>
# include <hurricane/viewer/CellWidget.h>
# include <hurricane/viewer/ZoomCommand.h>


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "ZoomCommand".


  ZoomCommand::ZoomCommand ()
    : AreaCommand()
  { }


  ZoomCommand::~ZoomCommand ()
  { }


  bool  ZoomCommand::wheelEvent ( CellWidget* widget, QWheelEvent* event )
  {
    if      ( event->delta() > 0 ) widget->setScale ( widget->getScale()/1.2 );
    else if ( event->delta() < 0 ) widget->setScale ( widget->getScale()*1.2 );
    event->accept ();

    return true;
  }


  bool  ZoomCommand::keyPressEvent ( CellWidget* widget, QKeyEvent* event )
  { 
    switch ( event->key() ) {
      case Qt::Key_Z: widget->setScale ( widget->getScale()*2.0 ); return true;
      case Qt::Key_M: widget->setScale ( widget->getScale()/2.0 ); return true;
    }
    return false;
  }


  bool  ZoomCommand::mousePressEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( isActive() ) return true;

    if ( ( event->button() == Qt::LeftButton ) && !event->modifiers() ) {
      setActive         ( true );
      setStartPoint     ( event->pos() );
      setDrawingEnabled ( true );
    }

    return isActive();
  }


  bool  ZoomCommand::mouseReleaseEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( !isActive() ) return false;

    setActive         ( false );
    setDrawingEnabled ( false );

    QRect zoomArea = QRect ( _startPoint, _stopPoint );
    if ( ( zoomArea.width() > 10 ) && ( zoomArea.height() > 10 ) )
      widget->reframe ( widget->screenToDbuBox(zoomArea) );
    else
      cerr << Warning("Rejecting too small zoom request.") << endl;

    return false;
  }


} // End of Hurricane namespace.
