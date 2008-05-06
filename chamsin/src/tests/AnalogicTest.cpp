#include <iostream>

#include "Warning.h"
using namespace Hurricane;

#include "AEnv.h"

#include "TrMos.h"
using namespace DEVICE;

int main() {
    try {
      cout << "simple analogic test" << endl;
      //CDataBase* db = CDataBase::create();
      //Library* workLibrary = Library::create(db->getRootLibrary(), Name("WorkLibrary"));
      //TrMos* trmos = TrMos::create(workLibrary, Name("MosTr"));
      AEnv::create("toto");
      trmos->create(Transistor::Polarity::P, true); 

      trmos->setMosLength(3);
      trmos->setMosWidth(3);

      trmos->generate(5, true, false, 3, 3); 

      exit(0);
    } catch (Hurricane::Warning& w) {
        cerr << w.what() << endl;
    } catch (Hurricane::Error& e) {
          cerr << e.what() << endl;
          exit (1);
    } catch (...) {
        cout << "Abnormal termination\n" << endl;
        exit(2);
    }
}
