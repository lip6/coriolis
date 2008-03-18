%{
#include <stdio.h>
#include <map>
#include <list>
#include <string>  

#include "Error.h"
#include "DtrAccess.h"
using namespace Hurricane;

#include "ParserDtrGram.hpp"

extern FILE * dtrin;
extern int dtrlex(void); 
extern char * dtrtext;
extern int DTRlineno;


// *************************************************************************
// Static Function&Data
// *************************************************************************

static int dtrerror( char * message)
// *********************************
{
    char * str = (char*)malloc(200*sizeof(char)); 
    sprintf(str, "ParserDtr():\n %s before %s at line %d.\n", message, dtrtext, DTRlineno);

    string errmsg = str; 

    throw Error (errmsg);

    return 0;
}

static DtrAccess * dtraccess = NULL;


static char* table_name = NULL;

static list<double> doubleList;
static list<string> stringList;


%}

%union { double  _value;
         char * _text;
       };

%token T_TABLE T_TABLE_END T_DOUBLE T_ID
%type < _value > T_DOUBLE 
%type < _text > T_ID

%start file

%%

file: lines tables 
    ;

lines : '\n' 
      | lines '\n'
      ;
      
tables: table
       |tables table
       ;

table: T_TABLE T_ID '\n' 
         { table_name = $2; } 
	 items T_TABLE_END lines
       ;

items:  item 
       |items item 
       ;

item:  T_ID elems '\n' 
  {

     string label = $1;
     delete $1;

     if( strcmp(table_name, "RULES")==0 ) {
          if ( doubleList.empty() ) {
	    
             dtrerror("ParserDtr detect no data in a line of table RULES : Please check it");
	  }

          list<double>::iterator i = doubleList.begin()
	      , j = doubleList.end();

	  while(i!=j) {
            (dtraccess->_getLabel2RuleMap())[label].push_back(*i);  
	    i++;
	  }
     }  
     else if( strcmp(table_name, "LAYERS")==0 ) { 
          if (  stringList.empty() ) {
             dtrerror("ParserDtr detect no data in a line of table LAYERS : Please check it");
	  }
                
          list<string>::iterator i = stringList.begin()
	      , j = stringList.end();

	  while(i!=j) {
            (dtraccess->_getLabel2LayerNameMap())[label].push_back(*i);;   
	    i++;
	  }
     }
     else if( strcmp(table_name, "ELECTRICAL") == 0 ) {
          if (  doubleList.empty() )  {
             dtrerror("ParserDtr detect no data in a line of table ELECTRICAL : Please check it");
	  }
                
          list<double>::iterator i = doubleList.begin()
	      , j = doubleList.end();

	  while(i!=j) {
            (dtraccess->_getLabel2ElectricalMap())[label].push_back(*i);  
	    i++;
	  }
     }
     else if( strcmp(table_name, "MODELLING") == 0 ) { 
          if (  (doubleList.empty()) || (doubleList.size()!=1) )  {
             dtrerror("ParserDtr detect no data or mutli datas in a line of table MODELLING: Please check it");
	  }
                
          list<double>::iterator i = doubleList.begin();
	     
          (dtraccess->_getLabel2ModellingMap())[label] = (int)(*i);  
     }

     else if( strcmp(table_name, "SIMPLE_CAPA_MIM")==0 ) {
          if( (doubleList.empty()) || (doubleList.size()!=1) ) {
             dtrerror("ParserDtr detect no data or mutli datas in a line of table SIMPLE_CAPA_MIM : Please check it");
	  }
 
          list<double>::iterator i = doubleList.begin();
	  (dtraccess->_getLabel2SimpleCapaMimMap())[label] = (*i);
       
     }
      
     else { 
         throw Error("ParserDtr detect unknown table name " + string(table_name));
     }        

     // Vide temporary list
     // ********************
     doubleList.clear();
     stringList.clear();

  }
   | lines
       ; 

elems: elem
      |elems elem 
      ;

elem: T_ID
   {
      if(!(doubleList.empty())) {

        dtrerror("ParserDtr detect incompatibles datas : Please check it");
      }

      stringList.push_back(string($1));
      delete $1;
   }
      | T_DOUBLE
   {
      if(!(stringList.empty())) {

        dtrerror("ParserDtr detect incompatibles datas : Please check it");
      }

      doubleList.push_back($1);
   }
       ;

%%

// *************************************************************************
// Generic Function
// *************************************************************************

void ParseDtr(const char* dtrfilename, DtrAccess* dtr)
// ***************************************************
{
  dtraccess = dtr;

  if((dtrin=fopen(dtrfilename, "r"))==NULL) 
  {
      throw Error("Can't open dtr file : " + string(dtrfilename));
  }

  dtrparse();

}  

