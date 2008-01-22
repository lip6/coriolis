#ifndef __CELL_SCENE_H
#define __CELL_SCENE_H

#include <QGraphicsScene>

#include "Cell.h"
using namespace H;

class CellScene : public QGraphicsScene {
    Q_OBJECT

    public:
        CellScene(Cell* cell);
    private:
        Cell* cell;
};

#endif /* __CELL_SCENE_H */ 
