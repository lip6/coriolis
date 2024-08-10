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
// |  C++ Header   :  "./tramontana/OpenCircuitWidget.h"             |
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

#include "tramontana/TramontanaEngine.h"


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
// Class  :  "OpenCircuitAbstractItem".


  class OpenCircuitAbstractItem {
    public:
      static const uint32_t  TypeRoot = (1 << 0);
      static const uint32_t  TypeNet  = (1 << 1);
      static const uint32_t  TypeEqui = (1 << 2);
    public:
                                       OpenCircuitAbstractItem ( OpenCircuitAbstractItem* );
      virtual                         ~OpenCircuitAbstractItem ();
      virtual uint32_t                 getType                  () const = 0;
      inline  OpenCircuitAbstractItem* getParent                () const; 
      virtual OpenCircuitAbstractItem* getChild                 ( int row ) const = 0; 
      virtual int                      getChildCount            () const = 0;
      virtual int                      getColumnCount           () const = 0;
      virtual int                      getRow                   () const = 0;
      virtual QVariant                 data                     ( int column ) const = 0;
      virtual Box                      getBoundingBox           () const = 0;
      virtual const TramontanaEngine::OpenCircuit&
                                       getOpenCircuit           () const;
      virtual const Equipotential*     getEquipotential         () const;
              void                     setTramontana            ( const TramontanaEngine* );
      virtual std::string              _getString               () const = 0;
    private:
      OpenCircuitAbstractItem* _parent;
  };

  
  inline  OpenCircuitAbstractItem* OpenCircuitAbstractItem::getParent () const { return _parent; }


  class OpenCircuitRootItem : public OpenCircuitAbstractItem {
    public:
                                       OpenCircuitRootItem ();
      virtual                         ~OpenCircuitRootItem ();
      virtual uint32_t                 getType             () const;
      virtual OpenCircuitAbstractItem* getChild            ( int row ) const; 
      virtual int                      getChildCount       () const;
      virtual int                      getColumnCount      () const;
      virtual int                      getRow              () const;
      virtual QVariant                 data                ( int column ) const;
              void                     setTramontana       ( const TramontanaEngine* );
      virtual Box                      getBoundingBox      () const;
              void                     clear               ();
      virtual std::string              _getString          () const;
    private:
      const TramontanaEngine*                _tramontana;
      std::vector<OpenCircuitAbstractItem*>  _childs;
  };


  class OpenCircuitNetItem : public OpenCircuitAbstractItem {
    public:
                                        OpenCircuitNetItem ( OpenCircuitAbstractItem*
                                                           , const TramontanaEngine::OpenCircuit&
                                                           , int row );
      virtual                          ~OpenCircuitNetItem ();
      virtual uint32_t                  getType            () const;
      virtual OpenCircuitAbstractItem*  getChild           ( int row ) const; 
      virtual int                       getChildCount      () const;
      virtual int                       getColumnCount     () const;
      virtual int                       getRow             () const;
      virtual QVariant                  data               ( int column ) const;
      virtual Box                       getBoundingBox     () const;
      virtual const TramontanaEngine::OpenCircuit&
                                       getOpenCircuit      () const;
      virtual std::string               _getString         () const;
    private:
      const TramontanaEngine::OpenCircuit    _openCircuit;
      std::vector<OpenCircuitAbstractItem*>  _childs;
      int                                    _row;
  };


  class OpenCircuitEquiItem : public OpenCircuitAbstractItem {
    public:
                                        OpenCircuitEquiItem  ( OpenCircuitAbstractItem*, const Equipotential*, int row );
      virtual                          ~OpenCircuitEquiItem  ();
      virtual uint32_t                  getType              () const;
      virtual OpenCircuitAbstractItem*  getChild             ( int row ) const; 
      virtual int                       getChildCount        () const;
      virtual int                       getColumnCount       () const;
      virtual int                       getRow               () const;
      virtual QVariant                  data                 ( int column ) const;
      virtual Box                       getBoundingBox       () const;
      virtual const Equipotential*      getEquipotential     () const;
      virtual std::string               _getString           () const;
    private:
      const Equipotential* _equipotential;
            int            _row;
  };


// -------------------------------------------------------------------
// Class  :  "OpenCircuitModel".

  
  class OpenCircuitModel : public QAbstractItemModel {
      Q_OBJECT
    public:
      explicit              OpenCircuitModel ( QObject *parent=nullptr);
                           ~OpenCircuitModel ();
              Qt::ItemFlags flags            ( const QModelIndex& ) const override;
              QModelIndex   index            ( int row, int column, const QModelIndex& parent=QModelIndex()) const override;
              QModelIndex   parent           ( const QModelIndex& ) const override;
              int           rowCount         ( const QModelIndex& parent=QModelIndex()) const override;
              int           columnCount      ( const QModelIndex& parent=QModelIndex()) const override;
              QVariant      headerData       ( int section, Qt::Orientation, int role=Qt::DisplayRole) const override;
              QVariant      data             ( const QModelIndex&, int role) const override;
              void          setTramontana    ( const TramontanaEngine* );
              void          clear            ();
    private:
      OpenCircuitRootItem  _rootItem;
  };


}  // Tramontana namespace.


GETSTRING_POINTER_SUPPORT(Tramontana::OpenCircuitAbstractItem);
IOSTREAM_POINTER_SUPPORT(Tramontana::OpenCircuitAbstractItem);
