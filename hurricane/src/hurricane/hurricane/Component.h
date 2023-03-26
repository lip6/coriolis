// ****************************************************************************************************
// File: ./hurricane/Component.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************


#ifndef HURRICANE_COMPONENT_H
#define HURRICANE_COMPONENT_H

#include  <set>
#include "hurricane/Points.h"
#include "hurricane/Go.h"
#include "hurricane/Components.h"
#include "hurricane/Hook.h"
#include "hurricane/Hooks.h"
#include "hurricane/Interval.h"


namespace Hurricane {

  class Net;
  class Rubber;
  class Layer;


// -------------------------------------------------------------------
// Class  :  "Component".

  class Component : public Go {
    public:
      typedef Go Inherit;

    public:
      class Points_Contour : public PointHC {
        public:
          class Locator : public PointHL {
            public:
                               Locator    ( const Component* );
              inline           Locator    ( const Locator& );
              virtual Point    getElement () const;
              virtual PointHL* getClone   () const;
              virtual bool     isValid    () const;
              virtual void     progress   ();
              virtual string   _getString () const;
            protected:
              const Component* _component;
                    size_t     _iPoint;
          };
        public:
          inline           Points_Contour ( const Component* );
          inline           Points_Contour ( const Points_Contour& );
          virtual PointHC* getClone       () const;
    	  virtual PointHL* getLocator     () const;
          virtual string   _getString     () const;
        protected:
          const Component*  _component;
      };

    public:
      class BodyHook : public Hook {
          friend class Component;
        public:
          typedef Hook Inherit;
        public:
          virtual Component* getComponent () const;
          virtual bool       isMaster     () const {return true;};
          virtual string     _getTypeName () const { return "Component::BodyHook"; };
          virtual string     _getString   () const;
          static  Hook*      _compToHook  ( Component* );
        private:
                             BodyHook     ( Component* );
      };

    protected:
                                    Component                 ( Net* , bool inPlugCreate = false );
    public:                         
    // Accessors.                   
      virtual       bool            isManhattanized           () const;
      virtual       bool            isNonRectangle            () const;
      virtual       Cell*           getCell                   () const;
                    Net*            getNet                    () const { return  _net; };
                    Rubber*         getRubber                 () const { return  _rubber; };
                    Hook*           getBodyHook               ()       { return &_bodyHook; };
      virtual       Hooks           getHooks                  () const;
      virtual       DbU::Unit       getX                      () const = 0;
      virtual       DbU::Unit       getY                      () const = 0;
      virtual       Point           getPosition               () const { return Point( getX(), getY() ); };
      virtual       Point           getCenter                 () const { return getPosition(); };
      virtual const Layer*          getLayer                  () const = 0;
      virtual       size_t          getPointsSize             () const;
      virtual       Point           getPoint                  ( size_t ) const;
      virtual       Box             getBoundingBox            () const = 0;
      virtual       Box             getBoundingBox            ( const BasicLayer* ) const = 0;
      inline        Points          getContour                () const;
      virtual       Points          getMContour               () const;
                    Components      getConnexComponents       () const;
                    Components      getSlaveComponents        () const;
    // Mutators.                                              
      virtual       void            materialize               ();
      virtual       void            unmaterialize             ();
      virtual       void            invalidate                ( bool propagateFlag = true );
      virtual       void            forceId                   ( unsigned int id );
    // Filters                                                
      static        ComponentFilter getIsUnderFilter          ( const Box& area );
    // Others
    protected:
      virtual       void            _postCreate               ();
      virtual       void            _preDestroy               ();
    public:                         
      virtual       void            _toJson                   ( JsonWriter* ) const;
      virtual       void            _toJsonSignature          ( JsonWriter* ) const;
      virtual       string          _getString                () const;
      virtual       Record*         _getRecord                () const;
                    Component*      _getNextOfNetComponentSet () const {return _nextOfNetComponentSet;};
                    void            _setNet                   ( Net* );
                    void            _setRubber                ( Rubber* );
                    void            _setNextOfNetComponentSet ( Component* component ) { _nextOfNetComponentSet = component; };
    private:
      Net*       _net;
      Rubber*    _rubber;
      BodyHook   _bodyHook;
      Component* _nextOfNetComponentSet;
  };


  inline Points  Component::getContour () const { return Points_Contour(this); }

  
  inline Component::Points_Contour::Locator::Locator ( const Locator &locator )
    : PointHL   ()
    , _component(locator._component)
    , _iPoint   (locator._iPoint)
  { }


  inline Component::Points_Contour::Points_Contour ( const Component* component )
    : PointHC   ()
    , _component(component)
  { }


  inline Component::Points_Contour::Points_Contour ( const Points_Contour& other )
    : PointHC   ()
    , _component(other._component)
  { }


  double  getArea ( Component* component );


// -------------------------------------------------------------------
// Class  :  "JsonComponent".

  class JsonComponent : public JsonEntity {
    public:
      JsonComponent ( unsigned long flags );
  };


  typedef  std::set<Component*,DBo::CompareById>  ComponentSet;


} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Component);
INSPECTOR_P_SUPPORT(Hurricane::Component::BodyHook);


#endif // HURRICANE_COMPONENT_H

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
