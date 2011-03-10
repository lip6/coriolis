/*
 *  Schematic.h
 *  openChams
 *
 *  Created by damien dupuis on 22/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_SCHEMATIC_H__
#define __OPENCHAMS_SCHEMATIC_H__

#include <map>

#include "vlsisapd/openChams/Name.h"

namespace OpenChams {
class Circuit;

class Schematic {
	public:
    class Infos {
    	public:
        Infos(double x, double y, Name orient);
        
    	inline double getX();
        inline double getY();
        inline Name   getOrientation();
        
	    private:
        double _x;
        double _y;
        Name   _orient;
    };
	public:
    Schematic(Circuit*);
        
    void addInstance(Name instanceName, double x, double y, Name orient );
        
    inline bool   hasNoInstances();
    inline const std::map<Name, Infos*>& getInstances();
        
    private:
    Circuit* _circuit;
    std::map<Name, Infos*> _instances;
};

inline bool   Schematic::hasNoInstances() { return (_instances.size() == 0) ? true : false; };
inline const std::map<Name, Schematic::Infos*>& Schematic::getInstances() { return _instances; };    
    
inline double Schematic::Infos::getX()           { return _x; };
inline double Schematic::Infos::getY()           { return _y; };
inline Name   Schematic::Infos::getOrientation() { return _orient; };
    
} // namespace
#endif

