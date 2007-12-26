#ifndef __INSTANCE_FIGURE_H
#define __INSTANCE_FIGURE_H

#include <QGraphicsItem>
#include <QObject>

#include "CellFigure.h"

#include "Instance.h"
using namespace Hurricane;

class InstanceFigure : public QGraphicsItem {
    public:
        InstanceFigure(CellFigure* parent, Instance* instance);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

    private:
        Instance* instance;

};

#endif /* __INSTANCE_FIGURE_H */
