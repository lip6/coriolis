#ifndef  __OA_WRAPPER_H__
#define  __OA_WRAPPER_H__

#include <set>
using namespace std;

#include "oa/oaDesignDB.h"
using namespace oa;

namespace CRL {

class OAWrapper {
    public:
        static OAWrapper* getOAWrapper();
        void loadOALib(const string& libNameStr, const string& libPath, bool asDesignLibrary = false);
        Cell* getCell(const string& cellName);
        void getDesigns(set<Cell*>& designCellSet);

    private:
        OAWrapper();
        ~OAWrapper();
        void openLib(string& libPath);
        void oaTechnology2Technology(oaLib* lib);
        oaLib* openOALib(const oaScalarName& libOAName, const string& libPath);

        typedef map<oaLib*, Library*> OALib2LibMap;
        typedef map<Name, Library*> Name2LibMap;
        typedef map<oaLayerNum, Layer*> OALayerNum2LayerMap;
        typedef map<Cell*, oaCell*> Cell2OACellMap;

        static OAWrapper* _singleton;
        oaTech* _oaTechnology; // only one technology;
        OALayerNum2LayerMap _oaLayerNum2LayerMap;
        OALib2LibMap _oaLib2LibMap;
        Name2LibMap _name2LibMap;
        Cell2OACellMap _cell2OACellMap;
        set<Cell*> _loadedCells;
};

}

#endif /* __OA_WRAPPER_H__ */ 
