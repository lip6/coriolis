// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/AutoContactTerminal.h"             |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_AUTOCONTACT_TERMINAL_H
#define  KATABATIC_AUTOCONTACT_TERMINAL_H

#include  "katabatic/AutoContact.h"


namespace Katabatic {


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContactTerminal".


  class AutoContactTerminal : public AutoContact {
      friend class AutoContact;
    public:
      static  AutoContactTerminal* create                 ( GCell*       gcell
                                                          , Component*   anchor
                                                          , const Layer* layer
                                                          , Point        point
                                                          , DbU::Unit    width
                                                          , DbU::Unit    height
                                                          );
      static  AutoContactTerminal* create                 (       GCell*     gcell
                                                          ,       Component* anchor
                                                          , const Layer*     layer
                                                          , const DbU::Unit  dx
                                                          , const DbU::Unit  dy
                                                          , const DbU::Unit  width
                                                          , const DbU::Unit  height
                                                          );
    protected:
    // Constructors & Destructors.
                                   AutoContactTerminal    ( GCell*, Contact* );
      virtual                     ~AutoContactTerminal    ();
      virtual void                 _invalidate            ( unsigned int flags );
    public:
      virtual Box                  getNativeConstraintBox () const;
      inline  AutoSegment*         getSegment             () const;
      virtual AutoSegment*         getSegment             ( unsigned int ) const;
      virtual AutoSegment*         getOpposite            ( const AutoSegment* ) const;
      virtual void                 updateGeometry         ();
      virtual void                 updateTopology         ();
      virtual void                 cacheDetach            ( AutoSegment* );
      virtual void                 cacheAttach            ( AutoSegment* );
      virtual void                 updateCache            ();
      virtual string               _getTypeName           () const;
    private:                               
                                   AutoContactTerminal    ( const AutoContactTerminal& );
              AutoContactTerminal& operator=              ( const AutoContactTerminal& );
    protected:
      AutoSegment* _segment;
  };


  inline AutoSegment* AutoContactTerminal::getSegment () const { return _segment; }
  

} // Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::AutoContactTerminal);


#endif  // KATABATIC_AUTOCONTACT_TERMINAL_H
