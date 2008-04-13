// ****************************************************************************************************
// File: TrMos.cpp
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************


#include "TrMos.h"

#include "Instances.h"
#include "MetaTransistor.h"
#include "Net.h"
#include "Transistors.h"
#include "Box.h"
#include "UpdateSession.h"
#include "HyperNet.h"
#include "DataBase.h"
#include "Technology.h"
#include "Vertical.h"
#include "Horizontal.h"
#include "Pin.h"

#include "RdsUnit.h"
#include "Transistor.h"
#include "DtrAccess.h"
using namespace Hurricane;



#include "DeviceUtil.h"


namespace DEVICE{


// ****************************************************************************************************
// TrMos implementation
// ****************************************************************************************************
void TrMos::_PlaceAndRoute()
// *************************
{

   // get Dtr Rules And Calculate the Size of AbutmentBox of Device.
   // **************************************************************
   DtrAccess * dtraccess = DtrAccess::getDtrAccess(); 

   Transistor::Polarity polarity;
   if(_polarity == Transistor::Polarity::P) polarity = Transistor::Polarity::N; 
   else polarity = Transistor::Polarity::P;

   long minImpWidth       = dtraccess->getSingleRdsRuleByLabel("RW_", getString(polarity), "IMP"); 
   long minContWidth      = dtraccess->getSingleRdsRuleByLabel(string("RW_CONT")); 
   long minAlu1Width      = dtraccess->getSingleRdsRuleByLabel(string("RW_ALU1")); 
   long minVia1Width      = dtraccess->getSingleRdsRuleByLabel(string("RW_VIA1")); 

   long rdImp             = dtraccess->getSingleRdsRuleByLabel(string("RD_NIMP")); 
   long rdActive          = dtraccess->getSingleRdsRuleByLabel(string("RD_ACTI")); 
   long rdAlu2            = dtraccess->getSingleRdsRuleByLabel(string("RD_ALU1")); 

   long reImpActi         = dtraccess->getSingleRdsRuleByLabel("RE_", getString(polarity), "IMP_CONT"); 
   long reActiContact     = dtraccess->getSingleRdsRuleByLabel("RE_ACTI_CONT"); 
   long reAlu1Contact     = dtraccess->getSingleRdsRuleByLabel("RE_ALU1_CONT"); 
   long reAlu1Via1        = dtraccess->getSingleRdsRuleByLabel("RE_ALU1_VIA1"); 

   long minActiWidth      = 2*reActiContact + minContWidth; 

   long widthOfSourceWire = ConvertRealToRdsUnit(_widthOfSourceWire);   
   long widthOfDrainWire  = ConvertRealToRdsUnit(_widthOfDrainWire);

   long widthOfActive     = MAX_INTEGER(minActiWidth, minContWidth + 2*reActiContact);
   long widthOfImp        = MAX_INTEGER(widthOfActive + 2*reImpActi, minImpWidth);


   // **************************************************************
   // Placing .
   // **************************************************************

   Transformation::Orientation::Code internalTransCode = Transformation::Orientation::ID;   

   Unit horizontalMargin   = 0;      // the horizontal margin of trmos. 
   Unit verticalLowMargin  = 0;      // the vertical low margin of trmos. 
   Unit verticalHighMargin = 0;      // the vertical high margin of trmos. 

   Unit fingerHeight = 0;          // the height of trmos. 
   Instance * leftins = NULL; 
   Instance * rightins = NULL;

   OccurrenceLocator locator =  getLeafInstanceOccurrences().getLocator();  
   Instance * instance = dynamic_cast<Instance*>(locator.getElement().getEntity());;  
   fingerHeight        = instance->getCell()->getBoundingBox().getHeight();
   horizontalMargin    = getUnit(RETURN_EVEN((long)(getValue(fingerHeight))/4));
   verticalLowMargin   = getUnit(RETURN_EVEN((long)(getValue(fingerHeight))/2));
   verticalHighMargin  = getUnit(RETURN_EVEN((long)(getValue(fingerHeight))/2));


   verticalLowMargin   = MAX_INTEGER(verticalLowMargin, getUnit(RETURN_EVEN(rdImp + widthOfImp/2 + widthOfSourceWire 
	                + rdAlu2 + widthOfActive + rdActive)) );

   verticalHighMargin  = MAX_INTEGER(verticalHighMargin, horizontalMargin + getUnit(2*rdAlu2 + 2*widthOfDrainWire) );
   horizontalMargin    = MAX_INTEGER(horizontalMargin, getUnit(RETURN_EVEN(rdImp + widthOfImp/2)) );

   openUpdateSession();

   if(_m ==  1 ) {  // If there is only one finger.
     _Place( instance, Transformation::Orientation::ID, Point( horizontalMargin, verticalLowMargin ) );
     //CloseUpdateSession();
     //return;

   }
   else {   
     // get instance who's model's abutment type is Left or Right.
     // ************************************************************
     for_each_occurrence(occurrence, getLeafInstanceOccurrences())
        instance = dynamic_cast<Instance*>(occurrence.getEntity());
        Transistor * trans = dynamic_cast<Transistor*>(instance->getMasterCell());

        if ( _sourceIsFirst ) {
          if(trans->isLeft() && !leftins)
             leftins = instance;
          else if ( trans->isLeft() && leftins)
             rightins = instance;
          else if ( trans->isRight())
             rightins = instance;
        }
        else {
          if(trans->isRight() && !leftins)
            leftins = instance;
          else if (trans->isRight() && leftins )
            rightins = instance;
          else if (trans->isLeft())
            rightins = instance;
        }
     end_for

     // You must place this first instance who's model is left finger in a point who's 
     // x, y are all pair. 
     // Because if you do this, you can be sure that all rectangle of this instance are
     // correctly in the grille of fondor.
     // ***********************************************************************************
     
     if(_sourceIsFirst)
       _Place( leftins, Transformation::Orientation::ID, Point(horizontalMargin, verticalLowMargin) );
     else
       _Place( leftins, Transformation::Orientation::MX, Point(horizontalMargin, verticalLowMargin) );

     _setRefIns(leftins);

     if(_sourceIsFirst)  // internal Finger's transformation. 
        internalTransCode = Transformation::Orientation::MX; 
     else
        internalTransCode = Transformation::Orientation::ID;

     // Place internal finger.
     // *********************
     for_each_occurrence(occurrence, getLeafInstanceOccurrences())

        Instance * instance = dynamic_cast<Instance*>(occurrence.getEntity());

        if(instance==leftins || instance==rightins )
          continue;

        _PlaceRight( instance, internalTransCode);
        
        if(internalTransCode == Transformation::Orientation::MX)
          internalTransCode = Transformation::Orientation::ID;
        else
          internalTransCode = Transformation::Orientation::MX;

     end_for

     // Place the last finger.
     // **********************
     Transistor * trans = dynamic_cast<Transistor*>(rightins->getMasterCell());

     if( trans->isRight())
       _PlaceRight( rightins, Transformation::Orientation::ID);
     else 
       _PlaceRight( rightins, Transformation::Orientation::MX);
     
   }  

   closeUpdateSession();



   // Set AbutmentBox.
   // ****************
   for_each_instance(instance, getInstances())
    instance->unmaterialize();      
    instance->materialize();      
   end_for

   openUpdateSession();


   cout <<"Bounding box of TrMos is "<<getString(getBoundingBox())<<endl;

   setAbutmentBox(Box(0, 0, 
		      getBoundingBox().getWidth()  + 2*horizontalMargin,
		      getBoundingBox().getHeight() + verticalLowMargin + verticalHighMargin
		      )
                 ); 


   // **************************************************************
   // Routing .
   // **************************************************************

   Unit expectedInterval  = getUnit(RETURN_EVEN((long)(getValue(horizontalMargin))/2)); 
   Unit interval          = 0;
   Unit initialPosition   = verticalLowMargin + fingerHeight + getUnit(rdAlu2 + widthOfDrainWire/2); 

   map<string, Unit> netName2PositionOfConnectorMap; 
   list<Box> routingZoneList;    
   list<Unit> sourcePositionList;
   list<Unit> drainPositionList;
   Unit sourceRoutingZoneWidth;
   Unit drainRoutingZoneWidth;
   
   DataBase * db = getDataBase();  
    if(!db) throw Error("Can't launch Trmos::PlaceAndRoute for " + getString(this) + " : can't find DataBase");

   Layer * layerAlu1    = db->getTechnology()->getLayer(Name("ALU1"));
   Layer * layerAlu2    = db->getTechnology()->getLayer(Name("ALU2"));

   Layer * layerVia1    = db->getTechnology()->getLayer(Name("VIA1"));
   Layer * layerActive  = db->getTechnology()->getLayer(Name("ACTIVE"));

   Layer * layerVia01   = db->getTechnology()->getLayer(Name("via01"));
   Layer * layerVia12   = db->getTechnology()->getLayer(Name("via12"));

   Layer * layerPoly    = db->getTechnology()->getLayer(Name("POLY"));
   Layer * layerNwell   = db->getTechnology()->getLayer(Name("NWELL"));
   Layer * layerCont    = db->getTechnology()->getLayer(Name("CONT"));

   Layer * layerImp = NULL;
   
   if(_polarity == Transistor::Polarity::P)  
     layerImp  = db->getTechnology()->getLayer(Name("NIMP"));
   else 
     layerImp  = db->getTechnology()->getLayer(Name("PIMP"));

   Pin * pin = NULL;        // For get the adresse of Created Pins. 

   long connectorPosition = 0; 


   cout << " Begin routage " << endl;


   // Set position of four connectors.
   // ********************************
   vector<PinName>::iterator i = _highPinOrder.begin(), 
      j = _highPinOrder.end();
   
   while(i!=j) {
     if(*i == D)
       netName2PositionOfConnectorMap[string("drain")] = initialPosition;   
     if(*i == G)
       netName2PositionOfConnectorMap[string("grid")] = initialPosition; 

     interval = MAX_INTEGER(expectedInterval, getUnit(widthOfDrainWire + rdAlu2));

     // initialPosition += horizontalMargin/2;
     initialPosition += interval;
     i++;
   }

   vector<PinName>::iterator m = _lowPinOrder.begin(), 
      n = _lowPinOrder.end();
   
   //initialPosition = verticalMargin - horizontalMargin/2; 
   //initialPosition = verticalLowMargin - MAX_INTEGER(expectedInterval, getUnit(rdImp + widthOfImp/2)); 
   initialPosition = verticalLowMargin - getUnit(rdImp + widthOfImp/2); 

   while(m!=n) {
     if(*m == S)
       netName2PositionOfConnectorMap[string("source")] = initialPosition;   
     if(*m == B)
       netName2PositionOfConnectorMap[string("bulk")] = initialPosition; 

     interval = MAX_INTEGER(expectedInterval, getUnit(rdAlu2 + widthOfSourceWire));

     initialPosition -= interval;
     m++;
   }

   cout << " Main loop "<< endl;

   // Main Routing Algorithm.
   // ***********************

   // Main Loop.
   // **********
   for_each_net(net, getNets())          // For all hypernets.

      if(getString(net->getName())=="bulk" || getString(net->getName())=="BULK"  )
	 continue;

      // get Routing Zone. 
      // *****************
      HyperNet hyperNet(Occurrence(net, Path()));    
      for_each_occurrence(occurrence, hyperNet.getNetOccurrences())  // For all net occurrences.
         Net * net = dynamic_cast<Net*>(occurrence.getEntity());
         Box routingZone;

         if(net->getCell()->isLeaf()) {
	   Transistor * trans = dynamic_cast<Transistor*>(net->getCell());
	   if ( !trans )
	      throw Error("Can't launch Trmos::PlaceAndRoute for " + getString(this) 
		  + ", it is not a Transistor");
	   
           cout << getString(occurrence) << endl;
	   cout << getString(occurrence.getPath().getTransformation()) <<endl;
	   cout << getString((*(trans->_getMapNet2Box()))[net]) << endl;

           // get Routing Zone.
	   // *****************
	   routingZone = occurrence.getPath().getTransformation().getBox((*(trans->_getMapNet2Box()))[net]);  
	   routingZoneList.push_back(routingZone);  

           if(getString(net->getName())=="SOURCE") {
             sourcePositionList.push_back(routingZone.getXCenter());         
      	     sourceRoutingZoneWidth = routingZone.getWidth();
      	   }
       	   else if (getString(net->getName())=="DRAIN") {
             drainPositionList.push_back(routingZone.getXCenter());         
             drainRoutingZoneWidth = routingZone.getWidth(); 
           }
	 }

      end_for


      cout <<"Print routing zone for " <<getString(net)<<endl;

      list<Box>::iterator it_begin_listbox = routingZoneList.begin(), 
	   it_end_listbox = routingZoneList.end();

      while(it_begin_listbox != it_end_listbox)
      {
         cout<< getString(*it_begin_listbox) <<endl;	
         it_begin_listbox++;
      }

      cout <<"End Print Routing Zone for "<<getString(net)<<endl;

      // Create routing line.
      // ********************
      list<Box>::iterator routingzonelist_begin_it = routingZoneList.begin(), 
	routingzonelist_end_it = routingZoneList.end();

      connectorPosition = netName2PositionOfConnectorMap[getString(net->getName())];
      cout << "Connector Position is " << netName2PositionOfConnectorMap[getString(net)] << endl;

      while(routingzonelist_begin_it!=routingzonelist_end_it) {

        Box routingZoneBox = *routingzonelist_begin_it;
	
        // Create vertical line and Contact.
        // ********************************
	if(connectorPosition > routingZoneBox.getYMin()) {
	   Vertical::create(net, layerAlu1, routingZoneBox.getXCenter()
	         , routingZoneBox.getWidth() + getUnit(2*reAlu1Contact)
	         , routingZoneBox.getYMin() - getUnit(reAlu1Contact)
	         , connectorPosition);        
	}   
	else  {
	   Vertical::create(net, layerAlu1, routingZoneBox.getXCenter()
	         , routingZoneBox.getWidth() + getUnit(2*reAlu1Contact)
		 , connectorPosition
		 , routingZoneBox.getYMax() + getUnit(reAlu1Contact) ) ;
        }

        Contact::create(net, layerVia12, routingZoneBox.getXCenter()
	      , connectorPosition
	      , getUnit(minVia1Width)
	      , getUnit(minVia1Width)
	    );

        routingzonelist_begin_it ++ ;
      } 
      
      // Create horizontal line.
      // ***********************
      long widthOfWire = 0;

      if(getString(net->getName())=="source")
	widthOfWire = widthOfSourceWire;
      else if(getString(net->getName())=="drain")
	widthOfWire = widthOfDrainWire;
      else 
	widthOfWire = widthOfDrainWire;
      

      Horizontal::create(net, layerAlu2, connectorPosition 
	                  , getUnit(widthOfWire)
			  , 0
			  , getAbutmentBox().getXMax()
                        );

      // Create Two Pins.
      // ****************
      pin = Pin::create(net
	        , Name(getString(net->getName())+"_west")
	        , Pin::AccessDirection(Pin::AccessDirection::WEST)
		, Pin::PlacementStatus(Pin::PlacementStatus::PLACED)
		, layerAlu2
		, getAbutmentBox().getXMin()
		, connectorPosition
		, getUnit(widthOfWire)
		, getUnit(widthOfWire)
		);

      _mapNetToPinBoxInLeftSide[net] = pin; 

      pin = Pin::create(net
	        , Name(getString(net->getName())+"_east")
	        , Pin::AccessDirection(Pin::AccessDirection::EAST)
		, Pin::PlacementStatus(Pin::PlacementStatus::PLACED)
		, layerAlu2
		, getAbutmentBox().getXMax()
		, connectorPosition
		, getUnit(widthOfWire)
		, getUnit(widthOfWire)
		);

      _mapNetToPinBoxInRightSide[net] = pin; 

      routingZoneList.clear();

   // End Of Main Loop.
   // *****************
   end_for

   // Route Net Bulk.
   // ***************
   connectorPosition  = netName2PositionOfConnectorMap[string("bulk")];

   Net * netBulk = getNet(Name("bulk"));

   if(!netBulk)   // bulk and source are connected.  
     netBulk = getNet(Name("source"));   

   // Calculate the width of Contact Alu1.
   // ************************************
   long widthOfAlu1  = MAX_INTEGER( MAX_INTEGER(minAlu1Width, 2*reAlu1Contact + minContWidth), 2*reAlu1Via1 + minVia1Width);

   Unit bulkPosition = netName2PositionOfConnectorMap[string("bulk")];
   Unit sourcePosition = netName2PositionOfConnectorMap[string("source")];

   Horizontal::create( netBulk
	             , layerImp
	             , bulkPosition 
                     , getUnit(widthOfImp)
   		     , 0 - getUnit(reImpActi)
   		     , getAbutmentBox().getXMax() + getUnit(reImpActi)
                    );

   Horizontal::create( netBulk
	             , layerActive
	             , bulkPosition 
                     , getUnit(widthOfActive)
   		     , 0
   		     , getAbutmentBox().getXMax()
                    );

   Horizontal::create( netBulk
                     , layerAlu2
		     , bulkPosition 
                     , getUnit(widthOfSourceWire)
   		     , 0
   		     , getAbutmentBox().getXMax()
                    );

   // Create Two Pins For Net bulk.
   // *****************************
   if(!_isBsConnected) {  

       pin = Pin::create(netBulk
                 , Name(getString(netBulk->getName())+"_west")
                 , Pin::AccessDirection(Pin::AccessDirection::WEST)
                 , Pin::PlacementStatus(Pin::PlacementStatus::PLACED)
                 , layerAlu2
                 , getAbutmentBox().getXMin()
                 , bulkPosition
                 , getUnit(widthOfSourceWire)
                 , getUnit(widthOfSourceWire)
                 );

       _mapNetToPinBoxInLeftSide[netBulk] = pin; 

       pin = Pin::create(netBulk
            , Name(getString(netBulk->getName())+"_east")
            , Pin::AccessDirection(Pin::AccessDirection::EAST)
            , Pin::PlacementStatus(Pin::PlacementStatus::PLACED)
            , layerAlu2
            , getAbutmentBox().getXMax()
            , bulkPosition
            , getUnit(widthOfSourceWire)
            , getUnit(widthOfSourceWire)
            );

       _mapNetToPinBoxInRightSide[netBulk] = pin; 
   }


   if( netName2PositionOfConnectorMap[string("source")] > netName2PositionOfConnectorMap[string("bulk")] ) {
     // Source Is Upper Than Bulk.  

     cout << "  Source is Upper Than Bulk" << endl;

     list<Unit>::iterator i = sourcePositionList.begin(),  j = sourcePositionList.end();
     
     while(i!=j) {
       
       cout << "  #########  Create Contact ###########" <<endl;

       Contact::create(netBulk, layerVia01, *i 
            , bulkPosition
            , getUnit(minContWidth)
            , getUnit(minContWidth)
            );

       Contact::create(netBulk, layerAlu1, *i 
            , bulkPosition
            , getUnit(widthOfAlu1)
            , getUnit(widthOfAlu1)
            );

       Contact::create(netBulk, layerVia12, *i
            , bulkPosition
            , getUnit(minVia1Width)
            , getUnit(minVia1Width)
            );

       if( _isBsConnected ) { // If bulk and Source are connected. 

	 cout << " B S is connected in " << *i << endl;

         Vertical::create(netBulk, layerAlu1, *i
                   , sourceRoutingZoneWidth 
          	   , bulkPosition
          	   , sourcePosition);
       } 

       i++;
     }

   } else {

     list<Unit>::iterator i , j;


     if( _isBsConnected ) { // If bulk and Source are connected. 
        i = sourcePositionList.begin();
	j = sourcePositionList.end();
     } 
     else {
        i = drainPositionList.begin();
	j = drainPositionList.end();
     }

     while(i!=j) {
       
       cout << "  #########  Create Contact ###########" <<endl;

       Contact::create(netBulk, layerVia01, *i 
            , bulkPosition
            , getUnit(minContWidth)
            , getUnit(minContWidth)
            );
   
       Contact::create(netBulk, layerAlu1, *i 
            , bulkPosition
            , getUnit(widthOfAlu1)
            , getUnit(widthOfAlu1)
            );
   
       Contact::create(netBulk, layerVia12, *i
            , bulkPosition
            , getUnit(minVia1Width)
            , getUnit(minVia1Width)
            );
   
       i++;
     } 
   }


   // Create Ring.
   // ************
   if( _hasRing ) {
     widthOfImp = MAX_INTEGER(minImpWidth, minActiWidth + 2*reImpActi); 

     Net * netRing = Net::create(this, Name("RING"));


     // Create rectangle in IMPLANT.
     // ***************************
     Horizontal::create( netRing
	             , layerImp
	             , getAbutmentBox().getYMax() 
                     , getUnit(widthOfImp)
   		     , getAbutmentBox().getXMin() - getUnit(widthOfImp/2)
   		     , getAbutmentBox().getXMax() + getUnit(widthOfImp/2)
                    );

     Horizontal::create( netRing
	             , layerImp
	             , getAbutmentBox().getYMin() 
                     , getUnit(widthOfImp)
   		     , getAbutmentBox().getXMin() - getUnit(widthOfImp/2)
   		     , getAbutmentBox().getXMax() + getUnit(widthOfImp/2)
                    );
   
     Vertical::create(netRing
	           , layerImp
		   , getAbutmentBox().getXMin()
                   , getUnit(widthOfImp) 
          	   , getAbutmentBox().getYMin()
          	   , getAbutmentBox().getYMax()
		   );

     Vertical::create(netRing
	           , layerImp
		   , getAbutmentBox().getXMax()
                   , getUnit(widthOfImp) 
          	   , getAbutmentBox().getYMin()
          	   , getAbutmentBox().getYMax()
		   );


     // Create rectangle in Active.
     // ***************************
     Horizontal::create( netRing
	             , layerActive
	             , getAbutmentBox().getYMax() 
                     , getUnit(minActiWidth)
   		     , getAbutmentBox().getXMin() - getUnit(minActiWidth/2)
   		     , getAbutmentBox().getXMax() + getUnit(minActiWidth/2)
                    );

     Horizontal::create( netRing
	             , layerActive
	             , getAbutmentBox().getYMin() 
                     , getUnit(minActiWidth)
   		     , getAbutmentBox().getXMin() - getUnit(minActiWidth/2)
   		     , getAbutmentBox().getXMax() + getUnit(minActiWidth/2)
                    );


     Vertical::create(netRing
	           , layerActive
		   , getAbutmentBox().getXMin()
                   , getUnit(minActiWidth) 
          	   , getAbutmentBox().getYMin()
          	   , getAbutmentBox().getYMax()
		   );

     Vertical::create(netRing
	           , layerActive
		   , getAbutmentBox().getXMax()
                   , getUnit(minActiWidth) 
          	   , getAbutmentBox().getYMin()
          	   , getAbutmentBox().getYMax()
		   );

   }

   // Create Caission NWELL if this is a PMOS.
   // ****************************************
   if(_polarity == Transistor::Polarity::P) {
       Net * netCaisson = Net::create(this, Name("CAISSON"));
       Contact::create(netCaisson, layerNwell
	    , getAbutmentBox().getXCenter()
            , getAbutmentBox().getYCenter()
            , getAbutmentBox().getWidth()
            , getAbutmentBox().getHeight()
            );
   }   

   closeUpdateSession();

}  



}  // END OF NAMESPACE DEVICE
