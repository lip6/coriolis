#include <iostream>

#include "Warning.h"
#include "Error.h"
using namespace Hurricane;

#include "AEnv.h"


int main() {
    try {
      cout << "simple analogic test" << endl;
      AEnv::create("toto");
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
