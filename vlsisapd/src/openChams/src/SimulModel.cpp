/*
 *  SimulModel.cpp
 *  openChams
 *
 *  Created by damien dupuis on 06/07/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

using namespace std;

#include "vlsisapd/openChams/SimulModel.h"

namespace OpenChams {
SimulModel::SimulModel(unsigned id, SimulModel::Base base, SimulModel::Version version, std::string filePath) 
    : _id(id)
    , _base(base)
    , _version(version)
    , _filePath(filePath) {}
    
} // namespace
