// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           Alliance / Hurricane  Interface                       |
// |                                                                 |
// |  Author      :                 The Borg Collective              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ToolBox.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/Plug.h"
#include "hurricane/Pin.h"
#include "hurricane/Library.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Segment.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Warning.h"
using namespace Hurricane;

#include "crlcore/ToolBox.h"

    
namespace {


  void attachPlugs ( Net* net )
  {
    forEach ( Plug*, plug, net->getPlugs() ) {
      if ( plug->getBodyHook()->isAttached() )
        throw Error("attachPlugs(): Aborted, a plug MasterHook is attached to %s."
                   ,getString(plug).c_str());
    }

    PlugLocator  plugLocator  = net->getPlugs().getLocator();
    Plug*        previousPlug = plugLocator.getElement();
    Plug*        plug;

    plugLocator.progress();
    while ( plugLocator.isValid() ) {
      plug= plugLocator.getElement ();
      plug->getBodyHook()->attach ( previousPlug->getBodyHook() );
      previousPlug = plug;
      plugLocator.progress ();
    }
  }


} // End of anonymous namespace.


namespace CRL {


Component* getBestExternalComponent ( Net* net )
{
  Component* bestComponent = NULL;

  forEach ( Component*, icomponent, NetExternalComponents::get(net) ) {
    if ( !bestComponent ) { bestComponent = *icomponent; continue; }
    if (      /*IsOnTop(component->getLayer(),bestComponent->getLayer())
                || */ ( getArea(*icomponent) > getArea(bestComponent) ) ) {
      bestComponent = *icomponent;
    }
  }
  return bestComponent;
}


bool  placeNet ( Net* net )
{
  assert(net);

  if ( not net->isExternal() ) return false;
  Component* bestComponent = getBestExternalComponent(net);

  if ( bestComponent ) {
    net->setPosition(bestComponent->getCenter());
    return true;
  }
  return false;
}


void  placeNets ( Cell* cell )
{
  assert(cell);

  UpdateSession::open();

  vector<Net*> unplaceds;

  forEach ( Net*, inet, cell->getExternalNets() ) {
    if ( !placeNet(*inet) ) unplaceds.push_back(*inet);
  }

  Point abCenter = cell->getAbutmentBox().getCenter();
  for ( size_t i=0 ; i<unplaceds.size() ; ++i ) {
    unplaceds[i]->setPosition(abCenter);
  }

  UpdateSession::close();
}


void  placeMasterNets ( const Library* library )
{
  assert ( library );

  for_each_cell ( cell, library->getCells() ) {
    placeNets ( cell );
    end_for
  }
}


void placePlug (Plug* plug)
{
  assert ( plug );

  placeNet ( plug->getMasterNet() );
}


void  placePlugs ( Cell* cell )
{
  assert ( cell );

  UpdateSession::open();

  for_each_net ( net, cell->getNets() ) {
    for_each_plug ( plug, net->getPlugs() ) {
      placePlug ( plug );
      end_for;
    }
    end_for
  }

  UpdateSession::close ();
}

// static void AttachPlug(Net& net)
// {
//     int temp1 = 0, temp2 = 0;

//     temp1 = net.getPlugs().getSize();
    
//     // si un seul plug, rien a faire    
//     if(temp1 == 1)
//       return;
//     // sinon
//     else
//     {
//       // on compte le nombre de plugs attachés
//       for_each_plug(plug, net.getPlugs())
//       {
// 	      if (plug->getBodyHook()->isAttached())
// 	        temp2++;
// 	      end_for;
//       }
      
//       // Problème avec l'attachement des SplittersContact
//       //// si deux plugs, aucun des deux ne doit être attaché      
//       //if(temp1 == 2 && temp2 != 0)
//       //  throw Error("Cannot create plugs ring: A plug MasterHook is attached");      
//       // si plus de plugs, un seul non attaché
//       //else if(temp1 >2 && temp2 != temp1-1)
//       //  throw Error("Cannot create plugs ring: A plug MasterHook is attached");
//       if(temp1 >2 && temp2 != temp1-1)
//       {
//         string message = "AttachPlug() : Net " + getString ( net ) + " has " + getString ( temp1 - temp2 )
//                          + " unattached plugs : it should have only one unattached plug.";
//         throw Error( message );
//       }
//     }
    
//     Plugs plugs = net.getPlugs();
//     PlugLocator plugLocator = plugs.getLocator();
//     Plug* previousPlug = plugLocator.getElement();
//     plugLocator.progress();

//     // si le plug est celui qui est non attaché
//     if(!previousPlug->getBodyHook()->isAttached())
//     {
// 	    Plug* plug= plugLocator.getElement();
//         previousPlug->getBodyHook()->attach(plug->getBodyHook());
//     }
//     else
//     {
//       // on parcourt jusqu'à obtenir le plug non attaché
//       while( plugLocator.IsValid() && plugLocator.getElement()->getBodyHook()->isAttached() )
//       {
//     	  previousPlug = plugLocator.getElement();      
//           plugLocator.progress();
//       }
    
//       if(plugLocator.IsValid())
//       {
// 	      Plug* plug= plugLocator.getElement();
//           plug->getBodyHook()->attach(previousPlug->getBodyHook());
//       }
//       else
//         throw Error("Cannot create plugs ring : no plug to attach");
//     }
//}

static void AttachPlugOrPin(Net& net)
{
    unsigned nbPlugs = 0;
    unsigned nbPins  = 0;
    unsigned nbUnattachedPlugs = 0;
    unsigned nbUnattachedPins  = 0;

    Component* firstAttachedComponent    = NULL;
    Component* firstUnattachedComponent  = NULL;
    Component* secondUnattachedComponent = NULL;

    for_each_plug ( plug, net.getPlugs() ) {
        nbPlugs++;
        if ( !plug->getBodyHook()->isAttached() ) {
            if ( !firstUnattachedComponent )
                firstUnattachedComponent = plug;
            else
                if ( !secondUnattachedComponent )
                    secondUnattachedComponent = plug;
            nbUnattachedPlugs++;
        }
        else {
            if ( !firstAttachedComponent )
                firstAttachedComponent = plug;
        }
        end_for
    }

    for_each_pin ( pin, net.getPins() ) {
        nbPins++;
        if ( !pin->getBodyHook()->isAttached() ) {
            if ( !firstUnattachedComponent )
                firstUnattachedComponent = pin;
            else
                if ( !secondUnattachedComponent )
                    secondUnattachedComponent = pin;
            nbUnattachedPins++;
        }
        else {
            if ( !firstAttachedComponent )
                firstAttachedComponent = pin;
        }
        end_for
    }

    if ( (nbPlugs + nbPins) == 1 )
        return;

    if ( ((nbPlugs + nbPins) > 2) && ((nbUnattachedPlugs + nbUnattachedPins) != 1) ) {
        string message = "AttachPlugOrPin() : Net " + getString(&net) + " has " + getString(nbUnattachedPlugs + nbUnattachedPins)
                                              + " unattached plugs/pins : it should have only one unattached plug/pin.";
        throw Error(message);
    }

    if ( !firstAttachedComponent ) {
        assert ( firstUnattachedComponent );
        assert ( secondUnattachedComponent );
        firstUnattachedComponent->getBodyHook()->attach ( secondUnattachedComponent->getBodyHook() );
    }
    else {
        assert ( firstUnattachedComponent );
        firstUnattachedComponent->getBodyHook()->attach ( firstAttachedComponent->getBodyHook() );
    }

}


void createPartRing(Cell* cell, Name nom)
{
    if (!cell)
        throw Error("Cannot create part ring : cell doesn't exist");
    UpdateSession::open();

    Net *net = cell->getNet(nom);

    if(!net)
        net = Net::create(cell, nom);

    createPartRing2(*net);

    UpdateSession::close();
}


void createPartRing2(Net& net)
{
    UpdateSession::open();
    
    if (net.isGlobal())
      throw Warning("Cannot create plugs ring on global Net");
      
    AttachPlugOrPin(net);
    
    UpdateSession::close();
}


static void AttachPlugsAndPins(Net* net)
{
    list<Component*> componentsToAttachList;
    for_each_plug(plug, net->getPlugs())
    {
	if (plug->getBodyHook()->isAttached())
	    throw Error("Cannot create plugs and pins ring: A plug BodyHook is attached");
        componentsToAttachList.push_back(plug);
	end_for;
    }
    for_each_pin(pin, net->getPins())
    {
        if (pin->getBodyHook()->isAttached())
            throw Error("Cannot create plugs and pins ring: A pin BodyHook is attached");
        componentsToAttachList.push_back(pin);
        end_for;
    }
    list<Component*>::iterator lcit = componentsToAttachList.begin();
    Component* previousComponent = *lcit++;
    while (lcit != componentsToAttachList.end())
    {
        Component* component = *lcit++;
        component->getBodyHook()->attach(previousComponent->getBodyHook());
        previousComponent = component;
    }
}

static void AttachContacts(Net* net)
{
    list<Component*> componentsToAttachList;
    for_each_contact(contact, net->getContacts())
    {
        if (contact->getBodyHook()->isAttached())
            throw Error("Cannot create contacts ring: A bodyHook is attached");
            componentsToAttachList.push_back(contact);
        end_for;
    }
    list<Component*>::iterator lcit = componentsToAttachList.begin();
    Component* previousComponent = *lcit++;
    while (lcit != componentsToAttachList.end())
    {
        Component* component = *lcit++;
        component->getBodyHook()->attach(previousComponent->getBodyHook());
        previousComponent = component;
    }
}

void createPlugsRing(Net& net)
{
    UpdateSession::open();
    
    if (net.isGlobal())
      throw Warning("Cannot create plugs ring on global Net");
    if (net.getPlugs().getSize() <= 1)
	    return;
    
    attachPlugs(&net);
    UpdateSession::close();
}

void createPlugsRing(Cell* cell)
{
    if (!cell)
        throw Error("Cannot create plugs ring : cell doesn't exist");
    UpdateSession::open();
    
    for_each_net(net, cell->getNets())
    {
	if (net->isGlobal())
	    continue;
	if (net->getPlugs().getSize() <= 1)
	    continue;
	attachPlugs(net);
	end_for;
    }
    
    UpdateSession::close();
}

  void createPlugsAndPinsRing ( Cell* cell )
  {
    if (!cell)
      throw Error( "Cannot create plugs and pins ring: NULL cell." );

    UpdateSession::open();
    forEach ( Net*, net, cell->getNets() ) {
      if ( net->isGlobal() ) continue;
      if ( net->getPlugs().getSize() + net->getPins().getSize() <= 1 ) continue;
      if ( net->getPins().getSize() == 0 ) {
        attachPlugs(*net);
      } else
        AttachPlugsAndPins(*net);
    }
    UpdateSession::close();
  }


void createContactsRing(Cell* cell)
{
    if (!cell)
        throw Error("Cannot create contacts ring : cell doesn't exist");
    UpdateSession::open();
    for_each_net(net, cell->getNets())
    {
        if (net->isGlobal())
            continue;
        if (net->getContacts().getSize() <= 1)
            continue;
        else
            AttachContacts(net);
        end_for;
    }
    UpdateSession::close();
}


  void  deleteEmptyNets ( Cell* cell )
  {
    vector<Net*> nets;
    for ( Net* net : cell->getNets() )
      nets.push_back ( net );

    for ( size_t i=0 ; i<nets.size() ; ++i ) {
      if (     not nets[i]->getComponents().getFirst()
           and (nets[i]->getType() != Net::Type::POWER   )
           and (nets[i]->getType() != Net::Type::GROUND  )
           and (nets[i]->getType() != Net::Type::CLOCK   )
           and (nets[i]->getType() != Net::Type::BLOCKAGE) ) {
        nets[i]->destroy();
      }
    }
  }


Transformation getTransformation(const Box& abox, const DbU::Unit& x, const DbU::Unit& y, const Transformation::Orientation& orientation)
{
    switch (orientation) {
        case Transformation::Orientation::ID :
            return Transformation(x-abox.getXMin(), y-abox.getYMin(), orientation);
        case Transformation::Orientation::R1 :
            return Transformation(x+abox.getYMax(), y-abox.getXMin(), orientation);
        case Transformation::Orientation::R2 :
            return Transformation(x+abox.getXMax(), y+abox.getYMax(), orientation);
        case Transformation::Orientation::R3 :
            return Transformation(x-abox.getYMin(), y+abox.getXMax(), orientation);
        case Transformation::Orientation::MX :
            return Transformation(x+abox.getXMax(), y-abox.getYMin(), orientation);
        case Transformation::Orientation::XR :
            return Transformation(x+abox.getYMax(), y+abox.getXMax(), orientation);
        case Transformation::Orientation::MY :
            return Transformation(x-abox.getXMin(), y+abox.getYMax(), orientation);
        case Transformation::Orientation::YR :
            return Transformation(x-abox.getYMin(), y-abox.getXMin(), orientation);
    }
    return Transformation();
}

bool isNoInstancePlacedOrFixed ( Cell* cell )
{
    bool noPlacedOrFixed = true;
    for_each_instance ( instance, cell->getInstances() )
    {
        if ( instance->isPlaced() || instance->isFixed() ) {
            noPlacedOrFixed = false;
            break;
        }
        end_for;
    }
    
    return noPlacedOrFixed;
}

set<Cell*> __plughooksconnected;

void _ConnectPlugHooks(Cell* cell)
{
    if (__plughooksconnected.find(cell) != __plughooksconnected.end()) return;
            
    __plughooksconnected.insert(cell);

    for_each_component(component,cell->getComponents())
    {
        for_each_instance(instance,cell->getInstancesUnder(component->getBoundingBox()))
        {
            for_each_component(external,instance->getMasterCell()->getComponentsUnder(
                instance->getTransformation().getInvert().getBox(component->getBoundingBox())))
            {
                if (component->getLayer()->intersect(external->getLayer())) {
                    Plug* plug= instance->getPlug(external->getNet());
                    if (!plug)
                        cerr << "[1;33mWarning[0m : Short circuit detected between " << component
                             << " and non external " << instance->getName() << ":" << external
                             << " in cell " << cell->getName() << endl; 
                    else if (plug->getNet() != component->getNet())
                        cerr << "[1;33mWarning[0m : Short circuit detected between " << component
                             << " (net " << component->getNet() << ") "
                             << " and " << instance->getName() << ":" << external
                             << " (net " << plug->getNet() << ")"
                             << " in cell " << cell->getName() << endl; 
                    else
                    {
                        Contact* contact= dynamic_cast<Contact*>(component);
                        if (!contact) continue;
                        if (contact->getAnchor() == plug) continue;
                        Hook* plugHook = plug->getBodyHook();

                        //detach plug from it's current ring
                        plugHook->detach();

                        //Attach contact to plug
                        Point position= contact->getPosition();
                        contact->getAnchorHook()->attach(plugHook);
                        contact->setPosition(position); //Compute the offset to the anchor
                    }
                } // if intersect layer
                end_for;
            } //for_each_component external
            end_for;
        } //for_each_instance
        end_for;
    } //for_each_component component

    for_each_instance(instance,cell->getInstances())
    {
        _ConnectPlugHooks(instance->getMasterCell());
        end_for;
    }
}

void ConnectPlugHooks(Cell* cell)
{
    UpdateSession::open();
    _ConnectPlugHooks(cell);
    UpdateSession::close();
    __plughooksconnected.clear();
}


  size_t  _getInstancesCount ( const Cell* cell, map<const Cell*,size_t>& gatesByMaster )
  {
    map<const Cell*,size_t>::iterator imaster = gatesByMaster.find ( cell );
    if ( imaster != gatesByMaster.end() )
      return imaster->second;

    size_t gates = 0;
    forEach ( Instance*, iinstance,  cell->getInstances() ) {
      Cell* masterCell = iinstance->getMasterCell();

      if ( masterCell->isTerminal() )
        ++gates;
      else
        _getInstancesCount ( masterCell, gatesByMaster );
    }
    gatesByMaster.insert ( make_pair(cell,gates) );

    return gates;
  }


  size_t  getInstancesCount ( const Cell* cell )
  {
    map<const Cell*,size_t>  gatesByMaster;

    return _getInstancesCount ( cell, gatesByMaster );
  }


}  // End of CRL namespace.
