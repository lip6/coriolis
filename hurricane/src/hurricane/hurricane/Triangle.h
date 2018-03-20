// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Triangle.h"                        |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_TRIANGLE_H
#define HURRICANE_TRIANGLE_H

#include "hurricane/Component.h"
#include "hurricane/Triangles.h"


namespace Hurricane {

  class Layer;

  
  class Triangle : public Component {
    public:
      typedef       Component  Super;
    public:
      static        Triangle*   create         ( Net*, const Layer*, const Point&, const Point&, const Point& );
    public:                     
      virtual       DbU::Unit   getX           () const;
      virtual       DbU::Unit   getY           () const;
      inline  const Point&      getPoint       ( size_t ) const;
      virtual       Box         getBoundingBox () const;
      virtual       Box         getBoundingBox ( const BasicLayer* ) const;
      virtual const Layer*      getLayer       () const;
                    void        setLayer       ( const Layer* layer );
      virtual       void        translate      ( const DbU::Unit& dx, const DbU::Unit& dy );
                    void        setPoints      ( const Point&, const Point&, const Point& );
      virtual       void        _toJson        ( JsonWriter* ) const;
      static        JsonObject* getJsonObject  ( unsigned long flags );
      virtual       string      _getTypeName   () const;
      virtual       string      _getString     () const;
      virtual       Record*     _getRecord     () const;
    protected:
                                Triangle       ( Net*, const Layer*, const Point&, const Point&, const Point& );
    private:
              const Layer*               _layer;
                    std::array<Point,3>  _points;
  };


  inline  const Point& Triangle::getPoint ( size_t i ) const { return _points[ (i<3) ? i : 0 ]; }


  class JsonTriangle : public JsonComponent {
    public:
      static  void          initialize   ();
                            JsonTriangle ( unsigned long flags );
      virtual string        getTypeName  () const;
      virtual JsonTriangle* clone        ( unsigned long ) const;
      virtual void          toData       ( JsonStack& ); 
  };


} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Triangle);

#endif // HURRICANE_TRIANGLE_H
