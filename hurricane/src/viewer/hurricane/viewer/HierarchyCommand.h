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
// |  C++ Header  :       "./HierarchyCommand.h"                     |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_HIERARCHY_COMMAND_H
#define  HURRICANE_HIERARCHY_COMMAND_H

#include <set>
#include <QObject>
#include <QPoint>
class QAction;

#include "hurricane/Occurrence.h"
#include "hurricane/viewer/Command.h"
#include "hurricane/viewer/CellWidget.h"


namespace Hurricane {

  class Cell;


  class HierarchyCommand : public Command {
    public:
                                  HierarchyCommand  ();
      virtual                    ~HierarchyCommand  ();
      virtual const std::string&  getName           () const;
      virtual void                reset             ();
      virtual void                keyReleaseEvent   ( QKeyEvent* );
    private:                  
                                  HierarchyCommand  ( const HierarchyCommand& );
              HierarchyCommand&   operator=         ( const HierarchyCommand& );

    private:
      static std::string                                 _name;
      std::vector< std::shared_ptr<CellWidget::State> >  _history;
      size_t                                             _historyIndex;
  };


} // Hurricane namespace.

#endif  // HURRICANE_HIERARCHY_COMMAND_H
