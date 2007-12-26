// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: UserGos.h
// Authors: C. Alexandre
// ****************************************************************************************************

#ifndef HURRICANE_USER_GOS
#define HURRICANE_USER_GOS

#include "Collection.h"

namespace Hurricane {

class UserGo;



// ****************************************************************************************************
// UserGosdeclaration
// ****************************************************************************************************

typedef GenericCollection<UserGo*> UserGos;



// ****************************************************************************************************
// UserGoLocator declaration
// ****************************************************************************************************

typedef GenericLocator<UserGo*> UserGoLocator;



// ****************************************************************************************************
// UserGoFilter declaration
// ****************************************************************************************************

typedef GenericFilter<UserGo*> UserGoFilter;



// ****************************************************************************************************
// for_each_usergo declaration
// ****************************************************************************************************

#define for_each_user_go(userGo, userGos)\
/***************************************/\
{\
    UserGoLocator _locator = userGos.GetLocator();\
    while (_locator.IsValid()) {\
        UserGo* userGo = _locator.GetElement();\
        _locator.Progress();


} // End of Hurricane namespace.

#endif // HURRICANE_USER_GOS
