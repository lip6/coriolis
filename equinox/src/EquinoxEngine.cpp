
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
// |            E q u i n o x   -   E x t r a c t o r                |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./EquinoxEngine.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>
#include  <vector>


#include  "hurricane/Path.h"
#include  "hurricane/Net.h"
#include  "hurricane/Name.h"
#include  "hurricane/Entity.h"
#include  "hurricane/Error.h"
#include  "hurricane/Collection.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Occurrences.h"
#include  "hurricane/Box.h"
#include  "hurricane/Instance.h"
#include  "hurricane/Components.h"

#include "crlcore/ToolEngine.h"
#include  "crlcore/Utilities.h"

#include "equinox/EquinoxFilters.h"
#include "equinox/Equis.h"
#include "equinox/Equi.h"
#include "equinox/Tile.h"
#include "equinox/Tiles.h"
#include "equinox/Strategy.h"
#include "equinox/UnionFind.h"
#include "equinox/EquinoxEngine.h"
#include "equinox/TileSweepLine.h"


namespace Equinox {


  using  namespace std;
  using  namespace Hurricane;
  



  // -------------------------------------------------------------------
  // Class  :  "Equinox::EquinoxEngine".

  /**/    Name                            EquinoxEngine::_toolName    = "Equinox";
  /**/    Strategy *                      EquinoxEngine::_strategy    = NULL;
 



  EquinoxEngine* EquinoxEngine::create (Cell* cell)  
  {
    EquinoxEngine* equinox = new EquinoxEngine ( cell );
    
    if(!equinox){
      throw Error("can't create Equinox : allocation failed");
    }
    
    equinox->_postCreate ();
    return equinox;
  }

  		           

  /**/    void                  EquinoxEngine::removeEquis              ()                   
  {
    set<Equi*>::iterator i;
    
    while(!_equis.empty()) {
      i = _equis.begin(); 
      (*i)->destroy();  // Delete also addresse of objet Equi from current Equinox
    } 
    _occurrences.clear(); // Clear also map<Occurrence, Equi*>
  }  


  Strategy* EquinoxEngine::getStrategy () 
  { 
    if (!_strategy) {
      _strategy =  new DefaultStrategy();
    }
    
    return _strategy;
  }


void EquinoxEngine::withAlimExtract(unsigned int nbwindows)
// **************************************************
{
  setStrategy(new WithAlimStrategy());
  /*DEBUG*/    cmess1 << "         o  extract " << endl;
  extract(nbwindows);
  /*DEBUG*/    cmess1 << "         o  printEqui " << endl;
  printEquis();
  /*DEBUG*/    cmess1 << "         o  Fin " << endl;
}  



void EquinoxEngine::withoutAlimExtract(unsigned int nbwindows)
// **************************************************
{
  setStrategy(new WithoutAlimStrategy());
  /*DEBUG*/    cmess1 << "         o  extract " << endl;
  extract(nbwindows);
  /*DEBUG*/    cmess1 << "         o  printEqui " << endl;
  printEquis();
  /*DEBUG*/    cmess1 << "         o  Fin " << endl;
}  

  void EquinoxEngine::extract(unsigned int nbwindows)
  {
    
    // Test for parameter
    // ******************
    if(!getStrategy())
      throw Error("Can't start extraction without mode, the current mode is null");
    
    if( nbwindows >256 || nbwindows < 1 ) {
      throw Error("Can't start extraction : the nbwindows " + getString(nbwindows) + " is not valid");
    }
      
    // Clean Equis
    // ***********
    if(isExtracted()) {
      flushEquis(_cell);
    }
    
    // Start extraction. 
    // *****************
    getStrategy()->run(this, nbwindows);

  }  
  

  Record* EquinoxEngine::_getRecord () const
  {
    Record* record = ToolEngine::_getRecord ();
    record->add ( getSlot           ( "_isExtracted"        ,  _isExtracted    ) ); 
    record->add ( getSlot           ( "_equis"              ,  _equis          ) ); 
    record->add ( getSlot           ( "_occurrences"        ,  _occurrences    ) );                                      
    return ( record );
  }
  

  string  EquinoxEngine::_getString () const
  {
    ostringstream  os;

    os << "<" << "Equinox " << _cell->getName () << ">";

    return ( os.str() );
  }

 string  EquinoxEngine::_getTypeName () const {  return "Equinox"; }




 Equi* EquinoxEngine::getEquiByOccurrence(Occurrence occurrence)
	      // *********************************************
	      {
                 map<Occurrence, Equi*>::iterator i = _occurrences.end();
                  if( _occurrences.find(occurrence) == i) { 
                     Component * component = dynamic_cast<Component*>(occurrence.getEntity());
               
                     if( component && occurrence.getPath().isEmpty() ) 
                     {
                      // If this is a component , maybe it has been factorized, after extraction.
                      // *************************************************************************
                          if( _occurrences.find( Occurrence(component->getNet()) ) != i )
                            return _occurrences[Occurrence(component->getNet())]; 	
                     }	
                     return NULL;
                  }   
                  else 
                     return _occurrences[occurrence];   
	      };	      



Occurrence EquinoxEngine::getEquiOccurrence(Occurrence occurrence)
	      // ***********************************************
	      {
                  map<Occurrence, Equi*>::iterator i = _occurrences.end();
                  if( _occurrences.find(occurrence) == i ) {
                     Component* component = dynamic_cast<Component*>(occurrence.getEntity());
                     if( component && occurrence.getPath().isEmpty() ) { // If this is a component, maybe it has been factorized.
                        if( _occurrences.find( Occurrence(component->getNet()) ) != i )
                           return Occurrence(static_cast<Entity*>(_occurrences[Occurrence(component->getNet())]));
                     }
                     return Occurrence(); 
                  }    
                  else 
                     return Occurrence(static_cast<Entity*>(_occurrences[occurrence]));
                };
 Occurrence EquinoxEngine::getUpperEquiOccurrence(Occurrence occurrence)
	      // *****************************************************
	      {
                 Path path = occurrence.getPath();
                 Equi * equi = NULL;
                 occurrence = Occurrence(occurrence.getEntity());
                 while(1) {
                   if(!path.isEmpty()) 
                     equi = get(path.getMasterCell())->getEquiByOccurrence(occurrence);
                   else 
                     equi = this->getEquiByOccurrence(occurrence); 

                   if(!equi)   return occurrence;
                   occurrence = Occurrence(equi, Path(path.getTailInstance()));
                   path=path.getHeadPath();
                 }	
                 return Occurrence();
	      };	  



  void EquinoxEngine::setIsExtracted (const bool flag)  
  {
    _isExtracted= flag; 
  }; 



 

  void EquinoxEngine::selectWindowsSize ()
  { 
    Box cellbox   = _cell->getBoundingBox();
    long cellwidth = cellbox.getWidth();
    if     (          cellwidth < 50000  ){ _nbWindows = 1;         } 
    else if(          cellwidth < 100000 ){ _nbWindows = 2;         } 
    else                                  { _nbWindows = cellwidth / 50000; }
    if (_nbWindows > 256) _nbWindows = 256;
    _nbWindows=20;
    ///*DEBUG*/    cmess1 << "  o  selectWindowsSize :  _nbWindows = " << _nbWindows << endl ;
    
  }

  void EquinoxEngine::initTiles  ()
  {

   getStrategy()->getTilesFor(this);

  }
  
  void EquinoxEngine::postSweepLine ()
  {
   getStrategy()->operationAfterScanLine(this);
  }

 

  void EquinoxEngine::scan (unsigned int nbwindows)
  {
    /*DEBUG*/    cmess1 << "  o EquinoxEngine::scan " << getString(_cell)  << endl;
    
    TileSweepLine* sweepLine = TileSweepLine::create(this,getStrategy()); 
    
    // Step 2 - Select Windows Size
    // ****************************
    /*DEBUG*/    cmess1 << "    - Step 2 - Select Windows Size" << endl;
    selectWindowsSize();    
    
    // Main algorithme
    // ***************
    for(_cWindows=0; _cWindows<_nbWindows; _cWindows++) {
      
      // Step 3 - Init tiles vector
      // **************************  
      /*DEBUG*/    cmess1 << "    - Step 3 - Init tiles vector " << flush;
      initTiles();
      
      // Step 4 - Run Sweep Line
      // ***********************  
      /*DEBUG*/    cmess1 << _tilesByXmin->size() <<" Tiles and SweepLine " << _cWindows << " sur " << _nbWindows << endl;
      sweepLine->run(
		     _tilesByXmin,
		     _tilesByXmax,
		     _cWindows < _nbWindows-1,
		     ((_cell->getBoundingBox().getXMin ()+(_cWindows+1)*_cell->getBoundingBox().getWidth()/_nbWindows)) );

    }  // end for
    
    // Step 5 - Post Sweep Line
    // ***********************  
      /*DEBUG*/    cmess1 << "    - Step 4 - PostSweepLine" << endl;
    postSweepLine();
    
    // Step 6 - Clean SweepLine
    // *************************  
    sweepLine->destroy();
  }
  

void EquinoxEngine::getOccurrencesOfEquis()
// *******************************************
{
  unsigned long count = 0; 
  forEach (Equi*,equi,getCollection(_equis))
     {
       // Factoriser occurrences of equi
       (*equi)->factorizeComponents();
       
       forEach(Occurrence,occurrence,((*equi)->getOccurrences()))
	 _occurrences[(*occurrence)] = *equi;
       (*equi)->setNumber(++count);		
     }   
}  


void EquinoxEngine::cleanUpLeafCell()
// *************************************
{

    ///*DEBUG*/    cmess1 << "                    -  cleanUpTerminalNetlistCell() " << endl;
   set<Net*> set_nets;
   Net * externalnet = NULL;
   unsigned long count = 1;
   Occurrence o;

   forEach(Equi*,equi,getCollection(_equis)) 
     {
       
       externalnet = NULL;
       forEach(  Occurrence,occurrence, (*equi)->getComponentOccurrences())   
	 {   
	   Net * net = dynamic_cast<Component*>((*occurrence).getEntity())->getNet();
	   
	   
	   // check for 2 externals nets on the same Equi
	   if(net->isExternal()) { 
	     if(externalnet && (externalnet != net) )
               throw Error("There is two external net in leaf cell : Library Error " + getString(externalnet)
			   + " " + getString(net) + " with component "
			   + getString((*occurrence)) + " " + getString(o));
	     
	     if(!externalnet) {
	       o = (*occurrence);
	       externalnet = net;      
	     } 
	   }	    
	   
	   set_nets.insert(net);
	 }
       
       // If there is no externalNet, choice the first
       if(!externalnet) externalnet = (*set_nets.begin());
       
     
   
       forEach(Net*,net, getCollection(set_nets))
	 {   
	   if( (*net)!= externalnet) {
	     externalnet->merge(*net);
	   }
	 }
       
       _occurrences[Occurrence(externalnet)] = *equi;  // Add <Occurrence, Equi*> to map<Occurrence, Equi*> _occurrences.
       (*equi)->erase();                               // Clean all component occurrences
       (*equi)->addOccurrence(Occurrence(externalnet));// Add net occurrence
       
       
       
       set_nets.clear(); 
       (*equi)->setNumber(count++);	
       
     }
 
}



  EquinoxEngine::EquinoxEngine ( Cell* cell )
    : ToolEngine (cell)
    , _cWindows(0)
    , _nbWindows(0)  
    , _isExtracted(false)
    , _tilesByXmin()
    , _tilesByXmax()
  { }
 
    
  

  void                EquinoxEngine::flushEquis              (Cell*cell) {
    if(!_isExtracted) // If the top cell hasn't been extracted.
      return ;
    
    InstanceLocator instancelocator = cell->getInstances().getLocator();
    
    if(instancelocator.isValid()){ // If the cell isn't a leaf cell.
      EquinoxEngine * equinox = NULL;
      Cell * mastercell = NULL;

      while(instancelocator.isValid()) {
	mastercell = instancelocator.getElement()->getMasterCell(); 
	equinox = get(mastercell);
	
	if(equinox && (equinox->isExtracted())) // If the sous-cell has been extracted.
	  flushEquis(mastercell);
	
	instancelocator.progress();
      }
    }
    
    EquinoxEngine * equinox = get(cell);
    equinox->removeEquis();
    equinox->setIsExtracted(false);
  }
  
  
  EquinoxEngine::~EquinoxEngine ()
  {	 
  }


  
  
  
  void  EquinoxEngine::_postCreate ()
  {
    ToolEngine::_postCreate ();
    _tilesByXmin = new TileVector();
    _tilesByXmax = new TileVector();
  }
  
  
 
  
  
  void  EquinoxEngine::_preDestroy ()
  {
    ToolEngine::_preDestroy ();
    removeEquis();
  }


  void  EquinoxEngine::_destroy ()
  {
    _preDestroy();
    _depthDestroy();
    delete this;
  }


void EquinoxEngine::_depthDestroy()
// **************************
{
   GenericLocator<Instance*> locator   = _cell->getInstances().getLocator();
   EquinoxEngine               * equinox = NULL;
   
   if( !(locator.isValid()) ) {
       this->destroy(); 
   }
   else {
      while(locator.isValid()) {
         Cell * cell = locator.getElement()->getMasterCell();
	 equinox = get(cell);
	 
	 if(equinox) 
            equinox->_depthDestroy();	    

	 locator.progress();
      }  
      this->destroy();
   }
}  

  

  void EquinoxEngine::printEquis()
  {
    cmess1 << "Nombre d'equis"<< _equis.size() <<endl;
/*DEBUG*///	 <<endl;
/*DEBUG*///   forEach(Equi*,equi,getCollection(_equis)) {
/*DEBUG*///     cmess1 << "     o Equi :  " << (*equi)->_getString() << endl;
/*DEBUG*///     forEach(Occurrence,occurrence,((*equi)->getOccurrences()))
/*DEBUG*///	{
/*DEBUG*///
/*DEBUG*///	  //NET Occurrence
/*DEBUG*///	  if  (dynamic_cast<Net*>((*occurrence).getEntity())) 
/*DEBUG*///	    {
/*DEBUG*///	      cmess1 << "       -  [NET]" << (*occurrence).getEntity() << " \t===>\t "  << getHyperNetRootNetOccurrence((*occurrence).getEntity())._getString() << endl;
/*DEBUG*///	    } 
/*DEBUG*///
/*DEBUG*///	  //COMPONENT Occurrence
/*DEBUG*///	  if  (dynamic_cast<Component*>((*occurrence).getEntity())) {
/*DEBUG*///	    Component* composant = dynamic_cast<Component*>((*occurrence).getEntity());
/*DEBUG*///	    cmess1 << "       -  [COM]" << composant << " \t===>\t "  << getHyperNetRootNetOccurrence(composant->getNet())._getString() << endl;
/*DEBUG*///	  }	  
/*DEBUG*///	  
/*DEBUG*///	  //EQUI Occurrence
/*DEBUG*///	  if  (dynamic_cast<Equi*>((*occurrence).getEntity())) {
/*DEBUG*///	    Equi* equi = dynamic_cast<Equi*>((*occurrence).getEntity());
/*DEBUG*///	    cmess1 << "       -  [EQU]" << equi << endl;
/*DEBUG*///	  }
/*DEBUG*///	}
/*DEBUG*///   }
      }
  
  void EquinoxEngine::removeInterval(Tile* item)
  {
    
    // on diminue le compteur de reference de l'Equi
    item->getEqui()->decrementCount();
    
    
    
    // on detruit le tile
    item->destroy();
    
  }
 
  void EquinoxEngine::insertInterval(Tile* newtile,stack <Interval*>*enumResultStack)
  {
      //Application des contacts 
    Equi * newroot = NULL;
    if( !(enumResultStack->empty()) ) {  // contact > 0
      if( newtile->getEqui() ) enumResultStack->push(newtile);
      newroot = UnionFind::getRootEquiWithCompression(dynamic_cast<Tile*>(enumResultStack->top()) );
      enumResultStack->pop();
      while (!enumResultStack->empty()) {//while contact > 0
	Equi * root = UnionFind::getRootEquiWithCompression(dynamic_cast<Tile*>(enumResultStack->top()) );
	enumResultStack->pop();
	if( root != newroot ) {
	  root->setNextEqui(newroot);
	  newroot->merge(root);
	  newroot->incrementCount();
	}
      }
      if(newtile->getEqui() == NULL){ 
	newtile->setEqui(newroot);
	newroot->incrementCount();
	newroot->addOccurrence(newtile->getOccurrence());
      }	 
    } 
    else {                              // contact == 0
      if( newtile->getEqui() == NULL) {
	newroot = Equi::create(this);
	newroot->incrementCount();
	newtile->setEqui(newroot);
	newroot->addOccurrence(newtile->getOccurrence());
      } 
    }

  }



}// End of namespace Equinox
