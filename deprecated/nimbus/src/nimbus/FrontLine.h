//
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>
//
// Authors-Tag 


#ifndef __NIMBUS_FRONTLINE_H__
#define __NIMBUS_FRONTLINE_H__

#include "hurricane/ExtensionGo.h"
#include "nimbus/Fences.h"


namespace Nimbus {

class Grid;

class FrontLine {


    private:

        vector<Fence*> _rootFences;

    protected:
        Grid* _grid;

        FrontLine ();
        ~FrontLine();

    public:
        FrontLine* create();
        void destroy();
        void _preDestroy();
        void _postCreate();
        
        Grid* getGrid()     { return _grid; };

        void addRootFence (Fence* fence);
        void moveTo (DbU::Unit target);

        DbU::Unit getX()    const { return 0; };
        DbU::Unit getY()    const { return 0; };

#if 0
        virtual DbU::Unit getXMin() const { return getX(); };
        virtual DbU::Unit getXMax() const { return getX(); };
        virtual DbU::Unit getYMin() const { return getY(); };
        virtual DbU::Unit getYMax() const { return getY(); };
        virtual Point getP1()  const { return Point (getX(), getY()); };
        virtual Point getP2()  const { return Point (getX(), getY()); };
        virtual DbU::Unit getXCenter() const { return 0;};
        virtual DbU::Unit getYCenter() const { return 0;};
        virtual Point getCenter() const { return Point (getXCenter(), getYCenter());};
        virtual Box getBoundingBox() const;
#endif
        string _getTypeName() const {return _TName("FrontLine");};
};

}


#endif
