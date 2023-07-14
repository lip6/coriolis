
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The Coriolis Project  is free software;  you can  redistribute it and/or
// modify  it  under the  terms  of  the  GNU  General  Public License  as
// published by  the Free  Software Foundation; either  version 2  of  the
// License, or (at your option) any later version.
// 
// The Coriolis Project  is distributed in the hope that it will be useful,
// but  WITHOUT  ANY  WARRANTY;  without  even  the  implied  warranty  of
// MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
// General Public License for more details.
// 
// You should have received a copy  of  the  GNU  General  Public  License
// along with  the Coriolis Project;  if  not,  write to the  Free Software
// Foundation, inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
// Date   : 29/01/2004
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 
#ifndef __CONTAINER_H
#define __CONTAINER_H

#include "hurricane/Box.h"
#include "hurricane/ExtensionGo.h"

namespace Mauka {

using Hurricane::Record;
using Hurricane::Name;
using Hurricane::DbU;
using Hurricane::Point;
using Hurricane::Box;
using Hurricane::Cell;
using Hurricane::ExtensionGo;

class Container: public ExtensionGo {
// ********************************

// Types
// *****
    public: typedef ExtensionGo Inherit;

// Attributes
// **********
    private: static const Name _goName;
    protected: Box _box;

// Constructors
// ************
    protected: Container(Cell* cell, const Box& box);
// 
// Accessors
// *********

    public: static const Name& getStaticName();
    public: virtual const Name& getName() const;
    public: Box getBoundingBox() const { return _box; };
    public: const Box& getBox() const { return _box; };
    public: const DbU::Unit& getXMin() const { return _box.getXMin(); };
    public: const DbU::Unit& getYMin() const { return _box.getYMin(); };
    public: const DbU::Unit& getXMax() const { return _box.getXMax(); };
    public: const DbU::Unit& getYMax() const { return _box.getYMax(); };
    public: DbU::Unit getHeight() const { return _box.getHeight(); };
    public: DbU::Unit getWidth() const { return _box.getWidth(); };
    public: Point getCenter() const { return _box.getCenter(); }

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy) {}

// Others
// ******
    public: virtual std::string _getTypeName() const {return "Container";};
    public: virtual std::string _getString() const;
    public: virtual Record* _getRecord() const;

};

}

#endif // __CONTAINER_H
