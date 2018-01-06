// ****************************************************************************************************
// File: ./hurricane/Plug.h
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
    public: virtual void _toJson(JsonWriter*) const;
    public: virtual string _getTypeName() const {return _TName("Plug");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: Plug* _getNextOfInstancePlugMap() const {return _nextOfInstancePlugMap;};

    public: virtual void _setMasterNet(Net* masterNet) {_masterNet = masterNet;};
    public: void _setNextOfInstancePlugMap(Plug* plug) {_nextOfInstancePlugMap = plug;};

};


class JsonPlug : public JsonComponent {
// ************************************

  public: static void initialize();
  public: JsonPlug(unsigned long flags);
  public: virtual string getTypeName() const;
  public: virtual JsonPlug* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};


class JsonPlugRef : public JsonObject {
// ************************************

  public: static void initialize();
  public: JsonPlugRef(unsigned long flags);
  public: virtual string getTypeName() const;
  public: virtual JsonPlugRef* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};

} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Plug);


#endif // HURRICANE_PLUG


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
