// -*-compile-command:"cd .. && make"-*-
// x-----------------------------------------------------------------x
// |  This file is part of                                           |
// |  Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved         |
// | =============================================================== |
// |  Author      :                    Jean-Manuel Caba              |
// |  E-mail      :       Jean-Manuel.Caba@asim.lip6.fr              |
// x-----------------------------------------------------------------x

#include <iostream>
#include <memory>
#include <assert.h>

using namespace std;

#include  <QtGui>
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and not defined (__APPLE__)
#  include  <QGtkStyle>
#endif
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Cell.h"

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/CellViewer.h"
#include  "hurricane/viewer/HApplication.h"

using namespace Hurricane;

#include  "crlcore/AllianceFramework.h"
#include "crlcore/OAParser.h"
using namespace CRL;

int main(int argc,char** argv) {
    int returnCode=0;

    if(argc!=5)
        return -1;

    dbo_ptr<AllianceFramework> af(AllianceFramework::create());
    auto_ptr<DataBase> sys(DataBase::getDB());

//    Cell* cell = af->getCell("inv_x1", Catalog::State::Views );
    Cell* cell = OAParser(argv[1],argv[2],argv[3],argv[4]).open("INV_X1");

    auto_ptr<QApplication> qa(new HApplication(argc,argv));
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and not defined (__APPLE__)
    qa->setStyle(new QGtkStyle());
#endif

    Graphics::enable();
    auto_ptr<CellViewer> blockViewer(new CellViewer());
    blockViewer->setCell(cell);
    blockViewer->show();

    returnCode = qa->exec();

    cerr << "ending normally ?" << endl;
    return returnCode;
}
