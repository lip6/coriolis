
#include  <boost/program_options.hpp>
namespace poptions = boost::program_options;

#include "hurricane/DataBase.h"
#include "hurricane/Cell.h"
#include "hurricane/Warning.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
using namespace CRL;

#include  <QtGui>

int main(int argc, char *argv[]) {
    
  try {
    unsigned int  traceLevel;
    bool          verbose1;
    bool          verbose2;
    bool          coreDump;

    poptions::options_description options ("Command line arguments & options");
    options.add_options()
      ( "help,h"        , "Print this help." )
      ( "verbose,v"     , poptions::bool_switch(&verbose1)->default_value(false)
                        , "First level of verbosity.")
      ( "very-verbose,V", poptions::bool_switch(&verbose2)->default_value(false)
                        , "Second level of verbosity.")
      ( "core-dump,D"   , poptions::bool_switch(&coreDump)->default_value(false)
                        , "Enable core dumping.")
      ( "trace-level,l" , poptions::value<unsigned int>(&traceLevel)->default_value(1000)
                        , "Set the level of trace, trace messages with a level superior to "
                          "<arg> will be printed on <stderr>." )
      ( "cell,c"        , poptions::value<string>()
                        , "The name of the cell to load, whithout extension." );

    poptions::variables_map arguments;
    poptions::store  ( poptions::parse_command_line(argc,argv,options), arguments );
    poptions::notify ( arguments );

    if ( arguments.count("help") ) {
      cout << options << endl;
      exit ( 0 );
    }

    System::get()->setCatchCore ( not coreDump ); 
    if (verbose1) mstream::enable ( mstream::Verbose0|mstream::Verbose1 );
    if (verbose2) mstream::enable ( mstream::Verbose0|mstream::Verbose1|mstream::Verbose2 ); 

    AllianceFramework* af = AllianceFramework::get ();

    Cell* cell = NULL;

    if ( arguments.count("cell") ) {
      cell = af->getCell (arguments["cell"].as<string>().c_str(), Catalog::State::Views );
      if (!cell) {
        cerr << af->getPrint() << endl;
        cerr << "[ERROR] Cell not found: " << arguments["cell"].as<string>() << endl;
        exit ( -45 );
      }
    }

    af->saveCell(cell, Catalog::State::Physical);
    af->destroy ();
  }
  catch ( Error& e ) {
    cerr << e.what() << endl;
    exit ( 1 );
  }
  catch ( ... ) {
    cout << "[ERROR] Abnormal termination: unmanaged exception.\n" << endl;
    exit ( 2 );
  }

  return 0;
}
