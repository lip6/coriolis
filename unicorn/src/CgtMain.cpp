// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./CgtMain.cpp"                            |
// +-----------------------------------------------------------------+


#include <Python.h>
#include <csignal>
#include <memory>
using namespace std;

#include <boost/program_options.hpp>
namespace bopts = boost::program_options;

// #include "vlsisapd/bookshelf/Exception.h"
#include "hurricane/configuration/Configuration.h"
#include "hurricane/utilities/Path.h"
#include "hurricane/DebugSession.h"
#include "hurricane/DataBase.h"
#include "hurricane/Cell.h"
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/isobar/Script.h"

#include <QtGui>
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and \
    (QT_VERSION <  QT_VERSION_CHECK(5,0,0)) and \
    not defined (__APPLE__) and not defined (__CYGWIN__) 
#  include  <QGtkStyle>
#endif

#include "hurricane/viewer/HApplication.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/SelectCommand.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/Hierarchy.h"
#include "crlcore/ToolBox.h"
//#include "crlcore/Ispd04Bookshelf.h"
#include "crlcore/Iccad04Lefdef.h"
#include "crlcore/DefImport.h"
#include "crlcore/DefExport.h"
using namespace CRL;

#include "katana/GraphicKatanaEngine.h"
#include "etesian/GraphicEtesianEngine.h"
#include "katana/GraphicKatanaEngine.h"
//#include "equinox/GraphicEquinoxEngine.h"
//#include "solstice/GraphicSolsticeEngine.h"
#include "unicorn/UnicornGui.h"
using namespace Unicorn;


// -------------------------------------------------------------------
// Function  :  "main()".

int main ( int argc, char *argv[] )
{
  int   returnCode = 0;
  bool  katanaSuccess = true;

  try {
    bool          destroyDatabase;
    float         edgeCapacity;
    unsigned long eventsLimit;
    bool          textMode;
    double        margin;
    bool          quadriPlace;
    bool          annealingPlace;
    unsigned int  partitionSizeStop;
    bool          globalRoute;
    bool          detailedRoute;
    bool          loadGlobal;
    bool          saveGlobal;
    bool          dumpMeasures;
    bool          exportDef;
    bool          saveImport;

    bopts::options_description options ("Command line arguments & options");
    options.add_options()
      ( "help,h"             , "Print this help." )
      ( "destroy-db"         , bopts::bool_switch(&destroyDatabase)->default_value(false)
                             , "Perform a complete deletion of the database (may be buggy).")
      ( "verbose,v"          , bopts::bool_switch()
                             , "First level of verbosity.")
      ( "very-verbose,V"     , bopts::bool_switch()
                             , "Second level of verbosity.")
      ( "info,i"             , bopts::bool_switch()
                             , "Lots of informational messages.")
      ( "show-conf"          , bopts::bool_switch()
                             , "Print Katana configuration settings.")
      ( "conf"               , bopts::value<string>()
                             , "An XML configuration file." )
      ( "core-dump,D"        , bopts::bool_switch()
                             , "Enable core dumping.")
      ( "log-mode,L"         , bopts::bool_switch()
                             , "Disable ANSI escape sequences displaying.")
      ( "text,t"             , bopts::bool_switch(&textMode)->default_value(false)
                             , "Run in pure text mode.")
      ( "stratus-script"     , bopts::value<string>()
                             , "Status script to run." )
      ( "margin,m"           , bopts::value<double>(&margin)
                             , "Percentage of free area to add to the minimal placement area.")
      ( "quadri-place,p"     , bopts::bool_switch(&quadriPlace)->default_value(false)
                             , "Place using quadripartitions then placement legalisation.")
      ( "annealing-place,P"  , bopts::bool_switch(&annealingPlace)->default_value(false)
                             , "Place using simulated annealing (slow).")
      ( "partition-size-stop", bopts::value<unsigned int>(&partitionSizeStop)
                             , "Sets the size of a leaf partition (quadripartition stage).")
      ( "global-route,G"     , bopts::bool_switch(&globalRoute)->default_value(false)
                             , "Run the global router (Knik).")
      ( "load-global,g"      , bopts::bool_switch(&loadGlobal)->default_value(false)
                             , "Reload the global routing from disk.")
      ( "save-global"        , bopts::bool_switch(&saveGlobal)->default_value(false)
                             , "Save the global routing solution.")
      ( "edge,e"             , bopts::value<float>(&edgeCapacity)
                             , "The egde density ratio applied on global router's edges." )
      ( "events-limit"       , bopts::value<unsigned long>(&eventsLimit)
                             , "The maximum number of iterations (events) that the router is"
                               "allowed to perform." )
      ( "detailed-route,R"   , bopts::bool_switch(&detailedRoute)->default_value(false)
                             , "Run the detailed router (Katana).")
      ( "dump-measures,M"    , bopts::bool_switch(&dumpMeasures)->default_value(false)
                             , "Dump statistical measurements on the disk.")
      ( "cell,c"             , bopts::value<string>()
                             , "The name of the cell to load, without extension." )
      ( "save-design,s"      , bopts::value<string>()
                             , "Save the routed design.")
      ( "export-def"         , bopts::bool_switch(&exportDef)->default_value(false)
                             , "Export the design in DEF format.")
      ( "import-def"         , bopts::value<string>()
                             , "Import the design in DEF format.")
      ( "importk-ispd04-bk"  , bopts::value<string>()
                             , "The name of the ISPD04 benchmark to import (Bookshelf .aux), without extension." )
      ( "import-iccad04-def" , bopts::value<string>()
                             , "The name of the ICCAD04 benchmark to import (LEF/DEF), without extension." )
      ( "save-import"        , bopts::bool_switch(&saveImport)->default_value(false)
                             , "Save of the imported design immediatly after loading.");

    bopts::variables_map arguments;
    bopts::store  ( bopts::parse_command_line(argc,argv,options), arguments );
    bopts::notify ( arguments );

    if ( arguments.count("help") ) {
      cout << options << endl;
      exit ( 0 );
    }

    if ( arguments.count("conf") ) {
      Utilities::Path userConfFile ( arguments["conf"].as<string>() );
      if ( userConfFile.exists() ) {
        Cfg::Configuration* conf = Cfg::Configuration::get ();
        conf->readFromFile ( userConfFile.toString() );
      } else {
        cerr << Warning("User defined configuration file:\n  <%s> not found."
                       ,userConfFile.toString().c_str()) << endl;
      }
    }

    if (arguments["core-dump"   ].as<bool>()) Cfg::getParamBool("misc.catchCore"    )->setBool ( false, Cfg::Parameter::CommandLine );
    if (arguments["verbose"     ].as<bool>()) Cfg::getParamBool("misc.verboseLevel1")->setBool ( true , Cfg::Parameter::CommandLine );
    if (arguments["very-verbose"].as<bool>()) Cfg::getParamBool("misc.verboseLevel2")->setBool ( true , Cfg::Parameter::CommandLine );
    if (arguments["info"        ].as<bool>()) Cfg::getParamBool("misc.info"         )->setBool ( true , Cfg::Parameter::CommandLine );
    if (arguments["log-mode"    ].as<bool>()) Cfg::getParamBool("misc.logMode"      )->setBool ( true , Cfg::Parameter::CommandLine );
    if (arguments["show-conf"   ].as<bool>()) Cfg::getParamBool("misc.showConf"     )->setBool ( true , Cfg::Parameter::CommandLine );
    bool showConf = Cfg::getParamBool("misc.showConf")->asBool();

    dbo_ptr<DataBase>          db   ( DataBase::getDB() );
    dbo_ptr<AllianceFramework> af   ( AllianceFramework::get() );
    Cell*                      cell = NULL;

    Utilities::Path path = Utilities::Path::cwd();
    vector<Utilities::Path>  cwdEntries = path.listdir();
    cerr << "Contents of cwd:" << path.toString() << endl;
    for ( Utilities::Path entry : cwdEntries ) {
      cerr << "- " << entry.toString() << endl; 
    }

    if ( arguments.count("cell") ) {
      cell = af->getCell (arguments["cell"].as<string>().c_str(), Catalog::State::Views );
      if (!cell) {
        cerr << af->getPrint() << endl;
        cerr << "[ERROR] Cell not found: " << arguments["cell"].as<string>() << endl;
        exit ( 2 );
      }
    } else {
      quadriPlace    = false;
      annealingPlace = false;
      loadGlobal     = false;
      saveGlobal     = false;
      globalRoute    = false;
      detailedRoute  = false;
    }

    if ( (cell == NULL) and arguments.count("import-def") ) {
      cell = DefImport::load ( arguments["import-def"].as<string>().c_str()
                             , DefImport::FitAbOnCells
                             );
    }

    if ( (cell == NULL) and arguments.count("import-iccad04-def") ) {
      cell = Iccad04Lefdef::load ( arguments["import-iccad04-def"].as<string>().c_str()
                                 , 0
                                 );
    }

    // if ( (cell == NULL) and arguments.count("import-ispd04-bk") ) {
    //   cell = Ispd04::load ( arguments["import-ispd04-bk"].as<string>().c_str() );
    // }

    if ( saveImport and (cell != NULL) ) {
      cmess1 << "  o  Immediate write back of <" << cell->getName() << ">" << endl;
      af->saveCell ( cell, Catalog::State::Views );
    }

    if ( (cell != NULL) and exportDef ) {
      DefExport::drive ( cell, DefExport::WithLEF );
    }

    if ( arguments.count("margin") )
      Cfg::getParamPercentage("nimbus.spaceMargin")->setPercentage ( margin );

    if ( arguments.count("partition-size-stop") )
      Cfg::getParamInt("metis.numberOfInstancesStopCriterion")->setInt ( partitionSizeStop );

    if ( arguments.count("edge") )
      Cfg::getParamPercentage("katana.edgeCapacity")->setPercentage ( edgeCapacity );

    if ( arguments.count("events-limit") )
      Cfg::getParamInt("katana.eventsLimit")->setInt ( eventsLimit );

    UnicornGui::getBanner().setName    ( "cgt" );
    UnicornGui::getBanner().setPurpose ( "Coriolis Graphical Tool" );

    cmess1 << UnicornGui::getBanner() << endl;
    cmess1 << "        Tool Credits" << endl;
    cmess1 << "        Hurricane .................... Remy Escassut & Christian Masson" << endl;
    cmess1 << "        Etesian - Placer .............................. Gabriel Gouvine" << endl;
    cmess1 << "        Katana - Detailed Router ...................... Jean-Paul Chaput" << endl;
    cmess1 << "        " << endl;

    cmess1 << "        Contributors" << endl;
    cmess1 << "        Sophie Belloeil, Hugo Clement, Marek Sroka, Wu Yifei" << endl;
    cmess1 << endl;

    cmess1 << "        Coloquinte software credits (used by Etesian)" << endl;
    cmess1 << "        Author ........................................ Gabriel Gouvine" << endl;
    cout   << endl;

    cout   << "        FLUTE software credits (used by Knik)" << endl;
    cout   << "        Author ........................................ Chris C. N. CHU" << endl;
    cout   << "        Prof. Ident. ............................ Iowa State University" << endl;
    cout   << "        URL ........................ http://home.eng.iastate.edu/~cnchu" << endl;
    cout   << endl;
  //cmess2 << af->getPrint() << endl;

    // if ( arguments.count("stratus-script") ) {
    //   string scriptName = arguments["stratus-script"].as<string>();
    //   cmess1 << "  o  Running stratus script:" << endl;
    //   cmess1 << "     - <" << scriptName << ">" << endl;

    //   dbo_ptr<StratusScript> script = StratusScript::create ( scriptName, NULL );
    //   script->run ();
    // }

    if ( cell ) {
    // Python Script test.

    //Isobar::Script::addPath ( "/dsk/l1/jpc/coriolis-2.x/work/benchs/routing/addaccu" );

      // for ( int i=0 ; i<25 ; ++i ) {
      //   cerr << "Call:" << i << "/25 ======================================" << endl;

      //   Isobar::Script* script = Isobar::Script::create ( "getCellName" );
      //   script->runFunction ( "__hurricane_main__", cell );
      // }
      // cerr << "DONE" << endl;

      // Box box ( DbU::lambda(0.0), DbU::lambda(0.0), DbU::lambda(5.0), DbU::lambda(10.0) );
      
      // throw Error ("Tu le vois bien: <b>%s</b><br>"
      //             "Un retour a la ligne\n<i>Une ligne</i>"
      //             ,getString(box).c_str() );
    }

    if ( not textMode ) {
      unique_ptr<QApplication> qa ( new HApplication(argc,argv) );
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and \
    (QT_VERSION <  QT_VERSION_CHECK(5,0,0)) and \
    not defined (__APPLE__) and not defined (__CYGWIN__)
      qa->setStyle ( new QGtkStyle() );
#endif
      Graphics::enable ();

      dbo_ptr<UnicornGui> unicorn ( UnicornGui::create() );
      unicorn->setApplicationName ( QObject::tr("cgt") );

      unicorn->registerTool ( Katana::GraphicKatanaEngine::grab() );
      unicorn->registerTool ( Etesian::GraphicEtesianEngine::grab() );
    //unicorn->setEnableRedrawInterrupt ( true );
    //unicorn->registerTool ( Equinox::GraphicEquinoxEngine::grab() );
    //unicorn->registerTool ( Solstice::GraphicSolsticeEngine::grab() );

      SelectCommand* command = static_cast<SelectCommand*>( unicorn->getCellWidget()->getCommand(SelectCommand::getStaticName()) );
      if ( command ) command->setSelectMode ( SelectCommand::NoAnonNetMode );

      unicorn->getCellWidget()->setLayerVisible ( "grid"          , false );
      unicorn->getCellWidget()->setLayerVisible ( "text.instance" , false );
      unicorn->getCellWidget()->setLayerVisible ( "text.component", false );
      unicorn->setCell ( cell );
      unicorn->show    ();

      returnCode = qa->exec ();
      ToolEngine::destroyAll ();
    } else {
      if ( quadriPlace or annealingPlace ) {
        loadGlobal  = false;
        globalRoute = true;
      }
      if ( quadriPlace and annealingPlace )
        annealingPlace = false;

      // if ( not MetisEngine::isHMetisCapable() and quadriPlace ) {
      //   cerr << Warning("hMETIS is not avalaible, revert to simulated annealing.") << endl;
        
      //   annealingPlace = true;
      //   quadriPlace    = false;
      // }

      // bool runMaukaTool = quadriPlace or annealingPlace;

      // if ( runMaukaTool ) {
      //   NimbusEngine* nimbus = NULL;
      //   MetisEngine*  metis  = NULL;
      //   MaukaEngine*  mauka  = NULL;

      //   nimbus = NimbusEngine::create ( cell );
      //   if ( showConf ) nimbus->printConfiguration();

      //   if ( annealingPlace ) {
      //     Cfg::getParamPercentage("mauka.standardAnnealing")->setBool ( true );
      //   }

      //   if ( quadriPlace ) {
      //     metis  = MetisEngine::create ( cell );
      //     if ( showConf ) metis->printConfiguration();

      //     MetisEngine::doQuadriPart ( cell );
      //     MaukaEngine::regroupOverloadedGCells ( cell );
      //   }

      //   mauka = MaukaEngine::create ( cell );
      //   if ( showConf ) mauka->printConfiguration();

      //   mauka->Run ();
      // }

      if ( detailedRoute and not (loadGlobal or globalRoute) ) globalRoute = true;

      bool runKatanaTool = loadGlobal or globalRoute or detailedRoute;

      if ( runKatanaTool ) {
      //cell->flattenNets ( not arguments.count("global") );

        // unsigned int globalFlags = (loadGlobal) ? Katana::LoadGlobalRouting
        //                                         : Katana::BuildGlobalRouting;

        Katana::KatanaEngine* katana = Katana::KatanaEngine::create ( cell );
        if ( showConf ) katana->printConfiguration ();
        
        katana->runGlobalRouter ( Katana::Flags::NoFlags );
        // if ( saveGlobal ) katana->saveGlobalSolution ();

        if ( detailedRoute ) {
          katana->loadGlobalRouting ( Anabatic::EngineLoadGrByNet );
          katana->layerAssign       ( Anabatic::EngineNoNetLayerAssign );
          katana->runNegociate      ();
          katanaSuccess = katana->isDetailedRoutingSuccess ();
          katana->finalizeLayout ();
          if ( dumpMeasures )
            katana->dumpMeasures ();
          katana->destroy ();

          if ( arguments.count("save-design") ) {
            cell->setName ( arguments["save-design"].as<string>().c_str() );
            af->saveCell ( cell, Catalog::State::Physical );
          }
        }

        returnCode = (katanaSuccess) ? 0 : 1;
      }
    }

    if ( not destroyDatabase ) exit ( 0 );
    cmess1 << "  o  Full database deletion (may be buggy)." << endl;
  }
  catch ( Bug& e ) {
    cerr << e.what() << endl;
    exit ( 1 );
  }
  catch ( Error& e ) {
    cerr << e.what() << endl;
    cerr << "\nProgram stack:\n" << e.where() << endl;
    exit ( 1 );
  }
  catch ( bopts::error& e ) {
    cerr << "[ERROR] " << e.what() << endl;
    exit ( 1 );
  }
  // catch ( Bookshelf::Exception& e ) {
  //   cerr << e.what() << endl;
  //   exit ( 1 );
  // }
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
