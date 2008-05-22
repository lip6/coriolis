#include <iostream>

#include <QApplication>

#include "hurricane/Warning.h"
#include "hurricane/Error.h"
#include "hurricane/DataBase.h"
#include "hurricane/Library.h"
using namespace Hurricane;

#include "AEnv.h"
#include "ATechnology.h"
#include "Transistor.h"


int main(int argc, char* argv[]) {
    try {

	QApplication* qa = new QApplication(argc, argv);

        cout << "simple analogic test" << endl;
        if (argc != 4) {
            cerr << "atest techno.xml graphic.xml anatechno.xml";
            exit(56);
        }
        AEnv::create(argv[1], argv[2], argv[3]);
        DataBase* db = getDataBase();
        Library* rootLibrary = db->getRootLibrary();
        Library* userLibrary = Library::create(rootLibrary, Name("USER"));

        ATechnology* aTechnology = AEnv::getATechnology();
        if (!aTechnology) {
            exit(56);
        }
        aTechnology->print();
        Transistor* trans = Transistor::create(userLibrary, Name("TEST"), Transistor::P);
        cerr << trans << endl;
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
