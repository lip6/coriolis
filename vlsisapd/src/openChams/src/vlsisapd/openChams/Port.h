/*
 *  Port.h
 *  openChams
 *
 *  Created by damien dupuis on 08/02/11.
 *  Copyright 2011 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_PORT_H__
#define __OPENCHAMS_PORT_H__

#include "vlsisapd/openChams/Name.h"

namespace OpenChams {
class Port {
	public:
    Port(Name type, unsigned idx , double x, double y, Name sym): _type(type), _idx(idx), _x(x), _y(y), _sym(sym) {}
    ~Port() {}
    
    inline Name     getType()     const;
    inline unsigned getIndex()    const;
    inline double   getX()        const;
    inline double   getY()        const;
    inline Name     getSymmetry() const;

	private:
	Name     _type;
    unsigned _idx;
    double   _x;
    double   _y;
    Name     _sym;
};

inline Name     Port::getType()     const { return _type; }
inline unsigned Port::getIndex()    const { return _idx; }
inline double   Port::getX()        const { return _x; }
inline double   Port::getY()        const { return _y; }
inline Name     Port::getSymmetry() const { return _sym; }
    
} // namespace
#endif

