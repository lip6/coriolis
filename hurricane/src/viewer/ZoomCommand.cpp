
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ZoomCommand.cpp"                        |
// +-----------------------------------------------------------------+


# include <QMouseEvent>
# include <QKeyEvent>

# include <hurricane/Warning.h>
# include <hurricane/viewer/CellWidget.h>
# include <hurricane/viewer/ZoomCommand.h>


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "ZoomCommand".


  string ZoomCommand::_name = "ZoomCommand";


  ZoomCommand::ZoomCommand ()
    : AreaCommand()
  { }


  ZoomCommand::~ZoomCommand ()
  { }


  const string& ZoomCommand::getName () const
  { return _name; }


  Command::Type  ZoomCommand::getType () const
  { return AlwaysActive; }


  void  ZoomCommand::wheelEvent ( QWheelEvent* event )
  {
    if      (event->delta() > 0) _cellWidget->setScale ( _cellWidget->getScale()/1.2 );
    else if (event->delta() < 0) _cellWidget->setScale ( _cellWidget->getScale()*1.2 );
    event->accept ();
  }


  void  ZoomCommand::keyPressEvent ( QKeyEvent* event )
  { 
    bool  control = (event->modifiers() & Qt::ControlModifier);

    switch ( event->key() ) {
      case Qt::Key_Z:
        if ( control ) _cellWidget->scaleHistoryDown();
        else           _cellWidget->setScale ( _cellWidget->getScale()*2.0 );
        return;
      case Qt::Key_M:
        if ( control ) _cellWidget->scaleHistoryUp ();
        else           _cellWidget->setScale ( _cellWidget->getScale()/2.0 );
        return;
    }
  }


  void  ZoomCommand::mousePressEvent ( QMouseEvent* event )
  {
    if (isActive()) return;
    if ( _cellWidget->getActiveCommand() and (_cellWidget->getActiveCommand() != this) )
      return;

    if ( (event->button() == Qt::LeftButton) and not event->modifiers() ) {
      setActive        ( true );
      setStartPoint    ( event->pos() );
      setDrawingEnabled( true );
    }
  }


  void  ZoomCommand::mouseReleaseEvent ( QMouseEvent* event )
  {
    if (not isActive()) return;

    setActive        ( false );
    setDrawingEnabled( false );

    QRect zoomArea = QRect( _startPoint, _stopPoint );
    if (   ( abs(zoomArea.width ()) > getDrawingThreshold() )
       and ( abs(zoomArea.height()) > getDrawingThreshold() ) ) {
      _cellWidget->reframe( _cellWidget->screenToDbuBox(zoomArea) ); 
      return;
    }
  //else {
  //  cerr << Warning("Rejecting too small zoom request.") << endl;
  //  _cellWidget->update ();
  //}
  }


} // End of Hurricane namespace.
