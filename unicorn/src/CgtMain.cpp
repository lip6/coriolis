
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
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./CgtMain.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <memory>
using namespace std;

#include  <boost/program_options.hpp>
namespace poptions = boost::program_options;

#include  <QtGui>
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and not defined (__APPLE__)
#  include  <QGtkStyle>
#endif

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

#include  "knik/GraphicKnikEngine.h"
using namespace Knik;

#include  "kite/GraphicKiteEngine.h"
using namespace Kite;

#include  "equinox/GraphicEquinoxEngine.h"
using namespace Equinox;

#include  "solstice/GraphicSolsticeEngine.h"
using namespace Solstice;

#include  "unicorn/UnicornGui.h"
using namespace Unicorn;


namespace {


// -------------------------------------------------------------------
// Function  :  "printHelp()".

  void  printHelp ()
  {
    cout << endl;
    cout << "Usage: cgt [-v|--verbose] [-V|--very-verbose] [-D|--core-dump]    \\\n"
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
    unsigned long eventsLimit;
    unsigned int  traceLevel;
    bool          verbose1;
    bool          verbose2;
    bool          info;
    bool          coreDump;
    bool          logMode;
    bool          textMode;
    bool          loadGlobal;
    bool          saveGlobal;
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
      ( "text,t"        , poptions::bool_switch(&textMode)->default_value(false)
                        , "Run in pure text mode.")
      ( "global,g"      , poptions::bool_switch(&loadGlobal)->default_value(false)
                        , "Reload the global routing from disk.")
      ( "trace-level,l" , poptions::value<unsigned int>(&traceLevel)->default_value(1000)
                        , "Set the level of trace, trace messages with a level superior to "
                          "<arg> will be printed on <stderr>." )
      ( "tool,T"        , poptions::value<string>()
                        , "The tool to be run, in text mode." )
      ( "edge,e"        , poptions::value<float>(&edgeCapacity)->default_value(0.65)
                        , "The egde density ratio applied on global router's edges." )
      ( "events-limit"  , poptions::value<unsigned long>(&eventsLimit)
                        , "The maximum number of iterations (events) that the router is"
                          "allowed to perform." )
      ( "cell,c"        , poptions::value<string>()
                        , "The name of the cell to load, whithout extension." )
      ( "save,s"        , poptions::bool_switch(&saveDesign)->default_value(false)
                        , "Save the routed design.")
      ( "save-global"   , poptions::bool_switch(&saveGlobal)->default_value(false)
                        , "Save the global routing solution.");

    poptions::variables_map arguments;
    poptions::store  ( poptions::parse_command_line(argc,argv,options), arguments );
    poptions::notify ( arguments );

    if ( arguments.count("help") ) {
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

    Kite::Configuration::setDefaultEdgeCapacity ( edgeCapacity );
  //KnikEngine::setEdgeCapacityPercent ( edgeCapacity );

    if ( cell ) {
    // addaccu.
    //DebugSession::addToTrace ( cell, "sel" );
    //DebugSession::addToTrace ( cell, "auxreg3" );
    //DebugSession::addToTrace ( cell, "auxsc21" );
    //DebugSession::addToTrace ( cell, "ram_nwen" );
    //DebugSession::addToTrace ( cell, "acc_o_down" );
    // model_shift.
    //DebugSession::addToTrace ( cell, "c1_2" );
    //DebugSession::addToTrace ( cell, "muxoutput_96" );
    //DebugSession::addToTrace ( cell, "msb" );
    //DebugSession::addToTrace ( cell, "op(0)" );
    //DebugSession::addToTrace ( cell, "i(1)" );
    // rf2r0_32x32.
    //DebugSession::addToTrace ( cell, "busa(0)" );
    // amd2901.
    //DebugSession::addToTrace ( cell, "acc_q(1)" );
    //DebugSession::addToTrace ( cell, "ram_adra(11)" );
    //DebugSession::addToTrace ( cell, "ram_adri(5)" );
    //DebugSession::addToTrace ( cell, "ram_q2(0)" );
    //DebugSession::addToTrace ( cell, "ram_reg5_nckx" );
    //DebugSession::addToTrace ( cell, "acc_reg_nscanx" );
    //DebugSession::addToTrace ( cell, "acc_o_down" );
    //DebugSession::addToTrace ( cell, "nb(3)" );
    // MIPS microprogrammed.
    //DebugSession::addToTrace ( cell, "crsrout_0" );
    //DebugSession::addToTrace ( cell, "dpt_dt_s_30" );
    //DebugSession::addToTrace ( cell, "dpt_wm_rf_dinx_0" );
    //DebugSession::addToTrace ( cell, "dpt_adder_sub_pi_0_10" );
    //DebugSession::addToTrace ( cell, "dpt_ishifter_c0_3" );
    //DebugSession::addToTrace ( cell, "dpt_adder_sub_r_16" );
    //DebugSession::addToTrace ( cell, "dpt_adder_sub_gi_0_24" );
    //DebugSession::addToTrace ( cell, "dpt_ishifter_muxoutput_117" );
    // MIPS pipeline.
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_shift32_rshift_se_muxoutput(98)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_shift32_rshift_se_msb" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_rarith_se(31)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_ct_nxr2_x1_7_sig" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_banc_busa(17)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_instaddbracry_sd_pi_1_17" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_yoper_se(16)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_toper_se(22)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_soper_se(22)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_jadr_sd(14)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_effto_sd(1)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_ioper_rd(30)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_nextpc_rd(1)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_ct_not_kill_se" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_ct_not_opcod_sd_2" );
    //DebugSession::addToTrace ( cell, "rsdnbr_sd(14)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_otheri_sd(31)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_soper_se(31)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_braadr_sd(3)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_seqadr_sd(30)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_data_rm(8)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_ct_not_i_ri(11)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_ct_noa22_x1_61_sig" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_ct_not_opcod_rd(6)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_ct_not_aux190" );
    //DebugSession::addToTrace ( cell, "s_eq_t_sd" );
    //DebugSession::addToTrace ( cell, "i(22)" );
    //DebugSession::addToTrace ( cell, "mips_r3000_1m_dp_mux32_data_o_sm_sel0" );
    // DCT/lvl3
    //DebugSession::addToTrace ( cell, "instance139_fixed_multiplier_mixed_direct_nbit0_16_nbit1_16_specified_report_5_18_0" );
    //DebugSession::addToTrace ( cell, "instance129_fixed_multiplier_mixed_direct_nbit0_16_nbit1_16_cin_i0_true_specified_colout_1_11_2" );
    //DebugSession::addToTrace ( cell, "coef0(7)" );
    //DebugSession::addToTrace ( cell, "c1_0(3)" );
    // eFPGA/04x04.
    //DebugSession::addToTrace ( cell, "group_2_4_nq_mem_45" );
    //DebugSession::addToTrace ( cell, "g_2_1_rig_7" );
    // eFPGA/08x08.
    //DebugSession::addToTrace ( cell, "group_1_7_lut_mux_q01" );
    //DebugSession::addToTrace ( cell, "group_0_5_decoder_vertical_track_0_not_cmd(1)" );
    // eFPGA/16x16.
    //DebugSession::addToTrace ( cell, "group_15_6_clb_topside_in1_mux_q01" );
    //DebugSession::addToTrace ( cell, "g_7_14_top_2" );
    }

    if ( not textMode ) {
      auto_ptr<QApplication> qa ( new HApplication(argc,argv) );
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and not defined (__APPLE__)
      qa->setStyle ( new QGtkStyle() );
#endif
      Graphics::enable ();

      dbo_ptr<UnicornGui> unicorn ( UnicornGui::create() );
      unicorn->setApplicationName     ( QObject::tr("cgt") );
      unicorn->getBanner().setName    ( "cgt" );
      unicorn->getBanner().setPurpose ( "Coriolis Graphical Tool" );

      cmess1 << unicorn->getBanner() << endl;
      cmess2 << af->getPrint() << endl;
    //unicorn->registerTool ( Knik::GraphicKnikEngine::grab() );
      unicorn->registerTool ( Kite::GraphicKiteEngine::grab() );
      unicorn->setEnableRedrawInterrupt ( true );
    //unicorn->registerTool ( Equinox::GraphicEquinoxEngine::grab() );
    //unicorn->registerTool ( Solstice::GraphicSolsticeEngine::grab() );

      unicorn->getCellWidget()->setLayerVisible ( "grid", false );
      unicorn->setCell ( cell );
      unicorn->show    ();

      returnCode = qa->exec ();
      ToolEngine::destroyAll ();
    } else {
      if ( arguments.count("tool") and (arguments["tool"].as<string>() == "kite") ) {
      //cell->flattenNets ( not arguments.count("global") );

        unsigned int globalFlags = (arguments["global"].as<bool>()) ? Kite::LoadGlobalSolution
                                                                    : Kite::BuildGlobalSolution;

        static vector<Net*> routingNets;
        KiteEngine* kite = KiteEngine::create ( af->getRoutingGauge(), cell );
        
        kite->runGlobalRouter ( globalFlags );
        if ( saveGlobal ) kite->saveGlobalSolution ();

        kite->loadGlobalRouting     ( Katabatic::LoadGrByNet, routingNets );
        kite->layerAssign           ( Katabatic::NoNetLayerAssign );
        kite->runNegociate          ();
        kiteSuccess = kite->getToolSuccess ();
        kite->finalizeLayout        ();
        kite->dumpMeasures          ();
        kite->destroy               ();

        if ( saveDesign ) {
          string name = getString(cell->getName()) + "_kite";
          cell->setName ( name );
          af->saveCell ( cell, Catalog::State::Physical );
        }

        returnCode = (kiteSuccess) ? 0 : 1;
      }
    }
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
