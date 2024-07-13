// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2024.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/TrackBaseFixedSpan.h"                 |
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
  using Hurricane::RoutingPad;

  class Track;


// -------------------------------------------------------------------
// Class  :  "TrackBaseFixedSpan".

  class TrackBaseFixedSpan : public TrackElement {
    public:                                         
      virtual AutoSegment*        base                   () const;
      virtual Segment*            getSegment             () const;
      virtual bool                isHorizontal           () const;
      virtual bool                isVertical             () const;
      virtual bool                isFixed                () const;
      virtual bool                isPriorityLocked       () const;
      virtual bool                isFixedSpan            () const;
      virtual bool                isFixedSpanRp          () const;
      virtual unsigned long       getId                  () const;
      virtual Net*                getNet                 () const = 0;
      virtual Flags               getDirection           () const;
      virtual DbU::Unit           getWidth               () const;
      virtual unsigned int        getDepth               () const;
      virtual RoutingPad*         getRoutingPad          () const;
      virtual const Layer*        getLayer               () const;
      virtual size_t              getTrackSpan           () const;
      virtual TrackElement*       getNext                () const;
      virtual TrackElement*       getPrevious            () const;
      virtual DbU::Unit           getAxis                () const;
      virtual DbU::Unit           getSourceAxis          () const;
      virtual DbU::Unit           getTargetAxis          () const;
      virtual Interval            getFreeInterval        () const;
      virtual void                addOverlapCost         ( TrackCost& ) const;
      virtual float               getPriority            () const;
      virtual void                setPriorityLock        ( bool );
      virtual void                forcePriority          ( float );
      virtual void                computePriority        ();
      virtual void                computeAlignedPriority ();
      virtual void                setNet                 ( Net* ) = 0;
      virtual void                detach                 ( TrackSet& );
      virtual Record*             _getRecord             () const;
      virtual string              _getString             () const;
      virtual string              _getTypeName           () const;
    private:
              Box  _boundingBox;
    protected:
    // Constructors & Destructors.
                                  TrackBaseFixedSpan ( Net*, const Box& );
      virtual                    ~TrackBaseFixedSpan ();
      virtual void                _postCreate        ( Track* );
    private:                   
                                  TrackBaseFixedSpan ( const TrackBaseFixedSpan& ) = delete;
              TrackBaseFixedSpan& operator=          ( const TrackBaseFixedSpan& ) = delete;
  };


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackBaseFixedSpan);
