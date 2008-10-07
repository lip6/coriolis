
// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ExtensionSlice.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_EXTENSION_SLICE__
#define  __HURRICANE_EXTENSION_SLICE__

#include  "hurricane/Name.h"
#include  "hurricane/ExtensionSlices.h"
#include  "hurricane/QuadTree.h"


namespace Hurricane {


  class Cell;


  class ExtensionSlice {

    public:
      typedef unsigned long long  Mask;
    public:
    // Constructor & Destructor.
      static  ExtensionSlice*     _create        ( Cell* , const Name& );
              void                _destroy       ();
    // Methods.
      inline  bool                isEmpty        () const;
      inline  Cell*               getCell        () const;
      inline  const Name&         getName        () const;
      inline  Mask                getMask        () const;
      inline  const Box&          getBoundingBox () const;
      inline  Gos                 getGos         () const;
      inline  Gos                 getGosUnder    ( const Box& area ) const;
      inline  QuadTree*           _getQuadTree   ();
    // Hurricane Managment.
              string              _getTypeName   () const;
              string              _getString     () const;
              Record*             _getRecord     () const;

    private:
    // Internal: Attributes.
      static  unsigned int        _masks;
              Cell*               _cell;
              Name                _name;
              Mask                _mask;
              QuadTree            _quadTree;

    protected:
    // Internal: Constructors & Destructors.
                                  ExtensionSlice ( Cell* , const Name&, Mask );
                                 ~ExtensionSlice ();
    private:
                                  ExtensionSlice ( const ExtensionSlice& );
              ExtensionSlice&     operator=      ( const ExtensionSlice& );
};


// Inline Functions.
  inline bool                  ExtensionSlice::isEmpty        () const { return _quadTree.isEmpty(); }
  inline Cell*                 ExtensionSlice::getCell        () const { return _cell; }
  inline const Name&           ExtensionSlice::getName        () const { return _name; }
  inline ExtensionSlice::Mask  ExtensionSlice::getMask        () const { return _mask; }
  inline const Box&            ExtensionSlice::getBoundingBox () const { return _quadTree.getBoundingBox(); }
  inline Gos                   ExtensionSlice::getGos         () const { return _quadTree.getGos(); }
  inline Gos                   ExtensionSlice::getGosUnder    ( const Box& area ) const {return _quadTree.getGosUnder(area); }
  inline QuadTree*             ExtensionSlice::_getQuadTree   () { return &_quadTree; }


} // End of Hurricane namespace.


# endif // __HURRICANE_EXTENSION_SLICE__
