// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/TrackFixedSpan.h"                     |
// +-----------------------------------------------------------------+


#pragma  once
#include "katana/TrackElement.h"


namespace Katana {

  using std::string;
  using std::map;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::DbU;
  using Hurricane::Net;
  using Hurricane::Layer;

  class Track;


// -------------------------------------------------------------------
// Class  :  "TrackFixedSpan".

  class TrackFixedSpan : public TrackElement {
    public:
      static  TrackFixedSpan* create                 ( Net*, const Box&, Track* );
    public:                                         
      virtual AutoSegment*    base                   () const;
      virtual Segment*        getSegment             () const;
      virtual bool            isHorizontal           () const;
      virtual bool            isVertical             () const;
      virtual bool            isFixed                () const;
      virtual bool            isPriorityLocked       () const;
      virtual unsigned long   getId                  () const;
      virtual Net*            getNet                 () const;
      virtual Flags           getDirection           () const;
      virtual DbU::Unit       getWidth               () const;
      virtual unsigned int    getDepth               () const;
      virtual const Layer*    getLayer               () const;
      virtual size_t          getTrackSpan           () const;
      virtual TrackElement*   getNext                () const;
      virtual TrackElement*   getPrevious            () const;
      virtual DbU::Unit       getAxis                () const;
      inline  DbU::Unit       getSourceAxis          () const;
      virtual DbU::Unit       getTargetAxis          () const;
      virtual Interval        getFreeInterval        () const;
      virtual void            addOverlapCost         ( TrackCost& ) const;
      virtual float           getPriority            () const;
      virtual void            setPriorityLock        ( bool );
      virtual void            forcePriority          ( float );
      virtual void            computePriority        ();
      virtual void            computeAlignedPriority ();
      inline  void            setNet                 ( Net* );
      virtual void            detach                 ( TrackSet& );
      virtual Record*         _getRecord             () const;
      virtual string          _getString             () const;
      virtual string          _getTypeName           () const;
    private:
              Net* _net;
              Box  _boundingBox;
    protected:
    // Constructors & Destructors.
                                 TrackFixedSpan ( Net*, const Box&, Track* ) ;
      virtual                   ~TrackFixedSpan ();
      virtual void               _postCreate    ();
      virtual void               _preDestroy    ();
    private:
                                 TrackFixedSpan ( const TrackFixedSpan& ) = delete;
              TrackFixedSpan& operator=         ( const TrackFixedSpan& ) = delete;
  };

  
  inline  void  TrackFixedSpan::setNet ( Net* net )
  {
    if (not net or (net == Session::getBlockageNet())) {
      _net = Session::getBlockageNet();
      setFlags( TElemBlockage );
    } else {
      _net = net;
      unsetFlags( TElemBlockage );
    }
  }


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackFixedSpan);
