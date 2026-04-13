
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
// |  C++ Module   :           "./ShortCircuitWidget.cpp"            |
// +-----------------------------------------------------------------+


#include "tramontana/ShortCircuit.h"
#include "tramontana/ShortCircuitWidget.h"


namespace Tramontana {

  using std::cerr;
  using std::endl;


// -------------------------------------------------------------------
// Class  :  "ShortCircuitAbstractItem".

  
  ShortCircuitAbstractItem::ShortCircuitAbstractItem ( ShortCircuitAbstractItem* parent )
    : _parent(parent)
  { }


  ShortCircuitAbstractItem::~ShortCircuitAbstractItem ()
  { }


  const ShortCircuit* ShortCircuitAbstractItem::getShortCircuit () const { return nullptr; }


// -------------------------------------------------------------------
// Class  :  "ShortCircuitRootItem".

  
  ShortCircuitRootItem::ShortCircuitRootItem ()
    : ShortCircuitAbstractItem(nullptr)
    , _equipotential(nullptr)
    , _childs()
  { }


  ShortCircuitRootItem::~ShortCircuitRootItem ()
  {
    for ( auto child : _childs ) delete child;
  }


  void ShortCircuitRootItem::setEqui ( const Equipotential* equi )
  {
    if (_equipotential == equi) return;
    _childs.clear();
    _equipotential = equi;
    if (_equipotential) {
      for ( const ShortCircuit* shortCircuits : _equipotential->getShortCircuits() )
        _childs.push_back( new ShortCircuitBaseItem( this, _equipotential, _childs.size() ));
    }
  }

  void      ShortCircuitRootItem::clear          () { _childs.clear(); }
  uint32_t  ShortCircuitRootItem::getType        () const { return ShortCircuitAbstractItem::Root; }
  int       ShortCircuitRootItem::getChildCount  () const { return _childs.size(); }
  int       ShortCircuitRootItem::getColumnCount () const { return 2; }
  int       ShortCircuitRootItem::getRow         () const { return 0; }
  Box       ShortCircuitRootItem::getBoundingBox () const { return (_equipotential ? _equipotential->getBoundingBox() : Box()); }


  ShortCircuitAbstractItem* ShortCircuitRootItem::getChild ( int row ) const
  {
    if ((row < 0) or (row >= (int)_childs.size())) return nullptr;
    return _childs[ row ];
  }


  QVariant ShortCircuitRootItem::data ( int column ) const
  {
    switch ( column ) {
      case 0: return QVariant( "#short" );
      case 1: return QVariant( "Net & shorting informations" );
    }
    return QVariant();
  }


  string ShortCircuitRootItem::_getString () const
  {
    string s = "<ShortCircuitRootItem " + getString(_childs.size()) + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "ShortCircuitBaseItem".
  

  ShortCircuitBaseItem::ShortCircuitBaseItem ( ShortCircuitAbstractItem* parent, const Equipotential* equi , size_t index )
    : ShortCircuitAbstractItem(parent)
    , _equipotential(equi)
    , _childs       ()
    , _shortIndex   (index)
  {
    _childs[0] = new ShortCircuitAttrItem( this, getShortCircuit(), CompA );
    _childs[1] = new ShortCircuitAttrItem( this, getShortCircuit(), CompB );
    _childs[2] = new ShortCircuitAttrItem( this, getShortCircuit(), Overlap );
  }


  ShortCircuitBaseItem::~ShortCircuitBaseItem ()
  {
    for ( auto child : _childs ) delete child;
  }


  uint32_t  ShortCircuitBaseItem::getType         () const { return ShortCircuitAbstractItem::Base; }
  int       ShortCircuitBaseItem::getChildCount   () const { return 3; }
  int       ShortCircuitBaseItem::getColumnCount  () const { return 3; }
  int       ShortCircuitBaseItem::getRow          () const { return _shortIndex; }
  Box       ShortCircuitBaseItem::getBoundingBox  () const { return Box(); }

  const ShortCircuit* ShortCircuitBaseItem::getShortCircuit () const
  { return _equipotential->getShortCircuits()[ _shortIndex ]; }


  ShortCircuitAbstractItem* ShortCircuitBaseItem::getChild ( int row ) const
  {
    if ((row < 0) or (row > 2)) return nullptr;
    return _childs[ row ];
  }


  QVariant ShortCircuitBaseItem::data ( int column ) const
  {
    const ShortCircuit* shortCircuit = _equipotential->getShortCircuits()[ _shortIndex ];
    if (column == 0) {
      QString shortNb = QString( "Short #%1 " ).arg( _shortIndex+1 );
      if (shortCircuit->isTopLevel    ()) shortNb += "[Top]";
      if (shortCircuit->isAcrossLevels()) shortNb += "[Accross]";
      if (shortCircuit->isDeepShort   ()) shortNb += "[Deep]";
      return QVariant( shortNb );
    }
    if (column == 1) {
      QString shortedNets = QString( "%1 + %2" );
      if (shortCircuit->isTopLevel())
        shortedNets = shortedNets.arg( getString( shortCircuit->getComponentA()->getNet()->getName() ).c_str())
                                 .arg( getString( shortCircuit->getComponentB()->getNet()->getName() ).c_str());
      if (shortCircuit->isAcrossLevels()) 
        shortedNets = shortedNets.arg( getString( shortCircuit->getComponentA()->getNet()->getName() ).c_str())
                                 .arg(            shortCircuit->getEquiBName().c_str() );
      if (shortCircuit->isDeepShort()) 
        shortedNets = shortedNets.arg( shortCircuit->getEquiAName().c_str() )
                                 .arg( shortCircuit->getEquiBName().c_str() );
      return QVariant( shortedNets );
    }
    return QVariant();
  }


  string ShortCircuitBaseItem::_getString () const
  {
    string s = "<ShortCircuitBaseItem [" +getString(_shortIndex) + "] " + getString(_equipotential) + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "ShortCircuitAttrItem".
  

  ShortCircuitAttrItem::ShortCircuitAttrItem ( ShortCircuitAbstractItem* parent, const ShortCircuit* sc, uint32_t type )
    : ShortCircuitAbstractItem(parent)
    , _type        (type)
    , _shortCircuit(sc)
  { }


  ShortCircuitAttrItem::~ShortCircuitAttrItem ()
  { }


  uint32_t  ShortCircuitAttrItem::getType         () const { return _type; }
  int       ShortCircuitAttrItem::getChildCount   () const { return 0; }
  int       ShortCircuitAttrItem::getColumnCount  () const { return 2; }


  int  ShortCircuitAttrItem::getRow () const
  {
    switch ( _type ) {
      case CompA:   return 0;
      case CompB:   return 1;
      case Overlap: return 2;
    }
    return 0;
  }

  
  Box  ShortCircuitAttrItem::getBoundingBox () const
  {
    switch ( _type ) {
      case CompA:   return _shortCircuit->getBoundingBoxA();
      case CompB:   return _shortCircuit->getBoundingBoxB();
      case Overlap: return _shortCircuit->getShortingBox();
    }
    return Box();
  }

  const ShortCircuit* ShortCircuitAttrItem::getShortCircuit () const
  { return _shortCircuit; }


  ShortCircuitAbstractItem* ShortCircuitAttrItem::getChild ( int row ) const
  { return nullptr; }


  QVariant ShortCircuitAttrItem::data ( int column ) const
  {
    switch ( column ) {
      case 0:
        switch ( _type ) {
          case CompA:   return QVariant( "CompA" );
          case CompB:   return QVariant( "CompB" );
          case Overlap: return QVariant( "Overlap" );
          default: QVariant();
        }
      case 1:
        switch ( _type ) {
          case CompA:   return QVariant( getString(_shortCircuit->getComponentA ()).c_str() );
          case CompB:   return QVariant( getString(_shortCircuit->getComponentB ()).c_str() );
          case Overlap: return QVariant( getString(_shortCircuit->getShortingBox()).c_str() );
        }
    }
    return QVariant();
  }


  string ShortCircuitAttrItem::_getString () const
  {
    string attrName = "Attr";
    switch ( _type ) {
      case CompA:   attrName = "CompA";
      case CompB:   attrName = "CompB";
      case Overlap: attrName = "Overlap";
    }
    
    string s = "<ShortCircuit" + attrName + "Item " + getString(_shortCircuit) + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "ShortCircuitModel".

  
  ShortCircuitModel::ShortCircuitModel ( QObject *parent )
    : QAbstractItemModel(parent)
    , _rootItem()
  { }

  
  ShortCircuitModel::~ShortCircuitModel ()
  { }


  QModelIndex  ShortCircuitModel::index ( int row, int column, const QModelIndex& parent ) const
  {
    if (not hasIndex(column,row, parent)) return QModelIndex();

    const ShortCircuitAbstractItem* parentItem = nullptr;
    if (not parent.isValid())
      parentItem = &_rootItem;
    else
      parentItem = reinterpret_cast<ShortCircuitAbstractItem*>( parent.internalPointer() );

    ShortCircuitAbstractItem* childItem = parentItem->getChild( row );
    if (childItem)
      return createIndex( row, column, childItem );
    return QModelIndex();
  }


  QModelIndex ShortCircuitModel::parent ( const QModelIndex& index ) const
  {
    if (not index.isValid()) return QModelIndex();

    ShortCircuitAbstractItem* childItem  = reinterpret_cast<ShortCircuitAbstractItem*>( index.internalPointer() );
    ShortCircuitAbstractItem* parentItem = childItem->getParent();

    if (parentItem == &_rootItem) return QModelIndex();
    return createIndex( parentItem->getRow(), 0, parentItem );
  }


  int ShortCircuitModel::rowCount ( const QModelIndex &parent ) const
  {
  //if (parent.column() > 0) return 0;

    const ShortCircuitAbstractItem* parentItem = nullptr;
    if (not parent.isValid())
      parentItem = &_rootItem;
    else
      parentItem = reinterpret_cast<ShortCircuitAbstractItem*>( parent.internalPointer() );
    return parentItem->getChildCount();
  }    


  int ShortCircuitModel::columnCount ( const QModelIndex& parent ) const
  {
    if (parent.isValid()) {
      const ShortCircuitAbstractItem* parentItem = reinterpret_cast<ShortCircuitAbstractItem*>( parent.internalPointer() );
      return parentItem->getColumnCount();
    }
    return _rootItem.getColumnCount();
  }
  

  QVariant ShortCircuitModel::data ( const QModelIndex& index, int role ) const
  {
    if (not index.isValid())     return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    ShortCircuitAbstractItem* item = reinterpret_cast<ShortCircuitAbstractItem*>( index.internalPointer() );
    return item->data( index.column() );
  }
  

  Qt::ItemFlags ShortCircuitModel::flags ( const QModelIndex &index ) const
  {
    if (not index.isValid()) return Qt::NoItemFlags;
    return QAbstractItemModel::flags( index );
  }


  QVariant ShortCircuitModel::headerData ( int section, Qt::Orientation orientation, int role ) const
  {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole))
      return _rootItem.data( section );
    return QVariant();
  }
  

  void ShortCircuitModel::setEqui ( const Equipotential* equi )
  {
    emit layoutAboutToBeChanged ();
    _rootItem.setEqui( equi );
    emit layoutChanged ();
  }
  

  void ShortCircuitModel::clear ()
  { _rootItem.clear(); }

  
}  // Tramontana namespace.
