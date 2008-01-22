#include "CellGraphicsItem.h"

#include "CellScene.h"

CellScene::CellScene(Cell* c)
    : QGraphicsScene()
    , cell(c)
{
    CellGraphicsItem* cellItem = new CellGraphicsItem(cell);
    addItem(cellItem);
}
