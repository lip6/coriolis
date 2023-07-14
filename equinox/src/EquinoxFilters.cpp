
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
// |  C++ Header  :       "./EquinoxFilters.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/Components.h"

#include "hurricane/Component.h"
#include "hurricane/Filter.h"
#include "hurricane/Collection.h"
#include "hurricane/Occurrence.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Net.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Cell.h"
#include "equinox/EquinoxFilters.h"
#include "equinox/Equi.h"
#include "equinox/Strategy.h"


namespace Equinox {
  
  using Hurricane::GenericCollection;
  using Hurricane::GenericLocator;
  using Hurricane::GenericFilter;
  using Hurricane::Filter;
  using Hurricane::Occurrence;
  using Hurricane::Component;
  using Hurricane::Net;


  using namespace Hurricane;







  // -------------------------------------------------------------------
  // Class  :  "Equinox::IsRoutingEquiFilter".
  
  /**/                 IsRoutingFilter::IsRoutingFilter   ()                           {};
  /**/                 IsRoutingFilter::IsRoutingFilter   (const IsRoutingFilter&)     {};
  IsRoutingFilter&     IsRoutingFilter::operator=         (const IsRoutingFilter&)     {return *this; };
  Filter<Equi*>*       IsRoutingFilter::getClone          () const                     {return new IsRoutingFilter(*this); };	   
  string               IsRoutingFilter::_getString        () const                     {return "<IsRoutingFilter>"; };
  bool                 IsRoutingFilter::accept            (Equi* item) const           
  {
    if(item->isLeafEqui()) {
      if( item->getCell()->isTerminalNetlist() ) {   
	OccurrenceLocator locator = item->getNetOccurrences().getLocator();
	Net * net = dynamic_cast<Net*>(locator.getElement().getEntity());
	if(!(net->isExternal()))  
	  return false;
      }
    }
    else 
      {
	if( !(item->hasNetOccurrence() || item->hasComponentOccurrence()) )
	  if( item->getEquiOccurrences().getSize() == 1 ) {
	    OccurrenceLocator locator = item->getEquiOccurrences().getLocator();
	    Equi * subequi = dynamic_cast<Equi*>(locator.getElement().getEntity());
	    return accept(subequi);
	  }
      }
    
    return true;
  };	
 

  // -------------------------------------------------------------------
  // Class  :  "Equinox::IsUsedByExtractFilter".
  
  /**/                      IsUsedByExtractFilter::IsUsedByExtractFilter   ()                               {};
  /**/                      IsUsedByExtractFilter::IsUsedByExtractFilter   (const IsUsedByExtractFilter&) {};
  IsUsedByExtractFilter&  IsUsedByExtractFilter::operator=                 (const IsUsedByExtractFilter&) {return *this; };
  Filter<Component*>*       IsUsedByExtractFilter::getClone                  () const                         {return new IsUsedByExtractFilter(*this); };	   
  string                    IsUsedByExtractFilter::_getString                () const                         {return "<IsUsedByExtractFilter>"; };

  bool                      IsUsedByExtractFilter::accept                    (Component* item) const          
  {


    Box box = item->getBoundingBox();

    if( (box.getYMin() == box.getYMax()) || (box.getXMin() == box.getXMax()) ) 
      return false;

    forEach ( BasicLayer*, i, item->getLayer()->getBasicLayers() )
      {
	if (  Strategy::isExtractableLayer(*i)) return true;
      }
    return false;

  };










  


  /**/                      IsEquiFilter::IsEquiFilter   ()                       {};
  /**/                      IsEquiFilter::IsEquiFilter   (const IsEquiFilter&)    {};
  IsEquiFilter&             IsEquiFilter::operator=      (const IsEquiFilter&)    {return *this; };
  Filter<Occurrence>*      IsEquiFilter::getClone       () const                 {return new IsEquiFilter(*this); };	   
  string                    IsEquiFilter::_getString     () const                 {return "<IsEquiFilter>"; };
  bool                      IsEquiFilter::accept         (Occurrence item) const 
  {
    return (dynamic_cast<Equi*>(item.getEntity())!=NULL);
  }



  /**/                      IsComponentFilter::IsComponentFilter     ()                         {};
  /**/                      IsComponentFilter::IsComponentFilter     (const IsComponentFilter&) {};
  IsComponentFilter&        IsComponentFilter::operator=             (const IsComponentFilter&) {return *this; };
  Filter<Occurrence>*      IsComponentFilter::getClone              () const                   {return new IsComponentFilter(*this); };	   
  string                    IsComponentFilter::_getString            () const                   {return "<IsComponentFilter>"; };
  

  bool IsComponentFilter::accept (Occurrence item) const   {
    
    return (dynamic_cast<Component*>(item.getEntity())!=NULL); 
    
  }
  
  
  /**/            IsNetFilter::IsNetFilter   ()                           {};
  /**/            IsNetFilter::IsNetFilter   (const IsNetFilter&) {};
  IsNetFilter& IsNetFilter::operator=             (const IsNetFilter&) {return *this; };
  Filter<Occurrence>*       IsNetFilter::getClone              () const                     {return new IsNetFilter(*this); };	   
  string               IsNetFilter::_getString            () const                     {return "<IsNetFilter>"; };
  bool                 IsNetFilter::accept                (Occurrence item) const           
  {
    return (dynamic_cast<Net*>(item.getEntity())!= NULL);
  }


  
  /**/            IsNetOrComponentFilter::IsNetOrComponentFilter   ()                           {};
  /**/                       IsNetOrComponentFilter::IsNetOrComponentFilter   (const IsNetOrComponentFilter&) {};
  IsNetOrComponentFilter&    IsNetOrComponentFilter::operator=             (const IsNetOrComponentFilter&) {return *this; };
  Filter<Occurrence>*       IsNetOrComponentFilter::getClone              () const                     {return new IsNetOrComponentFilter(*this); };	   
  string               IsNetOrComponentFilter::_getString            () const                     {return "<IsNetOrComponentFilter>"; };
  bool                 IsNetOrComponentFilter::accept                (Occurrence item) const           
{
  if( dynamic_cast<Net*>(item.getEntity()) )
    return true;
  
  if( dynamic_cast<Component*>(item.getEntity()) )
    return true;
  
  return false;
}
  



  /**/                      IsLeafEquiFilter::IsLeafEquiFilter   ()                       {};
  /**/                      IsLeafEquiFilter::IsLeafEquiFilter   (const IsLeafEquiFilter&)    {};
  IsLeafEquiFilter&         IsLeafEquiFilter::operator=      (const IsLeafEquiFilter&)    {return *this; };
  Filter<Occurrence>*       IsLeafEquiFilter::getClone       () const                 {return new IsLeafEquiFilter(*this); };	   
  string                    IsLeafEquiFilter::_getString     () const                 {return "<IsLeafEquiFilter>"; };
  bool                      IsLeafEquiFilter::accept         (Occurrence item) const {assert(false); return false;}



  /**/                      IsNotLeafEquiFilter::IsNotLeafEquiFilter   ()                       {};
  /**/                      IsNotLeafEquiFilter::IsNotLeafEquiFilter   (const IsNotLeafEquiFilter&)    {};
  IsNotLeafEquiFilter&      IsNotLeafEquiFilter::operator=      (const IsNotLeafEquiFilter&)    {return *this; };
  Filter<Occurrence>*       IsNotLeafEquiFilter::getClone       () const                 {return new IsNotLeafEquiFilter(*this); };	   
  string                    IsNotLeafEquiFilter::_getString     () const                 {return "<IsNotLeafEquiFilter>"; };
  bool                      IsNotLeafEquiFilter::accept         (Occurrence item) const {assert(false); return false;}



  // -------------------------------------------------------------------
  // Class  :  "Equinox::NetIsUsedByExtractorNetFilter".
  

  
  /**/                        NetIsUsedByExtractorFilter::NetIsUsedByExtractorFilter   ()                           {};
  /**/                        NetIsUsedByExtractorFilter::NetIsUsedByExtractorFilter   (const NetIsUsedByExtractorFilter&) {};
  NetIsUsedByExtractorFilter& NetIsUsedByExtractorFilter::operator=             (const NetIsUsedByExtractorFilter&) {return *this; };
  Filter<Component*>*         NetIsUsedByExtractorFilter::getClone              () const                     {return new NetIsUsedByExtractorFilter(*this); };	   
  string                      NetIsUsedByExtractorFilter::_getString            () const                     {return "<NetIsUsedByExtractorFilter>"; };
  bool                        NetIsUsedByExtractorFilter::accept                (Component* item) const           {

    Box box     = item->getBoundingBox();
  
    if( (box.getYMin() == box.getYMax()) || (box.getXMin() == box.getXMax()) ) 
      return false;
    

    forEach ( BasicLayer*, i, item->getLayer()->getBasicLayers() )
      {
	if   (WithAlimStrategy::isExtractableLayer(*i)) return true;
      }
    
   
    return false;
  }
  
  
  


  
}// End of namespace Equinox


