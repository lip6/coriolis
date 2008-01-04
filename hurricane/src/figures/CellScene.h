#ifndef __CELL_SCENE_H
#define __CELL_SCENE_H

#include <QGraphicsScene>

class CellScene : public QGraphicsScene {
    protected :
        void drawItems(QPainter *painter,
                int numItems,
                QGraphicsItem *items[],
                const QStyleOptionGraphicsItem options[],
                QWidget *widget);
};

#endif /* __CELL_SCENE_H */ 
