// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2018-2023, All Rights Reserved
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
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Rectilinear.h"                     |
// +-----------------------------------------------------------------+

#pragma  once
#include "hurricane/Component.h"


namespace Hurricane {

  class Layer;


// -------------------------------------------------------------------
// Class  :  "Rectilinear".

  class Rectilinear : public Component {
    public:
      typedef Component Super;
      static const uint32_t IsRectilinear = (1<<0);
  
    public:
      static        Rectilinear*   create            ( Net*, const Layer*, const vector<Point>& );
    // Accessors.                  
      virtual       bool           isNonRectangle    () const;
      inline        bool           isRectilinear     () const;
      virtual       DbU::Unit      getX              () const;
      virtual       DbU::Unit      getY              () const;
      virtual       Box            getBoundingBox    () const;
      virtual       Box            getBoundingBox    ( const BasicLayer* ) const;
      virtual       size_t         getPointsSize     () const;
      virtual       Point          getPoint          ( size_t i ) const;
      virtual const Layer*         getLayer          () const;
      inline        Points         getContour        () const;
                    bool           getAsRectangles   ( std::vector<Box>& ) const;
      inline  const vector<Point>& getPoints         () const;
                    Box            getNearestHSide   ( DbU::Unit y ) const;
    // Mutators.
                    void           setLayer          ( const Layer* );
      virtual       void           translate         ( const DbU::Unit& dx, const DbU::Unit& dy );
                    void           setPoints         ( const vector<Point>& );
    // Hurricane management.       
      virtual       void           _toJson           ( JsonWriter* ) const;
      static        JsonObject*    getJsonObject     ( unsigned long flags );
      virtual       string         _getTypeName      () const;
      virtual       string         _getString        () const;
      virtual       Record*        _getRecord        () const;
    protected:                               
                                   Rectilinear       ( Net*, const Layer*, const vector<Point>& );
    private:
      const Layer*         _layer;
            vector<Point>  _points;
            uint32_t       _flags;
  };

  
  inline        bool           Rectilinear::isRectilinear () const { return _flags & IsRectilinear; }
  inline        Points         Rectilinear::getContour    () const { return new VectorCollection<Point>(_points); }
  inline  const vector<Point>& Rectilinear::getPoints     () const { return _points; }


// -------------------------------------------------------------------
// Class  :  "JsonRoutingRectilinear".

  class JsonRectilinear : public JsonComponent {
    public:
      static  void             initialize   ();
                               JsonRectilinear ( unsigned long flags );
      virtual std::string      getTypeName  () const;
      virtual JsonRectilinear* clone        ( unsigned long ) const;
      virtual void             toData       ( JsonStack& ); 
  };


} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Rectilinear);
