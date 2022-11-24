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


// ****************************************************************************************************
// File: SplitterContacts.h
// Authors: R. Escassut
// Copyright (C) SILVACO & BULL S.A. 2000-2002 All rights reserved
// ****************************************************************************************************

#ifndef __NIMBUS_SPLITTER_CONTACTS__
#define __NIMBUS_SPLITTER_CONTACTS__

#include "hurricane/Collection.h"

namespace Nimbus {

class SplitterContact;



// ****************************************************************************************************
// SplitterContacts declaration
// ****************************************************************************************************

typedef GenericCollection<SplitterContact*> SplitterContacts;



// ****************************************************************************************************
// SplitterContactLocator declaration
// ****************************************************************************************************

typedef GenericLocator<SplitterContact*> SplitterContactLocator;



// ****************************************************************************************************
// SplitterContactFilter declaration
// ****************************************************************************************************

typedef GenericFilter<SplitterContact*> SplitterContactFilter;



// ****************************************************************************************************
// for_each_splittercontact declaration
// ****************************************************************************************************

#define for_each_splittercontact(splittercontact, splittercontacts)\
/******************************************/\
{\
	SplitterContactLocator _locator = splittercontacts.getLocator();\
	while (_locator.isValid()) {\
		SplitterContact* splittercontact = _locator.getElement();\
		_locator.progress();



}
#endif // HURRICANE_SPLITTERCONTACTS

// ****************************************************************************************************
// Copyright (C) SILVACO & BULL S.A. 2000-2002 All rights reserved
// ****************************************************************************************************
