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
namespace Hurricane {
  class Segment;
}
#include "anabatic/Constants.h"
#include "anabatic/Edges.h"



namespace Anabatic {

  using std::string;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Interval;
  using Hurricane::Box;
  using Hurricane::Segment;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::ExtensionGo;

  class GCell;
  class AnabaticEngine;


  class Edge : public ExtensionGo {
    public:
      typedef ExtensionGo  Super;
    public:
      static  DbU::Unit    unity;
    public:
      static        Edge*             create               ( GCell* source, GCell* target, Flags flags=Flags::NoFlags );
      virtual       void              destroy              ();
    public:                                    
      inline        bool              isVertical           () const;
      inline        bool              isHorizontal         () const;
      inline        bool              hasNet               ( const Net* ) const;
      inline        unsigned int      getCapacity          () const;
      inline        unsigned int      getRealOccupancy     () const;
      inline        unsigned int      getEstimateOccupancy () const;
      inline        float             getHistoricCost      () const;
                    DbU::Unit         getDistance          () const;
      inline        GCell*            getSource            () const;
      inline        GCell*            getTarget            () const;
                    GCell*            getOpposite          ( const GCell* ) const;
                    AnabaticEngine*   getAnabatic          () const;
      inline        DbU::Unit         getAxis              () const;
                    DbU::Unit         getAxisMin           () const;
                    Interval          getSide              () const;
                    Segment*          getSegment           ( const Net* ) const;
      inline const  vector<Segment*>& getSegments          () const;
      inline        void              incCapacity          ( int );
                    void              incRealOccupancy     ( int );
      inline        void              setHistoricCost      ( float );
                    void              add                  ( Segment* );
                    void              remove               ( Segment* );
                    void              replace              ( Segment* orig, Segment* repl );
      inline const  Flags&            flags                () const;
      inline        Flags&            flags                ();
      inline        void              revalidate           () const;
                    void              _setSource           ( GCell* );
                    void              _setTarget           ( GCell* );
    private:                          
                    void              _invalidate          ();
                    void              _revalidate          ();
    public:                                    
    // ExtensionGo support.                    
      inline  const Name&             staticGetName        (); 
      virtual const Name&             getName              () const;
      virtual       void              translate            ( const DbU::Unit&, const DbU::Unit& );
      virtual       Box               getBoundingBox       () const;
    public:                                              
    // Inspector support.                                
      virtual       string            _getTypeName         () const;
      virtual       string            _getString           () const;
      virtual       Record*           _getRecord           () const;
    protected:                                            
                                      Edge                 ( GCell* source, GCell* target, Flags flags );
      virtual                        ~Edge                 ();
      virtual       void              _postCreate          ();
      virtual       void              _preDestroy          ();
    private:                          
                                      Edge                 ( const Edge& );
                    Edge&             operator=            ( const Edge& );
    private:
      static  Name              _extensionName;
              Flags             _flags;
              unsigned int      _capacity;
              unsigned int      _realOccupancy;
              float             _estimateOccupancy;
              float             _historicCost;
              GCell*            _source;
              GCell*            _target;
              DbU::Unit         _axis;
              vector<Segment*>  _segments;  
  };


  inline const Name&             Edge::staticGetName        () { return _extensionName; }
  inline       bool              Edge::isVertical           () const { return _flags.isset(Flags::Vertical); }
  inline       bool              Edge::isHorizontal         () const { return _flags.isset(Flags::Horizontal); }
  inline       bool              Edge::hasNet               ( const Net* owner ) const { return getSegment(owner); }
  inline       unsigned int      Edge::getCapacity          () const { return _capacity; }
  inline       unsigned int      Edge::getRealOccupancy     () const { return _realOccupancy; }
  inline       unsigned int      Edge::getEstimateOccupancy () const { return _estimateOccupancy; }
  inline       float             Edge::getHistoricCost      () const { return _historicCost; }
  inline       GCell*            Edge::getSource            () const { return _source; }
  inline       GCell*            Edge::getTarget            () const { return _target; }
  inline       DbU::Unit         Edge::getAxis              () const { return _axis; }
  inline const vector<Segment*>& Edge::getSegments          () const { return _segments; }
  inline       void              Edge::incCapacity          ( int delta ) { _capacity = ((int)_capacity+delta > 0) ? _capacity+delta : 0; }
  inline       void              Edge::setHistoricCost      ( float hcost ) { _historicCost = hcost; }
  inline const Flags&            Edge::flags                () const { return _flags; }
  inline       Flags&            Edge::flags                () { return _flags; }
  inline       void              Edge::revalidate           () const { /*if (_flags&Flags::Invalidated)*/ const_cast<Edge*>(this)->_revalidate(); }


}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::Edge);

#endif  // ANABATIC_EDGE_H
