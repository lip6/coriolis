#ifndef __INSTANCE_FIGURE_H
#define __INSTANCE_FIGURE_H

#include <QGraphicsItem>
#include <QObject>

#include "Instance.h"
#include "Cell.h"
using namespace Hurricane;

class InstanceFigure : public QGraphicsItem {
    public:
        InstanceFigure(InstanceFigure* parent, Instance* instance);
        //InstanceFigure(Instance* instance);
        InstanceFigure(Cell* cell);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

    private:
        Instance* instance;
        Cell* cell;
	void drawPhantom(QPainter* painter);
	void drawBoundary(QPainter* painter);
	void drawElements(QPainter* painter);
        void constructSubInstances();

};

#endif /* __INSTANCE_FIGURE_H */
