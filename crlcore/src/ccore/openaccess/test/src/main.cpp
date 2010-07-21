// -*-compile-command:"cd .. && make"-*-
// x-----------------------------------------------------------------x
// |  This file is part of                                           |
// |  Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved         |
// | =============================================================== |
// |  Author      :                    Jean-Manuel Caba              |
// |  E-mail      :       Jean-Manuel.Caba@asim.lip6.fr              |
// x-----------------------------------------------------------------x

#include <iostream>
#include <assert.h>

using namespace std;

#include "hurricane/Cell.h"
#include "hurricane/Library.h"
#include "hurricane/DataBase.h"

using namespace Hurricane;

#include  "crlcore/Catalog.h"
#include  "crlcore/RealTechnologyParser.h"
#include  "crlcore/SymbolicTechnologyParser.h"
#include  "crlcore/AllianceFramework.h"

using namespace CRL;

#include "hurricaneAMS/environment/AnalogEnv.h"
#include "hurricaneAMS/devices/SimpleCurrentMirror.h"

#include "crlcore/GdsDriver.h"
#include "crlcore/OADriver.h"

namespace {
    static string designName = "design";
    static string libName = "WorkLibrary";
};

void testCell(Cell* dev,char* pathToTest){
    if(dev){
        cout << "Cell created" << endl;
    }else{
        cout << "Cell not created" << endl;
        return;
    }
    system((string("mkdir -p ") + string(pathToTest)).c_str());
    cout << "driving GDS" << endl;
    GdsDriver(dev).save(string(pathToTest) + "/GDSdrive");

    cout << "driving OA" << endl;
    OADriver(dev).save(string(pathToTest) + "/OAdrive");
}

void testAnalog(char* pathToConf,char* pathToTest){
    AnalogEnv::create(pathToConf);//create Database ...
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
    testCell(dev,pathToTest);
    db->destroy();
}

void testNum(char* pathToConf,char* pathToTest){
    cout << "creating cell from sxlib inv_x1" << endl;
    dbo_ptr<DataBase>          db   ( DataBase::create() );    
    dbo_ptr<AllianceFramework> af   ( AllianceFramework::create() );    
    Cell*                      cell = NULL;
    cell = af->getCell("inv_x1", Catalog::State::Views );
    cout << "testing cell from sxlib inv_x1" << endl;
    testCell(cell,pathToTest);
}

int main(int argc,char** argv) {
    if(argc != 3)
        exit(-5);

    //   testAnalog(argv[1],argv[2]);
    testNum(argv[1],argv[2]);

    cout << "ending normally" << endl;
    return 0;
}
