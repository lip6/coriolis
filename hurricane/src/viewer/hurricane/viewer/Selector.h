// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/Selector.h"                 |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_SELECTOR_H
#define  HURRICANE_SELECTOR_H

#include  "hurricane/Property.h"
#include  "hurricane/Selectors.h"
#include  "hurricane/Occurrence.h"


namespace Hurricane {


  class CellWidget;


  struct SelectorLess {
      bool operator () ( const Selector* lhs, const Selector* rhs ) const;
  };


  class Selector : public PrivateProperty {
    public:
      enum Flags { InModel             = (1<<0)
                 , Selected            = (1<<1)
                 , ToggledByController = (1<<2)
                 };
    public:
    // Constructor.
      static  Selector*                  create               ( Occurrence& occurrence );
    // Methods.                                               
      static  const Name&                getPropertyName      ();
      virtual Name                       getName              () const;
              const Quark*               getQuark             () const;
              Occurrence                 getOccurrence        () const;
              Path                       getPath              () const;
              Entity*                    getEntity            () const;
      inline  map<CellWidget*,uint32_t>& getCellWidgetSet     ();
      inline  bool                       isSelected           ( CellWidget* ) const;
      inline  bool                       isInModel            ( CellWidget* ) const;
      inline  bool                       isToggleByController ( CellWidget* ) const;
              bool                       isAttachedTo         ( CellWidget* ) const;
              void                       attachTo             ( CellWidget* );
              void                       detachFrom           ( CellWidget* , bool inDeletion=false );
              uint32_t                   getFlags             ( CellWidget* ) const;
              void                       setFlags             ( CellWidget* , uint32_t );
              void                       resetFlags           ( CellWidget* , uint32_t );
              void                       toggle               ( CellWidget* );
    // Inspector Managment.                                   
      virtual string                     _getTypeName         () const;
      virtual string                     _getString           () const;
      virtual Record*                    _getRecord           () const;

    protected:
    // Internal: Attributes.
      static  const Name                 _propertyName;
              map<CellWidget*,uint32_t>  _cellWidgets;

    protected:
    // Internal: Constructor.
                                Selector         ();
      virtual void              _preDestroy      ();
  };


// Inline Functions.
  inline map<CellWidget*,uint32_t>& Selector::getCellWidgetSet     () { return _cellWidgets; }
  inline bool                       Selector::isInModel            ( CellWidget* w ) const { return getFlags(w) & InModel ; }
  inline bool                       Selector::isSelected           ( CellWidget* w ) const { return getFlags(w) & Selected; }
  inline bool                       Selector::isToggleByController ( CellWidget* w ) const { return getFlags(w) & ToggledByController; }

  typedef set<Selector*,SelectorLess>  SelectorSet;


} // Hurricane namespace.

#endif  // HURRICANE_SELECTOR_H
