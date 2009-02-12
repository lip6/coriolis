#include <iostream>
using namespace std;

#include "hurricane/DataBase.h"
#include "hurricane/Library.h"
using namespace Hurricane;

int main() {
    DataBase* db = DataBase::create();
    cout << "Testing DataBase creation" << endl;
    if (!db) {
        cout << "Error in DataBase creation" << endl;
        return 1;
    }

    Library* rootLibrary = Library::create(db, Name("RootLibrary"));
    cout << "Testing RootLibrary creation" << endl;
    if (!rootLibrary) {
        cout << "Error in RootLibrary creation" << endl;
        return 1;
    }
    try {
        cout << "Testing RootLibrary collision" << endl;
        Library* rootLibrary2 = Library::create(db, Name("RootLibrary2"));
        if (rootLibrary2) {
            cout << "Error in RootLibrary creation" << endl;
            return 1;
        }
    } catch (const Hurricane::Error& e) {
        cout << e.what() << endl;
    }

    Library* workLibrary = Library::create(rootLibrary, Name("WorkLibrary"));
    if (!workLibrary) {
        cout << "Error in WorkLibrary creation" << endl;
        return 1;
    }

    try {
        Library* workLibrary2 = Library::create(rootLibrary, Name("WorkLibrary"));
        if (workLibrary2) {
            cout << "Error in WorkLibrary creation" << endl;
            return 1;
        }
    } catch (const Hurricane::Error& e) {
        cout << e.what() << endl;
    }

    workLibrary = rootLibrary->getLibrary(Name("WorkLibrary"));
    if (!workLibrary) {
        cout << "Error in Library->getLibrary(const Name& name)" << endl;
    }

    workLibrary->destroy();
    workLibrary = rootLibrary->getLibrary(Name("WorkLibrary"));
    if (workLibrary) {
        cout << "Error in Library destruction" << endl;
    }

    Library* rootLibrary2 = db->getRootLibrary(); 
    if (!rootLibrary2) {
        cout << "No Root Library in DataBase destruction" << endl;
        return 1;
    }

    rootLibrary2->destroy();
    rootLibrary2 = db->getRootLibrary(); 
    if (rootLibrary2) {
        cout << "Error in Root Library destruction" << endl;
        return 1;
    }

    db->destroy();
    db = DataBase::getDB();
    if (db) {
        cout << "Error in DataBase destruction" << endl;
        return 1;
    }

    /* Netlist creation Test */
    db = DataBase::create();
    rootLibrary = Library::create(db, Name("RootLibrary"));
    workLibrary = Library::create(rootLibrary, Name("WorkLibrary"));

    //Model1 creation
    Cell* model1 = Cell::create(workLibrary, Name("model1")); 
    cout << "Testing model1 presence in Library WorkLibrary" << endl;
    ////

    //Net net1 External and IN of Model1 creation
    Net* net1 = Net::create(model1, Name("net1"));
    cout << "Testing net1 presence in cell model1" << endl;
    /////
    //set net1 as External
    /////
    cout << "Testing net1 Is External" << endl;
    /////
    //set net1 direction as IN
    /////
    cout << "Testing net1 direction is IN" << endl;
    /////

    //Net net2 External and OUT of Model1 creation
    


    //Model2 creation (3 nets IN IN OUT, tous les 3 externes)


    //Design creation
    Cell* design = Cell::create(workLibrary, Name("design")); 

    Instance* ins1 = Instance::create(design, Name("ins1"), model1);
    cout << "Testing ins1 presence in cell design" << endl;

    cout << "Testing ins1 MasterCell is model1" << endl;

    cout << "Testing ins1 as two plugs : net1 and net2" << endl;

    for_each_plug(plug, ins1->getPlugs()) {
        Net* masterNet = plug->getMasterNet();
        if (!masterNet) {
            cout << "Plug with no MasterNet" << endl;
            return 1;
        }
        end_for;
    }

    return 0;
}
