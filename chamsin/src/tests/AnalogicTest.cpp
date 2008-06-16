#include <iostream>

#include <QApplication>

#include "hurricane/Warning.h"
#include "hurricane/Error.h"
#include "hurricane/DataBase.h"
#include "hurricane/Library.h"
#include "hurricane/viewer/CellViewer.h"

using namespace Hurricane;

#include "AEnv.h"
#include "ATechnology.h"
#include "Transistor.h"


int main(int argc, char* argv[]) {
    int  returnCode;
    try {

	QApplication* qa = new QApplication(argc, argv);

        cout << "simple analogic test" << endl;
        if (argc != 4) {
            cerr << "atest techno.xml graphic.xml anatechno.xml";
            exit(56);
        }
        AEnv::create(argv[1], argv[2], argv[3]);
        DataBase* db = DataBase::getDB();
        Library* rootLibrary = db->getRootLibrary();
        Library* userLibrary = Library::create(rootLibrary, Name("USER"));

        ATechnology* aTechnology = AEnv::getATechnology();
        if (!aTechnology) {
            exit(56);
        }
        aTechnology->print();
        Transistor* trans = Transistor::create(userLibrary, Name("TEST"), Transistor::P, 10, 10);
        trans->createLayout();
        CellViewer* viewer = new CellViewer ( trans );
        viewer->show();

        returnCode = qa->exec();
        delete viewer;
        delete qa;

    } catch (Hurricane::Warning& w) {
        cerr << w.what() << endl;
    } catch (Hurricane::Error& e) {
          cerr << e.what() << endl;
          exit (1);
    } catch (...) {
        cout << "Abnormal termination\n" << endl;
        exit(2);
    }
    return returnCode;
}
