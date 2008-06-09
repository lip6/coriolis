// ****************************************************************************************************
// File: Plug.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_PLUG
#define HURRICANE_PLUG

#include "hurricane/Component.h"
#include "hurricane/Plugs.h"

namespace Hurricane {

class Net;
class Instance;



// ****************************************************************************************************
// Plug declaration
// ****************************************************************************************************

class Plug : public Component {
// **************************

// Types
// *****

    public: typedef Component Inherit;

// Attributes
// **********

    private: Instance* _instance;
    private: Net* _masterNet;
    private: Plug* _nextOfInstancePlugMap;

// Constructors
// ************

    protected: Plug(Instance* instance, Net* masterNet);

// Destructor
// **********

    public: virtual void destroy();

// Accessors
// *********

    public: virtual Cell* getCell() const;
    public: virtual DbU::Unit getX() const;
    public: virtual DbU::Unit getY() const;
    public: virtual Point getPosition() const;
    public: virtual Box getBoundingBox() const;
    public: virtual const Layer* getLayer() const {return NULL;};
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: Instance* getInstance() const {return _instance;};
    public: Net* getMasterNet() const {return _masterNet;};

// Filters
// *******

    public: static PlugFilter getIsConnectedFilter();
    public: static PlugFilter getIsUnconnectedFilter();

// Predicates
// **********

    public: bool isConnected() const {return (getNet() != NULL);};

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy) {};

    public: void setNet(Net* net);

    public: virtual void materialize() {};
    public: virtual void unmaterialize() {};

// Others
// ******

    public: static Plug* _create(Instance* instance, Net* masterNet);
    protected: virtual void _postCreate();

    public: void _destroy();
    protected: virtual void _preDestroy();

    public: virtual string getName() const;
    public: virtual string _getTypeName() const {return _TName("Plug");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: Plug* _getNextOfInstancePlugMap() const {return _nextOfInstancePlugMap;};

    public: virtual void _setMasterNet(Net* masterNet) {_masterNet = masterNet;};
    public: void _setNextOfInstancePlugMap(Plug* plug) {_nextOfInstancePlugMap = plug;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Plug)

#endif // HURRICANE_PLUG

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
