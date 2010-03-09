
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
#ifndef CRL_CCELLPATHS_H
#define CRL_CCELLPATHS_H

#include  "hurricane/Collection.h"
using namespace Hurricane;

namespace CRL {

class CCellPath;



// ****************************************************************************************************
// CCellPaths declaration
// ****************************************************************************************************

typedef GenericCollection<CCellPath*> CCellPaths;



// ****************************************************************************************************
// CCellPathLocator declaration
// ****************************************************************************************************

typedef GenericLocator<CCellPath*> CCellPathLocator;



// ****************************************************************************************************
// CCellPathFilter declaration
// ****************************************************************************************************

typedef GenericFilter<CCellPath*> CCellPathFilter;



// ****************************************************************************************************
// for_each_ccellpath declaration
// ****************************************************************************************************

#define for_each_ccellpath(ccellpath, ccellpaths)\
/************************************************/\
{\
	CCellPathLocator _locator = ccellpaths.getLocator();\
	while (_locator.isValid()) {\
		CCellPath* ccellpath = _locator.getElement();\
		_locator.progress();

} //namespace CRL


#endif // CRL_CCELLPATHS_H
