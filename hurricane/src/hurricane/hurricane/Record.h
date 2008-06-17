

// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
//
// ===================================================================
//
// $Id: Record.h,v 1.7 2007/07/29 15:24:58 jpc Exp $
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Record.h"                               |
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
