// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/AutoContactHTee.h"                 |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_AUTOCONTACT_HTEE_H
#define  KATABATIC_AUTOCONTACT_HTEE_H

#include  "katabatic/AutoContact.h"


namespace Katabatic {

  class AutoHorizontal;
  class AutoVertical;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContactHTee".


  class AutoContactHTee : public AutoContact {
      friend class AutoContact;
    public:
      static  AutoContactHTee* create                 ( GCell*, Net*, const Layer* );
    protected:
    // Constructors & Destructors.
                               AutoContactHTee        ( GCell*, Contact* );
      virtual                 ~AutoContactHTee        ();
      virtual void             _invalidate            ( unsigned int flags );
    public:
      inline  AutoHorizontal*  getHorizontal1         () const;
      inline  AutoHorizontal*  getHorizontal2         () const;
      inline  AutoVertical*    getVertical1           () const;
      virtual AutoSegment*     getOpposite            ( const AutoSegment* ) const;
      virtual AutoSegment*     getSegment             ( unsigned int ) const;
      virtual void             updateGeometry         ();
      virtual void             updateTopology         ();
      virtual void             cacheDetach            ( AutoSegment* );
      virtual void             cacheAttach            ( AutoSegment* );
      virtual void             updateCache            ();
      virtual string           _getTypeName           () const;
    private:                               
                               AutoContactHTee        ( const AutoContactHTee& );
              AutoContactHTee& operator=              ( const AutoContactHTee& );
    private:
      AutoHorizontal* _horizontal1;
      AutoHorizontal* _horizontal2;
      AutoVertical*   _vertical1;
  };


  inline  AutoHorizontal*  AutoContactHTee::getHorizontal1 () const { return _horizontal1; };
  inline  AutoHorizontal*  AutoContactHTee::getHorizontal2 () const { return _horizontal2; };
  inline  AutoVertical*    AutoContactHTee::getVertical1   () const { return _vertical1; };
  

} // Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::AutoContactHTee);


#endif  // KATABATIC_AUTOCONTACT_HTEE_H
