#include<QMenu>
#include<QMenuBar>
#include<QAction>

#include "MainWindow.h"

MainWindow::MainWindow() 
{
    CompilationWidget* cWidget = new CompilationWidget(this);
    setCentralWidget(cWidget);

    QAction* configAct = new QAction(tr("&Configure"), this);
    configAct->setStatusTip(tr("Configure the application"));
    connect(configAct, SIGNAL(triggered()), cWidget, SLOT(reconfig()));
    QAction* aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the about dialog box"));
    connect(aboutAct, SIGNAL(triggered()), cWidget, SLOT(about()));
    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(configAct);
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
} 
