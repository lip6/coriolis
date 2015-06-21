
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
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


  string HierarchyCommand::_name = "HierarchyCommand";


  HierarchyCommand::HierarchyCommand ()
    : Command      ()
    , _history     ()
    , _historyIndex(0)
  { }


  HierarchyCommand::~HierarchyCommand ()
  { }


  const string& HierarchyCommand::getName () const
  { return _name; }


  void  HierarchyCommand::keyReleaseEvent ( QKeyEvent* event )
  {
    bool  control = (event->modifiers() & Qt::ControlModifier);
    bool  shift   = (event->modifiers() & Qt::ShiftModifier  );

    if ( !shift && !control ) return;
    if ( !_cellWidget->getCell() ) return;

    QPoint position ( _cellWidget->mapFromGlobal(QCursor::pos()) );
    Box    pointBox ( _cellWidget->screenToDbuBox(QRect(position,QSize(1,1))) );

    switch ( event->key() ) {
      case Qt::Key_Up:
        if ( ( _historyIndex > 0 ) && (shift || control) ) {
          _cellWidget->setState ( _history[--_historyIndex]._state );
        }
        break;
      case Qt::Key_Down:
        {
          if ( control ) {
            if ( _history.empty() )
              _history.push_back ( HistoryEntry ( NULL, _cellWidget->getState() ) );

            Instances instances = _cellWidget->getCell()->getInstancesUnder ( pointBox );
            if ( !instances.isEmpty() ) {
              _history.erase ( _history.begin()+_historyIndex+1, _history.end() );

              Instance* instance = instances.getFirst ();
              _cellWidget->setCell ( instance->getMasterCell() );
              _history.push_back ( HistoryEntry ( instance, _cellWidget->getState() ) );
              _historyIndex++;
            }
          } else if ( shift ) {
            if ( _historyIndex+1 < _history.size() ) {
              _cellWidget->setState ( _history[++_historyIndex]._state );
            }
          }
        }
        break;
    }
  }



} // End of Hurricane namespace.
