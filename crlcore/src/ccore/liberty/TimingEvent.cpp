
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
#include "crlcore/TimingEvent.h"

namespace CRL {
    
// ****************************************************************************************************
// CTimingEvent definition
// ****************************************************************************************************

CTimingEvent::CTimingEvent(const Code& code)
// *****************************************
:	_code(code)
{
}

CTimingEvent::CTimingEvent(const CTimingEvent& event)
// **************************************************
:	_code(event._code)
{
}

CTimingEvent& CTimingEvent::operator=(const CTimingEvent& event)
// *************************************************************
{
	_code = event._code;
	return *this;
}

string CTimingEvent::_getString() const
// ************************************
{
	switch (_code) {
		case UNDEFINED : return "UNDEFINED";
		case FALLING_EDGE : return "FALLING_EDGE";
		case RISING_EDGE : return "RISING_EDGE";
	}
	return "ABNORMAL";
}

string CTimingEvent::getName() const
// *********************************
{
	switch (_code) {
		case UNDEFINED : return "?";
		case FALLING_EDGE : return "D";
		case RISING_EDGE : return "U";
	}
	return "!?";
}

Record* CTimingEvent::_getRecord() const
// *******************************
{
	Record* record = new Record(getString(this));
	record->add(getSlot("Code", (unsigned*)&_code));
	return record;
}

CTimingEvent getCTimingEvent(const string &s)
// ******************************************
{
    if ((s == "Rise") || (s == "U") || (s == "u"))
        return CTimingEvent::RISING_EDGE;
    else if ((s == "Fall") || (s == "D") || (s == "d"))
        return CTimingEvent::FALLING_EDGE;
    else
        return CTimingEvent::UNDEFINED;
}

} //namespace CRL

CRL::CTimingEvent operator-(CRL::CTimingEvent event)
// *************************************************
{
    if ( event == CRL::CTimingEvent::FALLING_EDGE )
      return CRL::CTimingEvent::RISING_EDGE;

    return CRL::CTimingEvent::FALLING_EDGE;
}

