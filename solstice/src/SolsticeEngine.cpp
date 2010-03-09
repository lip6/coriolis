// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./SolsticeEngine.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x
#include <hurricane/DataBase.h>
#include <hurricane/Cell.h>
#include <hurricane/Technology.h>

#include <crlcore/Utilities.h>
#include <crlcore/ToolEngine.h>

#include <equinox/Equi.h>
#include <equinox/Strategy.h>
#include <equinox/IntervalTree.h>
#include <equinox/Tiles.h>
#include <equinox/EquinoxEngine.h>


#include <solstice/Brick.h>
#include <solstice/SolsticeEngine.h>
#include <solstice/BrickSweepLine.h>
#include <solstice/RoutingError.h>
#include <solstice/ShortCircuitError.h>
#include <solstice/DisconnectError.h>

namespace Solstice {
  
  
  using  namespace std;
  using  namespace Hurricane;
  using  Equinox::DefaultStrategy;
  using  Equinox::EquinoxEngine;
  using  Equinox::Equi;
  using  Equinox::CompByXmin;
  using  Equinox::CompByXmax;
  
  // -------------------------------------------------------------------
  // Class  :  "Solstice::SolsticeEngine".
  
  Name         SolsticeEngine::_toolName    = "Solstice";
  Strategy *   SolsticeEngine::_strategy    = NULL;
  
  
  
  void SolsticeEngine::setStrategy (Strategy * s)
  {
    if (_strategy)
      delete _strategy;
    _strategy = s;
  };
  
  Strategy* SolsticeEngine::getStrategy () 
  { 
    if (!_strategy) {
      _strategy =  new DefaultStrategy();
    }
    return _strategy;
  }
  
  
  Cell* SolsticeEngine::getCommonPath(Path path1, Path path2, Path& newpath1, Path& newpath2)
  {
    Instance * instance1 = path1.getHeadInstance();
    Instance * instance2 = path2.getHeadInstance();
    Path commonpath;
    
    while(instance1==instance2)
      {
	if(!instance1) break;
	
	path1 = path1.getTailPath(); 
	path2 = path2.getTailPath(); 
	
	commonpath = commonpath.isEmpty()?Path(instance1):Path(commonpath, instance1);       
	
	if(path1.isEmpty()) break;
	if(path2.isEmpty()) break;
	
	instance1 = path1.getHeadInstance();
	instance2 = path2.getHeadInstance();
      } 
    
    newpath1 = path1;
    newpath2 = path2;
    
    if (commonpath.isEmpty())
      {
	return NULL; 
      }
    else
      { 
	return commonpath.getMasterCell();
      }
  }  
  
  SolsticeEngine* SolsticeEngine::create (Cell* cell)  
  {
    SolsticeEngine* solstice = new SolsticeEngine ( cell );
    
    if(!solstice){
      throw Error("can't create Solstice : allocation failed");
    }
    
    solstice->_postCreate ();
    return solstice;
  }
  
  
  void  SolsticeEngine::_depthCreate(Cell * cell)
  {
    forEach(Instance*,instance, cell->getInstances())
      {
	Cell * subcell = (*instance)->getMasterCell();
	SolsticeEngine * solstice = get(subcell);
	if( (!(subcell->isLeaf())) && (!solstice) ) {
	  _depthCreate(subcell); 
	}
      }
    SolsticeEngine * solstice = get(cell);
    if(!solstice)
      create(cell);
  }  
  
  
  SolsticeEngine* SolsticeEngine::get (const Cell* cell ) 
  { 
    return static_cast<SolsticeEngine*>(ToolEngine::get(cell,_toolName)); 
  }


  Record* SolsticeEngine::_getRecord () const
  {
    Record* record = ToolEngine::_getRecord ();                                    
    return ( record );
  }
  
  
  SolsticeEngine::~SolsticeEngine ()
  { 
  }
  
  SolsticeEngine::SolsticeEngine (Cell* cell):
    ToolEngine(cell),
    _isCompared(false),
    _routingErrors()
  { 
    _routingErrors = new  set<RoutingError*>;
  }


  
  
  

  
  

  
  void SolsticeEngine::_depthDestroy()
  {
    if(_cell->isLeaf())
      return ;
    else {
      forEach(Instance*,instance, _cell->getInstances())
	{
	  Cell * cell = (*instance)->getMasterCell();
	  SolsticeEngine * solstice = get(cell);
	  if(solstice) solstice->_depthDestroy();
	}
      this->destroy();
    }
  }
  
  
  
  void SolsticeEngine::compare()
  {  
    //  Verify the extractor Equinox
    // ******************************  
    EquinoxEngine * equinox = EquinoxEngine::get(_cell);
    if(!equinox){
      throw Error("Can't do comparison, because the extractor haven't been created");
    }
    
    if(!(equinox->isExtracted())) {
      throw Error("Can't do comparison, because the extraction haven't been done");
    }
    
    //  Create CEngine Solstice for all models except standards models.
    // *****************************************************************
    _depthCreate(_cell);    
    
    //  If repeat Comparison, flush those olds errors and reset flags to false.  
    // *************************************************************************
    if(isCompared()) flushErrors();
    
    //  Run comparison and Print Statistique datas.
    // *********************************************
    runComparison();
    
  }  
  
  
  
  
  
  void SolsticeEngine::runComparison()
  {
    map<Occurrence, set<Equi*> > map_hypernet2hyperequi;
    set<Occurrence>              hypernets;
    EquinoxEngine *              equinox = Equinox::EquinoxEngine::get(_cell);
    
    forEach(Equi*,equi, equinox->getRoutingEquis())
      {
	forEach(Occurrence,occurrence, (*equi)->getAllOccurrences())
	  { 

	    // Check Net Power/Ground/Global
	    //*******************************
	    Net * net = dynamic_cast<Net*>((*occurrence).getEntity());
	    if(!net) 
	      net = dynamic_cast<Component*>((*occurrence).getEntity())->getNet();
	    
	     if(net->isGlobal() || net->isGround() || net->isPower()) {
	      continue;
	    } 

	    
	    // Add HyperNet
	    //**************
	    Occurrence hypernet = getTopNetOccurrence((*occurrence));
#ifdef ASSERT
	    assert(isHyperNetRootNetOccurrence(hypernet));
#endif
	    hypernets.insert(hypernet);
	    map_hypernet2hyperequi[hypernet].insert((*equi));
	    
	  } //end of forEach occurrence
	
	
	// ShortCircuits Detection
	//*************************
	if(hypernets.size() > 1) {
	  cmess1 << "[BUG] ShortCircuit Detection with " << hypernets.size() << " nets on same Equi" << endl;
	  for (set<Occurrence>::iterator i = hypernets.begin() ; i!= hypernets.end();i++)
	    cmess1 << "  - Net " << (*i).getEntity()->_getString() << endl;
	  detectShortCircuit((*equi));
	}
	
	hypernets.clear(); 
 
      } //end of forEach equi	
    

    // Disconnects Detection
    //***********************
    for (map<Occurrence, set<Equi*> >::iterator i = map_hypernet2hyperequi.begin();
	 i != map_hypernet2hyperequi.end();
	 i++)
      {
	if( (*i).second.size()>1 )  {
	  cmess1 << "[BUG] Disconnection Detection with " << (*i).second.size() << " equi for net " << (*i).first.getEntity()->_getString() << endl;

	  for (set<Equi*>::iterator j = (*i).second.begin();
	       j != (*i).second.end();
	       j++)
	    {
	      cmess1 << "   - " << (*j) << endl ;
	      forEach(Occurrence,k,(*j)->getAllOccurrences()) {
		cmess1 << "        - " << (*k) << endl ;
	      }
	    }

	  this->_routingErrors->insert(DisconnectError::create(getCell(),Occurrence((*i).first),(*i).second.size()));
	
	}
      }
    
    map_hypernet2hyperequi.clear();
    setIsComparedTrue(_cell);
  }
  
  
  
  
  Occurrence SolsticeEngine::getTopNetOccurrence(Occurrence occurrence)
  {
    Path path = occurrence.getPath();
    
    if( path.isEmpty() ) {
      Net * net = dynamic_cast<Net*>(occurrence.getEntity());
      if(net) {
	return occurrence;   
      }  
      else {
	Component * component = dynamic_cast<Component*>(occurrence.getEntity());
	if(!component)  
	  throw Error("Unknow occurrence in EQUINOX::GetUpperNetOccurrence()");
	return Occurrence(component->getNet());   
      }
    }
    else {
      Net * net = dynamic_cast<Net*>(occurrence.getEntity());
      
      if(net) {
	if( !(net->isExternal()) )
          return occurrence;
        Instance * tailinstance = path.getTailInstance();
	Plug * plug = tailinstance->getPlug(net);
	
        if( !(plug->isConnected()) )
	  return occurrence;

	
	Net * uppernet = plug->getNet();
	return getTopNetOccurrence(Occurrence(uppernet, path.getHeadPath()));
      }
      else {
	Component* component = dynamic_cast<Component*>(occurrence.getEntity()); 
	return getTopNetOccurrence(Occurrence(component->getNet(), occurrence.getPath()));
      }             
    }
  }
  
  
  
  void SolsticeEngine::detectShortCircuit(Equi* equi)
  {
  
    // Create Bricks for all component occurrences in this hyper-equi.
    // ***************************************************************
    vector<Brick*>* BricksByXmin = new vector<Brick*> ; 
    vector<Brick*>* BricksByXmax = new vector<Brick*> ;  
    
    forEach(Occurrence,occurrence, equi->getEquiComponentOccurrences()) {
      Occurrence hypernet = getTopNetOccurrence((*occurrence));
      Component* component = dynamic_cast<Component*>((*occurrence).getEntity());
      
      Box box = (*occurrence).getBoundingBox();
      
      forEach ( BasicLayer*, i, component->getLayer()->getBasicLayers() )
	if (Strategy::isExtractableLayer(*i))
	  {
	    
	    Brick * brick = Brick::create(hypernet, (*occurrence), box, *i);
	    BricksByXmin->push_back(brick);
	    BricksByXmax->push_back(brick); 
	  }
    }
    
    
    
    // Sort these two tables.
    // *********************    
    sort< vector<Brick*>::iterator , CompByXmin<Brick*> > ( BricksByXmin->begin(), BricksByXmin->end(), CompByXmin<Brick*>() );
    sort< vector<Brick*>::iterator , CompByXmax<Brick*> > ( (BricksByXmax->begin()), BricksByXmax->end(), CompByXmax<Brick*>() );
    
    
    BrickSweepLine* sweepLine = BrickSweepLine::create(this,getStrategy()); 
    
    sweepLine->run(BricksByXmin,BricksByXmax,false,0);
    
  }
  
  
  
  
  
  
  void SolsticeEngine::setIsComparedTrue(Cell * cell)
  {
    SolsticeEngine * solstice = get(cell);
    solstice->setIsCompared(true);
    
    forEach(Instance*,instance, cell->getInstances())
      {
	Cell * subcell = (*instance)->getMasterCell();
	SolsticeEngine * solstice = get(subcell);
	if( (!(subcell->isLeaf())) && (!(solstice->isCompared())) ) 
	  setIsComparedTrue(subcell); 
      }
  }  
  
  
  void SolsticeEngine::getAllSolstices(Cell* cell, set<SolsticeEngine*>& solstices)
  {
    SolsticeEngine* solstice = get(cell);
    solstices.insert(solstice);
    forEach(Instance*,instance, cell->getInstances())
      {
	Cell * subcell = (*instance)->getMasterCell();
	if( (!(subcell->isLeaf())) )
	  getAllSolstices(subcell, solstices);
      }
  }  
  
  

  void SolsticeEngine::insertInterval ( Brick*brick ,stack <Equinox::Interval*>* enumResultStack) 
  {
    while( !(enumResultStack->empty()) ) { 
      Brick* findedbrick = dynamic_cast<Brick*>(enumResultStack->top());
      enumResultStack->pop();
      if(findedbrick->getHyperNet()!=brick->getHyperNet()) { // Shorts-Circuits
	Occurrence occurrence1 = brick->getComponentOccurrence();
	Occurrence occurrence2 = findedbrick->getComponentOccurrence();
	Path newpath1;
	Path newpath2;
	
	Cell * errorcell = getCommonPath(occurrence1.getPath(), occurrence2.getPath(), newpath1, newpath2);
	SolsticeEngine * solstice = NULL;
	
	if(errorcell) {
	  solstice = get(errorcell);
	  
	  if(!solstice) {
	    cout << getString(errorcell) <<endl;
	    throw Error("Can't get CEngine solstice in function SolsticeEngine::insertInterval");
	  }
	}
	else  {  // If errorcell is Null, the top-model is current model. 
	  solstice = this;
	  errorcell = _cell;
	}
	
	ShortCircuitError* error = ShortCircuitError::create(
							  errorcell, 
							  Occurrence(occurrence1.getEntity(), newpath1),
							  Occurrence(occurrence2.getEntity(), newpath2) 
							  );
	
	solstice->_routingErrors->insert(error);
      }
    }
  }
  void SolsticeEngine::removeInterval (Brick* item)
  {
    item->destroy();
  }
  
  
  void SolsticeEngine::clearErrors() {
    for(set<RoutingError*>::iterator e =  _routingErrors->begin();
	e !=  _routingErrors->end() ;
	e++)
      (*e)->destroy();
    _routingErrors->clear();
    
  };
  
  void SolsticeEngine::flushErrors()
  {
    forEach(Instance*,instance, _cell->getInstances())
      {
	Cell * cell = (*instance)->getMasterCell();
	SolsticeEngine * solstice = get(cell);
	if( (!(cell->isLeaf())) && (solstice->isCompared()) ) {
	  solstice->flushErrors(); 
	}
      }	
    
    clearErrors();   
    setIsCompared(false); 
  }  
  
  
  
  
}// End of namespace Solstice
