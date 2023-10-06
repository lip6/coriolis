// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ToolEngine.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/Commons.h"
#include "hurricane/Error.h"
#include "hurricane/Cell.h"
#include "hurricane/Relation.h"
#include "crlcore/Utilities.h"
#include "crlcore/ToolEngine.h"


namespace {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::string;
  using std::ostringstream;
  using std::set;
  using std::vector;
  using Hurricane::ForEachIterator;
  using Hurricane::_TName;
  using Hurricane::Error;
  using Hurricane::Name;
  using Hurricane::Relation;
  using Hurricane::Record;
  using Hurricane::Cell;
  using CRL::ToolEngine;
    

  const Name  ToolEnginesRelationName   = "ToolEnginesRelationName";
    

// -------------------------------------------------------------------
// Class  :  "CRL::ToolEnginesRelation".

  
  class ToolEnginesRelation : public Relation {
    public:
    // Static Methods.
      static ToolEnginesRelation* getToolEnginesRelation          ( const Cell* cell );
      static void                 destroyAllToolEnginesRelations  ();
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
    // Internal: Constructor.
                                  ToolEnginesRelation             ( Cell* masterOwner );
    protected:
      virtual void                _preDestroy                     ();

    private:
    // Internal: Attributes
      static  set<ToolEnginesRelation*>  _toolEnginesRelations;
              unsigned int               _placementModificationFlag;
              unsigned int               _routingModificationFlag;
  };


  set<ToolEnginesRelation*>  ToolEnginesRelation::_toolEnginesRelations;


  ToolEnginesRelation::ToolEnginesRelation ( Cell* masterOwner )
    : Relation                  (masterOwner)
    , _placementModificationFlag(0)
    , _routingModificationFlag  (0)
  { }


  ToolEnginesRelation* ToolEnginesRelation::create ( Cell* masterOwner )
  {
    ToolEnginesRelation* enginesRelation = new ToolEnginesRelation(masterOwner);

    enginesRelation->_postCreate();
    _toolEnginesRelations.insert ( enginesRelation );

    return enginesRelation;
  }


  void  ToolEnginesRelation::_preDestroy ()
  {
    ToolEngine* tool;
    while ( (tool = getSlaveOwners().getSubSet<ToolEngine*>().getFirst()) ) {
      tool->setInRelationDestroy( true );
      tool->destroy();
    }
    Relation::_preDestroy();
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


  void  ToolEnginesRelation::destroyAllToolEnginesRelations ()
  {
    set<ToolEnginesRelation*>::iterator irelation = _toolEnginesRelations.begin();
    for ( ; irelation != _toolEnginesRelations.end() ; ++irelation ) {
      vector<ToolEngine*> tools;
      forEach ( ToolEngine*, itool, (*irelation)->getSlaveOwners().getSubSet<ToolEngine*>() ) {
        tools.push_back( *itool );
      }

      for ( size_t i=0 ; i<tools.size() ; ++i )
        tools[i]->destroy();
    }
    _toolEnginesRelations.clear();
  }


} // End of anonymous namespace.




namespace CRL {
    

// -------------------------------------------------------------------
// Class  :  "CRL::ToolEngine".


  bool  ToolEngine::_inDestroyAll = false;


  ToolEngine::ToolEngine ( Cell* cell, bool verbose )
    : Super()
    , _cell                     (cell)
    , _verbose                  (verbose)
    , _placementModificationFlag(0)
    , _routingModificationFlag  (0)
    , _inRelationDestroy        (false)
    , _timer                    ()
    , _passNumber               (0)
  { }


  void  ToolEngine::_postCreate ()
  {
    Super::_postCreate();
    if (not _cell)
        throw Error( "Can't create " + _TName("ToolEngine") + " : empty _cell" );

    ToolEnginesRelation*  enginesRelation = ToolEnginesRelation::getToolEnginesRelation(_cell);
    if (not enginesRelation)
      enginesRelation = ToolEnginesRelation::create( _cell );
    else 
      for ( ToolEngine* tool : enginesRelation->getSlaveOwners().getSubSet<ToolEngine*>() ) {
        if (tool->getName() == getName())
          throw Error( "Can't create " + _TName("ToolEngine") + " : already exists !!" );
      }

    put( enginesRelation );

    if (_verbose) {
      cmess1 << "  o  Creating ToolEngine<" << getName() << "> for Cell <"
             << _cell->getName() << ">" << endl;
      
      cmess1 << Dots::asString( "     - Initial memory"
                              , Timer::getStringMemory(Timer::getMemorySize()) ) << endl;
    }
  }


  void  ToolEngine::_preDestroy ()
  {
    ToolEnginesRelation* relation = ToolEnginesRelation::getToolEnginesRelation( _cell );
    if (not _inRelationDestroy) {
      if (not relation)
        throw Error( "Abnormal state: no ToolEnginesRelation on %s", getString(_cell).c_str() );
      remove( relation );
    }
    Super::_preDestroy();
    _cell->notify( Cell::Flags::CellChanged );
  }


  bool  ToolEngine::inDestroyAll ()
  { return _inDestroyAll; }


  void  ToolEngine::destroyAll ()
  {
    _inDestroyAll = true;
    ToolEnginesRelation::destroyAllToolEnginesRelations();
    _inDestroyAll = false;
  }


  string  ToolEngine::_getTypeName () const
  { return _TName ( "ToolEngine" ); }


  string  ToolEngine::_getString () const
  {
    string s = Super::_getString();
    s.insert(s.length() - 1, " " + getString(_cell->getName()));
    return s;
  }


  Record* ToolEngine::_getRecord () const
  {
    Record* record = Super::_getRecord();
    if ( record ) {
        record->add ( getSlot ( "_cell"                     , _cell                      ) );
      //record->add ( getSlot ( "Name"                      , getName()                  ) );
        record->add ( getSlot ( "_placementModificationFlag", _placementModificationFlag ) );
        record->add ( getSlot ( "_routingModificationFlag"  , _routingModificationFlag   ) );
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
    if (not relation) {
      return NULL; 
    } else {
      forEach ( ToolEngine*, itool, relation->getSlaveOwners().getSubSet<ToolEngine*>()) {
        if (itool->getName() == name)
          return *itool;
      }
      return NULL;
    }
  }


  void  ToolEngine::startMeasures ()
  {
    _timer.resetIncrease();
    _timer.start();
  }


  void  ToolEngine::stopMeasures ()
  { _timer.stop(); }


  void  ToolEngine::suspendMeasures ()
  { _timer.suspend(); }


  void  ToolEngine::resumeMeasures ()
  { _timer.resume(); }


  void  ToolEngine::printMeasures () const
  {
    ostringstream result;

    result <<  Timer::getStringTime(_timer.getCombTime()) 
           << ", " << Timer::getStringMemory(_timer.getIncrease());
    cmess1 << Dots::asString( "     - Done in", result.str() ) << endl;

    result.str("");
    result << _timer.getCombTime()
           << "s, +" << (_timer.getIncrease()>>10) <<  "Kb/"
           <<  Timer::getStringMemory(Timer::getMemorySize());
    cmess2 << Dots::asString( "     - Raw measurements", result.str() ) << endl;

    // result.str("");
    // result << Timer::getStringMemory(Timer::getMemorySize());
    // cmess1 << Dots::asString( "     - Total memory", result.str() ) << endl;
  }


} // End of CRL namespace.
