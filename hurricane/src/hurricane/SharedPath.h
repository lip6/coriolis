// ****************************************************************************************************
// File: SharedPath.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_SHARED_PATH
#define HURRICANE_SHARED_PATH

#include "Instances.h"
#include "SharedPathes.h"
#include "Quark.h"
#include "Transformation.h"
#include "IntrusiveMap.h"

namespace Hurricane {

class Cell;
class Entity;



// ****************************************************************************************************
// SharedPath declaration
// ****************************************************************************************************

class SharedPath {
// *************

// Types
// *****

	public: class QuarkMap : public IntrusiveMap<const Entity*, Quark> {
	// ***************************************************************

		public: typedef IntrusiveMap<const Entity*, Quark> Inherit;
	
		public: QuarkMap();

		public: virtual const Entity* _GetKey(Quark* quark) const;
		public: virtual unsigned _GetHashValue(const Entity* entity) const;
		public: virtual Quark* _GetNextElement(Quark* quark) const;
		public: virtual void _SetNextElement(Quark* quark, Quark* nextQuark) const;

	};

// Attributes
// **********

	private: Instance* _headInstance;
	private: SharedPath* _tailSharedPath;
	private: QuarkMap _quarkMap;
	private: SharedPath* _nextOfInstanceSharedPathMap;

// Constructors
// ************

	public: SharedPath(Instance* headInstance, SharedPath* tailSharedPath = NULL);

	private: SharedPath(const SharedPath& sharedPath);
				// not implemented to forbid copy construction

// Destructor
// **********

	public: ~SharedPath();

// Operators
// *********

	private: SharedPath& operator=(const SharedPath& sharedPath);
				// not implemented to forbid assignment

// Accessors
// *********

	public: static char GetNameSeparator();

	public: Instance* GetHeadInstance() const {return _headInstance;};
	public: SharedPath* GetTailSharedPath() const {return _tailSharedPath;};
	public: SharedPath* GetHeadSharedPath() const;
	public: Instance* GetTailInstance() const;
	public: string GetName() const;
	public: Cell* GetOwnerCell() const;
	public: Cell* GetMasterCell() const;
	public: Instances GetInstances() const;
	public: Transformation GetTransformation(const Transformation& transformation = Transformation()) const;

// Updators
// ********

	public: static void SetNameSeparator(char nameSeparator);

// Accessors
// *********

    public: string _GetTypeName() const { return _TName("SharedPath"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;

	public: Quark* _GetQuark(const Entity* entity) const {return _quarkMap.GetElement(entity);};
	public: Quarks _GetQuarks() const {return _quarkMap.GetElements();};
	public: QuarkMap& _GetQuarkMap() {return _quarkMap;};
	public: SharedPath* _GetNextOfInstanceSharedPathMap() const {return _nextOfInstanceSharedPathMap;};

	public: void _SetNextOfInstanceSharedPathMap(SharedPath* sharedPath) {_nextOfInstanceSharedPathMap = sharedPath;};

};



} // End of Hurricane namespace.



#endif // HURRICANE_SHARED_PATH

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
