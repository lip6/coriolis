#include <iostream>

#include "DataBase.h"
#include "Library.h"
USING_NAMESPACE_HURRICANE

#include "TrMos.h"
using namespace DEVICE;

int main() {
    cout << "simple analogic test" << endl;
    DataBase* db = DataBase::Create();
    Library* rootLibrary = Library::Create(db, Name("RootLibrary"));
    Library* workLibrary = Library::Create(rootLibrary, Name("WorkLibrary"));
    TrMos* trmos = TrMos::Create(workLibrary, Name("MosTr"));
    return 1;
}
