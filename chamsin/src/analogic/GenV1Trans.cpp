// ****************************************************************************************************
// File: GenV1Trans.cpp
// Authors: Wu YiFei
// Date   : 04/04/2007 
// ****************************************************************************************************

#include "Technology.h"
#include "UpdateSession.h"
#include "DataBase.h"

#include "RdsUnit.h"
#include "DtrAccess.h"
#include "AnalogicalCommons.h"
#include "GenTrans.h"

#define   MAXLONG(a,b) (a>b?a:b) 
#define   MINLONG(a,b) (a>b?b:a) 

#define   SAVE_RECTANGLE(s, x, y, dx, dy) \
   _mapString2Box[string(s)] = Box(getUnit(x), getUnit(y), getUnit(x+dx), getUnit(y+dy)); \
   xmin = MINLONG(xmin, getUnit(x)); \
   ymin = MINLONG(ymin, getUnit(y));

#define   GET_RULE(s)  \
   dtraccess->getSingleRdsRuleByLabel(string(s))

#define BOX_IS_VALID(box) \
   ( (long)(getValue(box.getXMin()))%2==0 )&& \
   ( (long)(getValue(box.getXMax()))%2==0 )&& \
   ( (long)(getValue(box.getYMin()))%2==0 )&& \
   ( (long)(getValue(box.getYMax()))%2==0 ) 

#define CREATE_CONTACT_MATRIX_UNDER(underbox, nbcolumn, layer, net) \
                                                                      \
        if(underbox.getHeight()<rw_cont)   \
           nbcontact = 0; \
        else \
           nbcontact = (underbox.getHeight()-rw_cont)/(rw_cont + rd_cont) + 1 ;\
         \
                                                                     \
        tmp_xcenter = underbox.getXMin() + (rw_cont/2);  \
        tmp_ycenter = underbox.getYMin() + (rw_cont/2);  \
                                                                  \
\
        for(unsigned i=0; i<nbcolumn; i++) { \
                                                                \
           for(unsigned j=0; j<nbcontact; j++) {                     \
             Contact::create(net, layer                    \
      	               , tmp_xcenter                            \
      		       , tmp_ycenter                           \
      		       , rw_cont              \
      		       , rw_cont              \
      		       );                    \
                                              \
             tmp_ycenter += (rw_cont + rd_cont); \
           }              \
              \
           tmp_xcenter += (rw_cont + rd_cont);  \
           tmp_ycenter = underbox.getYMin() + (rw_cont/2); \
	}
   


namespace Hurricane {


// ****************************************************************************************************
// Globals Datas 
// ****************************************************************************************************
string segsforsource[] = {string("20"), string("22")};
string segsfordrain[]  = {string("40"), string("42")};
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
  DtrAccess* dtraccess = DtrAccess::getDtrAccess(); 

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

  if(_masqueV1Info->getNbSourceColumn() < 1 || _masqueV1Info->getNbSourceColumn() > GenV1Trans::maxNbContacts || 
     _masqueV1Info->getNbDrainColumn() < 1 || _masqueV1Info->getNbDrainColumn() > GenV1Trans::maxNbContacts ) 

    throw Error("Can't launch function GenV1Trans::Calculate for " + getString(transistor) 
	+ " the nbsourcecolumn " + getString(_masqueV1Info->getNbSourceColumn()) 
	+ " or the nbdraincolumn " + getString(_masqueV1Info->getNbDrainColumn()) 
	+ " of this transistor is invalid."
	);

  IF_DEBUG_HUR_ANALOG
    cout << getString(transistor) + " 's masqueinfo is " + getString(_masqueV1Info)
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

  if(transistor->isNmos())
    mostype='N';
  else
    mostype='P';

  //string mostype;    // get Mos Type (N/P).
  //mostype=transistor->getType();

  // -------------------------------------------------------------
  // Begin Calculate.

  //long re_imp_acti  = GET_RULE_BYNP("RE_", mostype, "IMP_ACTI"); 
  long re_imp_acti  = dtraccess->getSingleRdsRuleByLabel("RE_", mostype, "IMP_ACTI"); 
  long re_imp_poly  = dtraccess->getSingleRdsRuleByLabel("RE_", mostype, "IMP_POLY");
  long re_imp_cont  = dtraccess->getSingleRdsRuleByLabel("RE_", mostype, "IMP_CONT");
  long re_imp_gate  = dtraccess->getSingleRdsRuleByLabel("RE_", mostype, "IMP_GATE");
  //long re_well_acti = GET_RULE_BYNP("RE_", mostype, "WELL_ACTI");

  // Calculate Rectangle 00
  // **********************
  x00 = 0; 
  y00 = -dtraccess->getSingleRdsRuleByLabel("RE_GATE_ACTI");

  dx00 = ConvertRealToRdsUnit(_masqueV1Info->getL());
  realw = ConvertRealToRdsUnit(_masqueV1Info->getW());

  dy00 = realw + 2*(-y00);

  SAVE_RECTANGLE("00", x00, y00, dx00, dy00);
  
    
  // Calculate Rectangle 30 
  // **********************

//  cout << "RD_ACTI_CONT is " << dtraccess->getSingleRdsRuleByLabel("RD_ACTI_CONT")<<endl;
//  cout << "RD_ACTI_POLY is " << dtraccess->getSingleRdsRuleByLabel("RD_ACTI_POLY")<<endl;
//  cout << "RE_POLY_CONT is " << dtraccess->getSingleRdsRuleByLabel("RE_POLY_CONT")<<endl;
//  cout << "MAX RD_ACTI_POLY is " << (MAXLONG(dtraccess->getSingleRdsRuleByLabel("RD_ACTI_CONT"), GET_RULE("RD_ACTI_POLY") + GET_RULE("RE_POLY_CONT"))) <<endl;
//  

  dx31 = dtraccess->getSingleRdsRuleByLabel("RW_CONT") + 2*GET_RULE("RE_POLY_CONT"); 
  if (dx31 >= dx00) {
    dx30 = dtraccess->getSingleRdsRuleByLabel("RW_CONT");
    dy30 = dx30;
    y30 = 0 + realw + MAXLONG(dtraccess->getSingleRdsRuleByLabel("RD_ACTI_CONT"), GET_RULE("RD_ACTI_POLY") + GET_RULE("RE_POLY_CONT"));
  } 
  else {
    dx30 = dx00 - 2*dtraccess->getSingleRdsRuleByLabel("RE_POLY_CONT");
    dy30 = dtraccess->getSingleRdsRuleByLabel("RW_CONT");
    y30 = 0 + realw + dtraccess->getSingleRdsRuleByLabel("RD_ACTI_CONT");
  }

  x30 = x00 + dx00/2 - dx30/2;    
  
  SAVE_RECTANGLE("30", x30, y30, dx30, dy30)
   

  // Calculate Rectangle 31 
  // **********************
  dx31 = dx30 + 2*dtraccess->getSingleRdsRuleByLabel("RE_POLY_CONT");
  dy31 = dy30 + 2*dtraccess->getSingleRdsRuleByLabel("RE_POLY_CONT");
  x31 = x30 - dtraccess->getSingleRdsRuleByLabel("RE_POLY_CONT");
  y31 = y30 - dtraccess->getSingleRdsRuleByLabel("RE_POLY_CONT");

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
  y20 = 0 + dtraccess->getSingleRdsRuleByLabel("RE_ACTI_CONT");
  dy20 = realw - 2 * dtraccess->getSingleRdsRuleByLabel("RE_ACTI_CONT");
  dx20 = (_masqueV1Info->getNbSourceColumn()) * dtraccess->getSingleRdsRuleByLabel("RW_CONT")  + 
      ((_masqueV1Info->getNbSourceColumn()) - 1) * dtraccess->getSingleRdsRuleByLabel("RD_CONT");
  x20 = 0 - ( dx20 + dtraccess->getSingleRdsRuleByLabel("RD_CONT_GATE") );

  SAVE_RECTANGLE("20", x20, y20, dx20, dy20)
  

  // Calculate Rectangle 40
  // **********************
  y40 = y20;
  x40 = x00 + dx00 + dtraccess->getSingleRdsRuleByLabel("RD_CONT_GATE");
  dx40 = (_masqueV1Info->getNbDrainColumn()) * dtraccess->getSingleRdsRuleByLabel("RW_CONT")  + 
      ((_masqueV1Info->getNbDrainColumn()) - 1) * dtraccess->getSingleRdsRuleByLabel("RD_CONT");
  dy40 = dy20;

  SAVE_RECTANGLE("40", x40, y40, dx40, dy40)

  // Calculate Rectangle 10
  // **********************
  y10 = 0;
  x10 = MINLONG(x20 - dtraccess->getSingleRdsRuleByLabel("RE_ACTI_CONT"), 0 - GET_RULE("RE_ACTI_GATE"));
  dy10 = realw; 

  extension1 = MAXLONG(0 + x40 + dx40 + dtraccess->getSingleRdsRuleByLabel("RE_ACTI_CONT"), dx00 + GET_RULE("RE_ACTI_GATE"));

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
  if (transistor->isPmos())  { // Calculate Rectangle 50 for PMos.
     x50 = x10 - dtraccess->getSingleRdsRuleByLabel("RE_NWELL_ACTI");
     y50 = y10 - dtraccess->getSingleRdsRuleByLabel("RE_NWELL_ACTI");
     dx50 = dx10 + 2 * dtraccess->getSingleRdsRuleByLabel("RE_NWELL_ACTI");
     dy50 = dy10 + 2 * dtraccess->getSingleRdsRuleByLabel("RE_NWELL_ACTI"); 

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
      cout << (*i).first <<"  " << getString((*i).second) << endl;
    END_IF

    assert(BOX_IS_VALID((*i).second));
    i++;
  }

}


void GenV1Trans::Generate(Transistor* transistor)
// *********************************************
{
    UpdateSession::open();

  Net* source = transistor->getNet(Name(transistor->getSourceName())); 
  Net* drain  = transistor->getNet(Name(transistor->getDrainName()) ); 
  Net* grid   = transistor->getNet(Name(transistor->getGridName())  ); 

  DtrAccess * dtraccess = DtrAccess::getDtrAccess(); 
  //string mostype(1, transistor->getType());    // get Mos Type (N/P).

  string mostype;    // get Mos Type (N/P).

  if(transistor->isNmos())
    mostype='N';
  else
    mostype='P';

  long rw_cont = getUnit(dtraccess->getSingleRdsRuleByLabel("RW_CONT"));
  long rd_cont = getUnit(dtraccess->getSingleRdsRuleByLabel("RD_CONT"));
  unsigned nbcontact = 0;
  long tmp_xcenter = 0;
  long tmp_ycenter = 0;

  DataBase * db = getDataBase();  

  if(!db) throw Error("In getV1Trans::Generate : can't find DataBase");

  //Technology * tech = db->getTechnology();

  Layer* layer_20 = dtraccess->getSingleLayerByLabel("M1TRANS_",mostype,"_LAYER_20"); 
  Layer* layer_30 = dtraccess->getSingleLayerByLabel("M1TRANS_",mostype,"_LAYER_30"); 
  Layer* layer_40 = dtraccess->getSingleLayerByLabel("M1TRANS_",mostype,"_LAYER_40"); 


  // -------------------------------------------------------------
  // Begin Generation. 

  // Cenerate Components For Net Source. 
  // ***********************************
IF_DEBUG_HUR_ANALOG
      cout << "Begin for create components for net Source of " << getString(transistor) << endl;
END_IF

  for(size_t i=0; i<sizeof(segsforsource)/sizeof(string); i++) {

    if(segsforsource[i]=="20") {
      //cout << " Begin create contact for source , Under Box is " << getString(_mapString2Box[segsforsource[i])) <<endl;
      Box underbox = _mapString2Box[segsforsource[i]];
      CREATE_CONTACT_MATRIX_UNDER(underbox, transistor->getNbSourceColumn(), layer_20, source)  
      //cout << " Finish create contact for source " <<endl;
    }
    else {     
  //    Contact::create(source, dtraccess->getSingleLayerByLabel("M1TRANS_",mostype,"_LAYER_"+segsforsource[i])
  //                  , _mapString2Box[segsforsource[i]].getXCenter()
  //                  , _mapString2Box[segsforsource[i]].getYCenter()
  //		    , _mapString2Box[segsforsource[i]].getWidth()
  //		    , _mapString2Box[segsforsource[i]].getHeight() 
  //		    );
    }
  }  

IF_DEBUG_HUR_ANALOG
      cout << "End for create components for net Source of " << getString(transistor) << endl;
END_IF

    
  // Generate Components For Net Grid.
  // *********************************
IF_DEBUG_HUR_ANALOG
      cout << "Begin for create components for net Grid of " << getString(transistor) << endl;
END_IF
  for(size_t i=0; i<sizeof(segsforgrid)/sizeof(string); i++) {
       if(segsforgrid[i]=="30"){
          if( _mapString2Box[segsforgrid[i]].getWidth()==dtraccess->getSingleRdsRuleByLabel("RW_CONT") ) {
              Contact::create(grid, dtraccess->getSingleLayerByLabel("M1TRANS_",mostype,"_LAYER_"+segsforgrid[i])
                              , _mapString2Box[segsforgrid[i]].getXCenter()
                              , _mapString2Box[segsforgrid[i]].getYCenter()
            		      , _mapString2Box[segsforgrid[i]].getWidth()
            		      , _mapString2Box[segsforgrid[i]].getHeight()
                             );
	  }    
	  else  {
              unsigned int nbcolumn = (_mapString2Box[segsforgrid[i]].getWidth()-rw_cont)/(rw_cont + rd_cont) + 1;
 
IF_DEBUG_HUR_ANALOG
	      cout << "nbcolumn in rectangle 30 is " << nbcolumn <<endl;
END_IF

              Box underbox = _mapString2Box[segsforgrid[i]];
              CREATE_CONTACT_MATRIX_UNDER(underbox, nbcolumn, layer_30, grid)  
	  }
       }
       else {
	  if(_mapString2Box[segsforgrid[i]].getXMin() < _mapString2Box[segsforgrid[i]].getXMax()) {
              Contact::create(grid, dtraccess->getSingleLayerByLabel("M1TRANS_",mostype,"_LAYER_"+segsforgrid[i])
                              , _mapString2Box[segsforgrid[i]].getXCenter()
                              , _mapString2Box[segsforgrid[i]].getYCenter()
            		      , _mapString2Box[segsforgrid[i]].getWidth()
            		      , _mapString2Box[segsforgrid[i]].getHeight()
                             );
          }
       }

  }     
     
IF_DEBUG_HUR_ANALOG
      cout << "End for create components for net Grid of " << getString(transistor) << endl;
END_IF


  // Generate Components For Net Drain. 
  // **********************************
IF_DEBUG_HUR_ANALOG
      cout << "Begin for create components for net Drain of " << getString(transistor) << endl;
END_IF
  for(size_t i=0; i<sizeof(segsfordrain)/sizeof(string); i++) {

    if(segsfordrain[i]=="40") {
      //cout << " Begin create contact for drain, Under Box is " << getString(_mapString2Box[segsforsource[i])) <<endl;
      Box underbox = _mapString2Box[segsfordrain[i]];
      CREATE_CONTACT_MATRIX_UNDER(underbox, transistor->getNbDrainColumn(), layer_40, drain) 
      //cout << " Finish create contact for drain" <<endl;
    }
    else {     
      Contact::create(drain, dtraccess->getSingleLayerByLabel("M1TRANS_",mostype,"_LAYER_"+segsfordrain[i])
                    , _mapString2Box[segsfordrain[i]].getXCenter()
                    , _mapString2Box[segsfordrain[i]].getYCenter()
  		    , _mapString2Box[segsfordrain[i]].getWidth()
  		    , _mapString2Box[segsfordrain[i]].getHeight() 
  		    );
    }
  }  

IF_DEBUG_HUR_ANALOG
      cout << "End for create components for net Drain of " << getString(transistor) << endl;
END_IF

  // Generate Components For Anonyms Nets.
  // *************************************
IF_DEBUG_HUR_ANALOG
      cout << "Begin for create components for net Anonyme of " << getString(transistor) << endl;
END_IF
  Net * anonym = Net::create(transistor, Name("anonym"));
  for(size_t i=0; i<sizeof(segsforanonym)/sizeof(string);i++) {
    if(transistor->isNmos() && segsforanonym[i]=="50")
      continue;

    Contact::create(anonym, dtraccess->getSingleLayerByLabel("M1TRANS_",mostype,"_LAYER_"+segsforanonym[i])
	           , _mapString2Box[segsforanonym[i]].getXCenter()
		   , _mapString2Box[segsforanonym[i]].getYCenter()
		   , _mapString2Box[segsforanonym[i]].getWidth()
		   , _mapString2Box[segsforanonym[i]].getHeight()
		   );
  }

IF_DEBUG_HUR_ANALOG
      cout << "End for create components for net Anonyme of " << getString(transistor) << endl;
END_IF

  // End Generation.
  // -------------------------------------------------------------

  UpdateSession::close();

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
        transistor->setAbutmentBox( Box(_mapString2Box[string("20")].getXCenter() 
       	                             , transistor->getBoundingBox().getYMin()
       	   			     , _mapString2Box[string("40")].getXCenter()
       	   			     , transistor->getBoundingBox().getYMax()
				    )
       	   			  );  
	break; 

    case Transistor::Type::LEFT: 
        transistor->setAbutmentBox( Box(_mapString2Box[string("11")].getXMin() 
       	                              , transistor->getBoundingBox().getYMin()
       	   			      , _mapString2Box[string("40")].getXCenter()
       	   			      , transistor->getBoundingBox().getYMax()
				    )
       	   			  );  
	break ;
     
    case Transistor::Type::RIGHT: 
        transistor->setAbutmentBox( Box(_mapString2Box[string("20")].getXCenter() 
       	                              , transistor->getBoundingBox().getYMin()
       	   			      , _mapString2Box[string("11")].getXMax()
       	   			      , transistor->getBoundingBox().getYMax()
				    )
       	   			  );  
	break ;

    case Transistor::Type::SINGLE: 
        transistor->setAbutmentBox( Box(_mapString2Box[string("11")].getXMin() 
       	                              , transistor->getBoundingBox().getYMin()
       	   			      , _mapString2Box[string("11")].getXMax()
       	   			      , transistor->getBoundingBox().getYMax()
                                    )
       	   			  );  
	break ;

    default :
        break;
  } 


}  

}
