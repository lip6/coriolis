// ****************************************************************************************************
// File: MapView.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_MAP_VIEW
#define HURRICANE_MAP_VIEW

#include "View.h"

namespace Hurricane {

class MainView;



// ****************************************************************************************************
// MapView declaration
// ****************************************************************************************************

class MapView : public View {
// ************************

// Types
// *****

	public: typedef View Inherit;

// Attributes
// **********

	private: MainView* _mainView;

// Constructors
// ************

	private: MapView(MainView* mainView); // can't be derived

// Destructor
// **********

	public: virtual void Delete();

// Accessors
// *********

	public: virtual Cell* GetCell() const;
	public: virtual Transformation GetTransformation() const;
	public: virtual Box GetClientArea() const;
	public: MainView* GetMainView() const {return _mainView;};

// Others
// ******

	public: static MapView* _Create(MainView* mainView);
	private: virtual void _PostCreate();

	public: void _Delete();

	public: virtual string _GetTypeName() const {return _TName("MapView");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: virtual void _SetCell(Cell* cell);
	public: virtual void _SetTransformation(const Transformation& transformation);

	public: virtual void _PostRepaint();

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::MapView)

#endif // HURRICANE_MAP_VIEW

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
