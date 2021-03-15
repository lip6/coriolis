// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
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
#include "hurricane/HyperNet.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Segment.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Warning.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
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


// -------------------------------------------------------------------
// Class  :  "::DepthOrder".

  class DepthOrder {
    private:
      typedef  map <const Cell*,size_t,Entity::CompareById>  CellMap;
      typedef  pair<const Cell*,size_t>                      CellDepth;

      class CompareDepth {
        public:
          inline bool operator() ( const CellDepth& lhs, const CellDepth& rhs );
      };
    public:
                                      DepthOrder    ( const Cell*, Cell::Flags stopMask=Cell::Flags::NoFlags );
      inline const vector<CellDepth>& getCellDepths () const;
    private:
      size_t                          computeDepth  ( CellMap&, const Cell* );
    private:
      Cell::Flags        _stopMask;
      vector<CellDepth>  _cellDepths;
  };


  inline bool DepthOrder::CompareDepth::operator() ( const CellDepth& lhs, const CellDepth& rhs )
  {
    if (lhs.second < rhs.second) return true;
    if (lhs.second > rhs.second) return false;
    return lhs.first->getId() < rhs.first->getId();
  }


  inline const vector<DepthOrder::CellDepth>& DepthOrder::getCellDepths () const { return _cellDepths; }


  DepthOrder::DepthOrder ( const Cell* top, Cell::Flags stopMask )
    : _stopMask  (stopMask)
    , _cellDepths()
  {
    CellMap  cellMap;

    computeDepth( cellMap, top );

    for ( auto element : cellMap ) _cellDepths.push_back( element );
    sort( _cellDepths.begin(), _cellDepths.end(), CompareDepth() );
  }


  size_t  DepthOrder::computeDepth ( CellMap& cellMap, const Cell* cell )
  {
    auto ielement = cellMap.find( cell );
    if (ielement != cellMap.end()) return (*ielement).second;

    size_t depth = 0;
    for ( const Instance* instance : cell->getInstances() ) {
      if (not (instance->getMasterCell()->getFlags() & _stopMask)) {
        depth = std::max( depth, computeDepth(cellMap,instance->getMasterCell()) + 1 );
      }
    }
    cellMap.insert( make_pair(cell,depth) );
    return depth;
  }


} // Anonymous namespace.


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

static void attachPlugOrPin ( Net* net )
{
  unsigned int nbPlugs                   = 0;
  unsigned int nbPins                    = 0;
  unsigned int nbUnattachedPlugs         = 0;
  unsigned int nbUnattachedPins          = 0;
  Component*   firstAttachedComponent    = NULL;
  Component*   firstUnattachedComponent  = NULL;
  Component*   secondUnattachedComponent = NULL;

  for ( Plug* plug : net->getPlugs() ) {
    nbPlugs++;
    if (not plug->getBodyHook()->isAttached()) {
      if      (not  firstUnattachedComponent)  firstUnattachedComponent = plug;
      else if (not secondUnattachedComponent) secondUnattachedComponent = plug;
      nbUnattachedPlugs++;
    } else {
      if (not firstAttachedComponent ) firstAttachedComponent = plug;
    }
  }

  for ( Pin* pin : net->getPins() ) {
    nbPins++;
    if (not pin->getBodyHook()->isAttached()) {
      if      (not firstUnattachedComponent )  firstUnattachedComponent = pin;
      else if (not secondUnattachedComponent) secondUnattachedComponent = pin;
      nbUnattachedPins++;
    } else {
      if (not firstAttachedComponent ) firstAttachedComponent = pin;
    }
  }

  if (nbPlugs + nbPins < 2) return;
  
  if ( (nbPlugs + nbPins > 2) and (nbUnattachedPlugs + nbUnattachedPins != 1) ) {
    throw Error( "attachPlugOrPin(): Net \"%s\" has %u unattached Plugs/Pins,"
                 " while it should have only one."
               , getString(net->getName()).c_str()
               , nbUnattachedPlugs + nbUnattachedPins
               );
  }

  if (not firstAttachedComponent) {
    assert( firstUnattachedComponent );
    assert( secondUnattachedComponent );
    firstUnattachedComponent->getBodyHook()->attach( secondUnattachedComponent->getBodyHook() );
  } else {
    assert( firstUnattachedComponent );
    firstUnattachedComponent->getBodyHook()->attach( firstAttachedComponent->getBodyHook() );
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
      
    attachPlugOrPin(&net);
    
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


  size_t  _getInstancesCount ( const Cell* cell, map<const Cell*,size_t,Entity::CompareById>& gatesByMaster )
  {
    map<const Cell*,size_t,Entity::CompareById>::iterator imaster = gatesByMaster.find ( cell );
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
    map<const Cell*,size_t,Entity::CompareById>  gatesByMaster;

    return _getInstancesCount ( cell, gatesByMaster );
  }


  void  _restoreNetsDirection ( Cell* cell )
  {
    for ( Net* net : cell->getNets() ) {
      if (not net->isExternal()) continue;
      if (net->isSupply()) continue;

      size_t drivers = 0;
      size_t sinks   = 0;
      for ( Plug* plug : net->getPlugs() ) {
        Net* masterNet = plug->getMasterNet();
        Net::Direction direction = masterNet->getDirection();
        if (direction & Net::Direction::DirIn)  {
          sinks += 1;
        } else {
          if (direction & Net::Direction::DirOut) { 
            drivers += 1;
          }
        }
        if ((direction & Net::Direction::INOUT) == Net::Direction::INOUT) {
          cparanoid << Warning( "Net \"%s\" of cell \"%s\" has INOUT plug, considered as IN."
                              , getString(net->getName()).c_str()
                              , getString(cell->getName()).c_str()
                              ) << endl;
        }
      }
      if ((drivers == 0) and (sinks == 0)) {
        cparanoid << Warning( "Net \"%s\" of cell \"%s\" has neither driver nor sink, default as IN."
                            , getString(net->getName()).c_str()
                            , getString(cell->getName()).c_str()
                            ) << endl;
        net->setDirection( Net::Direction::DirIn );
        continue;
      }
      Net::Direction direction = net->getDirection();
      if (drivers == 0) {
        if (not (direction & Net::Direction::DirIn)) {
          cerr << Warning( "In Cell \"%s\" restoring %s as input (d=%d, s=%d)."
                         , getString(cell->getName()).c_str()
                         , getString(net).c_str()
                         , drivers
                         , sinks
                         ) << endl;
          net->setDirection( Net::Direction::DirIn );
        }
        continue;
      }
      if (not (direction & Net::Direction::DirOut)) {
        cerr << Warning( "In Cell \"%s\" restoring %s as output (d=%d, s=%d)."
                       , getString(cell->getName()).c_str()
                       , getString(net).c_str()
                       , drivers
                       , sinks
                       ) << endl;
        net->setDirection( Net::Direction::DirOut );
        for ( Occurrence occurrence : HyperNet(Occurrence(net)).getComponentOccurrences() ) {
          Plug* plug = dynamic_cast<Plug*>( occurrence.getEntity() );
          if (plug) {
            cerr << "          | " << occurrence.getPath().getCompactString(false)
                 << "." << plug->getInstance()->getName()
                 << ":" << plug->getMasterNet()->getName()
                 << " (" << plug->getMasterNet()->getCell()->getName() << ")" << endl;
          } else {
            cerr << "          | " << occurrence << endl;
          }
        }
      }
      if (drivers > 1) {
        cerr << Warning( "In Cell \"%s\", %s has more than one driver (%d)."
                       , getString(cell->getName()).c_str()
                       , getString(net).c_str()
                       , drivers
                       ) << endl;
        for ( Occurrence occurrence : HyperNet(Occurrence(net)).getComponentOccurrences() ) {
          Plug* plug = dynamic_cast<Plug*>( occurrence.getEntity() );
          if (plug) {
            cerr << "          | " << occurrence.getPath().getCompactString(false)
                 << "." << plug->getInstance()->getName()
                 << ":" << plug->getMasterNet()->getName()
                 << " (" << plug->getMasterNet()->getCell()->getName() << ")" << endl;
          } else {
            cerr << "          | " << occurrence << endl;
          }
        }
      }
    }
  }


  void  restoreNetsDirection ( Cell* topCell, Cell::Flags mask )
  {
    DepthOrder cellOrder ( topCell, mask );
    for ( auto item : cellOrder.getCellDepths() ) {
      _restoreNetsDirection( const_cast<Cell*>( item.first ) );
    }
  }


}  // End of CRL namespace.
