// ****************************************************************************************************
// File: GenV1Trans.cpp
// Authors: Wu YiFei
// Date   : 04/04/2007 
// ****************************************************************************************************

#include "Collection.h"
#include "RdsUnit.h"
#include "GenTrans.h"
#include "DtrAccess.h"
#include "GenericDtrAccess.h"

#include "Technology.h"
#include "UpdateSession.h"

#include "DataBase.h"

namespace Hurricane {


// ****************************************************************************************************
// Globals Datas 
// ****************************************************************************************************
string segsforsource[] = {string("20"), string("23")};
string segsfordrain[]  = {string("40"), string("43")};
string segsforgrid[]   = {string("00"), string("01"), string("30"), string("31")};
//string segsforgrid[]   = {string("00"), string("30") };
string segsforanonym[] = {string("10"), string("11"), string("12"), string("50")};

 
// ****************************************************************************************************
// Class getV1Trans implementation
// ****************************************************************************************************
GenV1Trans::GenV1Trans(Transistor::MaskV1Info* masqueinfo) 
// *********************************************************
:   _masqueV1Info(masqueinfo)  
{
}


void GenV1Trans::Calculate(Transistor* transistor)
// **********************************************
{
  DtrAccess * dtraccess = DtrAccess::Instance(); 

  // Check out mask param.
  // *********************
  if(_masqueV1Info->getL() < dtraccess->getSingleRealRuleByLabel("L_TRANS") ||
     _masqueV1Info->getL() > dtraccess->getSingleRealRuleByLabel("L_TRANS_MAX") ||
     _masqueV1Info->getW() < dtraccess->getSingleRealRuleByLabel("W_TRANS") ||
     _masqueV1Info->getW() > dtraccess->getSingleRealRuleByLabel("W_TRANS_MAX") )

    throw Error("Can't launch function GenV1Trans::Calculate for " + getString(transistor) 
	+ " the L " + getString(_masqueV1Info->getL()) 
	+ " or the W " + getString(_masqueV1Info->getW()) 
	+ " of this transistor is invalid."
	);

  if(_masqueV1Info->getNbSourceColumn() < 1 || _masqueV1Info->getNbSourceColumn() > MAXNBCONTACT || 
     _masqueV1Info->getNbDrainColumn() < 1 || _masqueV1Info->getNbDrainColumn() > MAXNBCONTACT ) 

    throw Error("Can't launch function GenV1Trans::Calculate for " + getString(transistor) 
	+ " the nbsourcecolumn " + getString(_masqueV1Info->getNbSourceColumn()) 
	+ " or the nbdraincolumn " + getString(_masqueV1Info->getNbDrainColumn()) 
	+ " of this transistor is invalid."
	);

  IF_DEBUG_HUR_ANALOG
    cout << ts << getString(transistor) + " 's masqueinfo is " + getString(_masqueV1Info)
         << endl;
  END_IF


  // Tempory Variable.
  // **************************
  long x00, y00, dx00, dy00;
  long x10, y10, dx10, dy10;
  long x11, y11, dx11, dy11;
  long x12, y12, dx12, dy12;
  long x20, y20, dx20, dy20;
  long x23, y23, dx23, dy23;
  long x30, y30, dx30, dy30;
  long x31, y31, dx31, dy31;
  long x01, y01, dx01, dy01;
  long x40, y40, dx40, dy40;
  long x43, y43, dx43, dy43;
  long x50, y50, dx50, dy50;

  long xmin = 999999L, ymin = 999999L;
  long realw = 0;

  // Tempory Variable.
  // **************************
  long extension1 = 0;
  long extension2 = 0;
  long extension3 = 0;
  long extension4 = 0;
  long ymax     = 0;
  string mostype;    // get Mos Type (N/P).

  if(transistor->IsNmos())
    mostype='N';
  else
    mostype='P';

  //string mostype;    // get Mos Type (N/P).
  //mostype=transistor->getType();

  // -------------------------------------------------------------
  // Begin Calculate.

  //long re_imp_acti  = GET_RULE_BYNP("RE_", mostype, "IMP_ACTI"); 
  long re_imp_acti  = GET_RULE_BYNP("RE_", mostype, "IMP_ACTI"); 
  long re_imp_poly  = GET_RULE_BYNP("RE_", mostype, "IMP_POLY");
  long re_imp_cont  = GET_RULE_BYNP("RE_", mostype, "IMP_CONT");
  long re_imp_gate  = GET_RULE_BYNP("RE_", mostype, "IMP_GATE");
  //long re_well_acti = GET_RULE_BYNP("RE_", mostype, "WELL_ACTI");

  // Calculate Rectangle 00
  // **********************
  x00 = 0; 
  y00 = -( GET_RULE("RE_GATE_ACTI") ); 

  dx00 = ConvertRealToRdsUnit(_masqueV1Info->getL());
  realw = ConvertRealToRdsUnit(_masqueV1Info->getW());

  dy00 = realw + 2*(-y00);

  SAVE_RECTANGLE("00", x00, y00, dx00, dy00)
  
    
  // Calculate Rectangle 30 
  // **********************

//  cout << "RD_ACTI_CONT is " << GET_RULE("RD_ACTI_CONT")<<endl;
//  cout << "RD_ACTI_POLY is " << GET_RULE("RD_ACTI_POLY")<<endl;
//  cout << "RE_POLY_CONT is " << GET_RULE("RE_POLY_CONT")<<endl;
//  cout << "MAX RD_ACTI_POLY is " << (MAXLONG(GET_RULE("RD_ACTI_CONT"), GET_RULE("RD_ACTI_POLY") + GET_RULE("RE_POLY_CONT"))) <<endl;
//  

  dx31 = GET_RULE("RW_CONT") + 2*GET_RULE("RE_POLY_CONT"); 
  if (dx31 >= dx00) {
    dx30 = GET_RULE("RW_CONT");
    dy30 = dx30;
    y30 = 0 + realw + MAXLONG(GET_RULE("RD_ACTI_CONT"), GET_RULE("RD_ACTI_POLY") + GET_RULE("RE_POLY_CONT"));
  } 
  else {
    dx30 = dx00 - 2*GET_RULE("RE_POLY_CONT");
    dy30 = GET_RULE("RW_CONT");
    y30 = 0 + realw + GET_RULE("RD_ACTI_CONT");
  }

  x30 = x00 + dx00/2 - dx30/2;    
  
  SAVE_RECTANGLE("30", x30, y30, dx30, dy30)
   

  // Calculate Rectangle 31 
  // **********************
  dx31 = dx30 + 2*GET_RULE("RE_POLY_CONT");
  dy31 = dy30 + 2*GET_RULE("RE_POLY_CONT");
  x31 = x30 - GET_RULE("RE_POLY_CONT");
  y31 = y30 - GET_RULE("RE_POLY_CONT");

  SAVE_RECTANGLE("31", x31, y31, dx31, dy31)


  // Calculate Rectangle 01
  // **********************
  if ( y31 <= (y00+dy00) ) {
    x01 = 0; y01 = 0; dx01 = 0; dy01 = 0;
  }
  else {
    x01 = x00; 
    y01 = y00 + dy00; 
    dx01 = dx00; 
    dy01 = y31 - (y00 + dy00);
  }

  SAVE_RECTANGLE("01", x01, y01, dx01, dy01)


  // Calculate Rectangle 12
  // **********************
  x12 = MINLONG(x31, x00) - re_imp_poly;
  y12 = MINLONG(0 - re_imp_acti, y00 - re_imp_poly); 
  dx12 = MAXLONG(dx31, dx00) + 2 * re_imp_poly;

  ymax = MAXLONG( MAXLONG( y30 + dy30 + re_imp_cont
	          , MAXLONG(y31 + dy31, y00 + dy00) + re_imp_poly )
                  , realw + re_imp_acti );

  dy12 =  ymax - y12 ;

  SAVE_RECTANGLE("12", x12, y12, dx12, dy12)


  // Calculate Rectangle 20 
  // **********************
  y20 = 0 + GET_RULE("RE_ACTI_CONT");
  dy20 = realw - 2 * GET_RULE("RE_ACTI_CONT");
  dx20 = (_masqueV1Info->getNbSourceColumn()) * GET_RULE("RW_CONT")  + 
      ((_masqueV1Info->getNbSourceColumn()) - 1) * GET_RULE("RD_CONT");
  x20 = 0 - ( dx20 + GET_RULE("RD_CONT_GATE") );

  SAVE_RECTANGLE("20", x20, y20, dx20, dy20)
  

  // Calculate Rectangle 40
  // **********************
  y40 = y20;
  x40 = x00 + dx00 + GET_RULE("RD_CONT_GATE");
  dx40 = (_masqueV1Info->getNbDrainColumn()) * GET_RULE("RW_CONT")  + 
      ((_masqueV1Info->getNbDrainColumn()) - 1) * GET_RULE("RD_CONT");
  dy40 = dy20;

  SAVE_RECTANGLE("40", x40, y40, dx40, dy40)

  // Calculate Rectangle 10
  // **********************
  y10 = 0;
  x10 = MINLONG(x20 - GET_RULE("RE_ACTI_CONT"), 0 - GET_RULE("RE_ACTI_GATE"));
  dy10 = realw; 

  extension1 = MAXLONG(0 + x40 + dx40 + GET_RULE("RE_ACTI_CONT"), dx00 + GET_RULE("RE_ACTI_GATE"));

  dx10 = 0 - x10 + extension1;
  
  SAVE_RECTANGLE("10", x10, y10, dx10, dy10)
   

  // Calculate Rectangle 23
  // ***********************
  x23 = x10;
  y23 = y10;
  dx23 = 0 - x10;
  dy23 = realw;

  SAVE_RECTANGLE("23", x23, y23, dx23, dy23)


  // Calculate Rectangle 43
  // **********************
  x43 = x00 + dx00 ;
  y43 = y10;
  dx43 = x10 + dx10 - (x00 + dx00);
  dy43 = realw;

  SAVE_RECTANGLE("43", x43, y43, dx43, dy43)
     
  
  // Calculate Rectangle 11
  // **********************
  extension1 = re_imp_gate;
  extension2 = re_imp_cont + 0 - x20;
  extension3 = re_imp_acti + 0 - x10;

  extension4 = MAXLONG(MAXLONG(extension1, extension2), extension3);

  x11 = 0  - extension4;

  extension1 = re_imp_gate + x00 + dx00;
  extension2 = re_imp_cont + x40 + dx40;
  extension3 = re_imp_acti + x10 + dx10;

  extension4 = MAXLONG(MAXLONG(extension1, extension2), extension3);
  
  dx11 = 0 - x11 + extension4;  
  
  y11 = MINLONG(y20 - re_imp_cont, y23 - re_imp_acti);
 
  ymax = MAXLONG(y20 + dy20 + re_imp_cont, y23 + dy23 + re_imp_acti);

  dy11 = ymax - y11;

  SAVE_RECTANGLE("11", x11, y11, dx11, dy11);


  // Calculate Rectangle 50 just for PMOS.
  // -------------------------------------------------------------
  if (transistor->IsPmos())  { // Calculate Rectangle 50 for PMos.
     x50 = x10 - GET_RULE("RE_NWELL_ACTI");
     y50 = y10 - GET_RULE("RE_NWELL_ACTI");
     dx50 = dx10 + 2 * GET_RULE("RE_NWELL_ACTI");
     dy50 = dy10 + 2 * GET_RULE("RE_NWELL_ACTI"); 

     SAVE_RECTANGLE("50", x50, y50, dx50, dy50);
  }

  // End Calculate.
  // -------------------------------------------------------------

  // Translate rectangles.
  // *********************
  map<string, Box>::iterator i = _mapString2Box.begin(), 
    j = _mapString2Box.end();

  while(i!=j) {
    _mapString2Box[(*i).first] = (*i).second.translate(-xmin, -ymin);      

    IF_DEBUG_HUR_ANALOG
      cout << ts << (*i).first <<"  " << getString((*i).second) << endl;
    END_IF

    assert(BOX_IS_VALID((*i).second));
    i++;
  }

}


void GenV1Trans::Generate(Transistor* transistor)
// *********************************************
{
  OpenUpdateSession();

  Net* source = transistor->getNet(Name(transistor->getSourceName())); 
  Net* drain  = transistor->getNet(Name(transistor->getDrainName()) ); 
  Net* grid   = transistor->getNet(Name(transistor->getGridName())  ); 

  DtrAccess * dtraccess = DtrAccess::Instance(); 
  //string mostype(1, transistor->getType());    // get Mos Type (N/P).

  string mostype;    // get Mos Type (N/P).

  if(transistor->IsNmos())
    mostype='N';
  else
    mostype='P';

  long rw_cont = getUnit(GET_RULE("RW_CONT"));
  long rd_cont = getUnit(GET_RULE("RD_CONT"));
  unsigned nbcontact = 0;
  long tmp_xcenter = 0;
  long tmp_ycenter = 0;

  DataBase * db = getDataBase();  

  if(!db) throw Error("In getV1Trans::Generate : can't find DataBase");

  //Technology * tech = db->getTechnology();

  Layer * layer_20 = GET_LAYER_BYNP("TRANS_",mostype,"_LAYER_20"); 
  Layer * layer_30 = GET_LAYER_BYNP("TRANS_",mostype,"_LAYER_30"); 
  Layer * layer_40 = GET_LAYER_BYNP("TRANS_",mostype,"_LAYER_40"); 


  // -------------------------------------------------------------
  // Begin Generation. 

  // Cenerate Components For Net Source. 
  // ***********************************
IF_DEBUG_HUR_ANALOG
      cout << ts << "Begin for create components for net Source of " << getString(transistor) << endl;
END_IF

  for(size_t i=0; i<sizeof(segsforsource)/sizeof(string); i++) {

    if(segsforsource[i]=="20") {
      //cout << ts << " Begin create contact for source , Under Box is " << getString(GET_BOX(segsforsource[i])) <<endl;
      Box underbox = GET_BOX(segsforsource[i]);
      CREATE_CONTACT_MATRIX_UNDER(underbox, transistor->getNbSourceColumn(), layer_20, source)  
      //cout << ts << " Finish create contact for source " <<endl;
    }
    else {     
      Contact::create(source, GET_LAYER_BYNP("TRANS_",mostype,"_LAYER_"+segsforsource[i])
                    , GET_BOX(segsforsource[i]).getXCenter()
                    , GET_BOX(segsforsource[i]).getYCenter()
  		    , GET_BOX(segsforsource[i]).getWidth()
  		    , GET_BOX(segsforsource[i]).getHeight() 
  		    );
    }
  }  

IF_DEBUG_HUR_ANALOG
      cout << ts << "End for create components for net Source of " << getString(transistor) << endl;
END_IF

    
  // Generate Components For Net Grid.
  // *********************************
IF_DEBUG_HUR_ANALOG
      cout << ts << "Begin for create components for net Grid of " << getString(transistor) << endl;
END_IF
  for(size_t i=0; i<sizeof(segsforgrid)/sizeof(string); i++) {
       if(segsforgrid[i]=="30"){
          if( GET_BOX(segsforgrid[i]).getWidth()==GET_RULE("RW_CONT") ) {
              Contact::create(grid, GET_LAYER_BYNP("TRANS_",mostype,"_LAYER_"+segsforgrid[i])
                              , GET_BOX(segsforgrid[i]).getXCenter()
                              , GET_BOX(segsforgrid[i]).getYCenter()
            		      , GET_BOX(segsforgrid[i]).getWidth()
            		      , GET_BOX(segsforgrid[i]).getHeight()
                             );
	  }    
	  else  {
              unsigned int nbcolumn = (GET_BOX(segsforgrid[i]).getWidth()-rw_cont)/(rw_cont + rd_cont) + 1;
 
IF_DEBUG_HUR_ANALOG
	      cout << ts << "nbcolumn in rectangle 30 is " << nbcolumn <<endl;
END_IF

              Box underbox = GET_BOX(segsforgrid[i]);
              CREATE_CONTACT_MATRIX_UNDER(underbox, nbcolumn, layer_30, grid)  
	  }
       }
       else {
	  if(GET_BOX(segsforgrid[i]).getXMin() < GET_BOX(segsforgrid[i]).getXMax()) {
              Contact::create(grid, GET_LAYER_BYNP("TRANS_",mostype,"_LAYER_"+segsforgrid[i])
                              , GET_BOX(segsforgrid[i]).getXCenter()
                              , GET_BOX(segsforgrid[i]).getYCenter()
            		      , GET_BOX(segsforgrid[i]).getWidth()
            		      , GET_BOX(segsforgrid[i]).getHeight()
                             );
          }
       }

  }     
     
IF_DEBUG_HUR_ANALOG
      cout << ts << "End for create components for net Grid of " << getString(transistor) << endl;
END_IF


  // Generate Components For Net Drain. 
  // **********************************
IF_DEBUG_HUR_ANALOG
      cout << ts << "Begin for create components for net Drain of " << getString(transistor) << endl;
END_IF
  for(size_t i=0; i<sizeof(segsfordrain)/sizeof(string); i++) {

    if(segsfordrain[i]=="40") {
      //cout << ts << " Begin create contact for drain, Under Box is " << getString(GET_BOX(segsforsource[i])) <<endl;
      Box underbox = GET_BOX(segsfordrain[i]);
      CREATE_CONTACT_MATRIX_UNDER(underbox, transistor->getNbDrainColumn(), layer_40, drain) 
      //cout << ts << " Finish create contact for drain" <<endl;
    }
    else {     
      Contact::create(drain, GET_LAYER_BYNP("TRANS_",mostype,"_LAYER_"+segsfordrain[i])
                    , GET_BOX(segsfordrain[i]).getXCenter()
                    , GET_BOX(segsfordrain[i]).getYCenter()
  		    , GET_BOX(segsfordrain[i]).getWidth()
  		    , GET_BOX(segsfordrain[i]).getHeight() 
  		    );
    }
  }  

IF_DEBUG_HUR_ANALOG
      cout << ts << "End for create components for net Drain of " << getString(transistor) << endl;
END_IF

  // Generate Components For Anonyms Nets.
  // *************************************
IF_DEBUG_HUR_ANALOG
      cout << ts << "Begin for create components for net Anonyme of " << getString(transistor) << endl;
END_IF
  Net * anonym = Net::create(transistor, Name("anonym"));
  for(size_t i=0; i<sizeof(segsforanonym)/sizeof(string);i++) {
    if(transistor->IsNmos() && segsforanonym[i]=="50")
      continue;

    Contact::create(anonym, GET_LAYER_BYNP("TRANS_",mostype,"_LAYER_"+segsforanonym[i])
	           , GET_BOX(segsforanonym[i]).getXCenter()
		   , GET_BOX(segsforanonym[i]).getYCenter()
		   , GET_BOX(segsforanonym[i]).getWidth()
		   , GET_BOX(segsforanonym[i]).getHeight()
		   );
  }

IF_DEBUG_HUR_ANALOG
      cout << ts << "End for create components for net Anonyme of " << getString(transistor) << endl;
END_IF

  // End Generation.
  // -------------------------------------------------------------

  CloseUpdateSession();

  // Set Transistor::_mapNet2Box.  
  // ****************************
  (*(transistor->_getMapNet2Box()))[grid]   = _mapString2Box[string("30")];
  (*(transistor->_getMapNet2Box()))[source] = _mapString2Box[string("20")];
  (*(transistor->_getMapNet2Box()))[drain]  = _mapString2Box[string("40")];

  cout<< getString(_mapString2Box[string("30")]) <<endl;
  cout<< getString(_mapString2Box[string("20")]) <<endl;
  cout<< getString(_mapString2Box[string("40")]) <<endl;

  // Set Abutment Box.
  // ***************** 
  switch(transistor->getAbutmentType().getCode()) {

    case Transistor::Type::INTERNAL : 
        transistor->setAbutmentBox( Box(GET_BOX(string("20")).getXCenter() 
       	                             , transistor->getBoundingBox().getYMin()
       	   			     , GET_BOX(string("40")).getXCenter()
       	   			     , transistor->getBoundingBox().getYMax()
				    )
       	   			  );  
	break; 

    case Transistor::Type::LEFT: 
        transistor->setAbutmentBox( Box(GET_BOX(string("11")).getXMin() 
       	                              , transistor->getBoundingBox().getYMin()
       	   			      , GET_BOX(string("40")).getXCenter()
       	   			      , transistor->getBoundingBox().getYMax()
				    )
       	   			  );  
	break ;
     
    case Transistor::Type::RIGHT: 
        transistor->setAbutmentBox( Box(GET_BOX(string("20")).getXCenter() 
       	                              , transistor->getBoundingBox().getYMin()
       	   			      , GET_BOX(string("11")).getXMax()
       	   			      , transistor->getBoundingBox().getYMax()
				    )
       	   			  );  
	break ;

    case Transistor::Type::SINGLE: 
        transistor->setAbutmentBox( Box(GET_BOX(string("11")).getXMin() 
       	                              , transistor->getBoundingBox().getYMin()
       	   			      , GET_BOX(string("11")).getXMax()
       	   			      , transistor->getBoundingBox().getYMax()
                                    )
       	   			  );  
	break ;

    default :
        break;
  } 


}  

}
