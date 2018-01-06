// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/AutoContactHTee.h"                  |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_AUTOCONTACT_HTEE_H
#define  ANABATIC_AUTOCONTACT_HTEE_H

#include  "anabatic/AutoContact.h"


namespace Anabatic {

  class AutoHorizontal;
  class AutoVertical;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoContactHTee".


  class AutoContactHTee : public AutoContact {
      typedef  AutoContact  Super;
      friend class AutoContact;
    public:
      static  AutoContactHTee* create                 ( GCell*, Net*, const Layer* );
    protected:
    // Constructors & Destructors.
                               AutoContactHTee        ( GCell*, Contact* );
      virtual                 ~AutoContactHTee        ();
      virtual void             _invalidate            ( Flags flags );
    public:
      virtual AutoHorizontal*  getHorizontal1         () const;
      virtual AutoHorizontal*  getHorizontal2         () const;
      virtual AutoVertical*    getVertical1           () const;
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
                               AutoContactHTee        ( const AutoContactHTee& );
              AutoContactHTee& operator=              ( const AutoContactHTee& );
    private:
      AutoHorizontal* _horizontal1;
      AutoHorizontal* _horizontal2;
      AutoVertical*   _vertical1;
  };
  

} // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::AutoContactHTee);


#endif  // ANABATIC_AUTOCONTACT_HTEE_H
