#include <QGridLayout>
#include <QSlider>
#include <QDockWidget>
#include <QComboBox>

#include "Capacitor.h"
#include "hurricane/DataBase.h"
#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/HInspectorWidget.h"
#include "hurricane/viewer/HPalette.h"
#include "AEnv.h"
#include "ATechnology.h"

#include "CapacitorViewer.h"


CapacitorViewer::CapacitorViewer(Library* library) {

    ATechnology* aTechnology = AEnv::getATechnology();
    if (!aTechnology) {
        exit(56);
    }

    DbU::Unit capacitorMinL = aTechnology->getPhysicalRule("topPlateMIMminWidth")->getValue();
    DbU::Unit capacitorMaxL = 10 * capacitorMinL;
    DbU::Unit capacitorMinW = aTechnology->getPhysicalRule("topPlateMIMminWidth")->getValue();
    DbU::Unit capacitorMaxW = 10 * capacitorMinW;

    _capacitor = Capacitor::create(library, Name("TEST"));
    _capacitor->setL(capacitorMinL);
    _capacitor->setW(capacitorMinW);
    _cellWidget = new CellWidget;
    _palette    = new HPalette ();
    _cellWidget->bindToPalette ( _palette );
    _cellWidget->setCell(_capacitor);
    _cellWidget->fitToContents();
    setCentralWidget(_cellWidget);
    QSlider* wSlider = new QSlider(Qt::Horizontal);
    wSlider->setRange(capacitorMinW, 4 * capacitorMinW);
    wSlider->setPageStep(DbU::grid(1));
    wSlider->setSliderPosition(capacitorMinW);
    QSlider* lSlider = new QSlider(Qt::Horizontal);
    lSlider->setRange(capacitorMinL, 4 * capacitorMinL);
    lSlider->setPageStep(DbU::grid(1));
    lSlider->setSliderPosition(capacitorMinL);

    QWidget* slidersWidget = new QWidget;
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(wSlider, 0, 0);
    layout->addWidget(lSlider, 1, 0);
    slidersWidget->setLayout(layout);

    QDockWidget *dockWidget = new QDockWidget(tr("Dock Widget"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
            Qt::RightDockWidgetArea);
    dockWidget->setWidget(slidersWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);


    QDockWidget* layerMapDock = new QDockWidget ( tr("Layers") );
    layerMapDock->setFeatures     ( QDockWidget::DockWidgetVerticalTitleBar
                                  | QDockWidget::DockWidgetMovable
                                  | QDockWidget::DockWidgetFloatable
                                  );
    layerMapDock->setObjectName   ( "HPalette" );
    layerMapDock->setWidget       ( _palette );
    layerMapDock->setAllowedAreas ( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    addDockWidget ( Qt::RightDockWidgetArea, layerMapDock );

    _runInspectorOnDataBase= new QAction ( tr("Inspect &DataBase"), this );
    _runInspectorOnDataBase->setStatusTip ( tr("Run Inspector on Hurricane DataBase") );

    _runInspectorOnCell= new QAction ( tr("Inspect &Cell"), this );
    _runInspectorOnCell->setStatusTip ( tr("Run Inspector on the current Cell") );

    connect(wSlider, SIGNAL(valueChanged(int)), this, SLOT(wvalueChanged(int)));
    connect(lSlider, SIGNAL(valueChanged(int)), this, SLOT(lvalueChanged(int)));
    connect(_runInspectorOnDataBase, SIGNAL(triggered()) , this, SLOT(runInspectorOnDataBase()));
    connect(_runInspectorOnCell    , SIGNAL(triggered()) , this, SLOT(runInspectorOnCell    ()));

    QMenu* toolsMenu = menuBar()->addMenu(tr("Tool"));
    toolsMenu->addAction(_runInspectorOnDataBase);
    toolsMenu->addAction(_runInspectorOnCell);
}

void CapacitorViewer::wvalueChanged(int value) {
    _capacitor->setW(value);
    _cellWidget->redraw();
}

void CapacitorViewer::lvalueChanged(int value) {
    _capacitor->setL(value);
    _cellWidget->redraw();
}

void CapacitorViewer::runInspectorOnDataBase() {
    runInspector(getRecord(DataBase::getDB()));
}

void CapacitorViewer::runInspectorOnCell() {
    Cell* cell = _cellWidget->getCell();
    if (cell) {
        runInspector(getRecord(cell));
    }
}

void CapacitorViewer::runInspector(Record* record) {
    static HInspectorWidget* inspector = NULL;

    if (record) {
        inspector = new HInspectorWidget ();

        inspector->setRootRecord ( record );
        inspector->show ();
    } else {
        cerr << "[ERROR] Attempt to run Inspector on NULL record." << endl;
    }
}
