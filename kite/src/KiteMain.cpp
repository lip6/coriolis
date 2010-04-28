
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./KiteMain.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <memory>
using namespace std;

#include  <boost/program_options.hpp>
namespace poptions = boost::program_options;

#include  "hurricane/DebugSession.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Warning.h"
#include  "hurricane/UpdateSession.h"
#include  "hurricane/viewer/HApplication.h"
#include  "hurricane/viewer/Graphics.h"
using namespace Hurricane;

#include  "crlcore/Utilities.h"
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/Hierarchy.h"
#include  "crlcore/ToolBox.h"
using namespace CRL;

#include  "knik/KnikEngine.h"
using namespace Knik;

#include  "kite/KiteEngine.h"
using namespace Kite;


namespace {


// -------------------------------------------------------------------
// Function  :  "printHelp()".

  void  printHelp ()
  {
    cout << endl;
    cout << "Usage: kite [-v|--verbose] [-V|--very-verbose] [-D|--core-dump]    \\\n"
         << "           [-l|--trace-level <traceLevel>] [-c|--cell <cellName>] \\\n"
         << endl;
    cout << "Options:\n"
         << "    o  [-v|--verbose]      : First level of verbosity.\n"
         << "    o  [-V|--very-verbose] : Second level of verbosity (very talkative).\n"
         << "    o  [-D|--core-dump]    : Enable core dumping.\n"
         << "    o  [-l|--trace-level <traceLevel>] :\n"
         << "           Sets the level of trace, trace messages with a level superior to\n"
         << "           <traceLevel> will be printed on <stderr>.\n"
         << "    o  [-c|--cell <cellName>] :\n"
         << "           The name of the Cell to load, without extention.\n"
         << endl;
  }


} // End of anonymous namespace.




// x-----------------------------------------------------------------x
// |                      Fonctions Definitions                      |
// x-----------------------------------------------------------------x


// -------------------------------------------------------------------
// Function  :  "main()".

int main ( int argc, char *argv[] )
{
  int   returnCode  = 0;
  bool  kiteSuccess = false;

  try {
    float         edgeCapacity;
    unsigned int  traceLevel;
    bool          verbose1;
    bool          verbose2;
    bool          info;
    bool          coreDump;
    bool          logMode;
    bool          loadGlobal;
    bool          saveDesign;

    poptions::options_description options ("Command line arguments & options");
    options.add_options()
      ( "help,h"        , "Print this help." )
      ( "verbose,v"     , poptions::bool_switch(&verbose1)->default_value(false)
                        , "First level of verbosity.")
      ( "very-verbose,V", poptions::bool_switch(&verbose2)->default_value(false)
                        , "Second level of verbosity.")
      ( "info,i"        , poptions::bool_switch(&info)->default_value(false)
                        , "Lots of informational messages.")
      ( "core-dump,D"   , poptions::bool_switch(&coreDump)->default_value(false)
                        , "Enable core dumping.")
      ( "log-mode,L"    , poptions::bool_switch(&logMode)->default_value(false)
                        , "Disable ANSI escape sequences displaying.")
      ( "global,g"      , poptions::bool_switch(&loadGlobal)->default_value(false)
                        , "Reload the global routing from disk.")
      ( "trace-level,l" , poptions::value<unsigned int>(&traceLevel)->default_value(1000)
                        , "Set the level of trace, trace messages with a level superior to "
                          "<arg> will be printed on <stderr>." )
      ( "edge,e"        , poptions::value<float>(&edgeCapacity)->default_value(65.0)
                        , "The egde density ratio applied on global router's edges." )
      ( "cell,c"        , poptions::value<string>()
                        , "The name of the cell to load, whithout extension." )
      ( "save,s"        , poptions::bool_switch(&saveDesign)->default_value(false)
                        , "Save the routed design.");

    poptions::variables_map arguments;
    poptions::store  ( poptions::parse_command_line(argc,argv,options), arguments );
    poptions::notify ( arguments );

    if ( arguments.count("help") or not arguments.count("cell") ) {
      cout << options << endl;
      exit ( 0 );
    }

    System::getSystem()->setCatchCore ( not coreDump ); 

    if ( verbose1 ) mstream::enable ( mstream::VerboseLevel1 );
    if ( verbose2 ) mstream::enable ( mstream::VerboseLevel2 ); 
    if ( info     ) mstream::enable ( mstream::Info ); 
    if ( logMode  ) tty::disable ();

    ltracelevel ( traceLevel );

    dbo_ptr<DataBase>          db ( DataBase::create() );
    dbo_ptr<AllianceFramework> af ( AllianceFramework::create() );
    Cell* cell = NULL;

    if ( arguments.count("cell") ) {
      cell = af->getCell (arguments["cell"].as<string>().c_str(), Catalog::State::Views );
      if (!cell) {
        cerr << af->getPrint() << endl;
        cerr << "[ERROR] Cell not found: " << arguments["cell"].as<string>() << endl;
        exit ( 2 );
      }
    }

    Kite::Configuration::getDefault()->setEdgeCapacityPercent ( edgeCapacity );

    cell->flattenNets ( not arguments.count("global") );

    KnikEngine* knik = KnikEngine::create ( cell );
    if ( not loadGlobal ) {
      knik->run ();
    } else {
      knik->createRoutingGraph ();
      knik->loadSolution       ();
    }

    static vector<Net*> routingNets;
    KiteEngine* kite = KiteEngine::create ( cell );
    // kite->setSaturateRatio      ( 0.85 );
    // kite->setExpandStep         ( 0.20 );
    // kite->setRipupCost          ( 3 );
    // kite->setRipupLimit         ( 25, Configuration::BorderRipupLimit );
    // kite->setRipupLimit         ( 15, Configuration::StrapRipupLimit );
    // kite->setRipupLimit         (  5, Configuration::LocalRipupLimit );
    // kite->setRipupLimit         (  5, Configuration::GlobalRipupLimit );
    // kite->setRipupLimit         (  5, Configuration::LongGlobalRipupLimit );
    kite->runGlobalRouter       ( Kite::LoadGlobalSolution );
    kite->loadGlobalRouting     ( Katabatic::LoadGrByNet, routingNets );
    kite->layerAssign           ( Katabatic::NoNetLayerAssign );
    kite->runNegociate          ();
    kiteSuccess = kite->getToolSuccess ();
    kite->finalizeLayout        ();
    kite->destroy               ();
    knik->destroy               ();

    if ( saveDesign ) {
      string name = getString(cell->getName()) + "_kite";
      cell->setName ( name );
      af->saveCell ( cell, Catalog::State::Physical );
    }

    returnCode = (kiteSuccess) ? 0 : 1;
  }
  catch ( poptions::error& e ) {
    cerr << "[ERROR] " << e.what() << endl;
    exit ( 1 );
  }
  catch ( Error& e ) {
    cerr << e.what() << endl;
    exit ( 1 );
  }
  catch ( ... ) {
    cout << "[ERROR] Abnormal termination: unmanaged exception.\n" << endl;
    exit ( 2 );
  }

  return returnCode;
}
