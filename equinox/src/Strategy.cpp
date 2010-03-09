
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
// |                                                                 |
// |  Updater     :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Strategy.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <vector>
#include <map>
#include <set>


#include  "hurricane/Bug.h"
#include  "hurricane/Net.h"
#include  "hurricane/Name.h"
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Collection.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Box.h"
#include  "hurricane/Locator.h"
#include  "hurricane/Instance.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/Component.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/DebugSession.h"

#include  "crlcore/Utilities.h"
#include <crlcore/ToolEngine.h>

#include "equinox/Strategy.h"
#include "equinox/Equi.h"
#include "equinox/Tile.h"
#include "equinox/Tiles.h"
#include "equinox/EquinoxFilters.h"
#include "equinox/EquinoxEngine.h"

namespace {


}

namespace Equinox {


  using namespace std;
  using namespace Hurricane;



  
  Strategy::Strategy() {}


  bool const Strategy::isExtractableLayer(BasicLayer * basicLayer) 
  {
    static Layer * _gmetalh  = DataBase::getDB()->getTechnology()->getLayer("gmetalh");
    static Layer * _gmetalv  = DataBase::getDB()->getTechnology()->getLayer("gmetalv");
    static Layer * _gcontact = DataBase::getDB()->getTechnology()->getLayer("gcontact");

    if ( DataBase::getDB()->getTechnology()->isMetal(dynamic_cast<Layer *> ( basicLayer )) ) 
      if (( ( (basicLayer) != _gmetalh  ) && ( (basicLayer) != _gmetalv  )  && ( (basicLayer) != _gcontact ) )) return true;
    return false;
  }






  WithAlimStrategy::WithAlimStrategy() {}
  WithAlimStrategy::~WithAlimStrategy() {}
  
  
  
  void WithAlimStrategy::run(EquinoxEngine* equinox, unsigned nbwindows)
  {
      
      forEach(Instance*,instance, equinox->_cell->getInstances())
	{
	  Cell     * cell      = instance->getMasterCell();
	  EquinoxEngine* subequinox = EquinoxEngine::get(cell); 

	  if(!subequinox) 
	    subequinox = EquinoxEngine::create(cell);
	  if( !(subequinox->isExtracted()) ) 
	    run(subequinox, nbwindows);
	}
      
      
      equinox->scan(nbwindows);
      equinox->setIsExtracted(true);
  }  
    
  void WithAlimStrategy::getTilesFor(EquinoxEngine* equinox)
  {
    

    // Init
    //******
    ///*DEBUG*/    cmess1 << "  o  getTilesFor  :  " << endl ;
    Box  cellbox   = equinox->_cell->getBoundingBox();
    long cellwidth = cellbox.getWidth();
    long ymax      = cellbox.getYMax();
    long ymin      = cellbox.getYMin();
    long xmin      = cellbox.getXMin();
    long xmax      = cellbox.getXMax(); 
    long interval  = cellwidth/equinox->_nbWindows;


    // Cas de la premiere fenetre
    //****************************
    if( equinox->_cWindows == 0 )
	if ((equinox->_tilesByXmin->size()) + (equinox->_tilesByXmax->size())) 
	  throw "Listes non vides avant constitution des Tiles";


    // calcul de la UnderBox
    //******************
    long underbox_xmax = 0;
    if( equinox->_cWindows == equinox->_nbWindows-1 )
      underbox_xmax = xmax;
    else
      underbox_xmax = xmin + (equinox->_cWindows+1)*interval;
    
    Box underbox(xmin+equinox->_cWindows*interval, ymin, underbox_xmax, ymax);

        
    //Component Occurrence with a basiclayer on Metal at least
    Occurrences componentoccurrences = equinox->_cell->getComponentOccurrencesUnder(underbox,
										    DataBase::getDB()->getTechnology()->_getMetalMask()).getSubSet((WithAlimStrategyFilter()));
  
 
    Component * component = NULL;   
    Box    box; 
    Tile * tile = NULL;
    Equi * equi = NULL;
    bool   equicreated = false;
    Occurrence o = NULL; 

    forEach(Occurrence,occurrence,componentoccurrences)
      {	


	component   = dynamic_cast<Component*>((*occurrence).getEntity()); 
	box         = (*occurrence).getBoundingBox();
	tile        = NULL;
	equi        = NULL;
	equicreated = false;
       
	// ignorer les occurrences inutiles 
	// *****************************
	if( box.getXMin() < xmin || box.getXMin() >= xmax ) 
	  continue;

	if((*occurrence)._getSharedPath()) {
	    o = (equinox->getUpperEquiOccurrence((*occurrence))); 
	    if(!(o.isValid())) 
	     throw Error("Can't use Equinox::GetTilesFor() : GetUpperEquiOccurrence Return Invalid Occurrence");
	} else {
	  o = (*occurrence);
	}
	
	if (dynamic_cast<const BasicLayer*>(component->getLayer())) {

	  // BasicLayer
	  //************
	  tile = Tile::create((o), box, const_cast<BasicLayer*>(dynamic_cast<const BasicLayer*>(component->getLayer())), NULL);
	  equinox->_tilesByXmin->push_back(tile);
	  equinox->_tilesByXmax->push_back(tile);

	} else { 



	  // Not BasicLayer
	  //****************	

	  forEach ( BasicLayer*, i, component->getLayer()->getBasicLayers() )
	    {

	      
	      if  (isExtractableLayer(*i))
		{
		  if(!equicreated && tile){
		    equi = Equi::create(equinox);
		    tile->setEqui(equi); 
		    equi->incrementCount();
		    equi->addOccurrence((o));
		    equicreated = true;
		  } 
		  tile =  Tile::create((o), box, (*i), equi);
		  equinox->_tilesByXmin->push_back(tile); 
		  equinox->_tilesByXmax->push_back(tile);

		  
		  if(equi) 
		    equi->incrementCount();	
		}
	    }
	}
      }
    
    // Sort these two vectors 
    // *********************** 
    sort<vector<Tile*>::iterator, CompByXmin<Tile*> >( equinox->_tilesByXmin->begin(), equinox->_tilesByXmin->end(), CompByXmin<Tile*>() );
    sort<vector<Tile*>::iterator, CompByXmax<Tile*> >( equinox->_tilesByXmax->begin(), equinox->_tilesByXmax->end(), CompByXmax<Tile*>() );
    
  }
  
  
  
  
  
  
  
  
  void WithAlimStrategy::operationAfterScanLine(EquinoxEngine* equinox)
  {
    Cell * cell = equinox->_cell;
   if( cell->isLeaf())  // If this is a leaf cell
      equinox->cleanUpLeafCell();
   else 
      equinox->getOccurrencesOfEquis();
  }  
  






  WithoutAlimStrategy::WithoutAlimStrategy() {}
  WithoutAlimStrategy::~WithoutAlimStrategy() {}
  
  


  void WithoutAlimStrategy::run(EquinoxEngine* equinox, unsigned nbwindows)
  {
      
      forEach(Instance*,instance, equinox->_cell->getInstances())
	{
	  Cell     * cell      = instance->getMasterCell();
	  EquinoxEngine* subequinox = EquinoxEngine::get(cell); 

	  if(!subequinox) 
	    subequinox = EquinoxEngine::create(cell);
	  if( !(subequinox->isExtracted()) ) 
	    run(subequinox, nbwindows);
	}
      
      
      equinox->scan(nbwindows);
      equinox->setIsExtracted(true);
  }  
    
  
  void WithoutAlimStrategy::getTilesFor(EquinoxEngine* equinox)
  {
    

    // Init
    //******
    ///*DEBUG*/    cmess1 << "  o  getTilesFor  :  " << endl ;
    Box cellbox   = equinox->_cell->getBoundingBox();
    long cellwidth = cellbox.getWidth();
    long ymax = cellbox.getYMax();
    long ymin = cellbox.getYMin();
    long xmin = cellbox.getXMin();
    long xmax = cellbox.getXMax(); 
    long interval = cellwidth/equinox->_nbWindows;

    // Cas de la premiere fenetre
    //****************************
    if( equinox->_cWindows == 0 )
      {
	///*DEBUG*/    cmess1 << "              -  vidage de tilesByXmin (" << equinox->_tilesByXmin->size() << ") et tilesByXmax (" << equinox->_tilesByXmax->size() << ")" << endl;
	equinox->_tilesByXmin->clear();
	equinox->_tilesByXmax->clear();
      }

    // calcul de la box
    //******************
    long underbox_xmax = 0;
    if( equinox->_cWindows==equinox->_nbWindows-1 )
      underbox_xmax = xmax;
    else
      underbox_xmax = xmin + (equinox->_cWindows+1)*interval;
    
    Box underbox(xmin+equinox->_cWindows*interval, ymin, underbox_xmax, ymax);
    ///*DEBUG*/    cmess1 << "         -  underbox =  "<<  underbox  << endl ;
    
    

 Occurrences componentoccurrences ;
   
   if(equinox->_cell->isLeaf())
     componentoccurrences = equinox->_cell->getComponentOccurrencesUnder(underbox,
									 DataBase::getDB()->getTechnology()->_getMetalMask()).getSubSet((WithAlimStrategyFilter()));
   else 
   componentoccurrences = equinox->_cell->getComponentOccurrencesUnder(underbox,
									 DataBase::getDB()->getTechnology()->_getMetalMask()).getSubSet((WithoutAlimStrategyFilter()));

    



    Component * component = NULL;   
    Box    box; 
    Tile * tile = NULL;
    Equi * equi = NULL;
    bool   equicreated = false;
    Occurrence o = NULL; 

    
    forEach(Occurrence,occurrence,componentoccurrences)
      {	
	

	component   = dynamic_cast<Component*>((*occurrence).getEntity()); 
	box         = (*occurrence).getBoundingBox();
	tile        = NULL;
	equi        = NULL;
	equicreated = false;
	



	// ignorer les occurrences inutiles 
	// *****************************
	if( box.getXMin() < xmin || box.getXMin() >= xmax ) 
	  continue;

	if((*occurrence)._getSharedPath()){
	    o = (equinox->getUpperEquiOccurrence((*occurrence))); 
	    if(!(o.isValid())) 
	     throw Error("Can't use Equinox::GetTilesFor() : GetUpperEquiOccurrence Return Invalid Occurrence");
	} else {
	  o = (*occurrence);
	}
	//o = (*occurrence);
	
	if (dynamic_cast<const BasicLayer*>(component->getLayer())) {



	  // BasicLayer
	  //************


	  tile = Tile::create((o), box, const_cast<BasicLayer*>(dynamic_cast<const BasicLayer*>(component->getLayer())), NULL);
	  equinox->_tilesByXmin->push_back(tile);
	  equinox->_tilesByXmax->push_back(tile);
	  /////*DEBUG*/ bt++;



	} else { 



	  // Not BasicLayer
	  //****************

	  forEach ( BasicLayer*, i, component->getLayer()->getBasicLayers() )
	    {

	     
	      if  ( isExtractableLayer(*i))
		{
		  if(!equicreated && tile){
		    equi = Equi::create(equinox);
		    tile->setEqui(equi); 
		    equi->incrementCount();
		    equi->addOccurrence((o));
		    equicreated = true;
		  } 
		  tile =  Tile::create((o), box, (*i), equi);
		  equinox->_tilesByXmin->push_back(tile); 
		  equinox->_tilesByXmax->push_back(tile);

		  
		  if(equi) 
		    equi->incrementCount();	
		}

	     
	    }
	}
      }
    
      // Sort these two vectors 
       // *********************** 
       sort<vector<Tile*>::iterator, CompByXmin<Tile*> >( equinox->_tilesByXmin->begin(), equinox->_tilesByXmin->end(), CompByXmin<Tile*>() );
       sort<vector<Tile*>::iterator, CompByXmax<Tile*> >( equinox->_tilesByXmax->begin(), equinox->_tilesByXmax->end(), CompByXmax<Tile*>() );

  }
  
  
  
  
  
  
  
  
  void WithoutAlimStrategy::operationAfterScanLine(EquinoxEngine* equinox)
  {
    Cell * cell = equinox->_cell;
   if( cell->isLeaf())  // If this is a leaf cell
      equinox->cleanUpLeafCell();
   else 
      equinox->getOccurrencesOfEquis();
  }  
  




  // -------------------------------------------------------------------
  // Class  :  "Equinox::WithAlimStrategyFilter".
  
  /**/                      WithAlimStrategyFilter::WithAlimStrategyFilter   ()                               {};
  /**/                      WithAlimStrategyFilter::WithAlimStrategyFilter   (const WithAlimStrategyFilter&)  {};
  WithAlimStrategyFilter&   WithAlimStrategyFilter::operator=                (const WithAlimStrategyFilter&)  {return *this; };
  Filter<Occurrence>*       WithAlimStrategyFilter::getClone                  () const                        {return new WithAlimStrategyFilter(*this); };	   
  string                    WithAlimStrategyFilter::_getString                () const                        {return "<WithAlimStrategyFilter>"; };
  bool                      WithAlimStrategyFilter::accept                    (Occurrence item) const          
  {

    Component* component   = dynamic_cast<Component*>(item.getEntity()); 
    Box box     = component->getBoundingBox();

    // If box is not valid , this component occurrence is unuseful.
    // ************************************************************
    if( (box.getYMin() == box.getYMax()) || (box.getXMin() == box.getXMax()) ) 
      return false;

   forEach ( BasicLayer*, i, component->getLayer()->getBasicLayers() )
      {
	if   (WithAlimStrategy::isExtractableLayer(*i)) return true;
      }
    return false;
		 
    
  };








  // -------------------------------------------------------------------
  // Class  :  "Equinox::WithoutAlimStrategyFilter".
  
  /**/                      WithoutAlimStrategyFilter::WithoutAlimStrategyFilter   ()                               {};
  /**/                      WithoutAlimStrategyFilter::WithoutAlimStrategyFilter   (const WithoutAlimStrategyFilter&)  {};
  WithoutAlimStrategyFilter&   WithoutAlimStrategyFilter::operator=                (const WithoutAlimStrategyFilter&)  {return *this; };
  Filter<Occurrence>*       WithoutAlimStrategyFilter::getClone                  () const                        {return new WithoutAlimStrategyFilter(*this); };	   
  string                    WithoutAlimStrategyFilter::_getString                () const                        {return "<WithoutAlimStrategyFilter>"; };
  bool                      WithoutAlimStrategyFilter::accept                    (Occurrence item) const          
  {

    Component* component   = dynamic_cast<Component*>(item.getEntity()); 
    Box box     = component->getBoundingBox();
    Net * net = component->getNet();

    if(net->isGlobal() || net->isGround() || net->isPower()) {
      return false;
    }		
    
    // If box is not valid , this component occurrence is unuseful.
    // ************************************************************
    if( (box.getYMin() == box.getYMax()) || (box.getXMin() == box.getXMax()) ) 
      return false;

   forEach ( BasicLayer*, i, component->getLayer()->getBasicLayers() )
      {
	if (Strategy::isExtractableLayer(*i)) return true;
      }
    return false;
		 
    
  };











}// End of namespace Equinox


