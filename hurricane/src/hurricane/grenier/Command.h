// ****************************************************************************************************
// File: Command.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_COMMAND
#define HURRICANE_COMMAND

#include "DBo.h"
#include "Commands.h"
#include "Views.h"
#include "Point.h"

namespace Hurricane {



// ****************************************************************************************************
// Command declaration
// ****************************************************************************************************

class Command : public DBo {
// ***********************

// Types
// *****

	public: typedef DBo Inherit;

	public: typedef set<View*> ViewSet;

// Attributes
// **********

	private: Name _name;
	private: unsigned _button;
	private: ViewSet _viewSet;

// Constructors
// ************

	protected: Command(const Name& name, unsigned button);

// Accessors
// *********

	public: const Name& GetName() const {return _name;};
	public: unsigned GetButton() const {return _button;};
	public: Views GetViews() const {return GetCollection(_viewSet);};

// Predicates
// **********

	public: bool IsInstalledOn(View* view) const;

// Managers
// ********

	public: void InstallOn(View* view);
	public: void UninstallFrom(View* view);

// Others
// ******

	protected: virtual void _PreDelete();

	public: string _GetString() const;
	public: Record* _GetRecord() const;
	public: ViewSet& _GetViewSet() {return _viewSet;};

	public: virtual void _OnInstalledOn(View* view);
	public: virtual void _OnUninstalledFrom(View* view);
	public: virtual void _OnDrawGhost(View* view);
	public: virtual void _OnMouseEnter(View* view, const Point& position);
	public: virtual void _OnMouseMove(View* view, const Point& position, unsigned state);
	public: virtual void _OnMouseLeave(View* view, const Point& position);
	public: virtual void _OnButtonPress(View* view, const Point& position, unsigned state);
	public: virtual void _OnButtonRelease(View* view, const Point& position, unsigned state);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Command)

#endif // HURRICANE_COMMAND

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
