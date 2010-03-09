
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
#ifndef CRL_CCELLPATH_H
#define CRL_CCELLPATH_H

#include  "hurricane/Net.h"
using namespace Hurricane;

#include  "TimingEvent.h"
# include  "hurricane/Slot.h"

namespace CRL {
    
class CLuTable;
class CLuTableTemplate;

class CCellPath : public DBo {

    // Types
    // *****

    public: typedef DBo Inherit;

    public: class Type {
	// ***************

        public: enum Code {UNDEFINED=0, POSITIVE_UNATE=1, NEGATIVE_UNATE=2, LATCH_ACCESS=3, LATCH_SETUP=4, LATCH_HOLD=5};

		private: Code _code;

		public: Type(const Code& code = UNDEFINED);
		public: Type(const Type& type);

		public: Type& operator=(const Type& type);

		public: operator const Code&() const {return _code;};

		public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("CRL::CCellPath::Type"); };
		public: string _getString() const;
		public: Record* _getRecord() const;

	};


    // Attributes
    // **********
            
    private: Net* _net;
    private: Net* _relatedNet;
             
    private: Type _type;

    private: CLuTable* _riseDelay;
    private: CLuTable* _fallDelay;

    private: CLuTable* _riseTransition;
    private: CLuTable* _fallTransition;

    // Constructor
    // ***********
             
    protected: CCellPath (Net* target);
               
    public: static CCellPath* create(Net* target);

    // Accessors
    // *********
               
    public: Net* getNet() const        { return _net; }
    public: Net* getRelatedNet() const { return _relatedNet; }
    public: Type getType() const { return _type; }

    public: CLuTable* getRiseDelayTable() const { return _riseDelay; }
    public: CLuTable* getFallDelayTable() const { return _fallDelay; }
    public: CLuTable* getDelayTable(CTimingEvent event) const;

    public: CLuTable* getRiseTransitionTable() const { return _riseTransition; }
    public: CLuTable* getFallTransitionTable() const { return _fallTransition; }
    public: CLuTable* getTransitionTable(CTimingEvent event) const;


    // Updators
    // ********
            
    public: void _setRelatedNet(Net* relatedNet)    { _relatedNet=relatedNet; }
    public: void _setType(Type type) { _type=type; }

    public: CLuTable* _createRiseDelayTable(CLuTableTemplate* luTemplate);
    public: CLuTable* _createFallDelayTable(CLuTableTemplate* luTemplate);
            
    public: CLuTable* _createRiseTransitionTable(CLuTableTemplate* luTemplate);
    public: CLuTable* _createFallTransitionTable(CLuTableTemplate* luTemplate);
            
    // Others
    // ******
            
    public: string _getString() const;
    public: virtual string _getTypeName() const {return _TName("CCellPath");};
    public: virtual Record* _getRecord() const;
            
    protected: virtual void _postCreate();
};


} //namespace CRL


#endif // CRL_CCELLPATH_H
