
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackElements.cpp"                      |
// +-----------------------------------------------------------------+


#include  "hurricane/Bug.h"
#include  "hurricane/Interval.h"
#include  "katana/Session.h"
#include  "katana/TrackElement.h"


namespace Katana {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::Interval;
  using Hurricane::Bug;


// -------------------------------------------------------------------
// Class  :  "TrackElements_Perpandiculars".


  TrackElements_Perpandiculars::Locator::Locator ( TrackElement* segment, Flags flags )
    : TrackElementHL ()
    , _locator       (segment->base(),flags)
    , _element       (NULL)
  {
    cdebug_log(155,0) << "TrackElements_Perpandiculars::Locator::Locator()" << endl;
    cdebug_log(155,0) << "  " << segment << endl;

    Interval bounds;
    if ( _locator.isValid() ) {
      _element = Session::lookup( _locator.getElement()->getCanonical(bounds)->base() );
      if ( !_element ) {
        cerr << Bug("Canonical segment without TrackElement.") << endl;
        progress ();
      }
    }
  }


  TrackElement* TrackElements_Perpandiculars::Locator::getElement () const
  { return _element; }


  void  TrackElements_Perpandiculars::Locator::progress ()
  {
    cdebug_log(155,0) << "TrackElements_Perpandiculars::Locator::progress()" << endl;

    Interval bounds;
    while ( _locator.isValid() ) {
      _locator.progress ();

      if ( _locator.isValid() ) {
        _element = Session::lookup( _locator.getElement()->getCanonical(bounds)->base() );
        if ( !_element ) {
          cerr << Bug("Canonical segment whithout TrackElement.") << endl;
          continue;
        }

        break;
      }
    }
  }


  TrackElementHL* TrackElements_Perpandiculars::Locator::getClone () const
  { return new Locator(*this); }


  bool  TrackElements_Perpandiculars::Locator::isValid () const
  { return _locator.isValid(); }


  TrackElementHC* TrackElements_Perpandiculars::getClone () const
  { return new TrackElements_Perpandiculars(*this); }


  TrackElementHL* TrackElements_Perpandiculars::getLocator () const
  { return new Locator(_segment,_flags); }


  string  TrackElements_Perpandiculars::Locator::_getString () const
  {
    string s = "<TrackElements_Perpandiculars::Locator>";
    return s;
  }


  string  TrackElements_Perpandiculars::_getString () const
  {
    string s = "<TrackElements_Perpandiculars "
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
    if (_canonicals.find(segment) == _canonicals.end()) {
      _canonicals.insert( segment );
      return true;
    }

    return false;
  }


  string  TrackElements_UniqCanonical::_getString () const
  { return "<TrackElements_UniqCanonical>"; }


} // Katana namespace.
