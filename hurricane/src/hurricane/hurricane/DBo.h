
// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./DBo.h"                                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_DBO__
#define  __HURRICANE_DBO__

#include  "hurricane/DBos.h"
#include  "hurricane/Properties.h"
#include  "hurricane/Name.h"


namespace Hurricane {


  class Property;


// -------------------------------------------------------------------
// Class  :  "Hurricane::DBo".


  class DBo {

    public:
    // Types.
      typedef set<Property*> PropertySet;
    // Methods.
      virtual void           destroy();
      inline  PropertySet&   _getPropertySet ();
              void           _onDestroyed    ( Property* property );
              Property*      getProperty     ( const Name& ) const;
              Properties     getProperties   () const;
      inline  bool           hasProperty     () const;
              void           put             ( Property* );
              void           remove          ( Property* );
              void           removeProperty  ( const Name& );
              void           clearProperties ();
    // Hurricane Managment.
      virtual string         _getTypeName    () const;
      virtual string         _getString      () const;
      virtual Record*        _getRecord      () const;

    private:
    // Internal: Attributes.
      mutable PropertySet _propertySet;

    protected:
    // Internal: Constructors & Destructors.
                             DBo             ();
      virtual               ~DBo             ();
      virtual void           _postCreate     ();
      virtual void           _preDestroy     ();
    private:
    // Forbidden: Copies.
                             DBo             ( const DBo& );
              DBo&           operator=       ( const DBo& );
  };


// Inline Functions.
  inline DBo::PropertySet& DBo::_getPropertySet () { return _propertySet; }
  inline bool              DBo::hasProperty     () const { return !_propertySet.empty(); }


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::DBo);


template<>
inline Hurricane::Slot* getSlot ( const std::string& name, const std::set<Hurricane::Property*>* s )
{
  return new Hurricane::SlotTemplate<const std::set<Hurricane::Property*>*>(name,s);
}


template<>
inline Hurricane::Slot* getSlot ( const std::string& name, std::set<Hurricane::Property*>* s )
{
  return new Hurricane::SlotTemplate<std::set<Hurricane::Property*>*>(name,s);
}


#endif // __HURRICANE_DBO__
