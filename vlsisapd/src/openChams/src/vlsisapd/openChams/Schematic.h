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
        Infos(double x, double y, Name sym);
        
    	inline double getX();
        inline double getY();
        inline Name   getSymmetry();
        
	    private:
        double _x;
        double _y;
        Name   _sym;
    };
	public:
    Schematic(Circuit*, double);
        
    void addInstance(Name instanceName, double x, double y, Name sym );
        
    inline double getZoom();
    inline bool   hasNoInstances();
    inline const std::map<Name, Infos*>& getInstances();
        
    private:
    Circuit* _circuit;
    double   _zoom;
    std::map<Name, Infos*> _instances;
};

inline double Schematic::getZoom()        { return _zoom; };
inline bool   Schematic::hasNoInstances() { return (_instances.size() == 0) ? true : false; };
inline const std::map<Name, Schematic::Infos*>& Schematic::getInstances() { return _instances; };    
    
inline double Schematic::Infos::getX()        { return _x; };
inline double Schematic::Infos::getY()        { return _y; };
inline Name   Schematic::Infos::getSymmetry() { return _sym; };
    
} // namespace
#endif

