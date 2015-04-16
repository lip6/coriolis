
%{

#include <cstdio>
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
#include "crlcore/AcmSigda.h"
using namespace CRL;


// Symbols from Flex which should be substituted.
#define    yyrestart    AcmSigda_restart
#define    yytext       AcmSigda_text
#define    yywrap       AcmSigda_wrap
#define    yyin         AcmSigda_in


extern char*  yytext;
extern FILE*  yyin;
extern int    yylex     ();
extern int    yywrap    ();
extern void   yyrestart ( FILE* );

int yylineno = 0;

namespace {

  int  AcmSigda_error ( const char* message );


  class ParserState {
    public:
      Cell*           _cell;
      Catalog::State* _state;
      vector<string>  _inputs;
      string          _output;
      int             _gateType;
    public:
      inline       ParserState ();
      inline void  reset       ();
  };


  inline ParserState::ParserState () : _cell    (NULL)
                                     , _state   (NULL)
                                     , _inputs  ()
                                     , _output  ()
                                     , _gateType(0)
                                     { }

  inline void  ParserState::reset ()
  {
    _cell     = NULL;
    _state    = NULL;
    _inputs.clear ();
    _output   = "";
    _gateType = 0;
  }


  ParserState        __state;
  AllianceFramework* __framework;


  Net* getNet ( Cell*           cell
              , const string&   name
              , Net::Direction  direction=Net::Direction::UNDEFINED
              , bool            create   =false )
  {
     Net* net = cell->getNet ( Name(name) );
     if ( net == NULL ) {
       if ( not create )
         throw Error ( "AcmSigdaParserGrammar(), line %d:\n"
                       "  No net \"%s\" in cell \"%s\".\n"
                     , yylineno
                     , name.c_str()
                     , getString(cell->getName()).c_str()
                     );
       net = Net::create ( __state._cell, name.c_str() );
       net->setDirection ( direction );
     }

     return net;
  }


  void  addGlobalNets ( Cell* cell )
  {
    Net* vss = Net::create ( cell, "vss" );
    vss->setExternal ( true );
    vss->setGlobal   ( true );
    vss->setType     ( Net::Type::GROUND );

    Net* vdd = Net::create ( cell, "vdd" );
    vdd->setExternal ( true );
    vdd->setGlobal   ( true );
    vdd->setType     ( Net::Type::POWER );

    Net* ck = getNet ( __state._cell, "ck", Net::Direction::IN, true );
    ck->setExternal  ( true );
  }


  Instance* createInstance ();


} // End of local namespace.


%}

%union  { int         _value;
          const char *_text;
        };

%token            Equal
%token            LeftParent
%token            RightParent
%token            Comma
%token  <_value>  AND
%token  <_value>  NAND
%token  <_value>  OR
%token  <_value>  NOR
%token  <_value>  INPUT
%token  <_value>  OUTPUT
%token  <_value>  NOT
%token  <_value>  FROM
%token  <_value>  XOR
%token  <_value>  XNOR
%token  <_value>  BUFF
%token  <_value>  DFF
%token  <_text>   NAME

%type   <_value>  type
%type   <_value>  primary_io

%start  file

%%

file       :    
           | line
           | file  line
           ;

line       : primary_io LeftParent NAME RightParent
               {
                 //if ($1 == INPUT) cout << "INPUT (" << $3 << ")" << endl;
                 //else             cout << "OUTPUT(" << $3 << ")" << endl;

                 Net::Direction  direction = ($1 == INPUT) ? Net::Direction::IN : Net::Direction::OUT ;

                 Net* net = getNet ( __state._cell, $3, direction, true );
                 net->setExternal  ( true );
               }
           | output Equal type LeftParent inputs RightParent
               {
                 __state._gateType = $3;

                 createInstance ();

                 __state._inputs.clear();
		 __state._output.clear();
               }
           ;

primary_io : INPUT  { $$ = $1; }
           | OUTPUT { $$ = $1; }
           ;

inputs     : NAME              { __state._inputs.push_back ( $1 ); }
           | inputs Comma NAME { __state._inputs.push_back ( $3 ); }
           ;

output     : NAME { __state._output = $1; }
           ;

type       : AND  { $$ = $1; }
           | NAND { $$ = $1; }
           | OR   { $$ = $1; }
           | NOR  { $$ = $1; }
           | XOR  { $$ = $1; }
           | XNOR { $$ = $1; }
           | BUFF { $$ = $1; }
           | NOT  { $$ = $1; }
           | DFF  { $$ = $1; }
           ;

%%

namespace {


  Instance* createInstance ()
  {
    //cout << __state._output << " = " << __state._gateType << "(";
    //for ( size_t i=0 ; i<__state._inputs.size() ; ++i ) {
    //  if ( i>0 ) cout << ",";
    //  cout << __state._inputs[i];
    //}
    //cout << ")" << endl;

    const char* masterName = NULL;

    const char* AndTable  [3] = {   "a2_x2",  "a3_x2",  "a4_x4" };
    const char* NandTable [3] = {  "na2_x1", "na3_x1", "na4_x1" };
    const char* OrTable   [3] = {   "o2_x2",  "o3_x2",  "o4_x4" };
    const char* NorTable  [3] = {  "no2_x1", "no3_x1", "no4_x1" };

    size_t inputsNb = __state._inputs.size();

    switch ( __state._gateType ) {
      case AND:  if ( (inputsNb > 1) and (inputsNb < 5) ) masterName = AndTable [inputsNb-2]; break;
      case NAND: if ( (inputsNb > 1) and (inputsNb < 5) ) masterName = NandTable[inputsNb-2]; break;
      case OR:   if ( (inputsNb > 1) and (inputsNb < 5) ) masterName = OrTable  [inputsNb-2]; break;
      case NOR:  if ( (inputsNb > 1) and (inputsNb < 5) ) masterName = NorTable [inputsNb-2]; break;
      case XOR:  masterName = "xr2_x1";  break;
      case XNOR: masterName = "nxr2_x1"; break;
      case BUFF: masterName = "buf_x2";  break;
      case NOT:  masterName = "inv_x1";  break;
      case DFF:  masterName = "sff1_x4"; break;
    }

    if ( masterName == NULL ) {
      cerr << "[ERROR] Unmatched virtual cell " << __state._output << "." << endl;
      return NULL;
    }

    Cell*     masterCell = __framework->getCell(masterName,Catalog::State::Views);
    Instance* instance = Instance::create ( __state._cell
                                          , __state._output.c_str()
                                          , masterCell );

    // Inputs assignment.
    switch ( __state._gateType ) {
      case AND: 
      case NAND:
      case OR:  
      case NOR: 
      case XOR: 
      case XNOR:
        {
          char terminal[3] = "iX";
          for ( size_t i=0 ; i<inputsNb ; ++i ) {
            terminal[1] = '0' + (char)i;
            instance->getPlug ( masterCell->getNet(terminal) )->setNet ( getNet(__state._cell
                                                                       , __state._inputs[i].c_str()
                                                                       , Net::Direction::UNDEFINED
                                                                       , true) );
          }
        }
        break;
      case DFF: 
        instance->getPlug ( masterCell->getNet("ck") )->setNet ( getNet(__state._cell
                                                               , "ck"
                                                               , Net::Direction::UNDEFINED
                                                               , true) );
      case BUFF: 
      case NOT: 
        instance->getPlug ( masterCell->getNet("i") )->setNet ( getNet(__state._cell
                                                              , __state._inputs[0].c_str()
                                                              , Net::Direction::UNDEFINED
                                                              , true) );
        break;
    }

    // Output assignment.
    switch ( __state._gateType ) {
      case AND: 
      case OR: 
      case XOR: 
      case BUFF: 
      case DFF: 
        instance->getPlug ( masterCell->getNet("q") )->setNet ( getNet(__state._cell
                                                              , __state._output.c_str()
                                                              , Net::Direction::UNDEFINED
                                                              , true) );
        break;
      case NAND: 
      case NOR: 
      case XNOR: 
      case NOT: 
        instance->getPlug ( masterCell->getNet("nq") )->setNet ( getNet(__state._cell
                                                               , __state._output.c_str()
                                                               , Net::Direction::UNDEFINED
                                                               , true) );
        break;
    }
    
    return instance;
  }


  int  AcmSigda_error ( const char* message )
  {
    throw Error ( "AcmSigdaParser(): Syntax error at line %d.\n", yylineno );
    return 0;
  }


} // End of anonymous namespace.


namespace CRL {


  Cell* AcmSigda::load ( string benchmark )
  {
    UpdateSession::open ();

    cmess2 << "     " << tab << "+ " << benchmark << endl; tab++;

    static bool firstCall = true;
    if ( firstCall ) {
      firstCall   = false;
      __framework = AllianceFramework::get ();

      // Preload SxLib using <vst> format.
      __framework->loadLibraryCells ( "sxlib" );
    }

    IoFile ccell ( benchmark+".bench" );
    ccell.open ( "r" );
    if (not ccell.isOpen())
      throw Error( "AcmSigda::load(): Unable to open file %s.bench.", benchmark.c_str() );

    Cell* cell = __framework->createCell ( benchmark );

    CatalogProperty *sprop =
      (CatalogProperty*)cell->getProperty ( CatalogProperty::getPropertyName() );
    if ( sprop == NULL )
      throw Error ( "Missing CatalogProperty in cell %s.\n" , getString(cell->getName()).c_str() );

    __state._cell  = cell;
    __state._cell->setTerminal ( false );
    __state._state = sprop->getState();
    __state._state->setLogical ( true );

    addGlobalNets ( cell );

    yyin = ccell.getFile ();
    if ( not firstCall ) yyrestart ( yyin );

    yyparse ();

    ccell.close ();
    __state.reset ();

    UpdateSession::close ();

    return cell;
  }


} // End of CRL namespace.
