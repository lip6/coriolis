

// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$


#include  "hurricane/Commons.h"
#include  "hurricane/Error.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Relation.h"

#include  "crlcore/Utilities.h"
#include  "crlcore/ToolEngine.h"


namespace {


  using std::cout;
  using std::cerr;
  using std::endl;
  using std::string;
  using Hurricane::ForEachIterator;
  using Hurricane::_TName;
  using Hurricane::Error;
  using Hurricane::Name;
  using Hurricane::Relation;
  using Hurricane::Record;
  using Hurricane::Cell;
  using CRL::ToolEngine;
    

  const Name  ToolEnginesRelationName = "ToolEnginesRelationName";
    

// -------------------------------------------------------------------
// Class  :  "CRL::ToolEnginesRelation".

  
  class ToolEnginesRelation : public Relation {
    public:
    // Static Methods.
      static ToolEnginesRelation* getToolEnginesRelation          ( const Cell* cell );
    // Constructor.
      static ToolEnginesRelation* create                          ( Cell* masterOwner );
    // Methods.
      virtual Name                getName                         () const;
	  inline  unsigned int        getPlacementModificationFlag    () const;
      inline  unsigned int        updatePlacementModificationFlag ();
	  inline  unsigned int        getRoutingModificationFlag      () const;
      inline  unsigned int        updateRoutingModificationFlag   ();
      virtual string              _getTypeName                    () const;
      virtual Record*             _getRecord                      () const;

    private:
    // Internal: Attributes
              unsigned int        _placementModificationFlag;
              unsigned int        _routingModificationFlag;

    // Internal: Constructor.
                                  ToolEnginesRelation             ( Cell* masterOwner );
    protected:
      virtual void                _preDestroy                     ();
  };


  ToolEnginesRelation::ToolEnginesRelation ( Cell* masterOwner )
    : Relation                  (masterOwner)
    , _placementModificationFlag(0)
    , _routingModificationFlag  (0)
  { }


  ToolEnginesRelation* ToolEnginesRelation::create ( Cell* masterOwner )
  {
    ToolEnginesRelation* enginesRelation = new ToolEnginesRelation(masterOwner);

    enginesRelation->_postCreate();

    return enginesRelation;
  }


  void  ToolEnginesRelation::_preDestroy ()
  {
    ToolEngine* tool;
    while ( (tool = getSlaveOwners().getSubSet<ToolEngine*>().getFirst()) ) {
      tool->setInRelationDestroy ( true );
      tool->destroy ();
    }
    Relation::_preDestroy ();
  }


  Name  ToolEnginesRelation::getName () const
  { return ToolEnginesRelationName; }


  inline unsigned int  ToolEnginesRelation::getPlacementModificationFlag () const
  { return _placementModificationFlag; }


  inline unsigned int  ToolEnginesRelation::getRoutingModificationFlag () const
  { return _routingModificationFlag; }


  inline unsigned int  ToolEnginesRelation::updatePlacementModificationFlag ()
  { return ++_placementModificationFlag; }


  inline unsigned int  ToolEnginesRelation::updateRoutingModificationFlag ()
  { return ++_routingModificationFlag; }


  string  ToolEnginesRelation::_getTypeName () const
  { return _TName ( "ToolEnginesRelation" ); }


  Record* ToolEnginesRelation::_getRecord () const
  {
    Record* record = Relation::_getRecord();
    if ( record ) {
      record->add ( getSlot ( "placementModificationFlag", &_placementModificationFlag ) );
      record->add ( getSlot ( "routingModificationFlag"  , &_routingModificationFlag   ) );
    }
    return record;
  }


  ToolEnginesRelation* ToolEnginesRelation::getToolEnginesRelation ( const Cell* cell )
  {
    if ( !cell )
      throw Error ( "Can't get " + _TName("ToolEnginesRelation") + " : empty cell" );
    Property* property = cell->getProperty(ToolEnginesRelationName);
    if ( !property )
      return NULL;
    else {
      ToolEnginesRelation* enginesRelation = dynamic_cast<ToolEnginesRelation*>(property);
      if ( !enginesRelation )
        throw Error ( "Bad Property type: Must be a ToolEnginesRelation" );
      return enginesRelation;
    }
  }


} // End of anonymous namespace.




namespace CRL {
    

// -------------------------------------------------------------------
// Class  :  "CRL::ToolEngine".


  ToolEngine::ToolEngine ( Cell* cell )
    : DBo()
    , _cell                     (cell)
    , _placementModificationFlag(0)
    , _routingModificationFlag  (0)
    , _inRelationDestroy        (false)
  {}


  void  ToolEngine::_postCreate ()
  {
    DBo::_postCreate();
    if ( !_cell )
        throw Error ( "Can't create " + _TName("ToolEngine") + " : empty _cell" );
    ToolEnginesRelation*  enginesRelation = ToolEnginesRelation::getToolEnginesRelation(_cell);
    if ( !enginesRelation )
      enginesRelation = ToolEnginesRelation::create ( _cell );
    else 
      forEach ( ToolEngine*, itool, enginesRelation->getSlaveOwners().getSubSet<ToolEngine*>() ) {
        if (itool->getName() == getName())
          throw Error ( "Can't create " + _TName("ToolEngine") + " : already exists !!" );
      }
    put ( enginesRelation );
    cmess1 << "  o  Creating ToolEngine<" << getName() << "> for Cell <"
           << _cell->getName() << ">" << endl;
  }


  void  ToolEngine::_preDestroy ()
  {
    ToolEnginesRelation* relation = ToolEnginesRelation::getToolEnginesRelation(_cell);
    if ( !_inRelationDestroy ) {
      if ( !relation )
        throw Error ( "Abnormal state: no ToolEnginesRelation on cell ..." );
      remove ( relation );
    }
    DBo::_preDestroy();
  }


  string  ToolEngine::_getTypeName () const
  {
    return _TName ( "ToolEngine" );
  }


  string  ToolEngine::_getString () const
  {
    string s = DBo::_getString();
    s.insert(s.length() - 1, " " + getString(getName()));
    return s;
  }


  Record* ToolEngine::_getRecord () const
  {
    Record* record = DBo::_getRecord();
    if ( record ) {
        record->add ( getSlot ( "Cell"                     , _cell                      ) );
        record->add ( getSlot ( "Name"                     , getName()                  ) );
        record->add ( getSlot ( "placementModificationFlag", _placementModificationFlag ) );
        record->add ( getSlot ( "routingModificationFlag"  , _routingModificationFlag   ) );
    }
    return record;
  }


  void  ToolEngine::grabPlacementModificationFlag ()
  {
    ToolEnginesRelation* relation = ToolEnginesRelation::getToolEnginesRelation(_cell);
    if ( !relation )
      throw Error ( "Abnormal state: no ToolEnginesRelation on cell ..." );
    _placementModificationFlag = relation->updatePlacementModificationFlag();
  }


  void  ToolEngine::getPlacementModificationFlag ()
  {
    ToolEnginesRelation* relation = ToolEnginesRelation::getToolEnginesRelation(_cell);
    if ( !relation )
      throw Error ( "Abnormal state: no ToolEnginesRelation on cell ..." );
    _placementModificationFlag = relation->getPlacementModificationFlag();
  }


  bool  ToolEngine::placementModificationFlagHasChanged ()
  {
    ToolEnginesRelation* relation = ToolEnginesRelation::getToolEnginesRelation(_cell);
    if ( !relation )
      throw Error ( "Abnormal state: no ToolEnginesRelation on cell ..." );
    return ( _placementModificationFlag != relation->getPlacementModificationFlag() );
  }


  void  ToolEngine::grabRoutingModificationFlag ()
  {
    ToolEnginesRelation* relation = ToolEnginesRelation::getToolEnginesRelation(_cell);
    if ( !relation )
      throw Error ( "Abnormal state: no ToolEnginesRelation on cell ..." );
    _routingModificationFlag = relation->updateRoutingModificationFlag ();
  }


  void  ToolEngine::getRoutingModificationFlag ()
  {
    ToolEnginesRelation* relation = ToolEnginesRelation::getToolEnginesRelation(_cell);
    if ( !relation )
      throw Error ( "Abnormal state: no ToolEnginesRelation on cell ..." );
    _routingModificationFlag = relation->getRoutingModificationFlag ();
  }


  bool  ToolEngine::routingModificationFlagHasChanged ()
  {
    ToolEnginesRelation* relation = ToolEnginesRelation::getToolEnginesRelation(_cell);
    if ( !relation )
      throw Error ( "Abnormal state: no ToolEnginesRelation on cell ..." );
    return ( _routingModificationFlag != relation->getRoutingModificationFlag() );
  }


  ToolEngines  ToolEngine::get ( const Cell* cell )
  {
    ToolEnginesRelation* relation = ToolEnginesRelation::getToolEnginesRelation(cell);

    if ( relation )
      return relation->getSlaveOwners().getSubSet<ToolEngine*>();
    else
      return ToolEngines();
  }


  ToolEngine* ToolEngine::get ( const Cell* cell, const Name& name )
  {
    ToolEnginesRelation* relation = ToolEnginesRelation::getToolEnginesRelation(cell);
    if (!relation)
      return NULL;
    else
    {
      for_each_toolengine(toolengine, relation->getSlaveOwners().getSubSet<ToolEngine*>()) {
        if (toolengine->getName() == name)
          return toolengine;
        end_for;
      }
      return NULL;
    }
  }


} // End of CRL namespace.
