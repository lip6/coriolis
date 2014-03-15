
%option nounput

%{
#include <stdio.h>
#include <string.h>
#include  "IocParserGrammar.hpp"

#define YY_NO_UNPUT	/* Avoids warning */


%}

%%
#.*				{}
\n				{}
[Ii][Gg][Nn][Oo][Rr][Ee]	{return IGNORE;	}
[Tt][Oo][Pp]			{return TOP;	}
[Bb][Oo][Tt][Tt][Oo][Mm]	{return BOTTOM;	}
[Ll][Ee][Ff][Tt]		{return LEFT;	}
[Rr][Ii][Gg][Hh][Tt]		{return RIGHT;	}
[Ii][Oo][Pp][Ii][Nn]		{return IOPIN;	}
[Ss][Pp][Aa][Cc][Ee]		{return SPACE;	}
"("				{return PAROUV;	}
")"				{return PARFER;	}
;				{return PTVIRG;	}
[a-zA-Z][a-zA-Z0-9_]*"\."[0-9]+                 {Ioclval.text = strdup(yytext); return IOCID1; }
[a-zA-Z][a-zA-Z0-9_]*"\("[0-9]+"\)""\."[0-9]+   {Ioclval.text = strdup(yytext); return IOCID2; }
[0-9]+                          {Ioclval.value = atoi(yytext); return IOCVALUE; }
[ \t]+				{}
.				{return *yytext;}

%%

int yywrap()
{
  return 1;
}
