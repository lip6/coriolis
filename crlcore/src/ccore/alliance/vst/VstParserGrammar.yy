%{

// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |      Yacc Grammar for Alliance Structural VHDL                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  Yacc        :  "./VstParserGrammar.yy"                         |
// |                                                                 |
// |  This file is based on the Alliance VHDL parser written by      |
// |       L.A. Tabusse, Vuong H.N., P. Bazargan-Sabet & D. Hommais  |
// +-----------------------------------------------------------------+


#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <deque>
using namespace std;

#include  "hurricane/Warning.h"
#include  "hurricane/Net.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Plug.h"
#include  "hurricane/Instance.h"
#include  "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/Catalog.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/NetExtension.h"
#include "Vst.h"
#include "VstParser.h"
using namespace CRL;


// Symbols from Flex which should be substituted.
#define    yyrestart    VSTrestart
#define    yytext       VSTtext
#define    yywrap       VSTwrap
#define    yyin         VSTin
#define    YYDEBUG      0


extern int   yylex               ();
extern int   yywrap              ();
extern void  yyrestart           ( FILE* );
extern char* yytext;
extern FILE* yyin;

namespace {

  int   yyerror    ( const char* message );

}  // Anonymous namespace.


namespace Vst {


  YaccStateStack     states;
  AllianceFramework* framework;


  void  checkForIeee ( bool ieeeEnabled );
  void  Error        ( int code, const string& name );
  Net*  getNet       ( Cell* cell, const string& name ); 
  void  SetNetType   ( Net* net );

}  // Vst namespace.


%}


%union { unsigned int  _value;
         string*       _text;
         char          _flag;
         string*       _name;
         string*       _expr;
       };

%token          Ampersand
%token          Apostrophe
%token          LeftParen
%token          RightParen
%token          DoubleStar
%token          Star
%token <_flag>  Plus
%token          Comma
%token <_flag>  Minus
%token          VarAsgn
%token          Colon
%token          Semicolon
%token          _LESym
%token          _Box
%token          _LTSym
%token          Arrow
%token          _EQSym
%token          _GESym
%token          _GTSym
%token          Bar
%token          _NESym
%token          Dot
%token          Slash
%token <_text>  Identifier
%token          DecimalInt
%token          DecimalReal
%token <_text>  AbstractLit
%token          BasedInt
%token          BasedReal
%token <_text>  CharacterLit
%token <_text>  StringLit
%token <_text>  BitStringLit
%token          ABS
%token          ACCESS
%token          AFTER
%token          ALIAS
%token          ALL
%token          tok_AND
%token          ARCHITECTURE
%token          ARRAY
%token          ARG  
%token          ASSERT
%token          ATTRIBUTE
%token          _BEGIN
%token          BIT
%token          BIT_VECTOR
%token          BLOCK
%token          BODY
%token          BUFFER
%token          BUS
%token          CASE
%token          Y_CLOCK
%token          COMPONENT
%token          CONFIGURATION
%token          CONSTANT
%token          CONVERT
%token          DISCONNECT
%token <_value> DOWNTO
%token          ELSE
%token          ELSIF
%token          _END
%token          ENTITY
%token          ERROR
%token          _EXIT
%token          _FILE
%token          FOR
%token          FUNCTION
%token          GENERATE
%token          GENERIC
%token          GUARDED
%token          IF
%token          _IN
%token          _INOUT
%token          INTEGER
%token          IS
%token          _LABEL
%token          LIBRARY
%token          _LINKAGE
%token          _LIST
%token          LOOP
%token          MAP
%token          MOD
%token          MUX_BIT
%token          MUX_VECTOR
%token          _NAND
%token          NATURAL
%token          NATURAL_VECTOR
%token          NEW
%token          _NEXT
%token          _NOR
%token          _NOT
%token          tok_NULL
%token          OF
%token          ON
%token          OPEN
%token          _OR
%token          OTHERS
%token          _OUT
%token          _PACKAGE
%token          PORT
%token          POSITIVE
%token          PROCEDURE
%token          PROCESS
%token          RANGE
%token          RECORD
%token          REG_BIT
%token          REG_VECTOR
%token          REGISTER
%token          REM
%token          REPORT
%token          RETURN
%token          SELECT
%token          SEVERITY
%token          SIGNAL
%token          _STABLE
%token          STD_LOGIC
%token          STD_LOGIC_VECTOR
%token          STRING
%token          SUBTYPE
%token          THEN
%token <_value> TO
%token          TRANSPORT
%token          _TYPE
%token          UNITS
%token          UNTIL
%token          USE
%token          VARIABLE
%token          WAIT
%token          WARNING
%token          WHEN
%token          WHILE
%token          WITH
%token          WOR_BIT
%token          WOR_VECTOR
%token          _XOR

%type  <_value> .mode.
%type  <_value> .local_port_mode.
%type  <_value> type_mark
%type  <_value> .BUS.
%type  <_text>  .simple_name.
%type  <_text>  simple_name
%type  <_text>  a_label
%type  <_name>  formal_port_name
%type  <_expr>  actual_port_name
%type  <_expr>  expression
%type  <_expr>  relation
%type  <_expr>  simple_expression
%type  <_expr>  .sign.term..add_op__term..
%type  <_expr>  term
%type  <_expr>  factor
%type  <_expr>  primary
%type  <_expr>  aggregate
%type  <_expr>  type_convertion
%type  <_value> direction
%type  <_value> abstractlit
%type  <_name>  name
%type  <_name>  slice_name
%type  <_name>  indexed_name

%type  <_text>  formal_generic_name
%type  <_text>  generic_name
%type  <_flag>  .sign.

%start design_file


%%
design_file
    : ...libraries_declarations..
      entity_declaration
      architecture_body
    ;

...libraries_declarations..
    : /* Empty */
    | library_or_use_statement
      ...libraries_declarations..
    ;

library_or_use_statement
    : library_statement
    | use_statement
    ;

library_statement
    : LIBRARY 
      Identifier
      Semicolon_ERR
    ;

use_statement
    : USE 
      identifier_path
      Semicolon_ERR
          { bool ieeeVhdl = (Vst::states->_identifiersList.size() == 3); 
            for ( size_t i=0 ; ieeeVhdl and (i<Vst::states->_identifiersList.size()) ; ++i ) {
	      switch ( i ) {
                case 0: if (*(Vst::states->_identifiersList[i]) == "ieee")           continue;
                case 1: if (*(Vst::states->_identifiersList[i]) == "std_logic_1164") continue;
                case 2: if (*(Vst::states->_identifiersList[i]) == "all")            continue;
              }
              ieeeVhdl = false;
              break;
            }
	    Vst::states->_ieeeVhdl |= ieeeVhdl;
            Vst::states->_identifiersList.clear();
          }
    ;

identifier_path
    : path_element
    | identifier_path
      Dot
      path_element
    ;

path_element
    : Identifier { Vst::states->_identifiersList.push_back( $1 ); }
    | ALL        { Vst::states->_identifiersList.push_back( new string("all") ); }
    ;

entity_declaration
    : ENTITY
      .simple_name.
      IS
      .generic_clause.
      .port_clause.
      END_ERR
      .simple_name.
      Semicolon_ERR
    | ENTITY
      error
            { Vst::Error ( 2, "<no parameter>" ); }
    ;

.generic_clause.
    : /*empty*/
    | generic_clause
    ;

generic_clause
    : GENERIC
      LeftParen
      formal_generic_list
      RightParen_ERR
      Semicolon_ERR
    | GENERIC
      error
      Semicolon_ERR
    ;

formal_generic_list
    : formal_generic_element
      ...formal_generic_element..
    ;

...formal_generic_element..
    : /*empty*/
    | ...formal_generic_element..
      Semicolon_ERR
      formal_generic_element
    ;

formal_generic_element
    : CONSTANT
      identifier_list
      Colon
      type_mark
      .constraint.
      generic_VarAsgn__expression
          { if (not Vst::states->_firstPass) {
              Vst::states->_constraint.UnSet();
              Vst::states->_identifiersList.clear();
            }
          }
    | error
    ;

generic_VarAsgn__expression
    : VarAsgn
      generic_expression
    ;

.constraint.
    : /*empty*/
    | constraint
    ;

constraint
    : LeftParen
      range
      RightParen_ERR
    ;

range
    : abstractlit
      direction
      abstractlit { if (not Vst::states->_firstPass) Vst::states->_constraint.Set( $1, $2, $3 ); }
    ;

direction
    : TO       { $$ = Vst::VhdlTo; }
    | DOWNTO   { $$ = Vst::VhdlDownto; }
    ;

.port_clause.
    : /*empty*/
    | port_clause
    ;

port_clause
    : PORT
      LeftParen
      formal_port_list
      RightParen_ERR
      Semicolon_ERR
    | PORT
      error
      Semicolon_ERR
            { Vst::Error ( 3, "<no parameter>" ); }
    ;

formal_port_list
    : formal_port_element
      ...formal_port_element..
    ;

...formal_port_element..
    : /*empty*/
    | ...formal_port_element..
      Semicolon_ERR
      formal_port_element
    ;

formal_port_element
    : .SIGNAL.
      identifier_list
      Colon
      .mode.
      type_mark
      .constraint.
      .BUS.
            { if (not Vst::states->_firstPass) {
                Net::Direction  modeDirection = (Net::Direction::Code)$4;
                Net::Direction  typeDirection = (Net::Direction::Code)$5;
                Net::Direction  direction     = (Net::Direction::Code)(modeDirection | typeDirection);
                for ( unsigned int i=0 ; i < Vst::states->_identifiersList.size() ; i++ ) {
                  if ( Vst::states->_constraint.IsSet() ) {
                    int j;
                    for ( Vst::states->_constraint.Init(j) ; Vst::states->_constraint.End(j) ; Vst::states->_constraint.Next(j) ) {
                      ostringstream name;
                      name << *Vst::states->_identifiersList[i] << "(" << j << ")";
                      Net* net = Net::create ( Vst::states->_cell, name.str() );
                      net->setDirection ( direction );
                      net->setExternal  ( true );
                      Vst::SetNetType ( net );
                      Vst::states->_cellVectorMap[Vst::states->_cell][*Vst::states->_identifiersList[i]].push_back ( net );
                      NetExtension::addPort ( net, name.str() );
                    }
                  } else {
                    Net* net = Net::create ( Vst::states->_cell, *Vst::states->_identifiersList[i] );
                    net->setDirection ( direction );
                    net->setExternal  ( true );
                    Vst::SetNetType ( net );
                    Vst::states->_cellVectorMap[Vst::states->_cell][*Vst::states->_identifiersList[i]].push_back ( net );
                    NetExtension::addPort ( net, *Vst::states->_identifiersList[i] );
                  }
                }

                Vst::states->_constraint.UnSet ();
                Vst::states->_identifiersList.clear ();
              }
            }
    | error
            { /* Reject tokens until the sync token Semicolon is found. */
              do {
                yychar = yylex ();
              } while ( (yychar != Semicolon) && (yychar != 0) );
              yyerrok;

              Vst::Error ( 6, "<no parameter>" );
            }
    ;

architecture_body
    : ARCHITECTURE
      simple_name
          { if (    ( Vst::states->_behavioral )
                 || ( Vst::states->_state->isTerminalNetlist() )
                 || ( Vst::states->_state->getDepth() <= 0 )
               ) YYACCEPT;
          }
      OF
      simple_name
      IS
          { if ( Vst::states->_cell->getName() != *$5 ) Vst::Error ( 1, *$5 ); }
      architecture_declarative_part
      _BEGIN
          { /*if (Vst::states->_firstPass) YYACCEPT;*/ }
      architecture_statement_part
      END_ERR
      .simple_name.
      Semicolon_ERR
            { if ( ( $13 != NULL ) && ( *$13 != *$2 ) )
                Vst::Error ( 7, *$13 );
            }
    | ARCHITECTURE
      error
            { Vst::Error ( 8, "<no parameter>" ); }
    ;

architecture_declarative_part
    : ..block_declaration_item..
    ;

..block_declaration_item..
    : /*empty*/
    | ..block_declaration_item..
      block_declaration_item
    ;

block_declaration_item
    : signal_declaration
    | component_declaration
    | error
      Semicolon_ERR
            { Vst::Error ( 9, "<no parameter>" ); }
    ;

signal_declaration
    : SIGNAL
      identifier_list
      Colon
      type_mark
      .constraint.
      .BUS.
      Semicolon_ERR
          { if (not Vst::states->_firstPass) {
              Net::Direction  direction = (Net::Direction::Code)$4;
              for ( unsigned int i=0 ; i < Vst::states->_identifiersList.size() ; i++ ) {
                if ( Vst::states->_constraint.IsSet() ) {
                  int j;
                  for ( Vst::states->_constraint.Init(j) ; Vst::states->_constraint.End(j) ; Vst::states->_constraint.Next(j) ) {
                    ostringstream name;
                    name << *Vst::states->_identifiersList[i] << "(" << j << ")";
                    Net* net = Net::create ( Vst::states->_cell, name.str() );
                    net->setDirection ( direction );
                    net->setExternal  ( false );
                    Vst::SetNetType ( net );
                    Vst::states->_cellVectorMap[Vst::states->_cell][*Vst::states->_identifiersList[i]].push_back ( net );
                  }
                } else {
                  Net* net = Net::create ( Vst::states->_cell, *Vst::states->_identifiersList[i] );
                  net->setDirection ( direction );
                  net->setExternal  ( false );
                  Vst::SetNetType ( net );
                  Vst::states->_cellVectorMap[Vst::states->_cell][*Vst::states->_identifiersList[i]].push_back ( net );
                }
              }
            }

            Vst::states->_constraint.UnSet ();
            Vst::states->_identifiersList.clear ();
          }
    ;

component_declaration
    : COMPONENT
      Identifier
          { if (Vst::states->_firstPass) {
              if (not Vst::framework->getCell(*$2,Catalog::State::Logical|Catalog::State::InMemory))
                  Vst::states->pushCell( *$2 );
            } else {
              Vst::states->_masterCell = Vst::framework->getCell ( *$2, Catalog::State::Logical );
	      cerr.flush();
            }
          }
      .generic_clause.
      .PORT__local_port_list.
      END_ERR
      COMPONENT
      Semicolon_ERR
    ;

.PORT__local_port_list.
    : /*empty*/
    | PORT
      LeftParen
      local_port_list
      RightParen_ERR
      Semicolon_ERR
    ;

local_port_list
    : local_port_element
      ...local_port_element..
    ;

...local_port_element..
    : /*empty*/
    | ...local_port_element..
      Semicolon_ERR
      local_port_element
    ;

local_port_element
    : .SIGNAL.
      identifier_list
      Colon
      .local_port_mode.
      type_mark
      .constraint.
      .BUS.
          { if (not Vst::states->_firstPass) {
              for ( unsigned int i=0 ; i < Vst::states->_identifiersList.size() ; i++ ) {
                if ( Vst::states->_constraint.IsSet() ) {
                  int j;
                  for ( Vst::states->_constraint.Init(j) ; Vst::states->_constraint.End(j) ; Vst::states->_constraint.Next(j) ) {
                    ostringstream name;
                    name << *Vst::states->_identifiersList[i] << "(" << j << ")";
                    Net* net = Vst::getNet ( Vst::states->_masterCell, name.str() );
                    Vst::states->_cellVectorMap[Vst::states->_masterCell][*Vst::states->_identifiersList[i]].push_back ( net );
                  }
                } else {
                  Net* net = Vst::getNet ( Vst::states->_masterCell, *Vst::states->_identifiersList[i] );
                  Vst::states->_cellVectorMap[Vst::states->_masterCell][*Vst::states->_identifiersList[i]].push_back ( net );
                }
              }
            }

            Vst::states->_constraint.UnSet ();
            Vst::states->_identifiersList.clear ();
          }
    | error
            { /* Reject tokens until the sync token Semicolon is found. */
              do {
                yychar = yylex ();
              } while ( (yychar != Semicolon) && (yychar != 0) );
              yyerrok;

              Vst::Error ( 6, "<no parameter>" );
            }
    ;

architecture_statement_part
    : ..concurrent_statement..
    ;

..concurrent_statement..
    : /*empty*/
    | ..concurrent_statement..
      concurrent_statement
    ;

concurrent_statement
    : component_instantiation_statement
    | error
      Semicolon_ERR
          { Vst::Error (18, "<no parameter>"); }
    ;

component_instantiation_statement
    : a_label
      simple_name
          { if (not Vst::states->_firstPass) {
              Vst::states->_masterCell = Vst::framework->getCell( *$2, Catalog::State::Logical|Catalog::State::InMemory );
	      if (not Vst::states->_masterCell) {
                ostringstream message;
		message << "CParsVst() VHDL Parser - File:<" << Vst::states->_vhdFileName
                        <<  "> Line:" << Vst::states->_vhdLineNumber << "\n"
                        <<  "  Model cell " << *$2 << " of instance "
                        << *$1 << " has not been defined in the component list.";
                throw Error( message.str() );
              }
              Vst::states->_instance = Instance::create( Vst::states->_cell, *$1, Vst::states->_masterCell );
              Vst::states->_cell->setTerminalNetlist( false );
            } else {
              if (not Vst::framework->getCell(*$2,Catalog::State::Logical|Catalog::State::InMemory)) {
                if (Vst::states->pushCell(*$2)) {
                  ostringstream message;
		  message << "CParsVst() VHDL Parser - File:<" << Vst::states->_vhdFileName
                          <<  "> Line:" << Vst::states->_vhdLineNumber << "\n"
                          <<  "  Model cell " << *$2 << " of instance "
                          << *$1 << " has not been defined in the component list.";
                  cerr << Warning( message.str() );
                }
              }
            }
          }
      .generic_map_aspect.
      .port_map_aspect.
      Semicolon_ERR
    ;


// -------------------------------------------------------------------
// Generic Map.

.generic_map_aspect.
    : /*empty*/
    | generic_map_aspect
    ;

generic_map_aspect
    : GENERIC
      MAP
      LeftParen
      generic_association_list
      RightParen_ERR
    ;

generic_association_list
    : generic_association_element
      ...generic_association_element..
    ;

...generic_association_element..
    : /*empty*/
    | ...generic_association_element..
      Comma
      generic_association_element
    ;

generic_association_element
    : formal_generic_name
      Arrow
      actual_generic_name
    | actual_generic_name
    | error
    ;

formal_generic_name
    : simple_name
    ;

actual_generic_name
    : generic_expression
    ;

generic_expression
    : generic_relation
    ;

generic_relation
    : generic_simple_expression
    ;

generic_simple_expression
    : .sign.
      generic_term
      ...generic_term..
    ;

.sign.
    : /*empty*/  { $$ = (char)0; }
    | Plus       { $$ = (char)Vst::VhdlPlus; }
    | Minus      { $$ = (char)Vst::VhdlMinus; }
    ;

generic_term
    : generic_factor
    ;

...generic_term..
    : /*empty*/
       /*  | ...generic_term..
      generic_adding_operator
      generic_term */
    ;

generic_factor
    : generic_primary
    ;

generic_primary
    : generic_name
    | generic_literal
    | generic_aggregate
    ;

generic_aggregate
    : LeftParen
      generic_element_association
      ...generic_element_association..
      RightParen_ERR
    ;

...generic_element_association..
    : /*empty*/
    | ...generic_element_association..
      Comma
      generic_element_association
    ;

generic_element_association
    : .generic_choices.
      generic_expression
    ;

.generic_choices.
    : /*empty*/
    ;

/*
generic_adding_operator
    : Ampersand
    ;
*/

generic_name
    : simple_name
    ;

generic_literal
    : abstractlit
    | StringLit
    | CharacterLit
    ;




// -------------------------------------------------------------------
// Port Map.

.port_map_aspect.
    : /*empty*/
    | port_map_aspect
    ;

port_map_aspect
    : PORT
      MAP
      LeftParen
      association_list
      RightParen_ERR
    ;

association_list
    : association_element
      ...association_element..
    ;

...association_element..
    : /*empty*/
    | ...association_element..
      Comma
      association_element
    ;

association_element
    : formal_port_name
      Arrow
        { if (not Vst::states->_firstPass) Vst::states->_masterPort = false; }
      actual_port_name
        { if (not Vst::states->_firstPass) {
            if ( Vst::states->_masterNets.size() != Vst::states->_instanceNets.size() ) {
              ostringstream message;
              message <<  "CParsVst() VHDL Parser - File:<" << Vst::states->_vhdFileName.c_str()
                      << "> Line:" << Vst::states->_vhdLineNumber << "\n"
                      << "  Port map assignment discrepency "
                      << "instance:" << Vst::states->_instanceNets.size()
                      << " vs. model:"   << Vst::states->_masterNets.size();
              throw Error( message.str() );
            }
	    
            for ( unsigned int i=0 ; i < Vst::states->_masterNets.size() ; i++ )
              if (  not Vst::states->_masterNets[i]->isGlobal()
                 or (Vst::states->_masterNets[i]->getName() != Vst::states->_instanceNets[i]->getName()) )
                Vst::states->_instance->getPlug ( Vst::states->_masterNets[i] )->setNet ( Vst::states->_instanceNets[i] );
	    
            Vst::states->_masterPort = true;
            Vst::states->_masterNets.clear ();
            Vst::states->_instanceNets.clear ();
          }
        }
    | actual_port_name
        { ostringstream message;
          message << "CParsVst() VHDL Parser - File<" << Vst::states->_vhdFileName
                  << ">, Line:" << Vst::states->_vhdLineNumber << "\n"
                  << "  While processing " << Vst::states->_instance->getName()
                  << ": implicit connexions are not allowed.\n";
          throw Error( message.str() );
        }
    | error
            { /* Reject tokens until the sync token Comma is found. */
              do {
                yychar = yylex ();
              } while ( (yychar != Comma) && (yychar != 0) );
              yyerrok;

              Vst::Error ( 31, "<no parameter>" );
            }
    ;

formal_port_name
    : name
    ;

actual_port_name
    : expression
    ;

name
    : simple_name
          { if (not Vst::states->_firstPass) {
              if ( Vst::states->_masterPort ) {
                Vst::PinVector& nets = Vst::states->_cellVectorMap[Vst::states->_masterCell][*$1];
                for ( unsigned int i=0 ; i < nets.size() ; i++ ) {
                  Vst::states->_masterNets.push_back ( nets[i] );
                }
              } else {
                Vst::PinVector& nets = Vst::states->_cellVectorMap[Vst::states->_cell][*$1];
                for ( unsigned int i=0 ; i < nets.size() ; i++ ) {
                  Vst::states->_instanceNets.push_back ( nets[i] );
                }
              }
            }
          }
    | indexed_name
    | slice_name
    ;

indexed_name
    : simple_name
      LeftParen
      abstractlit
      RightParen_ERR
          { if (not Vst::states->_firstPass) {
              ostringstream name;
              name << *$1 << "(" << $3 << ")";
              if ( Vst::states->_masterPort )
                Vst::states->_masterNets.push_back ( Vst::getNet(Vst::states->_masterCell,name.str()) );
              else
                Vst::states->_instanceNets.push_back ( Vst::getNet(Vst::states->_cell,name.str()) );
            }
          }
    ;

slice_name
    : simple_name
      constraint
          { if (not Vst::states->_firstPass) {
              int j;
              for ( Vst::states->_constraint.Init(j) ; Vst::states->_constraint.End(j) ; Vst::states->_constraint.Next(j) ) {
                 ostringstream name;
                 name << *$1 << "(" << j << ")";
                 if ( Vst::states->_masterPort )
                   Vst::states->_masterNets.push_back ( Vst::getNet(Vst::states->_masterCell,name.str()) );
                 else
                   Vst::states->_instanceNets.push_back ( Vst::getNet(Vst::states->_cell,name.str()) );
              }
            }
          }
    ;


expression
    : relation
    ;

relation
    : simple_expression
    ;

simple_expression
    : .sign.term..add_op__term..
    ;

.sign.term..add_op__term..
    : term
    | .sign.term..add_op__term..
      Ampersand
      term
    ;

term
    : factor
    ;


factor
    : primary
    ;

primary
    : aggregate
    | type_convertion
    | name             { $$ = $1; }
    ;

aggregate
    : LeftParen
      expression
      RightParen_ERR  { $$ = $2; }
    ;

type_convertion
    : CONVERT
      LeftParen
      expression
      RightParen_ERR  { $$ = $3; }
    ;


.SIGNAL.
    : /*empty*/
    | SIGNAL
    ;

.local_port_mode.
    : /*empty*/   { $$ = Net::Direction::UNDEFINED; }
    | _IN         { $$ = Net::Direction::IN;        }  
    | _OUT        { $$ = Net::Direction::OUT;       }
    | _INOUT      { $$ = Net::Direction::INOUT;     }
    | _LINKAGE    { $$ = Net::Direction::UNDEFINED; }
    ;

.mode.
    : /*empty*/   { $$ = Net::Direction::UNDEFINED; }
    | _IN         { $$ = Net::Direction::IN;        }  
    | _OUT        { $$ = Net::Direction::OUT;       }
    | _INOUT      { $$ = Net::Direction::INOUT;     }
    | _LINKAGE    { $$ = Net::Direction::UNDEFINED; }
    ;

type_mark
    : BIT              { $$ = Net::Direction::UNDEFINED; }
    | STD_LOGIC        { $$ = Net::Direction::UNDEFINED;    Vst::checkForIeee(true ); }
    | STD_LOGIC_VECTOR { $$ = Net::Direction::UNDEFINED;    Vst::checkForIeee(true ); }
    | WOR_BIT          { $$ = Net::Direction::ConnWiredOr;  Vst::checkForIeee(false); }
    | MUX_BIT          { $$ = Net::Direction::ConnTristate; Vst::checkForIeee(false); }
    | BIT_VECTOR       { $$ = Net::Direction::UNDEFINED;    Vst::checkForIeee(false); }
    | WOR_VECTOR       { $$ = Net::Direction::ConnWiredOr;  Vst::checkForIeee(false); }
    | MUX_VECTOR       { $$ = Net::Direction::ConnTristate; Vst::checkForIeee(false); }
    | INTEGER          { $$ = Net::Direction::UNDEFINED; }
    | NATURAL          { $$ = Net::Direction::UNDEFINED; }
    | NATURAL_VECTOR   { $$ = Net::Direction::UNDEFINED; }
    | POSITIVE         { $$ = Net::Direction::UNDEFINED; }
    | STRING           { $$ = Net::Direction::UNDEFINED; }
    | _LIST            { $$ = Net::Direction::UNDEFINED; }
    | ARG              { $$ = Net::Direction::UNDEFINED; }
    ;

.BUS.
    : /*empty*/  { $$ = 0; }
    | BUS        { $$ = Vst::VhdlBus; }
    ;

identifier_list
    : Identifier { if (not Vst::states->_firstPass) Vst::states->_identifiersList.push_back( $1 ); }
      ...identifier..
    ;

...identifier..
    : /*empty*/
    | ...identifier..
      Comma
      Identifier { if (not Vst::states->_firstPass) Vst::states->_identifiersList.push_back( $3 ); }
    ;

a_label
    : Identifier
      Colon       { $$ = $1; }
    ;

.simple_name.
    : /*empty*/   { $$ = NULL; }
    | simple_name { $$ = $1;   }
    ;

simple_name
    : Identifier { $$ = $1; }
    ;

Semicolon_ERR
    : Semicolon { yyerrok; }
    ;

abstractlit
    : AbstractLit { $$ = atoi ( $1->c_str() ); }
    ;

RightParen_ERR
    : RightParen { yyerrok; }
    ;

END_ERR
    : _END { yyerrok; }
    ;


%%


namespace {


  // ---------------------------------------------------------------
  // Function  :  "yyerror()".
  //
  // The Bison standard parser error (yyerror).

  int  yyerror ( const char* message )
  {
     ostringstream formatted;
     formatted << "CParsVst() - VHDL Parser, File:<" << Vst::states->_vhdFileName
               << ">, Line:" << Vst::states->_vhdLineNumber << "\n  "
               << message << " before keyword or identifier \"" << yytext << "\".\n";
     throw Hurricane::Error( formatted.str() );
     return 0;
  }


}  // Anonymous namespace.


namespace Vst {


  void  incVhdLineNumber ()
  { ++(states->_vhdLineNumber); }


  // ---------------------------------------------------------------
  // Function  :  "Vst::Error()".
  //
  // Manage errors from wich we can recover and continue a little
  // while.

  void  Error ( int code, const string& name )
  {
    states->_errorCount++;

    if ( states->_errorCount >= states->_maxErrors )
        throw Hurricane::Error ( "CParsVst() VHDL Parser, Too many errors occured.\n" );

    if ( code < 100 )
      cerr << "[ERROR] CParsVst() VHDL Parser, File:<" << states->_vhdFileName
           << ">, Line:" << states->_vhdLineNumber << " Code:" << code  << " :\n  ";
    else {
      if (code < 200)
        cerr << "[ERROR] CParsVst() VHDL Parser, Code:" << code << " :\n  ";
    }

    switch ( code ) {
      case   1: cerr << "\"" << name << "\" is incompatible with the entity name." << endl; break;
      case   2: cerr << "Bad entity declaration." << endl; break;
      case   3: cerr << "Bad port clause declaration" << endl; break;
      case   4: cerr << "Port \"" << name << "\" already declared." << endl; break;
      case   5: cerr << "Illegal port declaration \"" << name << "\" (mode,type,guard mark)" << endl; break;
      case   6: cerr << "Bad port declaration." << endl; break;
      case   7: cerr << "\"" << name << "\" is incompatible with the architecture name." << endl; break;
      case   8: cerr << "Bad architecture declaration." << endl; break;
      case   9: cerr << "Illegal declaration" << endl; break;
      case  10: cerr << "Signal \"" << name << "\" already declared." << endl; break;
      case  11: cerr << "illegal signal declaration \"" << name << "\" (type,guard mark)." << endl; break;
      case  12: cerr << "Component \"" << name << "\" already declared." << endl; break;
      case  13: cerr << "Instance \"" << name << "\" already declared." << endl; break;
      case  14: cerr << "Unknown component \"" << name << "\"." << endl; break;
      case  15: cerr << "Illegal usage of implicit port map description" << endl; break;
      case  16: cerr << "Unknown local port \"" << name << "\"." << endl; break;
      case  17: cerr << "Unknown port or signal \"" << name << "\"." << endl; break;
      case  18: cerr << "Illegal concurrent statement." << endl; break;
      case  31: cerr << "Bad signal association." << endl; break;
      case  32: cerr << "Null array not supported." << endl; break;
      case  33: cerr << "Illegal constraint in declaration of type." << endl; break;
      case  36: cerr << "Signal \"" << name << "\" used out of declared range." << endl; break;
      case  38: cerr << "Width or/and type mismatch." << endl; break;
      case  41: cerr << "Port \"" << name << "\" connected to more than one signal." << endl; break;
      case  76: cerr << "Instance \"" << name << "\"mismatch with model." << endl; break;
      default:  cerr << "Syntax error." << endl; break;
      case 200:
        throw Hurricane::Error ( "Error(s) occured.\n" );
    }
  }


  // ---------------------------------------------------------------
  // Function  :  "checkForIeee()".

  void  checkForIeee ( bool ieeeEnabled )
  {
    if (not states->_ieeeWarned) {
      if (ieeeEnabled xor states->_ieeeVhdl) {
        states->_ieeeWarned = true;

        ostringstream formatted;
        formatted << "CParsVst() - VHDL Parser, File:<" << Vst::states->_vhdFileName
                  << ">, Line:" << Vst::states->_vhdLineNumber << "\n  "
                  << "Mixed IEEE VHDL & Alliance VHDL dialects, you should choose one.";
        cerr << formatted.str() << endl;
      }
    }
  }


  // ---------------------------------------------------------------
  // Function  :  "getNet()".

  Net* getNet ( Cell* cell, const string& name )
  {
     Net* net = cell->getNet ( Name(name) );
     if ( !net ) {
       ostringstream message;
       message << "CParsVst() VHDL Parser, File:<" << states->_vhdFileName
               << "> Line:" << states->_vhdLineNumber
               << "\n"
               << "  No net \"" << name
               << "\" in cell \"" << cell->getName() << "\".\n";
       throw Hurricane::Error( message.str() );
     }

     return net;
  }


  // ---------------------------------------------------------------
  // Function  :  "SetNetType()".

  void  SetNetType ( Net* net )
  {
    if ( framework->isPOWER(net->getName()) ) {
      net->setType   ( Net::Type::POWER  );
      net->setGlobal ( true );
    } else if ( framework->isGROUND(net->getName()) ) {
      net->setType   ( Net::Type::GROUND );
      net->setGlobal ( true );
    } else if ( framework->isCLOCK(net->getName()) ) {
      net->setType   ( Net::Type::CLOCK );
    } else
      net->setType ( Net::Type::LOGICAL );
  }


}  // Vst namespace.




namespace CRL {


// -------------------------------------------------------------------
// Function  :  "vstParser()".
//
// The parser work in 2.5 (two and a half) passes:
// 1.0 Read the vst in search of COMPONENTS and/or instances declarations.
//     Build an ordered list of all the model (i.e. master Cell) needed.
// 1.5 Load, in order all the Cell required. This is a recursive process,
//     that is, the model, in turn can trigger the loading of yet other
//     master Cells.
// 2.0 Read the vst a second time, actually builing the Cell.

void  vstParser ( const string cellPath, Cell *cell )
{
  static bool firstCall = true;
  if ( firstCall ) {
    firstCall      = false;
  }
  Vst::framework = AllianceFramework::get ();

  Vst::states.push_back ( new Vst::YaccState(cellPath) );

  if ( ( cellPath.size() > 4 ) && ( !cellPath.compare(cellPath.size()-4,4,".vbe") ) )
    Vst::states->_behavioral = true;

  CatalogProperty *sprop =
    (CatalogProperty*)cell->getProperty ( CatalogProperty::getPropertyName() );
  if ( sprop == NULL )
    throw Error ( "Missing CatalogProperty in cell %s.\n" , getString(cell->getName()).c_str() );

  Vst::states->_state = sprop->getState ();
  Vst::states->_state->setLogical ( true );
  Vst::states->_cell = cell;

  IoFile ccell ( cellPath );
  ccell.open ( "r" );

  if (Vst::states->_behavioral) {
    cmess2 << "     " << tab << "+ " << cellPath << " [behavioral]" << endl;
  } else {
    cmess2 << "     " << tab << "+ " << cellPath << " [models]" << endl; tab++;

  // 1.0 step: Build the ordered list of model (Cell) required by the instances.
    yyin = ccell.getFile ();
    if ( !firstCall ) yyrestart ( VSTin );
  //yydebug = 0;
    yyparse ();
  
  // 1.5 step: Load, in order, the model Cells (recursive).
    while ( !Vst::states->_cellQueue.empty() ) {
      if ( !Vst::framework->getCell ( getString(Vst::states->_cellQueue.front())
                                    , Catalog::State::Logical
                                    , Vst::states->_state->getDepth()-1) ) {
        throw Error ( "CParsVst() VHDL Parser:\n"
                      "  Unable to find cell \"%s\", please check your <./coriolis2/settings.py>.\n"
                    , getString(Vst::states->_cellQueue.front()).c_str()
                    );
      }
      Vst::states->_cellQueue.pop_front();
    }
    --tab;
    cmess2 << "     " << tab << "+ " << cellPath << " [structural]" << endl;
  }

  Vst::states->_firstPass     = false;
  Vst::states->_vhdLineNumber = 1;
  ccell.close ();

// 2.0 step: Now really read and build the Cell.
  ccell.open  ( "r" );
  yyin = ccell.getFile ();
  yyrestart ( VSTin );
  UpdateSession::open ();
  yyparse ();
  UpdateSession::close ();
  Vst::states.pop_back();

  ccell.close ();
}


}
