
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./AutoHorizontal.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KATABATIC_AUTOHORIZONTAL__
#define  __KATABATIC_AUTOHORIZONTAL__

#include  "hurricane/Horizontal.h"
#include  "katabatic/AutoSegment.h"


namespace Katabatic {


// -------------------------------------------------------------------
// Class  :  "AutoHorizontal".

 
  class AutoHorizontal : public AutoSegment {
      using AutoSegment::_computeTerminal;

    public:
    // Constructors.
      static  AutoHorizontal* create               ( Horizontal* horizontal
                                                   , int         type
                                                   , bool        terminal=false
                                                   , bool        collapsed=false
                                                   );
      static  AutoHorizontal* create               ( AutoContact* source
                                                   , AutoContact* target
                                                   , const Layer* layer
                                                   , DbU::Unit    y
                                                   , DbU::Unit    width
                                                   , int          type
                                                   , bool         terminal=false
                                                   , bool         collapsed=false
                                                   );
    // Predicates.                                 
      virtual bool            canDesalignate       ( AutoContact* ) const;
      virtual bool            _canSlacken          () const;
    // Accessors.                                  
      virtual Segment*        base                 ()       { return _horizontal; };
      virtual Segment*        base                 () const { return _horizontal; };
      virtual Segment*        getSegment           ()       { return _horizontal; };
      virtual Segment*        getSegment           () const { return _horizontal; };
      virtual Horizontal*     getHorizontal        ()       { return _horizontal; };
      virtual DbU::Unit       getSourceU           () const { return _horizontal->getSourceX(); };
      virtual DbU::Unit       getTargetU           () const { return _horizontal->getTargetX(); };
      virtual DbU::Unit       getDuSource          () const { return _horizontal->getDxSource(); };
      virtual DbU::Unit       getDuTarget          () const { return _horizontal->getDxTarget(); };
      virtual Interval        getSpanU             () const { return Interval(_horizontal->getSourceX(),_horizontal->getTargetX()); };
      virtual bool            getConstraints       ( DbU::Unit& min , DbU::Unit& max ) const;
      virtual Interval        getSourceConstraints ( bool native=false ) const;
      virtual Interval        getTargetConstraints ( bool native=false ) const;
      virtual unsigned int    getDirection         () const;
      virtual size_t          getGCells            ( vector<GCell*>& ) const;
    // Modifiers.                                  
      virtual void            setDuSource          ( DbU::Unit du ) { _horizontal->setDxSource(du); };
      virtual void            setDuTarget          ( DbU::Unit du ) { _horizontal->setDxTarget(du); };
      virtual void            alignate             ( DbU::Unit axis );
      virtual void            orient               ();
      virtual void            setPositions         ();
      virtual bool            checkPositions       () const;
      virtual bool            checkConstraints     () const;
      virtual void            _computeTerminal     ();
      virtual void            moveURight           ();
      virtual void            moveULeft            ();
      virtual void            _makeDogLeg          ( GCell*, bool upLayer );
      virtual void            desalignate          ( AutoContact* );
      virtual void            _slacken             ();
    // Inspector Management.                       
      virtual Record*         _getRecord           () const;
      virtual string          _getString           () const;
      virtual string          _getTypeName         () const { return "AutoHorizontal"; };

    // Internal: Attributes.
    protected:
              Horizontal*     _horizontal;

    // Internal: Constructors.
    protected:
                              AutoHorizontal         ( Horizontal* horizontal
                                                     , int         type
                                                     , bool        terminal
                                                     , bool        collapsed );
      virtual                ~AutoHorizontal         ();
      virtual void            _postCreate            ();
      virtual void            _preDestroy             ();
    private:
                              AutoHorizontal         ( const AutoHorizontal& );
              AutoHorizontal& operator=              ( const AutoHorizontal& );
  };


} // End of Katabatic namespace.


#endif  // __KATABATIC_AUTOHORIZONTAL__
