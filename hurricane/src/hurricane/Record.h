

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




# ifndef  __RECORD__
#   define  __RECORD__


#   ifndef  __HURRICANE_COMMONS__
#     error "Record.h musn't be included alone, please uses Commons.h."
#   endif


namespace Hurricane {


// -------------------------------------------------------------------
// Forward Declarations.

  class Slot;




// -------------------------------------------------------------------
// Class  :  "Record".

  class Record {

    // Types.
    public:
      typedef list<Slot*> SlotList;

    // Attributes
	private:
      string    _name;
      SlotList  _slotList;

    // Constructors
    public:
      Record ( const string& name );
	private:
      Record            ( const Record& record );
	  Record& operator= ( const Record& record );

    // Destructor
	public:
      virtual ~Record();

    // Accessors
	public:
      const string&   GetName          ()              const { return _name; };
            Slot*     GetSlot          ( unsigned no ) const;

    // Updators
	public:
            void      Add              ( Slot* slot );

    // Others
	public:
            string    _GetTypeName     () const { return _TName("Record"); };
            string    _GetString       () const;
	        SlotList& _GetSlotList     () { return _slotList; };
			  	
};


} // End of Hurricane namespace.




#endif
