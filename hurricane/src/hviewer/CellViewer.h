
// -*- C++ -*-


# ifndef  __CELLVIEWER_H__
#   define  __CELLVIEWER_H__


using namespace std;

# include  <QMainWindow>

class QKeyEvent;
class QAction;
class QMenu;

# include  "Commons.h"




namespace Hurricane {


  class Cell;
  class LayersList;
//class MapView;
  class CellWidget;


  class CellViewer : public QMainWindow {
      Q_OBJECT;

    protected:
      QAction*    _openAction;
      QAction*    _nextCellAction;
      QAction*    _previousCellAction;
      QAction*    _nextAction;
      QAction*    _saveAction;
      QAction*    _exitAction;
      QAction*    _refreshAction;
      QAction*    _fitToContentsAction;
      QAction*    _showBoundariesAction;
      QMenu*      _fileMenu;
      QMenu*      _viewMenu;
    //MapView*    _mapView;
      LayersList* _layersList;
      CellWidget* _cellWidget;

    public:
            CellViewer    ( Cell* cell );

    protected:
      void  createActions ();
      void  createMenus   ();
      void  createLayout  ( Cell* cell );
  };



} // End of Hurricane namespace.


# endif
