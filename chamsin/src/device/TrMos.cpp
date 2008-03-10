// ****************************************************************************************************
// File: TrMos.cpp
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************


#include "TrMos.h"

#include "Instance.h"
#include "MetaTransistor.h"
#include "Net.h"
#include "Transistor.h"
#include "Transistors.h"

#include "UpdateSession.h"

#include "DtrAccess.h"
using namespace Hurricane;


namespace DEVICE{

// ****************************************************************************************************
// TrMos implementation
// ****************************************************************************************************

TrMos::TrMos(Library* library, const Name& name)
// **************************************************************************
:	Inherit(library, name),
        _type('N'),
	_isBsConnected(false),
	_m(1),
	_sourceIsFirst(true),
	_hasDummy(false),
	_hasRing(true),
        _tr1(NULL),
	_capaRouting(0)
{
}


TrMos* TrMos::Create(Library* library, const Name & name)
// **************************************************************************
{
	TrMos* trmos= new TrMos(library, name);
	trmos->_PostCreate();
	return trmos;
}


void TrMos::Delete()
// *****************
{
   _PreDelete();
   delete this;
}


void TrMos::_PreDelete()
// ******************************
{
   // do something
   // ************
  
   Inherit::_PreDelete();
}


void TrMos::_PostCreate()
// *******************************
{
   Inherit::_PostCreate();

   // do something.
   // Initialize pin order list and other attributes. 
   // **********************************************
  // _lowPinOrder[0]=D;
  // _lowPinOrder[1]=G;

  // _highPinOrder[0]=S;
  // _highPinOrder[1]=B;

   _highPinOrder.push_back(D);
   _highPinOrder.push_back(G);

   _lowPinOrder.push_back(S);
   _lowPinOrder.push_back(B);

   double minWidth = (DtrAccess::Instance())->GetSingleRealRuleByLabel(string("RW_ALU1")); 

   _widthOfSourceWire = minWidth;
   _widthOfDrainWire = minWidth;

}


Transistors TrMos::GetTransistors() const 
// **************************************
{
    return  GetCollection(_transistorList);
}


void TrMos::Create(const char type, const bool isbsconnected)
// **********************************************************
{
   if( _tr1 ) {
      throw Error("Can't Create Logical View of TrMos " +  GetString(GetName()) + " : " 
	  + "it has already been created"); 
   }

   if( (type!=TRANSN) && (type!=TRANSP)) {
      throw Error("Can't Create TrMos " +  GetString(GetName()) + " : type " + GetString(type) + " is invalid"); 
   }

   _type = type;
   _isBsConnected = isbsconnected;

   // MetaTransistor is in the same library than Trmos
   // ************************************************
   Library * library = GetLibrary();

   // Create signals
   // **************
   Net * drain  = NULL;
   Net * source = NULL;
   Net * grid   = NULL;
   Net * bulk   = NULL;

   (drain  = Net::Create(this, Name("drain")))->SetExternal(true);
   (source = Net::Create(this, Name("source")))->SetExternal(true);
   (grid   = Net::Create(this, Name("grid")))->SetExternal(true);
   
   if(!isbsconnected) { 
     (bulk   = Net::Create(this, Name("bulk")))->SetExternal(true);
   }


   // Instancier a MetaTransistor and create the connection
   // The name of MetaTransistor is nameoftrmos_tr1
   // ****************************************************

   _tr1 = MetaTransistor::Create(library, Name( GetString(GetName())+"_Mos1" ), _type);
   Instance * instance = Instance::Create(this
                                        , Name("Ins_" + GetString(_tr1->GetName()))
					, _tr1
					);

   instance->GetPlug(_tr1->GetNet(Name("DRAIN")))->SetNet(drain);
   instance->GetPlug(_tr1->GetNet(Name("SOURCE")))->SetNet(source);
   instance->GetPlug(_tr1->GetNet(Name("GRID")))->SetNet(grid);

   if(!isbsconnected)
     instance->GetPlug(_tr1->GetNet(Name("BULK")))->SetNet(bulk);
   else 
     instance->GetPlug(_tr1->GetNet(Name("BULK")))->SetNet(source);


}  


void TrMos::Generate(const unsigned m, const bool sourceisfirst, const bool hasring
    , const unsigned nbsourcecolumn, const unsigned nbdraincolumn)
// *********************************************************************************
{
   if( !_tr1 ) {
      throw Error("Can't Create Physical View for " +  GetString(this)
	  + " : " + "Logical view has't been created yet.");
   }

//   if( !(_transistorList.empty()) ) {
//      throw Error("Can't Create Physical View of TrMos " +  GetString(GetName()) + " : " 
//	  + "it has already been created"); 
//   }

   // Check out param of realization.
   // *******************************
   if( m <= 0 ) 
     throw Error("Can't generate for " +  GetString(this) + " : m " + GetString(m)
	 + " is invalid."); 

   if(nbsourcecolumn<1)
     throw Error("Can't generate for " + GetString(this) + " : nbsourcecolumn "
	 + GetString(nbsourcecolumn) + " is invalid.");

   if(nbdraincolumn<1)
     throw Error("Can't generate for" + GetString(this) + " : nbdraincolumn " 
	 + GetString(nbdraincolumn) + " is invalid.");


   if(!(_transistorList.empty())) {
      _Flush();
   }

   _m = m;
   _sourceIsFirst = sourceisfirst;
   _hasRing = hasring;

   // Motifs are in the same library than Trmos
   // *****************************************
   Library * library = GetLibrary();

   cout << ts << "################################################################" <<endl
        << ts << "####    BEGIN AUTOGENERATON FOR " + _GetTypeName() + " " + GetString(GetName()) + " #####" <<endl
        << ts << "################################################################" <<endl
	<< endl;

//   OpenUpdateSession();

/* (1) */   

   IF_DEBUG_HUR_ANALOG
   cout << "***  Stage 1 : CreateLayout of " + GetString(this) + " Begin ***" <<endl;
   END_IF

   // Create Motifs according to m, and instance the Motifs according
   // to the Meta-Transistor . 
   // Set m of MetaTransistor.
   // The name of motif is nameofDevice_nameofMetaTrans_finger_index
   // ****************************************************************
   _tr1->SetM(_m);

   for(unsigned i=0; i<m; i++){
     Transistor* finger = Transistor::Create(library
	    , GetString(_tr1->GetName()) + "_Finger_" + GetString(i)
	    , _type 
	   );

     _transistorList.push_back(finger);
     Instance::Create(_tr1, Name("Ins_" + GetString(finger->GetName())), finger); 
   }

   IF_DEBUG_HUR_ANALOG

   cout << "***  Stage 1 : CreateLayout of " + GetString(this) + " finish ***" <<endl;
   cout << ts <<   GetString(_tr1) + " 's M is " + GetString(_tr1->GetM()) + ".\n"
        << ts <<   GetString(_m) + " Transistors are created.\n"
	<<endl; 

   END_IF
   
/* (2) */

   IF_DEBUG_HUR_ANALOG
   cout << "***  Stage 2 : CreateLayout of " + GetString(this) + " Begin ***" <<endl;
   END_IF

   // Create connexion for each MetaTransistor.
   // *****************************************
   _tr1->CreateConnection();

   IF_DEBUG_HUR_ANALOG
   cout << "***  Stage 2 : CreateLayout of " + GetString(this) + " finish ***" <<endl;
   cout << "  The connection in " + GetString(_tr1) + " is created.\n"
        <<endl;
   END_IF
   
   // Pseudo dimensionnement of metatransistor.
   // In the futur, this will be the work of auto-dimensionnement tool (DSES).
   // ************************************************************************
//   _tr1->SetLe(10);
//   _tr1->SetWe(11);

/* (3) */
   IF_DEBUG_HUR_ANALOG
   cout << "***  Stage 3 : CreateLayout of " + GetString(this) + " Begin ***" <<endl;
   END_IF
   
   // Set dessin Parameter of generation for each finger. 
   // ***************************************************
   double l_finger = _tr1->GetLe() ;
   double w_finger = (_tr1->GetWe()) / (double)(_tr1->GetM()) ;
   unsigned count = 0;

   Transistor::MaskV1Info * masqueinfo = Transistor::MaskV1Info::Create( l_finger, w_finger); 
   masqueinfo->SetNbSourceColumn(nbsourcecolumn);
   masqueinfo->SetNbDrainColumn(nbdraincolumn);

   list<Transistor*>::iterator i = _transistorList.begin()
      , j = _transistorList.end();   

   if(_m == 1){
      masqueinfo->SetType(Transistor::Type::SINGLE);
      (*(_transistorList.begin()))->SetMaskInfo(masqueinfo);
   }  
   else if(_m%2==0) { // if m is pair, create two left fingers if is source first.
                      // and create two right fingers if is drain first.
        while(i!=j) {
         if(++count>2) 
           masqueinfo->SetType(Transistor::Type::INTERNAL);
	 else {
	   if ( _sourceIsFirst )
	     masqueinfo->SetType(Transistor::Type::LEFT);
	   else 
	     masqueinfo->SetType(Transistor::Type::RIGHT);
	 }

	 (*i)->SetMaskInfo(masqueinfo);
	 i++;
        }
   }	
   else if(_m%2==1) { // if m is impair, create one left finger, one right finger.
        while(i!=j){
	  ++ count ;
	  if (count == 1)
	    masqueinfo-> SetType(Transistor::Type::LEFT);
	  else if (count == 2)  
	    masqueinfo-> SetType(Transistor::Type::RIGHT);
	  else
	    masqueinfo-> SetType(Transistor::Type::INTERNAL);

          (*i)->SetMaskInfo(masqueinfo);
	  i++;
	}
   }

   masqueinfo->Delete();
   
   IF_DEBUG_HUR_ANALOG
   cout << "***  Stage 3 : CreateLayout of " + GetString(this) + " finish ***" <<endl;
   cout << ts << "Real l_finger is " + GetString(l_finger) + "." << endl
        << ts << "Real w_finger is " + GetString(w_finger) + "." << endl
	<<endl;
   END_IF

/* (4) */
   IF_DEBUG_HUR_ANALOG
   cout << "***  Stage 4 : CreateLayout of " + GetString(this) + " Begin ***" <<endl;
   cout << ts << "Call GenerateLayout for " + GetString(_tr1) 
         + " who will launch the generator of its fingers" << ".\n"
	<<endl;
   END_IF

   // Call function CreateLayout of MetaTransistor to launch the generator of finger.
   // *******************************************************************************

   SetTerminal(false);
//
//   IF_DEBUG_HUR_ANALOG
//   cout << endl;
//   cout << ts << "Real l of " << (long)_tr1 << GetString(_tr1) + " is " + GetString(_tr1->_le) + "." << endl
//        << ts << "Real w of " << (long)_tr1 << GetString(_tr1) + " is " + GetString(_tr1->_we) + "." << endl
//	<<endl;
//   END_IF
//
   _tr1->CreateLayout();
   
   IF_DEBUG_HUR_ANALOG
   cout << "***  Stage 4 : CreateLayout of " + GetString(this) + " finish ***"<<endl
        << endl;
   END_IF

/* (5) */   
   // Lauch Algo of Placement and routage selected.
   // *********************************************
   /* to do */

   _PlaceAndRoute();

   cout << " Place And Route " <<endl;

   for_each_instance(instance, GetInstances())
    //instance->SetTransformation(instance->GetTransformation());      
    instance->Unmaterialize();      
    instance->Materialize();      

IF_DEBUG_HUR_ANALOG
    cout << ts << GetString(instance) <<" 's boundingBox is " << GetString(instance->GetBoundingBox())<<endl;
END_IF
     
   end_for

//   CloseUpdateSession();

IF_DEBUG_HUR_ANALOG
   cout << ts << GetString(this) << " 's primary (without wire) boundingBox is " << GetString(GetBoundingBox()) <<endl;
END_IF

   Materialize();

IF_DEBUG_HUR_ANALOG
   cout << ts << GetString(this) << " 's boundingBox is " << GetString(GetBoundingBox()) <<endl;
END_IF

   cout << ts << endl
        << ts << "################################################################" <<endl
        << ts << "####    END AUTOGENERATON FOR " + _GetTypeName() + " " + GetString(GetName()) + " #####" <<endl
        << ts << "################################################################" <<endl
	<< endl;
} 


void TrMos::SetLowPinOrder(const PinName pin1, const PinName pin2)
// ***************************************************************
{
   _lowPinOrder[0]=pin1;
   _lowPinOrder[1]=pin2;

   vector<PinName>::iterator i = _lowPinOrder.begin(), j = _lowPinOrder.end();

   cout << " Low Pin Order " << endl;

   while(i!=j) {
    cout << *i << endl;
    i++;
   }

}  


void TrMos::SetHighPinOrder(const PinName pin1, const PinName pin2)
// *****************************************************************
{
   _highPinOrder[0]=pin1;
   _highPinOrder[1]=pin2;

   vector<PinName>::iterator i = _highPinOrder.begin(), j = _highPinOrder.end();

   cout << " High Pin Order " << endl;

   while(i!=j) {
    cout << *i << endl;
    i++;
   }
}  


void TrMos::_Flush()
// ****************
{
  if(_transistorList.empty()) {
      throw Error("Can't delete Physical View of TrMos " +  GetString(GetName()) + " : "  + "il doesn't exist"); 
  }

  _tr1->Flush();

  _transistorList.clear();

  // Delete all segments of TrMos
  // ****************************
  /* to do */

  
}  


string TrMos::_GetString() const
// ***************************************
{
     string s= Inherit::_GetString();
     return s;
}

Record* TrMos::_GetRecord() const
// *********************************
{
	Record* record = Inherit::_GetRecord();
	return record;
}

} // end of namespace Device 
