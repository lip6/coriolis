// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PreRouteds.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/Warning.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/DeepNet.h"
#include "hurricane/Pin.h"
#include "hurricane/RoutingPad.h"
#include "crlcore/AllianceFramework.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::cerr;
  using std::cout;
  using std::endl;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::BasicLayer;
  using Hurricane::RegularLayer;
  using Hurricane::Component;
  using Hurricane::Pin;
  using Hurricane::Plug;
  using Hurricane::DeepNet;
  using Hurricane::Cell;
  using Hurricane::NetRoutingExtension;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;
  using CRL::AllianceFramework;


  void  AnabaticEngine::setupSpecialNets ()
  {
    AllianceFramework* af = AllianceFramework::get();
    for ( Net* net : _cell->getNets() ) {
      const char* excludedType = NULL;
      if (net->getType() == Net::Type::POWER ) excludedType = "POWER";
      if (net->getType() == Net::Type::GROUND) excludedType = "GROUND";
      if (excludedType) {
        cparanoid << Warning( "%s is not a routable net (%s,excluded)."
                            , getString(net).c_str(), excludedType ) << endl;
      }
      if (af->isBLOCKAGE(net->getName())) excludedType = "BLOCKAGE";
      if (excludedType) {
        NetData*         ndata = getNetData( net, Flags::Create );
        NetRoutingState* state = ndata->getNetRoutingState();
        state->setFlags( NetRoutingState::Fixed );
        ndata->setGlobalRouted( true );
      }
    }
  }



  size_t  AnabaticEngine::setupPreRouteds ()
  {
    cmess1 << "  o  Looking for fixed or manually global routed nets." << endl;

    openSession();

    size_t toBeRouteds = 0;
    Box    ab          = getCell()->getAbutmentBox();
    ab.inflate( -1 );

    for ( Net* net : getCell()->getNets() ) {
      if (net == _blockageNet) continue;
      if (net->getType() == Net::Type::POWER ) continue;
      if (net->getType() == Net::Type::GROUND) continue;
    // Don't skip the clock.

      vector<Pin*>      pins;
      vector<Segment*>  segments;
      vector<Contact*>  contacts;

      bool   isManualGlobalRouted = false;
      bool   isManualDetailRouted = false;
      bool   isFixed              = false;
      size_t rpCount              = 0;

      for( Component* component : net->getComponents() ) {
        Pin* pin = dynamic_cast<Pin *>( component );
        if (pin) {
          pins.push_back( pin );
          continue;
        }
        if (dynamic_cast<Plug*>(component)) continue;

        const RegularLayer* layer = dynamic_cast<const RegularLayer*>(component->getLayer());
        if (layer and (layer->getBasicLayer()->getMaterial() == BasicLayer::Material::blockage))
            continue;

        if (   not Session::isGaugeLayer(component->getLayer())
           and not Session::isGLayer    (component->getLayer())) {
          const BasicLayer* basicLayer = dynamic_cast<const BasicLayer*>( component->getLayer() );
          if (basicLayer and (basicLayer->getMaterial() == BasicLayer::Material::cut))
            continue;
          throw Error( "AnabaticEngine::setupPreRouted(): A component of \"%s\" has a routing gauge umanaged layer.\n"
                       "        (%s)"
                     , getString(net->getName()).c_str()
                     , getString(component).c_str()
                     );
        }

        Horizontal* horizontal = dynamic_cast<Horizontal*>(component);
        if (horizontal) {
          if (    not ab.contains(horizontal->getSourcePosition())
             and  not ab.contains(horizontal->getTargetPosition()) ) continue;

          segments.push_back( horizontal );
          if (Session::isGLayer(component->getLayer())) {
            isManualGlobalRouted = true;
          } else {
            isManualDetailRouted = true;
            if (horizontal->getWidth() != Session::getWireWidth(horizontal->getLayer()))
              isFixed = true;
          }
        } else {
          Vertical* vertical = dynamic_cast<Vertical*>(component);
          if (vertical) {
            if (    not ab.contains(vertical->getSourcePosition())
               and  not ab.contains(vertical->getTargetPosition()) ) continue;

            if (Session::isGLayer(component->getLayer())) {
              isManualGlobalRouted = true;
            } else {
              isManualDetailRouted = true;
              segments.push_back( vertical );
              if (vertical->getWidth() != Session::getWireWidth(vertical->getLayer()))
                isFixed = true;
            }
          } else {
            Pin* pin = dynamic_cast<Pin*>(component);
            if (pin) {
            //cerr << "| " << pin << endl;
              if (not ab.intersect(pin->getBoundingBox())) continue;
              pins.push_back( pin );
            } else {
              Contact* contact = dynamic_cast<Contact*>(component);
              if (contact) {
                if (not ab.contains(contact->getCenter())) continue;
              
                if (Session::isGLayer(component->getLayer())) {
                  isManualGlobalRouted = true;
                } else {
                  isManualGlobalRouted = true;
                  contacts.push_back( contact );
                  if (  (contact->getWidth () != Session::getViaWidth(contact->getLayer()))
                     or (contact->getHeight() != Session::getViaWidth(contact->getLayer()))
                     or (contact->getLayer () == Session::getContactLayer(0)) )
                    isFixed = true;
                }
              } else {
                RoutingPad* rp = dynamic_cast<RoutingPad*>(component);
                if (rp) {
                  ++rpCount;
                } else {
                  // Plug* plug = dynamic_cast<Plug*>(component);
                  // if (plug) {
                  //   cerr << "buildPreRouteds(): " << plug << endl;
                  //   ++rpCount;
                  // }
                }
              }
            }
          }
        }
      }

      // cerr << net << " deepNet:" << net->isDeepNet()
      //      << " pins:" << pins.size() 
      //      << " segments:" << segments.size() << endl;
      if (not net->isDeepNet() and (pins.size() >= 1) and (segments.size() < 2)) {
        ++toBeRouteds;
        continue;
      }

      if (   (not isFixed)
         and (not isManualGlobalRouted)
         and (not isManualDetailRouted)
         and net->isDeepNet()) {
        Net* rootNet = dynamic_cast<Net*>(
                         dynamic_cast<DeepNet*>(net)->getRootNetOccurrence().getEntity() );
        for( Component* component : rootNet->getComponents() ) {
          if (dynamic_cast<Horizontal*>(component)) { isFixed = true; break; }
          if (dynamic_cast<Vertical*>  (component)) { isFixed = true; break; }
          if (dynamic_cast<Contact*>   (component)) { isFixed = true; break; }
        }
      }

      if (isFixed or isManualDetailRouted or isManualGlobalRouted or (rpCount < 2)) {
        NetData*         ndata = getNetData( net, Flags::Create );
        NetRoutingState* state = ndata->getNetRoutingState();
        state->unsetFlags( NetRoutingState::AutomaticGlobalRoute );
        if (isManualGlobalRouted) {
          state->setFlags( NetRoutingState::ManualGlobalRoute );
          ndata->setGlobalFixed( true );
        }
        if (isManualDetailRouted)
          state->setFlags( NetRoutingState::ManualDetailRoute );
        ndata->setGlobalRouted( true );
        if (rpCount < 2)
          state->setFlags( NetRoutingState::Unconnected );

        if (isFixed) {
          if (rpCount > 1)
            cmess2 << "     - <" << net->getName() << "> is fixed." << endl;
          state->unsetFlags( NetRoutingState::ManualGlobalRoute|NetRoutingState::ManualDetailRoute );
          state->setFlags  ( NetRoutingState::Fixed );
        } else if (isManualGlobalRouted) {
          cmess2 << "     - <" << net->getName() << "> is manually global routed." << endl;
        } else if (isManualDetailRouted) {
          if (rpCount > 1) {
            ++toBeRouteds;
            cmess2 << "     - <" << net->getName() << "> is manually detail routed." << endl;
            for ( auto contact : contacts ) {
              AutoContact::createFrom( contact );
            }
            for ( auto segment : segments ) {
              AutoContact* source = Session::lookup( dynamic_cast<Contact*>( segment->getSource() ));
              AutoContact* target = Session::lookup( dynamic_cast<Contact*>( segment->getTarget() ));
              if (not source or not target) {
                cerr << Error( "Unable to protect %s", getString(segment).c_str() ) << endl;
                continue;
              }
              AutoSegment* autoSegment = AutoSegment::create( source, target, segment );
              autoSegment->setFlags( AutoSegment::SegUserDefined|AutoSegment::SegAxisSet );
            }
          }
        }
      } else {
        ++toBeRouteds;
      }
    }

    Session::close();

    return toBeRouteds;
  }


}  // Anabatic namespace.
