#ifndef __UTILS_H
#define __UTILS_H

#include <QRectF>
#include <QTransform>
#include "Box.h"
#include "Transformation.h"
using namespace H;

inline void BoxToRectangle(const Box& box, QRectF& rec) {
	double xmin = GetValue(box.getXMin());
	double xmax = GetValue(box.getXMax());
	double ymin = GetValue(box.getYMin());
	double ymax = GetValue(box.getYMax());
	rec.setCoords(xmin, ymin, xmax, ymax);
}

inline void HurricanePositionToQtPosition(const Transformation& transformation, QTransform& transform, QPoint& position) {
	double tx = GetValue(transformation.GetTx());
	double ty = GetValue(transformation.GetTy());

	position.setX((int)tx);
	position.setY((int)ty);

	switch (transformation.GetOrientation()) {
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
