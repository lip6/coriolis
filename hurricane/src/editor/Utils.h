#ifndef __UTILS_H
#define __UTILS_H

#include <QRectF>
#include <QTransform>
#include "Box.h"
#include "Transformation.h"
using namespace H;

inline void boxToRectangle(const Box& box, QRectF& rec) {
    double xmin = getValue(box.getXMin());
    double xmax = getValue(box.getXMax());
    double ymin = getValue(box.getYMin());
    double ymax = getValue(box.getYMax());
    rec.setCoords(xmin, ymin, xmax, ymax);
}

inline void hurricanePositionToQtPosition(const Transformation& transformation, QTransform& transform, QPoint& position) {
    double tx = getValue(transformation.getTx());
    double ty = getValue(transformation.getTy());

    position.setX((int)tx);
    position.setY((int)ty);

    switch (transformation.getOrientation()) {
    case Transformation::Orientation::ID:
        transform.setMatrix(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
        break;
    case Transformation::Orientation::MX:
        transform.setMatrix(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
        break;
    case Transformation::Orientation::MY:
        transform.setMatrix(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
        break;
    case Transformation::Orientation::R2:
        transform.setMatrix(-1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
        break;
    default:
        break;
    }
}


#endif /* __UTILS_H */
