

#include  <boost/program_options.hpp>
namespace boptions = boost::program_options;

#include "hurricane/DebugSession.h"
#include "hurricane/Interval.h"
#include "hurricane/RbTree.h"
#include "hurricane/IntervalTree.h"
#include "crlcore/Utilities.h"

namespace Hurricane {

  typedef  IntervalData<std::string>                IntvString;
  typedef  RbTree<Interval,Interval::CompareByMin>  RbInterval;
  typedef  IntervalTree<std::string>                IntervalTreeString;
  typedef  GenericCollection<Interval>              Intervals;
  typedef  GenericCollection<IntvString>            IntvStrings;

}

INSPECTOR_PR_SUPPORT(Hurricane::RbInterval);
INSPECTOR_PR_SUPPORT(Hurricane::RbInterval::Node);
INSPECTOR_PR_SUPPORT(Hurricane::IntvString);
INSPECTOR_PR_SUPPORT(Hurricane::IntervalTreeString);
INSPECTOR_PR_SUPPORT(Hurricane::IntervalTreeString::Node);

using namespace std;
using namespace Hurricane;
using namespace CRL;


namespace {

  using namespace std;
  using namespace Hurricane;


  inline DbU::Unit  l ( long v ) { return DbU::fromLambda(v); }
  
  
// -------------------------------------------------------------------
// Test  :  "testRbTree".


  int  testRbTree ()
  {
  //typedef  IntervalTree  RbInterval;
    RbInterval rb;

    long count = 13;
    for ( long key=0 ; key<count ; ++key ) {
      if (key == 5) {
        for ( long j=0 ; j<4 ; ++j ) rb.insert( Interval( l(key), l(key+1) ) );
      }
      rb.insert( Interval( l(key), l(key+1) ) );
    //rb.insert( count-1 - key );

      RbInterval::iterator lb = rb.lower_bound( Interval(l(5),l(6)) );
      RbInterval::iterator ub = rb.upper_bound( Interval(l(5),l(6)) );
      cerr << "range of 5: [ ";
      for ( ; lb != ub ; ++lb ) cerr << *lb << " ";
      cerr << "]" << endl;
    //cerr << "Thickness: " << rb.getThickness() << endl;
      cerr << endl << endl;
    }

    rb.remove( Interval( l(3), l(4) ));
    rb.write( "finalRbTree.gv" );

    for ( Interval intv : rb.getElements() ) {
      cerr << intv << endl;
    }

    return 0;
  }
  
  
// -------------------------------------------------------------------
// Test  :  "testIntervalTree".


  int  testIntervalTree ()
  {
    IntervalTreeString  itree;

    long count = 13;
    for ( long key=0 ; key<count ; ++key ) {
      if (key == 5) {
        for ( long j=0 ; j<4 ; ++j ) itree.insert( IntvString( "b", l(key), l(key+1) ) );
      }
      itree.insert( IntvString( "a", l(key), l(key+1) ) );
    //rb.insert( count-1 - key );

      IntervalTreeString::iterator lb = itree.lower_bound( IntvString("c",l(5),l(6)) );
      IntervalTreeString::iterator ub = itree.upper_bound( IntvString("c",l(5),l(6)) );
      cerr << "range of 5: [ ";
      for ( ; lb != ub ; ++lb ) cerr << *lb << " ";
      cerr << "]" << endl;
      cerr << "Thickness: " << itree.getThickness() << endl;
      cerr << endl << endl;
    }

    itree.remove( IntvString( "a", l(3), l(4) ));
    itree.write( "finalIntervalTree.gv" );

    for ( IntervalData<string> intv : itree.getElements() ) {
      cerr << intv << endl;
    }

    IntervalTreeString::overlap_iterator ob = itree.beginOverlaps( Interval(l(2),l(4)) );
    cerr << "overlap of [2:4]: [ ";
    for ( ; ob != itree.end() ; ++ob )
      cerr << *ob << " ";
    cerr << "]" << endl;
    cerr << endl << endl;

    ob = itree.beginOverlaps( Interval(l(-2),l(-1)) );
    cerr << "overlap of [-2:-1]: [ ";
    for ( ; ob != itree.end() ; ++ob )
      cerr << *ob << " ";
    cerr << "]" << endl;
    cerr << endl << endl;
    
    ob = itree.beginOverlaps( Interval(l(6),l(9)) );
    cerr << "overlap of [6:9]: [ ";
    for ( ; ob != itree.end() ; ++ob )
      cerr << *ob << " ";
    cerr << "]" << endl;
    cerr << endl << endl;

    cerr << "overlap of [6:9]: [ ";
    for ( const IntvString& intv : itree.getOverlaps( Interval( l(6), l(9) ) ) )
      cerr << intv << " ";
    cerr << "]" << endl;

    return 0;
  }

  
}  // Anonymous namespace.
  
  
int main ( int argc, char* argv[] )
{
  int  returnCode = 0;

  try {
    bool coreDump = false;
    bool rbTree   = false;
    bool intvTree = false;

    boptions::options_description options ("Command line arguments & options");
    options.add_options()
      ( "help,h"     , "Print this help." )
      ( "core-dump,D", boptions::bool_switch(&coreDump)->default_value(false)
                     , "Enable core dumping.")
      ( "rb-tree"    , boptions::bool_switch(&rbTree  )->default_value(false)
                     , "Test of the red/black tree \"hurricane/RbTree.h\".")
      ( "intv-tree"  , boptions::bool_switch(&intvTree)->default_value(false)
                     , "Test of the interval tree \"hurricane/IntervalTree.h\".");

    boptions::variables_map arguments;
    boptions::store ( boptions::parse_command_line(argc,argv,options), arguments );
    boptions::notify( arguments );

    if (arguments.count("help")) {
      cerr << options << endl;
      exit( 0 );
    }


    System::get()->setCatchCore( not coreDump ); 
    DebugSession::open( 0, 1000 );

    if (rbTree  ) returnCode += testRbTree();
    if (intvTree) returnCode += testIntervalTree();

    DebugSession::close();
  }
  catch ( Error& e ) {
    cerr << e.what() << endl;
    exit( 127 );
  }
  catch ( boptions::error& e ) {
    cerr << "[ERROR] " << e.what() << endl;
    exit( 127 );
  }
  catch ( exception& e ) {
    cerr << "[ERROR] " << e.what() << endl;
    exit( 127 );
  }
  catch ( ... ) {
    cerr << "[ERROR] Abnormal termination: unmanaged exception.\n" << endl;
    exit( 126 );
  }

  return returnCode;
}
