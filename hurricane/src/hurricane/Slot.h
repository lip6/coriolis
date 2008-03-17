
// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
//
// ===================================================================
//
// $Id: Slot.h,v 1.9 2007/07/29 15:25:00 jpc Exp $
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Slot.h"                                 |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# ifndef  __HURRICANE_SLOT__
#   define  __HURRICANE_SLOT__


#   ifndef  __HURRICANE_COMMONS__
#     error "Slot.h musn't be included alone, please uses Commons.h."
#   endif




namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Slot".

  class Slot : public NestedSlotAdapter {

    // Attributes
    protected:
      const string       _name;
      const SlotAdapter* _data;

    // Constructors
    protected:
      Slot ( const string& name, const SlotAdapter* data ) : _name(name), _data(data) {};
    private:
      Slot ( const Slot&   slot ); // Not implemented to forbid copy.

    // Destructor
    public:
      virtual ~Slot () {};

    // Operators
    private:
      Slot& operator= ( const Slot& slot ); // Not implemented to forbid assignment.

    // Accessors
    public:
      const   string& getName       () const { return _name; };
      virtual string  getDataString () const { return _data->_getString(); };
      virtual Record* getDataRecord () const { return _data->_getRecord(); };

   // Inspector Managment.
   public:
      virtual string _getString () const
                                {
                                  return "<" + _getTypeName()
                                             + " " + getName() + " "
                                             + _data->_getString() + ">";
                                };

};




// -------------------------------------------------------------------
// Class  :  "PointerSlot".
//
// The SlotAdapter is not duplicated, it is for objects in which
// it's directly integrated (inheritance). This is used for Hurricane
// objects with virtual functions.
//
// This is default behavior of Slot, so PointerSlot doesn't change
// it, but it's more clear for developpers.

  class PointerSlot : public Slot {

    // Constructors
    public:
      PointerSlot ( const string& name, const SlotAdapter* data ) : Slot(name,data) {};

    // Destructor.
    public:
      virtual ~PointerSlot () {};
    

    // Accessors
    public:
      virtual string  _getTypeName   () const { return _TName("PointerSlot"); };

  };




// -------------------------------------------------------------------
// Class  :  "ValueSlot".
//
// The SlotAdapter is duplicated. It's for objects coming from
// external libraries or that do not have virtual functions (like
// Points or Boxes).

  class ValueSlot : public Slot {

    // Constructors
    public:
      ValueSlot (const string& name, const SlotAdapter* data ) : Slot(name,data) {};

    // Destructor.
    public:
      virtual ~ValueSlot () { delete _data; };

   // Accessors
   public:
      virtual string  _getTypeName () const { return _TName("ValueSlot"); };

  };


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Slot)
SetNestedSlotAdapter(Hurricane::PointerSlot)
SetNestedSlotAdapter(Hurricane::ValueSlot)




#endif
