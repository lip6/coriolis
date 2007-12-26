%{

#include <map>
#include <vector>
#include <iostream>
using namespace std;
    
#include "ParserDtrGram.hpp"

#define YY_NO_UNPUT

int DTRlineno = 1;

%}
%%
([ \t]+|#.*)    {/* nothing to do */}
\n               { DTRlineno++; return '\n'; }
TABLE_1DM        { return T_TABLE;}
END_TABLE_1DM    { return T_TABLE_END;}
[1-9]?[0-9]*(\.)?[0-9]+  { dtrlval._value=atof(dtrtext);  return T_DOUBLE;}
[a-zA-Z0-9]+(_?[a-zA-Z0-9]+)*  {dtrlval._text=strdup(dtrtext); return T_ID;}
.  { return *dtrtext; }

%%

int dtrwrap() {return 1;}

