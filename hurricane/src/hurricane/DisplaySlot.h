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
        private: bool   _isVisible;

// Constructors
// ************

        protected: DisplaySlot(Cell* cell,const Name& name, unsigned short red1, unsigned short green1, unsigned short blue1, const string& pattern1, unsigned linewidth1, unsigned short red2, unsigned short green2, unsigned short blue2, const string& pattern2, unsigned linewidth2);
        public: static DisplaySlot* create(Cell* cell, const Name& name, unsigned short red1, unsigned short green1, unsigned short blue1, const string& pattern1 = "FFFFFFFFFFFFFFFF", unsigned linewidth1=0, unsigned short red2 = 0, unsigned short green2 = 0, unsigned short blue2 = 0, const string& pattern2 = "FFFFFFFFFFFFFFFF", unsigned linewidth2=0);
        protected: virtual void _postCreate();

// Accessors
// *********

        public: const Name& getName() const {return _name;};
        public: const Box& getBoundingBox() const {return _quadTree.getBoundingBox();};
        public: UserGos getUserGos() const; 
        public: UserGos getUserGosUnder(const Box& area) const; 
            
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
        public: QuadTree& _getQuadTree() { return _quadTree; }
                
        public: virtual string _getTypeName() const {return _TName("DisplaySlot");};
        public: virtual string _getString() const;
        public: virtual Record* _getRecord() const;

};

DisplaySlots getDisplaySlots(const Cell* cell);

DisplaySlot* getDisplaySlot(const Cell* cell, const Name& name);




} // End of Hurricane namespace.

#endif // HURRICANE_DISPLAY_SLOT
