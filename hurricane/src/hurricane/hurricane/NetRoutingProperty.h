// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2015, All Rights Reserved
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


#ifndef  HURRICANE_NET_ROUTING_PROPERTY_H
#define  HURRICANE_NET_ROUTING_PROPERTY_H

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
      enum State { Excluded             = 0x0001
                 , Fixed                = 0x0002
                 , Unconnected          = 0x0004
                 , ManualGlobalRoute    = 0x0008
                 , AutomaticGlobalRoute = 0x0010
                 , MixedPreRoute        = Fixed|ManualGlobalRoute
                 };
    public:
      inline  bool          isUnconnected          () const;
      inline  bool          isFixed                () const;
      inline  bool          isManualGlobalRoute    () const;
      inline  bool          isAutomaticGlobalRoute () const;
      inline  bool          isMixedPreRoute        () const;
      inline  unsigned int  getFlags               () const;
      inline  void          setFlags               ( unsigned int mask );
      inline  void          unsetFlags             ( unsigned int mask );
      inline  Net*          getNet                 () const;
              std::string   _getString             () const;
              Record*       _getRecord             () const;
    private:
      inline                NetRoutingState        ( Net*, unsigned int flags=0 );
                            NetRoutingState        ( const NetRoutingState& ) = delete;
      inline  void          setNet                 ( Net* );
    private:
      Net*          _net;
      unsigned int  _flags;
  };


  inline NetRoutingState::NetRoutingState ( Net* net, unsigned int flags ) : _net(net), _flags(flags) { }

  inline bool          NetRoutingState::isUnconnected          () const { return _flags & Unconnected; };
  inline bool          NetRoutingState::isFixed                () const { return _flags & Fixed; };
  inline bool          NetRoutingState::isManualGlobalRoute    () const { return _flags & ManualGlobalRoute; };
  inline bool          NetRoutingState::isAutomaticGlobalRoute () const { return _flags & AutomaticGlobalRoute; };
  inline bool          NetRoutingState::isMixedPreRoute        () const { return _flags & MixedPreRoute; };
  inline unsigned int  NetRoutingState::getFlags               () const { return _flags; };
  inline Net*          NetRoutingState::getNet                 () const { return _net; }
  inline void          NetRoutingState::setFlags               ( unsigned int mask ) { _flags |=  mask; }
  inline void          NetRoutingState::unsetFlags             ( unsigned int mask ) { _flags &= ~mask; }
  inline void          NetRoutingState::setNet                 ( Net* net ) { _net = net; }


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
// Class  :  "Hurricane::NetRoutingExtension".

  class NetRoutingExtension {
    public:
      static inline  bool             isUnconnected          ( const Net* );
      static inline  bool             isFixed                ( const Net* );
      static inline  bool             isManualGlobalRoute    ( const Net* );
      static inline  bool             isAutomaticGlobalRoute ( const Net* );
      static inline  bool             isMixedPreRoute        ( const Net* );
      static inline  unsigned int     getFlags               ( const Net* );
      static inline  void             setFlags               ( const Net*, unsigned int mask );
      static inline  void             unsetFlags             ( const Net*, unsigned int mask );
      static         NetRoutingState* get                    ( const Net* );
      static         NetRoutingState* create                 ( Net* );
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


  inline unsigned int  NetRoutingExtension::getFlags ( const Net* net )
  {
    NetRoutingState* state = get( net );
    return (state == NULL) ? 0 : state->getFlags();
  }


  inline void  NetRoutingExtension::setFlags ( const Net* net, unsigned int mask )
  {
    NetRoutingState* state = get( net );
    if (state != NULL) state->setFlags( mask );
  }


  inline void  NetRoutingExtension::unsetFlags ( const Net* net, unsigned int mask )
  {
    NetRoutingState* state = get( net );
    if (state != NULL) state->unsetFlags( mask );
  }


}  // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::NetRoutingState);


#endif   // HURRICANE_NET_ROUTING_PROPERTY_H
