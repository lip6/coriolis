
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
// |  C++ Header  :       "./AutoVertical.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KATABATIC_AUTOVERTICAL__
#define  __KATABATIC_AUTOVERTICAL__

#include  "hurricane/Vertical.h"
#include  "katabatic/AutoSegment.h"


namespace Katabatic {


// -------------------------------------------------------------------
// Class  :  "AutoVertical".

 
  class AutoVertical : public AutoSegment {
      using AutoSegment::_computeTerminal;

    public:
    // Constructors.
      static  AutoVertical* create               ( Vertical* vertical
                                                 , int       type
                                                 , bool      terminal=false
                                                 , bool      collapsed=false
                                                 );
      static  AutoVertical* create               ( AutoContact* source
                                                 , AutoContact* target
                                                 , const Layer* layer
                                                 , DbU::Unit    x
                                                 , DbU::Unit    width
                                                 , int          type
                                                 , bool         terminal=false
                                                 , bool         collapsed=false
                                                 );
    // Predicates.                               
      virtual bool          _canSlacken          () const;
      virtual bool          canDesalignate       ( AutoContact* ) const;
    // Accessors.                                
      virtual Segment*      base                 ()       { return _vertical; };
      virtual Segment*      base                 () const { return _vertical; };
      virtual Segment*      getSegment           ()       { return _vertical; };
      virtual Segment*      getSegment           () const { return _vertical; };
      virtual Vertical*     getVertical          ()       { return _vertical; };
      virtual DbU::Unit     getSourceU           () const { return _vertical->getSourceY(); };
      virtual DbU::Unit     getTargetU           () const { return _vertical->getTargetY(); };
      virtual DbU::Unit     getDuSource          () const { return _vertical->getDySource(); };
      virtual DbU::Unit     getDuTarget          () const { return _vertical->getDyTarget(); };
      virtual Interval      getSpanU             () const { return Interval(_vertical->getSourceY(),_vertical->getTargetY()); };
      virtual bool          getConstraints       ( DbU::Unit& min, DbU::Unit& max ) const;
      virtual Interval      getSourceConstraints ( bool native=false ) const;
      virtual Interval      getTargetConstraints ( bool native=false ) const;
      virtual unsigned int  getDirection         () const;
      virtual size_t        getGCells            ( vector<GCell*>& ) const;
    // Modifiers.                                
      virtual void          setDuSource          ( DbU::Unit du ) { _vertical->setDySource(du); };
      virtual void          setDuTarget          ( DbU::Unit du ) { _vertical->setDyTarget(du); };
      virtual void          alignate             ( DbU::Unit axis );
      virtual void          orient               ();
      virtual void          setPositions         ();
      virtual bool          checkPositions       () const;
      virtual bool          checkConstraints     () const;
      virtual void          _computeTerminal     ();
      virtual void          moveURight           ();
      virtual void          moveULeft            ();
      virtual void          _makeDogLeg          ( GCell*, bool upLayer );
      virtual void          desalignate          ( AutoContact* );
      virtual void          _slacken             ();
    // Inspector Management.
      virtual Record*       _getRecord         () const;
      virtual string        _getString         () const;
      virtual string        _getTypeName       () const { return "AutoVertical"; };

    protected:
    // Internal: Attributes.
              Vertical*     _vertical;

    // Constructors.
    protected:
                             AutoVertical      ( Vertical* vertical
                                               , int       type
                                               , bool      terminal
                                               , bool      collapsed );
      virtual              ~AutoVertical       ();
      virtual void          _postCreate        ();
      virtual void          _preDestroy        ();
    private:
                             AutoVertical      ( const AutoVertical& );
              AutoVertical&  operator=         ( const AutoVertical& );
  };


} // End of Katabatic namespace.


#endif  // __KATABATIC_AUTOHORIZONTAL__
