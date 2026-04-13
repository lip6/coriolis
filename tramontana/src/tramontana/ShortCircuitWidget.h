
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
// |  E-mail       :            Jean-Paul.Chaput@lip6.fr             |
// | =============================================================== |
// |  C++ Header   :  "./tramontana/ShortCircuitWidget.h"            |
// +-----------------------------------------------------------------+


#pragma  once
#include <set>
#include <QWidget>
#include <QTreeView>
#include <QHeaderView>
#include <QItemDelegate>
#include "hurricane/Commons.h"
#include "hurricane/Bug.h"

namespace Hurricane {
  class CellWidget;
}

#include "tramontana/Equipotential.h"


class QModelIndex;
class QLineEdit;
class QLabel;


namespace Tramontana {

  using std::string;
  using std::set;
  using Hurricane::Cell;
  using Hurricane::OccurrenceSet;
  using Hurricane::CellWidget;


// -------------------------------------------------------------------
// Class  :  "ShortCircuitAbstractItem".


  class ShortCircuitAbstractItem {
    public:
      static const uint32_t  Root    = (1 << 0);
      static const uint32_t  Base    = (1 << 1);
      static const uint32_t  CompA   = (1 << 2);
      static const uint32_t  CompB   = (1 << 3);
      static const uint32_t  Overlap = (1 << 4);
    public:
                                        ShortCircuitAbstractItem ( ShortCircuitAbstractItem* );
      virtual                          ~ShortCircuitAbstractItem ();
      virtual uint32_t                  getType                  () const = 0;
      inline  ShortCircuitAbstractItem* getParent                () const; 
      virtual ShortCircuitAbstractItem* getChild                 ( int row ) const = 0; 
      virtual int                       getChildCount            () const = 0;
      virtual int                       getColumnCount           () const = 0;
      virtual int                       getRow                   () const = 0;
      virtual QVariant                  data                     ( int column ) const = 0;
      virtual Box                       getBoundingBox           () const = 0;
      virtual const ShortCircuit*       getShortCircuit          () const;
              void                      setEqui                  ( Equipotential* ) const;
      virtual std::string               _getString               () const = 0;
    private:
      ShortCircuitAbstractItem* _parent;
  };

  
  inline  ShortCircuitAbstractItem* ShortCircuitAbstractItem::getParent () const { return _parent; }


  class ShortCircuitRootItem : public ShortCircuitAbstractItem {
    public:
                                        ShortCircuitRootItem ();
      virtual                          ~ShortCircuitRootItem ();
      virtual uint32_t                  getType              () const;
      virtual ShortCircuitAbstractItem* getChild             ( int row ) const; 
      virtual int                       getChildCount        () const;
      virtual int                       getColumnCount       () const;
      virtual int                       getRow               () const;
      virtual QVariant                  data                 ( int column ) const;
              void                      setEqui              ( const Equipotential* );
      virtual Box                       getBoundingBox       () const;
              void                      clear                ();
      virtual std::string               _getString           () const;
    private:
      const Equipotential*                    _equipotential;
      std::vector<ShortCircuitAbstractItem*>  _childs;
  };


  class ShortCircuitBaseItem : public ShortCircuitAbstractItem {
    public:
                                        ShortCircuitBaseItem ( ShortCircuitAbstractItem*, const Equipotential*, size_t );
      virtual                          ~ShortCircuitBaseItem ();
      virtual uint32_t                  getType              () const;
      virtual ShortCircuitAbstractItem* getChild             ( int row ) const; 
      virtual int                       getChildCount        () const;
      virtual int                       getColumnCount       () const;
      virtual int                       getRow               () const;
      virtual QVariant                  data                 ( int column ) const;
      virtual Box                       getBoundingBox       () const;
      virtual const ShortCircuit*       getShortCircuit      () const;
      virtual std::string               _getString           () const;
    private:
      const Equipotential*                    _equipotential;
      std::array<ShortCircuitAbstractItem*,3> _childs;
      size_t                                  _shortIndex;
  };


  class ShortCircuitAttrItem : public ShortCircuitAbstractItem {
    public:
                                        ShortCircuitAttrItem  ( ShortCircuitAbstractItem*, const ShortCircuit*, uint32_t type );
      virtual                          ~ShortCircuitAttrItem  ();
      virtual uint32_t                  getType               () const;
      virtual ShortCircuitAbstractItem* getChild              ( int row ) const; 
      virtual int                       getChildCount         () const;
      virtual int                       getColumnCount        () const;
      virtual int                       getRow                () const;
      virtual QVariant                  data                  ( int column ) const;
      virtual Box                       getBoundingBox        () const;
      virtual const ShortCircuit*       getShortCircuit       () const;
      virtual std::string               _getString            () const;
    private:
            uint32_t      _type;
      const ShortCircuit* _shortCircuit;
  };


// -------------------------------------------------------------------
// Class  :  "ShortCircuitModel".

  
  class ShortCircuitModel : public QAbstractItemModel {
      Q_OBJECT
    public:
      explicit              ShortCircuitModel ( QObject *parent=nullptr);
                           ~ShortCircuitModel ();
              Qt::ItemFlags flags             ( const QModelIndex& ) const override;
              QModelIndex   index             ( int row, int column, const QModelIndex& parent=QModelIndex()) const override;
              QModelIndex   parent            ( const QModelIndex& ) const override;
              int           rowCount          ( const QModelIndex& parent=QModelIndex()) const override;
              int           columnCount       ( const QModelIndex& parent=QModelIndex()) const override;
              QVariant      headerData        ( int section, Qt::Orientation, int role=Qt::DisplayRole) const override;
              QVariant      data              ( const QModelIndex&, int role) const override;
              void          setEqui           ( const Equipotential* );
              void          clear             ();
    private:
      ShortCircuitRootItem  _rootItem;
  };


}  // Tramontana namespace.


GETSTRING_POINTER_SUPPORT(Tramontana::ShortCircuitAbstractItem);
IOSTREAM_POINTER_SUPPORT(Tramontana::ShortCircuitAbstractItem);
