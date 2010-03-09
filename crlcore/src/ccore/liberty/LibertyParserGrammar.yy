%{ 

#include <stdio.h>
#include <errno.h>
using namespace std;

#include  "hurricane/DataBase.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Library.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/CellPath.h"
#include  "LibertyTechnology.h"
#include  "LuTable.h"
#include  "LuTableTemplate.h"
using namespace CRL;

// Symbols from Flex which should be substituted.
#define    yyrestart    LIBERTYrestart
#define    yytext       LIBERTYtext
#define    yywrap       LIBERTYwrap
#define    yyin         LIBERTYin

extern int   yylex               ();
extern int   yywrap              ();
extern void  yyrestart           ( FILE* );
extern char* yytext;
extern FILE* yyin;

int  libertyLineNumber = 1;
int  yydoubleindex;

namespace {


const char*   liberty_filename;

Library*            current_library;
Cell*               current_cell;
Net*                current_net;
CLibertyTechnology* current_technology;
CCellPath*          current_cellPath;
CLuTableTemplate*   current_luTemplate;
CLuTable*           current_luTable;


Library* start_library(const char* name);
void start_cell(const char* name);
void start_net(const char* name);

void start_technology();
void register_unit(CLibertyTechnology::Unit,double val,string str);
void start_luTemplate(string name);
void luTemplate_addVar(unsigned char index,CLuTableTemplate::Variable::Type type);
void luTemplate_setVarIndexes(unsigned char index,unsigned short count,double* values);
void start_cellPath();
void cellPath_setRelatedNet(string name);
void cellPath_setType(CCellPath::Type type);
void cellPath_riseDelayTable(string name);
void cellPath_fallDelayTable(string name);
void cellPath_riseTransitionTable(string name);
void cellPath_fallTransitionTable(string name);
//void cellPath_ignored();
void luTable_addValues(unsigned short count, double* values);


void check_net_direction(Net::Direction direction);
void set_net_capacitance(double value);

int   yyerror    ( const char* message );

}

%}

%union {
    unsigned char    _index;
    int              _type;
    double            _value; 
    char             _text[1024];
    double            _values[128];
} 

%token CLIBP_LIBRARY

// library statement tokens
%token CLIBP_TECHNOLOGY
%token CLIBP_DATE
%token CLIBP_DELAY_MODEL
%token CLIBP_NOM_VOLTAGE
%token CLIBP_NOM_TEMPERATURE
%token CLIBP_NOM_PROCESS
%token CLIBP_SLEW_DERATE_FROM_LIBRARY
%token CLIBP_DEFAULT_FANOUT_LOAD
%token CLIBP_DEFAULT_INOUT_PIN_CAP
%token CLIBP_DEFAULT_INPUT_PIN_CAP
%token CLIBP_DEFAULT_OUTPUT_PIN_CAP
%token CLIBP_CAPACITIVE_LOAD_UNIT
%token CLIBP_VOLTAGE_UNIT
%token CLIBP_TIME_UNIT
%token CLIBP_PULLING_RESISTANCE_UNIT
%token CLIBP_CURRENT_UNIT
%token CLIBP_INPUT_THRESHOLD_PCT_RISE
%token CLIBP_INPUT_THRESHOLD_PCT_FALL
%token CLIBP_OUTPUT_THRESHOLD_PCT_RISE
%token CLIBP_OUTPUT_THRESHOLD_PCT_FALL
%token CLIBP_SLEW_LOWER_THRESHOLD_PCT_RISE
%token CLIBP_SLEW_LOWER_THRESHOLD_PCT_FALL
%token CLIBP_SLEW_UPPER_THRESHOLD_PCT_RISE
%token CLIBP_SLEW_UPPER_THRESHOLD_PCT_FALL

// lu table template
%token CLIBP_LU_TABLE_TEMPLATE
%token CLIBP_VARIABLE_1 CLIBP_VARIABLE_2 CLIBP_VARIABLE_3
%token CLIBP_CONSTRAINED_PIN_TRANSITION
%token CLIBP_RELATED_PIN_TRANSITION
%token CLIBP_INPUT_NET_TRANSITION
%token CLIBP_TOTAL_OUTPUT_NET_CAPACITANCE 
%token CLIBP_INDEX_1 CLIBP_INDEX_2 CLIBP_INDEX_3

//cell
%token CLIBP_CELL
%token CLIBP_AREA
%token CLIBP_FF CLIBP_CLOCKED_ON CLIBP_NEXT_STATE
//cell::pin
%token CLIBP_PIN
%token CLIBP_DIRECTION CLIBP_INPUT CLIBP_OUTPUT CLIBP_TRISTATE
%token CLIBP_FUNCTION CLIBP_THREE_STATE CLIBP_X_FUNCTION 
%token CLIBP_CAPACITANCE
//cell::pin::timing
%token CLIBP_TIMING
%token CLIBP_RELATED_PIN
//cell::pin::timing::timing_sense
%token CLIBP_TIMING_SENSE CLIBP_POSITIVE_UNATE CLIBP_NEGATIVE_UNATE
//cell::pin::timing::timing_type
%token CLIBP_TIMING_TYPE CLIBP_SETUP_RISING CLIBP_HOLD_RISING CLIBP_RISING_EDGE
%token CLIBP_THREE_STATE_ENABLE CLIBP_THREE_STATE_DISABLE
//cell::pin::timing::lu_tables
%token CLIBP_CELL_RISE CLIBP_CELL_FALL CLIBP_VALUES
%token CLIBP_RISE_TRANSITION CLIBP_FALL_TRANSITION
%token CLIBP_RISE_CONSTRAINT CLIBP_FALL_CONSTRAINT

//values
%token CLIBP_FLOAT CLIBP_STRING CLIBP_NAME CLIBP_FLOAT_LIST
%start file

%left '+' '-'
%left '*' '/' '%'

%type<_index> lu_table_template_variable lu_table_template_index
%type<_type> lu_table_template_variable_type
%type<_value> CLIBP_FLOAT 
%type<_text> CLIBP_STRING CLIBP_NAME
%type<_values> CLIBP_FLOAT_LIST
%%

file: library_header '{' library_content '}'  //ok
    ;

library_header: CLIBP_LIBRARY '(' CLIBP_NAME ')' {  if (!start_library($3)) return 0; } 
              ;
          
library_content:                              //ok
               | library_item library_content //ok
               ;

library_item: library_statement  //ok
            | lu_table_template  //ok
            ;

library_statement: CLIBP_TECHNOLOGY                    '(' CLIBP_NAME ')' ';' { start_technology(); }
                 | CLIBP_DATE                          ':' CLIBP_STRING   ';'
                 | CLIBP_DELAY_MODEL                   ':' CLIBP_NAME     ';'
                 | CLIBP_NOM_VOLTAGE                   ':' CLIBP_FLOAT    ';'
                 | CLIBP_NOM_TEMPERATURE               ':' CLIBP_FLOAT    ';'
                 | CLIBP_NOM_PROCESS                   ':' CLIBP_FLOAT    ';'
                 | CLIBP_SLEW_DERATE_FROM_LIBRARY      ':' CLIBP_FLOAT    ';'
                 | CLIBP_DEFAULT_FANOUT_LOAD           ':' CLIBP_FLOAT    ';'
                 | CLIBP_DEFAULT_INOUT_PIN_CAP         ':' CLIBP_FLOAT    ';' //TODO
                 | CLIBP_DEFAULT_INPUT_PIN_CAP         ':' CLIBP_FLOAT    ';' //TODO
                 | CLIBP_DEFAULT_OUTPUT_PIN_CAP        ':' CLIBP_FLOAT    ';' //TODO
                 | CLIBP_CAPACITIVE_LOAD_UNIT          '(' CLIBP_FLOAT ',' CLIBP_NAME ')' ';'
                            { register_unit(CLibertyTechnology::Unit::CAPACITANCE,$3,$5); }
                 | CLIBP_VOLTAGE_UNIT                  ':' CLIBP_STRING   ';'
                 | CLIBP_TIME_UNIT                     ':' CLIBP_STRING   ';' { register_unit(CLibertyTechnology::Unit::TIME,0,$3); }
                 | CLIBP_PULLING_RESISTANCE_UNIT       ':' CLIBP_STRING   ';'
                 | CLIBP_CURRENT_UNIT                  ':' CLIBP_STRING   ';'
                 | CLIBP_INPUT_THRESHOLD_PCT_RISE      ':' CLIBP_FLOAT    ';'
                 | CLIBP_INPUT_THRESHOLD_PCT_FALL      ':' CLIBP_FLOAT    ';'
                 | CLIBP_OUTPUT_THRESHOLD_PCT_RISE     ':' CLIBP_FLOAT    ';'
                 | CLIBP_OUTPUT_THRESHOLD_PCT_FALL     ':' CLIBP_FLOAT    ';'
                 | CLIBP_SLEW_LOWER_THRESHOLD_PCT_RISE ':' CLIBP_FLOAT    ';'
                 | CLIBP_SLEW_LOWER_THRESHOLD_PCT_FALL ':' CLIBP_FLOAT    ';'
                 | CLIBP_SLEW_UPPER_THRESHOLD_PCT_RISE ':' CLIBP_FLOAT    ';'
                 | CLIBP_SLEW_UPPER_THRESHOLD_PCT_FALL ':' CLIBP_FLOAT    ';'
                 ;

lu_table_template: lu_table_template_header '{' lu_table_template_content '}' //ok
                 | cell_header '{' cell_content '}'                           //ok
                 ;

lu_table_template_header: CLIBP_LU_TABLE_TEMPLATE '(' CLIBP_NAME ')' { start_luTemplate($3);  }
                         ;
                 
lu_table_template_content:                                                   //ok
                         | lu_table_template_item  lu_table_template_content //ok
                         ;

lu_table_template_item: lu_table_template_variable ':' lu_table_template_variable_type ';'
                                { luTemplate_addVar($1,(CLuTableTemplate::Variable::Type::Code)$3); }
                      | lu_table_template_index '(' CLIBP_FLOAT_LIST ')' ';'
                                { luTemplate_setVarIndexes($1,yydoubleindex,$3); }
                      ;

lu_table_template_variable: CLIBP_VARIABLE_1 { $$ = 1; }
                          | CLIBP_VARIABLE_2 { $$ = 2; }
                          | CLIBP_VARIABLE_3 { $$ = 3; }
                          ;

lu_table_template_variable_type: CLIBP_CONSTRAINED_PIN_TRANSITION
                                         { $$ = (int)CLuTableTemplate::Variable::Type::CONSTRAINED_PIN_TRANSITION; }
                               | CLIBP_RELATED_PIN_TRANSITION
                                         { $$ = (int)CLuTableTemplate::Variable::Type::RELATED_PIN_TRANSITION; }
                               | CLIBP_INPUT_NET_TRANSITION
                                         { $$ = (int)CLuTableTemplate::Variable::Type::INPUT_NET_TRANSITION; }
                               | CLIBP_TOTAL_OUTPUT_NET_CAPACITANCE 
                                         { $$ = (int)CLuTableTemplate::Variable::Type::TOTAL_OUTPUT_NET_CAPACITANCE; }
                               ;
lu_table_template_index : CLIBP_INDEX_1 { $$ = 1; }
                        | CLIBP_INDEX_2 { $$ = 2; }
                        | CLIBP_INDEX_3 { $$ = 3; }
                        ;

cell_header: CLIBP_CELL '(' CLIBP_NAME ')' { start_cell($3); }
           ;
           
cell_content:                        //ok
            | cell_item cell_content //ok
            ;



cell_item: pin_header '{' pin_content '}' //ok
         | CLIBP_AREA ':' CLIBP_FLOAT ';' //ignore
         | CLIBP_FF '(' CLIBP_NAME ',' CLIBP_NAME ')' '{' latch_content '}' //ignore
         ;

pin_header: CLIBP_PIN '(' CLIBP_NAME ')' { start_net($3); }
          ;

pin_content:                       //ok
           | pin_item pin_content  //ok
           ;

pin_item: CLIBP_DIRECTION ':' direction ';'      //ok
        | CLIBP_CAPACITANCE ':' CLIBP_FLOAT ';'  { set_net_capacitance($3);}
        | timing_header  '{' timing_content '}'  // ok
        | CLIBP_FUNCTION ':' CLIBP_STRING ';'    // ignore
        | CLIBP_THREE_STATE ':' CLIBP_STRING ';' // ignore
        | CLIBP_X_FUNCTION ':' CLIBP_STRING ';'  // ignore
        ;

direction: CLIBP_INPUT    { check_net_direction(Net::Direction::IN);}
         | CLIBP_OUTPUT   { check_net_direction(Net::Direction::OUT);}
         | CLIBP_TRISTATE { check_net_direction(Net::Direction::TRISTATE);}
         ;

timing_header: CLIBP_TIMING '(' ')' { start_cellPath(); }
             ;

timing_content:                            //ok
              | timing_item timing_content //ok
              ;

timing_item: CLIBP_RELATED_PIN ':' CLIBP_STRING ';'  { cellPath_setRelatedNet($3); }
           | CLIBP_TIMING_SENSE ':' timing_sense ';' //ok
           | CLIBP_TIMING_TYPE ':' timing_type ';'   //ok
           | lu_table_header '{' CLIBP_VALUES '(' values ')' ';' '}' //ok
           ;

timing_sense: CLIBP_POSITIVE_UNATE  { cellPath_setType(CCellPath::Type::POSITIVE_UNATE); }
            | CLIBP_NEGATIVE_UNATE  { cellPath_setType(CCellPath::Type::NEGATIVE_UNATE); } 
            ;
            
timing_type: CLIBP_SETUP_RISING     { cellPath_setType(CCellPath::Type::LATCH_SETUP); }
           | CLIBP_HOLD_RISING      { cellPath_setType(CCellPath::Type::LATCH_HOLD); }
           | CLIBP_RISING_EDGE      { cellPath_setType(CCellPath::Type::LATCH_ACCESS); }
           | CLIBP_THREE_STATE_ENABLE  //ignored
           | CLIBP_THREE_STATE_DISABLE //ignored
           ;

lu_table_header: CLIBP_CELL_RISE '(' CLIBP_NAME ')'       { cellPath_riseDelayTable($3); }
               | CLIBP_CELL_FALL '(' CLIBP_NAME ')'       { cellPath_fallDelayTable($3); }
               | CLIBP_RISE_CONSTRAINT '(' CLIBP_NAME ')' { cellPath_riseDelayTable($3); }
               | CLIBP_FALL_CONSTRAINT '(' CLIBP_NAME ')' { cellPath_fallDelayTable($3); }
               | CLIBP_RISE_TRANSITION '(' CLIBP_NAME ')' { cellPath_riseTransitionTable($3); }
               | CLIBP_FALL_TRANSITION '(' CLIBP_NAME ')' { cellPath_fallTransitionTable($3); }
               ;

values: CLIBP_FLOAT_LIST            { luTable_addValues(yydoubleindex,$1); }
      | values ',' CLIBP_FLOAT_LIST { luTable_addValues(yydoubleindex,$3); }
      ;

latch_content :                          //ignored
              | latch_item latch_content //ignored
              ;

latch_item : CLIBP_CLOCKED_ON ':' CLIBP_STRING ';' //ignored
           | CLIBP_NEXT_STATE ':' CLIBP_STRING ';' //ignored
           ;
%%

namespace {

Library* find_library(Library* containing_library,string name)
// ***********************************************************
{
    Library* library=containing_library->getLibrary(name);

    if (library)
        return library;

    for_each_library(sub_library,containing_library->getLibraries())
    {
        library=find_library(sub_library,name);
        if (library)
            return library;
        end_for;
    }

    return NULL;
}

Library* start_library(const char* name)
// *************************************
{
    current_library=NULL;
    
    DataBase *database= DataBase::getDB();
    if (!database)
        throw Error("LibertyParser(): No DataBase found !");
    current_library=find_library(database->getRootLibrary(),name);
    //ltrace(364) << " o Library " << name << " (" << current_library << ")" << endl;
    if (current_library)
        cmess1 << "  - loading timing caracteristics for library " << current_library->getName() << endl;
    else
        cmess1 << "Warning : cannot load timing caracteristics for library " << name << endl;

    return current_library; //Will abort parsing if null
}

void start_cell(const char* name)
// ******************************
{
    //current_cell=NULL;
    //if (!current_library) return;
    
    current_cell = current_library->getCell(name);
    //ltrace(363) << "   - Cell " << name << " (" << current_cell << ")" << endl;
    if (current_cell)
        cmess2 << "    - cell " << current_cell->getName() << endl;
}

void start_net(const char* name)
// *****************************
{
    current_net=NULL;
    if (!current_cell) return;
    
    current_net= current_cell->getNet(name);
    //ltrace(362) << "     * Net " << name << " (" << current_net << ")" << endl;
    if (!current_net)
        throw Error("LibertyParser(): No net "+getString(name)+" in cell "+getString(current_cell->getName()));
}

void start_technology()
// ********************
{
    //current_technology=NULL;
    //if (!current_library) return;

    current_technology=CLibertyTechnology::create();
    current_library->put(current_technology);
}

void register_unit(CLibertyTechnology::Unit unit,double val,string str)
// ********************************************************************
{
    char unit_mod;
    double mod;
    istringstream iss(str);
    if (val == 0)
       iss >> val;
    iss >> unit_mod;
    switch (unit_mod) {
        case 'G':
            mod= 1e9;break;
        case 'M':
            mod= 1e6;break;
        case 'k':
            mod= 1e3;break;
        case 'm':
            mod= 1e-3;break;
        case 'u':
            mod= 1e-6;break;
        case 'n':
            mod= 1e-9;break;
        case 'p':
            mod= 1e-12;break;
        case 'f':
            mod= 1e-15;break;
        default:
            mod= 1;
    }
    current_technology->SetUnit(unit,val*mod);
}

void start_luTemplate(string name)
// *******************************
{
    if (!current_technology)
        throw Error("LibertyParser error : cannot create luTemplate (no CLibertyTechnology)");
    current_luTemplate = CLuTableTemplate::create(name,current_technology);
}

void luTemplate_addVar(unsigned char index,CLuTableTemplate::Variable::Type type)
// ******************************************************************************
{
    if (current_luTemplate)
        current_luTemplate->AddVariable(index,type);
    else
       throw Error("LibertyParser error : adding variable to undefined luTable template");
}

void luTemplate_setVarIndexes(unsigned char index,unsigned short count,double* values)
// ***********************************************************************************
{
    if (current_luTemplate)
        current_luTemplate->SetVariableIndexes(index,count,values);
    else
       throw Error("LibertyParser error : setting variable indexes to undefined luTable template");
}


void start_cellPath()
// ******************
{
    current_cellPath = NULL;
    if (!current_net) return;
    //ltrace(361) << "       CCellPath " << current_net << endl;
    
    current_cellPath = CCellPath::create(current_net);
}

void cellPath_setRelatedNet(string name)
// *************************************
{
    if (!current_cellPath) return; //current timing bloc is beeing ignored

    Cell* cell = current_cellPath->getNet()->getCell();
    Net* relatedNet= cell->getNet(name);
    if (!relatedNet)
    {
        string msg ="No net "+name+" in cell "+getString(cell->getName());
        yyerror(msg.c_str());
    }
    //ltrace(360) << "         Related Net : " << relatedNet << endl;
    current_cellPath->_setRelatedNet(relatedNet);
}

void cellPath_setType(CCellPath::Type type)
// ****************************************
{
    if (!current_cellPath) return; //current timing bloc is beeing ignored

    current_cellPath->_setType(type);
    //ltrace(360) << "         Type : " << getString(type) << endl;
}

CLuTableTemplate* cellPath_gettemplate(string name)
// ************************************************
{
    CLuTableTemplate* lutemplate = current_technology->getTemplate(name);
    if (lutemplate == NULL)
    {
        string message = "Undefined LuTableTemplate : "+name;
        yyerror(message.c_str());
    }
    return lutemplate;
}


void cellPath_riseDelayTable(string name)
// **************************************
{
    current_luTable=NULL; 
    if (!current_cellPath) return; //current timing bloc is beeing ignored

    //ltrace(360) << "         Rise Delay Table" << endl;
    current_luTable = current_cellPath->_createRiseDelayTable(cellPath_gettemplate(name));
}

void cellPath_fallDelayTable(string name)
// **************************************
{
    current_luTable=NULL; 
    if (!current_cellPath) return; //current timing bloc is beeing ignored
    
    //ltrace(360) << "         Fall Delay Table" << endl;
    current_luTable = current_cellPath->_createFallDelayTable(cellPath_gettemplate(name));
}

void cellPath_riseTransitionTable(string name)
// *******************************************
{
    current_luTable=NULL; 
    if (!current_cellPath) return; //current timing bloc is beeing ignored

    //ltrace(360) << "         Rise Transition Table" << endl;
    current_luTable = current_cellPath->_createRiseTransitionTable(cellPath_gettemplate(name));
}

void cellPath_fallTransitionTable(string name)
// *******************************************
{
    current_luTable=NULL; 
    if (!current_cellPath) return; //current timing bloc is beeing ignored
    
    //ltrace(360) << "         Fall Transition Table" << endl;
    current_luTable = current_cellPath->_createFallTransitionTable(cellPath_gettemplate(name));
}

#if 0
void cellPath_ignored()
// ********************
{
    current_luTable=NULL; 
}
#endif

void luTable_addValues(unsigned short count, double* values)
// *********************************************************
{
    if (!current_luTable) return;

    current_luTable->AddValues(count,values);
}

void check_net_direction(Net::Direction direction)
// ***********************************************
{
    if (!current_net) return;

    if (current_net->getDirection() != direction)
        yyerror("Wrong net direction");
}

Name NetCapacitancePropName("Coriolis::NetCapacitance");

void set_net_capacitance(double value)
// ***********************************
{
    if (!current_net) return;

    Property* property = current_net->getProperty(NetCapacitancePropName);
    if (property)
    {
        yyerror("Capacitance already set");
    }
    else
    {
        property = StandardPrivateProperty<double>::create(NetCapacitancePropName, value);
        current_net->put(property);
    }
}


int  yyerror ( const char* message )
// *********************************
{
   throw Error ( "LibertyParser():\n%s:%d: %s before %s.\n"
               ,liberty_filename,libertyLineNumber,message,yytext);
   return 0;
}

} //namespace

namespace CRL {
    
void  LibertyParser ( const string& libPath)
// ****************************************
{
    liberty_filename=libPath.c_str();
    yyin = fopen(liberty_filename,"r");
    if (!yyin) {
        string errstr = strerror(errno);
        throw Error (libPath+": "+errstr);
    }
    //ltrace(365) << "LibertyParser : " << libPath << endl; ltracein(390);
    yyparse();
    //ltraceout(365);
    //ltrace(365) << libPath << " : parsing complete" << endl;
}

double getNetCapacitance(const Net* net)
// *************************************
{
    Property* property = net->getProperty(NetCapacitancePropName);
    if (!property)  
    {
        throw Error("Can't get capacitance for net "+getString(net->getName())+" : no such property");
    }
    
    if (!dynamic_cast<StandardPrivateProperty<double>*>(property))
        throw Error("Can't get capacitance for net "+getString(net->getName())+" : bad property type");

    double value=(static_cast<StandardPrivateProperty<double>*>(property))->getValue();  

    Property* technology_property = net->getCell()->getLibrary()->getProperty(CLibertyTechnology::getPropertyName());
    if (!technology_property) throw Error("Library " + getString(net->getCell()->getLibrary()->getName())+" has no CLibertyTechnology");
    if (!dynamic_cast<CLibertyTechnology*>(technology_property)) throw Error(getString(CLibertyTechnology::getPropertyName())+" : Invalid Property Type");

    return value * static_cast<CLibertyTechnology*>(technology_property)->getUnit(CLibertyTechnology::Unit::CAPACITANCE);
}

void LibertyParserError(string message)
// ***********************************
{
    yyerror(message.c_str());
}

} //namespace CRL
