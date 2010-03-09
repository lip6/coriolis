%{


// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// This file is based on the Alliance VHDL parser written by
//    L.A. Tabusse, Vuong H.N., P. Bazargan-Sabet & D. Hommais.
//
//  Yacc Rules for Alliance Structural VHDL.
//
// ===================================================================
//
// $Id$


#include <stdio.h>
#include <string.h>

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <queue>
using namespace std;

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
using namespace CRL;


// Symbols from Flex which should be substituted.
# define    yyrestart    VSTrestart
# define    yytext       VSTtext
# define    yywrap       VSTwrap
# define    yyin         VSTin


extern void  ClearVstIdentifiers ();
extern int   yylex               ();
extern int   yywrap              ();
extern void  yyrestart           ( FILE* );
extern char* yytext;
extern FILE* yyin;

int  vhdLineNumber = 1;


namespace {


  class  Constraint {
    private:
      int   _from;
      int   _to;
      bool  _set;
    public:
      Constraint () : _from(0), _to(0), _set(false) { };
    public:
      int   getFrom  () const { return _from; }
      int   getTo    () const { return _to; }
      bool  IsSet    () const { return _set; }
      void  Set      ( int from, int direction, int to );
      void  UnSet    () { _set = false; }
      void  Init ( int& index ) { index = _from; };
      void  Next ( int& index );
      bool  End  ( int  index );
  };


  void  Constraint::Set ( int from, int direction, int to )
  {
    _set  = true;
    _from = from;
    _to   = to;
  }


  void  Constraint::Next ( int& index )
  {
    if ( _from < _to ) index++;
    else               index--;
  }


  bool  Constraint::End ( int index )
  {
    if ( _from < _to ) return index <= _to;

    return index >= _to;
  }


  typedef  vector<Net*>          PinVector;
  typedef  map<Name,PinVector>   VectorMap;
  typedef  map<Cell*,VectorMap>  CellVectorMap;


  AllianceFramework* __framework;


  class YaccState {
    public:
      int              _errorCount;
      int              _maxErrors;
      queue<Name>      _cellQueue;
      Catalog::State*  _state;
      Cell*            _cell;
      Cell*            _masterCell;
      Instance*        _instance;
      Constraint       _constraint;
      vector<string*>  _identifiersList;
      CellVectorMap    _cellVectorMap;
      PinVector        _instanceNets;
      PinVector        _masterNets;
      bool             _masterPort;
      bool             _firstPass;
      bool             _behavioral;
    public:
      YaccState ()
        : _errorCount(0)
        , _maxErrors(10)
        , _cellQueue()
        , _state(NULL)
        , _cell(NULL)
        , _masterCell(NULL)
        , _instance(NULL)
        , _constraint()
        , _identifiersList()
        , _cellVectorMap()
        , _instanceNets()
        , _masterNets()
        , _masterPort(true)
        , _firstPass(true) 
        , _behavioral(false) 
        { }
  };


  class YaccStateStack : public vector<YaccState*> {
    public:
      YaccState* operator->() { return back(); };
      void       pop_back  () { delete back (); vector<YaccState*>::pop_back (); }
  };


  YaccStateStack  __ys;


  


  int   yyerror    ( char* message );
  void  VstError   ( int code, const string& name );
  Net*  getNet     ( Cell* cell, const string& name ); 
  void  SetNetType ( Net* net );

}


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
%token          Plus
%token          Comma
%token          Minus
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
%token          DOWNTO
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
%token          STRING
%token          SUBTYPE
%token          THEN
%token          TO
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
    : entity_declaration
      architecture_body
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
            { VstError ( 2, "<no parameter>" ); }
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
          { __ys->_constraint.UnSet ();
            __ys->_identifiersList.clear ();
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
      abstractlit { __ys->_constraint.Set ( $1, $2, $3 ); }
    ;

direction
    : TO
    | DOWNTO
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
            { VstError ( 3, "<no parameter>" ); }
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
            { if ( !__ys->_firstPass ) {
                Net::Direction  modeDirection = (Net::Direction::Code)$4;
                Net::Direction  typeDirection = (Net::Direction::Code)$5;
                Net::Direction  direction;
                if ( typeDirection != Net::Direction::UNDEFINED )
                  direction = typeDirection;
                else
                  direction = modeDirection;
                for ( unsigned int i=0 ; i < __ys->_identifiersList.size() ; i++ ) {
                  if ( __ys->_constraint.IsSet() ) {
                    int j;
                    for ( __ys->_constraint.Init(j) ; __ys->_constraint.End(j) ; __ys->_constraint.Next(j) ) {
                      ostringstream name;
                      name << *__ys->_identifiersList[i] << "(" << j << ")";
                      Net* net = Net::create ( __ys->_cell, name.str() );
                      net->setDirection ( direction );
                      net->setExternal  ( true );
                      SetNetType ( net );
                      __ys->_cellVectorMap[__ys->_cell][*__ys->_identifiersList[i]].push_back ( net );
                      NetExtension::addPort ( net, name.str() );
                    }
                  } else {
                    Net* net = Net::create ( __ys->_cell, *__ys->_identifiersList[i] );
                    net->setDirection ( direction );
                    net->setExternal  ( true );
                    SetNetType ( net );
                    __ys->_cellVectorMap[__ys->_cell][*__ys->_identifiersList[i]].push_back ( net );
                    NetExtension::addPort ( net, *__ys->_identifiersList[i] );
                  }
                }
              }

              __ys->_constraint.UnSet ();
              __ys->_identifiersList.clear ();
            }
    | error
            { /* Reject tokens until the sync token Semicolon is found. */
              do {
                yychar = yylex ();
              } while ( (yychar != Semicolon) && (yychar != 0) );
              yyerrok;

              VstError ( 6, "<no parameter>" );
            }
    ;

architecture_body
    : ARCHITECTURE
      simple_name
          { if (    ( __ys->_behavioral )
                 || ( __ys->_state->isFlattenLeaf() )
                 || ( __ys->_state->getDepth() <= 0 )
               ) YYACCEPT;
          }
      OF
      simple_name
      IS
          { if ( __ys->_cell->getName() != *$5 ) VstError ( 1, *$5 ); }
      architecture_declarative_part
      _BEGIN
          { if ( __ys->_firstPass ) YYACCEPT; }
      architecture_statement_part
      END_ERR
      .simple_name.
      Semicolon_ERR
            { if ( ( $13 != NULL ) && ( *$13 != *$2 ) )
                VstError ( 7, *$13 );
            }
    | ARCHITECTURE
      error
            { VstError ( 8, "<no parameter>" ); }
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
            { VstError ( 9, "<no parameter>" ); }
    ;

signal_declaration
    : SIGNAL
      identifier_list
      Colon
      type_mark
      .constraint.
      .BUS.
      Semicolon_ERR
          { if ( !__ys->_firstPass ) {
              Net::Direction  direction = (Net::Direction::Code)$4;
              for ( unsigned int i=0 ; i < __ys->_identifiersList.size() ; i++ ) {
                if ( __ys->_constraint.IsSet() ) {
                  int j;
                  for ( __ys->_constraint.Init(j) ; __ys->_constraint.End(j) ; __ys->_constraint.Next(j) ) {
                    ostringstream name;
                    name << *__ys->_identifiersList[i] << "(" << j << ")";
                    Net* net = Net::create ( __ys->_cell, name.str() );
                    net->setDirection ( direction );
                    net->setExternal  ( false );
                    SetNetType ( net );
                    __ys->_cellVectorMap[__ys->_cell][*__ys->_identifiersList[i]].push_back ( net );
                  }
                } else {
                  Net* net = Net::create ( __ys->_cell, *__ys->_identifiersList[i] );
                  net->setDirection ( direction );
                  net->setExternal  ( false );
                  SetNetType ( net );
                  __ys->_cellVectorMap[__ys->_cell][*__ys->_identifiersList[i]].push_back ( net );
                }
              }
            }

            __ys->_constraint.UnSet ();
            __ys->_identifiersList.clear ();
          }
    ;

component_declaration
    : COMPONENT
      Identifier
          { if ( __ys->_firstPass ) {
              if ( !__framework->getCell(*$2,Catalog::State::Views|Catalog::State::InMemory) )
                  __ys->_cellQueue.push ( *$2 );
            } else {
              __ys->_masterCell = __framework->getCell ( *$2, Catalog::State::Views );
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
          { if ( !__ys->_firstPass ) {
              for ( unsigned int i=0 ; i < __ys->_identifiersList.size() ; i++ ) {
                if ( __ys->_constraint.IsSet() ) {
                  int j;
                  for ( __ys->_constraint.Init(j) ; __ys->_constraint.End(j) ; __ys->_constraint.Next(j) ) {
                    ostringstream name;
                    name << *__ys->_identifiersList[i] << "(" << j << ")";
                    Net* net = getNet ( __ys->_masterCell, name.str() );
                    __ys->_cellVectorMap[__ys->_masterCell][*__ys->_identifiersList[i]].push_back ( net );
                  }
                } else {
                  Net* net = getNet ( __ys->_masterCell, *__ys->_identifiersList[i] );
                  __ys->_cellVectorMap[__ys->_masterCell][*__ys->_identifiersList[i]].push_back ( net );
                }
              }
            }

            __ys->_constraint.UnSet ();
            __ys->_identifiersList.clear ();
          }
    | error
            { /* Reject tokens until the sync token Semicolon is found. */
              do {
                yychar = yylex ();
              } while ( (yychar != Semicolon) && (yychar != 0) );
              yyerrok;

              VstError ( 6, "<no parameter>" );
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
          { VstError (18, "<no parameter>"); }
    ;

component_instantiation_statement
    : a_label
      simple_name
          { __ys->_masterCell = __framework->getCell ( *$2, Catalog::State::Views|Catalog::State::InMemory );
	    if ( !__ys->_masterCell )
              throw Error ( "CParsVst(), Line %d:\n"
                            "  Model cell %s of instance %s has not been defined "
                            "in the compenent list."
                          , vhdLineNumber, *$2->c_str(), *$1->c_str()
                          );
            __ys->_instance = Instance::create ( __ys->_cell, *$1, __ys->_masterCell );
            __ys->_cell->setTerminal ( false );
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
    : /*empty*/
    | Plus
    | Minus
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
        { __ys->_masterPort = false; }
      actual_port_name
        { if ( __ys->_masterNets.size() != __ys->_instanceNets.size() )
            throw Error ( "CParsVst(), line %d:\n"
                          "  Port map assignment discrepency."
                        , vhdLineNumber
                        );

          for ( unsigned int i=0 ; i < __ys->_masterNets.size() ; i++ )
            if ( !__ys->_masterNets[i]->isGlobal() )
              __ys->_instance->getPlug ( __ys->_masterNets[i] )->setNet ( __ys->_instanceNets[i] );

          __ys->_masterPort = true;
          __ys->_masterNets.clear ();
          __ys->_instanceNets.clear ();
        }
    | actual_port_name
        { throw Error ( "CParsVst(), line %d:\n"
                        "  While processing %s: implicit connexions are not allowed.\n"
                      , vhdLineNumber, getString(__ys->_instance->getName()).c_str()
                      );
        }
    | error
            { /* Reject tokens until the sync token Comma is found. */
              do {
                yychar = yylex ();
              } while ( (yychar != Comma) && (yychar != 0) );
              yyerrok;

              VstError ( 31, "<no parameter>" );
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
          { if ( __ys->_masterPort ) {
              PinVector& nets = __ys->_cellVectorMap[__ys->_masterCell][*$1];
              for ( unsigned int i=0 ; i < nets.size() ; i++ ) {
                __ys->_masterNets.push_back ( nets[i] );
              }
            } else {
              PinVector& nets = __ys->_cellVectorMap[__ys->_cell][*$1];
              for ( unsigned int i=0 ; i < nets.size() ; i++ ) {
                __ys->_instanceNets.push_back ( nets[i] );
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
          { ostringstream name;
            name << *$1 << "(" << $3 << ")";
            if ( __ys->_masterPort )
              __ys->_masterNets.push_back ( getNet(__ys->_masterCell,name.str()) );
            else
              __ys->_instanceNets.push_back ( getNet(__ys->_cell,name.str()) );
          }
    ;

slice_name
    : simple_name
      constraint
          { int j;
            for ( __ys->_constraint.Init(j) ; __ys->_constraint.End(j) ; __ys->_constraint.Next(j) ) {
               ostringstream name;
               name << *$1 << "(" << j << ")";
               if ( __ys->_masterPort )
                 __ys->_masterNets.push_back ( getNet(__ys->_masterCell,name.str()) );
               else
                 __ys->_instanceNets.push_back ( getNet(__ys->_cell,name.str()) );
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
    | name
    ;

aggregate
    : LeftParen
      expression
      RightParen_ERR
    ;

type_convertion
    : CONVERT
      LeftParen
      expression
      RightParen_ERR
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
    : BIT             { $$ = Net::Direction::UNDEFINED; }
    | WOR_BIT         { $$ = Net::Direction::UNDEFINED; }
    | MUX_BIT         { $$ = Net::Direction::TRISTATE;  }
    | BIT_VECTOR      { $$ = Net::Direction::UNDEFINED; }
    | WOR_VECTOR      { $$ = Net::Direction::UNDEFINED; }
    | MUX_VECTOR      { $$ = Net::Direction::TRISTATE;  }
    | INTEGER         { $$ = Net::Direction::UNDEFINED; }
    | NATURAL         { $$ = Net::Direction::UNDEFINED; }
    | NATURAL_VECTOR  { $$ = Net::Direction::UNDEFINED; }
    | POSITIVE        { $$ = Net::Direction::UNDEFINED; }
    | STRING          { $$ = Net::Direction::UNDEFINED; }
    | _LIST           { $$ = Net::Direction::UNDEFINED; }
    | ARG             { $$ = Net::Direction::UNDEFINED; }
    ;

.BUS.
    : /*empty*/
    | BUS
    ;

identifier_list
    : Identifier { __ys->_identifiersList.push_back ( $1 ); }
      ...identifier..
    ;

...identifier..
    : /*empty*/
    | ...identifier..
      Comma
      Identifier { __ys->_identifiersList.push_back ( $3 ); }
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

  int  yyerror ( char* message )
  {
     throw Error ( "CParsVst():\n  %s before %s at line %d.\n"
                 , message, yytext, vhdLineNumber );
     return 0;
  }




  // ---------------------------------------------------------------
  // Function  :  "VstError()".
  //
  // Manage errors from wich we can recover and continue a little
  // while.

  void  VstError ( int code, const string& name )
  {
    __ys->_errorCount++;

    if ( __ys->_errorCount >= __ys->_maxErrors )
        throw Error ( "CParsVst(): Too many errors occured.\n" );

    if ( code < 100 )
      cerr << "[ERROR] CParsVst(): Code " << code << " line " << vhdLineNumber << " :\n  ";
    else {
      if (code < 200)
        cerr << "[ERROR] CParsVst(): Code " << code << " :\n  ";
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
        throw Error ( "Error(s) occured.\n" );
    }
  }




  // ---------------------------------------------------------------
  // Function  :  "getNet()".

  Net* getNet ( Cell* cell, const string& name )
  {
     Net* net = cell->getNet ( Name(name) );
     if ( !net ) {
       throw Error ( "CParsVst(), line %d:\n"
                     "  No net \"%s\" in cell \"%s\".\n"
                   , vhdLineNumber
                   , name.c_str()
                   , getString(cell->getName()).c_str()
                   );
     }

     return net;
  }




  // ---------------------------------------------------------------
  // Function  :  "SetNetType()".

  void  SetNetType ( Net* net )
  {
    if ( __framework->isPOWER(net->getName()) ) {
      net->setType   ( Net::Type::POWER  );
      net->setGlobal ( true );
    } else if ( __framework->isGROUND(net->getName()) ) {
      net->setType   ( Net::Type::GROUND );
      net->setGlobal ( true );
    } else if ( __framework->isCLOCK(net->getName()) ) {
      net->setType   ( Net::Type::CLOCK );
    } else
      net->setType ( Net::Type::LOGICAL );
  }


}




namespace CRL {


// -------------------------------------------------------------------
// Function  :  "vstParser()".

void  vstParser ( const string& cellPath, Cell *cell )
{
  cmess2 << "     " << tab << "+ " << cellPath << endl; tab++;

  static bool firstCall = true;
  if ( firstCall ) {
    firstCall   = false;
    __framework = AllianceFramework::get ();
  }

  __ys.push_back ( new YaccState() );

  if ( ( cellPath.size() > 4 ) && ( !cellPath.compare(cellPath.size()-4,4,".vbe") ) )
    __ys->_behavioral = true;

  CatalogProperty *sprop =
    (CatalogProperty*)cell->getProperty ( CatalogProperty::getPropertyName() );
  if ( sprop == NULL )
    throw Error ( "Missing CatalogProperty in cell %s.\n" , getString(cell->getName()).c_str() );

  __ys->_state = sprop->getState ();
  __ys->_state->setLogical ( true );
  __ys->_cell = cell;

  IoFile ccell ( cellPath );
  ccell.open ( "r" );
  yyin = ccell.getFile ();
  if ( !firstCall ) yyrestart ( VSTin );
  yyparse ();

  bool hasInstances = false;
  while ( !__ys->_cellQueue.empty() ) {
    hasInstances = true;
    if ( !__framework->getCell ( getString(__ys->_cellQueue.front())
                        , Catalog::State::Views
                        , __ys->_state->getDepth()-1) ) {
      throw Error ( "CParsVst():\n"
                    "  Unable to find cell \"%s\", please check your CRL_CATA_LIB.\n"
                  , getString(__ys->_cellQueue.front()).c_str()
                  );
    }
    __ys->_cellQueue.pop ();
  }

  if ( hasInstances ) cmess2 << "     " << --tab << "+ " << cellPath << endl;
  else                --tab;

  __ys->_firstPass = false;
  ccell.close ();
  ccell.open  ( "r" );
  yyin = ccell.getFile ();
  yyrestart ( VSTin );
  UpdateSession::open ();
  yyparse ();
  UpdateSession::close ();
  ClearVstIdentifiers ();

  __ys.pop_back();

  ccell.close ();
}


}
