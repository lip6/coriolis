
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
#ifndef CRL_CLIBERTYTECHNOLOGY_H
#define CRL_CLIBERTYTECHNOLOGY_H

#include <map>

#include  "hurricane/Property.h"
using namespace Hurricane;

#include  "LuTableTemplate.h"
# include  "hurricane/Slot.h"

namespace CRL {

class CLibertyTechnology : public PrivateProperty {

    // Types
    // *****
    
    public:typedef PrivateProperty Inherit;

	public: class Unit {
	// *****************

        public: enum Code {UNDEFINED=0, TIME=1, CAPACITANCE=2};

		private: Code _code;

		public: Unit(const Code& code = UNDEFINED);
		public: Unit(const Unit& unit);

		public: Unit& operator=(const Unit& unit);

		public: operator const Code&() const {return _code;};

		public: const Code& getCode() const {return _code;};

		public: string _getString() const;
		public: Record* _getRecord() const;

	};

    public: struct TemplateComp {
    // **************************
               
        public:bool operator() (const CLuTableTemplate* t1,const CLuTableTemplate* t2) const
        {
            return (t1->getName() < t2->getName());
        }
    };

    public:typedef set<CLuTableTemplate*,TemplateComp> TemplateSet;

    // Attributes
    // **********
           
    private:TemplateSet _templates;
    private:map<Unit,double> _units;

    // Constructor
    // ***********
             
    protected:CLibertyTechnology();

    public:static CLibertyTechnology* create();

    // Accessors
    // *********
           
    public: virtual Name getName() const;
    public: static const Name& getPropertyName();

    public:double getUnit(CLuTableTemplate::Variable::Type type) const;
    public:double getUnit(Unit unit) const;

    public:CLuTableTemplate* getTemplate(string name) const;
           
    // Updators
    // ********

    public:void SetUnit(Unit,double);

    public:void AddTemplate(CLuTableTemplate* lutemplate);
           
    // Others
    // ******
               
    public: string _getString() const;
    public: string _getTypeName() const { return _TName("CLibertyTechnology"); }
    public: Record* _getRecord() const;

    protected: void _postCreate();
};

}


#endif // CRL_CLIBERTYTECHNOLOGY_H
