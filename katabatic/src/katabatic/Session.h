
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Session.h"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __KATABATIC_SESSION__
#define  __KATABATIC_SESSION__

#include  <string>
#include  <vector>
#include  <set>
#include  <map>
#include  <boost/function.hpp>
#include  "hurricane/Commons.h"
#include  "hurricane/DbU.h"

namespace Hurricane {
  class Layer;
  class Technology;
  class Net;
  class Contact;
  class Segment;
}

namespace CRL {
  class RoutingGauge;
}


namespace Katabatic {


  using std::cerr;
  using std::endl;
  using std::string;
  using std::vector;
  using std::set;
  using std::map;
  using std::make_pair;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::_TName;
  using Hurricane::Record;
  using Hurricane::Layer;
  using Hurricane::Technology;
  using Hurricane::DbU;
  using Hurricane::Net;
  using Hurricane::Contact;
  using Hurricane::Segment;
  using CRL::RoutingGauge;

  class Configuration;
  class AutoContact;
  class AutoSegment;
  class KatabaticEngine;


// -------------------------------------------------------------------
// Class  :  "Katabatic::Session".
 

  class Session {
    public:
      enum InvalidateType { NetSplitContacts = (1<<0)
                          , NetCanonize      = (1<<1)
                          , RestoreHCon      = (1<<2)
                          , RestoreVCon      = (1<<3)
                          };

    public:
    // Static Methods.
      static inline bool                        doDestroyBaseContact  ();
      static inline bool                        doDestroyBaseSegment  ();
      static inline bool                        doDestroyTool         ();
      static        Session*                    get                   ( const char* message=NULL );
      static inline Technology*                 getTechnology         ();
      static inline const Configuration*        getConfiguration      ();
      static inline RoutingGauge*               getRoutingGauge       ();
      static inline KatabaticEngine*            getKatabatic          ();
      static        bool                        getDemoMode           ();
      static        float                       getSaturateRatio      ();
      static        bool                        getWarnGCellOverload  ();
      static        DbU::Unit                   getExtensionCap       ();
      static        const Layer*                getRoutingLayer       ( size_t );
      static        const Layer*                getContactLayer       ( size_t );
      static inline size_t                      getSegmentStackSize   ();
      static inline size_t                      getContactStackSize   ();
      static inline const vector<AutoSegment*>& getInvalidateds       (); 
      static inline const vector<AutoSegment*>& getRevalidateds       (); 
      static inline const set<AutoSegment*>&    getDestroyeds         (); 
      static inline const vector<AutoSegment*>& getDogLegs            (); 
      static inline const set<Net*>&            getNetsModificateds   (); 
      static        Session*                    open                  ( KatabaticEngine* );
      static        size_t                      close                 ();
      static        void                        setWarnGCellOverload  ( bool );
      static inline void                        dogLeg                ( AutoSegment* );
      static inline void                        dogLegReset           ();
      static inline void                        revalidateTopology    ();
      static inline void                        setInvalidateMask     ( unsigned int );
      static inline void                        invalidate            ( Net* );
      static inline void                        invalidate            ( AutoContact* );
      static inline void                        invalidate            ( AutoSegment* );
      static inline size_t                      revalidate            ();
      static        void                        link                  ( AutoContact* );
      static        void                        link                  ( AutoSegment* );
      static        void                        unlink                ( AutoContact* );
      static        void                        unlink                ( AutoSegment* );
      static        AutoContact*                lookup                ( Contact* );
      static        AutoSegment*                lookup                ( Segment* );
      static inline void                        destroyRequest        ( AutoSegment* );
    // Methods.                                                 
                    bool                        _doDestroyBaseContact ();
                    bool                        _doDestroyBaseSegment ();
                    bool                        _doDestroyTool        ();
      virtual       Configuration*              _getConfiguration     ();
             inline void                        _dogLeg               ( AutoSegment* );
             inline void                        _dogLegReset          ();
             inline void                        _setInvalidateMask    ( unsigned int );
             inline void                        _invalidate           ( Net* );
             inline void                        _invalidate           ( AutoContact* );
             inline void                        _invalidate           ( AutoSegment* );
             inline void                        _destroyRequest       ( AutoSegment* );
                    void                        _splitContacts        ();
                    void                        _restoreHCon          ();
                    void                        _restoreVCon          ();
                    void                        _canonize             ();
                    void                        _revalidateTopology   ();
                    size_t                      _revalidate           ();
                    Record*                     _getRecord            () const;
                    string                      _getString            () const;
            inline  string                      _getTypeName          () const;
                                   
    protected:                     
      static Session*              _session;
             KatabaticEngine*      _katabatic;
             Technology*           _technology;
             RoutingGauge*         _routingGauge;
             vector<AutoContact*>  _autoContacts;
             vector<AutoSegment*>  _autoSegments;
             vector<AutoSegment*>  _revalidateds;
             vector<AutoSegment*>  _dogLegs;
             set<Net*>             _netInvalidateds;
             set<Net*>             _netRevalidateds;
             set<AutoSegment*>     _destroyedSegments;
             unsigned int          _invalidateMask;

    // Constructors.
    protected:
                                   Session             ( KatabaticEngine* );
      virtual                     ~Session             ();
      virtual void                 _postCreate         ();
      virtual size_t               _preDestroy         ();
    private:                       
                                   Session             ( const Session& );
              Session&             operator=           ( const Session& );
  };


// Inline Functions.
  inline Technology*                 Session::getTechnology        () { return get("getTechnology()")->_technology; }
  inline RoutingGauge*               Session::getRoutingGauge      () { return get("getRoutingGauge()")->_routingGauge; }
  inline bool                        Session::doDestroyBaseContact () { return get("doDestroyBaseContact()")->_doDestroyBaseContact(); }
  inline bool                        Session::doDestroyBaseSegment () { return get("doDestroyBaseSegment()")->_doDestroyBaseSegment(); }
  inline bool                        Session::doDestroyTool        () { return get("doDestroyTool()")->_doDestroyTool(); }
  inline const Configuration*        Session::getConfiguration     () { return get("getConfiguration()")->_getConfiguration(); }
  inline KatabaticEngine*            Session::getKatabatic         () { return get("getKatabatic()")->_katabatic; }
  inline void                        Session::revalidateTopology   () { return get("revalidateTopology()")->_revalidateTopology(); }
  inline size_t                      Session::revalidate           () { return get("revalidate()")->_revalidate(); }
  inline size_t                      Session::getSegmentStackSize  () { return get("getSegmentStackSize()")->_autoSegments.size(); }
  inline size_t                      Session::getContactStackSize  () { return get("getContactStackSize()")->_autoContacts.size(); }
  inline const vector<AutoSegment*>& Session::getInvalidateds      () { return get("getInvalidateds()")->_autoSegments; }
  inline const vector<AutoSegment*>& Session::getRevalidateds      () { return get("getRevalidateds()")->_revalidateds; }
  inline const set<AutoSegment*>&    Session::getDestroyeds        () { return get("getDestroyeds()")->_destroyedSegments; }
  inline const vector<AutoSegment*>& Session::getDogLegs           () { return get("getDogLegs()")->_dogLegs; }
  inline const set<Net*>&            Session::getNetsModificateds  () { return get("getNetsModificateds()")->_netRevalidateds; }
  inline void                        Session::dogLegReset          () { return get("dogLegReset()")->_dogLegReset (); }
  inline void                        Session::setInvalidateMask    ( unsigned int mask ) { return get("setInvalidateMask()")->_setInvalidateMask ( mask ); }
  inline void                        Session::invalidate           ( Net* net ) { return get("invalidate(Net*)")->_invalidate ( net ); }
  inline void                        Session::invalidate           ( AutoContact* autoContact ) { return get("invalidate(AutoContact*)")->_invalidate ( autoContact ); }
  inline void                        Session::invalidate           ( AutoSegment* autoSegment ) { return get("invalidate(AutoSegment*)")->_invalidate ( autoSegment ); }
  inline void                        Session::dogLeg               ( AutoSegment* autoSegment ) { return get("dogLeg(AutoSegment*)")->_dogLeg ( autoSegment ); }
  inline void                        Session::destroyRequest       ( AutoSegment* autoSegment ) { return get("destroyRequest(AutoSegment*)")->_destroyRequest ( autoSegment ); }

  inline void                        Session::_dogLeg              ( AutoSegment* segment ) { _dogLegs.push_back(segment); }
  inline void                        Session::_dogLegReset         () { _dogLegs.clear(); }
  inline void                        Session::_setInvalidateMask   ( unsigned int mask ) { _invalidateMask |= mask; }
  inline void                        Session::_invalidate          ( AutoContact* contact ) { _autoContacts.push_back(contact); }
  inline void                        Session::_invalidate          ( AutoSegment* segment ) { _autoSegments.push_back(segment); }
  inline void                        Session::_destroyRequest      ( AutoSegment* segment ) { _destroyedSegments.insert(segment); }
  inline string                      Session::_getTypeName         () const { return _TName("Session"); }

  inline  void  Session::_invalidate ( Net* net ) {
    ltrace(200) << "Session::invalidate(Net*) - " << net << endl;
    _netInvalidateds.insert(net);
    _invalidateMask |= NetCanonize;
  }


} // End of Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::Session);


#endif  // __KATABATIC_SESSION__
