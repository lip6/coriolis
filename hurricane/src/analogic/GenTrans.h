// ****************************************************************************************************
// File: GenTrans.h
// Authors: Wu YiFei
// Date   : 04/04/2007 
// ****************************************************************************************************


#ifndef HURRICANE_GENTRANS
#define HURRICANE_GENTRANS


#include "Transistor.h"
#include "Box.h"


// --------------------------------------------------------------------
// Macro Method  :  "MAXLONG(a,b), MINLONG(a,b)" 
// Comparaison Method Macro For Calculate Rectangles.

#  define   MAXLONG(a,b) (a>b?a:b) 
#  define   MINLONG(a,b) (a>b?b:a) 


// --------------------------------------------------------------------
// Macro Method  :  "GET_RULE(s)"
// This Method Macro is For Geting The Value in RdsUnit of a DRM Rule.  
// To Use This Macro, you must predefine Pointeur dtraccess. 

#  define   GET_RULE(s)  \
   dtraccess->GetSingleRdsRuleByLabel(string(s))



// --------------------------------------------------------------------
// Macro Method "GET_RULE_BYNP(prefix, type, suffix)"
// This Method Macro is For Geting The Value in RdsUnit of a DRM Rule  
// selected by string type which is mostype . 

   /*  \prefix       must be a chain character.
    *  \type         mos type('N'/'P'), must be a string.
    *  \suffix       must be a chain character.
    */

// To Use This Macro, you must Predefine Pointeur dtraccess. 

#  define GET_RULE_BYNP(prefix, type, suffix) \
   dtraccess->GetSingleRdsRuleByLabel(prefix, type, suffix)



// --------------------------------------------------------------------
// Macro Method : "Get_LAYER_BYNP(prefix, type, suffix)"
// This Method Macro is For Geting the Layer of a Layer Rule Selected 
// by string type which is mostype. To Use This Macro, you must predefine
// Pointeur dtraccess.

   /*  \prefix       must be a chain of character.
    *  \type         mos type('N'/'P'), must be a string.
    *  \suffix       must be a string.
    */

// To Use This Macro, you must Predefine Pointeur dtraccess. 

#  define GET_LAYER_BYNP(prefix, type, suffix) \
   dtraccess->GetSingleLayerByLabel(prefix, type, suffix)



// --------------------------------------------------------------------
// Macro Method  : "SAVE_RECTANGLE(S, x, y, dx, dy)"
// This Method Macro is For Saving A Rectangle in a map .  

   /*  \s            name of rectangle, must be a chain of character.
    *  \x            xmin,    must be a long.
    *  \y            ymin,   must be a long.
    *  \dx           width,  must be a long.
    *  \dy           height, must be a long.
    */

// This Method Macro must be used in Membre Function Calculate.

#  define   SAVE_RECTANGLE(s, x, y, dx, dy) \
   _mapString2Box[string(s)] = Box(GetUnit(x), GetUnit(y), GetUnit(x+dx), GetUnit(y+dy)); \
   xmin = MINLONG(xmin, GetUnit(x)); \
   ymin = MINLONG(ymin, GetUnit(y));


// --------------------------------------------------------------------
// Macro Method   :  "GET_BOX(s)"
// This Method Macro is For Get the box According to the its name. 

   /*
    *  \s            name of rectangle, must be a string.  
    */

// This Method Macro must be used in member function of Class GenTrans.
// 

#  define GET_BOX(s) \
   _mapString2Box[s]


// ---------------------------------------------------------------------
// Macro Method   : "CREATE_CONTACT_MATRIX_UNDER(boxname, nbcolumn, layer, net)"
// This Method Macro is For Create a matrix of Contact under a zone for
// a net specified. 

   /*  \underbox   A rectangle under which contacts will be placed, must be a objet Box.
    *  \nbcolumn   the number of column, must be a integer. 
    *  \layer      the layer of contact, must be a pointer of type Layer*.
    *  \net        the net to be hooked, must be a pointer of type Net*.
    */

// To use this Macro Function. You Must also définir variable nbcontact,
// tmp_xcenter, tmp_ycenter, rw_cont, rd_cont.
//

#  define CREATE_CONTACT_MATRIX_UNDER(underbox, nbcolumn, layer, net) \
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
             Contact::Create(net, layer                    \
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

// ------------------------------------------------------------------
// Macro Function   :  "BOX_IS_VALID(box)"
// This Macro Function is for check if the box is correctly in the
// grille of fondor. It will check if the value of x,y,dx,dy are all
// pair.

   /*
    *  box     the box is to check. must be a objet Box.  
    */
 
#  define BOX_IS_VALID(box) \
   ( (long)(GetValue(box.getXMin()))%2==0 )&& \
   ( (long)(GetValue(box.getXMax()))%2==0 )&& \
   ( (long)(GetValue(box.getYMin()))%2==0 )&& \
   ( (long)(GetValue(box.getYMax()))%2==0 ) 
   

BEGIN_NAMESPACE_HURRICANE


class GenTrans {
// *************

// Attributes
// **********
  protected : map<string, Box> _mapString2Box;


// Constructors
// *************
  public : GenTrans() {};

// Destructors
// ***********
  public : virtual ~GenTrans() {};

// Operators
// **********
  public : virtual void Calculate(Transistor*) = 0; 
  public : virtual void Generate(Transistor*) = 0;

};


class GenV1Trans : public GenTrans {
// *********************************

// Types
// *****
  public : typedef GenTrans Inherit;

// Attributs
// *********
  public : const Transistor::MaskV1Info* _masqueV1Info;

// Constructors
// ************
  public : GenV1Trans(Transistor::MaskV1Info*);

// Destructors
// ************
  public : virtual ~GenV1Trans() {};

// Operators
// *********
  public : virtual void Calculate(Transistor*) ;
  public : virtual void Generate(Transistor*) ;
  
};


END_NAMESPACE_HURRICANE

#endif
