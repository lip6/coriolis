// ****************************************************************************************************
// File: ./hurricane/Exception.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#ifndef HURRICANE_EXCEPTION
#define HURRICANE_EXCEPTION

#include "hurricane/Commons.h"
#include "hurricane/TextTranslator.h"

namespace Hurricane {



// ****************************************************************************************************
// Exception declaration
// ****************************************************************************************************

class Exception {
// **************

// Attributes
// **********

   private: static TextTranslator  _textTranslator;
   private: static TextTranslator  _htmlTranslator;

// Constructors
// ************

    protected: Exception();

    private: Exception(const Exception& exception); // not implemented to forbid copy construction

// Destructor
// **********

    public: virtual ~Exception();

// Operators
// *********

    private: Exception& operator=(const Exception& exception); // not implemented to forbid assignment

// Accessors
// *********

    public: string what    () const { return textWhat(); };
    public: string textWhat() const { return _textTranslator.translate(_getString()); };
    public: string htmlWhat() const { return _htmlTranslator.translate(_getString()); };

// Others
// ******

    public: static  void   setTextTranslator ( const TextTranslator& translator ) { _textTranslator=translator; }
    public: static  void   setHtmlTranslator ( const TextTranslator& translator ) { _htmlTranslator=translator; }
    public: virtual string _getString        () const = 0;

};


} // End of Hurricane namespace.


#endif // HURRICANE_EXCEPTION


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
