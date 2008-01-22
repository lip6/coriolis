#ifndef __CELL_VIEWER_H
#define __CELL_VIEWER_H

#include "Cell.h"
using namespace H;

#include <QMainWindow>

class CellWidget;
class LayersWidget;

class CellViewer : public QMainWindow {
    Q_OBJECT

    public:
        CellViewer(Cell* cell);
        ~CellViewer();
    private:
        CellWidget* cellWidget;
        LayersWidget* layersWidget;

        QAction *zoomInAct;
        QAction *zoomOutAct;
        QAction *fitToWindowAct;
        QMenu *viewMenu;

        void createActions();
        void createMenus();

    private slots:
        void zoomIn();
        void zoomOut();
        void fitToWindow();
};

#endif
