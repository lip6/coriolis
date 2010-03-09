
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
#ifndef CRL_CTIMINGEVENT_H
#define CRL_CTIMINGEVENT_H

#include  "hurricane/Commons.h"
# include  "hurricane/Slot.h"
using namespace Hurricane;

namespace CRL {

//! Description des evenements temporels : front montant ou front descendant
  class CTimingEvent {
    public:enum Code { UNDEFINED=0, FALLING_EDGE=1, RISING_EDGE=2 } ;
    private: Code _code;

    public: CTimingEvent(const Code& code = UNDEFINED);
    public: CTimingEvent(const CTimingEvent& event);

    public: CTimingEvent& operator=(const CTimingEvent& event);

    public: operator const Code&() const {return _code;}
    //public: operator bool() const { return _code != UNDEFINED; } //creates ambiguities

    public: const Code& getCode() const {return _code;}

    public: string  getName() const;
    public: string  _getTypeName() const { return _TName("CTimingEvent"); };
    public: string  _getString() const;
    public: Record* _getRecord() const;
};

CTimingEvent getCTimingEvent(const string &s);

} //namespace CRL

CRL::CTimingEvent operator-(CRL::CTimingEvent event);

#define for_each_ctimingevent(event) \
    for(CRL::CTimingEvent event= CRL::CTimingEvent::FALLING_EDGE ; event != CRL::CTimingEvent::UNDEFINED ; \
        event= (event == CRL::CTimingEvent::FALLING_EDGE)?CRL::CTimingEvent::RISING_EDGE:CRL::CTimingEvent::UNDEFINED )

#endif // CRL_CTIMINGEVENT_H
