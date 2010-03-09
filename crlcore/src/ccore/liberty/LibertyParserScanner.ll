%{
#include  "LibertyParserGrammar.hpp"

#include <string.h>
#include <stdio.h>

extern int libertyLineNumber;
extern int yydoubleindex;
%}

%x COMM
%x FLOAT_STRING
%%

<INITIAL>[ \t]+
<INITIAL>"/*"       { BEGIN(COMM); }
<COMM>.
<COMM>"*/"          { BEGIN(INITIAL); }
<INITIAL,COMM>\n    { libertyLineNumber++; }
<INITIAL,COMM>\\\n  { libertyLineNumber++; }


<INITIAL>[lL][iI][bB][rR][aA][rR][yY]               { return CLIBP_LIBRARY; }

<INITIAL>[tT][eE][cC][hH][nN][oO][lL][oO][gG][yY]   { return CLIBP_TECHNOLOGY; }
<INITIAL>[dD][aA][tT][eE]                           { return CLIBP_DATE; }
<INITIAL>[dD][eE][lL][aA][yY]_[mM][oO][dD][eE][lL]  { return CLIBP_DELAY_MODEL; }
<INITIAL>[nN][oO][mM]_[vV][oO][lL][tT][aA][gG][eE]  { return CLIBP_NOM_VOLTAGE; }
<INITIAL>[nN][oO][mM]_[tT][eE][mM][pP][eE][rR][aA][tT][uU][rR][eE] {
                                                    return CLIBP_NOM_TEMPERATURE; }
<INITIAL>[nN][oO][mM]_[pP][rR][oO][cC][eE][sS][sS]  { return CLIBP_NOM_PROCESS; }
<INITIAL>[sS][lL][eE][wW]_[dD][eE][rR][aA][tT][eE]_[fF][rR][oO][mM]_[lL][iI][bB][rR][aA][rR][yY] {
                                                      return CLIBP_SLEW_DERATE_FROM_LIBRARY; }
<INITIAL>[dD][eE][fF][aA][uU][lL][tT]_[fF][aA][nN][oO][uU][tT]_[lL][oO][aA][dD] {
                                                      return CLIBP_DEFAULT_FANOUT_LOAD; }
<INITIAL>[dD][eE][fF][aA][uU][lL][tT]_[iI][nN][oO][uU][tT]_[pP][iI][nN]_[cC][aA][pP] {
                                                      return CLIBP_DEFAULT_INOUT_PIN_CAP; }
<INITIAL>[dD][eE][fF][aA][uU][lL][tT]_[iI][nN][pP][uU][tT]_[pP][iI][nN]_[cC][aA][pP] {
                                                      return CLIBP_DEFAULT_INPUT_PIN_CAP; }
<INITIAL>[dD][eE][fF][aA][uU][lL][tT]_[oO][uU][tT][pP][uU][tT]_[pP][iI][nN]_[cC][aA][pP] {
                                                      return CLIBP_DEFAULT_OUTPUT_PIN_CAP; }
<INITIAL>[cC][aA][pP][aA][cC][iI][tT][iI][vV][eE]_[lL][oO][aA][dD]_[uU][nN][iI][tT] {
                                                      return CLIBP_CAPACITIVE_LOAD_UNIT; }
<INITIAL>[vV][oO][lL][tT][aA][gG][eE]_[uU][nN][iI][tT] {
                                                      return CLIBP_VOLTAGE_UNIT; }
<INITIAL>[tT][iI][mM][eE]_[uU][nN][iI][tT]          { return CLIBP_TIME_UNIT; }
<INITIAL>[pP][uU][lL][lL][iI][nN][gG]_[rR][eE][sS][iI][sS][tT][aA][nN][cC][eE]_[uU][nN][iI][tT] {
                                                      return CLIBP_PULLING_RESISTANCE_UNIT; }
<INITIAL>[cC][uU][rR][rR][eE][nN][tT]_[uU][nN][iI][tT] {
                                                      return CLIBP_CURRENT_UNIT; }
<INITIAL>[iI][nN][pP][uU][tT]_[tT][hH][rR][eE][sS][hH][oO][lL][dD]_[pP][cC][tT]_[rR][iI][sS][eE] {
                                                      return CLIBP_INPUT_THRESHOLD_PCT_RISE; }
<INITIAL>[iI][nN][pP][uU][tT]_[tT][hH][rR][eE][sS][hH][oO][lL][dD]_[pP][cC][tT]_[fF][aA][lL][lL] {
                                                      return CLIBP_INPUT_THRESHOLD_PCT_FALL; }
<INITIAL>[oO][uU][tT][pP][uU][tT]_[tT][hH][rR][eE][sS][hH][oO][lL][dD]_[pP][cC][tT]_[rR][iI][sS][eE] {
                                                      return CLIBP_OUTPUT_THRESHOLD_PCT_RISE; }
<INITIAL>[oO][uU][tT][pP][uU][tT]_[tT][hH][rR][eE][sS][hH][oO][lL][dD]_[pP][cC][tT]_[fF][aA][lL][lL] {
                                                      return CLIBP_OUTPUT_THRESHOLD_PCT_FALL; }
<INITIAL>[sS][lL][eE][wW]_[lL][oO][wW][eE][rR]_[tT][hH][rR][eE][sS][hH][oO][lL][dD]_[pP][cC][tT]_[rR][iI][sS][eE] {
                                                      return CLIBP_SLEW_LOWER_THRESHOLD_PCT_RISE; }
<INITIAL>[sS][lL][eE][wW]_[lL][oO][wW][eE][rR]_[tT][hH][rR][eE][sS][hH][oO][lL][dD]_[pP][cC][tT]_[fF][aA][lL][lL] {
                                                      return CLIBP_SLEW_LOWER_THRESHOLD_PCT_FALL; }
<INITIAL>[sS][lL][eE][wW]_[uU][pP][pP][eE][rR]_[tT][hH][rR][eE][sS][hH][oO][lL][dD]_[pP][cC][tT]_[rR][iI][sS][eE] {
                                                      return CLIBP_SLEW_UPPER_THRESHOLD_PCT_RISE; }
<INITIAL>[sS][lL][eE][wW]_[uU][pP][pP][eE][rR]_[tT][hH][rR][eE][sS][hH][oO][lL][dD]_[pP][cC][tT]_[fF][aA][lL][lL] {
                                                      return CLIBP_SLEW_UPPER_THRESHOLD_PCT_FALL; }

<INITIAL>[lL][uU]_[tT][aA][bB][lL][eE]_[tT][eE][mM][pP][lL][aA][tT][eE] {
                                                      return CLIBP_LU_TABLE_TEMPLATE; }

<INITIAL>[vV][aA][rR][iI][aA][bB][lL][eE]_1         { return CLIBP_VARIABLE_1; }
<INITIAL>[vV][aA][rR][iI][aA][bB][lL][eE]_2         { return CLIBP_VARIABLE_2; }
<INITIAL>[vV][aA][rR][iI][aA][bB][lL][eE]_3         { return CLIBP_VARIABLE_3; }

<INITIAL>[cC][oO][nN][sS][tT][rR][aA][iI][nN][eE][dD]_[pP][iI][nN]_[tT][rR][aA][nN][sS][iI][tT][iI][oO][nN] {
                                                      return CLIBP_CONSTRAINED_PIN_TRANSITION; }
<INITIAL>[rR][eE][lL][aA][tT][eE][dD]_[pP][iI][nN]_[tT][rR][aA][nN][sS][iI][tT][iI][oO][nN] {
                                                      return CLIBP_RELATED_PIN_TRANSITION; }
<INITIAL>[iI][nN][pP][uU][tT]_[nN][eE][tT]_[tT][rR][aA][nN][sS][iI][tT][iI][oO][nN] {
                                                      return CLIBP_INPUT_NET_TRANSITION; }
<INITIAL>[tT][oO][tT][aA][lL]_[oO][uU][tT][pP][uU][tT]_[nN][eE][tT]_[cC][aA][pP][aA][cC][iI][tT][aA][nN][cC][eE] {
                                                      return CLIBP_TOTAL_OUTPUT_NET_CAPACITANCE; }

<INITIAL>[iI][nN][dD][eE][xX]_1                     { return CLIBP_INDEX_1; }
<INITIAL>[iI][nN][dD][eE][xX]_2                     { return CLIBP_INDEX_2; }
<INITIAL>[iI][nN][dD][eE][xX]_3                     { return CLIBP_INDEX_3; }

<INITIAL>[cC][eE][lL][lL]                           { return CLIBP_CELL; }
<INITIAL>[aA][rR][eE][aA]                           { return CLIBP_AREA; }

<INITIAL>[fF][fF]                                   { return CLIBP_FF; }
<INITIAL>[cC][lL][oO][cC][kK][eE][dD]_[oO][nN]      { return CLIBP_CLOCKED_ON; }
<INITIAL>[nN][eE][xX][tT]_[sS][tT][aA][tT][eE]      { return CLIBP_NEXT_STATE; }

<INITIAL>[pP][iI][nN]                               { return CLIBP_PIN; }
<INITIAL>[dD][iI][rR][eE][cC][tT][iI][oO][nN]       { return CLIBP_DIRECTION; }
<INITIAL>[iI][nN][pP][uU][tT]                       { return CLIBP_INPUT; }
<INITIAL>[oO][uU][tT][pP][uU][tT]                   { return CLIBP_OUTPUT; }
<INITIAL>[tT][rR][iI][sS][tT][aA][tT][eE]           { return CLIBP_TRISTATE; }

<INITIAL>[cC][aA][pP][aA][cC][iI][tT][aA][nN][cC][eE] {
                                                      return CLIBP_CAPACITANCE; }
<INITIAL>[fF][uU][nN][cC][tT][iI][oO][nN]           { return CLIBP_FUNCTION; }
<INITIAL>[tT][hH][rR][eE][eE]_[sS][tT][aA][tT][eE]  { return CLIBP_THREE_STATE; }
<INITIAL>[xX]_[fF][uU][nN][cC][tT][iI][oO][nN]      { return CLIBP_X_FUNCTION; }

<INITIAL>[tT][iI][mM][iI][nN][gG]                   { return CLIBP_TIMING; }
<INITIAL>[rR][eE][lL][aA][tT][eE][dD]_[pP][iI][nN]  { return CLIBP_RELATED_PIN; }
<INITIAL>[tT][iI][mM][iI][nN][gG]_[sS][eE][nN][sS][eE] {
                                                      return CLIBP_TIMING_SENSE; }
<INITIAL>[tT][iI][mM][iI][nN][gG]_[tT][yY][pP][eE]  { return CLIBP_TIMING_TYPE; }
<INITIAL>[pP][oO][sS][iI][tT][iI][vV][eE]_[uU][nN][aA][tT][eE] {
                                                      return CLIBP_POSITIVE_UNATE; }
<INITIAL>[nN][eE][gG][aA][tT][iI][vV][eE]_[uU][nN][aA][tT][eE] {
                                                      return CLIBP_NEGATIVE_UNATE; }
<INITIAL>[sS][eE][tT][uU][pP]_[rR][iI][sS][iI][nN][gG] {
                                                      return CLIBP_SETUP_RISING; }
<INITIAL>[hH][oO][lL][dD]_[rR][iI][sS][iI][nN][gG]  { return CLIBP_HOLD_RISING; }
<INITIAL>[rR][iI][sS][iI][nN][gG]_[eE][dD][gG][eE]  { return CLIBP_RISING_EDGE; }
<INITIAL>[tT][hH][rR][eE][eE]_[sS][tT][aA][tT][eE]_[eE][nN][aA][bB][lL][eE] {
                                                      return CLIBP_THREE_STATE_ENABLE; }
<INITIAL>[tT][hH][rR][eE][eE]_[sS][tT][aA][tT][eE]_[dD][iI][sS][aA][bB][lL][eE] {
                                                      return CLIBP_THREE_STATE_DISABLE; }
<INITIAL>[cC][eE][lL][lL]_[rR][iI][sS][eE]          { return CLIBP_CELL_RISE; }
<INITIAL>[cC][eE][lL][lL]_[fF][aA][lL][lL]          { return CLIBP_CELL_FALL; }
<INITIAL>[rR][iI][sS][eE]_[cC][oO][nN][sS][tT][rR][aA][iI][nN][tT] {
                                                      return CLIBP_RISE_CONSTRAINT; }
<INITIAL>[fF][aA][lL][lL]_[cC][oO][nN][sS][tT][rR][aA][iI][nN][tT] {
                                                      return CLIBP_FALL_CONSTRAINT; }
<INITIAL>[rR][iI][sS][eE]_[tT][rR][aA][nN][sS][iI][tT][iI][oO][nN] {
                                                      return CLIBP_RISE_TRANSITION; }
<INITIAL>[fF][aA][lL][lL]_[tT][rR][aA][nN][sS][iI][tT][iI][oO][nN] {
                                                      return CLIBP_FALL_TRANSITION; }
<INITIAL>[vV][aA][lL][uU][eE][sS]                   { return CLIBP_VALUES; }




<INITIAL>-?[0-9]+(\.[0-9]+)?([eE][+-]?[0-9])? {
            LIBERTYlval._value=atof(yytext);
            return CLIBP_FLOAT; }
            
 /*<INITIAL>[a-zA-Z][a-zA-Z0-9_]*\.lib {
            strcpy(LIBERTYlval._text,yytext);
            LIBERTYlval._text[strlen(LIBERTYlval._text)-4]='\0';
            return CLIBP_LIBNAME; } */
            
<INITIAL>[a-zA-Z][a-zA-Z0-9_]* {
            strcpy(LIBERTYlval._text,yytext);
            return CLIBP_NAME; }

<INITIAL>\"-?[0-9]+(\.[0-9]+)?(,\ -?[0-9]+(\.[0-9]+)?)+\" {
            yydoubleindex=0;
            BEGIN(FLOAT_STRING);
            yyless(1); }
            
<FLOAT_STRING>[ ,]* 

<FLOAT_STRING>-?[0-9]+(\.[0-9]+)? {
            LIBERTYlval._values[yydoubleindex++]=atof(yytext); }

<FLOAT_STRING>\" {
            BEGIN(INITIAL);
            return CLIBP_FLOAT_LIST; }


<INITIAL>\"[^\"]*\" {
            strcpy(LIBERTYlval._text,yytext+1);
            LIBERTYlval._text[strlen(LIBERTYlval._text)-1]='\0';
            return CLIBP_STRING; }

<INITIAL>.  { return *yytext; }

%%
int yywrap() { return -1; }
