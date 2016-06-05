#ifndef HURRICANE_VISUAL_MAP
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2016  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
#define HURRICANE_VISUAL_MAP

#include "View.h"
#include "VisualMaps.h"

namespace Hurricane {

// ****************************************************************************************************
// VisualMap declaration
// ****************************************************************************************************

class VisualMap : public DBo {
// ***************************

// Types
// *****

	public: typedef DBo Inherit;

// Attributes
// **********
	private: Cell* _cell;
	private: Name _name; 
	//private: QuadTree _quadTree;
	private: vector<GdkGC*> _GCs;
	private: bool _isVisible;

// Constructors
// ************

	protected: VisualMap(Cell* cell,const Name& name);
	protected: virtual void _PostCreate();

// Destructor
// **********

	protected: virtual void _PreDelete();

// Accessors
// *********

	public: const Name& GetName() const {return _name;};
            
// Predicates
// **********

	public: bool IsVisible() const {return _isVisible;};
	public: void Show();
	public: void Hide();

// Updators
// ********
    protected: void SetGC (unsigned short index, unsigned short red, unsigned short green, unsigned short blue, const string& pattern = "CC33CC33CC33CC33");

// Others
// ******
	public: virtual void _Draw(View* view, const Box& updateArea, const Transformation& transformation) = 0;
	        
	public: virtual string _GetTypeName() const {return _TName("VisualMap");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

};

VisualMaps GetVisualMaps(const Cell* cell);

VisualMap* GetVisualMap(const Cell* cell, const Name& name);




} // End of Hurricane namespace.

#endif // HURRICANE_VISUAL_MAP
