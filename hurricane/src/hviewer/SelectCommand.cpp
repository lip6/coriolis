
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
// |  C++ Module  :       "./SelectCommand.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>

#include "hurricane/Cell.h"

#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/SelectCommand.h"
#include "hurricane/viewer/SelectionPopup.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "SelectCommand".


  SelectCommand::SelectCommand ()
    : AreaCommand()
    , _selectionPopup(NULL)
  {
    _selectionPopup = new SelectionPopup ();

    connect ( _selectionPopup, SIGNAL(selectionToggled(Occurrence))
            , this           , SIGNAL(selectionToggled(Occurrence)) );
  }


  SelectCommand::~SelectCommand ()
  {
    delete _selectionPopup;
  }


  bool  SelectCommand::mousePressEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( isActive() ) return true;

    if ( event->button() == Qt::RightButton ) {
      if ( !event->modifiers() ) {
        setActive         ( true );
        setStartPoint     ( event->pos() );
        setDrawingEnabled ( true );
      } else if ( event->modifiers() == Qt::ControlModifier ) {
        QRect selectArea ( event->pos() - QPoint(2,2), QSize(4,4) );
        forEach ( Occurrence, ioccurrence, widget->getOccurrencesUnder(selectArea) )
          _selectionPopup->add ( *ioccurrence );

        _selectionPopup->updateLayout ();
        _selectionPopup->move ( event->globalPos() );
        _selectionPopup->popup ();
      }
    }

    return isActive();
  }


  bool  SelectCommand::mouseReleaseEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( !isActive() )
      _startPoint = _stopPoint = event->pos();

    setActive ( false );
    setDrawingEnabled ( false );

    QRect selectArea;
    if ( _startPoint == _stopPoint )
      selectArea = QRect ( _startPoint - QPoint(2,2), QSize(4,4) );
    else
      selectArea = QRect ( _startPoint, _stopPoint );
      
  //widget->unselectAll ();
    widget->selectOccurrencesUnder ( widget->screenToDbuBox(selectArea) );
    bool somethingSelected = !widget->getSelectorSet().empty();

    if ( widget->getState()->showSelection() != somethingSelected )
      widget->setShowSelection ( somethingSelected );
    else
      widget->refresh ();
    
    return true;
  }



} // End of Hurricane namespace.
