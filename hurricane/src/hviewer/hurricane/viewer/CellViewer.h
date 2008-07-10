
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
// |  C++ Header  :       "./CellViewer.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>


#ifndef  __CELL_VIEWER_H__
#define  __CELL_VIEWER_H__


using namespace std;

#include  <QMainWindow>

class QEvent;
class QKeyEvent;
class QAction;
class QMenu;

#include  "hurricane/Commons.h"
#include  "hurricane/Occurrence.h"




namespace Hurricane {


  class Cell;
  class HPalette;
//class MapView;
  class CellWidget;
  class HMousePosition;


  class CellViewer : public QMainWindow {
      Q_OBJECT;

    public:
                              CellViewer             ( QWidget* parent=NULL );
              void            setCell                ( Cell* cell );
      inline  CellWidget*     getCellWidget          ();
              void            select                 ( Occurrence& occurence );
              void            unselect               ( Occurrence& occurence );
              void            unselectAll            ();
    public slots:
              void            runInspectorOnDataBase ();
              void            runInspectorOnCell     ();
              void            browseNetlist          ();

    protected:
              QAction*        _openAction;
              QAction*        _nextCellAction;
              QAction*        _previousCellAction;
              QAction*        _nextAction;
              QAction*        _saveAction;
              QAction*        _exitAction;
              QAction*        _refreshAction;
              QAction*        _fitToContentsAction;
              QAction*        _showSelectionAction;
              QAction*        _runInspectorOnDataBase;
              QAction*        _runInspectorOnCell;
              QAction*        _browseNetlist;
              QMenu*          _fileMenu;
              QMenu*          _viewMenu;
              QMenu*          _toolsMenu;
            //MapView*        _mapView;
              HPalette*       _palette;
              HMousePosition* _mousePosition;
              CellWidget*     _cellWidget;

    protected:
              void            createActions          ();
              void            createMenus            ();
              void            createLayout           ();
              void            runInspector           ( Record* record );
  };


// Inline Functions.
  inline CellWidget* CellViewer::getCellWidget ()
  { return _cellWidget; }




} // End of Hurricane namespace.


#endif
