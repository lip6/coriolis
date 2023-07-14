// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2015-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/NetAlias.h"                        |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_NET_ALIAS_H
#define HURRICANE_NET_ALIAS_H

#include <string>
#include <set>
#include "hurricane/Name.h"


namespace Hurricane {

  class Record;
  class Net;
  class NetAliasName;


// -------------------------------------------------------------------
// Class  :  "Hurricane::NetAliasHook".

  class NetAliasHook {
    public:
              bool          isAttached    () const;
      virtual bool          isMaster      () const = 0;
      virtual bool          isSlave       () const = 0;
      virtual bool          isExternal    () const = 0;
      virtual Name          getName       () const = 0;
      virtual Net*          getNet        () const = 0;
              NetAliasHook* getNext       () const;
              NetAliasHook* getPrevious   () const;
              NetAliasName* find          ( const Name& ) const;
              void          attach        ( NetAliasHook* );
              void          detach        ();
              void          detachAll     ();
      virtual void          setExternal   ( bool ) = 0;
      inline  void          toJson        ( JsonWriter* ) const;
      virtual std::string   _getString    () const = 0;
      virtual Record*       _getRecord    () const;
    public:                 
                            NetAliasHook  ();
      virtual              ~NetAliasHook  ();
    private:
                            NetAliasHook  ( const NetAliasHook& );
              NetAliasHook&    operator=  ( const NetAliasHook& );
    private:
      NetAliasHook* _next;
  };


// -------------------------------------------------------------------
// Class  :  "Hurricane::NetMainName".

  class NetMainName : public NetAliasHook {
    public:
      virtual bool         isMaster    () const;
      virtual bool         isSlave     () const;
      virtual bool         isExternal  () const;
      virtual Name         getName     () const;
      virtual Net*         getNet      () const;
      virtual std::string  _getString  () const;
      virtual void         setExternal ( bool state );
              void         clear       ();
    public:
                           NetMainName ( Net* );
      virtual             ~NetMainName ();
    private:
                           NetMainName ( const NetMainName& );
              NetMainName& operator=   ( const NetMainName& );
    private:
      static  ptrdiff_t  _offset;
  };


// -------------------------------------------------------------------
// Class  :  "Hurricane::NetAliasName".

  class NetAliasName : public NetAliasHook {
    public:
      class CompareByName {
        public:
          inline bool operator() ( const NetAliasName* lhs, const NetAliasName* rhs ) const;
      };
    public:
      virtual bool          isMaster     () const;
      virtual bool          isSlave      () const;
      virtual bool          isExternal   () const;
      virtual Name          getName      () const;
      virtual Net*          getNet       () const;
      virtual void          toJson       ( JsonWriter* ) const;
      virtual void          setExternal  ( bool );
      virtual std::string   _getString   () const;
      virtual Record*       _getRecord   () const;
    public:
                            NetAliasName ( Name, bool isExternal=false );
      virtual              ~NetAliasName ();
    private:
                            NetAliasName ( const NetAliasName& );
              NetAliasName& operator=    ( const NetAliasName& );
    private:
      Name  _name;
      bool  _isExternal;
  };


  inline bool  NetAliasName::CompareByName::operator() ( const NetAliasName* lhs
                                                       , const NetAliasName* rhs ) const
  { return lhs->getName() < rhs->getName(); }


  typedef  std::set<NetAliasName*,NetAliasName::CompareByName>  AliasNameSet;


// -------------------------------------------------------------------
// Class  :  "Hurricane::AliasList" (Collection).

  typedef  GenericCollection<NetAliasHook*>  Aliases;
  typedef  GenericLocator   <NetAliasHook*>  AliasesLocator;
  typedef  GenericFilter    <NetAliasHook*>  AliasesFilter;


  class AliasList : public Collection<NetAliasHook*> {
    public:
    // Sub-Class: Locator.
      class Locator : public Hurricane::Locator<NetAliasHook*> {
        public:
                                                     Locator    ( const Net* );
          inline                                     Locator    ( const Locator& );
          virtual NetAliasHook*                      getElement () const;
          virtual Hurricane::Locator<NetAliasHook*>* getClone   () const;
          virtual bool                               isValid    () const;
          virtual void                               progress   ();
          virtual string                             _getString () const;
        protected:
          NetAliasHook* _hook;
      };

    public:
      inline                                        AliasList  ( const Net* net );
      inline                                        AliasList  ( const AliasList& );
      virtual Hurricane::Collection<NetAliasHook*>* getClone   () const;
      virtual Hurricane::Locator<NetAliasHook*>*    getLocator () const;
      virtual string                                _getString () const;
    protected:
      const Net* _net;
  };


  inline  AliasList::Locator::Locator ( const Locator& other )
    : Hurricane::Locator<NetAliasHook*>()
    , _hook(other._hook)
  { }
  
  inline  AliasList::AliasList ( const Net* net )
    : Hurricane::Collection<NetAliasHook*>()
    , _net(net)
  { }
  
  inline  AliasList::AliasList ( const AliasList& other )
    : Hurricane::Collection<NetAliasHook*>()
    , _net(other._net)
  { }


// -------------------------------------------------------------------
// Class  :  "JsonNetAlias".

  class JsonNetAlias : public JsonObject {
    public:
      static  void          initialize   ();
                            JsonNetAlias ( unsigned long flags );
      virtual string        getTypeName  () const;
      virtual JsonNetAlias* clone        ( unsigned long ) const;
      virtual void          toData       ( JsonStack& ); 
  };


}  // Namespace Hurricane.


INSPECTOR_P_SUPPORT(Hurricane::NetAliasHook);
INSPECTOR_P_SUPPORT(Hurricane::NetMainName);
INSPECTOR_P_SUPPORT(Hurricane::NetAliasName);

inline void  jsonWrite ( JsonWriter* w, const Hurricane::NetAliasHook* alias )
{
  const Hurricane::NetAliasName* aliasName = dynamic_cast<const Hurricane::NetAliasName*>( alias ); 
  if (aliasName) jsonWrite(w,aliasName);
}
    
#endif  // HURRICANE_NET_ALIAS_H
