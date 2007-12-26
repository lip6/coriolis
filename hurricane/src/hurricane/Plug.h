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

	public: virtual void Delete();

// Accessors
// *********

	public: virtual Cell* GetCell() const;
	public: virtual Unit GetX() const;
	public: virtual Unit GetY() const;
	public: virtual Point GetPosition() const;
	public: virtual Box GetBoundingBox() const;
	public: virtual Layer* GetLayer() const {return NULL;};
	public: virtual Box GetBoundingBox(BasicLayer* basicLayer) const;
	public: Instance* GetInstance() const {return _instance;};
	public: Net* GetMasterNet() const {return _masterNet;};

// Filters
// *******

	public: static PlugFilter GetIsConnectedFilter();
	public: static PlugFilter GetIsUnconnectedFilter();

// Predicates
// **********

	public: bool IsConnected() const {return (GetNet() != NULL);};

// Updators
// ********

	public: virtual void Translate(const Unit& dx, const Unit& dy) {};

	public: void SetNet(Net* net);

	public: virtual void Materialize();
	public: virtual void Unmaterialize();

// Others
// ******

	public: static Plug* _Create(Instance* instance, Net* masterNet);
	protected: virtual void _PostCreate();

	public: void _Delete();
	protected: virtual void _PreDelete();

    public: virtual string GetName() const;
	public: virtual string _GetTypeName() const {return _TName("Plug");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: Plug* _GetNextOfInstancePlugMap() const {return _nextOfInstancePlugMap;};

	public: virtual void _SetMasterNet(Net* masterNet) {_masterNet = masterNet;};
	public: void _SetNextOfInstancePlugMap(Plug* plug) {_nextOfInstancePlugMap = plug;};

	//public: virtual void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation) {};
	//public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation) {};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Plug)

#endif // HURRICANE_PLUG

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
