
%{

#include  <iostream>
#include  <string>

#include  "AcmSigdaParserGrammar.hpp"

static std::string  nameToken;
       int          yylineno = 0;

%}

NAME		[^ #(),\t\n]+
COMMENT 	#[^\n]*

%%

{COMMENT}      { std::cout << yytext << std::endl; std::cout.flush(); }
=              { return Equal; }
\(             { return LeftParent; }
\)             { return RightParent; }
\,             { return Comma; }
INPUT          { AcmSigda_lval._value = INPUT;  return AcmSigda_lval._value; }
OUTPUT         { AcmSigda_lval._value = OUTPUT; return AcmSigda_lval._value; }
AND            { AcmSigda_lval._value = AND;    return AcmSigda_lval._value; }
NAND           { AcmSigda_lval._value = NAND;   return AcmSigda_lval._value; }
OR             { AcmSigda_lval._value = OR;     return AcmSigda_lval._value; }
NOR            { AcmSigda_lval._value = NOR;    return AcmSigda_lval._value; }
XOR            { AcmSigda_lval._value = XOR;    return AcmSigda_lval._value; }
XNOR           { AcmSigda_lval._value = XNOR;   return AcmSigda_lval._value; }
BUFF           { AcmSigda_lval._value = BUFF;   return AcmSigda_lval._value; }
DFF            { AcmSigda_lval._value = DFF;    return AcmSigda_lval._value; }
(NOT|INV)      { AcmSigda_lval._value = NOT;    return AcmSigda_lval._value; }
{NAME}         { nameToken = yytext; AcmSigda_lval._text = nameToken.c_str(); return NAME; }
\n             { yylineno++; }
.              { }

%%

int  yywrap () { return 1; }
