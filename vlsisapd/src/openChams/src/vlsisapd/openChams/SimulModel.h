/*
 *  SimulModel.h
 *  openChams
 *
 *  Created by damien dupuis on 06/07/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_SIMULMODEL_H__
#define __OPENCHAMS_SIMULMODEL_H__

#include "vlsisapd/openChams/Name.h"

namespace OpenChams {
class SimulModel {
    public:
    enum Base    { BSIM3V3=0,
                   BSIM4=1,
                   PSP=2
                 };
    enum Version { UNDEFINED=0,
                   SVT=1,
                   HVT=2,
                   LVT=3
                 };

    SimulModel(unsigned, Base, Version, std::string);
    
    inline unsigned    getId() const;
    inline Base        getBase();
    inline Version     getVersion();
    inline std::string getFilePath();

    private:
    unsigned    _id;
    Base        _base;
    Version     _version;
    std::string _filePath;
};
    
inline unsigned            SimulModel::getId() const { return _id;      };
inline SimulModel::Base    SimulModel::getBase()     { return _base;    };
inline SimulModel::Version SimulModel::getVersion()  { return _version; };
inline std::string         SimulModel::getFilePath() { return _filePath;};

} // namespace
#endif

