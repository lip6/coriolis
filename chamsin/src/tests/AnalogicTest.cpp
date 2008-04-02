#include <iostream>

#include "DataBase.h"
#include "Library.h"
#include "Warning.h"
using namespace Hurricane;

#include "TrMos.h"
using namespace DEVICE;

int main() {
    try {
      cout << "simple analogic test" << endl;
      DataBase* db = DataBase::create();
      Technology* technology = Technology::create(db);
      Library* rootLibrary = Library::create(db, Name("RootLibrary"));
      Library* workLibrary = Library::create(rootLibrary, Name("WorkLibrary"));
      TrMos* trmos = TrMos::create(workLibrary, Name("MosTr"));
      exit(0);
    } catch (Hurricane::Warning& w) {
        cerr << w.What() << endl;
    } catch (Hurricane::Error& e) {
          cerr << e.What() << endl;
          exit (1);
    } catch (...) {
        cout << "Abnormal termination\n" << endl;
        exit(2);
    }
}
