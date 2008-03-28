#include <iostream>

#include "Library.h"
#include "Warning.h"
using namespace Hurricane;

#include "crlcore/CDataBase.h"
using namespace CRL;

#include "TrMos.h"
using namespace DEVICE;

int main() {
    try {
      cout << "simple analogic test" << endl;
      CDataBase* db = CDataBase::create();
      Library* workLibrary = Library::create(db->getRootLibrary(), Name("WorkLibrary"));
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
