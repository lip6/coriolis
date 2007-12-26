// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: DisplaySlot.h
// Author: C. Alexandre
// ****************************************************************************************************

#ifndef HURRICANE_DISPLAY_SLOT
#define HURRICANE_DISPLAY_SLOT

#include "QuadTree.h"
#include "UserGos.h"
#include "DisplaySlots.h"

namespace Hurricane {

// ****************************************************************************************************
// DisplaySlot declaration
// ****************************************************************************************************

class DisplaySlot : public DBo {
// ***************************

// Types
// *****

        public: typedef DBo Inherit;

// Attributes
// **********
        private: Cell* _cell;
        private: Name _name; 
        private: QuadTree _quadTree;
        //private: GdkGC* _drawGC;
        //private: GdkGC* _fillGC;
        private: bool   _isVisible;

// Constructors
// ************

        protected: DisplaySlot(Cell* cell,const Name& name, unsigned short red1, unsigned short green1, unsigned short blue1, const string& pattern1, unsigned linewidth1, unsigned short red2, unsigned short green2, unsigned short blue2, const string& pattern2, unsigned linewidth2);
        public: static DisplaySlot* Create(Cell* cell, const Name& name, unsigned short red1, unsigned short green1, unsigned short blue1, const string& pattern1 = "FFFFFFFFFFFFFFFF", unsigned linewidth1=0, unsigned short red2 = 0, unsigned short green2 = 0, unsigned short blue2 = 0, const string& pattern2 = "FFFFFFFFFFFFFFFF", unsigned linewidth2=0);
        protected: virtual void _PostCreate();

// Destructor
// **********

        protected: virtual void _PreDelete();

// Accessors
// *********

        public: const Name& GetName() const {return _name;};
        public: const Box& GetBoundingBox() const {return _quadTree.GetBoundingBox();};
        public: UserGos GetUserGos() const; 
        public: UserGos GetUserGosUnder(const Box& area) const; 
            
// Predicates
// **********

        public: bool IsVisible() const {return _isVisible;};
        public: void Show();
        public: void Hide();

// Updators
// ********
        public: void Flush();

// Others
// ******
        //public: GdkGC* _GetDrawGC() const { return _drawGC; }
        //public: GdkGC* _GetFillGC() const { return _fillGC; }
        public: QuadTree& _GetQuadTree() { return _quadTree; }
        //public: virtual void _Draw(View* view, const Box& updateArea, const Transformation& transformation);
                
        public: virtual string _GetTypeName() const {return _TName("DisplaySlot");};
        public: virtual string _GetString() const;
        public: virtual Record* _GetRecord() const;

};

DisplaySlots GetDisplaySlots(const Cell* cell);

DisplaySlot* GetDisplaySlot(const Cell* cell, const Name& name);




} // End of Hurricane namespace.

#endif // HURRICANE_DISPLAY_SLOT
