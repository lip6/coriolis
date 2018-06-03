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

    for ( Net* net : getCell()->getNets() ) {
      if (net == _blockageNet) continue;
      if (net->getType() == Net::Type::POWER ) continue;
      if (net->getType() == Net::Type::GROUND) continue;
    // Don't skip the clock.

      vector<Segment*>  segments;
      vector<Contact*>  contacts;

      bool   isPreRouted = false;
      bool   isFixed     = false;
      size_t rpCount     = 0;

      for( Component* component : net->getComponents() ) {
        if (dynamic_cast<Pin*>(component)) continue;

        const RegularLayer* layer = dynamic_cast<const RegularLayer*>(component->getLayer());
        if (layer and (layer->getBasicLayer()->getMaterial() == BasicLayer::Material::blockage))
          continue;

        Horizontal* horizontal = dynamic_cast<Horizontal*>(component);
        if (horizontal) {
          segments.push_back( horizontal );
          isPreRouted = true;
          if (horizontal->getWidth() != Session::getWireWidth(horizontal->getLayer()))
            isFixed = true;
        } else {
          Vertical* vertical = dynamic_cast<Vertical*>(component);
          if (vertical) {
            isPreRouted = true;
            segments.push_back( vertical );
            if (vertical->getWidth() != Session::getWireWidth(vertical->getLayer()))
              isFixed = true;
          } else {
            Contact* contact = dynamic_cast<Contact*>(component);
            if (contact) {
              isPreRouted = true;
              contacts.push_back( contact );
              if (  (contact->getWidth () != Session::getViaWidth(contact->getLayer()))
                 or (contact->getHeight() != Session::getViaWidth(contact->getLayer()))
                 or (contact->getLayer () == Session::getContactLayer(0)) )
                isFixed = true;
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

      if ((not isFixed and not isPreRouted) and net->isDeepNet()) {
        Net* rootNet = dynamic_cast<Net*>(
                         dynamic_cast<DeepNet*>(net)->getRootNetOccurrence().getEntity() );
        for( Component* component : rootNet->getComponents() ) {
          if (dynamic_cast<Horizontal*>(component)) { isFixed = true; break; }
          if (dynamic_cast<Vertical*>  (component)) { isFixed = true; break; }
          if (dynamic_cast<Contact*>   (component)) { isFixed = true; break; }
        }
      }

      if (isFixed or isPreRouted or (rpCount < 2)) {
        NetData*         ndata = getNetData( net, Flags::Create );
        NetRoutingState* state = ndata->getNetRoutingState();
        state->unsetFlags( NetRoutingState::AutomaticGlobalRoute );
        state->setFlags  ( NetRoutingState::ManualGlobalRoute );
        ndata->setGlobalRouted( true );
        if (rpCount < 2)
          state->setFlags  ( NetRoutingState::Unconnected );

        if (isFixed) {
          cmess2 << "     - <" << net->getName() << "> is fixed." << endl;
          state->unsetFlags( NetRoutingState::ManualGlobalRoute );
          state->setFlags  ( NetRoutingState::Fixed );
        } else {
          if (rpCount > 1) {
            ++toBeRouteds;
            
            cmess2 << "     - <" << net->getName() << "> is manually global routed." << endl;
            for ( auto icontact : contacts ) {
              AutoContact::createFrom( icontact );
            }
          
            for ( auto isegment : segments ) {
              AutoContact* source = Session::lookup( dynamic_cast<Contact*>( isegment->getSource() ));
              AutoContact* target = Session::lookup( dynamic_cast<Contact*>( isegment->getTarget() ));
              AutoSegment* autoSegment = AutoSegment::create( source, target, isegment );
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
