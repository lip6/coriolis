// -*- mode: C++; explicit-buffer-name: "Edge.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/Edge.h"                             |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_EDGE_H
#define  ANABATIC_EDGE_H

#include <string>
#include "hurricane/Name.h"
#include "hurricane/Interval.h"
#include "hurricane/Box.h"
#include "hurricane/ExtensionGo.h"
#include "anabatic/Constants.h"


namespace Anabatic {

  using std::string;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Interval;
  using Hurricane::Box;
  using Hurricane::Cell;
  using Hurricane::ExtensionGo;

  class GCell;


  class Edge : public ExtensionGo {
    public:
      typedef ExtensionGo  Super;
    public:
      static        Edge*         create               ( GCell* source, GCell* target, Flags flags=Flags::NoFlags );
      virtual       void          destroy              ();
    public:                                
      inline        unsigned int  getCapacity          () const;
      inline        unsigned int  getRealOccupancy     () const;
      inline        unsigned int  getEstimateOccupancy () const;
      inline        GCell*        getSource            () const;
      inline        GCell*        getTarget            () const;
                    GCell*        getOpposite          ( const GCell* ) const;
      inline        DbU::Unit     getAxis              () const;
                    DbU::Unit     getAxisMin           () const;
                    Interval      getSide              () const;
      inline const  Flags&        flags                () const;
      inline        Flags&        flags                ();
      inline        void          invalidate           ();
      inline        void          revalidate           () const;
                    void          _setSource           ( GCell* );
                    void          _setTarget           ( GCell* );
    private:
                    void          _revalidate          ();
    public:                                
    // ExtensionGo support.                
      inline  const Name&         staticGetName        (); 
      virtual const Name&         getName              () const;
      virtual       void          translate            ( const DbU::Unit&, const DbU::Unit& );
      virtual       Box           getBoundingBox       () const;
    public:                                            
    // Inspector support.                              
      virtual       string        _getTypeName         () const;
      virtual       string        _getString           () const;
      virtual       Record*       _getRecord           () const;
    protected:                                         
                                  Edge                 ( GCell* source, GCell* target, Flags flags );
      virtual                    ~Edge                 ();
      virtual       void          _postCreate          ();
      virtual       void          _preDestroy          ();
    private:
                                  Edge                 ( const Edge& );
                    Edge&         operator=            ( const Edge& );
    private:
      static  Name          _extensionName;
              Flags         _flags;
              unsigned int  _capacity;
              unsigned int  _realOccupancy;
              float         _estimateOccupancy;
              GCell*        _source;
              GCell*        _target;
              DbU::Unit     _axis;
  };


  inline const Name&   Edge::staticGetName        () { return _extensionName; }
  inline unsigned int  Edge::getCapacity          () const { return _capacity; }
  inline unsigned int  Edge::getRealOccupancy     () const { return _realOccupancy; }
  inline unsigned int  Edge::getEstimateOccupancy () const { return _estimateOccupancy; }
  inline GCell*        Edge::getSource            () const { return _source; }
  inline GCell*        Edge::getTarget            () const { return _target; }
  inline DbU::Unit     Edge::getAxis              () const { return _axis; }
  inline const Flags&  Edge::flags                () const { return _flags; }
  inline Flags&        Edge::flags                () { return _flags; }
  inline void          Edge::invalidate           () { _flags |= Flags::Invalidated; }
  inline void          Edge::revalidate           () const { /*if (_flags&Flags::Invalidated)*/ const_cast<Edge*>(this)->_revalidate(); }


}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::Edge);

#endif  // ANABATIC_EDGE_H
