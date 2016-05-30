// -*- mode: C++; explicit-buffer-name: "Edges.h<anabatic>" -*-
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
// |  C++ Header  :  "./anabatic/Edges.h"                            |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_EDGES_H
#define  ANABATIC_EDGES_H

#include <string>
#include <vector>
#include "hurricane/Collection.h"
#include "anabatic/Constants.h"


namespace Anabatic {

  using std::string;
  using std::vector;
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
                           Locator    ( const GCell* gcell );
          inline           Locator    ( const Locator& );
          virtual Edge*    getElement () const;
          virtual EdgesHL* getClone   () const;
          virtual bool     isValid    () const;
          virtual void     progress   ();
          virtual string   _getString () const;
        protected:
          const GCell*  _gcell;
                Flags   _flags;
                size_t  _iedge;
      };

    // GCell_Edges.
    public:
      inline           GCell_Edges ( const GCell* gcell );
      inline           GCell_Edges ( const GCell_Edges& );
      virtual EdgesHC* getClone    () const;
	  virtual EdgesHL* getLocator  () const;
      virtual string  _getString   () const;
    protected:
      const GCell*  _gcell;
  };

  
  inline GCell_Edges::Locator::Locator ( const Locator &locator )
    : EdgesHL()
    , _gcell(locator._gcell)
    , _flags(locator._flags)
    , _iedge(locator._iedge)
  {
    cdebug.log(110) << "GCell_Edges::Locator::Locator(const Locator&)" << std::endl;
  }


  inline GCell_Edges::GCell_Edges ( const GCell* gcell )
    : EdgesHC()
    , _gcell(gcell)
  { }


  inline GCell_Edges::GCell_Edges ( const GCell_Edges& edges )
    : EdgesHC()
    , _gcell(edges._gcell)
  { }


}  // Anabatic namespace.


GETSTRING_POINTER_SUPPORT(Anabatic::GCell_Edges);
GETSTRING_POINTER_SUPPORT(Anabatic::GCell_Edges::Locator);
IOSTREAM_POINTER_SUPPORT(Anabatic::GCell_Edges);
IOSTREAM_POINTER_SUPPORT(Anabatic::GCell_Edges::Locator);

#endif  // ANABATIC_EDGES_H
