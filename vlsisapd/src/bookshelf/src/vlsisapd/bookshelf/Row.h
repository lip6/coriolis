// -*- C++ -*-
//
// This file is part of the VLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./bookshelf/Row.h"                        |
// +-----------------------------------------------------------------+


#ifndef  VLSISAPD_BOOKSHELF_ROW_H
#define  VLSISAPD_BOOKSHELF_ROW_H

#include  <string>
#include  <iostream>
#include  "vlsisapd/bookshelf/Node.h"


namespace Bookshelf {


  class Row {
    public:
      inline                        Row             ();
      inline double                 getCoordinate   () const;
      inline double                 getHeight       () const;
      inline double                 getSitewidth    () const;
      inline double                 getSitespacing  () const;
      inline unsigned int           getSitesymmetry () const;
      inline unsigned int           getSiteorient   () const;
      inline double                 getSubrowOrigin () const;
      inline double                 getNumsites     () const;
      inline void                   setCoordinate   ( double );
      inline void                   setHeight       ( double );
      inline void                   setSitewidth    ( double );
      inline void                   setSitespacing  ( double );
      inline void                   setSitesymmetry ( unsigned int );
      inline void                   setSiteorient   ( unsigned int );
      inline void                   setSubrowOrigin ( double );
      inline void                   setNumsites     ( double );
             void                   writeToStream   ( std::ostream& );
    private:
      double        _coordinate;
      double        _height;
      double        _sitewidth;
      double        _sitespacing;
      unsigned int  _sitesymmetry;
      unsigned int  _siteorient;
      double        _subrowOrigin;
      double        _numsites;
  };


  inline Row::Row ()
    : _coordinate  (0.0)
    , _height      (0.0)
    , _sitewidth   (0.0)
    , _sitespacing (0.0)
    , _sitesymmetry(Symmetry::Disabled)
    , _siteorient  (Orientation::Disabled)
    , _subrowOrigin(0.0)
    , _numsites    (0.0)
  { }

  inline double        Row::getCoordinate   () const { return _coordinate; }
  inline double        Row::getHeight       () const { return _height; }
  inline double        Row::getSitewidth    () const { return _sitewidth; }
  inline double        Row::getSitespacing  () const { return _sitespacing; }
  inline unsigned int  Row::getSitesymmetry () const { return _sitesymmetry; }
  inline unsigned int  Row::getSiteorient   () const { return _siteorient; }
  inline double        Row::getSubrowOrigin () const { return _subrowOrigin; }
  inline double        Row::getNumsites     () const { return _numsites; }

  inline void          Row::setCoordinate   (double value) { _coordinate=value; }
  inline void          Row::setHeight       (double value) { _height=value; }
  inline void          Row::setSitewidth    (double value) { _sitewidth=value; }
  inline void          Row::setSitespacing  (double value) { _sitespacing=value; }
  inline void          Row::setSitesymmetry (unsigned int value) { _sitesymmetry=value; }
  inline void          Row::setSiteorient   (unsigned int value) { _siteorient=value; }
  inline void          Row::setSubrowOrigin (double value) { _subrowOrigin=value; }
  inline void          Row::setNumsites     (double value) { _numsites=value; }


} // End of Bookshelf namespace.


#endif  // __VLSISAPD_BOOKSHELF_ROW__
