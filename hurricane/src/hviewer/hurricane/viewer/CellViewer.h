
// -*- C++ -*-


# ifndef  __CELL_VIEWER_H__
#   define  __CELL_VIEWER_H__


using namespace std;

# include  <QMainWindow>

class QKeyEvent;
class QAction;
class QMenu;

# include  "hurricane/Commons.h"




namespace Hurricane {


  class Cell;
  class Palette;
//class MapView;
  class CellWidget;


  class CellViewer : public QMainWindow {
      Q_OBJECT;

    public:
                  CellViewer             ( QWidget* parent=NULL );
      void        setCell                ( Cell* cell );
    public slots:
      void        runInspectorOnDataBase ();
      void        runInspectorOnCell     ();

    protected:
      QAction*    _openAction;
      QAction*    _nextCellAction;
      QAction*    _previousCellAction;
      QAction*    _nextAction;
      QAction*    _saveAction;
      QAction*    _exitAction;
      QAction*    _refreshAction;
      QAction*    _fitToContentsAction;
      QAction*    _runInspectorOnDataBase;
      QAction*    _runInspectorOnCell;
      QMenu*      _fileMenu;
      QMenu*      _viewMenu;
      QMenu*      _toolsMenu;
    //MapView*    _mapView;
      Palette*    _palette;
      CellWidget* _cellWidget;

    protected:
      void        createActions          ();
      void        createMenus            ();
      void        createLayout           ();
      void        runInspector           ( Record* record );
  };


} // End of Hurricane namespace.


# endif
