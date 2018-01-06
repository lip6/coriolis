// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./HierarchyCommand.cpp"                   |
// +-----------------------------------------------------------------+


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


  void  HierarchyCommand::reset ()
  {
    _history.clear();
    _historyIndex = 0;
  }


  void  HierarchyCommand::keyReleaseEvent ( QKeyEvent* event )
  {
    bool  control = (event->modifiers() & Qt::ControlModifier);
    bool  shift   = (event->modifiers() & Qt::ShiftModifier  );

    if (not shift and not control) return;
    if (not _cellWidget->getCell()) return;

    QPoint position ( _cellWidget->mapFromGlobal(QCursor::pos()) );
    Box    pointBox ( _cellWidget->screenToDbuBox(QRect(position,QSize(1,1))) );

    switch ( event->key() ) {
      case Qt::Key_Up:
        if ( (_historyIndex > 0) and (shift or control) ) {
          _cellWidget->setState ( _history[--_historyIndex], CellWidget::NoResetCommands );
        }
        break;
      case Qt::Key_Down:
        {
          if (control) {
            Path topPath;
            if (_history.empty()) {
              _history.push_back( _cellWidget->getState() );
              topPath = _cellWidget->getState()->getTopPath();
            } else if (_historyIndex > 0) {
              topPath = _history[_historyIndex-1]->getTopPath();
            }

            Instances instances = _cellWidget->getCell()->getInstancesUnder( pointBox );
            if (not instances.isEmpty()) {
              _history.erase( _history.begin()+_historyIndex+1, _history.end() );

              topPath = Path( topPath, instances.getFirst() );
              _cellWidget->setCell( topPath.getMasterCell(), topPath, CellWidget::NoResetCommands );
              _history.push_back( _cellWidget->getState() );
              _historyIndex++;
            }
          } else if (shift) {
            if (_historyIndex+1 < _history.size()) {
              _cellWidget->setState( _history[++_historyIndex], CellWidget::NoResetCommands );
            }
          }
        }
        break;
    }
  }



} // End of Hurricane namespace.
