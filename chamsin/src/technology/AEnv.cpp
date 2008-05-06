#include "DataBase.h"
#include "Technology.h"
using namespace Hurricane;

#include "ATechnology.h"
#include "ATechnologyXmlParser.h"

#include "AEnv.h"

void AEnv::create(const char* technoFilePath) {
    DataBase* db = getDataBase();
    if (db) {
        throw Error("");
    }
    db = DataBase::create();
    Technology* techno = Technology::create(db, Name("AnalogicTechnology"));
    ATechnologyXmlParser::parse(technoFilePath, techno);
}

ATechnology* AEnv::getATechnology() {
    DataBase* db = getDataBase();
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
