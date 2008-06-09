// ****************************************************************************************************
// File: Segment.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
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

        public: virtual string _getTypeName() const { return _TName("Segment::SourceHook"); };
        public: virtual string _getString() const;
    };

    public: class TargetHook : public Hook {
    // ***********************************

        friend class Segment;

        public: typedef Hook Inherit;

        private: TargetHook(Segment* segment);

        public: virtual Component* getComponent() const;

        public: virtual bool isMaster() const {return false;};

        public: virtual string _getTypeName() const { return _TName("Segment::TargetHook"); };
        public: virtual string _getString() const;
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

    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Segment)
SetNestedSlotAdapter(Hurricane::Segment::SourceHook)
SetNestedSlotAdapter(Hurricane::Segment::TargetHook)

#endif // HURRICANE_SEGMENT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
