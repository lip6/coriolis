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
        cerr << "Cell created" << endl;
    }else{
        cerr << "Cell not created" << endl;
        return;
    }
    system((string("mkdir -p ") + string(pathToTest)).c_str());
/*    cerr << "driving GDS" << endl;
      GdsDriver(dev).save(string(pathToTest) + "/GDSdrive");
*/
    cerr << "driving OA" << endl;
    OADriver(dev).save(string(pathToTest) + "/OAdrive");
}

void testAnalog(char* pathToConf,char* pathToTest){
    AnalogEnv::create(pathToConf);//create Database ...
    cerr << "analog environment loaded and database created" << endl;
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

    cerr << "creating cell myCM" << endl;

    bool bulkConnected = true;//bulk connected to source
    Cell* dev = SimpleCurrentMirror::create(workLibrary, Name("myCM"),
                                            TransistorFamily::NMOS,
                                            bulkConnected);

    cerr << "testing cell myCM" << endl;
    testCell(dev,pathToTest);
    db->destroy();
}

void testNum(char* pathToConf,char* pathToTest,char* cellName){
    cerr << "creating cell from sxlib " << cellName << endl;
    DataBase* db = DataBase::getDB();
    AllianceFramework* af = AllianceFramework::get();
    Cell* cell = NULL;
    cell = af->getCell(cellName, Catalog::State::Views );
    cerr << "testing cell from sxlib "<< cellName << endl;
    testCell(cell,pathToTest);
}

int main(int argc,char** argv) {
    if(argc != 3)
        exit(-5);

    testAnalog(argv[1],argv[2]);

    testNum(argv[1],argv[2],"a2_x2");
    testNum(argv[1],argv[2],"a2_x4");
    testNum(argv[1],argv[2],"a3_x2");
    testNum(argv[1],argv[2],"a3_x4");
    testNum(argv[1],argv[2],"a4_x2");
    testNum(argv[1],argv[2],"a4_x4");
    testNum(argv[1],argv[2],"an12_x1");
    testNum(argv[1],argv[2],"an12_x4");
    testNum(argv[1],argv[2],"ao22_x2");
    testNum(argv[1],argv[2],"ao22_x4");
    testNum(argv[1],argv[2],"ao2o22_x2");
    testNum(argv[1],argv[2],"ao2o22_x4");
    testNum(argv[1],argv[2],"buf_x2");
    testNum(argv[1],argv[2],"buf_x4");
    testNum(argv[1],argv[2],"buf_x8");
    testNum(argv[1],argv[2],"fulladder_x2");
    testNum(argv[1],argv[2],"fulladder_x4");
    testNum(argv[1],argv[2],"halfadder_x2");
    testNum(argv[1],argv[2],"halfadder_x4");
    testNum(argv[1],argv[2],"inv_x1");
    testNum(argv[1],argv[2],"inv_x2");
    testNum(argv[1],argv[2],"inv_x4");
    testNum(argv[1],argv[2],"inv_x8");
    testNum(argv[1],argv[2],"mx2_x2");
    testNum(argv[1],argv[2],"mx2_x4");
    testNum(argv[1],argv[2],"mx3_x2");
    testNum(argv[1],argv[2],"mx3_x4");
    testNum(argv[1],argv[2],"na2_x1");
    testNum(argv[1],argv[2],"na2_x4");
    testNum(argv[1],argv[2],"na3_x1");
    testNum(argv[1],argv[2],"na3_x4");
    testNum(argv[1],argv[2],"na4_x1");
    testNum(argv[1],argv[2],"na4_x4");
    testNum(argv[1],argv[2],"nao22_x1");
    testNum(argv[1],argv[2],"nao22_x4");
    testNum(argv[1],argv[2],"nao2o22_x1");
    testNum(argv[1],argv[2],"nao2o22_x4");
    testNum(argv[1],argv[2],"nmx2_x1");
    testNum(argv[1],argv[2],"nmx2_x4");
    testNum(argv[1],argv[2],"nmx3_x1");
    testNum(argv[1],argv[2],"nmx3_x4");
    testNum(argv[1],argv[2],"no2_x1");
    testNum(argv[1],argv[2],"no2_x4");
    testNum(argv[1],argv[2],"no3_x1");
    testNum(argv[1],argv[2],"no3_x4");
    testNum(argv[1],argv[2],"no4_x1");
    testNum(argv[1],argv[2],"no4_x4");
    testNum(argv[1],argv[2],"noa22_x1");
    testNum(argv[1],argv[2],"noa22_x4");
    testNum(argv[1],argv[2],"noa2a22_x1");
    testNum(argv[1],argv[2],"noa2a22_x4");
    testNum(argv[1],argv[2],"noa2a2a23_x1");
    testNum(argv[1],argv[2],"noa2a2a23_x4");
    testNum(argv[1],argv[2],"noa2a2a2a24_x1");
    testNum(argv[1],argv[2],"noa2a2a2a24_x4");
    testNum(argv[1],argv[2],"noa2ao222_x1");
    testNum(argv[1],argv[2],"noa2ao222_x4");
    testNum(argv[1],argv[2],"noa3ao322_x1");
    testNum(argv[1],argv[2],"noa3ao322_x4");
    testNum(argv[1],argv[2],"nts_x1");
    testNum(argv[1],argv[2],"nts_x2");
    testNum(argv[1],argv[2],"nxr2_x1");
    testNum(argv[1],argv[2],"nxr2_x4");
    testNum(argv[1],argv[2],"o2_x2");
    testNum(argv[1],argv[2],"o2_x4");
    testNum(argv[1],argv[2],"o3_x2");
    testNum(argv[1],argv[2],"o3_x4");
    testNum(argv[1],argv[2],"o4_x2");
    testNum(argv[1],argv[2],"o4_x4");
    testNum(argv[1],argv[2],"oa22_x2");
    testNum(argv[1],argv[2],"oa22_x4");
    testNum(argv[1],argv[2],"oa2a22_x2");
    testNum(argv[1],argv[2],"oa2a22_x4");
    testNum(argv[1],argv[2],"oa2a2a23_x2");
    testNum(argv[1],argv[2],"oa2a2a23_x4");
    testNum(argv[1],argv[2],"oa2a2a2a24_x2");
    testNum(argv[1],argv[2],"oa2a2a2a24_x4");
    testNum(argv[1],argv[2],"oa2ao222_x2");
    testNum(argv[1],argv[2],"oa2ao222_x4");
    testNum(argv[1],argv[2],"oa3ao322_x2");
    testNum(argv[1],argv[2],"oa3ao322_x4");
    testNum(argv[1],argv[2],"on12_x1");
    testNum(argv[1],argv[2],"on12_x4");
    testNum(argv[1],argv[2],"one_x0");
    testNum(argv[1],argv[2],"powmid_x0");
    testNum(argv[1],argv[2],"rowend_x0");
    testNum(argv[1],argv[2],"sff1_x4");
    testNum(argv[1],argv[2],"sff2_x4");
    testNum(argv[1],argv[2],"sff3_x4");
    testNum(argv[1],argv[2],"tie_x0");
    testNum(argv[1],argv[2],"ts_x4");
    testNum(argv[1],argv[2],"ts_x8");
    testNum(argv[1],argv[2],"xr2_x1");
    testNum(argv[1],argv[2],"xr2_x4");
    testNum(argv[1],argv[2],"zero_x0");

    cerr << "ending normally" << endl;
    return 0;
}
