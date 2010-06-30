// -*-compile-command:"cd .. && make"-*-
// x-----------------------------------------------------------------x
// |  This file is part of                                           |
// |  Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved         |
// | =============================================================== |
// |  Author      :                    Jean-Manuel Caba              |
// |  E-mail      :       Jean-Manuel.Caba@asim.lip6.fr              |
// x-----------------------------------------------------------------x

#include <iostream>

using namespace std;

#include "hurricane/Cell.h"
#include "hurricane/Library.h"
#include "hurricane/DataBase.h"

using namespace Hurricane;

#include  "crlcore/Catalog.h"
#include  "crlcore/AllianceFramework.h"

#include "hurricaneAMS/environment/AnalogEnv.h"
#include "hurricaneAMS/devices/SimpleCurrentMirror.h"

#include "crlcore/GdsDriver.h"
#include "crlcore/OADriver.h"

namespace {
    static string designName = "design";
    static string libName = "WorkLibrary";
};

void testCell(Cell* dev){
    if(dev)
        cout << "Cell created" << endl;

    cout << "driving GDS" << endl;
    CRL::GdsDriver(dev).save("/tmp/testGDS");

    cout << "driving OA" << endl;
    CRL::OADriver(dev).save("/tmp/testOA");
}


int main(int argc,char** argv) {
    if(argc != 2)
        exit(-5);

    AnalogEnv::create(argv[1]);//create Database ...
    cout << "analog environment loaded and database created" << endl;
    DataBase* db = DataBase::getDB();
    assert(db != NULL);
    Library* rootLib = db->getRootLibrary();
    if(!rootLib){
        Library::create(db, "RootLibrary");
        rootLib = db->getRootLibrary();
    }
    assert(rootLib != NULL);

    Library* workLibrary = rootLib->getLibrary(libName);
    if(!workLibrary)
        workLibrary = Library::create(rootLib, libName);
    assert(workLibrary != NULL);

    Cell* design = workLibrary->getCell(designName);
    if(!design)
        design = Cell::create(workLibrary, designName);
    assert(design != NULL);

    cout << "creating cell myCM" << endl;

    bool bulkConnected = true;//bulk connected to source
    Cell* dev = SimpleCurrentMirror::create(workLibrary, Name("myCM"),
                                            TransistorFamily::NMOS,
                                            bulkConnected);

    cout << "testing cell myCM" << endl;

    testCell(dev);

//    cout << "creating and testing cell from sxlib inv_x1" << endl;

//    testCell(CRL::AllianceFramework::get()->getCell( "inv_x1", CRL::Catalog::State::Views ));

    cout << "ending normally" << endl;
    return 0;
}
