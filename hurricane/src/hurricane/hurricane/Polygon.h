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
// |  C++ Header  :  "./hurricane/Polygon.h"                         |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_POLYGON_H
#define HURRICANE_POLYGON_H

#include "hurricane/Component.h"
#include "hurricane/Polygons.h"


namespace Hurricane {

  class Layer;

  
  class Polygon : public Component {
    public:
      typedef       Component  Super;
    public:
      static const  uint32_t Above      = (1<<0);
      static const  uint32_t YSteps     = (1<<1);
      static const  uint32_t XSteps     = (1<<2);
      static const  uint32_t XIncrease  = (1<<3);
      static const  uint32_t YIncrease  = (1<<4);
      static const  uint32_t Horizontal = (1<<5);
      static const  uint32_t Vertical   = (1<<6);

    public:
      class Edge {
        public:
                         Edge            ( Point origin, Point extremity, uint32_t flags );
          inline size_t  getSize         () const;
                 Point   getPoint        ( size_t i ) const;
          inline bool    isPositiveSlope () const;
          inline bool    isYIncrease     () const;
          inline bool    isXIncrease     () const;
          inline bool    isHV            () const;
          inline bool    isXSteps        () const;
          inline bool    isYSteps        () const;
                 void    translate       ( DbU::Unit dx, DbU::Unit dy );
                 string  _getTypeName    () const;
                 string  _getString      () const;
                 Record* _getRecord      () const;
        private:
          uint32_t           _flags;
          DbU::Unit          _xyOrigin;
          vector<DbU::Unit>  _steps;
      };

    public:
      class Points_Manhattan : public PointHC {
        public:
          class Locator : public PointHL {
            public:
                               Locator    ( const Polygon* );
              inline           Locator    ( const Locator& );
              virtual Point    getElement () const;
              virtual PointHL* getClone   () const;
              virtual bool     isValid    () const;
              virtual void     progress   ();
              virtual string   _getString () const;
            protected:
              const Polygon* _polygon;
                    size_t    _iEdge;
                    size_t    _iPoint;
          };
        public:
          inline           Points_Manhattan ( const Polygon* );
          inline           Points_Manhattan ( const Points_Manhattan& );
          virtual PointHC* getClone         () const;
    	  virtual PointHL* getLocator       () const;
          virtual string   _getString       () const;
        protected:
          const Polygon*  _polygon;
      };
          
    public:
      static        Polygon*       create          ( Net*, const Layer*, const std::vector<Point>& );
      static        float          getSlope        ( const Point&, const Point& );
    public:                                        
      virtual       bool           isNonRectangle  () const;
      virtual       bool           isManhattanized () const;
      virtual       DbU::Unit      getX            () const;
      virtual       DbU::Unit      getY            () const;
      inline  const vector<Point>& getPoints       () const;
      inline  const vector<Edge*>& getEdges        () const;
      virtual       size_t         getPointsSize   () const;
      virtual       Point          getPoint        ( size_t ) const;
      virtual       Box            getBoundingBox  () const;
      virtual       Box            getBoundingBox  ( const BasicLayer* ) const;
                    void           getSubPolygons  ( vector< vector<Point> >& ) const;
      virtual const Layer*         getLayer        () const;
                    void           setLayer        ( const Layer* layer );
      virtual       void           translate       ( const DbU::Unit& dx, const DbU::Unit& dy );
                    void           setPoints       ( const vector<Point>& );
      static        float          getSign         ( const vector<Point>&, size_t );
                    float          getSlope        ( size_t i ) const;
                    void           manhattanize    ();
      virtual       Points         getMContour     () const;
      virtual       void           _toJson         ( JsonWriter* ) const;
      static        JsonObject*    getJsonObject   ( unsigned long flags );
      virtual       string         _getTypeName    () const;
      virtual       string         _getString      () const;
      virtual       Record*        _getRecord      () const;
    protected:                                     
                                   Polygon        ( Net*, const Layer*, const std::vector<Point>& );
                                  ~Polygon        ();
    private:
              const Layer*              _layer;
                    std::vector<Point>  _points;
                    std::vector<Edge*>  _edges;
  };


  inline  const vector<Polygon::Edge*>& Polygon::getEdges        () const { return _edges; }
  inline  const vector<Point>&          Polygon::getPoints       () const { return _points; }

  inline bool    Polygon::Edge::isYIncrease     () const { return (_flags & Polygon::YIncrease); }
  inline bool    Polygon::Edge::isXIncrease     () const { return (_flags & Polygon::XIncrease); }
  inline bool    Polygon::Edge::isPositiveSlope () const { return not ( (_flags & Polygon::XIncrease) xor (_flags & Polygon::YIncrease) ); }
  inline bool    Polygon::Edge::isHV            () const { return (_flags & (Polygon::Horizontal|Polygon::Vertical)); }
  inline bool    Polygon::Edge::isXSteps        () const { return (_flags & Polygon::XSteps); }
  inline bool    Polygon::Edge::isYSteps        () const { return (_flags & Polygon::YSteps); }
  inline size_t  Polygon::Edge::getSize         () const { if (isHV() or (_steps.size() < 2)) return 1; return (_steps.size() - 1)*2; }

  
  inline Polygon::Points_Manhattan::Locator::Locator ( const Locator &locator )
    : PointHL ()
    , _polygon(locator._polygon)
    , _iEdge  (locator._iEdge)
    , _iPoint (locator._iPoint)
  { }


  inline Polygon::Points_Manhattan::Points_Manhattan ( const Polygon* polygon )
    : PointHC ()
    , _polygon(polygon)
  { }


  inline Polygon::Points_Manhattan::Points_Manhattan ( const Points_Manhattan& other )
    : PointHC()
    , _polygon(other._polygon)
  { }


  class JsonPolygon : public JsonComponent {
    public:
      static  void         initialize  ();
                           JsonPolygon ( unsigned long flags );
      virtual string       getTypeName () const;
      virtual JsonPolygon* clone       ( unsigned long ) const;
      virtual void         toData      ( JsonStack& ); 
  };


} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Polygon::Edge);
INSPECTOR_P_SUPPORT(Hurricane::Polygon);

#endif // HURRICANE_POLYGON_H
