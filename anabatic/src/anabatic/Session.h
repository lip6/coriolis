// -*- mode: C++; explicit-buffer-name: "Session.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/Session.h"                          |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_SESSION_H
#define  ANABATIC_SESSION_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <boost/function.hpp>
#include "hurricane/Commons.h"
#include "hurricane/Box.h"
#include "crlcore/CellGauge.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/Constants.h"
#include "anabatic/Configuration.h"

namespace Hurricane {
  class Layer;
  class Technology;
  class Net;
  class Contact;
  class Segment;
}


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using std::string;
  using std::vector;
  using std::set;
  using std::map;
  using std::make_pair;
  using Hurricane::tab;
  using Hurricane::_TName;
  using Hurricane::Record;
  using Hurricane::Layer;
  using Hurricane::Technology;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Box;
  using Hurricane::Net;
  using Hurricane::Contact;
  using Hurricane::Segment;
  using CRL::RoutingGauge;

  class AutoContact;
  class AutoSegment;
  class AnabaticEngine;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Session".

  class Session {
    public:
    // Static Methods.
      static  inline bool                        doDestroyBaseContact  ();
      static  inline bool                        doDestroyBaseSegment  ();
      static  inline bool                        doDestroyTool         ();
      static         bool                        isInDemoMode          ();
      static         bool                        doWarnGCellOverload   ();
      static         Session*                    get                   ( const char* message=NULL );
      static  inline Technology*                 getTechnology         ();
      static  inline AnabaticEngine*             getAnabatic           ();
      static  inline const Configuration*        getConfiguration      ();
      static         float                       getSaturateRatio      ();
      static         size_t                      getSaturateRp         ();
      static  inline size_t                      getAllowedDepth       ();
      static         DbU::Unit                   getExtensionCap       ();
      static  inline CellGauge*                  getCellGauge          ();
      static  inline DbU::Unit                   getSliceHeight        ();
      static  inline DbU::Unit                   getSliceStep          ();
      static  inline RoutingGauge*               getRoutingGauge       ();
      static  inline RoutingLayerGauge*          getLayerGauge         ( size_t depth );
      static  inline size_t                      getDepth              ();
      static  inline size_t                      getViaDepth           ( const Layer* layer );
      static  inline size_t                      getLayerDepth         ( const Layer* layer );
      static  inline const Layer*                getRoutingLayer       ( size_t );
      static  inline const Layer*                getContactLayer       ( size_t );
      static         Flags                       getDirection          ( size_t depth );
      static  inline DbU::Unit                   getPitch              ( size_t depth, Flags flags );
      static  inline DbU::Unit                   getOffset             ( size_t depth );
      static  inline DbU::Unit                   getWireWidth          ( size_t depth );
      static  inline DbU::Unit                   getViaWidth           ( size_t depth );
      static  inline Flags                       getDirection          ( const Layer* );
      static  inline DbU::Unit                   getPitch              ( const Layer*, Flags flags );
      static  inline DbU::Unit                   getOffset             ( const Layer* );
      static  inline DbU::Unit                   getWireWidth          ( const Layer* );
      static  inline DbU::Unit                   getViaWidth           ( const Layer* );
      static  inline DbU::Unit                   getExtensionCap       ( const Layer* );
      static  inline Point                       getNearestGridPoint   ( Point, Box constraints );
      static  inline size_t                      getSegmentStackSize   ();
      static  inline size_t                      getContactStackSize   ();
      static  inline const vector<AutoSegment*>& getInvalidateds       (); 
      static  inline const vector<AutoSegment*>& getRevalidateds       (); 
      static  inline const set<AutoSegment*>&    getDestroyeds         (); 
      static  inline const vector<AutoSegment*>& getDoglegs            (); 
      static  inline const set<Net*>&            getNetsModificateds   (); 
      static         void                        close                 ();
      static         void                        setAnabaticFlags      ( Flags );
      static  inline void                        dogleg                ( AutoSegment* );
      static  inline void                        doglegReset           ();
      static  inline void                        revalidateTopology    ();
      static  inline void                        setInvalidateMask     ( Flags );
      static  inline void                        invalidate            ( Net* );
      static  inline void                        invalidate            ( AutoContact* );
      static  inline void                        invalidate            ( AutoSegment* );
      static  inline size_t                      revalidate            ();
      static         void                        link                  ( AutoContact* );
      static         void                        link                  ( AutoSegment* );
      static         void                        unlink                ( AutoContact* );
      static         void                        unlink                ( AutoSegment* );
      static         AutoContact*                lookup                ( Contact* );
      static         AutoSegment*                lookup                ( Segment* );
      static  inline void                        destroyRequest        ( AutoSegment* );
    // Methods.                                                 
      static         Session*                    _open                 ( AnabaticEngine* );
                     bool                        _doDestroyBaseContact ();
                     bool                        _doDestroyBaseSegment ();
                     bool                        _doDestroyTool        ();
      virtual        Configuration*              _getConfiguration     ();
              inline void                        _dogleg               ( AutoSegment* );
              inline void                        _doglegReset          ();
                     void                        _invalidate           ( Net* );
              inline void                        _invalidate           ( AutoContact* );
              inline void                        _invalidate           ( AutoSegment* );
              inline void                        _destroyRequest       ( AutoSegment* );
                     void                        _canonize             ();
                     void                        _revalidateTopology   ();
      virtual        size_t                      _revalidate           ();
                     DbU::Unit                   _getPitch             ( size_t depth, Flags flags ) const;
                     Point                       _getNearestGridPoint  ( Point, Box constraints );
                     Record*                     _getRecord            () const;
                     string                      _getString            () const;
              inline string                      _getTypeName          () const;
                                   
    protected:                     
      static Session*              _session;
             AnabaticEngine*       _anabatic;
             Technology*           _technology;
             CellGauge*            _cellGauge;
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
                                   Session             ( AnabaticEngine* );
      virtual                     ~Session             ();
      virtual void                 _postCreate         ();
      virtual void                 _preDestroy         ();
    private:                       
                                   Session             ( const Session& );
              Session&             operator=           ( const Session& );
  };


// Inline Functions.
  inline Technology*                 Session::getTechnology        () { return get("getTechnology()")->_technology; }
  inline CellGauge*                  Session::getCellGauge         () { return get("getCellGauge()")->_cellGauge; }
  inline RoutingGauge*               Session::getRoutingGauge      () { return get("getRoutingGauge()")->_routingGauge; }
  inline bool                        Session::doDestroyBaseContact () { return get("doDestroyBaseContact()")->_doDestroyBaseContact(); }
  inline bool                        Session::doDestroyBaseSegment () { return get("doDestroyBaseSegment()")->_doDestroyBaseSegment(); }
  inline bool                        Session::doDestroyTool        () { return get("doDestroyTool()")->_doDestroyTool(); }
  inline const Configuration*        Session::getConfiguration     () { return get("getConfiguration()")->_getConfiguration(); }
  inline AnabaticEngine*             Session::getAnabatic          () { return get("getAnabatic()")->_anabatic; }
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

  inline size_t                      Session::getAllowedDepth      () { return getConfiguration()->getAllowedDepth(); }

  inline DbU::Unit                   Session::getSliceHeight       ()                     { return getCellGauge()->getSliceHeight(); }
  inline DbU::Unit                   Session::getSliceStep         ()                     { return getCellGauge()->getSliceStep(); }
  inline RoutingLayerGauge*          Session::getLayerGauge        ( size_t depth )       { return getRoutingGauge()->getLayerGauge(depth); }
  inline size_t                      Session::getDepth             ()                     { return getRoutingGauge()->getDepth(); }
  inline size_t                      Session::getViaDepth          ( const Layer* layer ) { return getRoutingGauge()->getViaDepth(layer); }
  inline size_t                      Session::getLayerDepth        ( const Layer* layer ) { return getRoutingGauge()->getLayerDepth(layer); }
  inline const Layer*                Session::getRoutingLayer      ( size_t depth )       { return getRoutingGauge()->getRoutingLayer(depth); }
  inline const Layer*                Session::getContactLayer      ( size_t depth )       { return getRoutingGauge()->getContactLayer(depth); }
  inline DbU::Unit                   Session::getPitch             ( size_t depth, Flags flags=Flags::NoFlags ) { return get("getPitch(depth,flags)")->_getPitch( depth, flags ); }
  inline DbU::Unit                   Session::getOffset            ( size_t depth )       { return getRoutingGauge()->getLayerOffset(depth); }
  inline DbU::Unit                   Session::getWireWidth         ( size_t depth )       { return getRoutingGauge()->getLayerWireWidth(depth); }
  inline DbU::Unit                   Session::getViaWidth          ( size_t depth )       { return getRoutingGauge()->getViaWidth(depth); }
  inline DbU::Unit                   Session::getPitch             ( const Layer* layer, Flags flags=Flags::NoFlags ) { return getPitch( getLayerDepth(layer), flags ); }
  inline DbU::Unit                   Session::getOffset            ( const Layer* layer ) { return getOffset   ( getLayerDepth(layer) ); }
  inline DbU::Unit                   Session::getWireWidth         ( const Layer* layer ) { return getWireWidth( getLayerDepth(layer) ); }
  inline DbU::Unit                   Session::getViaWidth          ( const Layer* layer ) { return getViaWidth ( getViaDepth(layer) ); }
  inline DbU::Unit                   Session::getExtensionCap      ( const Layer* layer ) { return getConfiguration()->getExtensionCap(layer); }
  inline Flags                       Session::getDirection         ( const Layer* layer ) { return getDirection( getLayerDepth(layer) ); }
  inline Point                       Session::getNearestGridPoint  ( Point p, Box b )     { return get("getNearestGridPoint()")->_getNearestGridPoint(p,b); }

  inline void                        Session::_dogleg              ( AutoSegment* segment ) { _doglegs.push_back(segment); }
  inline void                        Session::_doglegReset         () { _doglegs.clear(); }
  inline void                        Session::_invalidate          ( AutoContact* contact ) { _autoContacts.push_back(contact); }
  inline void                        Session::_invalidate          ( AutoSegment* segment ) { _segmentInvalidateds.push_back(segment); }
  inline void                        Session::_destroyRequest      ( AutoSegment* segment ) { _destroyedSegments.insert(segment); }
  inline string                      Session::_getTypeName         () const { return _TName("Session"); }


} // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::Session);


#endif  // ANABATIC_SESSION_H
