#include <iostream>
using namespace std;

#include <QApplication>
#include <QAction>
#include <QMenu>
#include "MainWindow.h"

int main ( int argc, char **argv ) {
    QApplication app    ( argc, argv );
    
    MainWindow mWindow;
    mWindow.show();

    return app.exec();
}
