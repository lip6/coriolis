// -*- mode: C++; explicit-buffer-name: "Edges.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/Edges.h"                            |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_EDGES_H
#define  ANABATIC_EDGES_H

#include <string>
#include <vector>
#include "hurricane/DbU.h"
#include "hurricane/Collection.h"
#include "anabatic/Constants.h"


namespace Anabatic {

  using std::string;
  using std::vector;
  using Hurricane::DbU;
  using Hurricane::Record;
  using Hurricane::Filter;
  using Hurricane::Locator;
  using Hurricane::Collection;
  using Hurricane::GenericFilter;
  using Hurricane::GenericLocator;
  using Hurricane::GenericCollection;
  class Edge;
  class GCell;


// -------------------------------------------------------------------
// Collections.

  typedef Hurricane::Filter<Edge*>     EdgesHF;
  typedef Hurricane::Locator<Edge*>    EdgesHL;
  typedef Hurricane::Collection<Edge*> EdgesHC;
  typedef GenericCollection<Edge*>     Edges;
  typedef GenericLocator<Edge*>        EdgesLocator;
  typedef GenericFilter<Edge*>         EdgesFilter;
  class GCell;


// -------------------------------------------------------------------
// Class  :  "GCell_Edges".

  class GCell_Edges : public EdgesHC {
    public:
    // Sub-Class: Locator.
      class Locator : public EdgesHL {
        public:
                           Locator    ( const GCell* gcell, Flags filterFlags );
          inline           Locator    ( const Locator& );
          virtual Edge*    getElement () const;
          virtual EdgesHL* getClone   () const;
          virtual bool     isValid    () const;
          virtual void     progress   ();
          virtual string   _getString () const;
        protected:
          const GCell*  _gcell;
                Flags   _stateFlags;
                Flags   _filterFlags;
                size_t  _iedge;
      };

    // GCell_Edges.
    public:
      inline           GCell_Edges ( const GCell* gcell, Flags filterFlags=Flags::AllSides );
      inline           GCell_Edges ( const GCell_Edges& );
      virtual EdgesHC* getClone    () const;
	  virtual EdgesHL* getLocator  () const;
      virtual string  _getString   () const;
    protected:
      const GCell*  _gcell;
            Flags   _filterFlags;
  };

  
  inline GCell_Edges::Locator::Locator ( const Locator &locator )
    : EdgesHL()
    , _gcell      (locator._gcell)
    , _stateFlags (locator._stateFlags)
    , _filterFlags(locator._filterFlags)
    , _iedge      (locator._iedge)
  {
    // cdebug_log(110,0) << "GCell_Edges::Locator::Locator(const Locator&)" << std::endl;
  }


  inline GCell_Edges::GCell_Edges ( const GCell* gcell, Flags filterFlags )
    : EdgesHC()
    , _gcell      (gcell)
    , _filterFlags(filterFlags)
  { }


  inline GCell_Edges::GCell_Edges ( const GCell_Edges& edges )
    : EdgesHC()
    , _gcell      (edges._gcell)
    , _filterFlags(edges._filterFlags)
  { }


// -------------------------------------------------------------------
// Class  :  "Path_Edges".

  class Path_Edges : public EdgesHC {
    public:
    // Sub-Class: Locator.
      class Locator : public EdgesHL {
        public:
                           Locator    ( const GCell* source, const GCell* target, Flags pathFlags );
          inline           Locator    ( const Locator& );
          virtual Edge*    getElement () const;
          virtual EdgesHL* getClone   () const;
          virtual bool     isValid    () const;
          virtual void     progress   ();
          virtual string   _getString () const;
        protected:
          const GCell*     _source;
          const GCell*     _target;
                Flags      _stateFlags;
                DbU::Unit  _uprobe;
                Edge*      _edge;
      };

    // GCell_Edges.
    public:
      inline           Path_Edges  ( const GCell* source, const GCell* target, Flags pathFlags=Flags::NorthPath );
      inline           Path_Edges  ( const Path_Edges& );
      virtual EdgesHC* getClone    () const;
	  virtual EdgesHL* getLocator  () const;
      virtual string  _getString   () const;
    protected:
      const GCell*  _source;
      const GCell*  _target;
            Flags   _pathFlags;
  };

  
  inline Path_Edges::Locator::Locator ( const Locator &locator )
    : EdgesHL()
    , _source    (locator._source)
    , _target    (locator._target)
    , _stateFlags(locator._stateFlags)
    , _uprobe    (locator._uprobe)
    , _edge      (locator._edge)
  {
    // cdebug_log(110,0) << "GCell_Edges::Locator::Locator(const Locator&)" << std::endl;
  }


  inline Path_Edges::Path_Edges ( const GCell* source, const GCell* target, Flags pathFlags )
    : EdgesHC()
    , _source   (source)
    , _target   (target)
    , _pathFlags(pathFlags)
  { }


  inline Path_Edges::Path_Edges ( const Path_Edges& path )
    : EdgesHC()
    , _source   (path._source)
    , _target   (path._target)
    , _pathFlags(path._pathFlags)
  { }


}  // Anabatic namespace.


GETSTRING_POINTER_SUPPORT(Anabatic::GCell_Edges);
GETSTRING_POINTER_SUPPORT(Anabatic::GCell_Edges::Locator);
GETSTRING_POINTER_SUPPORT(Anabatic::Path_Edges);
GETSTRING_POINTER_SUPPORT(Anabatic::Path_Edges::Locator);

IOSTREAM_POINTER_SUPPORT(Anabatic::GCell_Edges);
IOSTREAM_POINTER_SUPPORT(Anabatic::GCell_Edges::Locator);
IOSTREAM_POINTER_SUPPORT(Anabatic::Path_Edges);
IOSTREAM_POINTER_SUPPORT(Anabatic::Path_Edges::Locator);

#endif  // ANABATIC_EDGES_H
