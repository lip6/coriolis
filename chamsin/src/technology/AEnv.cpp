#include "DataBase.h"
#include "Technology.h"
using namespace Hurricane;

#include "ATechnologyXmlParser.h"

#include "AEnv.h"

void AEnv::create(const char* technoFilePath) {
    DataBase* db = getDataBase();
    if (db) {
    }
    db = DataBase::create();
    Technology* techno = Technology::create(db, Name("AnalogicTechnology"));
}
