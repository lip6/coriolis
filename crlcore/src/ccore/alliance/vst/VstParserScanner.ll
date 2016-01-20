
%option nounput

%{

// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  Lex         :  "./VstParserScanner.ll"                         |
// |                                                                 |
// |  This file is based on the Alliance VHDL parser written by      |
// |       L.A. Tabusse, Vuong H.N., P. Bazargan-Sabet & D. Hommais  |
// |                                                                 |
// |  Lexical VHDL Analyser. Please refer to the IEEE Standard VHDL  |
// |  LRM Chapter 13 : Lexical Elements.                             |
// +-----------------------------------------------------------------+


# include  <string.h>

# include  <iostream>
# include  <string>
# include  <vector>
# include  <map>

using namespace std;

# include  "VstParserGrammar.hpp"



namespace Vst {

  extern void  ClearIdentifiers ();
  extern void  incVhdLineNumber ();

}


namespace {

    class VHDLKeywords : public map<string,int> {
      public:
        VHDLKeywords ();
    };


    VHDLKeywords::VHDLKeywords () {
      (*this)[ "abs"              ] = ABS;
      (*this)[ "access"           ] = ACCESS;
      (*this)[ "after"            ] = AFTER;
      (*this)[ "alias"            ] = ALIAS;
      (*this)[ "all"              ] = ALL;
      (*this)[ "and"              ] = tok_AND;
      (*this)[ "architecture"     ] = ARCHITECTURE;
      (*this)[ "arg"              ] = ARG;
      (*this)[ "array"            ] = ARRAY;
      (*this)[ "assert"           ] = ASSERT;
      (*this)[ "attribute"        ] = ATTRIBUTE;
			          
      (*this)[ "begin"            ] = _BEGIN;
      (*this)[ "bit"              ] = BIT;
      (*this)[ "bit_vector"       ] = BIT_VECTOR;
      (*this)[ "block"            ] = BLOCK;
      (*this)[ "body"             ] = BODY;
      (*this)[ "buffer"           ] = BUFFER;
      (*this)[ "bus"              ] = BUS;
			          
      (*this)[ "case"             ] = CASE;
      (*this)[ "component"        ] = COMPONENT;
      (*this)[ "configuration"    ] = CONFIGURATION;
      (*this)[ "constant"         ] = CONSTANT;
			          
      (*this)[ "disconnect"       ] = DISCONNECT;
      (*this)[ "downto"           ] = DOWNTO;
			          
      (*this)[ "else"             ] = ELSE;
      (*this)[ "elsif"            ] = ELSIF;
      (*this)[ "end"              ] = _END;
      (*this)[ "entity"           ] = ENTITY;
      (*this)[ "error"            ] = ERROR;
      (*this)[ "exit"             ] = _EXIT;
			          
      (*this)[ "file"             ] = _FILE;
      (*this)[ "for"              ] = FOR;
      (*this)[ "function"         ] = FUNCTION;
			          
      (*this)[ "generate"         ] = GENERATE;
      (*this)[ "generic"          ] = GENERIC;
      (*this)[ "guarded"          ] = GUARDED;
			          
      (*this)[ "if"               ] = IF;
      (*this)[ "in"               ] = _IN;
      (*this)[ "inout"            ] = _INOUT;
      (*this)[ "integer"          ] = INTEGER;
      (*this)[ "is"               ] = IS;
			          
      (*this)[ "label"            ] = _LABEL;
      (*this)[ "library"          ] = LIBRARY;
      (*this)[ "linkage"          ] = _LINKAGE;
      (*this)[ "list"             ] = _LIST;
      (*this)[ "loop"             ] = LOOP;
			          
      (*this)[ "map"              ] = MAP;
      (*this)[ "mod"              ] = MOD;
      (*this)[ "mux_bit"          ] = MUX_BIT;
      (*this)[ "mux_vector"       ] = MUX_VECTOR;
			          
      (*this)[ "nand"             ] = _NAND;
      (*this)[ "natural"          ] = NATURAL;
      (*this)[ "new"              ] = NEW;
      (*this)[ "next"             ] = _NEXT;
      (*this)[ "nor"              ] = _NOR;
      (*this)[ "not"              ] = _NOT;
      (*this)[ "null"             ] = tok_NULL;
			          
      (*this)[ "of"               ] = OF;
      (*this)[ "on"               ] = ON;
      (*this)[ "open"             ] = OPEN;
      (*this)[ "or"               ] = _OR;
      (*this)[ "others"           ] = OTHERS;
      (*this)[ "out"              ] = _OUT;
			          
      (*this)[ "package"          ] = _PACKAGE;
      (*this)[ "port"             ] = PORT;
      (*this)[ "positive"         ] = POSITIVE;
      (*this)[ "procedure"        ] = PROCEDURE;
      (*this)[ "process"          ] = PROCESS;
			          
      (*this)[ "range"            ] = RANGE;
      (*this)[ "record"           ] = RECORD;
      (*this)[ "reg_bit"          ] = REG_BIT;
      (*this)[ "reg_vector"       ] = REG_VECTOR;
      (*this)[ "register"         ] = REGISTER;
      (*this)[ "rem"              ] = REM;
      (*this)[ "report"           ] = REPORT;
      (*this)[ "return"           ] = RETURN;
			          
      (*this)[ "select"           ] = SELECT;
      (*this)[ "severity"         ] = SEVERITY;
      (*this)[ "signal"           ] = SIGNAL;
      (*this)[ "stable"           ] = _STABLE;
      (*this)[ "std_logic"        ] = STD_LOGIC;
      (*this)[ "std_logic_vector" ] = STD_LOGIC_VECTOR;
      (*this)[ "string"           ] = STRING;
      (*this)[ "subtype"          ] = SUBTYPE;
			          
      (*this)[ "then"             ] = THEN;
      (*this)[ "to"               ] = TO;
      (*this)[ "transport"        ] = TRANSPORT;
      (*this)[ "type"             ] = _TYPE;
			          
      (*this)[ "units"            ] = UNITS;
      (*this)[ "until"            ] = UNTIL;
      (*this)[ "use"              ] = USE;
			          
      (*this)[ "variable"         ] = VARIABLE;
			          
      (*this)[ "wait"             ] = WAIT;
      (*this)[ "warning"          ] = WARNING;
      (*this)[ "when"             ] = WHEN;
      (*this)[ "while"            ] = WHILE;
      (*this)[ "with"             ] = WITH;
      (*this)[ "wor_bit"          ] = WOR_BIT;
      (*this)[ "wor_vector"       ] = WOR_VECTOR;
			          
      (*this)[ "xor"              ] = _XOR;
    }


    class Identifiers : public vector<string*> {
      public:
        ~Identifiers ();
        void clear   ();
    };


    Identifiers::~Identifiers () {
      for ( unsigned int i=0 ; i < size() ; i++ )
        delete (*this)[i];
    }


    void  Identifiers::clear () {
      for ( unsigned int i=0 ; i < size() ; i++ )
        delete (*this)[i];

      vector<string*>::clear ();
    }


    VHDLKeywords vhdlKeywords;
    Identifiers  identifiers;

    char* lower ( char* str );

}


%}


upper_case_letter         [A-Z]
digit                     [0-9]
special_character         [\#\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\_\|]
space_character           [ \t]
format_effector           [\t\v\r\l\f]
end_of_line               \n
lower_case_letter         [a-z]
other_special_character   [\!\$\@\?\[\\\]\^\`\{\}\~]

graphic_character         ({basic_graphic_character}|{lower_case_letter}|{other_special_character})
basic_graphic_character   ({upper_case_letter}|{digit}|{special_character}|{space_character})
letter                    ({upper_case_letter}|{lower_case_letter})
letter_or_digit           ({letter}|{digit})
decimal_literal           {integer}(\.{integer})?({exponent})?
integer                   {digit}(_?{digit})*
exponent                  ([eE][-+]?{integer})
base                      {integer}
based_integer             {extended_digit}(_?{extended_digit})*
extended_digit            ({digit}|[a-fA-F])
base_specifier            (B|b|O|o|X|x)


%%


{space_character}         { /* separators : skip them. */ }
\&                        { return Ampersand;  }
\'                        { return Apostrophe; }
\(                        { return LeftParen;  }
\)                        { return RightParen; }
"**"                      { return DoubleStar; }
\*                        { return Star;       }
\+                        { return Plus;       }
\,                        { return Comma;      }
\-                        { return Minus;      }
":="                      { return VarAsgn;    }
\:                        { return Colon;      }
\;                        { return Semicolon;  }
"<="                      { return _LESym;     }
">="                      { return _GESym;     }
\<                        { return _LTSym;     }
\>                        { return _GTSym;     }
=                         { return _EQSym;     }
\/=                       { return _NESym;     }
"=>"                      { return Arrow;      }
"<>"                      { return _Box;       }
\|                        { return Bar;        }
!                         { return Bar;        }
\.                        { return Dot;        }
\/                        { return Slash;      }

{letter}(_?{letter_or_digit})*  {
      VHDLKeywords::iterator it = vhdlKeywords.find ( lower(yytext) );

      if ( it != vhdlKeywords.end() ) { return it->second; }

      VSTlval._text = new string ( yytext );
      identifiers.push_back ( VSTlval._text );

      return Identifier;
    }

({decimal_literal})|({base}#{based_integer}(\.{based_integer})?#({exponent})?)|({base}:{based_integer}(\.{based_integer})?:({exponent})?)  {
      VSTlval._text = new string ( yytext );
      identifiers.push_back ( VSTlval._text );
      return AbstractLit;
    }

'({graphic_character}|\"|\%)'  {
      VSTlval._text = new string ( yytext );
      identifiers.push_back ( VSTlval._text );
      return CharacterLit;
    }

(\"({graphic_character}|(\"\")|\%)*\")|(\%({graphic_character}|(\%\%)|\")*\%)  {
      VSTlval._text = new string ( yytext );
      identifiers.push_back ( VSTlval._text );
      return StringLit;
    }

{base_specifier}(\"{extended_digit}(_?{extended_digit})*\"|\%{extended_digit}(_?{extended_digit})*\%)  {
      VSTlval._text = new string ( yytext );
      identifiers.push_back ( VSTlval._text );
      return BitStringLit;
    }

\n       { /* end of line */         Vst::incVhdLineNumber(); }
\-\-.*$  { /* comment : skip them */ }
.        { return *yytext; }


%%


int yywrap () { return 1; }


namespace Vst {


  void  ClearIdentifiers ()
  { identifiers.clear (); }


}  // Vst namespace.


namespace {


  char* lower ( char* str )
  {
     for ( char* ptr = str; *ptr != '\0' ; ptr++ )
       *ptr = (char)tolower ( *ptr );
  
     return str;
  }


}  // Anonymous namespace.
