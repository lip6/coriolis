
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//   Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//   Hugo Clément                   <Hugo.Clement@lip6.fr>
//   Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//   Christian Masson           <Christian.Masson@lip6.fr>
// 
// The Coriolis Project  is free software;  you can  redistribute it and/or
// modify  it  under the  terms  of  the  GNU  General  Public License  as
// published by  the Free  Software Foundation; either  version 2  of  the
// License, or (at your option) any later version.
// 
// The Coriolis Project  is distributed in the hope that it will be useful,
// but  WITHOUT  ANY  WARRANTY;  without  even  the  implied  warranty  of
// MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
// General Public License for more details.
// 
// You should have received a copy  of  the  GNU  General  Public  License
// along with  the Coriolis Project;  if  not,  write to the  Free Software
// Foundation, inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
// Date   : 29/01/2004
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 


#include <cmath>
using namespace std;

#include "hurricane/Warning.h"
#include "hurricane/Net.h"
#include "hurricane/Instance.h"
#include "hurricane/Plug.h"
#include "hurricane/Timer.h"
#include "crlcore/ToolBox.h"
#include "crlcore/AllianceFramework.h"
using namespace CRL;

#include "nimbus/NimbusEngine.h"
using namespace Nimbus;

#ifdef HAVE_HMETIS_LIB
#include "metis/MetisGraph.h"
#endif
#include "metis/MetisEngine.h"


namespace Metis {


  const Name MetisEngine::_toolName = "Metis";

    
  MetisEngine::MetisEngine ( Cell* cell )
    : Inherit       (cell)
    , _configuration(Configuration::getDefault()->clone())
    , _step         (0)
    , _actualGraphs ()
    , _newGraphs    ()
    , _globalEdgeCut(0)
  {
#ifdef HAVE_HMETIS_LIB

    NimbusEngine* nimbus = NimbusEngine::get ( getCell() );
    if ( nimbus == NULL )
      throw Error ("Nimbus must be created before Metis, on cell <%s>"
                  ,getString(getCell()->getName()).c_str());

    _actualGraphs = new MetisGraphs();
    _newGraphs    = new MetisGraphs();
    _actualGraphs->push_back ( new MetisGraph(this,nimbus->getGrid()->getRoot()) );

    // What the fuck does this loop do?
    // forEach ( Net*, inet, getCell()->getNets() ) {
    //   if ( inet->isGlobal() ) continue;
    //   if ( inet->isSupply() ) continue;
    // }
#endif // HAVE_HMETIS_LIB
  }

    
  MetisEngine::~MetisEngine ()
  { delete _configuration; }


  MetisEngine* MetisEngine::create ( Cell* cell )
  {
    MetisEngine* metis = new MetisEngine(cell);

    metis->_postCreate();
    return metis;
  }


  void MetisEngine::_preDestroy ()
  {
#ifdef HAVE_HMETIS_LIB
    for (MetisGraphs::iterator mgit = _actualGraphs->begin(); mgit != _actualGraphs->end(); ++mgit)
      delete *mgit;
    for (MetisGraphs::iterator mgit = _newGraphs->begin(); mgit != _newGraphs->end(); ++mgit)
      delete *mgit;

    delete _actualGraphs;
    delete _newGraphs;
#endif // HAVE_HMETIS_LIB
    Inherit::_preDestroy();
  }


  bool MetisEngine::isHMetisCapable ()
  {
#ifdef HAVE_HMETIS_LIB
    return true;
#else
    return false;
#endif // HAVE_HMETIS_LIB
  }


  Record* MetisEngine::_getRecord () const
  {
    Record* record = Inherit::_getRecord();
    if (record) {
      record->add ( getSlot("step", _step) );
    }
    return record;
  }


  void MetisEngine::run ()
  {
#ifdef HAVE_HMETIS_LIB
    cmess2 << "     o  hMetis quadri-partition step." << endl;

    Timer timer;
    timer.start();

    NimbusEngine* nimbus = NimbusEngine::get ( getCell() );

    forEach ( GCell*, igcell, nimbus->getPlacementLeaves() ) {
      if ( not igcell->hasSubGCells() )
        throw Warning("You must first progress in Nimbus before calling Metis");

      if ( igcell->getSubGCells().getSize() < 2 )
        throw Error("Metis needs at least 2 Nimbus subboxes");
    }

    _globalEdgeCut = 0;
    
    linefill  output ("        ",cmess2);

    for ( MetisGraphs::iterator mgit = _actualGraphs->begin(); mgit != _actualGraphs->end(); ++mgit ) {
      MetisGraph* graph = *mgit;

      try {
        _globalEdgeCut += graph->part ( output );
      } 
      catch ( MetisGraph::TooLowNVTXSException& e ) {
        cerr << Warning("Impossible to part graph, only %d nodes",e._nvtxs);
      }
        
      for ( size_t id=0; id < graph->_partResultVector.size(); ++id ) {
        _newGraphs->push_back
          ( new MetisGraph(this
                          ,graph
                          ,graph->_partResultVector[id]->first
                          ,graph->_partResultVector[id]->second
                          ) );
      }
    }

    output << endl;
    
    for ( MetisGraphs::iterator mgit=_actualGraphs->begin(); mgit != _actualGraphs->end(); ++mgit )
      delete *mgit;
    delete _actualGraphs;

    _actualGraphs = _newGraphs;
    _newGraphs    = new MetisGraphs ();
    
    save ( ++_step );
       
    timer.stop();    
    cmess2 << "        - Refine placement done in " << getString(timer.getUserTime()) << "s." << endl;

    if ( getRefreshCb() != NULL ) getRefreshCb() ();
#else
    throw Warning ( "hmetis library is needed to use Metis ToolEngine" );
#endif  // HAVE_HMETIS_LIB
  }


  void MetisEngine::save ( unsigned step )
  {
#ifdef HAVE_HMETIS_LIB
  //for ( MetisGraphs::iterator mgit=_actualGraphs->begin(); mgit != _actualGraphs->end(); ++mgit)
  //  (*mgit)->save ( step );
    grabPlacementModificationFlag ();
#else
    throw Warning ( "hmetis library is needed to use Metis ToolEngine" );
#endif /* HAVE_HMETIS_LIB */
  }


  int MetisEngine::getGlobalEdgeCut () const
  { return _globalEdgeCut; }


  const Name& MetisEngine::staticGetName ()
  { return _toolName; }


  const Name& MetisEngine::getName () const
  { return _toolName; }


  MetisEngine* MetisEngine::get ( Cell* cell )
  { return dynamic_cast<MetisEngine*> ( ToolEngine::get(cell,_toolName) ); }


  bool MetisEngine::_reInit()
  {
#if 0
#ifdef HAVE_HMETIS_LIB
    for (MetisGraphs::iterator mgit=_actualGraphs->begin(); mgit != _actualGraphs->end(); ++mgit)
      delete *mgit;
    for (MetisGraphs::iterator mgit=_newGraphs->begin(); mgit != _newGraphs->end(); ++mgit)
      delete *mgit;

    delete _actualGraphs;
    delete _newGraphs;

    _actualGraphs = new MetisGraphs();
    _newGraphs    = new MetisGraphs();

    NimbusEngine* nimbus       = NimbusEngine::get ( getCell() );
    bool          partToBeDone = false;

    forEach ( GCell*, igcell, nimbus->getPlacementLeaves() ) {
      Instances instances = getCell()->getInstancesUnder(**igcell);

      if ( instances.getSize() >= getNumberOfInstancesStopCriterion() ) {
        if ( not partToBeDone ) {
          partToBeDone = true;
          ++_step;
        }
        _actualGraphs->push_back ( new MetisGraph(this,instances,igcell) );
      }
    }

    if ( not partToBeDone )
      throw Warning("Not enough instances to part, minimum is %d",getNumberOfInstancesStopCriterion());

    return true;
#else
    throw Warning ( "hmetis library is needed to use Metis ToolEngine" );
#endif  // HAVE_HMETIS_LIB
#else
    return false;
#endif
}


  unsigned int  MetisEngine::computeQuadriPartitions ( Cell* cell )
  {
    size_t gates = getInstancesCount ( cell );

    double partitions = log((double)gates / (double)Configuration::getDefault()
                           ->getNumberOfInstancesStopCriterion() ) / log(4.0) + 1.0;

    return (unsigned int)(partitions);
  }


  void  MetisEngine::doQuadriPart ( Cell* cell )
  {
    if ( not isHMetisCapable() )
      throw Error ("Metis::doQuadriPart(): HMetis wasn't found at compile time <%s>"
                  ,getString(cell->getName()).c_str());

    // MetisEngine* metis = MetisEngine::get ( cell );
    // if ( metis != NULL )
    //   throw Error ("Metis::doQuadriPart(): Metis already exists on <%s>"
    //               ,getString(cell->getName()).c_str());

    // NimbusEngine* nimbus = NimbusEngine::get ( cell );
    // if ( nimbus != NULL )
    //   throw Error ("Metis::doQuadriPart(): Nimbus already exists on <%s>"
    //               ,getString(cell->getName()).c_str());

    // nimbus = NimbusEngine::create ( cell, AllianceFramework::get()->getLibrary(1) );
    // metis  = MetisEngine ::create ( cell );

    NimbusEngine* nimbus = NimbusEngine::get ( cell );
    if ( nimbus == NULL )
      nimbus = NimbusEngine::create ( cell );

    MetisEngine* metis = MetisEngine::get ( cell );
    if ( metis == NULL )
      metis  = MetisEngine ::create ( cell );

    size_t partitions = computeQuadriPartitions ( cell );
    for ( size_t part=0 ; part<partitions ; ++part ) {
      nimbus->progress ();
      metis-> run ();
    }
  }


} // End of Metis namespace.
