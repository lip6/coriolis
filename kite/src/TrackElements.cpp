
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
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
// |  C++ Module  :       "./TrackElements.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Bug.h"
#include  "hurricane/Interval.h"
#include  "kite/Session.h"
#include  "kite/TrackElement.h"


namespace Kite {


  using namespace std;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::ForEachIterator;
  using Hurricane::Interval;
  using Hurricane::Bug;


// -------------------------------------------------------------------
// Class  :  "TrackElements_CollapsedPerpandicular".


  TrackElements_CollapsedPerpandicular::Locator::Locator ( TrackElement* segment )
    : TrackElementHL ()
    , _locator       (segment->base())
    , _element       (NULL)
  {
    ltrace(80) << "TrackElements_CollapsedPerpandicular::Locator::Locator()" << endl;
    ltrace(80) << "  " << segment << endl;

    Interval bounds;
    if ( _locator.isValid() ) {
      _element = Session::lookup ( _locator.getElement()->getCanonical(bounds)->base() );
      if ( !_element ) {
        cerr << Bug("Canonical segment without TrackElement.") << endl;
        progress ();
      }
    }
  }


  TrackElement* TrackElements_CollapsedPerpandicular::Locator::getElement () const
  { return _element; }


  void  TrackElements_CollapsedPerpandicular::Locator::progress ()
  {
    ltrace(80) << "TrackElements_CollapsedPerpandicular::Locator::progress()" << endl;

    Interval bounds;
    while ( _locator.isValid() ) {
      _locator.progress ();

      if ( _locator.isValid() ) {
        _element = Session::lookup ( _locator.getElement()->getCanonical(bounds)->base() );
        if ( !_element ) {
          cerr << Bug("Canonical segment whithout TrackElement.") << endl;
          continue;
        }

        break;
      }
    }
  }


  TrackElementHL* TrackElements_CollapsedPerpandicular::Locator::getClone () const
  { return new Locator(*this); }


  bool  TrackElements_CollapsedPerpandicular::Locator::isValid () const
  { return _locator.isValid(); }


  TrackElementHC* TrackElements_CollapsedPerpandicular::getClone () const
  { return new TrackElements_CollapsedPerpandicular(*this); }


  TrackElementHL* TrackElements_CollapsedPerpandicular::getLocator () const
  { return new Locator(_segment); }


  string  TrackElements_CollapsedPerpandicular::Locator::_getString () const
  {
    string s = "<TrackElements_CollapsedPerpandicular::Locator>";
    return s;
  }


  string  TrackElements_CollapsedPerpandicular::_getString () const
  {
    string s = "<TrackElements_CollapsedPerpandicular "
             + getString(_segment)
             + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "TrackElements_UniqCanonical".


  TrackElementHF* TrackElements_UniqCanonical::getClone () const
  { return new TrackElements_UniqCanonical(_canonicals); }


  bool  TrackElements_UniqCanonical::accept ( TrackElement* segment ) const
  {
    if ( _canonicals.find(segment) == _canonicals.end() ) {
      _canonicals.insert ( segment );
      return true;
    }

    return false;
  }


  string  TrackElements_UniqCanonical::_getString () const
  { return "<TrackElements_UniqCanonical>"; }


} // End of Kite namespace.
