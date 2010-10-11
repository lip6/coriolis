%{ 
#include <stdio.h>
#include <iostream>
#include <sstream>
using namespace std;
#include<vector>
#include<boost/regex.hpp>

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Library.h"
#include "vlsisapd/liberty/Attribute.h"
#include "vlsisapd/liberty/Timing.h"
using namespace LIB;

int          Liberty_error ( const char* message );
extern int   Liberty_lex ();
extern FILE* Liberty_in;
extern int   Liberty_lineno;

static Library* library = NULL;
Name currentWireLoad = "";
Name currentCell = "";
vector<Name> listPins;
static Timing* currentTiming = NULL;

string intToString(int d) {
   ostringstream oss;
   oss << d;
   return oss.str();
}

string doubleToString(double d) {
   ostringstream oss;
   oss << d;
   return oss.str();
}

%}

%union {
    double      _value;
    const char* _text;
} 

%token LIBRARY
%token DATE REVISION
%token DEFAULT_INOUT_PIN_CAP DEFAULT_INOUT_PIN_FALL_RES DEFAULT_INOUT_PIN_RISE_RES DEFAULT_INPUT_PIN_CAP DEFAULT_INTRINSIC_FALL DEFAULT_INTRINSIC_RISE DEFAULT_OUTPUT_PIN_CAP DEFAULT_OUTPUT_PIN_FALL_RES DEFAULT_OUTPUT_PIN_RISE_RES DEFAULT_SLOPE_FALL DEFAULT_SLOPE_RISE DEFAULT_FANOUT_LOAD DEFAULT_MAX_FANOUT DEFAULT_WIRE_LOAD_CAPACITANCE  DEFAULT_WIRE_LOAD_RESISTANCE DEFAULT_WIRE_LOAD_AREA DEFAULT_WIRE_LOAD_MODE
%token K_PROCESS_DRIVE_FALL K_PROCESS_DRIVE_RISE K_PROCESS_INTRINSIC_FALL K_PROCESS_INTRINSIC_RISE K_PROCESS_PIN_CAP K_PROCESS_SLOPE_FALL K_PROCESS_SLOPE_RISE K_PROCESS_WIRE_CAP K_PROCESS_WIRE_RES K_TEMP_DRIVE_FALL K_TEMP_DRIVE_RISE K_TEMP_INTRINSIC_FALL K_TEMP_INTRINSIC_RISE K_TEMP_PIN_CAP K_TEMP_SLOPE_FALL K_TEMP_SLOPE_RISE K_TEMP_WIRE_CAP K_TEMP_WIRE_RES K_VOLT_DRIVE_FALL K_VOLT_DRIVE_RISE K_VOLT_INTRINSIC_FALL K_VOLT_INTRINSIC_RISE K_VOLT_PIN_CAP K_VOLT_SLOPE_FALL K_VOLT_SLOPE_RISE K_VOLT_WIRE_CAP K_VOLT_WIRE_RES
%token TIME_UNIT VOLTAGE_UNIT CURRENT_UNIT PULLING_RESISTANCE_UNIT CAPACITIVE_LOAD_UNIT
%token NOM_PROCESS NOM_TEMPERATURE NOM_VOLTAGE IN_PLACE_SWAP_MODE

// wire_load
%token WIRE_LOAD
%token RESISTANCE CAPACITANCE AREA SLOPE FANOUT_LENGTH
%token WIRE_LOAD_SELECTION WIRE_LOAD_FROM_AREA
%token DEFAULT_WIRE_LOAD_SELECTION

// cell
%token CELL
%token CELL_FOOTPRINT DONT_USE // AREA already done
// cell::pin
%token PIN
%token DIRECTION
%token FANOUT_LOAD // CAPACITANCE already done
%token MAX_FANOUT FUNCTION THREE_STATE
%token DRIVER_TYPE
// cell::pin::timing
%token TIMING
%token TIMING_SENSE TIMING_TYPE INTRINSIC_RISE INTRINSIC_FALL RISE_RESISTANCE FALL_RESISTANCE RELATED_PIN
// cell::ff
%token  FF NEXT_STATE CLOCKED_ON
// text_cell
%token TEST_CELL SIGNAL_TYPE

// values
%token INTEGER FLOAT
%token STRING_NAME STRING_DATE STRING_UNIT STRING_FUNCTION STRING_FF

%type<_value> INTEGER FLOAT
%type<_text>  STRING_NAME STRING_DATE STRING_UNIT STRING_FUNCTION STRING_FF

%debug

%%

file
    : library_header '{' library_content '}'
    ;

library_header
    : LIBRARY '(' STRING_NAME ')' { library = new Library ( Name($3) ); } 
    ;
                
library_content
    : // empty
    | library_item library_content
    ;

library_item
    : DATE     ':' '"' STRING_DATE '"' ';' { /*cerr << "  ° Date is : " << $4 << endl;*/ }
    | REVISION ':' FLOAT ';' { /*cerr << "  ° Revision is : " << $3 << endl;*/ }
    | library_default
    | library_wire_load
    | library_wire_load_selection
    | library_cell
    ;

library_default
    : DEFAULT_INOUT_PIN_CAP         ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_inout_pin_cap"),         Attribute::Double, value); }
    | DEFAULT_INOUT_PIN_FALL_RES    ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_inout_pin_fall_res"),    Attribute::Double, value); }
    | DEFAULT_INOUT_PIN_RISE_RES    ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_inout_pin_rise_res"),    Attribute::Double, value); }
    | DEFAULT_INPUT_PIN_CAP         ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_input_pin_cap"),         Attribute::Double, value); }
    | DEFAULT_INTRINSIC_FALL        ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_intrinsic_fall"),        Attribute::Double, value); }
    | DEFAULT_INTRINSIC_RISE        ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_intrinsic_rise"),        Attribute::Double, value); }
    | DEFAULT_OUTPUT_PIN_CAP        ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_output_pin_cap"),        Attribute::Double, value); }
    | DEFAULT_OUTPUT_PIN_FALL_RES   ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_output_pin_fall_res"),   Attribute::Double, value); }
    | DEFAULT_OUTPUT_PIN_RISE_RES   ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_output_pin_rise_res"),   Attribute::Double, value); }
    | DEFAULT_SLOPE_FALL            ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_slope_fall"),            Attribute::Double, value); }
    | DEFAULT_SLOPE_RISE            ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_slope_rise"),            Attribute::Double, value); }
    | DEFAULT_FANOUT_LOAD           ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_fanout_load"),           Attribute::Double, value); }
    | DEFAULT_MAX_FANOUT            ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_max_fanout"),            Attribute::Double, value); }
    | DEFAULT_WIRE_LOAD_CAPACITANCE ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_wire_load_capacitance"), Attribute::Double, value); }
    | DEFAULT_WIRE_LOAD_RESISTANCE  ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_wire_load_resistance"),  Attribute::Double, value); }
    | DEFAULT_WIRE_LOAD_AREA        ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("default_wire_load_area"),        Attribute::Double, value); }
    | K_PROCESS_DRIVE_FALL          ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_process_drive_fall"),          Attribute::Double, value); } 
    | K_PROCESS_DRIVE_RISE          ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_process_drive_rise"),          Attribute::Double, value); }
    | K_PROCESS_INTRINSIC_FALL      ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_process_intrinsic_fall"),      Attribute::Double, value); }
    | K_PROCESS_INTRINSIC_RISE      ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_process_intrinsic_rise"),      Attribute::Double, value); }
    | K_PROCESS_PIN_CAP             ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_process_pin_cap"),             Attribute::Double, value); }
    | K_PROCESS_SLOPE_FALL          ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_process_slope_fall"),          Attribute::Double, value); }
    | K_PROCESS_SLOPE_RISE          ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_process_slope_rise"),          Attribute::Double, value); }
    | K_PROCESS_WIRE_CAP            ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_process_wire_cap"),            Attribute::Double, value); }
    | K_PROCESS_WIRE_RES            ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_process_wire_res"),            Attribute::Double, value); }
    | K_TEMP_DRIVE_FALL             ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_temp_drive_fall"),             Attribute::Double, value); }
    | K_TEMP_DRIVE_RISE             ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_temp_drive_rise"),             Attribute::Double, value); }
    | K_TEMP_INTRINSIC_FALL         ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_temp_intrinsic_fall"),         Attribute::Double, value); }
    | K_TEMP_INTRINSIC_RISE         ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_temp_intrinsic_rise"),         Attribute::Double, value); }
    | K_TEMP_PIN_CAP                ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_temp_pin_cap"),                Attribute::Double, value); }
    | K_TEMP_SLOPE_FALL             ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_temp_slope_fall"),             Attribute::Double, value); }
    | K_TEMP_SLOPE_RISE             ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_temp_slope_rise"),             Attribute::Double, value); }
    | K_TEMP_WIRE_CAP               ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_temp_wire_cap"),               Attribute::Double, value); }
    | K_TEMP_WIRE_RES               ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_temp_wire_res"),               Attribute::Double, value); }
    | K_VOLT_DRIVE_FALL             ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_volt_drive_fall"),             Attribute::Double, value); }
    | K_VOLT_DRIVE_RISE             ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_volt_drive_rise"),             Attribute::Double, value); }
    | K_VOLT_INTRINSIC_FALL         ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_volt_intrinsic_fall"),         Attribute::Double, value); }
    | K_VOLT_INTRINSIC_RISE         ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_volt_intrinsic_rise"),         Attribute::Double, value); }
    | K_VOLT_PIN_CAP                ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_volt_pin_cap"),                Attribute::Double, value); }
    | K_VOLT_SLOPE_FALL             ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_volt_slop_fall"),              Attribute::Double, value); }
    | K_VOLT_SLOPE_RISE             ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_volt_slop_rise"),              Attribute::Double, value); }
    | K_VOLT_WIRE_CAP               ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_volt_wire_cap"),               Attribute::Double, value); }
    | K_VOLT_WIRE_RES               ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("k_volt_wire_res"),               Attribute::Double, value); }
    | NOM_PROCESS                   ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("nom_process"),                   Attribute::Double, value); }
    | NOM_TEMPERATURE               ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("nom_temperature"),               Attribute::Double, value); }
    | NOM_VOLTAGE                   ':' FLOAT ';' { string value=doubleToString($3); library->addAttribute(Name("nom_voltage"),                   Attribute::Double, value); }
    | DEFAULT_WIRE_LOAD_SELECTION ':' STRING_NAME { string value=$3; library->addAttribute(Name("default_wire_load_selection"), Attribute::String, value); }
    | DEFAULT_WIRE_LOAD_MODE ':' STRING_NAME ';' {
        string value=$3;
        if ( (value != "top") & (value != "segmented") & (value != "enclosed") ) {
            cerr << "[Liberty PARSE ERROR] Invalid value for default_wire_load_mode parameter. Valid values are top, segmented and enclosed." << endl;
            exit(1);
        }
        library->addAttribute(Name("default_wire_load_mode"), Attribute::String, value);
    }
    | TIME_UNIT ':' '"' STRING_UNIT '"' ';' { 
        string chaine=$4; 
        boost::regex expression("([0-1]+)([pn]s)");
        boost::cmatch match;
        string value, unit;
        if(boost::regex_match(chaine.c_str(), match, expression)) {
            value=match[1];
            unit=match[2];
        } else {
            cerr << "[Liberty PARSE ERROR] Invalid value for time_unit parameter. Valid values are 1ps, 10ps, 100ps, and 1ns." << endl;
            exit(1);
        }
        library->addAttribute(Name("time_unit"), Attribute::Unit, value, unit);
    }
    | VOLTAGE_UNIT ':' '"' STRING_UNIT '"' ';' { 
        string chaine=$4; 
        boost::regex expression("([0-1]+)([m]?V)");
        boost::cmatch match;
        string value, unit;
        if(boost::regex_match(chaine.c_str(), match, expression)) {
            value=match[1];
            unit=match[2];
        } else {
            cerr << "[Liberty PARSE ERROR] Invalid value for voltage_unit parameter. Valid values are 1mV, 10mV, 100mV, and 1V." << endl;
            exit(1);
        }
        library->addAttribute(Name("voltage_unit"), Attribute::Unit, value, unit);
    } 
    | CURRENT_UNIT ':' '"' STRING_UNIT '"' ';' {
        string chaine=$4; 
        boost::regex expression("([0-1]+)([um]?A)");
        boost::cmatch match;
        string value, unit;
        if(boost::regex_match(chaine.c_str(), match, expression)) {
            value=match[1];
            unit=match[2];
        } else {
            cerr << "[Liberty PARSE ERROR] Invalid value for current_unit parameter. Valid values are 1uA, 10uA, 100uA, 1mA, 10mA, 100mA, and 1A." << endl;
            exit(1);
        }
        library->addAttribute(Name("current_unit"), Attribute::Unit, value, unit);
    } 
    | PULLING_RESISTANCE_UNIT ':' '"' STRING_UNIT '"' ';' {
        string chaine=$4; 
        boost::regex expression("([0-1]+)([k]?ohm)");
        boost::cmatch match;
        string value, unit;
        if(boost::regex_match(chaine.c_str(), match, expression)) {
            value=match[1];
            unit=match[2];
        } else {
            cerr << "[Liberty PARSE ERROR] Invalid value for pulling_resistance_unit parameter. Valid values are 1ohm, 10ohm, 100ohm, and 1kohm." << endl;
            exit(1);
        }
        library->addAttribute(Name("pulling_resistance_unit"), Attribute::Unit, value, unit);
    }
    | IN_PLACE_SWAP_MODE ':' STRING_NAME ';' { 
        string value=$3;
        if ( (value != "match_footprint") & (value != "no_swapping") ) {
            cerr << "[Liberty PARSE ERROR] Invalid value for pulling_resistance_unit parameter. Valid values are match_footprint and no_swapping." << endl;
            exit(1);
        }
        library->addAttribute(Name("in_place_swap_mode"), Attribute::String, value);
    }
    | CAPACITIVE_LOAD_UNIT '(' FLOAT ',' STRING_NAME ')' ';' { string value=doubleToString($3), unit=$5; library->addAttribute(Name("capacitive_load_unit"), Attribute::Unit, value, unit); }
    ;

library_wire_load
    : wire_header '{' wire_content '}'
    ;
    
wire_header
    : WIRE_LOAD '(' '"' STRING_NAME '"' ')' { currentWireLoad=Name($4); library->addWireLoad(currentWireLoad); }
    ;

wire_content
    : // empty
    | wire_item wire_content
    ;

wire_item
    : RESISTANCE  ':' FLOAT ';' { string value=doubleToString($3); library->getWireLoad(currentWireLoad)->addAttribute(Name("resistance"),  Attribute::Double, value); }
    | CAPACITANCE ':' FLOAT ';' { string value=doubleToString($3); library->getWireLoad(currentWireLoad)->addAttribute(Name("capacitance"), Attribute::Double, value); }
    | AREA        ':' FLOAT ';' { string value=doubleToString($3); library->getWireLoad(currentWireLoad)->addAttribute(Name("area"),        Attribute::Double, value); }
    | SLOPE       ':' FLOAT ';' { string value=doubleToString($3); library->getWireLoad(currentWireLoad)->addAttribute(Name("slope"),       Attribute::Double, value); }
    | FANOUT_LENGTH '(' FLOAT ',' FLOAT ')' ';' { string value1=doubleToString($3), value2=doubleToString($5); library->getWireLoad(currentWireLoad)->addAttribute(Name("fanout_length"), Attribute::Pair, value1, value2); }
    ;

library_wire_load_selection
    : wire_selection_header '{' wire_selection_content '}'
    ;

wire_selection_header
    : WIRE_LOAD_SELECTION '(' STRING_NAME ')' { library->addWireLoadSelection(Name($3)); }
    ;

wire_selection_content
    : // empty
    | wire_selection_item wire_selection_content
    ;

wire_selection_item
    : WIRE_LOAD_FROM_AREA '(' FLOAT ',' FLOAT ',' '"' STRING_NAME '"' ')' ';'
    {
        double value1=$3, value2=$5;
        string name=$8;
        library->getWireLoadSelection()->addWireLoadArea(value1, value2, Name(name));
    }
    ;

library_cell
    : cell_header '{' cell_content '}'
    ;

cell_header
    : CELL '(' STRING_NAME ')' { currentCell = Name($3); library->addCell(currentCell); }
    ;

cell_content
    : // empty
    | cell_item cell_content
    ;

cell_item
    : AREA ':' FLOAT { /* TODO ; a la fin optionnel ? pour tous en fait ... */string value=doubleToString($3); library->getCell(currentCell)->addAttribute(Name("area"), Attribute::Double, value); }
    | CELL_FOOTPRINT ':' '"' STRING_NAME '"' ';' { string value=$4; library->getCell(currentCell)->addAttribute(Name("cell_footprint"), Attribute::String, value); }
    | DONT_USE ':' STRING_NAME ';' { string value=$3; library->getCell(currentCell)->addAttribute(Name("dont_use"), Attribute::String, value); }
    | test_cell_header '{' cell_content '}'
    | pin_header '{' pin_content '}' { listPins.clear(); }
    | ff_header '{' ff_content '}'
    ;

test_cell_header
    : TEST_CELL '(' ')'
    { 
        Name oldCell = currentCell; 
        currentCell = Name("test_cell"); 
        library->addCell(currentCell);
        library->getCell(oldCell)->setTestCell(library->getCell(currentCell));
    }
    ;

pin_header
    : PIN '(' pin_list ')' 
    ;

pin_list
    : STRING_NAME              { listPins.push_back(Name($1)); library->getCell(currentCell)->addPin(Name($1)); }
    | pin_list ',' STRING_NAME { listPins.push_back(Name($3)); library->getCell(currentCell)->addPin(Name($3)); }
    ;

pin_content
    : // empty
    | pin_item pin_content
    ;

pin_item
    : CAPACITANCE ':' FLOAT ';'
    { 
        string value=doubleToString($3);
        Cell* cell = library->getCell(currentCell);
        for (size_t i = 0 ; i < listPins.size() ; i++)
            cell->getPin(listPins[i])->addAttribute(Name("capacitance"), Attribute::Double, value);
    }
    | FANOUT_LOAD ':' FLOAT ';'
    { 
        string value=doubleToString($3);
        Cell* cell = library->getCell(currentCell);
        for (size_t i = 0 ; i < listPins.size() ; i++)
            cell->getPin(listPins[i])->addAttribute(Name("fanout_load"), Attribute::Double, value);
    }
    | MAX_FANOUT ':' FLOAT ';'
    { 
        string value=doubleToString($3);
        Cell* cell = library->getCell(currentCell);
        for (size_t i = 0 ; i < listPins.size() ; i++)
            cell->getPin(listPins[i])->addAttribute(Name("max_fanout"), Attribute::Double, value);
    } 
    | THREE_STATE ':' '"' STRING_FUNCTION '"' ';' 
    { 
        string value=$4; 
        Cell* cell = library->getCell(currentCell);
        for (size_t i = 0 ; i < listPins.size() ; i++)
            cell->getPin(listPins[i])->addAttribute(Name("three_state"), Attribute::String, value);
    }
    | DRIVER_TYPE ':' STRING_NAME ';'
    {
        string value=$3;
        if ( (value != "pull_up") & (value != "pull_down") ) {
            cerr << "[Liberty PARSE ERROR] Invalid value for driver_type parameter. Valid values are pull_up and pull_down." << endl;
            exit(1);
        }
        Cell* cell = library->getCell(currentCell);
        for (size_t i = 0 ; i < listPins.size() ; i++)
            cell->getPin(listPins[i])->addAttribute(Name("driver_type"), Attribute::String, value);
    }
    | FUNCTION ':' '"' STRING_FUNCTION '"' ';' 
    { 
        string value=$4; 
        Cell* cell = library->getCell(currentCell);
        for (size_t i = 0 ; i < listPins.size() ; i++)
            cell->getPin(listPins[i])->addAttribute(Name("function"), Attribute::String, value);
   }
    | FUNCTION ':' '"' STRING_NAME '"' ';'
    {
        /* STRING_NAME matched in case of a buffer */
        string value=$4; 
        Cell* cell = library->getCell(currentCell);
        for (size_t i = 0 ; i < listPins.size() ; i++)
            cell->getPin(listPins[i])->addAttribute(Name("function"), Attribute::String, value);
    }
    | FUNCTION ':' '"' FLOAT '"' ';' 
    {   
        /* FLOAT matched in case of one and zero */
        string value=doubleToString($4); 
        Cell* cell = library->getCell(currentCell);
        for (size_t i = 0 ; i < listPins.size() ; i++)
            cell->getPin(listPins[i])->addAttribute(Name("function"), Attribute::String, value);
    }
    | DIRECTION ':' STRING_NAME ';'
    {
        string value=$3;
        if ( (value != "input") & (value != "output") ) {
            cerr << "[Liberty PARSE ERROR] Invalid value for direction parameter. Valid values are input and output." << endl;
            exit(1);
        }        
        Cell* cell = library->getCell(currentCell);
        for (size_t i = 0 ; i < listPins.size() ; i++)
            cell->getPin(listPins[i])->addAttribute(Name("direction"), Attribute::String, value);
    }
    | SIGNAL_TYPE ':' STRING_NAME ';'  {
        string value=$3;
        if ( (value != "test_scan_in") & (value != "test_scan_in_inverted") & (value != "test_scan_out") & (value != "test_scan_out_inverted") & (value != "test_scan_enable") & (value != "test_scan_enable_inverted") & (value != "test_scan_clock") & (value != "test_scan_clock_a") & (value != "test_scan_clock_b") & (value != "test_clock") ) {
            cerr << "[Liberty PARSE ERROR] Invalid value for signal_type parameter. Valid values are test_scan_in, test_scan_in_inverted, test_scan_out, test_scan_out_inverted, test_scan_enable, test_scan_enable_inverted, test_scan_clock, test_scan_clock_a, test_scan_clock_b and test_clock." << endl;
            exit(1);
        }        
        Cell* cell = library->getCell(currentCell);
        for (size_t i = 0 ; i < listPins.size() ; i++)
            cell->getPin(listPins[i])->addAttribute(Name("signal_type"), Attribute::String, value);
    }
    | timing_header '{' timing_content '}'
    ;

timing_header
    : TIMING '(' ')' 
    {
        library->getCell(currentCell)->getPin(listPins.back())->addTiming();
        currentTiming=library->getCell(currentCell)->getPin(listPins.back())->getTimings().back();
        // only for 1 pin, not available for pins' list
        if(listPins.size()>1)
            cerr << "[Warning] timing information for a list of pins, not available." << endl;
    }
    ;

timing_content
    : // empty
    | timing_item timing_content
    ;

timing_item
    : INTRINSIC_RISE  ':' FLOAT       ';' { string value=doubleToString($3); currentTiming->addAttribute(Name("intrinsic_rise"),  Attribute::Double, value); }
    | INTRINSIC_FALL  ':' FLOAT       ';' { string value=doubleToString($3); currentTiming->addAttribute(Name("intrinsic_fall"),  Attribute::Double, value); }
    | RISE_RESISTANCE ':' FLOAT       ';' { string value=doubleToString($3); currentTiming->addAttribute(Name("rise_resistance"), Attribute::Double, value); }
    | FALL_RESISTANCE ':' FLOAT       ';' { string value=doubleToString($3); currentTiming->addAttribute(Name("fall_resistance"), Attribute::Double, value); }
    | TIMING_SENSE    ':' STRING_NAME ';' { string value=$3; currentTiming->addAttribute(Name("timing_sense"), Attribute::String, value); }
    | TIMING_TYPE     ':' STRING_NAME ';' { string value=$3; currentTiming->addAttribute(Name("timing_type"),  Attribute::String, value); }
    | RELATED_PIN ':' '"' STRING_NAME '"' ';' { string value=$4; currentTiming->addAttribute(Name("related_pin"),  Attribute::String, value); }
    ;

ff_header
    : FF '(' STRING_FF ')'
    { 
        string chaine=$3;
        boost::regex expression("\"?([A-Z]+)\"?,\"?([A-Z]+)\"?");
        boost::cmatch match;
        string value1, value2;
        if(boost::regex_match(chaine.c_str(), match, expression)) {
            value1=match[1];
            value2=match[2];
        } else {
            cerr << "[Liberty PARSE ERROR] Invalid syntax for ff group." << endl;
            exit(1);
        }
        library->getCell(currentCell)->addFF(Name(value1), Name(value2));
    }
    ;

ff_content
    : // empty
    | ff_item ff_content
    ;

ff_item
    : NEXT_STATE ':' '"' STRING_FUNCTION '"' ';' { string value=$4; library->getCell(currentCell)->getFF()->addAttribute(Name("next_state"), Attribute::String, value); }
    | NEXT_STATE ':' '"' STRING_NAME '"' ';'     { string value=$4; library->getCell(currentCell)->getFF()->addAttribute(Name("next_state"), Attribute::String, value); /* STRING_NAME matched in case of a buffer */ }
    | CLOCKED_ON ':' '"' STRING_NAME '"' ';'     { string value=$4; library->getCell(currentCell)->getFF()->addAttribute(Name("clocked_on"), Attribute::String, value); }
    ;

%%

int  Liberty_error ( const char* message )
// ***************************************
{
   cerr << endl << "libertyParser(): " << message << " at line " << Liberty_lineno << endl << endl;
   return 1;
}

void read ( std::string fileName )
// *******************************
{
    const char* fileC = fileName.c_str ();
    Liberty_in = fopen ( fileC, "r" );
    if ( !Liberty_in ) {
        cerr << "libertyParser(): no file found with the name " << fileC << ".";
    }
    yyparse ();
}

namespace LIB{
Library* Library::readFromFile ( std::string fileName )
// ****************************************************
{
    read ( fileName );
    return library;
}
} // namespace
