// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/AutoContactVTee.h"                  |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_AUTOCONTACT_VTEE_H
#define  ANABATIC_AUTOCONTACT_VTEE_H

#include  "anabatic/AutoContact.h"


namespace Anabatic {


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoContactVTee".


  class AutoContactVTee : public AutoContact {
      friend class AutoContact;
    public:
      static  AutoContactVTee* create                 ( GCell*, Net*, const Layer* );
    protected:
    // Constructors & Destructors.
                               AutoContactVTee        ( GCell*, Contact* );
      virtual                 ~AutoContactVTee        ();
      virtual void             _invalidate            ( unsigned int flags );
    public:
      inline  AutoHorizontal*  getHorizontal1         () const;
      inline  AutoVertical*    getVertical1           () const;
      inline  AutoVertical*    getVertical2           () const;
      virtual AutoSegment*     getOpposite            ( const AutoSegment* ) const;
      virtual AutoSegment*     getPerpandicular       ( const AutoSegment* ) const;
      virtual AutoSegment*     getSegment             ( unsigned int ) const;
      virtual void             updateGeometry         ();
      virtual void             updateTopology         ();
      virtual void             cacheDetach            ( AutoSegment* );
      virtual void             cacheAttach            ( AutoSegment* );
      virtual void             updateCache            ();
      virtual string           _getTypeName           () const;
    private:                               
                               AutoContactVTee        ( const AutoContactVTee& );
              AutoContactVTee& operator=              ( const AutoContactVTee& );
    private:
      AutoHorizontal* _horizontal1;
      AutoVertical*   _vertical1;
      AutoVertical*   _vertical2;
  };


  inline  AutoHorizontal*  AutoContactVTee::getHorizontal1 () const { return _horizontal1; };
  inline  AutoVertical*    AutoContactVTee::getVertical1   () const { return _vertical1; };
  inline  AutoVertical*    AutoContactVTee::getVertical2   () const { return _vertical2; };


} // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::AutoContactVTee);


#endif  // ANABATIC_AUTOCONTACT_VTEE_H
