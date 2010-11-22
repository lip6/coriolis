/*
 *  Device.h
 *  openChams
 *
 *  Created by damien dupuis on 22/11/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_DEVICE_H__
#define __OPENCHAMS_DEVICE_H__

#include <map>
#include <vector>

#include "vlsisapd/openChams/Name.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Parameters.h"

namespace OpenChams {
class Netlist;
class Transistor;
class Device : public Instance {
	public:
    Device(Name name, Name model, Name mosType, bool, Netlist*);

    virtual ~Device() {};
    
    inline Name getMosType();
    inline bool isSourceBulkConnected();
    // pour parcourir les transistors
    inline bool hasNoTransistors();
    inline const std::vector<Transistor*>& getTransistors();

    Transistor* addTransistor(Name);

	private:
    Name 	   		_mosType;
    bool            _sourceBulkConnected;
    std::vector<Transistor*> _trans;
};

inline       Name                      Device::getMosType()            { return _mosType; };
inline       bool                      Device::isSourceBulkConnected() { return _sourceBulkConnected; };
inline       bool                      Device::hasNoTransistors()      { return (_trans.size() == 0)? true : false; };
inline const std::vector<Transistor*>& Device::getTransistors()        { return _trans; };
    
} // namespace
#endif


