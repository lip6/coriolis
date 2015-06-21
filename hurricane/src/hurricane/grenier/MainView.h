// ****************************************************************************************************
// File: MainView.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_MAIN_VIEW
#define HURRICANE_MAIN_VIEW

#include "View.h"

namespace Hurricane {

class MapView;



// ****************************************************************************************************
// MainView declaration
// ****************************************************************************************************

class MainView : public View {
// *************************

// Types
// *****

	public: typedef View Inherit;

// Attributes
// **********

	private: Cell* _cell;
	private: Transformation _transformation;
	private: MapView* _mapView;

// Constructors
// ************

	protected: MainView();

	public: static MainView* Create();

// Accessors
// *********

	public: virtual Cell* GetCell() const {return _cell;};
	public: virtual Transformation GetTransformation() const {return _transformation;};
	public: MapView* GetMapView() const {return _mapView;};

// Managers
// ********

	public: virtual bool Update(bool useDoubleBuffering = false);

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("MainView");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: virtual void _SetCell(Cell* cell);
	public: virtual void _SetTransformation(const Transformation& transformation);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::MainView)

#endif // HURRICANE_MAIN_VIEW

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
