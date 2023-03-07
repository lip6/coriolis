// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2023, All Rights Reserved
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
// |  C++ Header  :  "./hurricane/Diagonal.h"                        |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Component.h"


namespace Hurricane {

  class Layer;


// -------------------------------------------------------------------
// Class  :  "Diagonal".

  class Diagonal : public Component {
    public:
      typedef Component Super;
  
    public:
      static  Diagonal*    create            ( Net*, const Layer*, const Point& source, const Point& target, DbU::Unit width );
    // Accessors.
      virtual bool         isNonRectangle    () const;
      virtual DbU::Unit    getX              () const;
      virtual DbU::Unit    getY              () const;
      virtual DbU::Unit    getSourceX        () const;
      virtual DbU::Unit    getSourceY        () const;
      virtual DbU::Unit    getTargetX        () const;
      virtual DbU::Unit    getTargetY        () const;
      virtual Point        getSourcePosition () const;
      virtual Point        getTargetPosition () const;
      virtual Box          getBoundingBox    () const;
      virtual Box          getBoundingBox    ( const BasicLayer* ) const;
      virtual size_t       getPointsSize     () const;
      virtual Point        getPoint          ( size_t i ) const;
              DbU::Unit    getWidth          () const;
      virtual const Layer* getLayer          () const;
    // Mutators.
              void         setLayer          ( const Layer* );
              void         setWidth          ( DbU::Unit );
      virtual void         translate         ( const DbU::Unit& dx, const DbU::Unit& dy );
              void         setSource         ( Point );
              void         setTarget         ( Point );
    // Hurricane management.
      virtual void         _toJson           ( JsonWriter* ) const;
      static  JsonObject*  getJsonObject     ( unsigned long flags );
      virtual string       _getTypeName      () const;
      virtual string       _getString        () const;
      virtual Record*      _getRecord        () const;
    protected:                               
                           Diagonal          ( Net*, const Layer*, const Point& source, const Point& target, DbU::Unit width );
              void         _updateB          ();
    private:
      const Layer*     _layer;
            Point      _source;
            Point      _target;
            DbU::Unit  _width;
            DbU::Unit  _dWidth;  // octagon half Y.
  };


// -------------------------------------------------------------------
// Class  :  "JsonRoutingDiagonal".

  class JsonDiagonal : public JsonComponent {
    public:
      static  void          initialize   ();
                            JsonDiagonal ( unsigned long flags );
      virtual std::string   getTypeName  () const;
      virtual JsonDiagonal* clone        ( unsigned long ) const;
      virtual void          toData       ( JsonStack& ); 
  };

} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Diagonal);
