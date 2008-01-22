#include <QtGui>

#include "CellWidget.h"
#include "LayersWidget.h"

#include "CellViewer.h"

CellViewer::CellViewer(Cell* cell)
    : QMainWindow()
{
    cellWidget = new CellWidget(cell);
    setCentralWidget(cellWidget);

    createActions();
    createMenus();

    setWindowTitle(tr("Cell Viewer"));
    resize(1000,  500);
}

CellViewer::~CellViewer() {
    delete cellWidget;
    delete layersWidget;
}

//void CellViewer::keyPressEvent(QKeyEvent *event) {
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
//void CellViewer::wheelEvent(QWheelEvent *event) {
//    int numDegrees = event->delta() / 8;
//    double numSteps = numDegrees / 15.0f;
//    zoom(pow(ZoomInFactor, numSteps));
//}

void CellViewer::zoomIn() {
    cellWidget->reframe(cellWidget->getScale() * 1.2);
    cellWidget->update();
}

void CellViewer::zoomOut() {
    cellWidget->reframe(cellWidget->getScale() / 1.2);
    cellWidget->update();
}

void CellViewer::fitToWindow() {
    cellWidget->fitToContent();
    cellWidget->update();
}

void CellViewer::createActions() {
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

void CellViewer::createMenus() {
    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(fitToWindowAct);

    menuBar()->addMenu(viewMenu);
}
