// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2020, All Rights Reserved
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
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Commons.h"                         |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Component.h"
#include "hurricane/Contacts.h"


namespace Hurricane {

  
// -------------------------------------------------------------------
// Class  :  "Hurricane::Contact".

  class Contact : public Component {
    public:
      typedef Component Inherit;
    public:
      class AnchorHook : public Hook {
        friend class Contact;
        public:
          typedef Hook Inherit;
        private:
                               AnchorHook   ( Contact* );
        public:
          virtual Component*   getComponent () const;
          virtual bool         isMaster     () const { return false; };
          virtual std::string  _getTypeName () const { return "Contact::AnchorHook"; };
          virtual std::string  _getString   () const;
          static  Hook*        _compToHook  ( Component* );
    };

    protected:
      Contact (       Net*      net
              , const Layer*    layer
              ,       DbU::Unit x
              ,       DbU::Unit y
              ,       DbU::Unit width
              ,       DbU::Unit height
              );
      Contact (       Net*       net
              ,       Component* anchor
              , const Layer*     layer
              ,       DbU::Unit  dx
              ,       DbU::Unit  dy
              ,       DbU::Unit  width
              ,       DbU::Unit  height
              );
    public:
      static inline void     enableCheckMinSize  ();
      static inline void     disableCheckMinSize ();
      static        Contact* create (       Net*      net
                                    , const Layer*    layer
                                    ,       DbU::Unit x
                                    ,       DbU::Unit y
                                    ,       DbU::Unit width =0
                                    ,       DbU::Unit height=0
                                    );
      static        Contact* create (       Component* anchor
                                    , const Layer*     layer
                                    ,       DbU::Unit  dx
                                    ,       DbU::Unit  dy
                                    ,       DbU::Unit  width =0
                                    ,       DbU::Unit  height=0
                                    );
    public:
      virtual       Hooks        getHooks       () const;
      virtual       DbU::Unit    getX           () const;
      virtual       DbU::Unit    getY           () const;
      virtual       Point        getPosition    () const;
      virtual       Box          getBoundingBox () const;
      virtual const Layer*       getLayer       () const {return _layer;};
      virtual       Box          getBoundingBox ( const BasicLayer* ) const;
                    Hook*        getAnchorHook  () { return &_anchorHook; };
                    Component*   getAnchor      () const;
                    DbU::Unit    getDx          () const { return _dx; };
                    DbU::Unit    getDy          () const { return _dy; };
                    DbU::Unit    getWidth       () const { return _width; };
                    DbU::Unit    getHalfWidth   () const { return (_width / 2); };
                    DbU::Unit    getHeight      () const { return _height; };
                    DbU::Unit    getHalfHeight  () const { return (_height / 2); };
    public:
      virtual       void         translate      ( const DbU::Unit& dx, const DbU::Unit& dy );
                    void         setLayer       ( const Layer* );
                    void         setWidth       ( DbU::Unit );
                    void         setHeight      ( DbU::Unit );
                    void         setSizes       ( DbU::Unit width, DbU::Unit height);
                    void         setX           ( DbU::Unit );
                    void         setY           ( DbU::Unit );
                    void         setPosition    ( DbU::Unit x, DbU::Unit y);
                    void         setPosition    ( const Point& );
                    void         setDx          ( DbU::Unit );
                    void         setDy          ( DbU::Unit );
                    void         setOffset      ( DbU::Unit dx, DbU::Unit dy);
    private:                     
                    bool         _postCheck     ();
    protected:                   
      virtual       void         _preDestroy    ();
    public:
      virtual       void         _toJson        ( JsonWriter* ) const;
      virtual       std::string  _getTypeName   () const { return _TName("Contact"); };
      virtual       std::string  _getString     () const;
      virtual       Record*      _getRecord     () const;
    private:
      static bool        _checkMinSize;
             AnchorHook  _anchorHook;
      const  Layer*      _layer;
             DbU::Unit   _dx;
             DbU::Unit   _dy;
    protected:
             DbU::Unit   _width;
             DbU::Unit   _height;
  };

  
  inline void  Contact::enableCheckMinSize  () { _checkMinSize=true; }
  inline void  Contact::disableCheckMinSize () { _checkMinSize=false; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonContact".

  class JsonContact : public JsonComponent {
    public:
      static void          initialize  ();
                           JsonContact ( unsigned long flags );
      virtual std::string  getTypeName () const;
      virtual JsonContact* clone       ( unsigned long ) const;
      virtual void         toData      ( JsonStack& ); 
  };


} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Contact);
INSPECTOR_P_SUPPORT(Hurricane::Contact::AnchorHook);
