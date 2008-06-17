// ****************************************************************************************************
// File: UpdateSession.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_UPDATE_SESSION
#define HURRICANE_UPDATE_SESSION

#include "hurricane/Property.h"

namespace Hurricane {

class Go;



// ****************************************************************************************************
// UpdateSession declaration
// ****************************************************************************************************

class UpdateSession : public SharedProperty {
// ****************************************

// Types
// *****

    public: typedef SharedProperty Inherit;

// Constructors
// ************

    protected: UpdateSession();

    public: virtual void destroy();

// Accessors
// *********

    public: static const Name& getPropertyName();
    public: virtual Name getName() const {return getPropertyName();};

// Managers
// ********

    public: virtual void onCapturedBy(DBo* owner);
    public: virtual void onNotOwned();

// Ohers
// *****

    public: static UpdateSession* _create();
    protected: virtual void _postCreate();

    public: void _destroy();
    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("UpdateSession");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: static void open();
    public: static void close();


};


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

//void openUpdateSession();
//
//void closeUpdateSession();
//


} // End of Hurricane namespace.


#endif // HURRICANE_UPDATE_SESSION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
