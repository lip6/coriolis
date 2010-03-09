
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
// |  C++ Header  :       "./Equi.cpp"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <string>

using namespace std;


#include  "hurricane/Box.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Component.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/Collection.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Entity.h"
#include  "hurricane/Filter.h"
#include  "hurricane/Net.h"
#include  "hurricane/Commons.h"
#include  "hurricane/Error.h"
#include  "hurricane/Relation.h"
#include  "hurricane/Locator.h"

#include  "crlcore/Utilities.h"
#include  "crlcore/ToolEngine.h"

#include "equinox/Tile.h"
#include "equinox/Equi.h"
#include "equinox/Strategy.h"
#include "equinox/EquinoxEngine.h"
#include "equinox/EquinoxFilters.h"
#include "equinox/EquinoxCollections.h"


namespace Equinox {

  using namespace Hurricane;


// -------------------------------------------------------------------
// Class  :  "Equinox::Equi".

  map<Net*, unsigned long>         Equi::_map_net2nb_usefulcomponent;
  


  void Equi::_preDestroy()
  {
    if(_nextequi) _nextequi->decrementCount();	
    _equinox->removeEqui(this);
    Entity::_preDestroy(); 
  }
  
  void Equi::factorizeComponents() 
  {

    // get all components to netmap (net2Component)
    map<Net*, set<Occurrence> > netmap;
    forEach(Occurrence,occurrence,getComponentOccurrences()) {
      netmap[( dynamic_cast<Component*>((*occurrence).getEntity()))->getNet()].insert((*occurrence));
    }
    
    for ( map<Net*, set<Occurrence> >::iterator i = netmap.begin();
	  i != netmap.end(); 
	  i++ )
      {
	Net *       net       = (*i).first;
	
	
	//process nb_component and map_net2nb_usefulcomponent[net]
	unsigned long nb_component = 0;
	map<Net*, unsigned long>::iterator end = _map_net2nb_usefulcomponent.end();
	if(_map_net2nb_usefulcomponent.find(net)==end) {    /* haven't been calculed */
	  nb_component = net->getComponents().getSubSet(IsUsedByExtractFilter()).getSize();
	  _map_net2nb_usefulcomponent[net] = nb_component; 
	}	
	else {
	  nb_component = _map_net2nb_usefulcomponent[net]  ; 
	} 
	
	if( (*i).second.size() == nb_component ){
	  forEach (Occurrence,occurrence,getCollection((*i).second)) _occurrences.erase((*occurrence));
	  _occurrences.insert(Occurrence(net));
	} 	
      }
  }
  
  
  void Equi::_postCreate()  
  {
    Entity::_postCreate();
    _equinox->addEqui(this);
  }  
  
  
  string Equi::_getString() const
  {
    return "<Equi " +  getString(_number)  + " - " + getString(_occurrences.size()) + " occurences>";
  }


  Equi::Equi(EquinoxEngine* equinox, Equi* nextequi, const unsigned long& number)
    : Entity()
    , _equinox(equinox)
    , _occurrences()
    , _nextequi(nextequi)
    , _count(0)
    , _number(number)
  {
    if(!equinox){
      throw Error("Can't create Equi: equinox is null");   
    }
  }
  
  
  
  
  Equi * Equi::create(EquinoxEngine* equinox, Equi* nextequi, const unsigned long& number)
  {
    Equi * equi = new Equi(equinox, nextequi, number);
    if(!equi) {
      throw Error("can't create Equi : allocation error");
    }
    equi->_postCreate();
    return equi;
  }
  
  

  Cell* Equi::getCell() const
  {
    return _equinox->getCell();
  }


  
  // filters

  GenericFilter<Occurrence> Equi::getIsEquiFilter                       (){ return IsEquiFilter();}  
  GenericFilter<Occurrence> Equi::getIsComponentFilter                  (){ return IsComponentFilter();}
  GenericFilter<Occurrence> Equi::getIsNetFilter                        (){ return IsNetFilter();}
  GenericFilter<Occurrence> Equi::getIsLeafEquiFilter                   (){ return IsLeafEquiFilter(); }
  GenericFilter<Occurrence> Equi::getIsNotLeafEquiFilter                (){ return !IsLeafEquiFilter(); }
  //  GenericFilter<Component*> Equi::getIsUsedByExtractFilter              (){ return IsUsedByExtractFilter();}
  GenericFilter<Occurrence> Equi::getIsNetOrComponentFilter             (){ return IsNetOrComponentFilter();}
 							                
  Occurrences               Equi::getAllOccurrences                     () const { return AllOccurrencesCollection(this);} 

  Occurrences               Equi::getEquiOccurrences                    () const { return getOccurrences().getSubSet(IsEquiFilter());};
  Occurrences               Equi::getComponentOccurrences               () const { return getOccurrences().getSubSet(IsComponentFilter());};
  Occurrences               Equi::getNetOccurrences                     () const { return getOccurrences().getSubSet(IsNetFilter());};
  Occurrences               Equi::getNetAndComponentOccurrences         () const { return getOccurrences().getSubSet(IsNetOrComponentFilter());};

  Occurrences               Equi::getEquiComponentOccurrences           () const { return EquiComponentOccurrencesCollection(this);};
  Occurrences               Equi::getCurrentComponentOccurrences        () const { return CurrentComponentOccurrencesCollection(this); };
  		            
  bool                      Equi::isLeafEqui                            () const { GenericLocator<Occurrence> l = getEquiOccurrences().getLocator(); return !(l.isValid());};
  bool                      Equi::hasNetOccurrence                      () const { GenericLocator<Occurrence> l = getNetOccurrences().getLocator();  return (l.isValid());};
  bool                      Equi::hasComponentOccurrence                () const { GenericLocator<Occurrence> l = getComponentOccurrences().getLocator();  return (l.isValid());};  
  
}// End of namespace Equinox
