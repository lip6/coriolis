#include "hurricane/DataBase.h"
#include "hurricane/Library.h"
#include "hurricane/Technology.h"
using namespace Hurricane;

#include "crlcore/GraphicsParser.h"
#include "crlcore/SymbolicTechnologyParser.h"
using namespace CRL;

#include "ATechnology.h"
#include "ATechnologyXmlParser.h"

#include "AEnv.h"

void AEnv::create(const char* technoFilePath, const char* graphicFilePath, const char* analogTechnoFilePath) {
    DataBase* db = DataBase::getDB();
    if (db) {
        throw Error("");
    }
    db = DataBase::create();
    SymbolicTechnologyParser::load(db, technoFilePath); 
    GraphicsParser::load(graphicFilePath);

    Library* rootLibrary = Library::create(db, Name("RootLibrary"));
    Technology* techno = db->getTechnology();
    ATechnologyXmlParser::parse(analogTechnoFilePath, techno);
}

ATechnology* AEnv::getATechnology() {
    DataBase* db = DataBase::getDB();
    if (!db) {
        throw Error("");
    }
    Technology* technology = db->getTechnology();
    if (!technology) {
        throw Error("");
    }
    ATechnology* atechnology = ATechnology::getATechnology(technology);
    return atechnology;
}
