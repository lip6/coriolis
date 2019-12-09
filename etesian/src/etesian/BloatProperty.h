// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/BloatProperty.h"                   |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_BLOAT_PROPERTY_H
#define  HURRICANE_BLOAT_PROPERTY_H

#include <string>
#include <map>
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Slot.h"


namespace Etesian {

  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Cell;
  using Hurricane::Record;
  using Hurricane::PrivateProperty;
  using Hurricane::JsonObject;
  using Hurricane::JsonStack;
  using Hurricane::Occurrence;
  class BloatProperty;
  class JsonBloatProperty;


// -------------------------------------------------------------------
// Class  :  "Etesian::BloatState".

  class BloatState {
      friend class BloatProperty;
      friend class BloatExtension;
      friend class JsonBloatProperty;
    public:
      inline  void         setTracksCount ( size_t );
      inline  size_t       getTracksCount () const;
              std::string  _getString     () const;
              Record*      _getRecord     () const;
    private:
      inline               BloatState     ( size_t );
                           BloatState     ( const BloatState& ) = delete;
    private:
      size_t  _tracksCount;
  };


  inline BloatState::BloatState ( size_t tracksCount ) : _tracksCount(tracksCount) { }

  inline void    BloatState::setTracksCount ( size_t tracksCount ) { _tracksCount=tracksCount; }
  inline size_t  BloatState::getTracksCount () const               { return _tracksCount; }
                                                                                   

// -------------------------------------------------------------------
// Class  :  "Etesian::BloatProperty".

  class BloatProperty : public PrivateProperty {
      friend class BloatExtension;
    public:
      static  Name           _name;
    public:
      static  BloatProperty* create          ();
      static  Name           getPropertyName ();
      virtual Name           getName         () const;
      inline  BloatState*    getState        ();
      virtual void           onReleasedBy    ( DBo* owner );
      virtual bool           hasJson         () const;
      virtual void           toJson          ( JsonWriter*, const DBo* ) const;
      virtual std::string    _getTypeName    () const;
      virtual std::string    _getString      () const;
      virtual Record*        _getRecord      () const;
    protected:
    // Attributes.
      BloatState  _state;
    protected:
    // Constructor.
      inline  BloatProperty ();
  };


  inline             BloatProperty::BloatProperty () : PrivateProperty(), _state(0) { }
  inline BloatState* BloatProperty::getState      () { return &_state; }


// -------------------------------------------------------------------
// Class  :  "Etesian::JsonBloatProperty".

  class JsonBloatProperty : public JsonObject {
    public:
      static  void               initialize        ();
                                 JsonBloatProperty ( unsigned long );
      virtual std::string        getTypeName       () const;
      virtual JsonBloatProperty* clone             ( unsigned long ) const;
      virtual void               toData            ( JsonStack& ); 
  };


// -------------------------------------------------------------------
// Class  :  "Etesian::BloatExtension".

  class BloatExtension {
    public:
      static inline  size_t      getTracksCount ( Occurrence );
      static inline  void        setTracksCount ( Occurrence, size_t );
      static         BloatState* get            ( Occurrence );
      static         BloatState* create         ( Occurrence, size_t tracksCount=0 );
    private:
      static Occurrence  _owner;
      static BloatState* _cache;
  };


  inline size_t  BloatExtension::getTracksCount ( Occurrence o )
  {
    BloatState* state = get( o );
    return (state) ? state->getTracksCount() : 0;
  }


  inline void  BloatExtension::setTracksCount ( Occurrence o, size_t tracksCount )
  {
    BloatState* state = get( o );
    if (state) state->setTracksCount( tracksCount );
  }


}  // Etesian namespace.


INSPECTOR_P_SUPPORT(Etesian::BloatState);


#endif   // HURRICANE_BLOAT_PROPERTY_H
