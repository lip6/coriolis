
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./TrackSegmentCost.h"                     |
// +-----------------------------------------------------------------+


#ifndef  __TRACK_SEGMENT_COST__
#define  __TRACK_SEGMENT_COST__


#include  <vector>
#include  <string>

#include  "hurricane/DbU.h"
namespace Hurricane {
  class Record;
  class Net;
}


namespace Katana {


  using std::vector;
  using std::string;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Net;

  class TrackElement;


// -------------------------------------------------------------------
// Class  :  "TrackSegmentCost".
 

  class TrackSegmentCost {

    public:
                               TrackSegmentCost ( TrackElement* );
                              ~TrackSegmentCost ();
      inline unsigned int      getTerminals     () const;
      inline unsigned int      getRipupCount    () const;
      inline DbU::Unit         getLeftMinExtend () const;
      inline DbU::Unit         getRightMinExtend() const;
      inline Net*              getNet           () const;
             DbU::Unit         getWiringDelta   ( DbU::Unit axis ) const;
      inline void              setRipupCount    ( unsigned int );
      inline void              incRipupCount    ();
      inline void              decRipupCount    ();
      inline void              resetRipupCount  ();
             void              update           ( TrackElement* );
             Record*           _getRecord       () const;
             string            _getString       () const;
      inline string            _getTypeName     () const;

    protected:
    // Attributes.
      unsigned int       _terminals  :  5;
      unsigned int       _ripupCount : 16;
      DbU::Unit          _leftMinExtend;
      DbU::Unit          _rightMinExtend;
      Net*               _net;
      vector<DbU::Unit>  _attractors;

  };

// Inline Functions.
  inline unsigned int  TrackSegmentCost::getTerminals      () const { return _terminals; }
  inline unsigned int  TrackSegmentCost::getRipupCount     () const { return _ripupCount; }
  inline DbU::Unit     TrackSegmentCost::getLeftMinExtend  () const { return _leftMinExtend; }
  inline DbU::Unit     TrackSegmentCost::getRightMinExtend () const { return _rightMinExtend; }
  inline Net*          TrackSegmentCost::getNet            () const { return _net; }
  inline void          TrackSegmentCost::setRipupCount     ( unsigned int count ) { _ripupCount = count; }
  inline void          TrackSegmentCost::incRipupCount     () { _ripupCount++; }
  inline void          TrackSegmentCost::decRipupCount     () { if (_ripupCount) _ripupCount--; }
  inline void          TrackSegmentCost::resetRipupCount   () { _ripupCount = 0; }
  inline string        TrackSegmentCost::_getTypeName      () const { return "TrackSegmentCost"; }


} // End of Katana namespace.


INSPECTOR_PV_SUPPORT(Katana::TrackSegmentCost);


#endif  // __TRACK_SEGMENT_COST__
