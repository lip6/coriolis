#include <QApplication>
#include "MainWindow.h"

int main ( int argc, char **argv ) {
    QApplication app    ( argc, argv );

    MainWindow mWindow;
    mWindow.show();

    return app.exec();
}
