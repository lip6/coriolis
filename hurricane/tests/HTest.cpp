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
    if (!rootLibrary) {
        cout << "Error in RootLibrary creation" << endl;
        return 1;
    }

    Library* workLibrary = Library::create(rootLibrary, Name("WorkLibrary"));
    if (!workLibrary) {
        cout << "Error in WorkLibrary creation" << endl;
        return 1;
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

    return 0;
}
