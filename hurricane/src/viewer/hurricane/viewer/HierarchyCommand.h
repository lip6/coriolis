
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
// |  C++ Header  :       "./HierarchyCommand.h"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_HIERARCHY_COMMAND__
#define  __HURRICANE_HIERARCHY_COMMAND__

#include  <set>

#include  <QObject>
#include  <QPoint>

class QAction;

#include  "hurricane/Occurrence.h"
#include  "hurricane/viewer/Command.h"
#include  "hurricane/viewer/CellWidget.h"


using namespace std;


namespace Hurricane {


  class Cell;


  class HierarchyCommand : public Command {
    public:
                                HierarchyCommand  ();
      virtual                  ~HierarchyCommand  ();
      virtual const string&     getName           () const;
      virtual void              keyReleaseEvent   ( QKeyEvent* );
    private:                  
                                HierarchyCommand  ( const HierarchyCommand& );
              HierarchyCommand& operator=         ( const HierarchyCommand& );

    private:
      class HistoryEntry {
        public:
          inline HistoryEntry ( Instance*, shared_ptr<CellWidget::State> );
        public:
          Instance*                      _instance;
          shared_ptr<CellWidget::State>  _state;
      };
    private:
      static string         _name;
      vector<HistoryEntry>  _history;
      size_t                _historyIndex;
  };


// Inline Functions.
  inline HierarchyCommand::HistoryEntry::HistoryEntry  ( Instance*                     instance
                                                       , shared_ptr<CellWidget::State> state
                                                       )
    : _instance(instance)
    , _state   (state)
  { }


} // End of Hurricane namespace.


#endif
