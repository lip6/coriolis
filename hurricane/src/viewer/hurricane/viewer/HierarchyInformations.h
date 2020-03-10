// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/HierarchyInformations.h"    |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_HIERARCHY_INFORMATIONS_H
#define  HURRICANE_HIERARCHY_INFORMATIONS_H

#include <vector>
#include <QRegExp>
#include <QVariant>
#include "hurricane/Commons.h"
#include "hurricane/Path.h"


namespace Hurricane {

  class Instance;
  class Cell;


// -------------------------------------------------------------------
// Class  :  "HierarchyInfos"


  class HierarchyInfos {
    public:
      enum Flags { NoFlags=0x0000, Collapsed=0x0001 };
    public:
                                                  HierarchyInfos    ( HierarchyInfos* parent, size_t rowInParent );
      virtual                                    ~HierarchyInfos    ();
      virtual bool                                isRoot            () const = 0;
      virtual bool                                isTerminalNetlist () const = 0;
      virtual bool                                isCollapsed       () const = 0;
      static  int                                 getColumnCount    ();
      static  QVariant                            getColumnName     ( int column );
              QVariant                            getColumn         ( int column ) const;
      virtual const HierarchyInfos*               getRow            ( int row ) const = 0;
      inline  const HierarchyInfos*               getParent         () const;
      inline  size_t                              getRowInParent    () const;
      virtual int                                 size              () const = 0;
      virtual Cell*                               getMasterCell     () const = 0;
      virtual const Instance*                     getInstance       () const = 0;
      virtual const std::vector<HierarchyInfos*>& getInstances      () const = 0;
      inline  const Name                          getName           () const;
      virtual Path                                getPath           () const;
      virtual QString                             getFilterPattern  () const = 0;
      virtual void                                setFilterPattern  ( const QString& ) = 0;
      inline  void                                setRowInParent    ( size_t );
      virtual void                                expand            () = 0;
      virtual void                                collapse          () = 0;
    protected:
      HierarchyInfos* _parent;
      size_t          _rowInParent;
  };


  inline size_t                HierarchyInfos::getRowInParent () const { return _rowInParent; }
  inline const HierarchyInfos* HierarchyInfos::getParent      () const { return _parent; }
  inline const Name            HierarchyInfos::getName        () const { return isRoot() ? Name("Root") : getInstance()->getName(); }
  inline void                  HierarchyInfos::setRowInParent ( size_t rowInParent ) { _rowInParent=rowInParent; }


// -------------------------------------------------------------------
// Class  :  "TerminalNetlistHierarchyInfos"


  class TerminalNetlistHierarchyInfos : public HierarchyInfos {
    public:
                                                  TerminalNetlistHierarchyInfos
                                                                     ( const Instance*
                                                                     , HierarchyInfos* parent
                                                                     , size_t          rowInParent );
      virtual                                    ~TerminalNetlistHierarchyInfos
                                                                     ();
      virtual bool                                isRoot             () const;
      virtual bool                                isTerminalNetlist  () const;
      virtual bool                                isCollapsed        () const;
      virtual const HierarchyInfos*               getRow             ( int row ) const;
      virtual int                                 size               () const;
      virtual Cell*                               getMasterCell      () const;
      virtual const Instance*                     getInstance        () const;
      virtual const std::vector<HierarchyInfos*>& getInstances       () const;
      virtual void                                setFilterPattern   ( const QString& );
      virtual QString                             getFilterPattern   () const;
      virtual void                                expand             ();
      virtual void                                collapse           ();
    protected:
             const Instance*                     _instance;
      static const std::vector<HierarchyInfos*>  _instances;
  };


// -------------------------------------------------------------------
// Class  :  "InstHierarchyInfos"


  class InstHierarchyInfos : public HierarchyInfos {
    public:
                                                  InstHierarchyInfos ( const Instance*
                                                                     , HierarchyInfos* parent
                                                                     , size_t          rowInParent );
      virtual                                    ~InstHierarchyInfos ();
      virtual bool                                isRoot             () const;
      virtual bool                                isTerminalNetlist  () const;
      virtual bool                                isCollapsed        () const;
      virtual const HierarchyInfos*               getRow             ( int row ) const;
      virtual int                                 size               () const;
      virtual Cell*                               getMasterCell      () const;
      virtual const Instance*                     getInstance        () const;
      virtual const std::vector<HierarchyInfos*>& getInstances       () const;
      virtual void                                setFilterPattern   ( const QString& );
      virtual QString                             getFilterPattern   () const;
      virtual void                                expand             ();
      virtual void                                collapse           ();
    protected:
      unsigned int                  _flags;
      const Instance*               _instance;
      std::vector<HierarchyInfos*>  _instances;
      QRegExp                       _filter;
  };


// -------------------------------------------------------------------
// Class  :  "TopCellHierarchyInfos"


  class TopCellHierarchyInfos : public InstHierarchyInfos {
    public:
                                        TopCellHierarchyInfos ( Cell*, HierarchyInfos* parent );
      virtual                          ~TopCellHierarchyInfos ();
      virtual bool                      isRoot                () const;
      virtual Cell*                     getMasterCell         () const;
      virtual const Instance*           getInstance           () const;
              void                      setCell               ( Cell* );
    protected:
      Cell* _rootCell;
  };


// -------------------------------------------------------------------
// Class  :  "RootHierarchyInfos"


  class RootHierarchyInfos : public HierarchyInfos {
    public:
                                                  RootHierarchyInfos ( Cell* );
      virtual                                    ~RootHierarchyInfos ();
      virtual bool                                isRoot             () const;
      virtual bool                                isTerminalNetlist  () const;
      virtual bool                                isCollapsed        () const;
      virtual const HierarchyInfos*               getRow             ( int row ) const;
      virtual int                                 size               () const;
      virtual Cell*                               getMasterCell      () const;
      virtual const Instance*                     getInstance        () const;
      virtual const std::vector<HierarchyInfos*>& getInstances       () const;
      virtual void                                setFilterPattern   ( const QString& );
      virtual QString                             getFilterPattern   () const;
      virtual void                                expand             ();
      virtual void                                collapse           ();
              void                                setCell            ( Cell* );
    protected:
      std::vector<HierarchyInfos*>  _instances;
  };


}  // Hurricane namespace.

#endif // HURRICANE_HIERARCHY_INFORMATIONS_H
