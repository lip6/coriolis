// ****************************************************************************************************
// File: Builder.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_BUILDER
#define HURRICANE_BUILDER

#include "InputFile.h"

namespace Hurricane {



// ****************************************************************************************************
// Builder declaration
// ****************************************************************************************************

class Builder : public NestedSlotAdapter {
// *************************************

// Attributes
// **********

	private: string _token;

// Constructors
// ************

	public: Builder(const string& token);

	private: Builder(const Builder& builder); // not implemented to forbid copy

// Destructor
// **********

	public: virtual ~Builder();

// Operators
// *********

	private: Builder& operator=(const Builder& builder); // not implemented to forbid assinment

// Accessors
// *********

	public: string GetToken() const {return _token;};

// Managers
// ********

	public: virtual void Scan(InputFile& inputFile, char*& arguments);
	public: virtual DBo* CreateDBo() = 0;

// Others
// ******

	public: virtual string _GetTypeName() const = 0;
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Builder)


H::Builder* GetBuilder(const string& token);


#endif // HURRICANE_BUILDER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
