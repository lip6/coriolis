// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./KiteMain.cpp"                           |
// +-----------------------------------------------------------------+


#include <memory>
using namespace std;

#include <boost/program_options.hpp>
namespace bopts = boost::program_options;

#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/DebugSession.h"
#include "hurricane/DataBase.h"
#include "hurricane/Cell.h"
#include "hurricane/Warning.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/Banner.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/Hierarchy.h"
#include "crlcore/ToolBox.h"
using namespace CRL;

#include "knik/KnikEngine.h"
using namespace Knik;

#include "kite/KiteEngine.h"
using namespace Kite;


// -------------------------------------------------------------------
// Function  :  "main()".

int main ( int argc, char *argv[] )
{
  int   returnCode  = 0;
  bool  kiteSuccess = false;

  try {
    Banner banner( "Kite"
                 , "1.0b"
                 , "Coriolis Router"
                 , "2008"
                 , "Jean-Paul Chaput"
                 , ""
                 );

    unsigned int  traceLevel;
    bool          verbose1;
    bool          verbose2;
    bool          showConf;
    bool          info;
    bool          bug;
    bool          coreDump;
    bool          logMode;
    bool          loadGlobal;
    bool          dumpMeasures;
    bool          saveGlobal;
    bool          destroyDatabase;

    bopts::options_description options ("Command line arguments & options");
    options.add_options()
      ( "help,h"         , "Print this help." )
      ( "verbose,v"      , bopts::bool_switch(&verbose1)->default_value(false)
                         , "First level of verbosity.")
      ( "very-verbose,V" , bopts::bool_switch(&verbose2)->default_value(false)
                         , "Second level of verbosity.")
      ( "show-conf"      , bopts::bool_switch(&showConf)->default_value(false)
                         , "Print Kite configuration settings.")
      ( "info,i"         , bopts::bool_switch(&info)->default_value(false)
                         , "Lots of informational messages.")
      ( "bug,b"          , bopts::bool_switch(&bug)->default_value(false)
                         , "Display bug related messages.")
      ( "log-mode,L"     , bopts::bool_switch(&logMode)->default_value(false)
                         , "Disable ANSI escape sequences displaying.")
      ( "trace-level,l"  , bopts::value<unsigned int>(&traceLevel)->default_value(1000)
                         , "Set the level of trace, trace messages with a level superior to "
                           "<arg> will be printed on <stderr>." )
      ( "core-dump,D"    , bopts::bool_switch(&coreDump)->default_value(false)
                         , "Enable core dumping.")
      ( "load-global,g"  , bopts::bool_switch(&loadGlobal)->default_value(false)
                         , "Do *not* run the global router (Knik), reuse previous routing (.kgr).")
      ( "save-global"    , bopts::bool_switch(&saveGlobal)->default_value(false)
                         , "Save the global routing solution.")
      ( "dump-measures,M", bopts::bool_switch(&dumpMeasures)->default_value(false)
                         , "Dump statistical measurements on the disk.")
      ( "cell,c"         , bopts::value<string>()
                         , "The name of the cell to load, without extension." )
      ( "save-design,s"  , bopts::value<string>()
                         , "Save the routed design under the given name.")
      ( "destroy-db"     , bopts::bool_switch(&destroyDatabase)->default_value(false)
                         , "Perform a complete deletion of the database (may be buggy).");

    bopts::variables_map arguments;
    bopts::store  ( bopts::parse_command_line(argc,argv,options), arguments );
    bopts::notify ( arguments );

    if ( arguments.count("help") or not arguments.count("cell") ) {
      cout << banner << endl;
      cout << options << endl;
      exit ( 0 );
    }

    Cfg::Configuration::pushDefaultPriority( Cfg::Parameter::CommandLine );
    if (arguments["core-dump"   ].as<bool>()) Cfg::getParamBool("misc.catchCore"    )->setBool( false );
    if (arguments["verbose"     ].as<bool>()) Cfg::getParamBool("misc.verboseLevel1")->setBool( true  );
    if (arguments["very-verbose"].as<bool>()) Cfg::getParamBool("misc.verboseLevel2")->setBool( true  );
    if (arguments["info"        ].as<bool>()) Cfg::getParamBool("misc.info"         )->setBool( true  );
    if (arguments["bug"         ].as<bool>()) Cfg::getParamBool("misc.bug"          )->setBool( true  );
    if (arguments["log-mode"    ].as<bool>()) Cfg::getParamBool("misc.logMode"      )->setBool( true  );
    if (arguments["show-conf"   ].as<bool>()) Cfg::getParamBool("misc.showConf"     )->setBool( true  );

    if (arguments.count("trace-level" )) Cfg::getParamInt("misc.traceLevel")->setInt( traceLevel );
    Cfg::Configuration::popDefaultPriority();

    cmess1 << banner << endl;
    showConf = Cfg::getParamBool("misc.showConf")->asBool();

    dbo_ptr<DataBase>          db ( DataBase::create() );
    dbo_ptr<AllianceFramework> af ( AllianceFramework::create() );
    Cell* cell = NULL;

    if ( arguments.count("cell") ) {
      cell = af->getCell( arguments["cell"].as<string>().c_str(), Catalog::State::Views );
      if (not cell) {
        cerr << af->getPrint() << endl;
        cerr << "[ERROR] Cell not found: " << arguments["cell"].as<string>() << endl;
        exit ( 2 );
      }
    }

    unsigned int globalFlags = (loadGlobal) ? Kite::KtLoadGlobalRouting
                                            : Kite::KtBuildGlobalRouting;

    KiteEngine* kite = KiteEngine::create( cell );
    if (showConf) kite->printConfiguration();

    kite->runGlobalRouter( globalFlags );
    if (saveGlobal) kite->saveGlobalSolution ();

    kite->loadGlobalRouting   ( Katabatic::EngineLoadGrByNet );
    kite->balanceGlobalDensity();
    kite->layerAssign         ( Katabatic::EngineNoNetLayerAssign );
    kite->runNegociate        ();
    kiteSuccess = kite->getToolSuccess();
    kite->finalizeLayout   ();

    if (dumpMeasures) kite->dumpMeasures();
    kite->destroy();

    if (arguments.count("save-design")) {
      string name = arguments["save-design"].as<string>();
      cell->setName ( name );
      af->saveCell ( cell, Catalog::State::Physical );
    }

    returnCode = (kiteSuccess) ? 0 : 1;

    if (not destroyDatabase) exit( returnCode );
    cmess1 << "  o  Full database deletion (may be buggy)." << endl;
  }
  catch ( bopts::error& e ) {
    cerr << "[ERROR] " << e.what() << endl;
    exit ( 1 );
  }
  catch ( Error& e ) {
    cerr << e.what() << endl;
    exit ( 1 );
  }
  catch ( exception& e ) {
    cerr << "[ERROR] " << e.what() << endl;
    exit ( 1 );
  }
  catch ( ... ) {
    cout << "[ERROR] Abnormal termination: unmanaged exception.\n" << endl;
    exit ( 2 );
  }

  return returnCode;
}
