
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
// |  C++ Module  :       "./HierarchyCommand.cpp"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include <QMouseEvent>
# include <QKeyEvent>
# include <QAction>

# include "hurricane/Cell.h"

# include "hurricane/viewer/CellWidget.h"
# include "hurricane/viewer/HierarchyCommand.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "HierarchyCommand".


  HierarchyCommand::HierarchyCommand ()
    : Command ()
    , _history()
    , _historyIndex(0)
  {
  }


  HierarchyCommand::~HierarchyCommand ()
  {
  }


  bool  HierarchyCommand::keyReleaseEvent ( CellWidget* widget, QKeyEvent* event )
  {
    bool  control = (event->modifiers() & Qt::ControlModifier);
    bool  shift   = (event->modifiers() & Qt::ShiftModifier  );

    if ( !shift && !control ) return false;
    if ( !widget->getCell() ) return false;

    QPoint position ( widget->mapFromGlobal(QCursor::pos()) );
    Box    pointBox ( widget->screenToDbuBox(QRect(position,QSize(1,1))) );

    switch ( event->key() ) {
      case Qt::Key_U:
        if ( ( _historyIndex > 0 ) && (shift || control) ) {
          widget->setState ( _history[--_historyIndex]._state );
        }
        break;
      case Qt::Key_D:
        {
          if ( control ) {
            if ( _history.empty() )
              _history.push_back ( HistoryEntry ( NULL, widget->getState() ) );

            Instances instances = widget->getCell()->getInstancesUnder ( pointBox );
            if ( !instances.isEmpty() ) {
              _history.erase ( _history.begin()+_historyIndex+1, _history.end() );

              Instance* instance = instances.getFirst ();
              widget->setCell ( instance->getMasterCell() );
              _history.push_back ( HistoryEntry ( instance, widget->getState() ) );
              _historyIndex++;
            }
          } else if ( shift ) {
            if ( _historyIndex+1 < _history.size() ) {
              widget->setState ( _history[++_historyIndex]._state );
            }
          }
        }
        break;
      default:
        return false;
    }
    return true;
  }



} // End of Hurricane namespace.
