
%option nounput

%{

#include  <stdio.h>
#include  <cstdio>
#include  <iostream>
#include  <string>

#include  "LibertyParserGrammar.hpp"

static double       floatToken;
static std::string  nameToken;

extern int  Liberty_lineno;



%}

%x COMM

SPACE               [ \t]
UPPER_CASE_LETTER   [A-Z]
LOWER_CASE_LETTER   [a-z]
DIGIT               [0-9]
OPERATION           [\*\+\^]

INTEGER             {DIGIT}+
FLOAT               {DIGIT}+(\.{DIGIT}+)*
LETTER              {UPPER_CASE_LETTER}|{LOWER_CASE_LETTER}
ALPHA_NUM           {LETTER}+{DIGIT}*

STRING_NAME         {ALPHA_NUM}+(_{ALPHA_NUM})*

HOUR                {DIGIT}+:{DIGIT}+:{DIGIT}+
STRING_DATE         {LETTER}+{SPACE}+{LETTER}+{SPACE}+{DIGIT}+{SPACE}+{HOUR}{SPACE}+MET{SPACE}+{DIGIT}+

STRING_UNIT         {DIGIT}+{LETTER}+

OPERATION_SPACED    {SPACE}*{OPERATION}{SPACE}*
FUNCTION1           {ALPHA_NUM}'?
FUNCTION2           (\()?{FUNCTION1}{OPERATION_SPACED}{FUNCTION1}(\))?
FUNCTION3_1         (\()?{FUNCTION1}{OPERATION_SPACED}{FUNCTION1}{OPERATION_SPACED}*{FUNCTION1}(\))?
FUNCTION3_2         (\()?{FUNCTION2}{OPERATION_SPACED}{FUNCTION1}(\))?
FUNCTION3_3         (\()?{FUNCTION1}{OPERATION_SPACED}{FUNCTION2}(\))?
FUNCTION3           {FUNCTION3_1}|{FUNCTION3_2}|{FUNCTION3_3}
FUNCTION4_1         (\()?{FUNCTION1}{OPERATION_SPACED}{FUNCTION3}(\))?
FUNCTION4_2         (\()?{FUNCTION2}{OPERATION_SPACED}{FUNCTION2}(\))?
FUNCTION4_3         (\()?(\()?{FUNCTION2}{OPERATION_SPACED}{FUNCTION1}(\))?{OPERATION_SPACED}{FUNCTION1}(\))?
FUNCTION4           {FUNCTION4_1}|{FUNCTION4_2}|{FUNCTION4_3}
FUNCTION5           (\()?{FUNCTION2}{OPERATION_SPACED}{FUNCTION3}(\))?
FUNCTION6           (\()?{FUNCTION2}{OPERATION_SPACED}{FUNCTION2}(\))?{OPERATION_SPACED}{FUNCTION2}(\))?
FUNCTION7           (\()?{FUNCTION3}{OPERATION_SPACED}{FUNCTION4}(\))?
FUNCTION8           (\()?{FUNCTION4}{OPERATION_SPACED}{FUNCTION4}(\))?
FUNCTION_MUX3       (\()?{FUNCTION2}{OPERATION_SPACED}(\()?{FUNCTION1}{OPERATION_SPACED}{FUNCTION4}(\))?(\))?
FUNCTION_MUX        {FUNCTION_MUX3}
STRING_FUNCTION     {FUNCTION1}|{FUNCTION2}|{FUNCTION3}|{FUNCTION4}|{FUNCTION5}|{FUNCTION6}|{FUNCTION7}|{FUNCTION8}|{FUNCTION_MUX}

STRING_FF           [\"]?{UPPER_CASE_LETTER}+[\"]?,[\"]?{UPPER_CASE_LETTER}+[\"]?


%%

{SPACE}                         { /* spaces : skip them */ }
\n                              { Liberty_lineno++; /* end of line */ }

<INITIAL>[ \t]+
<INITIAL>"/*"                   { BEGIN(COMM); }
<COMM>.                         
<COMM>"*/"                      { BEGIN(INITIAL); }
<INITIAL,COMM>\n                { Liberty_lineno++; }
<INITIAL,COMM>\\\n              { Liberty_lineno++; }

library                         { return LIBRARY; }

date                            { return DATE; }
revision                        { return REVISION; }
                                
default_inout_pin_cap           { return DEFAULT_INOUT_PIN_CAP; }
default_inout_pin_fall_res      { return DEFAULT_INOUT_PIN_FALL_RES; }
default_inout_pin_rise_res      { return DEFAULT_INOUT_PIN_RISE_RES; }
default_input_pin_cap           { return DEFAULT_INPUT_PIN_CAP; }
default_intrinsic_fall          { return DEFAULT_INTRINSIC_FALL; }
default_intrinsic_rise          { return DEFAULT_INTRINSIC_RISE; }
default_output_pin_cap          { return DEFAULT_OUTPUT_PIN_CAP; }
default_output_pin_fall_res     { return DEFAULT_OUTPUT_PIN_FALL_RES; }
default_output_pin_rise_res     { return DEFAULT_OUTPUT_PIN_RISE_RES; }
default_slope_fall              { return DEFAULT_SLOPE_FALL; }           
default_slope_rise              { return DEFAULT_SLOPE_RISE; }
default_fanout_load             { return DEFAULT_FANOUT_LOAD; }
default_max_fanout              { return DEFAULT_MAX_FANOUT; }
default_wire_load_capacitance   { return DEFAULT_WIRE_LOAD_CAPACITANCE; }
default_wire_load_resistance    { return DEFAULT_WIRE_LOAD_RESISTANCE; }
default_wire_load_area          { return DEFAULT_WIRE_LOAD_AREA; }
default_wire_load_mode          { return DEFAULT_WIRE_LOAD_MODE; }
k_process_drive_fall            { return K_PROCESS_DRIVE_FALL; }
k_process_drive_rise            { return K_PROCESS_DRIVE_RISE; }
k_process_intrinsic_fall        { return K_PROCESS_INTRINSIC_FALL; }
k_process_intrinsic_rise        { return K_PROCESS_INTRINSIC_RISE; }
k_process_pin_cap               { return K_PROCESS_PIN_CAP; }
k_process_slope_fall            { return K_PROCESS_SLOPE_FALL; }
k_process_slope_rise            { return K_PROCESS_SLOPE_RISE; }
k_process_wire_cap              { return K_PROCESS_WIRE_CAP; }
k_process_wire_res              { return K_PROCESS_WIRE_RES; }
k_temp_drive_fall               { return K_TEMP_DRIVE_FALL; }
k_temp_drive_rise               { return K_TEMP_DRIVE_RISE; }
k_temp_intrinsic_fall           { return K_TEMP_INTRINSIC_FALL; }
k_temp_intrinsic_rise           { return K_TEMP_INTRINSIC_RISE; }
k_temp_pin_cap                  { return K_TEMP_PIN_CAP; }
k_temp_slope_fall               { return K_TEMP_SLOPE_FALL; }
k_temp_slope_rise               { return K_TEMP_SLOPE_RISE; }
k_temp_wire_cap                 { return K_TEMP_WIRE_CAP; }
k_temp_wire_res                 { return K_TEMP_WIRE_RES; }
k_volt_drive_fall               { return K_VOLT_DRIVE_FALL; }
k_volt_drive_rise               { return K_VOLT_DRIVE_RISE; }
k_volt_intrinsic_fall           { return K_VOLT_INTRINSIC_FALL; }
k_volt_intrinsic_rise           { return K_VOLT_INTRINSIC_RISE; }
k_volt_pin_cap                  { return K_VOLT_PIN_CAP; }
k_volt_slope_fall               { return K_VOLT_SLOPE_FALL; }
k_volt_slope_rise               { return K_VOLT_SLOPE_RISE; }
k_volt_wire_cap                 { return K_VOLT_WIRE_CAP; }
k_volt_wire_res                 { return K_VOLT_WIRE_RES; }
time_unit                       { return TIME_UNIT; }
voltage_unit                    { return VOLTAGE_UNIT; }
current_unit                    { return CURRENT_UNIT; }
pulling_resistance_unit         { return PULLING_RESISTANCE_UNIT; }
capacitive_load_unit            { return CAPACITIVE_LOAD_UNIT; }
nom_process                     { return NOM_PROCESS; }
nom_temperature                 { return NOM_TEMPERATURE; }
nom_voltage                     { return NOM_VOLTAGE; }
in_place_swap_mode              { return IN_PLACE_SWAP_MODE; }

wire_load                       { return WIRE_LOAD; }
resistance                      { return RESISTANCE; }
capacitance                     { return CAPACITANCE; }
area                            { return AREA; }
slope                           { return SLOPE; }
fanout_length                   { return FANOUT_LENGTH; }
wire_load_selection             { return WIRE_LOAD_SELECTION; }
wire_load_from_area             { return WIRE_LOAD_FROM_AREA; }
default_wire_load_selection     { return DEFAULT_WIRE_LOAD_SELECTION; }

cell                            { return CELL; }
cell_footprint                  { return CELL_FOOTPRINT; }
dont_use                        { return DONT_USE; }
pin                             { return PIN; }
direction                       { return DIRECTION; }
fanout_load                     { return FANOUT_LOAD; }
max_fanout                      { return MAX_FANOUT; }
function                        { return FUNCTION; }
three_state                     { return THREE_STATE; }
driver_type                     { return DRIVER_TYPE; }
timing                          { return TIMING; }
timing_sense                    { return TIMING_SENSE; }
timing_type                     { return TIMING_TYPE; }
intrinsic_rise                  { return INTRINSIC_RISE; }
intrinsic_fall                  { return INTRINSIC_FALL; }
rise_resistance                 { return RISE_RESISTANCE; }
fall_resistance                 { return FALL_RESISTANCE; }
related_pin                     { return RELATED_PIN; }
ff                              { return FF; }
next_state                      { return NEXT_STATE; }
clocked_on                      { return CLOCKED_ON; }
test_cell                       { return TEST_CELL; }
signal_type                     { return SIGNAL_TYPE; }
                                
{FLOAT}                         { floatToken = atof(yytext); Liberty_lval._value = floatToken;        return FLOAT; }
{STRING_NAME}                   { nameToken  = yytext;       Liberty_lval._text  = nameToken.c_str(); return STRING_NAME; }
{STRING_DATE}                   { nameToken  = yytext;       Liberty_lval._text  = nameToken.c_str(); return STRING_DATE; }
{STRING_UNIT}                   { nameToken  = yytext;       Liberty_lval._text  = nameToken.c_str(); return STRING_UNIT; }
{STRING_FUNCTION}               { nameToken  = yytext;       Liberty_lval._text  = nameToken.c_str(); return STRING_FUNCTION; }
{STRING_FF}                     { nameToken  = yytext;       Liberty_lval._text  = nameToken.c_str(); return STRING_FF; }
                                
.                               { /* std::cerr << "autre :" << yytext << std::endl;*/ return *yytext; /* else */ }

%%

int  yywrap () { return 1; }
