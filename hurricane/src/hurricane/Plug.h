// ****************************************************************************************************
// File: Plug.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_PLUG
#define HURRICANE_PLUG

#include "Component.h"
#include "Plugs.h"

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
    public: virtual Unit getX() const;
    public: virtual Unit getY() const;
    public: virtual Point getPosition() const;
    public: virtual Box getBoundingBox() const;
    public: virtual Layer* getLayer() const {return NULL;};
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: Instance* getInstance() const {return _instance;};
    public: Net* getMasterNet() const {return _masterNet;};

// Filters
// *******

    public: static PlugFilter getIsConnectedFilter();
    public: static PlugFilter getIsUnconnectedFilter();

// Predicates
// **********

    public: bool IsConnected() const {return (getNet() != NULL);};

// Updators
// ********

    public: virtual void Translate(const Unit& dx, const Unit& dy) {};

    public: void SetNet(Net* net);

    public: virtual void Materialize();
    public: virtual void Unmaterialize();

// Others
// ******

    public: static Plug* _Create(Instance* instance, Net* masterNet);
    protected: virtual void _postCreate();

    public: void _destroy();
    protected: virtual void _preDestroy();

    public: virtual string getName() const;
    public: virtual string _getTypeName() const {return _TName("Plug");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: Plug* _getNextOfInstancePlugMap() const {return _nextOfInstancePlugMap;};

    public: virtual void _SetMasterNet(Net* masterNet) {_masterNet = masterNet;};
    public: void _SetNextOfInstancePlugMap(Plug* plug) {_nextOfInstancePlugMap = plug;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Plug)

#endif // HURRICANE_PLUG

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
