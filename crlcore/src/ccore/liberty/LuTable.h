
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//   Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//   Hugo Clément                   <Hugo.Clement@lip6.fr>
//   Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//   Christian Masson           <Christian.Masson@lip6.fr>
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
// Date   : 15/05/2007
// Author : Marek Sroka                    <Marek.Sroka@lip6.fr>
//
// Authors-Tag 
#ifndef CRL_CLUTABLE_H
#define CRL_CLUTABLE_H

#include  "hurricane/DBo.h"
# include  "hurricane/Slot.h"
using namespace Hurricane;

namespace CRL {

class CLuTableTemplate;

class CLuTable : public DBo {

    // Types
    // *****

    public:typedef DBo Inherit;

    // Atributes
    // *********

    private: CLuTableTemplate* _template;
    private: vector<double>     _values;

    // Constructors
    // ************

    public: CLuTable(CLuTableTemplate* luTemplate);

    public: static CLuTable* create(CLuTableTemplate* luTemplate);

    // Updators
    // ********

    public: void AddValues(unsigned short count, double* values);
            
    // Others
    // ******

    public: string _getString() const;
    public: virtual string _getTypeName() const {return _TName("CLuTable");};
    public: virtual Record* _getRecord() const;
            
    public: unsigned short getDimension() const;

    public: double _getValue(unsigned short x=0,unsigned short y=0, unsigned short z=0) const;
    public: double getValue(double var1, double var2=0.0, double var3=0.0) const;

    //! Return the Cut for var=value. Decrease the table dimension by one.
    public: CLuTable* getCut(unsigned short var, double value);

    //! Return the average slope of the table (must be one-dimensionnal)
    public: double Linearize();
};

}

#endif // CRL_CLUTABLE_H
