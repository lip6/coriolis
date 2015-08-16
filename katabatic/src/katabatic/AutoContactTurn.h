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
// |  C++ Header  :  "./katabatic/AutoContactTurn.h"                 |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_AUTOCONTACT_TURN_H
#define  KATABATIC_AUTOCONTACT_TURN_H

#include  "katabatic/AutoContact.h"


namespace Katabatic {

  class AutoContactTerminal;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContactTurn".


  class AutoContactTurn : public AutoContact {
      friend class AutoContact;
    public:
      static  AutoContactTurn* create                 ( GCell*, Net*, const Layer* );
      static  void             insert                 ( AutoContactTerminal* );
    protected:
    // Constructors & Destructors.
                               AutoContactTurn        ( GCell*, Contact* );
      virtual                 ~AutoContactTurn        ();
      virtual void             _invalidate            ( unsigned int flags );
    public:
      inline  AutoHorizontal*  getHorizontal1         () const;
      inline  AutoVertical*    getVertical1           () const;
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


  inline  AutoHorizontal*  AutoContactTurn::getHorizontal1 () const { return _horizontal1; };
  inline  AutoVertical*    AutoContactTurn::getVertical1   () const { return _vertical1; };

  

} // Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::AutoContactTurn);


#endif  // KATABATIC_AUTOCONTACT_TURN_H
