// -*- mode: C++; explicit-buffer-name: "Session.h<katabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/Session.h"                         |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_SESSION_H
#define  KATABATIC_SESSION_H

#include  <string>
#include  <vector>
#include  <set>
#include  <map>
#include  <boost/function.hpp>
#include  "hurricane/Commons.h"
#include  "hurricane/DbU.h"
#include  "katabatic/Constants.h"

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
    // Static Methods.
      static inline bool                        doDestroyBaseContact  ();
      static inline bool                        doDestroyBaseSegment  ();
      static inline bool                        doDestroyTool         ();
      static        bool                        isInDemoMode          ();
      static        bool                        doWarnGCellOverload   ();
      static        Session*                    get                   ( const char* message=NULL );
      static inline Technology*                 getTechnology         ();
      static inline KatabaticEngine*            getKatabatic          ();
      static inline const Configuration*        getConfiguration      ();
      static inline RoutingGauge*               getRoutingGauge       ();
      static        unsigned int                getLayerDirection     ( const Layer* );
      static        float                       getSaturateRatio      ();
      static        size_t                      getSaturateRp         ();
      static        DbU::Unit                   getExtensionCap       ();
      static        const Layer*                getRoutingLayer       ( size_t );
      static        const Layer*                getContactLayer       ( size_t );
      static inline size_t                      getSegmentStackSize   ();
      static inline size_t                      getContactStackSize   ();
      static inline const vector<AutoSegment*>& getInvalidateds       (); 
      static inline const vector<AutoSegment*>& getRevalidateds       (); 
      static inline const set<AutoSegment*>&    getDestroyeds         (); 
      static inline const vector<AutoSegment*>& getDoglegs            (); 
      static inline const set<Net*>&            getNetsModificateds   (); 
      static        Session*                    open                  ( KatabaticEngine* );
      static        void                        close                 ();
      static        void                        setKatabaticFlags     ( unsigned int );
      static inline void                        dogleg                ( AutoSegment* );
      static inline void                        doglegReset           ();
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
             inline void                        _dogleg               ( AutoSegment* );
             inline void                        _doglegReset          ();
                    void                        _invalidate           ( Net* );
             inline void                        _invalidate           ( AutoContact* );
             inline void                        _invalidate           ( AutoSegment* );
             inline void                        _destroyRequest       ( AutoSegment* );
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
             vector<AutoSegment*>  _doglegs;
             vector<AutoSegment*>  _segmentInvalidateds;
             vector<AutoSegment*>  _segmentRevalidateds;
             set<Net*>             _netInvalidateds;
             set<Net*>             _netRevalidateds;
             set<AutoSegment*>     _destroyedSegments;

    // Constructors.
    protected:
                                   Session             ( KatabaticEngine* );
      virtual                     ~Session             ();
      virtual void                 _postCreate         ();
      virtual void                 _preDestroy         ();
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
  inline size_t                      Session::getSegmentStackSize  () { return get("getSegmentStackSize()")->_segmentInvalidateds.size(); }
  inline size_t                      Session::getContactStackSize  () { return get("getContactStackSize()")->_autoContacts.size(); }
  inline const vector<AutoSegment*>& Session::getInvalidateds      () { return get("getInvalidateds()")->_segmentInvalidateds; }
  inline const vector<AutoSegment*>& Session::getRevalidateds      () { return get("getRevalidateds()")->_segmentRevalidateds; }
  inline const set<AutoSegment*>&    Session::getDestroyeds        () { return get("getDestroyeds()")->_destroyedSegments; }
  inline const vector<AutoSegment*>& Session::getDoglegs           () { return get("getDoglegs()")->_doglegs; }
  inline const set<Net*>&            Session::getNetsModificateds  () { return get("getNetsModificateds()")->_netRevalidateds; }
  inline void                        Session::doglegReset          () { return get("doglegReset()")->_doglegReset (); }
  inline void                        Session::invalidate           ( Net* net ) { return get("invalidate(Net*)")->_invalidate(net); }
  inline void                        Session::invalidate           ( AutoContact* autoContact ) { return get("invalidate(AutoContact*)")->_invalidate(autoContact); }
  inline void                        Session::invalidate           ( AutoSegment* autoSegment ) { return get("invalidate(AutoSegment*)")->_invalidate(autoSegment); }
  inline void                        Session::dogleg               ( AutoSegment* autoSegment ) { return get("dogleg(AutoSegment*)")->_dogleg(autoSegment); }
  inline void                        Session::destroyRequest       ( AutoSegment* autoSegment ) { return get("destroyRequest(AutoSegment*)")->_destroyRequest(autoSegment); }

  inline void                        Session::_dogleg              ( AutoSegment* segment ) { _doglegs.push_back(segment); }
  inline void                        Session::_doglegReset         () { _doglegs.clear(); }
  inline void                        Session::_invalidate          ( AutoContact* contact ) { _autoContacts.push_back(contact); }
  inline void                        Session::_invalidate          ( AutoSegment* segment ) { _segmentInvalidateds.push_back(segment); }
  inline void                        Session::_destroyRequest      ( AutoSegment* segment ) { _destroyedSegments.insert(segment); }
  inline string                      Session::_getTypeName         () const { return _TName("Session"); }


} // Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::Session);


#endif  // KATABATIC_SESSION_H
