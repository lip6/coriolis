// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        A n a b a t i c  -  Routing Toolbox                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/AutoContactTurn.h"                  |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_AUTOCONTACT_TURN_H
#define  ANABATIC_AUTOCONTACT_TURN_H

#include  "anabatic/AutoContact.h"


namespace Anabatic {

  class AutoContactTerminal;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoContactTurn".


  class AutoContactTurn : public AutoContact {
      friend class AutoContact;
    public:
      static  AutoContactTurn* create                 ( GCell*, Net*, const Layer* );
      static  void             insert                 ( AutoContactTerminal* );
    protected:
    // Constructors & Destructors.
                               AutoContactTurn        ( GCell*, Contact* );
      virtual                 ~AutoContactTurn        ();
      virtual void             _invalidate            ( Flags flags );
    public:
      virtual AutoHorizontal*  getHorizontal1         () const;
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
                               AutoContactTurn        ( const AutoContactTurn& );
              AutoContactTurn& operator=              ( const AutoContactTurn& );
    private:
      AutoHorizontal* _horizontal1;
      AutoVertical*   _vertical1;
  };

  

} // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::AutoContactTurn);


#endif  // ANABATIC_AUTOCONTACT_TURN_H
