
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id: ProxyProperty.h,v 1.9 2007/07/29 15:27:27 jpc Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ProxyProperty.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x





#ifndef  __PROXYPROPERTY__
#define  __PROXYPROPERTY__


#include "hurricane/DBo.h"
#include "hurricane/Property.h"


namespace  Isobar {


using namespace Hurricane;




// -------------------------------------------------------------------
// Miscellaneous.

#define F_PY_OWNED    ( 1 << 0 )

#define CHECK_OFFSET(PY_STRUCT)                                                 \
  if ( ProxyProperty::getOffset() < 0 )                                         \
      ProxyProperty::SetOffset (   (unsigned long)(&(PY_STRUCT->ACCESS_OBJECT)) \
                                 - (unsigned long)PY_STRUCT );



// -------------------------------------------------------------------
// Class  :  "::ProxyProperty"

  class ProxyProperty : public Property {

    // Shared Attributes.
    protected: static Name  _name;
    protected: static int   _offset;

    // Attributes.
    protected: DBo*      _owner;
    protected: void*     _shadow;

    // Constructors.
    protected:        ProxyProperty ( void* _shadow );
    private:          ProxyProperty ( const ProxyProperty& );
    private:          ProxyProperty& operator= ( const ProxyProperty& );
    public:    static ProxyProperty* create ( void* _shadow=NULL );

    // Accessors.
    public: DBo*  getOwner        () const { return ( _owner ); };
    public: void* getShadow       () const { return ( _shadow ); };
    public: void* getShadowMember () const { return ( (void*)((unsigned long)_shadow+_offset) ); };
    public: template<typename DBoType>
              DBoType* getAsType () { return ( dynamic_cast<DBoType*>(_owner) ); };

    // Static methods.
    public: static int   getOffset () { return ( _offset ); };
    public: static void  SetOffset ( int offset );

    // Property Managment.
    public:    static  const Name &getPropertyName () { return ( _name ); }
    public:    virtual Name  getName () const { return (getPropertyName()); }
    public:    virtual void  onCapturedBy ( DBo* owner );
    public:    virtual void  onReleasedBy ( DBo* owner );
    public:    virtual void  onNotOwned   ();
    protected: virtual void  _preDestroy  ();

    // Hurricane Managment.
    public: virtual string  _getString   () const;
    public: virtual Record* _getRecord   () const;
    public: virtual string  _getTypeName () const { return _TName("ProxyProperty"); };

  };




}  // End of Isobar namespace.



#endif
