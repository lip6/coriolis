// ****************************************************************************************************
// File: ./hurricane/Segment.h
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

#ifndef HURRICANE_SEGMENT
#define HURRICANE_SEGMENT

#include "hurricane/Component.h"
#include "hurricane/Segments.h"

namespace Hurricane {


// ****************************************************************************************************
// Segment declaration
// ****************************************************************************************************

class Segment : public Component {
// *****************************

// Types
// *****

    public: typedef Component Inherit;

    public: class SourceHook : public Hook {
    // ***********************************

        friend class Segment;

        public: typedef Hook Inherit;

        private: SourceHook(Segment* segment);

        public: virtual Component* getComponent() const;

        public: virtual bool isMaster() const {return false;};

        public: virtual string _getTypeName() const { return "Segment::SourceHook"; };
        public: virtual string _getString() const;
        public: static Hook* _compToHook(Component*);
    };

    public: class TargetHook : public Hook {
    // ***********************************

        friend class Segment;

        public: typedef Hook Inherit;

        private: TargetHook(Segment* segment);

        public: virtual Component* getComponent() const;

        public: virtual bool isMaster() const {return false;};

        public: virtual string _getTypeName() const { return "Segment::TargetHook"; };
        public: virtual string _getString() const;
        public: static Hook* _compToHook(Component*);
    };

// Attributes
// **********

    private: SourceHook _sourceHook;
    private: TargetHook _targetHook;
    private: const Layer* _layer;
    private: DbU::Unit _width;

// Constructors
// ************

  protected: Segment ( Net* net
                     , Component* source
                     , Component* target
                     , const Layer* layer
                     , const DbU::Unit& width = 0
                     );

// Accessors
// *********

    public: virtual Hooks getHooks() const;
    public: virtual DbU::Unit getX() const {return 0;};
    public: virtual DbU::Unit getY() const {return 0;};
    public: virtual const Layer* getLayer() const {return _layer;};
    public: Hook* getSourceHook() {return &_sourceHook;};
    public: Hook* getTargetHook() {return &_targetHook;};
    public: Hook* getOppositeHook(const Hook* hook) const;
    public: Component* getSource() const;
    public: Component* getTarget() const;
    public: Components getAnchors() const;
    public: Component* getOppositeAnchor(Component* anchor) const;
    public: const DbU::Unit& getWidth() const {return _width;};
    public: DbU::Unit getHalfWidth() const {return (_width / 2);};
    public: virtual DbU::Unit getSourceX() const = 0;
    public: virtual DbU::Unit getSourceY() const = 0;
    public: virtual Point getSourcePosition() const;
    public: virtual DbU::Unit getTargetX() const = 0;
    public: virtual DbU::Unit getTargetY() const = 0;
    public: virtual Point getTargetPosition() const;
    public: virtual DbU::Unit getLength() const = 0;

// Updators
// ********

    public: void setLayer(const Layer* layer);
    public: void setWidth(const DbU::Unit& width);
    public: void invert();

// Others
// ******

    protected: virtual void _preDestroy();

    public: virtual void _toJson(JsonWriter*) const;
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


// ****************************************************************************************************
// JsonSegment declaration
// ****************************************************************************************************

class JsonSegment : public JsonComponent {
// ***************************************

  public: JsonSegment(unsigned long flags);
};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Segment);
INSPECTOR_P_SUPPORT(Hurricane::Segment::SourceHook);
INSPECTOR_P_SUPPORT(Hurricane::Segment::TargetHook);


#endif // HURRICANE_SEGMENT


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
