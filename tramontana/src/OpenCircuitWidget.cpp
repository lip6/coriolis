
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
// |  C++ Module   :           "./OpenCircuitWidget.cpp"             |
// +-----------------------------------------------------------------+


#include "tramontana/OpenCircuitWidget.h"


namespace Tramontana {

  using std::cerr;
  using std::endl;
  using Hurricane::Error;


// -------------------------------------------------------------------
// Class  :  "OpenCircuitAbstractItem".

  
  OpenCircuitAbstractItem::OpenCircuitAbstractItem ( OpenCircuitAbstractItem* parent )
    : _parent(parent)
  { }


  OpenCircuitAbstractItem::~OpenCircuitAbstractItem ()
  { }


  const TramontanaEngine::OpenCircuit& OpenCircuitAbstractItem::getOpenCircuit () const
  {
    static TramontanaEngine::OpenCircuit emptyOpen;
    return emptyOpen;
  }


  const Equipotential* OpenCircuitAbstractItem::getEquipotential () const
  { return nullptr; }


  void  OpenCircuitAbstractItem::setTramontana ( const TramontanaEngine* )
  {
    throw Error( "OpenCircuitAbstractItem::setTramontana(): Must be called only on OpenCircuitRootItem" );
  }


// -------------------------------------------------------------------
// Class  :  "OpenCircuitRootItem".

  
  OpenCircuitRootItem::OpenCircuitRootItem ()
    : OpenCircuitAbstractItem(nullptr)
    , _tramontana(nullptr)
    , _childs()
  { }


  OpenCircuitRootItem::~OpenCircuitRootItem ()
  {
    for ( auto child : _childs ) delete child;
  }


  void OpenCircuitRootItem::setTramontana ( const TramontanaEngine* tramontana )
  {
    if (_tramontana == tramontana) return;
    _childs.clear();
    _tramontana = tramontana;
    if (_tramontana) {
      for ( auto openNet : _tramontana->getOpenNets() )
        _childs.push_back( new OpenCircuitNetItem( this, openNet, _childs.size() ));
    }
  }

  void      OpenCircuitRootItem::clear          () { _childs.clear(); _tramontana=nullptr; }
  uint32_t  OpenCircuitRootItem::getType        () const { return OpenCircuitAbstractItem::TypeRoot; }
  int       OpenCircuitRootItem::getChildCount  () const { return _childs.size(); }
  int       OpenCircuitRootItem::getColumnCount () const { return 1; }
  int       OpenCircuitRootItem::getRow         () const { return 0; }
  Box       OpenCircuitRootItem::getBoundingBox () const { return Box(); }


  OpenCircuitAbstractItem* OpenCircuitRootItem::getChild ( int row ) const
  {
    if ((row < 0) or (row >= (int)_childs.size())) return nullptr;
    return _childs[ row ];
  }


  QVariant OpenCircuitRootItem::data ( int column ) const
  {
    if (column) return QVariant();
    return QVariant( "#open" );
  }


  string OpenCircuitRootItem::_getString () const
  {
    string s = "<OpenCircuitRootItem " + getString(_childs.size()) + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "OpenCircuitNetItem".
  

  OpenCircuitNetItem::OpenCircuitNetItem ( OpenCircuitAbstractItem* parent
                                         , const TramontanaEngine::OpenCircuit& openCircuit
                                         , int row )
    : OpenCircuitAbstractItem(parent)
    , _openCircuit(openCircuit)
    , _childs     ()
    , _row        (row)
  {
    for ( const Equipotential* equi : _openCircuit.second )
      _childs.push_back( new OpenCircuitEquiItem( this, equi, _childs.size() ));
  }


  OpenCircuitNetItem::~OpenCircuitNetItem ()
  {
    for ( auto child : _childs ) delete child;
  }


  const TramontanaEngine::OpenCircuit& OpenCircuitNetItem::getOpenCircuit () const
  { return _openCircuit; }


  uint32_t  OpenCircuitNetItem::getType         () const { return OpenCircuitAbstractItem::TypeNet; }
  int       OpenCircuitNetItem::getChildCount   () const { return _childs.size(); }
  int       OpenCircuitNetItem::getColumnCount  () const { return 1; }
  int       OpenCircuitNetItem::getRow          () const { return _row; }
  Box       OpenCircuitNetItem::getBoundingBox  () const { return _openCircuit.first->getBoundingBox(); }


  OpenCircuitAbstractItem* OpenCircuitNetItem::getChild ( int row ) const
  {
    if ((row < 0) or (row+1 > (int)_childs.size())) return nullptr;
    return _childs[ row ];
  }


  QVariant OpenCircuitNetItem::data ( int column ) const
  {
    if (column != 0) return QVariant();
    
    string  text   = (_childs.empty() ? "N/A" : getString(_childs.size()-1))
                   + " open(s) on \"" + getString(_openCircuit.first->getName()) + "\"";
    QString openNb = QString( text.c_str() );
    return QVariant( openNb );
  }


  string OpenCircuitNetItem::_getString () const
  {
    string s = "<OpenCircuitNetItem [" +getString(_row) + "] " + getString(_openCircuit.first) + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "OpenCircuitEquiItem".
  

  OpenCircuitEquiItem::OpenCircuitEquiItem ( OpenCircuitAbstractItem* parent, const Equipotential* equi, int row )
    : OpenCircuitAbstractItem(parent)
    , _equipotential(equi)
    , _row(row)
  { }


  OpenCircuitEquiItem::~OpenCircuitEquiItem ()
  { }


  uint32_t             OpenCircuitEquiItem::getType          () const { return TypeEqui; }
  int                  OpenCircuitEquiItem::getChildCount    () const { return 0; }
  int                  OpenCircuitEquiItem::getColumnCount   () const { return 1; }
  const Equipotential* OpenCircuitEquiItem::getEquipotential () const { return _equipotential; }


  int  OpenCircuitEquiItem::getRow () const
  { return _row; }

  
  Box  OpenCircuitEquiItem::getBoundingBox () const
  {
    OpenCircuitNetItem* parent = static_cast<OpenCircuitNetItem*>( getParent() );
    return parent->getBoundingBox();
  }


  OpenCircuitAbstractItem* OpenCircuitEquiItem::getChild ( int row ) const
  { return nullptr; }


  QVariant OpenCircuitEquiItem::data ( int column ) const
  {
    if (column != 0) return QVariant();
    return QVariant( getString(_equipotential).c_str() );
  }


  string OpenCircuitEquiItem::_getString () const
  {
    string s = "<OpenCircuitEquiItem " + getString(_equipotential) + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "OpenCircuitModel".

  
  OpenCircuitModel::OpenCircuitModel ( QObject *parent )
    : QAbstractItemModel(parent)
    , _rootItem()
  { }

  
  OpenCircuitModel::~OpenCircuitModel ()
  { }


  QModelIndex  OpenCircuitModel::index ( int row, int column, const QModelIndex& parent ) const
  {
    if (not hasIndex(column,row, parent)) return QModelIndex();

    const OpenCircuitAbstractItem* parentItem = nullptr;
    if (not parent.isValid())
      parentItem = &_rootItem;
    else
      parentItem = reinterpret_cast<OpenCircuitAbstractItem*>( parent.internalPointer() );

    OpenCircuitAbstractItem* childItem = parentItem->getChild( row );
    if (childItem)
      return createIndex( row, column, childItem );
    return QModelIndex();
  }


  QModelIndex OpenCircuitModel::parent ( const QModelIndex& index ) const
  {
    if (not index.isValid()) return QModelIndex();

    OpenCircuitAbstractItem* childItem  = reinterpret_cast<OpenCircuitAbstractItem*>( index.internalPointer() );
    OpenCircuitAbstractItem* parentItem = childItem->getParent();

    if (parentItem == &_rootItem) return QModelIndex();
    return createIndex( parentItem->getRow(), 0, parentItem );
  }


  int OpenCircuitModel::rowCount ( const QModelIndex &parent ) const
  {
    if (parent.column() > 0) return 0;

    const OpenCircuitAbstractItem* parentItem = nullptr;
    if (not parent.isValid())
      parentItem = &_rootItem;
    else
      parentItem = reinterpret_cast<OpenCircuitAbstractItem*>( parent.internalPointer() );
    return parentItem->getChildCount();
  }    


  int OpenCircuitModel::columnCount ( const QModelIndex& parent ) const
  {
    if (parent.isValid()) {
      const OpenCircuitAbstractItem* parentItem = reinterpret_cast<OpenCircuitAbstractItem*>( parent.internalPointer() );
    //return parentItem->getColumnCount();
      return parentItem->getChildCount();
    }
  //return _rootItem.getColumnCount();
    return _rootItem.getChildCount();
  }
  

  QVariant OpenCircuitModel::data ( const QModelIndex& index, int role ) const
  {
    if (not index.isValid())     return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    OpenCircuitAbstractItem* item = reinterpret_cast<OpenCircuitAbstractItem*>( index.internalPointer() );
    return item->data( index.column() );
  }
  

  Qt::ItemFlags OpenCircuitModel::flags ( const QModelIndex &index ) const
  {
    if (not index.isValid()) return Qt::NoItemFlags;
    return QAbstractItemModel::flags( index );
  }


  QVariant OpenCircuitModel::headerData ( int section, Qt::Orientation orientation, int role ) const
  {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole))
      return _rootItem.data( section );
    return QVariant();
  }
  

  void OpenCircuitModel::setTramontana ( const TramontanaEngine* tramontana )
  {
    emit layoutAboutToBeChanged ();
    _rootItem.setTramontana( tramontana );
    emit layoutChanged ();
  }
  

  void OpenCircuitModel::clear ()
  { _rootItem.clear(); }


}  // Tramontana namespace.
