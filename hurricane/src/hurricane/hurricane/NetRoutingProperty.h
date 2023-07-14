// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/NetRoutingProperty.h"              |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <map>
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include "hurricane/Slot.h"
#include "hurricane/Net.h"


namespace Hurricane {

  class NetRoutingProperty;


// -------------------------------------------------------------------
// Class  :  "Hurricane::NetRoutingState".

  class NetRoutingState {
      friend class NetRoutingProperty;
      friend class NetRoutingExtension;

    public:
      enum State { Excluded             = (1<< 0)
                 , Fixed                = (1<< 1)
                 , Unconnected          = (1<< 2)
                 , ManualGlobalRoute    = (1<< 3)
                 , ManualDetailRoute    = (1<< 4)
                 , AutomaticGlobalRoute = (1<< 5)
                 , MixedPreRoute        = Fixed|ManualDetailRoute
                 , Horizontal           = (1<< 6)
                 , Vertical             = (1<< 7)
                 , Symmetric            = (1<< 8)
                 , SymmetricMaster      = (1<< 9)
                 , Analog               = (1<<10)
                 , ShortNet             = (1<<11)
                 , HasAntenna           = (1<<11)
                 };
    public:
      inline  bool          isExcluded             () const;
      inline  bool          isFixed                () const;
      inline  bool          isUnconnected          () const;
      inline  bool          isManualGlobalRoute    () const;
      inline  bool          isManualDetailRoute    () const;
      inline  bool          isAutomaticGlobalRoute () const;
      inline  bool          isMixedPreRoute        () const;
      inline  bool          isSymmetric            () const;
      inline  bool          isSymHorizontal        () const;
      inline  bool          isSymVertical          () const;
      inline  bool          isSymMaster            () const;
      inline  bool          isSymSlave             () const;
      inline  bool          isAnalog               () const;
      inline  bool          isShortNet             () const;
      inline  bool          hasAntenna             () const;
      inline  Net*          getNet                 () const;
      inline  Net*          getSymNet              () const;
      inline  DbU::Unit     getSymAxis             () const;
      inline  uint32_t      getFlags               () const;
      inline  void          setSymNet              ( Net* );
      inline  void          setSymAxis             ( DbU::Unit );
      inline  void          setFlags               ( uint32_t mask );
      inline  void          unsetFlags             ( uint32_t mask );
      inline  bool          isSelfSym              () const;
              void          setWPitch              ( uint32_t );
      inline  uint32_t      getWPitch              () const;
              DbU::Unit     getSymValue            ( DbU::Unit ) const;
              std::string   _getString             () const;
              Record*       _getRecord             () const;
    private:
      inline                NetRoutingState        ( Net*, uint32_t flags=0 );
                            NetRoutingState        ( const NetRoutingState& ) = delete;
      inline  void          setNet                 ( Net* );
    private:
      Net*          _net;
      Net*          _symNet;
      uint32_t      _flags;
      DbU::Unit     _axis;
      uint32_t      _wPitch;
  };


  inline NetRoutingState::NetRoutingState ( Net* net, uint32_t flags ) : _net(net), _symNet(NULL), _flags(flags), _axis(0), _wPitch(1) { }

  inline bool          NetRoutingState::isExcluded             () const { return _flags & Excluded; };
  inline bool          NetRoutingState::isFixed                () const { return _flags & Fixed; };
  inline bool          NetRoutingState::isUnconnected          () const { return _flags & Unconnected; };
  inline bool          NetRoutingState::isManualGlobalRoute    () const { return _flags & ManualGlobalRoute; };
  inline bool          NetRoutingState::isManualDetailRoute    () const { return _flags & ManualDetailRoute; };
  inline bool          NetRoutingState::isAutomaticGlobalRoute () const { return _flags & AutomaticGlobalRoute; };
  inline bool          NetRoutingState::isMixedPreRoute        () const { return _flags & MixedPreRoute; };
  inline bool          NetRoutingState::isSymmetric            () const { return _flags & Symmetric; }
  inline bool          NetRoutingState::isSymHorizontal        () const { return _flags & Horizontal; }
  inline bool          NetRoutingState::isSymVertical          () const { return _flags & Vertical; }
  inline bool          NetRoutingState::isSymMaster            () const { return _flags & SymmetricMaster; }
  inline bool          NetRoutingState::isAnalog               () const { return _flags & Analog; }
  inline bool          NetRoutingState::isShortNet             () const { return _flags & ShortNet; }
  inline bool          NetRoutingState::hasAntenna             () const { return _flags & HasAntenna; }
  inline Net*          NetRoutingState::getSymNet              () const { return _symNet; }
  inline DbU::Unit     NetRoutingState::getSymAxis             () const { return _axis; }
  inline uint32_t      NetRoutingState::getFlags               () const { return _flags; };
  inline Net*          NetRoutingState::getNet                 () const { return _net; }
  inline void          NetRoutingState::setFlags               ( uint32_t mask ) { _flags |=  mask; }
  inline void          NetRoutingState::unsetFlags             ( uint32_t mask ) { _flags &= ~mask; }
  inline void          NetRoutingState::setNet                 ( Net* net ) { _net = net; }
  inline void          NetRoutingState::setSymNet              ( Net* symNet ) { _symNet = symNet; }
  inline void          NetRoutingState::setSymAxis             ( DbU::Unit axis ) { _axis = axis; } 
  inline bool          NetRoutingState::isSelfSym              () const { return (_symNet == NULL) and (isSymmetric()); }
  inline bool          NetRoutingState::isSymSlave             () const { return (_symNet != NULL) and (not isSymMaster()); }
  inline uint32_t      NetRoutingState::getWPitch              () const { return _wPitch; }
                                                                                   

// -------------------------------------------------------------------
// Class  :  "Hurricane::NetRoutingProperty".

  class NetRoutingProperty : public PrivateProperty {
      friend class NetRoutingExtension;
    public:
      static  Name                _name;
    public:
      static  NetRoutingProperty* create          ( Net* owner );
      static  Name                getPropertyName ();
      virtual Name                getName         () const;
      inline  NetRoutingState*    getState        ();
      virtual void                onReleasedBy    ( DBo* owner );
      virtual bool                hasJson         () const;
      virtual void                toJson          ( JsonWriter*, const DBo* ) const;
      virtual std::string         _getTypeName    () const;
      virtual std::string         _getString      () const;
      virtual Record*             _getRecord      () const;
    protected:
    // Attributes.
      NetRoutingState  _state;
      
    protected:
    // Constructor.
      inline  NetRoutingProperty ( Net* owner );
  };


  inline                     NetRoutingProperty::NetRoutingProperty ( Net* owner ) : PrivateProperty(), _state(owner) { }
  inline NetRoutingState*    NetRoutingProperty::getState           () { return &_state; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonNetRoutingProperty".

  class JsonNetRoutingProperty : public JsonObject {
    public:
      static  void                    initialize             ();
                                      JsonNetRoutingProperty ( unsigned long );
      virtual std::string             getTypeName            () const;
      virtual JsonNetRoutingProperty* clone                  ( unsigned long ) const;
      virtual void                    toData                 ( JsonStack& ); 
  };


// -------------------------------------------------------------------
// Class  :  "Hurricane::NetRoutingExtension".

  class NetRoutingExtension {
    public:
      static inline  bool             isUnconnected          ( const Net* );
      static inline  bool             isFixed                ( const Net* );
      static inline  bool             isManualGlobalRoute    ( const Net* );
      static inline  bool             isManualDetailRoute    ( const Net* );
      static inline  bool             isAutomaticGlobalRoute ( const Net* );
      static inline  bool             isMixedPreRoute        ( const Net* );
      static inline  bool             isSymmetric            ( const Net* );
      static inline  bool             isSymHorizontal        ( const Net* );
      static inline  bool             isSymVertical          ( const Net* );
      static inline  bool             isSymMaster            ( const Net* );
      static inline  bool             isAnalog               ( const Net* );
      static inline  bool             isShortNet             ( const Net* );
      static inline  uint32_t         getFlags               ( const Net* );
      static inline  Net*             getSymNet              ( const Net* );
      static inline  DbU::Unit        getSymAxis             ( const Net* );
      static inline  void             setSymNet              ( const Net*, Net* );
      static inline  void             setSymAxis             ( const Net*, DbU::Unit );
      static inline  void             setFlags               ( const Net*, uint32_t mask );
      static inline  void             unsetFlags             ( const Net*, uint32_t mask );

      static inline  uint32_t         getWPitch              ( const Net* );
      static inline  void             setWPitch              ( const Net*, uint32_t w );

      static         NetRoutingState* get                    ( const Net* );
      static         NetRoutingState* create                 ( Net*, uint32_t flags=0 );
    private:
      static const Net*       _owner;
      static NetRoutingState* _cache;
  };


  inline bool  NetRoutingExtension::isUnconnected ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isUnconnected();
  }


  inline bool  NetRoutingExtension::isFixed ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isFixed();
  }


  inline bool  NetRoutingExtension::isManualGlobalRoute ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isManualGlobalRoute();
  }


  inline bool  NetRoutingExtension::isManualDetailRoute ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isManualDetailRoute();
  }


  inline bool  NetRoutingExtension::isAutomaticGlobalRoute ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? true : state->isAutomaticGlobalRoute();
  }


  inline bool  NetRoutingExtension::isMixedPreRoute ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isMixedPreRoute();
  }


  inline bool  NetRoutingExtension::isSymmetric ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isSymmetric();
  }


  inline bool  NetRoutingExtension::isSymHorizontal ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isSymHorizontal();
  }


  inline bool  NetRoutingExtension::isSymVertical ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isSymVertical();
  }


  inline bool  NetRoutingExtension::isSymMaster ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isSymMaster();
  }


  inline bool  NetRoutingExtension::isAnalog ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isAnalog();
  }


  inline bool  NetRoutingExtension::isShortNet ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? false : state->isShortNet();
  }


  inline uint32_t  NetRoutingExtension::getFlags ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? 0 : state->getFlags();
  }


  inline Net* NetRoutingExtension::getSymNet ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? NULL : state->getSymNet();
  }


  inline DbU::Unit  NetRoutingExtension::getSymAxis ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? 0 : state->getSymAxis();
  }


  inline void  NetRoutingExtension::setFlags ( const Net* net, uint32_t mask )
  {
    NetRoutingState* state = get( net );
    if (state != NULL) state->setFlags( mask );
  }


  inline void  NetRoutingExtension::setSymNet ( const Net* net, Net* symNet )
  {
    NetRoutingState* state = get( net );
    if (state != NULL) state->setSymNet( symNet );
  }


  inline void  NetRoutingExtension::setSymAxis ( const Net* net, DbU::Unit axis )
  {
    NetRoutingState* state = get( net );
    if (state != NULL) state->setSymAxis( axis );
  }


  inline void  NetRoutingExtension::unsetFlags ( const Net* net, uint32_t mask )
  {
    NetRoutingState* state = get( net );
    if (state != NULL) state->unsetFlags( mask );
  }


  inline uint32_t  NetRoutingExtension::getWPitch ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? 0 : state->getWPitch();
  }


  inline void  NetRoutingExtension::setWPitch ( const Net* net, uint32_t w )
  {
    NetRoutingState* state = get( net );
    if (state != NULL) state->setWPitch( w );
  }


}  // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::NetRoutingState);
