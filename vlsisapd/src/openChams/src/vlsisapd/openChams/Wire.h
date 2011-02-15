/*
 *  Wire.h
 *  openChams
 *
 *  Created by damien dupuis on 08/02/11.
 *  Copyright 2011 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_WIRE_H__
#define __OPENCHAMS_WIRE_H__

#include <vector>

#include "vlsisapd/openChams/Name.h"

namespace OpenChams {
class WirePoint {
    public:
    WirePoint() {}
    virtual ~WirePoint() {};
};

class InstancePoint: public WirePoint {
    private:
    Name _name;
    Name _plug;

    public:
    InstancePoint(Name name, Name plug): _name(name), _plug(plug) {}
    virtual ~InstancePoint() {}

    inline Name getName() { return _name; }
    inline Name getPlug() { return _plug; }
};

class PortPoint: public WirePoint {
    private:
    unsigned _idx;

    public:
    PortPoint(unsigned idx): _idx(idx) {}
    virtual ~PortPoint() {}

    inline unsigned getIndex() { return _idx; }
};

class IntermediatePoint: public WirePoint {
    private:
    double _x;
    double _y;

    public:
    IntermediatePoint(double x, double y): _x(x), _y(y) {}
    virtual ~IntermediatePoint() {}

    inline double getX() { return _x; }
    inline double getY() { return _y; }
};

class Wire {
	private:
    WirePoint*  _start;
    WirePoint*  _end;
    std::vector<IntermediatePoint*> _inters;

	public:
    Wire(): _start(NULL), _end(NULL), _inters() {}
    ~Wire() {}
    
    // Accessors
    inline WirePoint* getStartPoint() { return _start; }
    inline WirePoint* getEndPoint()   { return _end; }
    inline       bool hasNoIntermediatePoints() { return (_inters.size() == 0)? true : false; }
    inline const std::vector<IntermediatePoint*>& getIntermediatePoints() { return _inters; }

    // Modifiers
    void setStartPoint(Name name, Name plug);
    void setStartPoint(unsigned idx);
    void setEndPoint(Name name, Name plug);
    void setEndPoint(unsigned idx);
    void addIntermediatePoint(double x, double y);
};

} // namespace
#endif

