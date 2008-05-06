#include <iostream>

#include "DBo.h"
#include "Warning.h"
#include "Error.h"
using namespace Hurricane;

#include "AEnv.h"
#include "ATechnology.h"


int main(int argc, char* argv[]) {
    try {
        cout << "simple analogic test" << endl;
        if (argc != 2) {
            cerr << "atest techno.xml";
            exit(56);
        }
        AEnv::create(argv[1]);
        ATechnology* aTechnology = AEnv::getATechnology();
        if (!aTechnology) {
            exit(56);
        }
        aTechnology->print();
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
