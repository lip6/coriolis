// ****************************************************************************************************
// File: Cell.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Slice.h"
#include "Cell.h"
//#include "MainView.h"
#include "HyperNet.h"

namespace Hurricane {

// ****************************************************************************************************
// Cell_Slices implementation
// ****************************************************************************************************

class Cell_Slices : public Collection<Slice*> {
// ******************************************

// Types
// *****

    public: typedef Collection<Slice*> Inherit;

    public: class Locator : public Hurricane::Locator<Slice*> {
    // ******************************************************

        public: typedef Hurricane::Locator<Slice*> Inherit;

        private: const Cell* _cell;
        private: Layer::Mask _mask;
        private: SliceLocator _sliceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Layer::Mask& mask = ~0);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Slice* GetElement() const;
        public: virtual Hurricane::Locator<Slice*>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Cell_Slices();
    public: Cell_Slices(const Cell* cell, const Layer::Mask& mask = ~0);
    public: Cell_Slices(const Cell_Slices& slices);

// Operators
// *********

    public: Cell_Slices& operator=(const Cell_Slices& slices);

// Accessors
// *********

    public: virtual Collection<Slice*>* GetClone() const;
    public: virtual Hurricane::Locator<Slice*>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Cell_Components implementation
// ****************************************************************************************************

class Cell_Components : public Collection<Component*> {
// **************************************************

// Types
// *****

    public: typedef Collection<Component*> Inherit;

    public: class Locator : public Hurricane::Locator<Component*> {
    // **********************************************************

        public: typedef Hurricane::Locator<Component*> Inherit;

        private: const Cell* _cell;
        private: Layer::Mask _mask;
        private: SliceLocator _sliceLocator;
        private: ComponentLocator _componentLocator;
        private: Component* _component;

        public: Locator();
        public: Locator(const Cell* cell, const Layer::Mask& mask = ~0);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Component* GetElement() const;
        public: virtual Hurricane::Locator<Component*>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Cell_Components();
    public: Cell_Components(const Cell* cell, const Layer::Mask& mask = ~0);
    public: Cell_Components(const Cell_Components& components);

// Operators
// *********

    public: Cell_Components& operator=(const Cell_Components& components);

// Accessors
// *********

    public: virtual Collection<Component*>* GetClone() const;
    public: virtual Hurricane::Locator<Component*>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Cell_ComponentsUnder implementation
// ****************************************************************************************************

class Cell_ComponentsUnder : public Collection<Component*> {
// *******************************************************

// Types
// *****

    public: typedef Collection<Component*> Inherit;

    public: class Locator : public Hurricane::Locator<Component*> {
    // **********************************************************

        public: typedef Hurricane::Locator<Component*> Inherit;

        private: const Cell* _cell;
        private: Box _area;
        private: Layer::Mask _mask;
        private: SliceLocator _sliceLocator;
        private: ComponentLocator _componentLocator;
        private: Component* _component;

        public: Locator();
        public: Locator(const Cell* cell, const Box& area, const Layer::Mask& mask = ~0);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Component* GetElement() const;
        public: virtual Hurricane::Locator<Component*>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Box _area;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Cell_ComponentsUnder();
    public: Cell_ComponentsUnder(const Cell* cell, const Box& area, const Layer::Mask& mask = ~0);
    public: Cell_ComponentsUnder(const Cell_ComponentsUnder& components);

// Operators
// *********

    public: Cell_ComponentsUnder& operator=(const Cell_ComponentsUnder& components);

// Accessors
// *********

    public: virtual Collection<Component*>* GetClone() const;
    public: virtual Hurricane::Locator<Component*>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Cell_Occurrences implementation
// ****************************************************************************************************

class Cell_Occurrences : public Collection<Occurrence> {
// *************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: unsigned _searchDepth;
        private: unsigned _state;
        private: ComponentLocator _componentLocator;
        private: RubberLocator _rubberLocator;
        private: MarkerLocator _markerLocator;
        private: InstanceLocator _instanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, unsigned searchDepth);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence GetElement() const;
        public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: unsigned _searchDepth;

// Constructors
// ************

    public: Cell_Occurrences();
    public: Cell_Occurrences(const Cell* cell, unsigned searchDepth);
    public: Cell_Occurrences(const Cell_Occurrences& occurrences);

// Operators
// *********

    public: Cell_Occurrences& operator=(const Cell_Occurrences& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* GetClone() const;
    public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Cell_OccurrencesUnder implementation
// ****************************************************************************************************

class Cell_OccurrencesUnder : public Collection<Occurrence> {
// ******************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: Box _area;
        private: unsigned _searchDepth;
        private: unsigned _state;
        private: ComponentLocator _componentLocator;
        private: RubberLocator _rubberLocator;
        private: MarkerLocator _markerLocator;
        private: InstanceLocator _instanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Box& area, unsigned searchDepth = (unsigned)-1);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence GetElement() const;
        public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Box _area;
    private: unsigned _searchDepth;

// Constructors
// ************

    public: Cell_OccurrencesUnder();
    public: Cell_OccurrencesUnder(const Cell* cell, const Box& area, unsigned searchDepth = (unsigned)-1);
    public: Cell_OccurrencesUnder(const Cell_OccurrencesUnder& occurrences);

// Operators
// *********

    public: Cell_OccurrencesUnder& operator=(const Cell_OccurrencesUnder& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* GetClone() const;
    public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};

// ****************************************************************************************************
// Cell_LeafInstanceOccurrences declaration
// ****************************************************************************************************

class Cell_LeafInstanceOccurrences : public Collection<Occurrence> {
// *****************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: int _state;
        private: InstanceLocator _leafInstanceLocator;
        private: InstanceLocator _nonLeafInstanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator(const Cell* cell = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence GetElement() const;
        public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;

// Constructors
// ************

    public: Cell_LeafInstanceOccurrences(const Cell* cell = NULL);
    public: Cell_LeafInstanceOccurrences(const Cell_LeafInstanceOccurrences& occurrences);

// Operators
// *********

    public: Cell_LeafInstanceOccurrences& operator=(const Cell_LeafInstanceOccurrences& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* GetClone() const;
    public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Cell_LeafInstanceOccurrencesUnder declaration
// ****************************************************************************************************

class Cell_LeafInstanceOccurrencesUnder : public Collection<Occurrence> {
// **********************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: Box _area;
        private: int _state;
        private: InstanceLocator _leafInstanceLocator;
        private: InstanceLocator _nonLeafInstanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Box& area);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence GetElement() const;
        public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Box _area;

// Constructors
// ************

    public: Cell_LeafInstanceOccurrencesUnder();
    public: Cell_LeafInstanceOccurrencesUnder(const Cell* cell, const Box& area);
    public: Cell_LeafInstanceOccurrencesUnder(const Cell_LeafInstanceOccurrencesUnder& occurrences);

// Operators
// *********

    public: Cell_LeafInstanceOccurrencesUnder& operator=(const Cell_LeafInstanceOccurrencesUnder& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* GetClone() const;
    public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrences declaration
// ****************************************************************************************************

class Cell_TerminalInstanceOccurrences : public Collection<Occurrence> {
// *****************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: int _state;
        private: InstanceLocator _terminalInstanceLocator;
        private: InstanceLocator _nonTerminalInstanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator(const Cell* cell = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence GetElement() const;
        public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;

// Constructors
// ************

    public: Cell_TerminalInstanceOccurrences(const Cell* cell = NULL);
    public: Cell_TerminalInstanceOccurrences(const Cell_TerminalInstanceOccurrences& occurrences);

// Operators
// *********

    public: Cell_TerminalInstanceOccurrences& operator=(const Cell_TerminalInstanceOccurrences& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* GetClone() const;
    public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrencesUnder declaration
// ****************************************************************************************************

class Cell_TerminalInstanceOccurrencesUnder : public Collection<Occurrence> {
// **********************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: Box _area;
        private: int _state;
        private: InstanceLocator _terminalInstanceLocator;
        private: InstanceLocator _nonTerminalInstanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Box& area);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence GetElement() const;
        public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Box _area;

// Constructors
// ************

    public: Cell_TerminalInstanceOccurrencesUnder();
    public: Cell_TerminalInstanceOccurrencesUnder(const Cell* cell, const Box& area);
    public: Cell_TerminalInstanceOccurrencesUnder(const Cell_TerminalInstanceOccurrencesUnder& occurrences);

// Operators
// *********

    public: Cell_TerminalInstanceOccurrencesUnder& operator=(const Cell_TerminalInstanceOccurrencesUnder& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* GetClone() const;
    public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Cell_ComponentOccurrences implementation
// ****************************************************************************************************

class Cell_ComponentOccurrences : public Collection<Occurrence> {
// **********************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: Layer::Mask _mask;
        private: int _state;
        private: ComponentLocator _componentLocator;
        private: InstanceLocator _instanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Layer::Mask& mask = ~0);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence GetElement() const;
        public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Cell_ComponentOccurrences();
    public: Cell_ComponentOccurrences(const Cell* cell, const Layer::Mask& mask = ~0);
    public: Cell_ComponentOccurrences(const Cell_ComponentOccurrences& occurrences);

// Operators
// *********

    public: Cell_ComponentOccurrences& operator=(const Cell_ComponentOccurrences& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* GetClone() const;
    public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Cell_ComponentOccurrencesUnder implementation
// ****************************************************************************************************

class Cell_ComponentOccurrencesUnder : public Collection<Occurrence> {
// ***************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: Box _area;
        private: Layer::Mask _mask;
        private: int _state;
        private: ComponentLocator _componentLocator;
        private: InstanceLocator _instanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Box& area, const Layer::Mask& mask = ~0);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence GetElement() const;
        public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Box _area;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Cell_ComponentOccurrencesUnder();
    public: Cell_ComponentOccurrencesUnder(const Cell* cell, const Box& area, const Layer::Mask& mask = ~0);
    public: Cell_ComponentOccurrencesUnder(const Cell_ComponentOccurrencesUnder& occurrences);

// Operators
// *********

    public: Cell_ComponentOccurrencesUnder& operator=(const Cell_ComponentOccurrencesUnder& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* GetClone() const;
    public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};

// ****************************************************************************************************
// Cell_HyperNetRootNetOccurrences implementation
// ****************************************************************************************************

class Cell_HyperNetRootNetOccurrences : public Collection<Occurrence> {
// ****************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: Path _path;
        private: NetLocator _netLocator;
        private: InstanceLocator _instanceLocator;
        private: OccurrenceLocator _hyperNetRootNetOccurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Path path);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence GetElement() const;
        public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Path _path;

// Constructors
// ************

    public: Cell_HyperNetRootNetOccurrences();
    public: Cell_HyperNetRootNetOccurrences(const Cell* cell, const Path path);
    public: Cell_HyperNetRootNetOccurrences(const Cell_HyperNetRootNetOccurrences& netOccurrences);

// Operators
// *********

    public: Cell_HyperNetRootNetOccurrences& operator=(const Cell_HyperNetRootNetOccurrences& HyperNetRootNetOccurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* GetClone() const;
    public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};


//// ****************************************************************************************************
//// Cell_ImpactedMainViews implementation
//// ****************************************************************************************************
//
//class Cell_ImpactedMainViews : public Collection<MainView*> {
//// ********************************************************
//
//// Types
//// *****
//
//    public: typedef Collection<MainView*> Inherit;
//
//    public: class Locator : public Hurricane::Locator<MainView*> {
//    // *********************************************************
//
//        public: typedef Hurricane::Locator<MainView*> Inherit;
//
//        private: const Cell* _cell;
//        private: set<Cell*> _cellSet;
//        private: stack<Cell*> _cellStack;
//        private: MainViewLocator _mainViewLocator;
//        private: MainView* _mainView;
//
//        public: Locator(const Cell* cell = NULL);
//        public: Locator(const Locator& locator);
//
//        public: Locator& operator=(const Locator& locator);
//
//        public: virtual MainView* GetElement() const;
//        public: virtual Hurricane::Locator<MainView*>* GetClone() const;
//
//        public: virtual bool IsValid() const;
//
//        public: virtual void Progress();
//
//        public: virtual string _GetString() const;
//
//    };
//
//// Attributes
//// **********
//
//    private: const Cell* _cell;
//
//// Constructors
//// ************
//
//    public: Cell_ImpactedMainViews(const Cell* cell = NULL);
//    public: Cell_ImpactedMainViews(const Cell_ImpactedMainViews& impactedMainViews);
//
//// Operators
//// *********
//
//    public: Cell_ImpactedMainViews& operator=(const Cell_ImpactedMainViews& impactedMainViews);
//
//// Accessors
//// *********
//
//    public: virtual Collection<MainView*>* GetClone() const;
//    public: virtual Hurricane::Locator<MainView*>* GetLocator() const;
//
//// Others
//// ******
//
//    public: virtual string _GetString() const;
//
//};
//
// ****************************************************************************************************
// Cell_SubCells declaration
// ****************************************************************************************************

class Cell_SubCells : public Collection<Cell*> {
// *********************************************

// Types
// *****

    public: typedef Collection<Cell*> Inherit;

    public: class Locator : public Hurricane::Locator<Cell*> {
    // *******************************************************

        public: typedef Hurricane::Locator<Cell*> Inherit;

        private: set <Cell*> _cellSet;
        private: InstanceLocator _instanceLocator;

        public: Locator();
        public: Locator(InstanceLocator instanceLocator);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Cell* GetElement() const;
        public: virtual Hurricane::Locator<Cell*>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;

// Constructors
// ************

    public: Cell_SubCells();
    public: Cell_SubCells(const Cell* cell);
    public: Cell_SubCells(const Cell_SubCells& subCells);

// Operators
// *********

    public: Cell_SubCells& operator=(const Cell_SubCells& subCells);

// Accessors
// *********

    public: virtual Collection<Cell*>* GetClone() const;
    public: virtual Hurricane::Locator<Cell*>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

};




// -------------------------------------------------------------------
// Class  :  "Cell_RecursiveSlavePathes".

  class Cell_RecursiveSlavePathes : public Collection<Path> {

    // Locator Sub-Class.
    public:
      class Locator : public Hurricane::Locator<Path> {
        // Attributes.
        protected:
          vector<InstanceLocator>  _instancesStack;
          Path                     _instancePath;
          bool                     _isInPop;
        // Constructors.
        public:
          Locator ( const Cell*    cell     );
          Locator ( const Locator  &locator ) : Hurricane::Locator<Path>()
                                              , _instancesStack(locator._instancesStack)
                                              , _instancePath(locator._instancePath)
                                              , _isInPop(locator._isInPop)
                                              { };
        // Accessors.
        public:
          virtual Path                      GetElement () const { return _instancePath; };
          virtual Hurricane::Locator<Path>* GetClone   () const { return new Locator(*this); };
          virtual bool                      IsValid    () const { return _instancesStack.size(); };
          virtual void                      Progress   ();
        // Hurricane Managment.
        public:
          virtual string _GetString() const;
      };

    // Attributes.
    public:
      const Cell* _cell;

    // Constructors.
    public:
      Cell_RecursiveSlavePathes ( const Cell* cell )
                                : Collection<Path>()
                                , _cell(cell)
                                { };
      Cell_RecursiveSlavePathes ( const Cell_RecursiveSlavePathes& pathes )
                                : Collection<Path>()
                                , _cell(pathes._cell)
                                { };
      
    // Accessors.
    public:
      virtual Collection<Path>*         GetClone   () const { return new Cell_RecursiveSlavePathes(*this); };
      virtual Hurricane::Locator<Path>* GetLocator () const { return new Locator(_cell); };

    // Hurricane Managment.
    public:
      virtual string _GetString() const;

  };




// ****************************************************************************************************
// Cell further definition
// ****************************************************************************************************

Instances Cell::GetInstancesUnder(const Box& area) const
// *****************************************************
{
    // return _quadTree.GetGosUnder(area).GetSubSet<Instance*>();
    return SubTypeCollection<Go*, Instance*>(_quadTree.GetGosUnder(area));
}

Instances Cell::GetSlaveInstances() const
// **************************************
{
    return _slaveInstanceSet.GetElements();
}

Instances Cell::GetTerminalInstances() const
// *****************************************
{
    return GetInstances().GetSubSet(Instance::GetIsTerminalFilter());
}

Instances Cell::GetLeafInstances() const
// *************************************
{
    return GetInstances().GetSubSet(Instance::GetIsLeafFilter());
}

Instances Cell::GetPlacedInstances() const
// ***************************************
{
    return GetInstances().GetSubSet(Instance::GetIsPlacedFilter());
}

Instances Cell::GetFixedInstances() const
// **************************************
{
    return GetInstances().GetSubSet(Instance::GetIsFixedFilter());
}

Instances Cell::GetUnplacedInstances() const
// **************************************
{
    return GetInstances().GetSubSet(Instance::GetIsUnplacedFilter());
}

Instances Cell::GetNotUnplacedInstances() const
// **************************************
{
    return GetInstances().GetSubSet(Instance::GetIsNotUnplacedFilter());
}

Instances Cell::GetPlacedInstancesUnder(const Box& area) const
// ***********************************************************
{
    return GetInstancesUnder(area).GetSubSet(Instance::GetIsPlacedFilter());
}

Instances Cell::GetFixedInstancesUnder(const Box& area) const
// **********************************************************
{
    return GetInstancesUnder(area).GetSubSet(Instance::GetIsFixedFilter());
}

Instances Cell::GetUnplacedInstancesUnder(const Box& area) const
// *************************************************************
{
    return GetInstancesUnder(area).GetSubSet(Instance::GetIsUnplacedFilter());
}

Instances Cell::GetNotUnplacedInstancesUnder(const Box& area) const
// ****************************************************************
{
    return GetInstancesUnder(area).GetSubSet(Instance::GetIsNotUnplacedFilter());
}

Instances Cell::GetTerminalInstancesUnder(const Box& area) const
// *************************************************************
{
    return GetInstancesUnder(area).GetSubSet(Instance::GetIsTerminalFilter());
}

Instances Cell::GetNonTerminalInstances() const
// ********************************************
{
    return GetInstances().GetSubSet(!Instance::GetIsTerminalFilter());
}

Instances Cell::GetNonTerminalInstancesUnder(const Box& area) const
// ****************************************************************
{
    return GetInstancesUnder(area).GetSubSet(!Instance::GetIsTerminalFilter());
}

Instances Cell::GetLeafInstancesUnder(const Box& area) const
// *********************************************************
{
    return GetInstancesUnder(area).GetSubSet(Instance::GetIsLeafFilter());
}

Instances Cell::GetNonLeafInstances() const
// ****************************************
{
    return GetInstances().GetSubSet(!Instance::GetIsLeafFilter());
}

Instances Cell::GetNonLeafInstancesUnder(const Box& area) const
// ************************************************************
{
    return GetInstancesUnder(area).GetSubSet(!Instance::GetIsLeafFilter());
}

Nets Cell::GetGlobalNets() const
// *****************************
{
    return GetNets().GetSubSet(Net::GetIsGlobalFilter());
}

Nets Cell::GetExternalNets() const
// *******************************
{
    return GetNets().GetSubSet(Net::GetIsExternalFilter());
}

Nets Cell::GetInternalNets() const
// *******************************
{
    return GetNets().GetSubSet(Net::GetIsInternalFilter());
}

Nets Cell::GetClockNets() const
// ****************************
{
    return GetNets().GetSubSet(Net::GetIsClockFilter());
}

Nets Cell::GetSupplyNets() const
// *****************************
{
    return GetNets().GetSubSet(Net::GetIsSupplyFilter());
}

Nets Cell::GetPowerNets() const
// *****************************
{
    return GetNets().GetSubSet(Net::GetIsPowerFilter());
}

Nets Cell::GetGroundNets() const
// *****************************
{
    return GetNets().GetSubSet(Net::GetIsGroundFilter());
}

Slices Cell::GetSlices(const Layer::Mask& mask) const
// **************************************************
{
    return Cell_Slices(this, mask);
}

//MainViews Cell::GetMainViews() const
//// *********************************
//{
//    // return GetViews().GetSubSet<MainView*>();
//    return SubTypeCollection<View*, MainView*>(GetViews());
//}
//
//MainViews Cell::GetImpactedMainViews() const
//// *****************************************
//{
//    return Cell_ImpactedMainViews(this);
//}
//
Rubbers Cell::GetRubbers() const
// *****************************
{
    // return _quadTree.GetGos().GetSubSet<Rubber*>();
    return SubTypeCollection<Go*, Rubber*>(_quadTree.GetGos());
}

Rubbers Cell::GetRubbersUnder(const Box& area) const
// *************************************************
{
    // return (area.isEmpty()) ? Rubbers() : _quadTree.GetGosUnder(area).GetSubSet<Rubber*>();
    return SubTypeCollection<Go*, Rubber*>(_quadTree.GetGosUnder(area));
}

Markers Cell::GetMarkersUnder(const Box& area) const
// *************************************************
{
    // return (area.isEmpty()) ? Markers() : _quadTree.GetGosUnder(area).GetSubSet<Marker*>();
    return SubTypeCollection<Go*, Marker*>(_quadTree.GetGosUnder(area));
}

References Cell::GetReferences() const
// **********************************
{
    return SubTypeCollection<Marker*, Reference*>(GetMarkers());
}

Cells Cell::GetSubCells() const
// ****************************
{
    return Cell_SubCells(this);
}

Components Cell::GetComponents(const Layer::Mask& mask) const
// **********************************************************
{
    return Cell_Components(this, mask);
}

Components Cell::GetComponentsUnder(const Box& area, const Layer::Mask& mask) const
// ********************************************************************************
{
    return Cell_ComponentsUnder(this, area, mask);
}

Occurrences Cell::GetOccurrences(unsigned searchDepth) const
// *******************************************************
{
    return Cell_Occurrences(this, searchDepth);
}

Occurrences Cell::GetOccurrencesUnder(const Box& area, unsigned searchDepth) const
// *****************************************************************************
{
    return Cell_OccurrencesUnder(this, area, searchDepth);
}

Occurrences Cell::GetTerminalInstanceOccurrences() const
// ***************************************************
{
    return Cell_TerminalInstanceOccurrences(this);
}

Occurrences Cell::GetTerminalInstanceOccurrencesUnder(const Box& area) const
// ***********************************************************************
{
    return Cell_TerminalInstanceOccurrencesUnder(this, area);
}

Occurrences Cell::GetLeafInstanceOccurrences() const
// ***********************************************
{
    return Cell_LeafInstanceOccurrences(this);
}

Occurrences Cell::GetLeafInstanceOccurrencesUnder(const Box& area) const
// *******************************************************************
{
    return Cell_LeafInstanceOccurrencesUnder(this, area);
}

Occurrences Cell::GetComponentOccurrences(const Layer::Mask& mask) const
// *******************************************************************
{
    return Cell_ComponentOccurrences(this, mask);
}

Occurrences Cell::GetComponentOccurrencesUnder(const Box& area, const Layer::Mask& mask) const
// *****************************************************************************************
{
    return Cell_ComponentOccurrencesUnder(this, area, mask);
}

Pathes Cell::GetRecursiveSlavePathes() const
// *****************************************
{
  return Cell_RecursiveSlavePathes(this);
}

Occurrences Cell::GetHyperNetRootNetOccurrences() const
// *********************************
{
    return Cell_HyperNetRootNetOccurrences(this,Path());
}

// ****************************************************************************************************
// Cell_Slices implementation
// ****************************************************************************************************

Cell_Slices::Cell_Slices()
// ***********************
:     Inherit(),
    _cell(NULL),
    _mask(0)
{
}

Cell_Slices::Cell_Slices(const Cell* cell, const Layer::Mask& mask)
// ****************************************************************
:     Inherit(),
    _cell(cell),
    _mask(mask)
{
}

Cell_Slices::Cell_Slices(const Cell_Slices& slices)
// ************************************************
:     Inherit(),
    _cell(slices._cell),
    _mask(slices._mask)
{
}

Cell_Slices& Cell_Slices::operator=(const Cell_Slices& slices)
// ***********************************************************
{
    _cell = slices._cell;
    _mask = slices._mask;
    return *this;
}

Collection<Slice*>* Cell_Slices::GetClone() const
// **********************************************
{
    return new Cell_Slices(*this);
}

Locator<Slice*>* Cell_Slices::GetLocator() const
// *********************************************
{
    return new Locator(_cell, _mask);
}

string Cell_Slices::_GetString() const
// ***********************************
{
    string s = "<" + _TName("Cell::Slices");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_Slices::Locator implementation
// ****************************************************************************************************

Cell_Slices::Locator::Locator()
// ****************************
:    Inherit(),
    _cell(NULL),
    _mask(0),
    _sliceLocator()
{
}

Cell_Slices::Locator::Locator(const Cell* cell, const Layer::Mask& mask)
// *********************************************************************
:    Inherit(),
    _cell(cell),
    _mask(mask),
    _sliceLocator()
{
    if (_cell && (_mask != 0)) {
        _sliceLocator = ((Cell*)_cell)->_GetSliceMap().GetElements().GetLocator();
        while (_sliceLocator.IsValid() && !(_sliceLocator.GetElement()->GetLayer()->getMask() & _mask))
            _sliceLocator.Progress();
    }
}

Cell_Slices::Locator::Locator(const Locator& locator)
// **************************************************
:    Inherit(),
    _cell(locator._cell),
    _mask(locator._mask),
    _sliceLocator(locator._sliceLocator)
{
}

Cell_Slices::Locator& Cell_Slices::Locator::operator=(const Locator& locator)
// **************************************************************************
{
    _cell = locator._cell;
    _mask = locator._mask;
    _sliceLocator = locator._sliceLocator;
    return *this;
}

Slice* Cell_Slices::Locator::GetElement() const
// ********************************************
{
    return _sliceLocator.GetElement();
}

Locator<Slice*>* Cell_Slices::Locator::GetClone() const
// ****************************************************
{
    return new Locator(*this);
}

bool Cell_Slices::Locator::IsValid() const
// ***************************************
{
    return _sliceLocator.IsValid();
}

void Cell_Slices::Locator::Progress()
// **********************************
{
    if (_sliceLocator.IsValid()) {
        do {
            _sliceLocator.Progress();
        }
        while (_sliceLocator.IsValid() && !(_sliceLocator.GetElement()->GetLayer()->getMask() & _mask));
    }
}

string Cell_Slices::Locator::_GetString() const
// ********************************************
{
    string s = "<" + _TName("Cell::Slices::Locator");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_Components implementation
// ****************************************************************************************************

Cell_Components::Cell_Components()
// *******************************
:     Inherit(),
    _cell(NULL),
    _mask(0)
{
}

Cell_Components::Cell_Components(const Cell* cell, const Layer::Mask& mask)
// ************************************************************************
:     Inherit(),
    _cell(cell),
    _mask(mask)
{
}

Cell_Components::Cell_Components(const Cell_Components& components)
// ****************************************************************
:     Inherit(),
    _cell(components._cell),
    _mask(components._mask)
{
}

Cell_Components& Cell_Components::operator=(const Cell_Components& components)
// ***************************************************************************
{
    _cell = components._cell;
    _mask = components._mask;
    return *this;
}

Collection<Component*>* Cell_Components::GetClone() const
// ******************************************************
{
    return new Cell_Components(*this);
}

Locator<Component*>* Cell_Components::GetLocator() const
// *****************************************************
{
    return new Locator(_cell, _mask);
}

string Cell_Components::_GetString() const
// ***************************************
{
    string s = "<" + _TName("Cell::Components");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_Components::Locator implementation
// ****************************************************************************************************

Cell_Components::Locator::Locator()
// ********************************
:    Inherit(),
    _cell(NULL),
    _mask(0),
    _sliceLocator(),
    _componentLocator(),
    _component(NULL)
{
}

Cell_Components::Locator::Locator(const Cell* cell, const Layer::Mask& mask)
// *************************************************************************
:    Inherit(),
    _cell(cell),
    _mask(mask),
    _sliceLocator(),
    _componentLocator(),
    _component(NULL)
{
    if (_cell && (_mask != 0)) {
        _sliceLocator = _cell->GetSlices(_mask).GetLocator();
        while (!_component && _sliceLocator.IsValid()) {
            Slice* slice = _sliceLocator.GetElement();
            if (slice) {
                _componentLocator = slice->GetComponents().GetLocator();
                while (!_component && _componentLocator.IsValid()) {
                    _component = _componentLocator.GetElement();
                    if (!_component) _componentLocator.Progress();
                }
            }
            if (!_component) _sliceLocator.Progress();
        }
    }
}

Cell_Components::Locator::Locator(const Locator& locator)
// ******************************************************
:    Inherit(),
    _cell(locator._cell),
    _mask(locator._mask),
    _sliceLocator(locator._sliceLocator),
    _componentLocator(locator._componentLocator),
    _component(locator._component)
{
}

Cell_Components::Locator& Cell_Components::Locator::operator=(const Locator& locator)
// **********************************************************************************
{
    _cell = locator._cell;
    _mask = locator._mask;
    _sliceLocator = locator._sliceLocator;
    _componentLocator = locator._componentLocator;
    _component = locator._component;
    return *this;
}

Component* Cell_Components::Locator::GetElement() const
// ****************************************************
{
    return _component;
}

Locator<Component*>* Cell_Components::Locator::GetClone() const
// ************************************************************
{
    return new Locator(*this);
}

bool Cell_Components::Locator::IsValid() const
// *******************************************
{
    return (_component != NULL);
}

void Cell_Components::Locator::Progress()
// **************************************
{
    if (_component) {
        _component = NULL;
        do {
            _componentLocator.Progress();
            _component = _componentLocator.GetElement();
        }
        while (!_component && _componentLocator.IsValid());
        if (!_component) {
            do {
                _sliceLocator.Progress();
                Slice* slice = _sliceLocator.GetElement();
                if (slice) {
                    _componentLocator = slice->GetComponents().GetLocator();
                    while (!_component && _componentLocator.IsValid()) {
                        _component = _componentLocator.GetElement();
                        if (!_component) _componentLocator.Progress();
                    }
                }
            }
            while (!_component && _sliceLocator.IsValid());
        }
    }
}

string Cell_Components::Locator::_GetString() const
// ************************************************
{
    string s = "<" + _TName("Cell::Components::Locator");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentsUnder implementation
// ****************************************************************************************************

Cell_ComponentsUnder::Cell_ComponentsUnder()
// *****************************************
:     Inherit(),
    _cell(NULL),
    _area(),
    _mask(0)
{
}

Cell_ComponentsUnder::Cell_ComponentsUnder(const Cell* cell, const Box& area, const Layer::Mask& mask)
// ***************************************************************************************************
:     Inherit(),
    _cell(cell),
    _area(area),
    _mask(mask)
{
}

Cell_ComponentsUnder::Cell_ComponentsUnder(const Cell_ComponentsUnder& components)
// *******************************************************************************
:     Inherit(),
    _cell(components._cell),
    _area(components._area),
    _mask(components._mask)
{
}

Cell_ComponentsUnder& Cell_ComponentsUnder::operator=(const Cell_ComponentsUnder& components)
// ******************************************************************************************
{
    _cell = components._cell;
    _area = components._area;
    _mask = components._mask;
    return *this;
}

Collection<Component*>* Cell_ComponentsUnder::GetClone() const
// ***********************************************************
{
    return new Cell_ComponentsUnder(*this);
}

Locator<Component*>* Cell_ComponentsUnder::GetLocator() const
// **********************************************************
{
    return new Locator(_cell, _area, _mask);
}

string Cell_ComponentsUnder::_GetString() const
// ********************************************
{
    string s = "<" + _TName("Cell::ComponentsUnder");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_area);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentsUnder::Locator implementation
// ****************************************************************************************************

Cell_ComponentsUnder::Locator::Locator()
// *************************************
:    Inherit(),
    _cell(NULL),
    _area(),
    _mask(0),
    _sliceLocator(),
    _componentLocator(),
    _component(NULL)
{
}

Cell_ComponentsUnder::Locator::Locator(const Cell* cell, const Box& area, const Layer::Mask& mask)
// ***********************************************************************************************
:    Inherit(),
    _cell(cell),
    _area(area),
    _mask(mask),
    _sliceLocator(),
    _componentLocator(),
    _component(NULL)
{
    if (_cell && !_area.isEmpty()) {
        _sliceLocator = _cell->GetSlices(_mask).GetLocator();
        while (!_component && _sliceLocator.IsValid()) {
            Slice* slice = _sliceLocator.GetElement();
            if (slice) {
                _componentLocator = slice->GetComponentsUnder(_area).GetLocator();
                if (_componentLocator.IsValid())
                    _component = _componentLocator.GetElement();
            }
            if (!_component) _sliceLocator.Progress();
        }
    }
}

Cell_ComponentsUnder::Locator::Locator(const Locator& locator)
// ***********************************************************
:    Inherit(),
    _cell(locator._cell),
    _area(locator._area),
    _mask(locator._mask),
    _sliceLocator(locator._sliceLocator),
    _componentLocator(locator._componentLocator),
    _component(locator._component)
{
}

Cell_ComponentsUnder::Locator& Cell_ComponentsUnder::Locator::operator=(const Locator& locator)
// ********************************************************************************************
{
    _cell = locator._cell;
    _area = locator._area;
    _mask = locator._mask;
    _sliceLocator = locator._sliceLocator;
    _componentLocator = locator._componentLocator;
    _component = locator._component;
    return *this;
}

Component* Cell_ComponentsUnder::Locator::GetElement() const
// *********************************************************
{
    return _component;
}

Locator<Component*>* Cell_ComponentsUnder::Locator::GetClone() const
// *****************************************************************
{
    return new Locator(*this);
}

bool Cell_ComponentsUnder::Locator::IsValid() const
// ************************************************
{
    return (_component != NULL);
}

void Cell_ComponentsUnder::Locator::Progress()
// *******************************************
{
    if (_component) {
        _component = NULL;
        _componentLocator.Progress();
        if (_componentLocator.IsValid())
            _component = _componentLocator.GetElement();
        else {
            do {
                _sliceLocator.Progress();
                Slice* slice = _sliceLocator.GetElement();
                if (slice) {
                    _componentLocator = slice->GetComponentsUnder(_area).GetLocator();
                    if (_componentLocator.IsValid())
                        _component = _componentLocator.GetElement();
                }
            } while (!_component && _sliceLocator.IsValid());
        }
    }
}

string Cell_ComponentsUnder::Locator::_GetString() const
// *****************************************************
{
    string s = "<" + _TName("Cell::ComponentsUnder::Locator");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_area);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_Occurrences implementation
// ****************************************************************************************************

Cell_Occurrences::Cell_Occurrences()
// *******************************
:     Inherit(),
    _cell(NULL),
    _searchDepth(0)
{
}

Cell_Occurrences::Cell_Occurrences(const Cell* cell, unsigned searchDepth)
// *********************************************************************
:     Inherit(),
    _cell(cell),
    _searchDepth(searchDepth)
{
}

Cell_Occurrences::Cell_Occurrences(const Cell_Occurrences& occurrences)
// ****************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _searchDepth(occurrences._searchDepth)
{
}

Cell_Occurrences& Cell_Occurrences::operator=(const Cell_Occurrences& occurrences)
// ***************************************************************************
{
    _cell = occurrences._cell;
    _searchDepth = occurrences._searchDepth;
    return *this;
}

Collection<Occurrence>* Cell_Occurrences::GetClone() const
// *****************************************************
{
    return new Cell_Occurrences(*this);
}

Locator<Occurrence>* Cell_Occurrences::GetLocator() const
// ****************************************************
{
    return new Locator(_cell, _searchDepth);
}

string Cell_Occurrences::_GetString() const
// ***************************************
{
    string s = "<" + _TName("Cell::Occurrences");
    if (_cell) {
        s += " " + GetString(_cell);
        if (_searchDepth != ((unsigned)-1)) s += " " + GetString(_searchDepth);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_Occurrences::Locator implementation
// ****************************************************************************************************

Cell_Occurrences::Locator::Locator()
// ********************************
:    Inherit(),
    _cell(NULL),
    _searchDepth(0),
    _state(0),
    _componentLocator(),
    _rubberLocator(),
    _markerLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
}

Cell_Occurrences::Locator::Locator(const Cell* cell, unsigned searchDepth)
// **********************************************************************
:    Inherit(),
    _cell(cell),
    _searchDepth(searchDepth),
    _state(0),
    _componentLocator(),
    _rubberLocator(),
    _markerLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
    if (_cell) {
        _componentLocator = _cell->GetComponents().GetLocator();
        if (_componentLocator.IsValid())
            _state = 1;
        else {
            _rubberLocator = _cell->GetRubbers().GetLocator();
            if (_rubberLocator.IsValid())
                _state = 2;
            else {
                _markerLocator = _cell->GetMarkers().GetLocator();
                if (_markerLocator.IsValid())
                    _state = 3;
                else {
                    _instanceLocator = _cell->GetInstances().GetLocator();
                    if (_instanceLocator.IsValid())
                        _state = 4;
                }
            }
        }
    }
}

Cell_Occurrences::Locator::Locator(const Locator& locator)
// ******************************************************
:    Inherit(),
    _cell(locator._cell),
    _searchDepth(locator._searchDepth),
    _state(locator._state),
    _componentLocator(locator._componentLocator),
    _rubberLocator(locator._rubberLocator),
    _markerLocator(locator._markerLocator),
    _instanceLocator(locator._instanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_Occurrences::Locator& Cell_Occurrences::Locator::operator=(const Locator& locator)
// **********************************************************************************
{
    _cell = locator._cell;
    _searchDepth = locator._searchDepth;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
    _rubberLocator = locator._rubberLocator;
    _markerLocator = locator._markerLocator;
    _instanceLocator = locator._instanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_Occurrences::Locator::GetElement() const
// ***************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_componentLocator.GetElement());
            case 2 : return Occurrence(_rubberLocator.GetElement());
            case 3 : return Occurrence(_markerLocator.GetElement());
            case 4 : return Occurrence(_instanceLocator.GetElement());
            case 5 : {
                Occurrence occurrence = _occurrenceLocator.GetElement();
                Entity* entity = occurrence.GetEntity();
                Path path = Path(_instanceLocator.GetElement(), occurrence.GetPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_Occurrences::Locator::GetClone() const
// ***********************************************************
{
    return new Locator(*this);
}

bool Cell_Occurrences::Locator::IsValid() const
// *******************************************
{
    return (_state != 0);
}

void Cell_Occurrences::Locator::Progress()
// **************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _componentLocator.Progress();
                if (!_componentLocator.IsValid()) {
                    _rubberLocator = _cell->GetRubbers().GetLocator();
                    if (_rubberLocator.IsValid())
                        _state = 2;
                    else {
                        _markerLocator = _cell->GetMarkers().GetLocator();
                        if (_markerLocator.IsValid())
                            _state = 3;
                        else {
                            _instanceLocator = _cell->GetInstances().GetLocator();
                            if (_instanceLocator.IsValid())
                                _state = 4;
                            else
                                _state = 0;
                        }
                    }
                }
                break;
            case 2 :
                _rubberLocator.Progress();
                if (!_rubberLocator.IsValid()) {
                    _markerLocator = _cell->GetMarkers().GetLocator();
                    if (_markerLocator.IsValid())
                        _state = 3;
                    else {
                        _instanceLocator = _cell->GetInstances().GetLocator();
                        if (_instanceLocator.IsValid())
                            _state = 4;
                        else
                            _state = 0;
                    }
                }
                break;
            case 3 :
                _markerLocator.Progress();
                if (!_markerLocator.IsValid()) {
                    _instanceLocator = _cell->GetInstances().GetLocator();
                    if (_instanceLocator.IsValid())
                        _state = 4;
                    else
                        _state = 0;
                }
                break;
            case 4 :
                if (!_searchDepth) {
                    _instanceLocator.Progress();
                    if (!_instanceLocator.IsValid()) _state = 0;
                }
                else {
                    Instance* instance = _instanceLocator.GetElement();
                    if (instance->IsTerminal()) {
                        _instanceLocator.Progress();
                        if (!_instanceLocator.IsValid()) _state = 0;
                    }
                    else {
                        _occurrenceLocator =
                            instance->GetMasterCell()->GetOccurrences(_searchDepth - 1).GetLocator();
                        if (_occurrenceLocator.IsValid())
                            _state = 5;
                        else {
                            _instanceLocator.Progress();
                            if (!_instanceLocator.IsValid()) _state = 0;
                        }
                    }
                }
                break;
            case 5 :
                _occurrenceLocator.Progress();
                if (!_occurrenceLocator.IsValid()) {
                    _instanceLocator.Progress();
                    if (_instanceLocator.IsValid())
                        _state = 4;
                    else
                        _state = 0;
                }
                break;
        }
    }
}

string Cell_Occurrences::Locator::_GetString() const
// ************************************************
{
    string s = "<" + _TName("Cell::Occurrences::Locator");
    if (_cell) {
        s += " " + GetString(_cell);
        if (_searchDepth != ((unsigned)-1)) s += " " + GetString(_searchDepth);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_OccurrencesUnder implementation
// ****************************************************************************************************

Cell_OccurrencesUnder::Cell_OccurrencesUnder()
// *****************************************
:     Inherit(),
    _cell(NULL),
    _area(),
    _searchDepth(0)
{
}

Cell_OccurrencesUnder::Cell_OccurrencesUnder(const Cell* cell, const Box& area, unsigned searchDepth)
// ************************************************************************************************
:     Inherit(),
    _cell(cell),
    _area(area),
    _searchDepth(searchDepth)
{
}

Cell_OccurrencesUnder::Cell_OccurrencesUnder(const Cell_OccurrencesUnder& occurrences)
// *******************************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _area(occurrences._area),
    _searchDepth(occurrences._searchDepth)
{
}

Cell_OccurrencesUnder& Cell_OccurrencesUnder::operator=(const Cell_OccurrencesUnder& occurrences)
// ******************************************************************************************
{
    _cell = occurrences._cell;
    _area = occurrences._area;
    _searchDepth = occurrences._searchDepth;
    return *this;
}

Collection<Occurrence>* Cell_OccurrencesUnder::GetClone() const
// **********************************************************
{
    return new Cell_OccurrencesUnder(*this);
}

Locator<Occurrence>* Cell_OccurrencesUnder::GetLocator() const
// *********************************************************
{
    return new Locator(_cell, _area, _searchDepth);
}

string Cell_OccurrencesUnder::_GetString() const
// ********************************************
{
    string s = "<" + _TName("Cell::OccurrencesUnder");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_area);
        if (_searchDepth != ((unsigned)-1)) s += " " + GetString(_searchDepth);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_OccurrencesUnder::Locator implementation
// ****************************************************************************************************

Cell_OccurrencesUnder::Locator::Locator()
// *************************************
:    Inherit(),
    _cell(NULL),
    _area(),
    _searchDepth(0),
    _state(0),
    _componentLocator(),
    _rubberLocator(),
    _markerLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
}

Cell_OccurrencesUnder::Locator::Locator(const Cell* cell, const Box& area, unsigned searchDepth)
// ********************************************************************************************
:    Inherit(),
    _cell(cell),
    _area(area),
    _searchDepth(searchDepth),
    _state(0),
    _componentLocator(),
    _rubberLocator(),
    _markerLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
    if (_cell && !_area.isEmpty()) {
        _componentLocator = _cell->GetComponentsUnder(_area).GetLocator();
        if (_componentLocator.IsValid())
            _state = 1;
        else {
            _rubberLocator = _cell->GetRubbersUnder(_area).GetLocator();
            if (_rubberLocator.IsValid())
                _state = 2;
            else {
                _markerLocator = _cell->GetMarkersUnder(_area).GetLocator();
                if (_markerLocator.IsValid())
                    _state = 3;
                else {
                    _instanceLocator = _cell->GetInstancesUnder(_area).GetLocator();
                    if (_instanceLocator.IsValid())
                        _state = 4;
                }
            }
        }
    }
}

Cell_OccurrencesUnder::Locator::Locator(const Locator& locator)
// ***********************************************************
:    Inherit(),
    _cell(locator._cell),
    _area(locator._area),
    _searchDepth(locator._searchDepth),
    _state(locator._state),
    _componentLocator(locator._componentLocator),
    _rubberLocator(locator._rubberLocator),
    _markerLocator(locator._markerLocator),
    _instanceLocator(locator._instanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_OccurrencesUnder::Locator& Cell_OccurrencesUnder::Locator::operator=(const Locator& locator)
// ********************************************************************************************
{
    _cell = locator._cell;
    _area = locator._area;
    _searchDepth = locator._searchDepth;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
    _rubberLocator = locator._rubberLocator;
    _markerLocator = locator._markerLocator;
    _instanceLocator = locator._instanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_OccurrencesUnder::Locator::GetElement() const
// ********************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_componentLocator.GetElement());
            case 2 : return Occurrence(_rubberLocator.GetElement());
            case 3 : return Occurrence(_markerLocator.GetElement());
            case 4 : return Occurrence(_instanceLocator.GetElement());
            case 5 : {
                Occurrence occurrence = _occurrenceLocator.GetElement();
                Entity* entity = occurrence.GetEntity();
                Path path = Path(_instanceLocator.GetElement(), occurrence.GetPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_OccurrencesUnder::Locator::GetClone() const
// ****************************************************************
{
    return new Locator(*this);
}

bool Cell_OccurrencesUnder::Locator::IsValid() const
// ************************************************
{
    return (_state != 0);
}

void Cell_OccurrencesUnder::Locator::Progress()
// *******************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _componentLocator.Progress();
                if (!_componentLocator.IsValid()) {
                    _rubberLocator = _cell->GetRubbersUnder(_area).GetLocator();
                    if (_rubberLocator.IsValid())
                        _state = 2;
                    else {
                        _markerLocator = _cell->GetMarkersUnder(_area).GetLocator();
                        if (_markerLocator.IsValid())
                            _state = 3;
                        else {
                            _instanceLocator = _cell->GetInstancesUnder(_area).GetLocator();
                            if (_instanceLocator.IsValid())
                                _state = 4;
                            else
                                _state = 0;
                        }
                    }
                }
                break;
            case 2 :
                _rubberLocator.Progress();
                if (!_rubberLocator.IsValid()) {
                    _markerLocator = _cell->GetMarkersUnder(_area).GetLocator();
                    if (_markerLocator.IsValid())
                        _state = 3;
                    else {
                        _instanceLocator = _cell->GetInstancesUnder(_area).GetLocator();
                        if (_instanceLocator.IsValid())
                            _state = 4;
                        else
                            _state = 0;
                    }
                }
                break;
            case 3 :
                _markerLocator.Progress();
                if (!_markerLocator.IsValid()) {
                    _instanceLocator = _cell->GetInstancesUnder(_area).GetLocator();
                    if (_instanceLocator.IsValid())
                        _state = 4;
                    else
                        _state = 0;
                }
                break;
            case 4 :
                if (!_searchDepth) {
                    _instanceLocator.Progress();
                    if (!_instanceLocator.IsValid()) _state = 0;
                }
                else {
                    Instance* instance = _instanceLocator.GetElement();
                    if (instance->IsTerminal()) {
                        _instanceLocator.Progress();
                        if (!_instanceLocator.IsValid()) _state = 0;
                    }
                    else {
                        Box masterArea = _area;
                        instance->GetTransformation().getInvert().applyOn(masterArea);
                        Cell* masterCell = instance->GetMasterCell();
                        _occurrenceLocator =
                            masterCell->GetOccurrencesUnder(masterArea, _searchDepth - 1).GetLocator();
                        if (_occurrenceLocator.IsValid())
                            _state = 5;
                        else {
                            _instanceLocator.Progress();
                            if (!_instanceLocator.IsValid()) _state = 0;
                        }
                    }
                }
                break;
            case 5 :
                _occurrenceLocator.Progress();
                if (!_occurrenceLocator.IsValid()) {
                    _instanceLocator.Progress();
                    if (_instanceLocator.IsValid())
                        _state = 4;
                    else
                        _state = 0;
                }
                break;
        }
    }
}

string Cell_OccurrencesUnder::Locator::_GetString() const
// *****************************************************
{
    string s = "<" + _TName("Cell::OccurrencesUnder::Locator");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_area);
        if (_searchDepth != ((unsigned)-1)) s += " " + GetString(_searchDepth);
    }
    s += ">";
    return s;
}

// ****************************************************************************************************
// Cell_LeafInstanceOccurrences implementation
// ****************************************************************************************************

Cell_LeafInstanceOccurrences::Cell_LeafInstanceOccurrences(const Cell* cell)
// *******************************************************************************
:     Inherit(),
    _cell(cell)
{
}

Cell_LeafInstanceOccurrences::Cell_LeafInstanceOccurrences(const Cell_LeafInstanceOccurrences& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell)
{
}

Cell_LeafInstanceOccurrences& Cell_LeafInstanceOccurrences::operator=(const Cell_LeafInstanceOccurrences& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    return *this;
}

Collection<Occurrence>* Cell_LeafInstanceOccurrences::GetClone() const
// *********************************************************************
{
    return new Cell_LeafInstanceOccurrences(*this);
}

Locator<Occurrence>* Cell_LeafInstanceOccurrences::GetLocator() const
// ********************************************************************
{
    return new Locator(_cell);
}

string Cell_LeafInstanceOccurrences::_GetString() const
// *******************************************************
{
    string s = "<" + _TName("Cell::LeafInstanceOccurrences");
    if (_cell) s += " " + GetString(_cell);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_LeafInstanceOccurrences::Locator implementation
// ****************************************************************************************************

Cell_LeafInstanceOccurrences::Locator::Locator(const Cell* cell)
// ****************************************************************
:    Inherit(),
    _cell(cell),
    _state(0),
    _leafInstanceLocator(),
    _nonLeafInstanceLocator(),
    _occurrenceLocator()
{
    if (_cell) {
        _leafInstanceLocator = _cell->GetLeafInstances().GetLocator();
        if (_leafInstanceLocator.IsValid())
            _state = 1;
        else {
            _nonLeafInstanceLocator = _cell->GetNonLeafInstances().GetLocator();
            while (!_state && _nonLeafInstanceLocator.IsValid()) {
                Cell* masterCell = _nonLeafInstanceLocator.GetElement()->GetMasterCell();
                _occurrenceLocator = masterCell->GetLeafInstanceOccurrences().GetLocator();
                if (_occurrenceLocator.IsValid())
                    _state = 2;
                else
                    _nonLeafInstanceLocator.Progress();
            }
        }
    }
}

Cell_LeafInstanceOccurrences::Locator::Locator(const Locator& locator)
// **********************************************************************
:    Inherit(),
    _cell(locator._cell),
    _state(locator._state),
    _leafInstanceLocator(locator._leafInstanceLocator),
    _nonLeafInstanceLocator(locator._nonLeafInstanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_LeafInstanceOccurrences::Locator& Cell_LeafInstanceOccurrences::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _state = locator._state;
    _leafInstanceLocator = locator._leafInstanceLocator;
    _nonLeafInstanceLocator = locator._nonLeafInstanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_LeafInstanceOccurrences::Locator::GetElement() const
// *******************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_leafInstanceLocator.GetElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.GetElement();
                Entity* entity = occurrence.GetEntity();
                Path path = Path(_nonLeafInstanceLocator.GetElement(), occurrence.GetPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_LeafInstanceOccurrences::Locator::GetClone() const
// ***************************************************************************
{
    return new Locator(*this);
}

bool Cell_LeafInstanceOccurrences::Locator::IsValid() const
// ***********************************************************
{
    return (_state != 0);
}

void Cell_LeafInstanceOccurrences::Locator::Progress()
// ******************************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _leafInstanceLocator.Progress();
                if (!_leafInstanceLocator.IsValid()) {
                    _state = 0;
                    _nonLeafInstanceLocator = _cell->GetNonLeafInstances().GetLocator();
                    while (!_state && _nonLeafInstanceLocator.IsValid()) {
                        Cell* masterCell = _nonLeafInstanceLocator.GetElement()->GetMasterCell();
                        _occurrenceLocator = masterCell->GetLeafInstanceOccurrences().GetLocator();
                        if (_occurrenceLocator.IsValid())
                            _state = 2;
                        else
                            _nonLeafInstanceLocator.Progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.Progress();
                if (!_occurrenceLocator.IsValid()) {
                    _state = 0;
                    if (_nonLeafInstanceLocator.IsValid()) {
                        _nonLeafInstanceLocator.Progress();
                        while (!_state && _nonLeafInstanceLocator.IsValid()) {
                            Cell* masterCell = _nonLeafInstanceLocator.GetElement()->GetMasterCell();
                            _occurrenceLocator =
                                masterCell->GetLeafInstanceOccurrences().GetLocator();
                            if (_occurrenceLocator.IsValid())
                                _state = 2;
                            else
                                _nonLeafInstanceLocator.Progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_LeafInstanceOccurrences::Locator::_GetString() const
// ****************************************************************
{
    string s = "<" + _TName("Cell::LeafInstanceOccurrences::Locator");
    if (_cell) s += " " + GetString(_cell);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_LeafInstanceOccurrencesUnder implementation
// ****************************************************************************************************

Cell_LeafInstanceOccurrencesUnder::Cell_LeafInstanceOccurrencesUnder()
// *************************************************************************
:     Inherit(),
    _cell(NULL),
    _area()
{
}

Cell_LeafInstanceOccurrencesUnder::Cell_LeafInstanceOccurrencesUnder(const Cell* cell, const Box& area)
// ****************************************************************************************************
:     Inherit(),
    _cell(cell),
    _area(area)
{
}

Cell_LeafInstanceOccurrencesUnder::Cell_LeafInstanceOccurrencesUnder(const Cell_LeafInstanceOccurrencesUnder& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _area(occurrences._area)
{
}

Cell_LeafInstanceOccurrencesUnder& Cell_LeafInstanceOccurrencesUnder::operator=(const Cell_LeafInstanceOccurrencesUnder& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    _area = occurrences._area;
    return *this;
}

Collection<Occurrence>* Cell_LeafInstanceOccurrencesUnder::GetClone() const
// **************************************************************************
{
    return new Cell_LeafInstanceOccurrencesUnder(*this);
}

Locator<Occurrence>* Cell_LeafInstanceOccurrencesUnder::GetLocator() const
// *************************************************************************
{
    return new Locator(_cell, _area);
}

string Cell_LeafInstanceOccurrencesUnder::_GetString() const
// ************************************************************
{
    string s = "<" + _TName("Cell::LeafInstanceOccurrencesUnder");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_area);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_LeafInstanceOccurrencesUnder::Locator implementation
// ****************************************************************************************************

Cell_LeafInstanceOccurrencesUnder::Locator::Locator()
// *****************************************************
:    Inherit(),
    _cell(NULL),
    _area(),
    _state(0),
    _leafInstanceLocator(),
    _nonLeafInstanceLocator(),
    _occurrenceLocator()
{
}

Cell_LeafInstanceOccurrencesUnder::Locator::Locator(const Cell* cell, const Box& area)
// **************************************************************************************
:    Inherit(),
    _cell(cell),
    _area(area),
    _state(0),
    _leafInstanceLocator(),
    _nonLeafInstanceLocator(),
    _occurrenceLocator()
{
    if (_cell && !_area.isEmpty()) {
        _leafInstanceLocator = _cell->GetLeafInstancesUnder(_area).GetLocator();
        if (_leafInstanceLocator.IsValid())
            _state = 1;
        else {
            _nonLeafInstanceLocator = _cell->GetNonLeafInstancesUnder(_area).GetLocator();
            while (!_state && _nonLeafInstanceLocator.IsValid()) {
                Instance* instance = _nonLeafInstanceLocator.GetElement();
                Cell* masterCell = instance->GetMasterCell();
                Box masterArea = _area;
                instance->GetTransformation().getInvert().applyOn(masterArea);
                _occurrenceLocator =
                    masterCell->GetLeafInstanceOccurrencesUnder(masterArea).GetLocator();
                if (_occurrenceLocator.IsValid())
                    _state = 2;
                else
                    _nonLeafInstanceLocator.Progress();
            }
        }
    }
}

Cell_LeafInstanceOccurrencesUnder::Locator::Locator(const Locator& locator)
// ***************************************************************************
:    Inherit(),
    _cell(locator._cell),
    _area(locator._area),
    _state(locator._state),
    _leafInstanceLocator(locator._leafInstanceLocator),
    _nonLeafInstanceLocator(locator._nonLeafInstanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_LeafInstanceOccurrencesUnder::Locator& Cell_LeafInstanceOccurrencesUnder::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _area = locator._area;
    _state = locator._state;
    _leafInstanceLocator = locator._leafInstanceLocator;
    _nonLeafInstanceLocator = locator._nonLeafInstanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_LeafInstanceOccurrencesUnder::Locator::GetElement() const
// ************************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_leafInstanceLocator.GetElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.GetElement();
                Entity* entity = occurrence.GetEntity();
                Path path = Path(_nonLeafInstanceLocator.GetElement(), occurrence.GetPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_LeafInstanceOccurrencesUnder::Locator::GetClone() const
// ********************************************************************************
{
    return new Locator(*this);
}

bool Cell_LeafInstanceOccurrencesUnder::Locator::IsValid() const
// ****************************************************************
{
    return (_state != 0);
}

void Cell_LeafInstanceOccurrencesUnder::Locator::Progress()
// ***********************************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _leafInstanceLocator.Progress();
                if (!_leafInstanceLocator.IsValid()) {
                    _state = 0;
                    _nonLeafInstanceLocator = _cell->GetNonLeafInstancesUnder(_area).GetLocator();
                    while (!_state && _nonLeafInstanceLocator.IsValid()) {
                        Instance* instance = _nonLeafInstanceLocator.GetElement();
                        Cell* masterCell = instance->GetMasterCell();
                        Box masterArea = _area;
                        instance->GetTransformation().getInvert().applyOn(masterArea);
                        _occurrenceLocator =
                            masterCell->GetLeafInstanceOccurrencesUnder(masterArea).GetLocator();
                        if (_occurrenceLocator.IsValid())
                            _state = 2;
                        else
                            _nonLeafInstanceLocator.Progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.Progress();
                if (!_occurrenceLocator.IsValid()) {
                    _state = 0;
                    if (_nonLeafInstanceLocator.IsValid()) {
                        _nonLeafInstanceLocator.Progress();
                        while (!_state && _nonLeafInstanceLocator.IsValid()) {
                            Instance* instance = _nonLeafInstanceLocator.GetElement();
                            Cell* masterCell = instance->GetMasterCell();
                            Box masterArea = _area;
                            instance->GetTransformation().getInvert().applyOn(masterArea);
                            _occurrenceLocator =
                                masterCell->GetLeafInstanceOccurrencesUnder(masterArea).GetLocator();
                            if (_occurrenceLocator.IsValid())
                                _state = 2;
                            else
                                _nonLeafInstanceLocator.Progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_LeafInstanceOccurrencesUnder::Locator::_GetString() const
// *********************************************************************
{
    string s = "<" + _TName("Cell::LeafInstanceOccurrencesUnder::Locator");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_area);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrences implementation
// ****************************************************************************************************

Cell_TerminalInstanceOccurrences::Cell_TerminalInstanceOccurrences(const Cell* cell)
// *******************************************************************************
:     Inherit(),
    _cell(cell)
{
}

Cell_TerminalInstanceOccurrences::Cell_TerminalInstanceOccurrences(const Cell_TerminalInstanceOccurrences& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell)
{
}

Cell_TerminalInstanceOccurrences& Cell_TerminalInstanceOccurrences::operator=(const Cell_TerminalInstanceOccurrences& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    return *this;
}

Collection<Occurrence>* Cell_TerminalInstanceOccurrences::GetClone() const
// *********************************************************************
{
    return new Cell_TerminalInstanceOccurrences(*this);
}

Locator<Occurrence>* Cell_TerminalInstanceOccurrences::GetLocator() const
// ********************************************************************
{
    return new Locator(_cell);
}

string Cell_TerminalInstanceOccurrences::_GetString() const
// *******************************************************
{
    string s = "<" + _TName("Cell::TerminalInstanceOccurrences");
    if (_cell) s += " " + GetString(_cell);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrences::Locator implementation
// ****************************************************************************************************

Cell_TerminalInstanceOccurrences::Locator::Locator(const Cell* cell)
// ****************************************************************
:    Inherit(),
    _cell(cell),
    _state(0),
    _terminalInstanceLocator(),
    _nonTerminalInstanceLocator(),
    _occurrenceLocator()
{
    if (_cell) {
        _terminalInstanceLocator = _cell->GetTerminalInstances().GetLocator();
        if (_terminalInstanceLocator.IsValid())
            _state = 1;
        else {
            _nonTerminalInstanceLocator = _cell->GetNonTerminalInstances().GetLocator();
            while (!_state && _nonTerminalInstanceLocator.IsValid()) {
                Cell* masterCell = _nonTerminalInstanceLocator.GetElement()->GetMasterCell();
                _occurrenceLocator = masterCell->GetTerminalInstanceOccurrences().GetLocator();
                if (_occurrenceLocator.IsValid())
                    _state = 2;
                else
                    _nonTerminalInstanceLocator.Progress();
            }
        }
    }
}

Cell_TerminalInstanceOccurrences::Locator::Locator(const Locator& locator)
// **********************************************************************
:    Inherit(),
    _cell(locator._cell),
    _state(locator._state),
    _terminalInstanceLocator(locator._terminalInstanceLocator),
    _nonTerminalInstanceLocator(locator._nonTerminalInstanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_TerminalInstanceOccurrences::Locator& Cell_TerminalInstanceOccurrences::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _state = locator._state;
    _terminalInstanceLocator = locator._terminalInstanceLocator;
    _nonTerminalInstanceLocator = locator._nonTerminalInstanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_TerminalInstanceOccurrences::Locator::GetElement() const
// *******************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_terminalInstanceLocator.GetElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.GetElement();
                Entity* entity = occurrence.GetEntity();
                Path path = Path(_nonTerminalInstanceLocator.GetElement(), occurrence.GetPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_TerminalInstanceOccurrences::Locator::GetClone() const
// ***************************************************************************
{
    return new Locator(*this);
}

bool Cell_TerminalInstanceOccurrences::Locator::IsValid() const
// ***********************************************************
{
    return (_state != 0);
}

void Cell_TerminalInstanceOccurrences::Locator::Progress()
// ******************************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _terminalInstanceLocator.Progress();
                if (!_terminalInstanceLocator.IsValid()) {
                    _state = 0;
                    _nonTerminalInstanceLocator = _cell->GetNonTerminalInstances().GetLocator();
                    while (!_state && _nonTerminalInstanceLocator.IsValid()) {
                        Cell* masterCell = _nonTerminalInstanceLocator.GetElement()->GetMasterCell();
                        _occurrenceLocator = masterCell->GetTerminalInstanceOccurrences().GetLocator();
                        if (_occurrenceLocator.IsValid())
                            _state = 2;
                        else
                            _nonTerminalInstanceLocator.Progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.Progress();
                if (!_occurrenceLocator.IsValid()) {
                    _state = 0;
                    if (_nonTerminalInstanceLocator.IsValid()) {
                        _nonTerminalInstanceLocator.Progress();
                        while (!_state && _nonTerminalInstanceLocator.IsValid()) {
                            Cell* masterCell = _nonTerminalInstanceLocator.GetElement()->GetMasterCell();
                            _occurrenceLocator =
                                masterCell->GetTerminalInstanceOccurrences().GetLocator();
                            if (_occurrenceLocator.IsValid())
                                _state = 2;
                            else
                                _nonTerminalInstanceLocator.Progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_TerminalInstanceOccurrences::Locator::_GetString() const
// ****************************************************************
{
    string s = "<" + _TName("Cell::TerminalInstanceOccurrences::Locator");
    if (_cell) s += " " + GetString(_cell);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrencesUnder implementation
// ****************************************************************************************************

Cell_TerminalInstanceOccurrencesUnder::Cell_TerminalInstanceOccurrencesUnder()
// *************************************************************************
:     Inherit(),
    _cell(NULL),
    _area()
{
}

Cell_TerminalInstanceOccurrencesUnder::Cell_TerminalInstanceOccurrencesUnder(const Cell* cell, const Box& area)
// ****************************************************************************************************
:     Inherit(),
    _cell(cell),
    _area(area)
{
}

Cell_TerminalInstanceOccurrencesUnder::Cell_TerminalInstanceOccurrencesUnder(const Cell_TerminalInstanceOccurrencesUnder& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _area(occurrences._area)
{
}

Cell_TerminalInstanceOccurrencesUnder& Cell_TerminalInstanceOccurrencesUnder::operator=(const Cell_TerminalInstanceOccurrencesUnder& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    _area = occurrences._area;
    return *this;
}

Collection<Occurrence>* Cell_TerminalInstanceOccurrencesUnder::GetClone() const
// **************************************************************************
{
    return new Cell_TerminalInstanceOccurrencesUnder(*this);
}

Locator<Occurrence>* Cell_TerminalInstanceOccurrencesUnder::GetLocator() const
// *************************************************************************
{
    return new Locator(_cell, _area);
}

string Cell_TerminalInstanceOccurrencesUnder::_GetString() const
// ************************************************************
{
    string s = "<" + _TName("Cell::TerminalInstanceOccurrencesUnder");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_area);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrencesUnder::Locator implementation
// ****************************************************************************************************

Cell_TerminalInstanceOccurrencesUnder::Locator::Locator()
// *****************************************************
:    Inherit(),
    _cell(NULL),
    _area(),
    _state(0),
    _terminalInstanceLocator(),
    _nonTerminalInstanceLocator(),
    _occurrenceLocator()
{
}

Cell_TerminalInstanceOccurrencesUnder::Locator::Locator(const Cell* cell, const Box& area)
// **************************************************************************************
:    Inherit(),
    _cell(cell),
    _area(area),
    _state(0),
    _terminalInstanceLocator(),
    _nonTerminalInstanceLocator(),
    _occurrenceLocator()
{
    if (_cell && !_area.isEmpty()) {
        _terminalInstanceLocator = _cell->GetTerminalInstancesUnder(_area).GetLocator();
        if (_terminalInstanceLocator.IsValid())
            _state = 1;
        else {
            _nonTerminalInstanceLocator = _cell->GetNonTerminalInstancesUnder(_area).GetLocator();
            while (!_state && _nonTerminalInstanceLocator.IsValid()) {
                Instance* instance = _nonTerminalInstanceLocator.GetElement();
                Cell* masterCell = instance->GetMasterCell();
                Box masterArea = _area;
                instance->GetTransformation().getInvert().applyOn(masterArea);
                _occurrenceLocator =
                    masterCell->GetTerminalInstanceOccurrencesUnder(masterArea).GetLocator();
                if (_occurrenceLocator.IsValid())
                    _state = 2;
                else
                    _nonTerminalInstanceLocator.Progress();
            }
        }
    }
}

Cell_TerminalInstanceOccurrencesUnder::Locator::Locator(const Locator& locator)
// ***************************************************************************
:    Inherit(),
    _cell(locator._cell),
    _area(locator._area),
    _state(locator._state),
    _terminalInstanceLocator(locator._terminalInstanceLocator),
    _nonTerminalInstanceLocator(locator._nonTerminalInstanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_TerminalInstanceOccurrencesUnder::Locator& Cell_TerminalInstanceOccurrencesUnder::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _area = locator._area;
    _state = locator._state;
    _terminalInstanceLocator = locator._terminalInstanceLocator;
    _nonTerminalInstanceLocator = locator._nonTerminalInstanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_TerminalInstanceOccurrencesUnder::Locator::GetElement() const
// ************************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_terminalInstanceLocator.GetElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.GetElement();
                Entity* entity = occurrence.GetEntity();
                Path path = Path(_nonTerminalInstanceLocator.GetElement(), occurrence.GetPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_TerminalInstanceOccurrencesUnder::Locator::GetClone() const
// ********************************************************************************
{
    return new Locator(*this);
}

bool Cell_TerminalInstanceOccurrencesUnder::Locator::IsValid() const
// ****************************************************************
{
    return (_state != 0);
}

void Cell_TerminalInstanceOccurrencesUnder::Locator::Progress()
// ***********************************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _terminalInstanceLocator.Progress();
                if (!_terminalInstanceLocator.IsValid()) {
                    _state = 0;
                    _nonTerminalInstanceLocator = _cell->GetNonTerminalInstancesUnder(_area).GetLocator();
                    while (!_state && _nonTerminalInstanceLocator.IsValid()) {
                        Instance* instance = _nonTerminalInstanceLocator.GetElement();
                        Cell* masterCell = instance->GetMasterCell();
                        Box masterArea = _area;
                        instance->GetTransformation().getInvert().applyOn(masterArea);
                        _occurrenceLocator =
                            masterCell->GetTerminalInstanceOccurrencesUnder(masterArea).GetLocator();
                        if (_occurrenceLocator.IsValid())
                            _state = 2;
                        else
                            _nonTerminalInstanceLocator.Progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.Progress();
                if (!_occurrenceLocator.IsValid()) {
                    _state = 0;
                    if (_nonTerminalInstanceLocator.IsValid()) {
                        _nonTerminalInstanceLocator.Progress();
                        while (!_state && _nonTerminalInstanceLocator.IsValid()) {
                            Instance* instance = _nonTerminalInstanceLocator.GetElement();
                            Cell* masterCell = instance->GetMasterCell();
                            Box masterArea = _area;
                            instance->GetTransformation().getInvert().applyOn(masterArea);
                            _occurrenceLocator =
                                masterCell->GetTerminalInstanceOccurrencesUnder(masterArea).GetLocator();
                            if (_occurrenceLocator.IsValid())
                                _state = 2;
                            else
                                _nonTerminalInstanceLocator.Progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_TerminalInstanceOccurrencesUnder::Locator::_GetString() const
// *********************************************************************
{
    string s = "<" + _TName("Cell::TerminalInstanceOccurrencesUnder::Locator");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_area);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentOccurrences implementation
// ****************************************************************************************************

Cell_ComponentOccurrences::Cell_ComponentOccurrences()
// *************************************************
:     Inherit(),
    _cell(NULL),
    _mask(0)
{
}

Cell_ComponentOccurrences::Cell_ComponentOccurrences(const Cell* cell, const Layer::Mask& mask)
// ******************************************************************************************
:     Inherit(),
    _cell(cell),
    _mask(mask)
{
}

Cell_ComponentOccurrences::Cell_ComponentOccurrences(const Cell_ComponentOccurrences& occurrences)
// *******************************************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _mask(occurrences._mask)
{
}

Cell_ComponentOccurrences& Cell_ComponentOccurrences::operator=(const Cell_ComponentOccurrences& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    _mask = occurrences._mask;
    return *this;
}

Collection<Occurrence>* Cell_ComponentOccurrences::GetClone() const
// **************************************************************
{
    return new Cell_ComponentOccurrences(*this);
}

Locator<Occurrence>* Cell_ComponentOccurrences::GetLocator() const
// *************************************************************
{
    return new Locator(_cell, _mask);
}

string Cell_ComponentOccurrences::_GetString() const
// ************************************************
{
    string s = "<" + _TName("Cell::ComponentOccurrences");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentOccurrences::Locator implementation
// ****************************************************************************************************

Cell_ComponentOccurrences::Locator::Locator()
// *****************************************
:    Inherit(),
    _cell(NULL),
    _mask(0),
    _state(0),
    _componentLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
}

Cell_ComponentOccurrences::Locator::Locator(const Cell* cell, const Layer::Mask& mask)
// **********************************************************************************
:    Inherit(),
    _cell(cell),
    _mask(mask),
    _state(0),
    _componentLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
    if (_cell && (_mask != 0)) {
        _componentLocator = _cell->GetComponents(_mask).GetLocator();
        if (_componentLocator.IsValid())
            _state = 1;
        else {
            _instanceLocator = _cell->GetInstances().GetLocator();
            while (!_state && _instanceLocator.IsValid()) {
                Cell* masterCell = _instanceLocator.GetElement()->GetMasterCell();
                _occurrenceLocator = masterCell->GetComponentOccurrences(_mask).GetLocator();
                if (_occurrenceLocator.IsValid())
                    _state = 2;
                else
                    _instanceLocator.Progress();
            }
        }
    }
}

Cell_ComponentOccurrences::Locator::Locator(const Locator& locator)
// ***************************************************************
:    Inherit(),
    _cell(locator._cell),
    _mask(locator._mask),
    _state(locator._state),
    _componentLocator(locator._componentLocator),
    _instanceLocator(locator._instanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_ComponentOccurrences::Locator& Cell_ComponentOccurrences::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _mask = locator._mask;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
    _instanceLocator = locator._instanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_ComponentOccurrences::Locator::GetElement() const
// ************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_componentLocator.GetElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.GetElement();
                Entity* entity = occurrence.GetEntity();
                Path path = Path(_instanceLocator.GetElement(), occurrence.GetPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_ComponentOccurrences::Locator::GetClone() const
// ********************************************************************
{
    return new Locator(*this);
}

bool Cell_ComponentOccurrences::Locator::IsValid() const
// ****************************************************
{
    return (_state != 0);
}

void Cell_ComponentOccurrences::Locator::Progress()
// ***********************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _componentLocator.Progress();
                if (!_componentLocator.IsValid()) {
                    _state = 0;
                    _instanceLocator = _cell->GetInstances().GetLocator();
                    while (!_state && _instanceLocator.IsValid()) {
                        Cell* masterCell = _instanceLocator.GetElement()->GetMasterCell();
                        _occurrenceLocator = masterCell->GetComponentOccurrences(_mask).GetLocator();
                        if (_occurrenceLocator.IsValid())
                            _state = 2;
                        else
                            _instanceLocator.Progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.Progress();
                if (!_occurrenceLocator.IsValid()) {
                    _state = 0;
                    if (_instanceLocator.IsValid()) {
                        _instanceLocator.Progress();
                        while (!_state && _instanceLocator.IsValid()) {
                            Cell* masterCell = _instanceLocator.GetElement()->GetMasterCell();
                            _occurrenceLocator = masterCell->GetComponentOccurrences(_mask).GetLocator();
                            if (_occurrenceLocator.IsValid())
                                _state = 2;
                            else
                                _instanceLocator.Progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_ComponentOccurrences::Locator::_GetString() const
// *********************************************************
{
    string s = "<" + _TName("Cell::ComponentOccurrences::Locator");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentOccurrencesUnder implementation
// ****************************************************************************************************

Cell_ComponentOccurrencesUnder::Cell_ComponentOccurrencesUnder()
// ***********************************************************
:     Inherit(),
    _cell(NULL),
    _area(),
    _mask(0)
{
}

Cell_ComponentOccurrencesUnder::Cell_ComponentOccurrencesUnder(const Cell* cell, const Box& area, const Layer::Mask& mask)
// ****************************************************************************************************
:     Inherit(),
    _cell(cell),
    _area(area),
    _mask(mask)
{
}

Cell_ComponentOccurrencesUnder::Cell_ComponentOccurrencesUnder(const Cell_ComponentOccurrencesUnder& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _area(occurrences._area),
    _mask(occurrences._mask)
{
}

Cell_ComponentOccurrencesUnder& Cell_ComponentOccurrencesUnder::operator=(const Cell_ComponentOccurrencesUnder& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    _area = occurrences._area;
    _mask = occurrences._mask;
    return *this;
}

Collection<Occurrence>* Cell_ComponentOccurrencesUnder::GetClone() const
// *******************************************************************
{
    return new Cell_ComponentOccurrencesUnder(*this);
}

Locator<Occurrence>* Cell_ComponentOccurrencesUnder::GetLocator() const
// ******************************************************************
{
    return new Locator(_cell, _area, _mask);
}

string Cell_ComponentOccurrencesUnder::_GetString() const
// *****************************************************
{
    string s = "<" + _TName("Cell::ComponentOccurrencesUnder");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_area);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentOccurrencesUnder::Locator implementation
// ****************************************************************************************************

Cell_ComponentOccurrencesUnder::Locator::Locator()
// **********************************************
:    Inherit(),
    _cell(NULL),
    _area(),
    _mask(0),
    _state(0),
    _componentLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
}

Cell_ComponentOccurrencesUnder::Locator::Locator(const Cell* cell, const Box& area, const Layer::Mask& mask)
// ****************************************************************************************************
:    Inherit(),
    _cell(cell),
    _area(area),
    _mask(mask),
    _state(0),
    _componentLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
    if (_cell && !_area.isEmpty() && (_mask != 0)) {
        _componentLocator = _cell->GetComponentsUnder(_area, _mask).GetLocator();
        if (_componentLocator.IsValid())
            _state = 1;
        else {
            _instanceLocator = _cell->GetInstancesUnder(_area).GetLocator();
            while (!_state && _instanceLocator.IsValid()) {
                Instance* instance = _instanceLocator.GetElement();
                Cell* masterCell = instance->GetMasterCell();
                Box masterArea = _area;
                instance->GetTransformation().getInvert().applyOn(masterArea);
                _occurrenceLocator =
                    masterCell->GetComponentOccurrencesUnder(masterArea, _mask).GetLocator();
                if (_occurrenceLocator.IsValid())
                    _state = 2;
                else
                    _instanceLocator.Progress();
            }
        }
    }
}

Cell_ComponentOccurrencesUnder::Locator::Locator(const Locator& locator)
// ********************************************************************
:    Inherit(),
    _cell(locator._cell),
    _area(locator._area),
    _mask(locator._mask),
    _state(locator._state),
    _componentLocator(locator._componentLocator),
    _instanceLocator(locator._instanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_ComponentOccurrencesUnder::Locator& Cell_ComponentOccurrencesUnder::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _area = locator._area;
    _mask = locator._mask;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
    _instanceLocator = locator._instanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_ComponentOccurrencesUnder::Locator::GetElement() const
// *****************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_componentLocator.GetElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.GetElement();
                Entity* entity = occurrence.GetEntity();
                Path path = Path(_instanceLocator.GetElement(), occurrence.GetPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_ComponentOccurrencesUnder::Locator::GetClone() const
// *************************************************************************
{
    return new Locator(*this);
}

bool Cell_ComponentOccurrencesUnder::Locator::IsValid() const
// *********************************************************
{
    return (_state != 0);
}

void Cell_ComponentOccurrencesUnder::Locator::Progress()
// ****************************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _componentLocator.Progress();
                if (!_componentLocator.IsValid()) {
                    _state = 0;
                    _instanceLocator = _cell->GetInstancesUnder(_area).GetLocator();
                    while (!_state && _instanceLocator.IsValid()) {
                        Instance* instance = _instanceLocator.GetElement();
                        Cell* masterCell = instance->GetMasterCell();
                        Box masterArea = _area;
                        instance->GetTransformation().getInvert().applyOn(masterArea);
                        _occurrenceLocator =
                            masterCell->GetComponentOccurrencesUnder(masterArea, _mask).GetLocator();
                        if (_occurrenceLocator.IsValid())
                            _state = 2;
                        else
                            _instanceLocator.Progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.Progress();
                if (!_occurrenceLocator.IsValid()) {
                    _state = 0;
                    if (_instanceLocator.IsValid()) {
                        _instanceLocator.Progress();
                        while (!_state && _instanceLocator.IsValid()) {
                            Instance* instance = _instanceLocator.GetElement();
                            Cell* masterCell = instance->GetMasterCell();
                            Box masterArea = _area;
                            instance->GetTransformation().getInvert().applyOn(masterArea);
                            _occurrenceLocator =
                                masterCell->GetComponentOccurrencesUnder(masterArea, _mask).GetLocator();
                            if (_occurrenceLocator.IsValid())
                                _state = 2;
                            else
                                _instanceLocator.Progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_ComponentOccurrencesUnder::Locator::_GetString() const
// **************************************************************
{
    string s = "<" + _TName("Cell::ComponentOccurrencesUnder::Locator");
    if (_cell) {
        s += " " + GetString(_cell);
        s += " " + GetString(_area);
        s += " " + GetString(_mask);
    }
    s += ">";
    return s;
}

// ****************************************************************************************************
// Cell_HyperNetRootNetOccurrences implementation
// ****************************************************************************************************

Cell_HyperNetRootNetOccurrences::Cell_HyperNetRootNetOccurrences()
// *************************************
:     Inherit(),
    _cell(NULL),
    _path()
{
}

Cell_HyperNetRootNetOccurrences::Cell_HyperNetRootNetOccurrences(const Cell* cell, Path path)
// ****************************************************************
:     Inherit(),
    _cell(cell),
    _path(path)
{
}

Cell_HyperNetRootNetOccurrences::Cell_HyperNetRootNetOccurrences(const Cell_HyperNetRootNetOccurrences& netOccurrences)
// ****************************************************************************
:     Inherit(),
    _cell(netOccurrences._cell),
    _path(netOccurrences._path)
{
}

Cell_HyperNetRootNetOccurrences& Cell_HyperNetRootNetOccurrences::operator=(const Cell_HyperNetRootNetOccurrences& netOccurrences)
// ***************************************************************************************
{
    _cell = netOccurrences._cell;
    _path = netOccurrences._path;
    return *this;
}

Collection<Occurrence>* Cell_HyperNetRootNetOccurrences::GetClone() const
// ********************************************************
{
    return new Cell_HyperNetRootNetOccurrences(*this);
}

Locator<Occurrence>* Cell_HyperNetRootNetOccurrences::GetLocator() const
// *******************************************************
{
    return new Locator(_cell, _path);
}

string Cell_HyperNetRootNetOccurrences::_GetString() const
// ******************************************
{
    string s = "<" + _TName("Cell::HyperNetRootNetOccurrences");
    if (_cell) {
        s += " " + GetString(_cell);
        if (!_path.IsEmpty()) s += " " + GetString(_path);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_HyperNetRootNetOccurrences::Locator implementation
// ****************************************************************************************************

Cell_HyperNetRootNetOccurrences::Locator::Locator()
// ***********************************
:    Inherit(),
    _path(),
    _netLocator(),
    _instanceLocator(),
    _hyperNetRootNetOccurrenceLocator()
{
}

Cell_HyperNetRootNetOccurrences::Locator::Locator(const Cell* cell, Path path)
// **************************************************************
:    Inherit(),
    _path(path),
    _netLocator(),
    _instanceLocator(),
    _hyperNetRootNetOccurrenceLocator()
{
    _netLocator=cell->GetNets().GetLocator();

    _instanceLocator=cell->GetInstances().GetLocator();

    while (_netLocator.IsValid() && !IsHyperNetRootNetOccurrence(Occurrence(_netLocator.GetElement(),_path)))
        _netLocator.Progress();

    if (!_netLocator.IsValid())
        while (!_hyperNetRootNetOccurrenceLocator.IsValid() && _instanceLocator.IsValid())
        {
            Instance* instance = _instanceLocator.GetElement();
            _hyperNetRootNetOccurrenceLocator=Locator(instance->GetMasterCell(),Path(_path,instance));
            _instanceLocator.Progress();
        }
}

Cell_HyperNetRootNetOccurrences::Locator::Locator(const Locator& locator)
// *********************************************************
:    Inherit(),
    _path(locator._path),
    _netLocator(locator._netLocator),
    _instanceLocator(locator._instanceLocator),
    _hyperNetRootNetOccurrenceLocator(locator._hyperNetRootNetOccurrenceLocator)
{
}

Cell_HyperNetRootNetOccurrences::Locator& Cell_HyperNetRootNetOccurrences::Locator::operator=(const Locator& locator)
// ****************************************************************************************
{
    _path = locator._path;
    _netLocator = locator._netLocator;
    _instanceLocator = locator._instanceLocator;
    _hyperNetRootNetOccurrenceLocator = locator._hyperNetRootNetOccurrenceLocator;
    return *this;
}

Occurrence Cell_HyperNetRootNetOccurrences::Locator::GetElement() const
// ******************************************************
{
    if (_netLocator.IsValid())
        return Occurrence(_netLocator.GetElement(),_path);
    else if (_hyperNetRootNetOccurrenceLocator.IsValid())
        return _hyperNetRootNetOccurrenceLocator.GetElement();
    else
        return Occurrence();

}

Locator<Occurrence>* Cell_HyperNetRootNetOccurrences::Locator::GetClone() const
// **************************************************************
{
    return new Locator(*this);
}

bool Cell_HyperNetRootNetOccurrences::Locator::IsValid() const
// **********************************************
{
    return (_netLocator.IsValid() || (_hyperNetRootNetOccurrenceLocator.IsValid()));
}

void Cell_HyperNetRootNetOccurrences::Locator::Progress()
// *****************************************
{
    if (_netLocator.IsValid())
    {
        do {
            _netLocator.Progress();
        }
        while (_netLocator.IsValid() && !IsHyperNetRootNetOccurrence(Occurrence(_netLocator.GetElement(),_path)));
    }
    else if (_hyperNetRootNetOccurrenceLocator.IsValid())
        _hyperNetRootNetOccurrenceLocator.Progress();

    if (!_netLocator.IsValid())
        while (!_hyperNetRootNetOccurrenceLocator.IsValid() && _instanceLocator.IsValid())
        {
            Instance* instance = _instanceLocator.GetElement();
            _hyperNetRootNetOccurrenceLocator=Locator(instance->GetMasterCell(),Path(_path,instance));
            _instanceLocator.Progress();
        }

}

string Cell_HyperNetRootNetOccurrences::Locator::_GetString() const
// ***************************************************
{
    string s = "<" + _TName("Cell::HyperNetRootNetOccurrences::Locator");
    if (!_path.IsEmpty())
        s += " " + _path.GetName();
    s += ">";
    return s;
}


// ****************************************************************************************************
// Cell_ImpactedMainViews implementation
// ****************************************************************************************************

//Cell_ImpactedMainViews::Cell_ImpactedMainViews(const Cell* cell)
//// *************************************************************
//:     Inherit(),
//    _cell(cell)
//{
//}
//
//Cell_ImpactedMainViews::Cell_ImpactedMainViews(const Cell_ImpactedMainViews& impactedMainViews)
//// ********************************************************************************************
//:     Inherit(),
//    _cell(impactedMainViews._cell)
//{
//}
//
//Cell_ImpactedMainViews& Cell_ImpactedMainViews::operator=(const Cell_ImpactedMainViews& impactedMainViews)
//// ****************************************************************************************************
//{
//    _cell = impactedMainViews._cell;
//    return *this;
//}
//
//Collection<MainView*>* Cell_ImpactedMainViews::GetClone() const
//// ************************************************************
//{
//    return new Cell_ImpactedMainViews(*this);
//}
//
//Locator<MainView*>* Cell_ImpactedMainViews::GetLocator() const
//// ***********************************************************
//{
//    return new Locator(_cell);
//}
//
//string Cell_ImpactedMainViews::_GetString() const
//// **********************************************
//{
//    string s = "<" + _TName("Cell::ImpactedMainViews");
//    if (_cell) s += " " + GetString(_cell);
//    s += ">";
//    return s;
//}
//
//
//
//// ****************************************************************************************************
//// Cell_ImpactedMainViews::Locator implementation
//// ****************************************************************************************************
//
//Cell_ImpactedMainViews::Locator::Locator(const Cell* cell)
//// *******************************************************
//:    Inherit(),
//    _cell(cell),
//    _cellSet(),
//    _cellStack(),
//    _mainViewLocator(),
//    _mainView(NULL)
//{
//    if (_cell) {
//        _cellSet.insert((Cell*)_cell);
//        _cellStack.push((Cell*)_cell);
//        while (!_mainView && !_cellStack.empty()) {
//            Cell* cell = _cellStack.top();
//            _cellStack.pop();
//            for_each_instance(instance, cell->GetSlaveInstances()) {
//                Cell* cell = instance->GetCell();
//                if (_cellSet.find(cell) == _cellSet.end()) {
//                    _cellSet.insert(cell);
//                    _cellStack.push(cell);
//                }
//                end_for;
//            }
//            _mainViewLocator = cell->GetMainViews().GetLocator();
//            _mainView = _mainViewLocator.GetElement();
//        }
//    }
//}
//
//Cell_ImpactedMainViews::Locator::Locator(const Locator& locator)
//// *************************************************************
//:    Inherit(),
//    _cell(locator._cell),
//    _cellSet(locator._cellSet),
//    _cellStack(locator._cellStack),
//    _mainViewLocator(locator._mainViewLocator),
//    _mainView(locator._mainView)
//{
//}
//
//Cell_ImpactedMainViews::Locator& Cell_ImpactedMainViews::Locator::operator=(const Locator& locator)
//// ************************************************************************************************
//{
//    _cell = locator._cell;
//    _cellSet = locator._cellSet;
//    _cellStack = locator._cellStack;
//    _mainViewLocator = locator._mainViewLocator;
//    _mainView = locator._mainView;
//    return *this;
//}
//
//MainView* Cell_ImpactedMainViews::Locator::GetElement() const
//// **********************************************************
//{
//    return _mainView;
//}
//
//Locator<MainView*>* Cell_ImpactedMainViews::Locator::GetClone() const
//// ******************************************************************
//{
//    return new Locator(*this);
//}
//
//bool Cell_ImpactedMainViews::Locator::IsValid() const
//// **************************************************
//{
//    return (_mainView != NULL);
//}
//
//void Cell_ImpactedMainViews::Locator::Progress()
//// *********************************************
//{
//    if (_mainView) {
//        _mainView = NULL;
//        do {
//            _mainViewLocator.Progress();
//            _mainView = _mainViewLocator.GetElement();
//            if (!_mainView && !_cellStack.empty()) {
//                Cell* cell = _cellStack.top();
//                _cellStack.pop();
//                for_each_instance(instance, cell->GetSlaveInstances()) {
//                    Cell* cell = instance->GetCell();
//                    if (_cellSet.find(cell) == _cellSet.end()) {
//                        _cellSet.insert(cell);
//                        _cellStack.push(cell);
//                    }
//                    end_for;
//                }
//                _mainViewLocator = cell->GetMainViews().GetLocator();
//                _mainView = _mainViewLocator.GetElement();
//            }
//        } while (!_mainView && !_cellStack.empty());
//    }
//}
//
//string Cell_ImpactedMainViews::Locator::_GetString() const
//// *******************************************************
//{
//    string s = "<" + _TName("Cell::ImpactedMainViews::Locator");
//    if (_cell) s += " " + GetString(_cell);
//    s += ">";
//    return s;
//}
//
//
// ****************************************************************************************************
// Cell_SubCells implementation
// ****************************************************************************************************

Cell_SubCells::Cell_SubCells()
// ***************************
:     Inherit(),
    _cell(NULL)
{
}

Cell_SubCells::Cell_SubCells(const Cell* cell)
// *******************************************
:     Inherit(),
    _cell(cell)
{
}

Cell_SubCells::Cell_SubCells(const Cell_SubCells& subCells)
// ********************************************************
:     Inherit(),
    _cell(subCells._cell)
{
}

Cell_SubCells& Cell_SubCells::operator=(const Cell_SubCells& subCells)
// *******************************************************************
{
    _cell = subCells._cell;
    return *this;
}

Collection<Cell*>* Cell_SubCells::GetClone() const
// ***********************************************
{
    return new Cell_SubCells(*this);
}

Locator<Cell*>* Cell_SubCells::GetLocator() const
// **********************************************
{
    return new Locator(_cell->GetInstances().GetLocator());
}

string Cell_SubCells::_GetString() const
// *************************************
{
    string s = "<" + _TName("Cell::SubCells");
    if (_cell) {
        s += " " + GetString(_cell);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_SubCells::Locator implementation
// ****************************************************************************************************

Cell_SubCells::Locator::Locator()
// **********************************
:    Inherit(),
    _cellSet(),
    _instanceLocator()
{
}

Cell_SubCells::Locator::Locator(InstanceLocator instanceLocator)
// *************************************************************
:    Inherit(),
    _cellSet(),
    _instanceLocator(instanceLocator)
{
    if ( IsValid() )
        _cellSet.insert ( _instanceLocator.GetElement()->GetMasterCell() );
}

Cell_SubCells::Locator::Locator(const Locator& locator)
// ****************************************************
:    Inherit(),
    _cellSet(locator._cellSet),
    _instanceLocator(locator._instanceLocator)
{
}

Cell_SubCells::Locator& Cell_SubCells::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
    _cellSet = locator._cellSet;
    _instanceLocator = locator._instanceLocator;
    return *this;
}

Cell* Cell_SubCells::Locator::GetElement() const
// *********************************************
{
    return _instanceLocator.GetElement()->GetMasterCell();
}

Locator<Cell*>* Cell_SubCells::Locator::GetClone() const
// *****************************************************
{
    return new Locator(*this);
}

bool Cell_SubCells::Locator::IsValid() const
// *****************************************
{
    return _instanceLocator.IsValid();
}

void Cell_SubCells::Locator::Progress()
// ************************************
{
    while ( IsValid() && ( _cellSet.find ( _instanceLocator.GetElement()->GetMasterCell() ) != _cellSet.end() ) )
        _instanceLocator.Progress();
    if ( IsValid() )
        _cellSet.insert ( _instanceLocator.GetElement()->GetMasterCell() );
}

string Cell_SubCells::Locator::_GetString() const
// **************************************************
{
    string s = "<" + _TName("Cell::SubCells::Locator");
    s += " " + _GetString();
    s += " " + GetString(_instanceLocator);
    s += ">";
    return s;
}




// x-----------------------------------------------------------------x
// |    "::Cell_RecursiveSlavePathes::Locator" Class Definitions     |
// x-----------------------------------------------------------------x


// -------------------------------------------------------------------
// Constructor  :  "Cell_RecursiveSlavePathes::Locator::Locator ()".

Cell_RecursiveSlavePathes::Locator::Locator ( const Cell* cell )
  : Hurricane::Locator<Path>()
  , _instancesStack()
  , _instancePath()
  , _isInPop(false)
{
  const Cell* topCell = cell;

  do {
    Instances slaveInstances = topCell->GetSlaveInstances();

    if ( !slaveInstances.IsEmpty() ) {
      _instancesStack.push_back ( *slaveInstances.GetLocator() );
      _instancePath = Path ( _instancesStack.back().GetElement(), _instancePath );

//       cerr << "Instances of I: " << topCell->GetName() << endl;
//       cerr << "Head         I: " << _instancesStack.back().GetElement()->GetName() << endl;
//       cerr << "TailPath     I: " << _instancePath.GetName() << endl;

      topCell = _instancesStack.back().GetElement()->GetCell();
    } else
      topCell = NULL;

  } while ( topCell );
}




// -------------------------------------------------------------------
// Modifier  :  "Cell_RecursiveSlavePathes::Locator::Progress ()".

void  Cell_RecursiveSlavePathes::Locator::Progress ()
{
  if ( !IsValid() ) return;

  _instancesStack.back().Progress();
  if ( _instancesStack.back().IsValid() ) {
//     cerr << "Head         N: " << _instancesStack.back().GetElement()->GetName() << endl;
//     cerr << "TailPath     N: " << _instancePath.GetTailPath().GetName() << endl;
    _instancePath = Path ( _instancesStack.back().GetElement(), _instancePath.GetTailPath() );

    Cell* topCell = _instancesStack.back().GetElement()->GetCell();

    do {
      Instances slaveInstances = topCell->GetSlaveInstances();

      if ( !slaveInstances.IsEmpty() ) {
        _instancesStack.push_back ( *slaveInstances.GetLocator() );
//         cerr << "Instances of R: " << topCell->GetName() << endl;
//         cerr << "Head         R: " << _instancesStack.back().GetElement()->GetName() << endl;
//         cerr << "TailPath     R: " << _instancePath.GetName() << endl;
        _instancePath = Path ( _instancesStack.back().GetElement(), _instancePath );

        topCell = _instancesStack.back().GetElement()->GetCell();
      } else
        topCell = NULL;
    } while ( topCell );

    return;
  } else {
    _instancesStack.pop_back();
    _instancePath = _instancePath.GetTailPath();
//     cerr << "TailPath     F: " << _instancePath.GetName() << endl;
    return;
  }
}




// -------------------------------------------------------------------
// Hurricane Management  :  "Cell_RecursiveSlavePathes::Locator::_GetString ()".

string  Cell_RecursiveSlavePathes::Locator::_GetString () const
{
  Name  masterCellName = "Finished";

  if ( IsValid() )
    masterCellName = _instancesStack.front().GetElement()->GetMasterCell()->GetName();

  string s = "<" + _TName("Cell_RecursiveSlavePathes::Locator")
                 + GetString(masterCellName)
                 + ">";

  return s;
}




// x-----------------------------------------------------------------x
// |        "::Cell_RecursiveSlavePathes" Class Definitions          |
// x-----------------------------------------------------------------x


// -------------------------------------------------------------------
// Hurricane Management  :  "Cell_RecursiveSlavePathes::_GetString ()".

string  Cell_RecursiveSlavePathes::_GetString () const
{
  string s = "<" + _TName("Cell_RecursiveSlavePathes") + " "
                 + GetString(_cell) + " "
                 + ">";

  return s;
}




} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
