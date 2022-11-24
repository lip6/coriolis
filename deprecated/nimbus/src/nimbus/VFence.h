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


#ifndef __NIMBUS_VFENCE_H__
#define __NIMBUS_VFENCE_H__

#include "hurricane/DbU.h"
#include "nimbus/Fence.h"

namespace Nimbus {

class GCell;

class VFence : public Fence {

    public:
        typedef Fence Inherit;
        
    private:
        GCell* _gcleft;
        GCell* _gcright;

    protected:
        VFence (Grid* grid, GCell* gcleft = NULL, GCell* gcright = NULL);
        ~VFence();

    public:
        static VFence* create (Grid* grid, GCell* gcleft, GCell* gcright);

        virtual void _postCreate();
        //virtual void _preDestroy();

        virtual GCell* getLeftGCell()  const { return _gcleft;  };
        virtual GCell* getRightGCell() const { return _gcright; };

        virtual GCell* getGCell1() const { return getLeftGCell();  };
        virtual GCell* getGCell2() const { return getRightGCell(); };

        virtual DbU::Unit getX() const;
        virtual DbU::Unit getY() const;
        virtual DbU::Unit getYMin() const;
        virtual DbU::Unit getYMax() const;
        virtual Point getP1() const;
        virtual Point getP2() const;
        virtual DbU::Unit getXCenter() const;
        virtual DbU::Unit getYCenter() const;
        virtual Point getCenter() const;

        virtual bool isHorizontal() { return true;  };
        virtual bool isVertical()   { return false; };

        virtual void computeCapacity();

        virtual void _moveTo (DbU::Unit target);

        virtual Record* _getRecord() const;
        virtual string  _getString() const;
        virtual string _getTypeName() const {return _TName("VFence");};
};

}
#endif
