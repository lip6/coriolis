#ifndef __CELL_EDITOR_H
#define __CELL_EDITOR_H

#include "Cell.h"
using namespace Hurricane;

#include <QMainWindow>

class QGraphicsView;
class CellScene;

class CellEditor : public QMainWindow {
    Q_OBJECT

    public:
        CellEditor(Cell* cell);
        ~CellEditor();
    private:
        Cell* cell;
        CellScene *scene;
        QGraphicsView *view;

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
