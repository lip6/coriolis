// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2018, All Rights Reserved
//
// x-----------------------------------------------------------------x
// |                   C O R I O L I S                               |
// |        K n i k  -  G l o b a l   R o u t e r                    |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :               Damien.Dupuis@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./KnikEngine.cpp"                              |
// x-----------------------------------------------------------------x


#include <climits>
#include "flute.h"
#include "hurricane/Warning.h"
#include "hurricane/Property.h"
#include "hurricane/NetRoutingProperty.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Contact.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Cell.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Breakpoint.h"
#include "crlcore/Utilities.h"
#include "crlcore/ToolBox.h"
#include "crlcore/Measures.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/RoutingLayerGauge.h"
#include "crlcore/AllianceFramework.h"
#include "knik/Configuration.h"
#include "knik/Edge.h"
#include "knik/Vertex.h"
#include "knik/Graph.h"
#include "knik/RoutingGrid.h"
#include "knik/NetExtension.h"
#include "knik/KnikEngine.h"


#define MAX_RUNTIME 86400
#define MAX_ITERATION UINT_MAX


namespace Knik {

  using Hurricane::Warning;
  using Hurricane::NetRoutingExtension;
  using Hurricane::NetRoutingState;
  using CRL::addMeasure;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;
  using CRL::AllianceFramework;


// Global Variables
  unsigned int  __congestion__;
  unsigned int  __precongestion__;
  float         __edge_cost__;
  bool          __initialized__;
  
  extern bool   __ripupMode__;

  const Name KnikEngine::_toolName           = "Knik::KnikEngine";
  size_t     KnikEngine::_hEdgeReservedLocal = 0;
  size_t     KnikEngine::_vEdgeReservedLocal = 0;


  KnikEngine::KnikEngine ( Cell*     cell
                         , unsigned  congestion
                         , unsigned  precongestion
                         , bool      benchMode
                         , bool      useSegments
                         , float     edgeCost )
    : CRL::ToolEngine  ( cell )
    , _routingGauge    ( NULL )
    , _allowedDepth    ( 0)
    , _routingGraph    ( NULL )
    , _routingGrid     ( NULL )
    , _timer           ()
    , _nets_to_route   ()
    , _benchMode       ( benchMode )
    , _useSegments     ( useSegments )
    , _routingDone     ( false )
    , _rerouteIteration( 0 )
    , _segmentOverEdges()                              
    , _sortSegmentOv   ()
  {
    if (congestion > 1)
      throw Error ( "KnikEngine::KnikEngine(): congestion argument must be 0 (None) or 1 (Congestion) : %s."
                  , getString(congestion).c_str() );
    __congestion__    = congestion;

    if ( precongestion > 2 )
      throw Error ( "KnikEngine::KnikEngine():  precongestion argument must be 0 (None), 1 (Static) or 2 (Dynamic) : %s."
                  , getString(precongestion).c_str() );

    __precongestion__ = precongestion;
    __edge_cost__     = edgeCost;
    __initialized__   = false;
  }


KnikEngine::~KnikEngine ()
// ***********************
{
}

KnikEngine* KnikEngine::create ( Cell* cell, unsigned congestion, unsigned precongestion, bool benchMode, bool useSegments, float edgeCost )
// *****************************************************************************************************************************************
{
    CRL::deleteEmptyNets ( cell );
    KnikEngine* _knik = new KnikEngine ( cell, congestion, precongestion, benchMode, useSegments, edgeCost );
    
    _knik->_postCreate();

    // This is also printed in the banner of Unicorn/Cgt, no need to remind it here again.
    // cout << "  o  Knik -- Global router makes use of FLUTE software" << endl;
    // cout << Dots::asIdentifier("     - Author"       ,"Chris C. N. CHU") << endl;
    // cout << Dots::asIdentifier("     - Prof. Ident. ","Iowa State University") << endl;
    // cout << Dots::asIdentifier("     - URL"          ,"http://home.eng.iastate.edu/~cnchu") << endl;

    return _knik;
}

void KnikEngine::_postCreate()
// ***************************
{
    Inherit::_postCreate();

    // For Flute : readLUT to be able to use POWV9.dat & POST9.dat
    Flute::readLUT( AllianceFramework::get()->getEnvironment()->getCORIOLIS_TOP() );

    return;
}

void KnikEngine::destroy()
// ***********************
{
    _preDestroy();
    delete this;
}

void KnikEngine::_preDestroy()
// ***************************
{
    _routingGraph->destroy();
    Inherit::_preDestroy();
  //Configuration::get()->destroy();
    return;
}


  void  KnikEngine::setRoutingGauge ( RoutingGauge* rg )
  {
    _routingGauge = rg;
    _allowedDepth = rg->getDepth();
  }


  void  KnikEngine::setAllowedDepth ( unsigned int allowedDepth )
  {
    if (not _routingGauge)
      cerr << Error( "KnikEngine::setAllowedDepth(): Must set the RoutingGauge before the allowed depth." ) << endl;

    _allowedDepth = (allowedDepth < _routingGauge->getDepth()) ? allowedDepth : _routingGauge->getDepth();
  }


void KnikEngine::MakeRoutingLeaves()
// *********************************
{
    //unsigned steps = getCell()->getAbutmentBox().getHeight() / DbU::lambda(50); // +o( mieux vaudrait recuperer le slice !
    //if ( _nimbus->getDepth() < steps ) {
    //    _nimbus->progress ( steps - _nimbus->getDepth() );
    //    _nimbus->PlacementLeavesDown();
    //}
//    for_each_gcell(gcell, _nimbus->getPlacementLeaves()) {
//        //if (_metisseLimit)  // dans un premier temps on se limite � respecter les placementLeaves (on ne descend pas en dessous)
//        if (gcell->isAStrictSubGCellOf(gcell->getPlacementLeaf()))
//            break;
//        gcell->setAsRoutingLeaf();
//        for_each_fence (fence, gcell->getSurroundingFences()) {
//            fence->setAsRoutingFence();
//            end_for;
//        }
//        end_for;
//    }
    return;
}

void KnikEngine::initGlobalRouting( const map<Name,Net*>& excludedNets )
// *********************************************************************
{
  assert( _nets_to_route.empty() );
  cmess2 << "  o  Initializing global routing." << endl;
//#if defined(__USE_STATIC_PRECONGESTION__) || defined(__USE_DYNAMIC_PRECONGESTION__)
  cmess1 << Dots::asUInt  ( "     - Congestion Mode"    , __congestion__    ) << endl;
  cmess1 << Dots::asUInt  ( "     - Pre-Congestion Mode", __precongestion__ ) << endl;
  cmess1 << Dots::asDouble( "     - Edge Cost"          , __edge_cost__     ) << endl;
//#endif

  if (not _routingGraph) {
    _timer.resetIncrease();
    _timer.start();

    _routingGraph = Graph::create ( this, _routingGrid, _benchMode, _useSegments );
    cmess2 << "     - Created RoutingGraph [" << _routingGraph->getXSize()
           << "x" << _routingGraph->getYSize() << "]." << endl;

    _timer.stop();
  } else {
    cmess2 << "     - Reusing pre-existing graph." << endl;
  }

  _timer.resetIncrease();
  _timer.start();

  cmess2 << "  o  Selecting nets to route and create precongestion." << endl;

  const unsigned int MaxDegree = 13000;
  Name obstacleNetName ("obstaclenet");

  forEach ( Net*, inet, getCell()->getNets() ) {
    if (excludedNets.find(inet->getName()) != excludedNets.end()) {
      if (NetRoutingExtension::isUnconnected(*inet))
        cparanoid << "     - <" << inet->getName() << "> not routed (unconnected)." << endl;
      else
        cparanoid << "     - <" << inet->getName() << "> not routed (pre-routing found)." << endl;
      continue;
    }

    if (   inet->isGlobal()
       or  inet->isSupply()
       or (inet->getName() == obstacleNetName) ) {
      cmess2 << "     - <" << inet->getName() << "> not routed (global, supply, clock or obstacle)." << endl;
      continue;
    }

  // We want to route nets with more than 2 and less than MaxDegree vertexes
    unsigned netDegree = _routingGraph->countVertexes ( *inet );
    if ( (netDegree > 1) and (netDegree < MaxDegree) ) {
      Box       bbox   = inet->getBoundingBox();
      NetRecord record ( *inet, (long int)((DbU::getLambda(bbox.getWidth())+1)*(DbU::getLambda(bbox.getHeight())+1)) );
      assert( record._net );
      assert( record._exArea > 0 );

      _nets_to_route.push_back( record );
//#if defined(__USE_STATIC_PRECONGESTION__) || defined(__USE_DYNAMIC_PRECONGESTION__)
      if (__precongestion__)
        _routingGraph->UpdateEstimateCongestion( true );
//#endif
    } else {
      if (netDegree > MaxDegree-1)
        cmess1 << Warning("%s has a not a degree in [2:%u[ (%d), not routed."
                         ,getString(*inet).c_str(),MaxDegree,netDegree) << endl;
    }

    _routingGraph->resetVertexes();
  }

  stable_sort( _nets_to_route.begin(), _nets_to_route.end(), NetSurfacesComp() );
  NetVector::iterator new_end = unique( _nets_to_route.begin(), _nets_to_route.end() );
  _nets_to_route.erase( new_end, _nets_to_route.end() );

  _timer.stop();
  cmess2 << "     - # of Nets to route:" << _nets_to_route.size() << endl;

  // for ( NetRecord record : _nets_to_route ) {
  //   cerr << "Route: " << record._net << endl;
  // }
  
  __initialized__ = true;
}


void KnikEngine::createRoutingGrid ( unsigned   nbXTiles
                                   , unsigned   nbYTiles
                                   , const Box& boundingBox
                                   , DbU::Unit  tileWidth
                                   , DbU::Unit  tileHeight
                                   , unsigned   hcapacity
                                   , unsigned   vcapacity )
{
    _routingGrid = RoutingGrid::create ( nbXTiles
                                       , nbYTiles
                                       , boundingBox
                                       , tileWidth
                                       , tileHeight
                                       , hcapacity
                                       , vcapacity ); 
}


void KnikEngine::createRoutingGraph()
// **********************************
{
    _routingGraph = Graph::create ( this, _routingGrid, _benchMode, _useSegments );
      
  //Breakpoint::stop ( 0, "Point d'arret:<br>&nbsp;&nbsp;<b>createGlobalGraph()</b>&nbsp;"
  //                      "after Knik createGlobalGraph()." );
}

void KnikEngine::addRoutingPadToGraph ( RoutingPad* routingPad )
// *************************************************************
{
    Vertex*  rpVertex = _routingGraph->getVertex ( routingPad->getCenter() );
    Contact* rpContact = rpVertex->getContact();
    if ( !rpContact || (rpContact->getNet() != routingPad->getNet()) ) {
        Contact* contact = Contact::create ( routingPad->getNet()
                                           , DataBase::getDB()->getTechnology()->getLayer("metal2")
                                           , DbU::lambda(floor(DbU::getLambda(rpVertex->getPosition().getX())))
                                           , DbU::lambda(floor(DbU::getLambda(rpVertex->getPosition().getY())))
                                           , rpVertex->getHalfWidth()/5
                                           , rpVertex->getHalfHeight()/5
                                           );
        rpVertex->setContact ( contact );
    }
}

Edge* KnikEngine::getEdge ( unsigned col1, unsigned row1, unsigned col2, unsigned row2 )
// *************************************************************************************
{
    return _routingGraph->getEdge ( col1, row1, col2, row2 );
}

void KnikEngine::updateEdgeCapacity ( unsigned col1, unsigned row1, unsigned col2, unsigned row2, unsigned capacity )
// ******************************************************************************************************************
{
    _routingGraph->UpdateEdgeCapacity ( col1, row1, col2, row2, capacity );
}

void KnikEngine::increaseEdgeCapacity ( unsigned col1, unsigned row1, unsigned col2, unsigned row2, int capacity )
// ***************************************************************************************************************
{
    _routingGraph->increaseEdgeCapacity ( col1, row1, col2, row2, capacity );
}

void KnikEngine::insertSegment ( Segment* segment )
// ************************************************
{
   _routingGraph->insertSegment ( segment ); 
}

Vertex* KnikEngine::getVertex ( Point position )
// *********************************************
{
   return _routingGraph->getVertex ( position ); 
}

Vertex* KnikEngine::getVertex ( DbU::Unit x, DbU::Unit y )
// *******************************************************
{
  return _routingGraph->getVertex ( x, y ); 
}


void KnikEngine::printTime()
// *************************
{
    ostringstream result;

    cmess2 << "           + Done in " << Timer::getStringTime(_timer.getCombTime()) 
           << " [+" << Timer::getStringMemory(_timer.getIncrease()) << "]." << endl;
    cmess2 << "             (raw measurements : " << _timer.getCombTime()
           << "s [+" << (_timer.getIncrease()>>10) <<  "Ko/"
           << (_timer.getMemorySize()>>10) << "Ko])" << endl;

    result.str("");
    result << _timer.getCombTime()
           << "s, +" << (_timer.getIncrease()>>10) <<  "Kb/"
           <<  Timer::getStringMemory(Timer::getMemorySize());
    cmess2 << Dots::asString( "     - Raw measurements", result.str() ) << endl;
}
// void KnikEngine::showEstimateOccupancy()
// // *************************************
// {
//     if ( _routingGraph )
//         _routingGraph->UpdateEstimateOccupancyWindow();
// }

inline bool netId_sort(Net* net1, Net* net2) { return NetExtension::getId(net1) < NetExtension::getId(net2); }

string KnikEngine::_getSolutionName() const
// ****************************************
{
  return getString(_cell->getName()) + ".kgr";
}

void KnikEngine::saveSolution ( const string& fileName )
// *****************************************************
{
    Name obstacleNetName ("obstaclenet");

    string saveFileName = fileName;
    if ( saveFileName.empty() )
      saveFileName = _getSolutionName();

    _nets_to_route.clear();
    vector<Net*> all_nets;
    forEach (Net*, net, _cell->getNets()) {
      if (   net->isGlobal()
         or  net->isSupply()
         or  net->isClock()
         or (net->getName() == obstacleNetName) 
         or not NetRoutingExtension::isAutomaticGlobalRoute(*net) ) continue;
      all_nets.push_back(*net);
    }
    stable_sort ( all_nets.begin(), all_nets.end(), netId_sort ); 

    CRL::IoFile saveStream ( saveFileName );
    saveStream.open ("w");
    FILE* saveFile = saveStream.getFile();
    if ( !saveFile )
       throw Error ("Cannot open solution file to write !");

    const Layer* gcontact = Configuration::getGContact();
    const Layer* gmetalh  = Configuration::getGMetalH();
    const Layer* gmetalv  = Configuration::getGMetalV();

    for ( size_t i=0 ; i<all_nets.size() ; ++i ) {
        Net*  net   = all_nets[i];
        long  netId = NetExtension::getId ( net );
      //assert ( netId >= 0 );

        vector<Contact*> viaContacts;
        forEach ( Contact*, icontact, net->getContacts() ) {
          if ( (icontact->getLayer() == gcontact) or (icontact->getLayer() == gmetalv) ) 
            viaContacts.push_back ( *icontact );
        }

        vector<Segment*> grSegments;
        forEach ( Segment*, isegment, net->getSegments() ) {
          if ( (isegment->getLayer() == gmetalh) or (isegment->getLayer() == gmetalv) ) {
            grSegments.push_back ( *isegment );
          }
        }

        unsigned nbEntries = grSegments.size() + viaContacts.size();
        fprintf ( saveFile, "%s %ld %d\n", getString(net->getName()).c_str(), netId, nbEntries );

        for ( size_t j=0 ; j<grSegments.size() ; ++j ) {
          unsigned layer = (dynamic_cast<Horizontal*>(grSegments[j]))? 1 : 2;
          fprintf ( saveFile, "(%d,%d,%d)-(%d,%d,%d)\n"
                  , (unsigned)DbU::getLambda(grSegments[j]->getSourceX())
                  , (unsigned)DbU::getLambda(grSegments[j]->getSourceY())
                  , layer
                  , (unsigned)DbU::getLambda(grSegments[j]->getTargetX())
                  , (unsigned)DbU::getLambda(grSegments[j]->getTargetY())
                  , layer
                  );

        //if ( layer == 2 ) { // pour rajouter les vias de descentes aux connecteurs
        //    if ( segment->getSource()->getLayer() == layerGMetalV ) {
        //        unsigned x = (unsigned)DbU::getLambda(segment->getSourceX());
        //        unsigned y = (unsigned)DbU::getLambda(segment->getSourceY());
        //        fprintf(saveFile, "(%d,%d,1)-(%d,%d,2)\n", x, y, x, y);
        //    }
        //    if ( segment->getTarget()->getLayer() == layerGMetalV ) {
        //        unsigned x = (unsigned)DbU::getLambda(segment->getTargetX());
        //        unsigned y = (unsigned)DbU::getLambda(segment->getTargetY());
        //        fprintf(saveFile, "(%d,%d,1)-(%d,%d,2)\n", x, y ,x, y);
        //    }
        //}
        }

        for ( size_t i=0 ; i<viaContacts.size() ; i++ ) {
          Contact* contact = viaContacts[i];
          fprintf ( saveFile, "(%d,%d,1)-(%d,%d,2)\n"
                  , (unsigned)DbU::getLambda(contact->getX())
                  , (unsigned)DbU::getLambda(contact->getY())
                  , (unsigned)DbU::getLambda(contact->getX())
                  , (unsigned)DbU::getLambda(contact->getY())
                  );
        }
        fprintf ( saveFile, "!\n" );
    }
    saveStream.close ();
}

void KnikEngine::getHorizontalCutLines ( vector<DbU::Unit>& horizontalCutLines )
// *****************************************************************************
{
    _routingGraph->getHorizontalCutLines ( horizontalCutLines );
}

void KnikEngine::getVerticalCutLines ( vector<DbU::Unit>& verticalCutLines )
// *************************************************************************
{
    _routingGraph->getVerticalCutLines ( verticalCutLines );
}

void KnikEngine::unrouteSelected()
// *******************************
{
//     static const Layer* layerGAlu2 = NULL;
//     static const Layer* layerGAlu3 = NULL;
//     static const Layer* layerGCont = NULL;
//     if ( !layerGAlu2 ) {
//         Technology* technology = DataBase::getDB()->getTechnology();
//         layerGAlu2 = technology->getLayer(Name("GALU2"));
//         layerGAlu3 = technology->getLayer(Name("GALU3"));
//         layerGCont = technology->getLayer(Name("GCONTACT"));
//     }

//     set<Segment*> segmentsToUnroute;
//     CEditor* editor = getCEditor ( getCell() );
//     if (editor->hasSomethingSelected() ) {
//         for_each_selector ( selector, editor->getSelectors() ) {
//             if ( Segment* segment = dynamic_cast<Segment*>(selector->getOccurrence().getEntity()) ) {
//                 Layer* layer = segment->getLayer();
//                 if ( (layer == layerGAlu2) || (layer == layerGAlu3) || (layer == layerGCont) ) {
//                     //cerr << "segment " << segment << " passes " << _routingGraph->getCongestEdgeNb(segment) << endl;
//                     segmentsToUnroute.insert ( segment );
//                 }
//             }
//             end_for;
//         }
//     }
    
//     UpdateSession::open();
//     // Previous call to recursive function
//     //while ( !segmentsToUnroute.empty() ) {
//     //    Segment* seg = (*segmentsToUnroute.begin());
//     //    unroute ( seg, segmentsToUnroute );

//     //}
//     do {
//         set<Contact*> contacts;
//         while ( !segmentsToUnroute.empty() ) {  // on parcourt tous les segments a derouter
//             Segment* segment = (*segmentsToUnroute.begin());
//             if ( !dynamic_cast<Contact*>(segment->getSource()) )     throw Error ( "unroute segment : segment's source is not a contact !" );
//             if ( !dynamic_cast<Contact*>(segment->getTarget()) )     throw Error ( "unroute segment : segment's target is not a contact !" );
//             // on insere les contacts source et target dans le set
//             contacts.insert ( static_cast<Contact*>(segment->getSource()) );
//             contacts.insert ( static_cast<Contact*>(segment->getTarget()) );
//             segmentsToUnroute.erase ( segment );  // suppression du segment dans le set
//             _routingGraph->removeSegment ( segment ); // il faut mettre a jour les occupations des edges dans le graph !
//             segment->getSourceHook()->detach();
//             segment->getTargetHook()->detach();
//             segment->destroy(); // delete du segment
//         }
//         for ( set<Contact*>::iterator cit = contacts.begin(); cit != contacts.end() ; cit++ ) { // pour chacun des contacts du set
//             Contact* contact = (*cit);
//             Vertex* contactVertex = _routingGraph->getVertex(contact->getCenter());
//             Hook* contactHook = contact->getBodyHook();
//             bool routingPadPresent = false;
//             Hook* currentHook = contactHook->getNextHook();
//             // on verifie si le contact est associe a un routingPad
//             while ( currentHook != contactHook ) {
//                 if ( dynamic_cast<RoutingPad*>(currentHook->getComponent()) ) {
//                     RoutingPad* rp = static_cast<RoutingPad*>(currentHook->getComponent());
//                     if ( _routingGraph->getVertex(rp->getCenter()) == contactVertex ) {
//                         routingPadPresent = true;
//                         break;
//                     }
//                 }
//                 currentHook = currentHook->getNextHook();
//             }
//             // si le contact est associe a un routingPad, on passe au contact suivant
//             if ( routingPadPresent ) continue;
//             unsigned nbSegments = contact->getSlaveComponents().getSize();
//             // sinon s'il a au moins 2 segments on passe au suivant aussi
//             if ( nbSegments >= 2 ) continue;
//             // sinon s'il n'a qu'un segment, on ajoutele segment au set des segments a derouter
//             if ( nbSegments == 1 ) {
//                 if ( !dynamic_cast<Segment*>(contact->getSlaveComponents().getFirst()) ) throw Error ("somthing else than a segment is on a contact ...");
//                 segmentsToUnroute.insert(static_cast<Segment*>(contact->getSlaveComponents().getFirst()));
//                 continue;
//             }
//             // sinon sile contact est "seul", on le delete, apres vérif tout de meme
//             if ( nbSegments == 0 ) {
//                 contactVertex->setContact( NULL ); // pour etre surqu'on ne pointe pas sur un objet efface
//                 contact->destroy();
//             }
//         }
//     } while ( !segmentsToUnroute.empty() );
//     UpdateSession::close();

//     editor->UnselectAll();
//     editor->Refresh();
}

string KnikEngine::adaptString ( string s )
// ****************************************
{
    string adapt = s;
    unsigned pos = adapt.find ( '<' );
    while ( pos < adapt.size() ) {
        adapt.replace ( pos, 1, "&lt;" );
        pos = adapt.find ( '<', pos );
    }
    pos = adapt.find ( '>' );
    while ( pos < adapt.size() ) {
        adapt.replace ( pos, 1, "&gt;" );
        pos = adapt.find ( '>', pos );
    }
    return adapt;
}

void KnikEngine::unrouteOvSegments()
// *********************************
{
   //cmess2 << "     o  Unroute overflowed segments :" << endl;
     unsigned countSegments = 0;
     unsigned countContacts = 0;
     UpdateSession::open();
     _timer.resume();
     do {
         set<Contact*> contacts;
         while ( !_segmentsToUnroute.empty() ) {  // on parcourt tous les segments a derouter
             Segment* segment = (*_segmentsToUnroute.begin());
             assert(segment);
             if ( !dynamic_cast<Contact*>(segment->getSource()) )
                 throw Error ( "unroute segment : segment's source is not a contact !" );
             if ( !dynamic_cast<Contact*>(segment->getTarget()) )
                 throw Error ( "unroute segment : segment's target is not a contact !" );
             // on insere les contacts source et target dans le set
             contacts.insert ( static_cast<Contact*>(segment->getSource()) );
             contacts.insert ( static_cast<Contact*>(segment->getTarget()) );
             _segmentsToUnroute.erase ( segment );  // suppression du segment dans le set
             _routingGraph->removeSegment ( segment ); // il faut mettre a jour les occupations des edges dans le graph !
             countSegments++;
             segment->getSourceHook()->detach();
             segment->getTargetHook()->detach();
             segment->destroy(); // delete du segment
         }
         for ( set<Contact*>::iterator cit = contacts.begin(); cit != contacts.end() ; cit++ ) { // pour chacun des contacts du set
             Contact* contact = (*cit);
             Vertex* contactVertex = _routingGraph->getVertex(contact->getCenter());
             Hook* contactHook = contact->getBodyHook();
             bool routingPadPresent = false;
             Hook* currentHook = contactHook->getNextHook();
             // on verifie si le contact est associe a un routingPad
             while ( currentHook != contactHook ) {
                 if ( dynamic_cast<RoutingPad*>(currentHook->getComponent()) ) {
                     RoutingPad* rp = static_cast<RoutingPad*>(currentHook->getComponent());
                     if ( _routingGraph->getVertex(rp->getCenter()) == contactVertex ) {
                         routingPadPresent = true;
                         break;
                     }
                 }
                 currentHook = currentHook->getNextHook();
             }
             // si le contact est associe a un routingPad, on passe au contact suivant
             if ( routingPadPresent ) continue;
             unsigned nbSegments = contact->getSlaveComponents().getSize();
             // sinon s'il a plus de 2 segments on passe au suivant aussi
             if ( nbSegments > 2 ) continue;
             // s'il y a 2 segments
             if ( nbSegments == 2 ) {
                 // s'ils sont de meme direction : on les fusionne
                 Segment* segment1 = NULL;
                 Segment* segment2 = NULL;
                 forEach ( Component*, component, contact->getSlaveComponents() ) {
                     if ( !dynamic_cast<Segment*>(*component) )
                         throw Error ("I did did see a pussy cat");
                     Segment* segment = static_cast<Segment*>(*component);
                     if ( !segment1 )
                         segment1 = segment;
                     else if ( !segment2 )
                         segment2 = segment;
                     else
                         throw Error ("Houston we've got a problem");
                 }
                 // on remove les 2 segments dans le graphe pour que les occupations et les références dans les edges soient correctes!
                 if ( dynamic_cast<Horizontal*>(segment1) && dynamic_cast<Horizontal*>(segment2) ) {
                     //cerr << "On remplace : " << segment1 << " et " << segment2 << " par:" << endl;
                     _routingGraph->removeSegment(segment1);
                     _routingGraph->removeSegment(segment2);
                     Horizontal* horiz1 = static_cast<Horizontal*>(segment1);
                     Horizontal* horiz2 = static_cast<Horizontal*>(segment2);
                     assert(horiz1->getNet() == horiz2->getNet());
                     Contact* toDel = NULL;
                     if ( horiz1->getSourceX() < horiz2->getSourceX() ) {
                         Contact* target = dynamic_cast<Contact*>(horiz2->getTarget());
                         toDel = dynamic_cast<Contact*>(horiz1->getTarget());
                         if (!target || !toDel)
                             throw Error ("Mummy help me !");
                         Hook* hook = horiz1->getTargetHook();
                         hook->detach();
                         hook->attach(target->getBodyHook());
                     }
                     else {
                         Contact* source = dynamic_cast<Contact*>(horiz2->getSource());
                         toDel = dynamic_cast<Contact*>(horiz1->getSource());
                         if (!source || !toDel)
                             throw Error ("It's not my fault");
                         Hook* hook = horiz1->getSourceHook();
                         hook->detach();
                         hook->attach(source->getBodyHook());
                     }
                     // il est tres important de verifier que le segment qui va etre implicitement detruit n'est pas présent dans _segmentsToUnroute !
                     set<Segment*>::iterator horiz2it = _segmentsToUnroute.find(horiz2);
                     if ( horiz2it != _segmentsToUnroute.end() ) {
                         //throw Error ("Implicity destroying a horizontal segment that is present in _segmentToUnroute !");
                         _segmentsToUnroute.erase(horiz2it); // doit-on rajouter horiz1 dans _segmentsToUnroute ?
                         _segmentsToUnroute.insert(horiz1); // oui puique horiz2 y était et que horiz1 remplace en partie horiz2
                     }
                     toDel->destroy(); // le segment horiz2 s'appuie sur le contact et il est donc destroy implicitement
                     //cerr << "  " << segment1 << endl;
                     // on rajoute le segment1 agrandi dans le graphe, toujours pour les edges A NE FAIRE QUE SI LES SEG SONT DE MEME TYPE
                     _routingGraph->insertSegment(segment1);
                 }
                 else if ( dynamic_cast<Vertical*>(segment1) && dynamic_cast<Vertical*>(segment2) ) {
                     //cerr << "On remplace : " << segment1 << " et " << segment2 << " par:" << endl;
                     _routingGraph->removeSegment(segment1);
                     _routingGraph->removeSegment(segment2);
                     Vertical* verti1 = static_cast<Vertical*>(segment1);
                     Vertical* verti2 = static_cast<Vertical*>(segment2);
                     assert(verti1->getNet() == verti2->getNet());
                     Contact* toDel = NULL;
                     if ( verti1->getSourceY() < verti2->getSourceY() ) {
                         Contact* target = dynamic_cast<Contact*>(verti2->getTarget());
                         toDel  = dynamic_cast<Contact*>(verti1->getTarget());
                         if (!target || !toDel)
                             throw Error ("Not again!");
                         Hook* hook = verti1->getTargetHook();
                         hook->detach();
                         hook->attach(target->getBodyHook());
                     }
                     else {
                         Contact* source = dynamic_cast<Contact*>(verti2->getSource());
                         toDel = dynamic_cast<Contact*>(verti1->getSource());
                         if (!source || !toDel)
                             throw Error ("Please reboot computer ... or not.");
                         Hook* hook = verti1->getSourceHook();
                         hook->detach();
                         hook->attach(source->getBodyHook());
                     }
                     // il est tres important de verifier que le segment qui va etre implicitement detruit n'est pas présent dans _segmentsToUnroute !
                     set<Segment*>::iterator verti2it = _segmentsToUnroute.find(verti2);
                     if ( verti2it != _segmentsToUnroute.end() ) {
                         //throw Error ("Implicity destroying a vertical segment that is present in _segmentToUnroute !");
                         _segmentsToUnroute.erase(verti2it);
                         _segmentsToUnroute.insert(verti1);
                     }
                     toDel->destroy(); // le segment verti2 s'appuie sur le contact et il est donc destroy implicitement
                     //cerr << "  " << segment1 << endl;
                     // on rajoute le segment1 agrandi dans le graphe, toujours pour les edges A NE FAIRE QUE SI LES SEG SONT DE MEME TYPE
                     _routingGraph->insertSegment(segment1);
                 }
                 // sinon on continue
                 else
                     continue;
             }
             // sinon s'il n'a qu'un segment, on ajoute le segment au set des segments a derouter
             if ( nbSegments == 1 ) {
                 if ( !dynamic_cast<Segment*>(contact->getSlaveComponents().getFirst()) )
                     throw Error ("somthing else than a segment is on a contact ...");
                 _segmentsToUnroute.insert(static_cast<Segment*>(contact->getSlaveComponents().getFirst())); // XXX euh oui mais s'il est déjà présent dans le set ? il se passe quoi ? XXX
                 continue;
             }
             // sinon si le contact est "seul", on le delete, apres vérif tout de meme
             if ( nbSegments == 0 ) {
                 contactVertex->setContact( NULL ); // pour etre surqu'on ne pointe pas sur un objet efface
                 contact->destroy();
                 countContacts++;
             }
         }
     } while ( !_segmentsToUnroute.empty() );

     _timer.suspend();
   //cmess2 << "        + Done in " << _timer.getCombTime() 
   //       << "s [+" << Timer::getStringMemory(_timer.getIncrease()) << "]." << endl;

     UpdateSession::close();
   //cmess2 << "        - Segments destroyed : " << countSegments << endl
   //       << "        - Contacts destroyed : " << countContacts << endl;

     cmess2 << "                     Destroyed.  Segments: " << countSegments
            << "  Contacts:" << countContacts << endl;

}

void KnikEngine::unroute ( Segment* segment, set<Segment*> &segmentsToUnroute, Contact* fromContact )
// **************************************************************************************************
{
    bool deleteSource = false;
    bool deleteTarget = false;
    if ( !dynamic_cast<Contact*>(segment->getSource()) )
        throw Error ( "KnikEngine::unroute(): found a Segment not based on a Contact." );
    Contact* sourceContact = static_cast<Contact*>(segment->getSource());
    if ( sourceContact != fromContact ) {
        Hook* contactHook = sourceContact->getBodyHook();
        Hook* tempHook = contactHook->getNextHook();
        Segment* firstSeg = NULL;
        Segment* secondSeg = NULL;
        unsigned NbSeg = 0;
        bool  routingPadPresent = false;
        while ( tempHook != contactHook ) {
            if ( dynamic_cast<Segment*>(tempHook->getComponent()) ) {
                NbSeg++;
                if ( NbSeg > 2 )
                    break; // ca sert a rien de continuer on a au moins 3 elements
                if ( !firstSeg )
                    firstSeg = static_cast<Segment*>(tempHook->getComponent());
                else
                    secondSeg = static_cast<Segment*>(tempHook->getComponent());
            }
            if ( dynamic_cast<RoutingPad*>(tempHook->getComponent()) ) {
                routingPadPresent = true;
                break;
            }

            tempHook = tempHook->getNextHook();
        }
        if ( !routingPadPresent && (NbSeg == 2) ) { // on doit "unroute" l'autre segment aussi
            if ( firstSeg == segment )
                unroute ( secondSeg, segmentsToUnroute, sourceContact );
            else if ( secondSeg == segment )
                unroute ( firstSeg, segmentsToUnroute, sourceContact );
            else
                throw Error ("KnikEngine::unroute(): Wow how did I do that!");
            deleteSource = true;
        }
    }
    if ( !dynamic_cast<Contact*>(segment->getTarget()) )
        throw Error ( "KnikEngine::unroute(): found a Segment not based on a Contact." );
    Contact* targetContact = static_cast<Contact*>(segment->getTarget());
    if ( targetContact != fromContact ) {
        Hook* contactHook = targetContact->getBodyHook();
        Hook* tempHook = contactHook->getNextHook();
        Segment* firstSeg = NULL;
        Segment* secondSeg = NULL;
        unsigned NbSeg = 0;
        bool  routingPadPresent = false;
        while ( tempHook != contactHook ) {
            if ( Segment* seg = dynamic_cast<Segment*>(tempHook->getComponent()) ) {
                NbSeg++;
                if ( NbSeg > 2 )
                    break; // ca sert a rien de continuer on a au moins 3 �l�ments
                if ( !firstSeg )
                    firstSeg = seg;
                else
                    secondSeg = seg;
            }
            if ( dynamic_cast<RoutingPad*>(tempHook->getComponent()) ) {
                routingPadPresent = true;
                break;
            }

            tempHook = tempHook->getNextHook();
        }
        if ( !routingPadPresent && (NbSeg == 2) ) { // on doit "unroute" l'autre segment aussi
            if ( firstSeg == segment )
                unroute ( secondSeg, segmentsToUnroute, targetContact );
            else if ( secondSeg == segment )
                unroute ( firstSeg, segmentsToUnroute, targetContact );
            else
                throw Error ("KnikEngine::unroute(): Once again, how did I do that!");
            deleteTarget = true;
        }
    }
    segmentsToUnroute.erase ( segment );
    _routingGraph->removeSegment ( segment ); // il faut mettre a jour les occupations des edges dans le graph !
    segment->getSourceHook()->detach();
    segment->getTargetHook()->detach();
    segment->destroy();
    if ( deleteSource )
        sourceContact->destroy();
    if ( deleteTarget )
        targetContact->destroy();
}

void KnikEngine::computeOverflow()
// *******************************
{
  //cmess1 << "     o  Computing Statistics" << endl;
    Vertex* currentVertex = _routingGraph->getLowerLeftVertex();
    int   nbEdgesTotal = 0;
    int   nbEdgesOver  = 0;

    unsigned overflow  = 0;
    unsigned maxOver   = 0;
    //float maxOver      = 0;
    //float averageOver  = 0;
    while ( currentVertex ) {
        Vertex* firstLineVertex = currentVertex;
        while ( currentVertex ) {
            Edge* hEdgeOut = currentVertex->getHEdgeOut();
            if ( hEdgeOut ) {
                nbEdgesTotal++;
                int ov = 2*(hEdgeOut->getRealOccupancy() - hEdgeOut->getCapacity()); // 2 = minimum spacing + minimum width
                if ( ov > 0 ) {
                    nbEdgesOver++;
                    overflow += ov;
                    maxOver = (unsigned)ov > maxOver ? ov : maxOver;
                }
                for_each_segment ( segment, hEdgeOut->getSegments() ) {
                    map<Segment*,SegRecord>::iterator ssit = _segmentOverEdges.find ( segment );
                    SegRecord record;
                    if ( ssit != _segmentOverEdges.end() )
                        record = (*ssit).second;
                    else
                        record = SegRecord();

                    record.incNbTotEdges();
                    if ( ov > 0 ) {
                        record.incNbOvEdges();
                        record.incSumOv ( ov );
                        record.UpdateMaxOv ( ov );
                    }
                    end_for;
                }
            }
            if ( Edge* vEdgeOut = currentVertex->getVEdgeOut() ) {
                nbEdgesTotal++;
                int ov = 2*(vEdgeOut->getRealOccupancy() - vEdgeOut->getCapacity());
                if ( ov > 0 ) {
                    nbEdgesOver++;
                    overflow += ov;
                    maxOver = (unsigned)ov > maxOver ? ov : maxOver;
                }
                for_each_segment ( segment, vEdgeOut->getSegments() ) {
                    map<Segment*,SegRecord>::iterator ssit = _segmentOverEdges.find ( segment );
                    SegRecord record;
                    if ( ssit != _segmentOverEdges.end() )
                        record = (*ssit).second;
                    else
                        record = SegRecord();

                    record.incNbTotEdges();
                    if ( ov > 0 ) {
                        record.incNbOvEdges();
                        record.incSumOv ( ov );
                        record.UpdateMaxOv ( ov );
                    }
                    end_for;
                }
            }

            if ( hEdgeOut )
                currentVertex = hEdgeOut->getOpposite ( currentVertex );
            else
                break;
        }
        Edge* vEdgeOut = firstLineVertex->getVEdgeOut();
        if ( vEdgeOut )
            currentVertex = vEdgeOut->getOpposite ( firstLineVertex );
        else
            break;
    }
    //cmess2 << "        - first skimming through edges done (overflow computed)" << endl;
    //averageOver = nbEdgesOver == 0 ? 0 : averageOver / (float)nbEdgesOver;
    
    // Now we've got the max we can print more detailed statistics about edges overflow
    //    except if there is no overflow...
    vector<unsigned> ovEdgesStats;
    if ( maxOver ) {
        Vertex* currentVertex = _routingGraph->getLowerLeftVertex();
        unsigned maxOvIdx = maxOver / (int)(10);
        ovEdgesStats.resize(maxOvIdx+1);
        while ( currentVertex ) {
            Vertex* firstLineVertex = currentVertex;
            while ( currentVertex ) {
                Edge* hEdgeOut = currentVertex->getHEdgeOut();
                if ( hEdgeOut ) {
                    int ov = 2*(hEdgeOut->getRealOccupancy() - hEdgeOut->getCapacity()); // 2 = minimum sapcing + minimum width
                    if ( ov > 0 ) {
                        unsigned ovIdx = ov / int(10);
                        ovEdgesStats[ovIdx]++;
                    }
                }
                if ( Edge* vEdgeOut = currentVertex->getVEdgeOut() ) {
                    int ov = 2*(vEdgeOut->getRealOccupancy() - vEdgeOut->getCapacity());
                    if ( ov > 0 ) {
                        unsigned ovIdx = ov / int(10);
                        ovEdgesStats[ovIdx]++;
                    }
                }

                if ( hEdgeOut )
                    currentVertex = hEdgeOut->getOpposite ( currentVertex );
                else
                    break;
            }
            Edge* vEdgeOut = firstLineVertex->getVEdgeOut();
            if ( vEdgeOut )
                currentVertex = vEdgeOut->getOpposite ( firstLineVertex );
            else
                break;
        }
      //cmess2 << "        - second skimming through edges done (overflow details)" << endl;
    }

    unsigned _wirelength          = 0;
    unsigned _gridWirelength      = 0;
    unsigned _gridWirelengthWoVia = 0;
    Layer* layerContact = DataBase::getDB()->getTechnology()->getLayer(Name("GCONTACT"));
    //for ( unsigned i = 0 ; i < _nets_to_route.size() ; i++ ) {
    //    Net* net = _nets_to_route[i]._net;
    forEach ( Net*, net, _cell->getNets() ) {
        unsigned netWirelength = 0;
        for_each_segment ( segment, net->getSegments() ) {
            _wirelength += (unsigned)DbU::getLambda ( segment->getLength() );
            unsigned gridWirelength = _routingGraph->getGridLength ( segment );
            _gridWirelength      += gridWirelength;
            _gridWirelengthWoVia += gridWirelength;
            netWirelength        += gridWirelength;
            end_for;
        }
        for_each_contact ( contact, net->getContacts() ) {
            if ( contact->getLayer() == layerContact ) {
                _gridWirelength += 3;
                netWirelength += 3;
            }
            end_for; 
        }
        NetExtension::setWireLength ( *net, netWirelength );
    }
  //cmess2 << "        - Wirelength computed" << endl;
    // *** Cannot build column several times, no hasColumn function ***
//     CEditor* editor = getCEditor ( getCell() );
//     CNetListWindow* netListWindow = editor->getNetListWindow();
//     if ( ! netListWindow->hasColumn ( "GridWirelength" ) ) {
//         netListWindow->addColumn ( "GridWirelength", getNetWirelength, NetExtension::compare, CListWidget::RightJustify );
//         netListWindow->Rebuild();
//     }
//    cmess2 << "        - Netlist window rebuild" << endl;
//    cmess1 << "        - Total number of edges : "   << nbEdgesTotal << endl
//     cmess1 << "        - Number of overcapacity edges : " << nbEdgesOver << " / " << nbEdgesTotal << endl
//          //<< "        - Total calls to Dijkstra : " << countDijkstra << endl
//          //<< "        - Total calls to Monotonic : " << countMonotonic << endl
//          //<< "        - Total calls to Materialize : " << countMaterialize << endl
//          //<< "        - Taille du Graphe de routage : " << _xSize << " x " << _ySize << endl
//            << "        - # of overflow : "           << overflow               << endl
//            << "        - max of overflow : "         << maxOver                << endl;
// //         << "    - # of net with overflow : "  << _netNbOverEdges.size() << endl
//     cmess1 << "        - grid wirelength : "         << _gridWirelength        << endl;
//     cmess2 << "        - grid wirelength : "         << _gridWirelength        << endl
//            << "        - grid wirelength w/o via : " << _gridWirelengthWoVia   << endl
//            << "        - real wirelength : "         << _wirelength            << endl;

    //if ( ! ovEdgesStats.empty() ) {
    //    // print details about edges overflow
    //    cmess2 << endl
    //           << "        - Number of edges for overflow interval :" << endl;
    //    for ( unsigned i = 0 ; i < ovEdgesStats.size() ; i++ ) {
    //        unsigned first = i*10;
    //        unsigned second = (i+1)*10 > maxOver ? maxOver : (i+1)*10;
    //        cmess2 << "           - " << first << " - " << second << " : " << ovEdgesStats[i] << endl;
    //    }
    //}

    return;
}

// void KnikEngine::showOccupancy()
// // *****************************
// {
//     if ( _routingGraph )
//         _routingGraph->UpdateOccupancyWindow();
// }

void KnikEngine::run( const map<Name,Net*>& excludedNets )
// *******************************************************
{
    Route( excludedNets );
    bool done = analyseRouting();
    while ( !done ) {
        unrouteOvSegments();
        reroute();
        done = analyseRouting();
    }

    ostringstream result;

    result <<  Timer::getStringTime(_timer.getCombTime()) 
           << ", " << Timer::getStringMemory(_timer.getIncrease());
    cmess1 << "  o  Global Routing Completed." << endl;
    cmess1 << Dots::asString( "     - Done in", result.str() ) << endl;

    result.str("");
    result << _timer.getCombTime()
           << "s, +" << (_timer.getIncrease()>>10) <<  "Kb/"
           <<  Timer::getStringMemory(Timer::getMemorySize());
    cmess2 << Dots::asString( "     - Raw measurements", result.str() ) << endl;

    addMeasure<double> ( getCell(), "knikT",  _timer.getCombTime  () );
    addMeasure<size_t> ( getCell(), "knikS", (_timer.getMemorySize() >> 20) );

    computeSymbolicWireLength ();
}

void KnikEngine::Route( const map<Name,Net*>& excludedNets )
// *********************************************************
{
    UpdateSession::open();
    if ( !__initialized__ )
        initGlobalRouting( excludedNets );

    _timer.resetIncrease();
    _timer.start();

    cmess1 << "  o  Global Routing." << endl;
    cmess2 << "     Iteration INIT"
           <<    "  # of nets to route:" << left  << _nets_to_route.size() << endl;

    // initializing netStamp for routingGraph:
    //_routingGraph->setNetStamp(1); // Maybe NetStamp should not be initialized here !
    // Be aware that initializingthe NetStamp in the construction of the routingGraph, might be a bad idea, if a lotof rerouting processes are run, it may overpass the unsigne limit (really ?)

    Name nameDebug ("ck_dpt");
    unsigned size = _nets_to_route.size(); 
    for ( unsigned i = 0 ; i < size ; i++ ) {
        Net* net = _nets_to_route[i]._net;
        assert ( net );

        //_routingGraph->checkGraphConsistency();
        switch ( _routingGraph->initRouting ( net ) ) {
            case 0:
            case 1:
                //cerr << "Nothing to global route" << endl;
                break;
            case 2:
                //_routingGraph->Monotonic();
                //break;
            default:
                _routingGraph->Dijkstra();
                break;
        }
        
        _routingGraph->incNetStamp();
        _routingGraph->CleanRoutingState();
        //cmess1 << "        - [";
        //cmess1.width(3);
        //cmess1 << floor((float)(i*100)/(float)(size));
        //cmess1 << "%]\r";
    }
    _timer.suspend();

    cmess2 << "                     Elapsed time: " << _timer.getCombTime() 
           << "  Memory: " << Timer::getStringMemory(_timer.getIncrease()) << endl;

    // Comment to test with transhierarchic MIPS
    //computeOverflow();

    // To be able to plot congestionMap, we need to UpdateMaxEstimateCongestion :
    //_routingGraph->UpdateMaxEstimateCongestion(); // no more useful since qe use QT which allow to see colored edges.
    
    // passage en mode PERFORMANCE !
    //_routingGraph->testSTuplePQ();

    // While not debugging, comment this out :
    //_routingGraph->destroy();
    UpdateSession::close();

    _routingDone = true;
}

bool KnikEngine::analyseRouting()
// ******************************
{
    //computeOverflow();
    if (_routingDone)
        _timer.resume();
    else {
        _timer.resetIncrease();
        _timer.start();
    }
    
    unsigned overflow = _routingGraph->analyseRouting (_segmentsToUnroute);
  //cmess2 << "        - Segments to unroute : " << _segmentsToUnroute.size() << endl;

    // redefine the new _nets_to_route vector
    _nets_to_route.clear();

    for ( set<Segment*>::iterator it = _segmentsToUnroute.begin() ; it != _segmentsToUnroute.end() ; it++ ) {
    //cmess2 << "           "<< (*it) << endl;
        Net* net = (*it)->getNet();

        bool present = false;
        for ( unsigned i = 0 ; i < _nets_to_route.size() ; i++ ) {
            if ( _nets_to_route[i]._net == net ) {
                present = true;
                break;
            }
        } 
        if ( !present ) {
            Box bbox = net->getBoundingBox();
            NetRecord record ( net, (long int)((DbU::getLambda(bbox.getWidth())+1)*(DbU::getLambda(bbox.getHeight())+1)) );
            _nets_to_route.push_back ( record );
        }
    }

    // Il est nécessaire de retrier les nets à rerouter de façon uqe le Dijkstra soit optimisé
    stable_sort ( _nets_to_route.begin(), _nets_to_route.end(), NetSurfacesComp() );

    NetVector::iterator new_end = unique ( _nets_to_route.begin(), _nets_to_route.end() );
    _nets_to_route.erase ( new_end, _nets_to_route.end() );

    //cmess1 << "        - Nets to reroute : " << _nets_to_route.size() << endl;
    //cmess1 << "           ";
    //for ( unsigned i = 0 ; i < _nets_to_route.size() ; i++ )
    //    cmess1 << _nets_to_route[i]._net->getName() << "|";
    //cmess1 << endl;

    _timer.suspend();
    // cmess1 << "        + Done in " << _timer.getCombTime() 
    //        << "s [+" << Timer::getStringMemory(_timer.getIncrease()) << "]." << endl;

    bool done = false;
    if ( (overflow==0) || (_timer.getCombTime() >= MAX_RUNTIME) || (_rerouteIteration >= MAX_ITERATION) )
        done = true;
    return done;
}

void KnikEngine::reroute()
// ***********************
{
    UpdateSession::open();

  //Breakpoint::setStopLevel(1);
  //analyseRouting();
  //unrouteOvSegments();

    cmess2 << "     Iteration "     << right << setw(4) << setfill('0') << ++_rerouteIteration
           <<    "  # of nets to route:" << left  << _nets_to_route.size() << endl;

    _timer.resume();

    unsigned int size = _nets_to_route.size(); 
    __ripupMode__ = true;

    for ( unsigned i = 0 ; i < size ; ++i ) {
      Net* net = _nets_to_route[i]._net;
      assert( net );
    //_routingGraph->checkGraphConsistency();

      switch ( _routingGraph->initRouting(net) ) {
        case 0:
        case 1:
        //cerr << "Nothing to global route" << endl;
          break;
        default:
          _routingGraph->Dijkstra();
          break;
      }
        
      _routingGraph->incNetStamp();
      _routingGraph->CleanRoutingState();
    }

    _timer.suspend();

    cmess2 << "                     Elapsed time: " << _timer.getCombTime() 
           << "  Memory: " << Timer::getStringMemory(_timer.getIncrease()) << endl;

    // cmess2 << "        + Done in " << _timer.getCombTime() 
    //        << " [+" << Timer::getStringMemory(_timer.getIncrease()) << "]." << endl;
    // cmess2 << "          (raw measurements : " << _timer.getCombTime()
    //        << "s [+" << (_timer.getIncrease()>>10) <<  "Ko/"
    //        << (_timer.getMemorySize()>>10) << "Ko])" << endl;

    // Comment to test with transhierarchic MIPS
    //computeOverflow();

    // While not debugging, comment this out :
    //_routingGraph->destroy();
    UpdateSession::close();

}


void  KnikEngine::computeSymbolicWireLength ()
{
  if (not _routingGauge)
    throw Error( "KnikEngine::computeSymbolicWireLength(): The routing gauge has not been set." );

  size_t     hEdgeCapacity = 0;
  size_t     vEdgeCapacity = 0;
  DbU::Unit  gcellSide     = DbU::fromLambda( 50.0 );

  vector<RoutingLayerGauge*>::const_iterator ilayerGauge = _routingGauge->getLayerGauges().begin();
  for ( ; ilayerGauge != _routingGauge->getLayerGauges().end() ; ++ilayerGauge ) {
    RoutingLayerGauge* layerGauge = (*ilayerGauge);
    if (layerGauge->getType() != Constant::Default) continue;
    if (layerGauge->getDepth() > _allowedDepth)     continue;

    if (layerGauge->getDirection() == Constant::Horizontal) {
      hEdgeCapacity += layerGauge->getTrackNumber( 0, gcellSide ) - 1;
    } else if ( layerGauge->getDirection() == Constant::Vertical ) {
      vEdgeCapacity += layerGauge->getTrackNumber( 0, gcellSide ) - 1;
    }
  }

// Complete formula: unitarian Wirelength/Area for one GCell.
//   (side*(hEdgeCapacity+vEdgeCapacity)) / (side * side).
  const double       normalize          = ((double)(hEdgeCapacity+vEdgeCapacity)) / DbU::toLambda(gcellSide);
  unsigned long long symbolicWireLength = 0;

  forEach ( Net*, net, getCell()->getNets() ) {
    if (   net->isGlobal()
       or  net->isSupply()
       or  net->isClock() ) {
      continue;
    }

    forEach ( Segment*, isegment, net->getSegments() ) {
      symbolicWireLength += (unsigned long long)DbU::getLambda ( isegment->getLength() );
    }
  }

  addMeasure<unsigned long long> ( getCell(), "GWL(l)", symbolicWireLength, 14 );

  Box    ab   ( getCell()->getAbutmentBox() );
  double area = (DbU::getLambda(ab.getWidth()) * DbU::getLambda(ab.getHeight()) );

  addMeasure<double> ( getCell(), "Area(l2)", area, 14 );
  addMeasure<double> ( getCell(), "Sat."    , (symbolicWireLength/area)/normalize );
}


Record* KnikEngine::_getRecord() const
// *****************************
{
    Record* record = Inherit::_getRecord();

    if ( !record )
        record = new Record ( getString ( this ) );

    record->add ( getSlot ( "RoutingGraph", _routingGraph  ) );

    return record;
}

KnikEngine* KnikEngine::get (const Cell* cell )
// ********************************************
{
  return ( dynamic_cast<KnikEngine*> ( ToolEngine::get ( cell, KnikEngine::staticGetName() ) ) );
}

} // namespace Knik


// *********************************************************************
// Copyright (C) UPMC/LIP6/ASIM 2003-2004 All rights reserved
// *********************************************************************
