// ****************************************************************************************************
// File: Selector.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_SELECTOR
#define HURRICANE_SELECTOR

#include "Property.h"
#include "Selectors.h"
#include "Occurrence.h"
#include "Views.h"

namespace Hurricane {



// ****************************************************************************************************
// Selector declaration
// ****************************************************************************************************

class Selector : public PrivateProperty {
// ************************************

// Types
// *****

	public: typedef PrivateProperty Inherit;

	public: typedef set<View*> ViewSet;

// Attributes
// **********

	private: ViewSet _viewSet;

// Constructors
// ************

	protected: Selector();

// Accessors
// *********

	public: static const Name& GetPropertyName();
	public: virtual Name GetName() const;
	public: Occurrence GetOccurrence() const;
	public: Views GetViews() const {return GetCollection(_viewSet);};

// Ohers
// *****

	public: static Selector* _Create(Occurrence& occurrence);

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("Selector");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: ViewSet& _GetViewSet() {return _viewSet;};

	public: void _AttachTo(View* view);
	public: void _DetachFrom(View* view, bool inDeletion = false);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Selector)

#endif // HURRICANE_SELECTOR

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
