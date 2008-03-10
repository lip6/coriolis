#include <iostream>

#include "Library.h"
#include "Warning.h"
using namespace H;

#include "crlcore/CDataBase.h"
using namespace CRL;

#include "TrMos.h"
using namespace DEVICE;

int main() {
    try {
      cout << "simple analogic test" << endl;
      CDataBase* db = CDataBase::Create();
      Library* workLibrary = Library::Create(db->GetRootLibrary(), Name("WorkLibrary"));
      TrMos* trmos = TrMos::Create(workLibrary, Name("MosTr"));
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
