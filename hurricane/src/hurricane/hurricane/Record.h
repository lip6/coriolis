
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Record.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_RECORD__
#define  __HURRICANE_RECORD__


#ifndef  __HURRICANE_COMMONS__
#error "Record.h musn't be included alone, please uses Commons.h."
#endif


namespace Hurricane {


  class Slot;


  class Record {

    public:
    // Types.
      typedef list<Slot*>   SlotList;

    public:
    // Constructor & Destructor.
                            Record        ( const string& name );
      virtual              ~Record        ();
    // Methods.
      static  size_t        getAllocateds ();
      inline  const string& getName       () const;
              Slot*         getSlot       ( unsigned no ) const;
              void          add           ( Slot* slot );
      inline  SlotList&     _getSlotList  ();

	private:
    // Internal: Static Attributes.
      static  size_t        _allocateds;
    // Internal: Attributes
              string        _name;
              SlotList      _slotList;

	private:
    // Forbidden: Constructors
                            Record        ( const Record& record );
              Record&       operator=     ( const Record& record );
};


// Inline Functions.
  inline  const string&     Record::getName      () const { return _name; }
  inline  Record::SlotList& Record::_getSlotList () { return _slotList; }


} // End of Hurricane namespace.




#endif
