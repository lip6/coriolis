#include <QtGui>

#include "Utils.h"
#include "CellScene.h"

#include "CellEditor.h"

CellEditor::CellEditor(Cell* c)
    : QMainWindow(),
    cell(c)
{

    createActions();
    createMenus();

    scene = new CellScene(cell);
    view = new QGraphicsView(scene);

    setCentralWidget(view);

    setWindowTitle(tr("Cell Editor"));
    resize(1000,  500);
    fitToWindow();
}

CellEditor::~CellEditor() {
    delete view;
    delete scene;
}

//void CellEditor::keyPressEvent(QKeyEvent *event) {
//    switch (event->key()) {
//        case Qt::Key_Plus:
//            zoom(ZoomInFactor);
//            break;
//        case Qt::Key_Minus:
//            zoom(ZoomOutFactor);
//            break;
//        case Qt::Key_Left:
//            scroll(-ScrollStep, 0);
//            break;
//        case Qt::Key_Right:
//            scroll(+ScrollStep, 0);
//            break;
//        case Qt::Key_Down:
//            scroll(0, -ScrollStep);
//            break;
//        case Qt::Key_Up:
//            scroll(0, +ScrollStep);
//            break;
//        default:
//            QWidget::keyPressEvent(event);
//    }
//}
//
//void CellEditor::wheelEvent(QWheelEvent *event) {
//    int numDegrees = event->delta() / 8;
//    double numSteps = numDegrees / 15.0f;
//    zoom(pow(ZoomInFactor, numSteps));
//}

void CellEditor::zoomIn() {
}

void CellEditor::zoomOut() {
}

void CellEditor::fitToWindow() {
    if (cell) {
	Box area(cell->GetBoundingBox());
        QRectF rect;
        boxToRectangle(area, rect);
        view->fitInView(rect);
    }
}

void CellEditor::createActions() {
    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(true);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(true);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    fitToWindowAct = new QAction(tr("Fit &To &Window"), this);
    fitToWindowAct->setShortcut(tr("f"));
    fitToWindowAct->setEnabled(true);
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));
}

void CellEditor::createMenus() {
    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(fitToWindowAct);

    menuBar()->addMenu(viewMenu);
}
