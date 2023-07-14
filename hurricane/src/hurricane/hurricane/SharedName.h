// ****************************************************************************************************
// File: ./hurricane/SharedName.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#ifndef HURRICANE_SHARED_NAME
#define HURRICANE_SHARED_NAME

#include "hurricane/Commons.h"

namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::SharedName".


  class SharedName {
      friend class Name;
    public:
      static void           dump         ();
    public:
      inline unsigned long  getHash      () const;
             const string&  _getSString  () const { return _string; };
             string         _getTypeName () const { return _TName("SharedName"); };
             string         _getString   () const;
             Record*        _getRecord   () const;
    private:               
                            SharedName   ( const string& );
                            SharedName   ( const SharedName& );
                           ~SharedName   ();
             SharedName&    operator=    ( const SharedName& );
             void           capture      ();
             void           release      ();

    private:
      struct SharedNameMapComparator {
          bool operator() ( string* lhs, string* rhs ) const;
      };
      typedef  map<string*, SharedName*, SharedNameMapComparator>  SharedNameMap;

    private:
      static SharedNameMap* _SHARED_NAME_MAP;
             unsigned long  _hash;
             int            _count;
             string         _string;
  };


  inline  unsigned long  SharedName::getHash () const { return _hash; }


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::SharedName);


#endif // HURRICANE_SHARED_NAME


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
