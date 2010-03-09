
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
// |  C++ Header  :       "./EquinoxCollections.cpp"                 |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

/********************************************************************
*  This file contains :                                             *  
*    - See  "./equinox/EquinoxCollections.h"                        *
********************************************************************/


#include <hurricane/Collection.h>
#include <hurricane/Occurrence.h>
#include <hurricane/Occurrences.h>
#include <hurricane/Net.h>

#include <equinox/Equi.h>
#include <equinox/EquinoxFilters.h>
#include <equinox/EquinoxCollections.h>



namespace Equinox {


  AllOccurrencesCollection::AllOccurrencesCollection()
    // ***************************************************
    :   Inherit()
    , _equi(NULL)
  {
  }
  
  
  AllOccurrencesCollection::AllOccurrencesCollection(const Equi* equi)
    // ******************************************************************
   :  Inherit()
   , _equi(equi)
  {
  }  
  
  
  AllOccurrencesCollection::AllOccurrencesCollection(const AllOccurrencesCollection& occurrences)
    // ***********************************************************************************************
    :   Inherit()
    , _equi(occurrences._equi)
  {
  }
  
  
  AllOccurrencesCollection& AllOccurrencesCollection::operator=(const AllOccurrencesCollection& occurrences)
  // **********************************************************************************************************
  {
    _equi = occurrences._equi;
    return *this;
  }  
  
  
  Collection<Occurrence> * AllOccurrencesCollection::getClone() const
  // *****************************************************************
  {
    return new AllOccurrencesCollection(*this);
  }
  
  
  Locator<Occurrence>* AllOccurrencesCollection::getLocator() const
  // ***************************************************************
  {
    return new Locator(_equi);
  }
  
  
  string AllOccurrencesCollection::_getString() const
  // *************************************************
  {
    string s = "<Equi::AllOccurrences";
    if(_equi) {
      s += " " + getString(_equi);
    }
    s += ">";
    return s;
  }
  
  // **********************************************************************************************************
  //  AllOccurrencesCollection::Locator implementation
  // **********************************************************************************************************
  
  AllOccurrencesCollection::Locator::Locator()
    // ******************************************
     :    Inherit()
     , _equi(NULL)
     , _state(0)
     , _componentLocator()
     , _equioccurrenceLocator()
     , _occurrenceLocator() 
  {
  }  
  
  
  AllOccurrencesCollection::Locator::Locator(const Equi* equi)
    // **********************************************************
     :    Inherit()
     , _equi(equi)
     , _state(0)
     , _componentLocator()
     , _equioccurrenceLocator()
     , _occurrenceLocator()
  {
    if(_equi )  {   // If this is un locator valid
      _componentLocator = _equi->getNetAndComponentOccurrences().getLocator();  
      
      if(_componentLocator.isValid())
	_state = 1;
      else {
	_equioccurrenceLocator = _equi->getEquiOccurrences().getLocator();
	
	while(!_state && _equioccurrenceLocator.isValid()) {
	  Equi * equi = dynamic_cast<Equi*>(_equioccurrenceLocator.getElement().getEntity()); 
	  _occurrenceLocator = equi->getAllOccurrences().getLocator();
	  if(_occurrenceLocator.isValid())
	    _state = 2;
	  else
	    _equioccurrenceLocator.progress();
	}
      }
    }  
  }  
  
  
  AllOccurrencesCollection::Locator::Locator(const Locator& locator)
    // ****************************************************************
   :  Inherit()
   ,_equi(locator._equi)
   ,_state(locator._state)
   ,_componentLocator(locator._componentLocator)
   ,_equioccurrenceLocator(locator._equioccurrenceLocator)
   ,_occurrenceLocator(locator._occurrenceLocator)
  {
  }  
  
  
  AllOccurrencesCollection::Locator& AllOccurrencesCollection::Locator::operator=(const Locator& locator)
  // *****************************************************************************************************  
  {
    _equi = locator._equi;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
    _equioccurrenceLocator = locator._equioccurrenceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
  }
  
  
  Occurrence AllOccurrencesCollection::Locator::getElement() const
  // **************************************************************
  {
    if(_state) {  // If locator is valid
      switch(_state) {
      case 1 : return Occurrence(_componentLocator.getElement());
	
      case 2 : {
	Occurrence occurrence = _occurrenceLocator.getElement();
	
	Entity* entity = occurrence.getEntity();
	Path path = Path(_equioccurrenceLocator.getElement().getPath().getHeadInstance() , occurrence.getPath());
	
	return Occurrence(entity, path);
      }		 
      }
    }
    return Occurrence();
  }
  
  
  Locator<Occurrence> * AllOccurrencesCollection::Locator::getClone() const
  // ***********************************************************************
  {
    return new Locator(*this);
  }
  
  
  bool AllOccurrencesCollection::Locator::isValid() const
  // *****************************************************
  {
    return (_state!=0);
  }
  
  void AllOccurrencesCollection::Locator::progress() 
  // *************************************************
  {
    if(_state) {
      switch(_state) {
      case 1 :
	_componentLocator.progress();
	if(!_componentLocator.isValid()) {
	  _state = 0;
	  _equioccurrenceLocator = _equi-> getEquiOccurrences().getLocator();
	  
	  while(!_state && _equioccurrenceLocator.isValid()) {
	    Equi* equi = dynamic_cast<Equi*>(_equioccurrenceLocator.getElement().getEntity());
	    _occurrenceLocator = equi->getAllOccurrences().getLocator();
	    if(_occurrenceLocator.isValid())
	      _state = 2;
	    else
	      _equioccurrenceLocator.progress();
	  }
	}
	break;
	
      case 2 :
	_occurrenceLocator.progress();
	if(!_occurrenceLocator.isValid()) {
	  _state = 0;
	  if(_equioccurrenceLocator.isValid()) {
	    _equioccurrenceLocator.progress();
	    
	    while(!_state && _equioccurrenceLocator.isValid()) {
	      Equi* equi = dynamic_cast<Equi*>(_equioccurrenceLocator.getElement().getEntity());
	      _occurrenceLocator = equi->getAllOccurrences().getLocator();
	      if(_occurrenceLocator.isValid())
		_state = 2;
	      else
		_equioccurrenceLocator.progress();
	      
	    }
	  }
	}
	break;
      }
    } 
  }
  
  
  string AllOccurrencesCollection::Locator::_getString() const
  // **********************************************************
  {
    string s ="<Equi::AllOccurrences::Locator";
    if(_equi) {
      s += " " + getString(_equi);
    }
    s += ">";
    return s;
  }
  
  
  



  EquiComponentOccurrencesCollection::EquiComponentOccurrencesCollection()
    // ***************************************************
    :   Inherit()
    , _equi(NULL)
  {
  }
  
  
  EquiComponentOccurrencesCollection::EquiComponentOccurrencesCollection(const Equi* equi)
    // ******************************************************************
   :  Inherit()
   , _equi(equi)
  {
  }  
  
  
  EquiComponentOccurrencesCollection::EquiComponentOccurrencesCollection(const EquiComponentOccurrencesCollection& occurrences)
    // ***********************************************************************************************
    :   Inherit()
    , _equi(occurrences._equi)
  {
  }
  
  
  EquiComponentOccurrencesCollection& EquiComponentOccurrencesCollection::operator=(const EquiComponentOccurrencesCollection& occurrences)
  // **********************************************************************************************************
  {
    _equi = occurrences._equi;
    return *this;
  }  
  
  
  Collection<Occurrence> * EquiComponentOccurrencesCollection::getClone() const
  // *****************************************************************
  {
    return new EquiComponentOccurrencesCollection(*this);
  }
  
  
  Locator<Occurrence>* EquiComponentOccurrencesCollection::getLocator() const
  // ***************************************************************
  {
    return new Locator(_equi);
  }
  
  
  string EquiComponentOccurrencesCollection::_getString() const
  // *************************************************
  {
    string s = "<Equi::EquiComponentOccurrences";
    if(_equi) {
      s += " " + getString(_equi);
    }
    s += ">";
    return s;
  }
  
  // **********************************************************************************************************
  //  EquiComponentOccurrencesCollection::Locator implementation
  // **********************************************************************************************************
  
  EquiComponentOccurrencesCollection::Locator::Locator()
    // ******************************************
     :    Inherit()
     , _equi(NULL)
     , _state(0)
     , _componentLocator()
     , _equioccurrenceLocator()
     , _occurrenceLocator() 
  {
  }  
  
  
  EquiComponentOccurrencesCollection::Locator::Locator(const Equi* equi)
    // **********************************************************
     :    Inherit()
     , _equi(equi)
     , _state(0)
     , _componentLocator()
     , _equioccurrenceLocator()
     , _occurrenceLocator()
  {
    if(_equi )  {   // If this is un locator valid
      _componentLocator = _equi->getCurrentComponentOccurrences().getLocator();
      
      if(_componentLocator.isValid())
	_state = 1;
      else {
	_equioccurrenceLocator = _equi->getEquiOccurrences().getLocator();
	
	while(!_state && _equioccurrenceLocator.isValid()) {
	  Equi * equi = dynamic_cast<Equi*>(_equioccurrenceLocator.getElement().getEntity()); 
	  _occurrenceLocator = equi->getEquiComponentOccurrences().getLocator();
	  if(_occurrenceLocator.isValid())
	    _state = 2;
	  else
	    _equioccurrenceLocator.progress();
	}
      }
    }  
  }  
  
  
  EquiComponentOccurrencesCollection::Locator::Locator(const Locator& locator)
    // ****************************************************************
   :  Inherit()
   ,_equi(locator._equi)
   ,_state(locator._state)
   ,_componentLocator(locator._componentLocator)
   ,_equioccurrenceLocator(locator._equioccurrenceLocator)
   ,_occurrenceLocator(locator._occurrenceLocator)
  {
  }  
  
  
  EquiComponentOccurrencesCollection::Locator& EquiComponentOccurrencesCollection::Locator::operator=(const Locator& locator)
  // *****************************************************************************************************  
  {
    _equi = locator._equi;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
    _equioccurrenceLocator = locator._equioccurrenceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
  }
  
  
  Occurrence EquiComponentOccurrencesCollection::Locator::getElement() const
  // **************************************************************
  {
    if(_state) {  // If locator is valid
      switch(_state) {
      case 1 : return Occurrence(_componentLocator.getElement());
	
      case 2 : {
	Occurrence occurrence = _occurrenceLocator.getElement();
	
	Entity* entity = occurrence.getEntity();
	Path path = Path(_equioccurrenceLocator.getElement().getPath().getHeadInstance() , occurrence.getPath());
	
	return Occurrence(entity, path);
      }		 
      }
    }
    return Occurrence();
  }
  
  
  Locator<Occurrence> * EquiComponentOccurrencesCollection::Locator::getClone() const
  // ***********************************************************************
  {
    return new Locator(*this);
  }
  
  
  bool EquiComponentOccurrencesCollection::Locator::isValid() const
  // *****************************************************
  {
    return (_state!=0);
  }
  
  void EquiComponentOccurrencesCollection::Locator::progress() 
  // *************************************************
  {
    if(_state) {
      switch(_state) {
      case 1 :
	_componentLocator.progress();
	if(!_componentLocator.isValid()) {
	  _state = 0;
	  _equioccurrenceLocator = _equi-> getEquiOccurrences().getLocator();
	  
	  while(!_state && _equioccurrenceLocator.isValid()) {
	    Equi* equi = dynamic_cast<Equi*>(_equioccurrenceLocator.getElement().getEntity());
	    _occurrenceLocator = equi->getEquiComponentOccurrences().getLocator();
	    if(_occurrenceLocator.isValid())
	      _state = 2;
	    else
	      _equioccurrenceLocator.progress();
	  }
	}
	break;
	
      case 2 :
	_occurrenceLocator.progress();
	if(!_occurrenceLocator.isValid()) {
	  _state = 0;
	  if(_equioccurrenceLocator.isValid()) {
	    _equioccurrenceLocator.progress();
	    
	    while(!_state && _equioccurrenceLocator.isValid()) {
	      Equi* equi = dynamic_cast<Equi*>(_equioccurrenceLocator.getElement().getEntity());
	      _occurrenceLocator = equi->getEquiComponentOccurrences().getLocator();
	      if(_occurrenceLocator.isValid())
		_state = 2;
	      else
		_equioccurrenceLocator.progress();
	      
	    }
	  }
	}
	break;
      }
    } 
  }
  
  
  string EquiComponentOccurrencesCollection::Locator::_getString() const
  // **********************************************************
  {
    string s ="<Equi::EquiComponentOccurrences::Locator";
    if(_equi) {
      s += " " + getString(_equi);
    }
    s += ">";
    return s;
  }
  





  CurrentComponentOccurrencesCollection::CurrentComponentOccurrencesCollection()
    // ***************************************************
    :   Inherit()
    , _equi(NULL)
  {
  }
  
  
  CurrentComponentOccurrencesCollection::CurrentComponentOccurrencesCollection(const Equi* equi)
    // ******************************************************************
   :  Inherit()
   , _equi(equi)
  {
  }  
  
  
  CurrentComponentOccurrencesCollection::CurrentComponentOccurrencesCollection(const CurrentComponentOccurrencesCollection& occurrences)
    // ***********************************************************************************************
    :   Inherit()
    , _equi(occurrences._equi)
  {
  }
  
  
  CurrentComponentOccurrencesCollection& CurrentComponentOccurrencesCollection::operator=(const CurrentComponentOccurrencesCollection& occurrences)
  // **********************************************************************************************************
  {
    _equi = occurrences._equi;
    return *this;
  }  
  
  
  Collection<Occurrence> * CurrentComponentOccurrencesCollection::getClone() const
  // *****************************************************************
  {
    return new CurrentComponentOccurrencesCollection(*this);
  }
  
  
  Locator<Occurrence>* CurrentComponentOccurrencesCollection::getLocator() const
  // ***************************************************************
  {
    return new Locator(_equi);
  }
  
  
  string CurrentComponentOccurrencesCollection::_getString() const
  // *************************************************
  {
    string s = "<Equi::currentComponentOccurrences";
    if(_equi) {
      s += " " + getString(_equi);
    }
    s += ">";
    return s;
  }
  
  // **********************************************************************************************************
  //  CurrentComponentOccurrencesCollection::Locator implementation
  // **********************************************************************************************************
  
  CurrentComponentOccurrencesCollection::Locator::Locator()
    // ******************************************
     :    Inherit()
     , _equi(NULL)
     , _state(0)
     , _componentLocator()
     , _netoccurrenceLocator()
     , _occurrenceLocator() 
  {
  }  
  
  
  CurrentComponentOccurrencesCollection::Locator::Locator(const Equi* equi)
    // **********************************************************
     :    Inherit()
     , _equi(equi)
     , _state(0)
     , _componentLocator()
     , _netoccurrenceLocator() 
     , _occurrenceLocator()
  {
    if(_equi) { // If locator is valid
      _componentLocator = _equi->getComponentOccurrences().getLocator();
      if( _componentLocator.isValid() ) {
	_state = 1;         
      } 
      else
	{
          _netoccurrenceLocator = _equi->getNetOccurrences().getLocator();
	  while(!_state && _netoccurrenceLocator.isValid()) {
	    Net * net = dynamic_cast<Net*>(_netoccurrenceLocator.getElement().getEntity());
	     _occurrenceLocator = net->getComponents().getSubSet(NetIsUsedByExtractorFilter()).getLocator(); 
	     
	    if(_occurrenceLocator.isValid()) 
	      _state = 2;
	    else
	      _netoccurrenceLocator.progress();
	  }
	}
    }
  } 
  

  
  CurrentComponentOccurrencesCollection::Locator::Locator(const Locator& locator)
    // ****************************************************************
   :  Inherit()
   ,_equi(locator._equi)
   ,_state(locator._state)
   ,_componentLocator(locator._componentLocator)
  ,_netoccurrenceLocator(locator._netoccurrenceLocator)
   ,_occurrenceLocator(locator._occurrenceLocator)
  {
  }  
  
  
  CurrentComponentOccurrencesCollection::Locator& CurrentComponentOccurrencesCollection::Locator::operator=(const Locator& locator)
  // *****************************************************************************************************  
  {
    _equi = locator._equi;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
   _netoccurrenceLocator = locator._netoccurrenceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
  }
  
  
  Occurrence CurrentComponentOccurrencesCollection::Locator::getElement() const
  // **************************************************************
  {
    if(_state){  // If Locator is Valid
      switch(_state) {
      case 1 : return _componentLocator.getElement();
        
      case 2 : {
	return Occurrence( _occurrenceLocator.getElement(), Path() );		 
      }
      }  
    }
    return Occurrence();
  }
  
  
  Locator<Occurrence> * CurrentComponentOccurrencesCollection::Locator::getClone() const
  // ***********************************************************************
  {
    return new Locator(*this);
  }
  
  
  bool CurrentComponentOccurrencesCollection::Locator::isValid() const
  // *****************************************************
  {
    return (_state!=0);
  }
  
  void CurrentComponentOccurrencesCollection::Locator::progress() 
  // *************************************************
  {

    if(_state) { // If locator is Valid
      switch(_state) {
      case 1 : {
	_componentLocator.progress();
	if(!_componentLocator.isValid()) {
	  _state  = 0;
	  _netoccurrenceLocator = _equi->getNetOccurrences().getLocator();
	  while(!_state && _netoccurrenceLocator.isValid()) {
	    Net * net = dynamic_cast<Net*>(_netoccurrenceLocator.getElement().getEntity());         
	    
	    //_occurrenceLocator = net->GetComponents().GetLocator();
	    _occurrenceLocator = net->getComponents().getSubSet(NetIsUsedByExtractorFilter()).getLocator(); 
	    if(_occurrenceLocator.isValid())
	      _state = 2;
	    else
	      _netoccurrenceLocator.progress();
	  }
	}
	break;		  
      } 
	
      case 2 : {
	_occurrenceLocator.progress();
	if(!_occurrenceLocator.isValid()) {
	  _state = 0;
	  if (_netoccurrenceLocator.isValid()) {
	    _netoccurrenceLocator.progress();
	    
	    while(!_state && _netoccurrenceLocator.isValid()) { 
	      Net * net = dynamic_cast<Net*>(_netoccurrenceLocator.getElement().getEntity());         
	      //_occurrenceLocator = net->GetComponents().GetLocator();
	      _occurrenceLocator = net->getComponents().getSubSet(NetIsUsedByExtractorFilter()).getLocator(); 
	      
	      if(_occurrenceLocator.isValid()) 
		_state = 2;
	      else
		_netoccurrenceLocator.progress();
	    }
	  }
	}
	break;
      } 
      }
    } 
  }
  
  
  string CurrentComponentOccurrencesCollection::Locator::_getString() const
  // **********************************************************
  {
    string s ="<Equi::currentComponentOccurrences::Locator";
    if(_equi) {
      s += " " + getString(_equi);
    }
    s += ">";
    return s;
  }
  




} // End of Equinox namespace.



