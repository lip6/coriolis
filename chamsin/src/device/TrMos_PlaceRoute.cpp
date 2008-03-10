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

   // Get Dtr Rules And Calculate the Size of AbutmentBox of Device.
   // **************************************************************
   DtrAccess * dtraccess = DtrAccess::Instance(); 

   char type;
   if(_type == 'P') type = 'N'; 
   else type = 'P';

   long minImpWidth       = dtraccess->GetSingleRdsRuleByLabel("RW_", GetString(type), "IMP"); 
   long minContWidth      = dtraccess->GetSingleRdsRuleByLabel(string("RW_CONT")); 
   long minAlu1Width      = dtraccess->GetSingleRdsRuleByLabel(string("RW_ALU1")); 
   long minVia1Width      = dtraccess->GetSingleRdsRuleByLabel(string("RW_VIA1")); 

   long rdImp             = dtraccess->GetSingleRdsRuleByLabel(string("RD_NIMP")); 
   long rdActive          = dtraccess->GetSingleRdsRuleByLabel(string("RD_ACTI")); 
   long rdAlu2            = dtraccess->GetSingleRdsRuleByLabel(string("RD_ALU1")); 

   long reImpActi         = dtraccess->GetSingleRdsRuleByLabel("RE_", GetString(type), "IMP_CONT"); 
   long reActiContact     = dtraccess->GetSingleRdsRuleByLabel("RE_ACTI_CONT"); 
   long reAlu1Contact     = dtraccess->GetSingleRdsRuleByLabel("RE_ALU1_CONT"); 
   long reAlu1Via1        = dtraccess->GetSingleRdsRuleByLabel("RE_ALU1_VIA1"); 

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

   OccurrenceLocator locator =  GetLeafInstanceOccurrences().GetLocator();  
   Instance * instance = dynamic_cast<Instance*>(locator.GetElement().GetEntity());;  
   fingerHeight        = instance->GetCell()->GetBoundingBox().getHeight();
   horizontalMargin    = GetUnit(RETURN_EVEN((long)(GetValue(fingerHeight))/4));
   verticalLowMargin   = GetUnit(RETURN_EVEN((long)(GetValue(fingerHeight))/2));
   verticalHighMargin  = GetUnit(RETURN_EVEN((long)(GetValue(fingerHeight))/2));


   verticalLowMargin   = MAX_INTEGER(verticalLowMargin, GetUnit(RETURN_EVEN(rdImp + widthOfImp/2 + widthOfSourceWire 
	                + rdAlu2 + widthOfActive + rdActive)) );

   verticalHighMargin  = MAX_INTEGER(verticalHighMargin, horizontalMargin + GetUnit(2*rdAlu2 + 2*widthOfDrainWire) );
   horizontalMargin    = MAX_INTEGER(horizontalMargin, GetUnit(RETURN_EVEN(rdImp + widthOfImp/2)) );

   OpenUpdateSession();

   if(_m ==  1 ) {  // If there is only one finger.
     _Place( instance, Transformation::Orientation::ID, Point( horizontalMargin, verticalLowMargin ) );
     //CloseUpdateSession();
     //return;

   }
   else {   
     // Get instance who's model's abutment type is Left or Right.
     // ************************************************************
     for_each_occurrence(occurrence, GetLeafInstanceOccurrences())
        instance = dynamic_cast<Instance*>(occurrence.GetEntity());
        Transistor * trans = dynamic_cast<Transistor*>(instance->GetMasterCell());

        if ( _sourceIsFirst ) {
          if(trans->IsLeft() && !leftins)
             leftins = instance;
          else if ( trans->IsLeft() && leftins)
             rightins = instance;
          else if ( trans->IsRight())
             rightins = instance;
        }
        else {
          if(trans->IsRight() && !leftins)
            leftins = instance;
          else if (trans->IsRight() && leftins )
            rightins = instance;
          else if (trans->IsLeft())
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

     _SetRefIns(leftins);

     if(_sourceIsFirst)  // internal Finger's transformation. 
        internalTransCode = Transformation::Orientation::MX; 
     else
        internalTransCode = Transformation::Orientation::ID;

     // Place internal finger.
     // *********************
     for_each_occurrence(occurrence, GetLeafInstanceOccurrences())

        Instance * instance = dynamic_cast<Instance*>(occurrence.GetEntity());

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
     Transistor * trans = dynamic_cast<Transistor*>(rightins->GetMasterCell());

     if( trans->IsRight())
       _PlaceRight( rightins, Transformation::Orientation::ID);
     else 
       _PlaceRight( rightins, Transformation::Orientation::MX);
     
   }  

   CloseUpdateSession();



   // Set AbutmentBox.
   // ****************
   for_each_instance(instance, GetInstances())
    instance->Unmaterialize();      
    instance->Materialize();      
   end_for

   OpenUpdateSession();


   cout <<"Bounding box of TrMos is "<<GetString(GetBoundingBox())<<endl;

   SetAbutmentBox(Box(0, 0, 
		      GetBoundingBox().getWidth()  + 2*horizontalMargin,
		      GetBoundingBox().getHeight() + verticalLowMargin + verticalHighMargin
		      )
                 ); 


   // **************************************************************
   // Routing .
   // **************************************************************

   Unit expectedInterval  = GetUnit(RETURN_EVEN((long)(GetValue(horizontalMargin))/2)); 
   Unit interval          = 0;
   Unit initialPosition   = verticalLowMargin + fingerHeight + GetUnit(rdAlu2 + widthOfDrainWire/2); 

   map<string, Unit> netName2PositionOfConnectorMap; 
   list<Box> routingZoneList;    
   list<Unit> sourcePositionList;
   list<Unit> drainPositionList;
   Unit sourceRoutingZoneWidth;
   Unit drainRoutingZoneWidth;
   
   DataBase * db = GetDataBase();  
    if(!db) throw Error("Can't launch Trmos::PlaceAndRoute for " + GetString(this) + " : can't find DataBase");

   Layer * layerAlu1    = db->GetTechnology()->GetLayer(Name("ALU1"));
   Layer * layerAlu2    = db->GetTechnology()->GetLayer(Name("ALU2"));

   Layer * layerVia1    = db->GetTechnology()->GetLayer(Name("VIA1"));
   Layer * layerActive  = db->GetTechnology()->GetLayer(Name("ACTIVE"));

   Layer * layerVia01   = db->GetTechnology()->GetLayer(Name("via01"));
   Layer * layerVia12   = db->GetTechnology()->GetLayer(Name("via12"));

   Layer * layerPoly    = db->GetTechnology()->GetLayer(Name("POLY"));
   Layer * layerNwell   = db->GetTechnology()->GetLayer(Name("NWELL"));
   Layer * layerCont    = db->GetTechnology()->GetLayer(Name("CONT"));

   Layer * layerImp = NULL;
   
   if(_type == 'P')  
     layerImp  = db->GetTechnology()->GetLayer(Name("NIMP"));
   else 
     layerImp  = db->GetTechnology()->GetLayer(Name("PIMP"));

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

     interval = MAX_INTEGER(expectedInterval, GetUnit(widthOfDrainWire + rdAlu2));

     // initialPosition += horizontalMargin/2;
     initialPosition += interval;
     i++;
   }

   vector<PinName>::iterator m = _lowPinOrder.begin(), 
      n = _lowPinOrder.end();
   
   //initialPosition = verticalMargin - horizontalMargin/2; 
   //initialPosition = verticalLowMargin - MAX_INTEGER(expectedInterval, GetUnit(rdImp + widthOfImp/2)); 
   initialPosition = verticalLowMargin - GetUnit(rdImp + widthOfImp/2); 

   while(m!=n) {
     if(*m == S)
       netName2PositionOfConnectorMap[string("source")] = initialPosition;   
     if(*m == B)
       netName2PositionOfConnectorMap[string("bulk")] = initialPosition; 

     interval = MAX_INTEGER(expectedInterval, GetUnit(rdAlu2 + widthOfSourceWire));

     initialPosition -= interval;
     m++;
   }

   cout << " Main loop "<< endl;

   // Main Routing Algorithm.
   // ***********************

   // Main Loop.
   // **********
   for_each_net(net, GetNets())          // For all hypernets.

      if(GetString(net->GetName())=="bulk" || GetString(net->GetName())=="BULK"  )
	 continue;

      // Get Routing Zone. 
      // *****************
      HyperNet hyperNet(Occurrence(net, Path()));    
      for_each_occurrence(occurrence, hyperNet.GetNetOccurrences())  // For all net occurrences.
         Net * net = dynamic_cast<Net*>(occurrence.GetEntity());
         Box routingZone;

         if(net->GetCell()->IsLeaf()) {
	   Transistor * trans = dynamic_cast<Transistor*>(net->GetCell());
	   if ( !trans )
	      throw Error("Can't launch Trmos::PlaceAndRoute for " + GetString(this) 
		  + ", it is not a Transistor");
	   
           cout << GetString(occurrence) << endl;
	   cout << GetString(occurrence.GetPath().GetTransformation()) <<endl;
	   cout << GetString((*(trans->_GetMapNet2Box()))[net]) << endl;

           // Get Routing Zone.
	   // *****************
	   routingZone = occurrence.GetPath().GetTransformation().getBox((*(trans->_GetMapNet2Box()))[net]);  
	   routingZoneList.push_back(routingZone);  

           if(GetString(net->GetName())=="SOURCE") {
             sourcePositionList.push_back(routingZone.getXCenter());         
      	     sourceRoutingZoneWidth = routingZone.getWidth();
      	   }
       	   else if (GetString(net->GetName())=="DRAIN") {
             drainPositionList.push_back(routingZone.getXCenter());         
             drainRoutingZoneWidth = routingZone.getWidth(); 
           }
	 }

      end_for


      cout <<"Print routing zone for " <<GetString(net)<<endl;

      list<Box>::iterator it_begin_listbox = routingZoneList.begin(), 
	   it_end_listbox = routingZoneList.end();

      while(it_begin_listbox != it_end_listbox)
      {
         cout<< GetString(*it_begin_listbox) <<endl;	
         it_begin_listbox++;
      }

      cout <<"End Print Routing Zone for "<<GetString(net)<<endl;

      // Create routing line.
      // ********************
      list<Box>::iterator routingzonelist_begin_it = routingZoneList.begin(), 
	routingzonelist_end_it = routingZoneList.end();

      connectorPosition = netName2PositionOfConnectorMap[GetString(net->GetName())];
      cout << "Connector Position is " << netName2PositionOfConnectorMap[GetString(net)] << endl;

      while(routingzonelist_begin_it!=routingzonelist_end_it) {

        Box routingZoneBox = *routingzonelist_begin_it;
	
        // Create vertical line and Contact.
        // ********************************
	if(connectorPosition > routingZoneBox.getYMin()) {
	   Vertical::Create(net, layerAlu1, routingZoneBox.getXCenter()
	         , routingZoneBox.getWidth() + GetUnit(2*reAlu1Contact)
	         , routingZoneBox.getYMin() - GetUnit(reAlu1Contact)
	         , connectorPosition);        
	}   
	else  {
	   Vertical::Create(net, layerAlu1, routingZoneBox.getXCenter()
	         , routingZoneBox.getWidth() + GetUnit(2*reAlu1Contact)
		 , connectorPosition
		 , routingZoneBox.getYMax() + GetUnit(reAlu1Contact) ) ;
        }

        Contact::Create(net, layerVia12, routingZoneBox.getXCenter()
	      , connectorPosition
	      , GetUnit(minVia1Width)
	      , GetUnit(minVia1Width)
	    );

        routingzonelist_begin_it ++ ;
      } 
      
      // Create horizontal line.
      // ***********************
      long widthOfWire = 0;

      if(GetString(net->GetName())=="source")
	widthOfWire = widthOfSourceWire;
      else if(GetString(net->GetName())=="drain")
	widthOfWire = widthOfDrainWire;
      else 
	widthOfWire = widthOfDrainWire;
      

      Horizontal::Create(net, layerAlu2, connectorPosition 
	                  , GetUnit(widthOfWire)
			  , 0
			  , GetAbutmentBox().getXMax()
                        );

      // Create Two Pins.
      // ****************
      pin = Pin::Create(net
	        , Name(GetString(net->GetName())+"_west")
	        , Pin::AccessDirection(Pin::AccessDirection::WEST)
		, Pin::PlacementStatus(Pin::PlacementStatus::PLACED)
		, layerAlu2
		, GetAbutmentBox().getXMin()
		, connectorPosition
		, GetUnit(widthOfWire)
		, GetUnit(widthOfWire)
		);

      _mapNetToPinBoxInLeftSide[net] = pin; 

      pin = Pin::Create(net
	        , Name(GetString(net->GetName())+"_east")
	        , Pin::AccessDirection(Pin::AccessDirection::EAST)
		, Pin::PlacementStatus(Pin::PlacementStatus::PLACED)
		, layerAlu2
		, GetAbutmentBox().getXMax()
		, connectorPosition
		, GetUnit(widthOfWire)
		, GetUnit(widthOfWire)
		);

      _mapNetToPinBoxInRightSide[net] = pin; 

      routingZoneList.clear();

   // End Of Main Loop.
   // *****************
   end_for

   // Route Net Bulk.
   // ***************
   connectorPosition  = netName2PositionOfConnectorMap[string("bulk")];

   Net * netBulk = GetNet(Name("bulk"));

   if(!netBulk)   // bulk and source are connected.  
     netBulk = GetNet(Name("source"));   

   // Calculate the width of Contact Alu1.
   // ************************************
   long widthOfAlu1  = MAX_INTEGER( MAX_INTEGER(minAlu1Width, 2*reAlu1Contact + minContWidth), 2*reAlu1Via1 + minVia1Width);

   Unit bulkPosition = netName2PositionOfConnectorMap[string("bulk")];
   Unit sourcePosition = netName2PositionOfConnectorMap[string("source")];

   Horizontal::Create( netBulk
	             , layerImp
	             , bulkPosition 
                     , GetUnit(widthOfImp)
   		     , 0 - GetUnit(reImpActi)
   		     , GetAbutmentBox().getXMax() + GetUnit(reImpActi)
                    );

   Horizontal::Create( netBulk
	             , layerActive
	             , bulkPosition 
                     , GetUnit(widthOfActive)
   		     , 0
   		     , GetAbutmentBox().getXMax()
                    );

   Horizontal::Create( netBulk
                     , layerAlu2
		     , bulkPosition 
                     , GetUnit(widthOfSourceWire)
   		     , 0
   		     , GetAbutmentBox().getXMax()
                    );

   // Create Two Pins For Net bulk.
   // *****************************
   if(!_isBsConnected) {  

       pin = Pin::Create(netBulk
                 , Name(GetString(netBulk->GetName())+"_west")
                 , Pin::AccessDirection(Pin::AccessDirection::WEST)
                 , Pin::PlacementStatus(Pin::PlacementStatus::PLACED)
                 , layerAlu2
                 , GetAbutmentBox().getXMin()
                 , bulkPosition
                 , GetUnit(widthOfSourceWire)
                 , GetUnit(widthOfSourceWire)
                 );

       _mapNetToPinBoxInLeftSide[netBulk] = pin; 

       pin = Pin::Create(netBulk
            , Name(GetString(netBulk->GetName())+"_east")
            , Pin::AccessDirection(Pin::AccessDirection::EAST)
            , Pin::PlacementStatus(Pin::PlacementStatus::PLACED)
            , layerAlu2
            , GetAbutmentBox().getXMax()
            , bulkPosition
            , GetUnit(widthOfSourceWire)
            , GetUnit(widthOfSourceWire)
            );

       _mapNetToPinBoxInRightSide[netBulk] = pin; 
   }


   if( netName2PositionOfConnectorMap[string("source")] > netName2PositionOfConnectorMap[string("bulk")] ) {
     // Source Is Upper Than Bulk.  

     cout << "  Source is Upper Than Bulk" << endl;

     list<Unit>::iterator i = sourcePositionList.begin(),  j = sourcePositionList.end();
     
     while(i!=j) {
       
       cout << "  #########  Create Contact ###########" <<endl;

       Contact::Create(netBulk, layerVia01, *i 
            , bulkPosition
            , GetUnit(minContWidth)
            , GetUnit(minContWidth)
            );

       Contact::Create(netBulk, layerAlu1, *i 
            , bulkPosition
            , GetUnit(widthOfAlu1)
            , GetUnit(widthOfAlu1)
            );

       Contact::Create(netBulk, layerVia12, *i
            , bulkPosition
            , GetUnit(minVia1Width)
            , GetUnit(minVia1Width)
            );

       if( _isBsConnected ) { // If bulk and Source are connected. 

	 cout << " B S is connected in " << *i << endl;

         Vertical::Create(netBulk, layerAlu1, *i
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

       Contact::Create(netBulk, layerVia01, *i 
            , bulkPosition
            , GetUnit(minContWidth)
            , GetUnit(minContWidth)
            );
   
       Contact::Create(netBulk, layerAlu1, *i 
            , bulkPosition
            , GetUnit(widthOfAlu1)
            , GetUnit(widthOfAlu1)
            );
   
       Contact::Create(netBulk, layerVia12, *i
            , bulkPosition
            , GetUnit(minVia1Width)
            , GetUnit(minVia1Width)
            );
   
       i++;
     } 
   }


   // Create Ring.
   // ************
   if( _hasRing ) {
     widthOfImp = MAX_INTEGER(minImpWidth, minActiWidth + 2*reImpActi); 

     Net * netRing = Net::Create(this, Name("RING"));


     // Create rectangle in IMPLANT.
     // ***************************
     Horizontal::Create( netRing
	             , layerImp
	             , GetAbutmentBox().getYMax() 
                     , GetUnit(widthOfImp)
   		     , GetAbutmentBox().getXMin() - GetUnit(widthOfImp/2)
   		     , GetAbutmentBox().getXMax() + GetUnit(widthOfImp/2)
                    );

     Horizontal::Create( netRing
	             , layerImp
	             , GetAbutmentBox().getYMin() 
                     , GetUnit(widthOfImp)
   		     , GetAbutmentBox().getXMin() - GetUnit(widthOfImp/2)
   		     , GetAbutmentBox().getXMax() + GetUnit(widthOfImp/2)
                    );
   
     Vertical::Create(netRing
	           , layerImp
		   , GetAbutmentBox().getXMin()
                   , GetUnit(widthOfImp) 
          	   , GetAbutmentBox().getYMin()
          	   , GetAbutmentBox().getYMax()
		   );

     Vertical::Create(netRing
	           , layerImp
		   , GetAbutmentBox().getXMax()
                   , GetUnit(widthOfImp) 
          	   , GetAbutmentBox().getYMin()
          	   , GetAbutmentBox().getYMax()
		   );


     // Create rectangle in Active.
     // ***************************
     Horizontal::Create( netRing
	             , layerActive
	             , GetAbutmentBox().getYMax() 
                     , GetUnit(minActiWidth)
   		     , GetAbutmentBox().getXMin() - GetUnit(minActiWidth/2)
   		     , GetAbutmentBox().getXMax() + GetUnit(minActiWidth/2)
                    );

     Horizontal::Create( netRing
	             , layerActive
	             , GetAbutmentBox().getYMin() 
                     , GetUnit(minActiWidth)
   		     , GetAbutmentBox().getXMin() - GetUnit(minActiWidth/2)
   		     , GetAbutmentBox().getXMax() + GetUnit(minActiWidth/2)
                    );


     Vertical::Create(netRing
	           , layerActive
		   , GetAbutmentBox().getXMin()
                   , GetUnit(minActiWidth) 
          	   , GetAbutmentBox().getYMin()
          	   , GetAbutmentBox().getYMax()
		   );

     Vertical::Create(netRing
	           , layerActive
		   , GetAbutmentBox().getXMax()
                   , GetUnit(minActiWidth) 
          	   , GetAbutmentBox().getYMin()
          	   , GetAbutmentBox().getYMax()
		   );

   }

   // Create Caission NWELL if this is a PMOS.
   // ****************************************
   if(_type == 'P') {
       Net * netCaisson = Net::Create(this, Name("CAISSON"));
       Contact::Create(netCaisson, layerNwell
	    , GetAbutmentBox().getXCenter()
            , GetAbutmentBox().getYCenter()
            , GetAbutmentBox().getWidth()
            , GetAbutmentBox().getHeight()
            );
   }   

   CloseUpdateSession();

}  



}  // END OF NAMESPACE DEVICE
