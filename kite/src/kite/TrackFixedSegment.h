

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
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./TrackFixedSegment.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __KITE_TRACK_FIXED_SEGMENT__
#define  __KITE_TRACK_FIXED_SEGMENT__


#include  "kite/TrackElement.h"


namespace Kite {


  using std::string;
  using std::map;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::DbU;
  using Hurricane::Net;
  using Hurricane::Layer;

  class Track;


// -------------------------------------------------------------------
// Class  :  "TrackFixedSegment".
 

  class TrackFixedSegment : public TrackElement {
    public:
      static  TrackElement*  create             ( Track*, Segment* );
    public:
      virtual AutoSegment*   base               () const;
      virtual bool           isFixed            () const;
      virtual bool           isBlockage         () const;
      virtual bool           isHorizontal       () const;
      virtual bool           isVertical         () const;
      virtual unsigned long  getId              () const;
      virtual unsigned int   getDirection       () const;
      virtual Net*           getNet             () const;
      virtual const Layer*   getLayer           () const;
      virtual TrackElement*  getNext            () const;
      virtual TrackElement*  getPrevious        () const;
      virtual DbU::Unit      getAxis            () const;
      virtual Interval       getFreeInterval    ( bool useOrder=false ) const;
      virtual Record*        _getRecord         () const;
      virtual string         _getString         () const;
      virtual string         _getTypeName       () const;

    protected:
    // Attributes.
      static Net*     _blockageNet;
             Segment* _segment;
             bool     _isBlockage;

    protected:
    // Constructors & Destructors.
                                 TrackFixedSegment ( Track*, Segment* ) ;
      virtual                   ~TrackFixedSegment ();
      virtual void               _postCreate       ();
      virtual void               _preDestroy       ();
    private:
                                 TrackFixedSegment ( const TrackFixedSegment& );
              TrackFixedSegment& operator=         ( const TrackFixedSegment& );
      
  };


} // End of Kite namespace.


INSPECTOR_P_SUPPORT(Kite::TrackFixedSegment);


# endif
