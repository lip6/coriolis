// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./HFNS.cpp"                               |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DebugSession.h"
#include "hurricane/DataBase.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DeepNet.h"
#include "hurricane/Pin.h"
#include "hurricane/Plug.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Path.h"
#include "hurricane/Library.h"
#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/CellViewer.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/ToolBox.h"
#include "etesian/EtesianEngine.h"


namespace Etesian {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::Warning;
  using Hurricane::Error;
  using Hurricane::Path;
  using Hurricane::Transformation;
  using Hurricane::DataBase;
  using Hurricane::Library;
  using Hurricane::Go;
  using Hurricane::DeepNet;
  using Hurricane::Pin;
  using Hurricane::Plug;
  using Hurricane::RoutingPad;
  using Hurricane::UpdateSession;
  using CRL::AllianceFramework;
  using CRL::CatalogExtension;
  using CRL::getTransformation;
  using CRL::SubNetNames;
  using Etesian::EtesianEngine;


// -------------------------------------------------------------------
// Class  :  "::Cluster".

  class Cluster {
    public:
                             Cluster        ( EtesianEngine* );
      virtual               ~Cluster        ();
      inline  size_t         getSize        () const;
      inline  EtesianEngine* getEtesian     () const;
      virtual Cluster*       getParent      () const;
      virtual SubNetNames*   getSubNetNames ();
      inline  void           setParent      ( Cluster* );
              bool           merge          ( RoutingPad* );
              bool           merge          ( Cluster* );
      inline  void           swapRps        ( Cluster* );
      inline  Net*           getInputNet    ();
      inline  Net*           getOutputNet   ();
              Plug*          raddTransPlug  ( Net* topNet, Path ); 
              Net*           raddTransNet   ( Net* topNet, Path ); 
              Plug*          getPlugByNet   ( Instance* instance, Net* cellNet );
              void           createInput    ( Net* );
              void           createOutput   ();
      virtual void           splitNet       ();
      virtual string         _getTypeName   () const;
      virtual string         _getString     () const;
    private:
      EtesianEngine*      _etesian;
      vector<RoutingPad*> _rps;
      vector<Cluster*>    _clusters;
      Cluster*            _parent;
      Instance*           _buffer;
      Net*                _driverNet;
  };


  Cluster::Cluster ( EtesianEngine* etesian )
    : _etesian  (etesian)
    , _rps      ()
    , _clusters ()
    , _parent   (NULL)
    , _buffer   (NULL)
    , _driverNet(NULL)
  { }


  Cluster::~Cluster ()
  { }


  inline EtesianEngine* Cluster::getEtesian     () const { return _etesian; }
  inline size_t         Cluster::getSize        () const { return _rps.size() + _clusters.size(); }
         Cluster*       Cluster::getParent      () const { return _parent; }
         SubNetNames*   Cluster::getSubNetNames () { return _parent->getSubNetNames(); }
  inline void           Cluster::setParent      ( Cluster* parent ) { _parent = parent; }
  inline void           Cluster::swapRps        ( Cluster* other ) { _rps.swap( other->_rps ); }


  bool  Cluster::merge ( RoutingPad* newRp )
  {
    for ( RoutingPad* rp : _rps ) {
      if (rp == newRp) return false;
    }
    _rps.push_back( newRp );
    return true;
  }


  bool  Cluster::merge ( Cluster* child )
  {
    for ( Cluster* cluster : _clusters ) {
      if (cluster == child) return false;
    }
    _clusters.push_back( child );
    child->setParent( this );
    return true;
  }


  Plug* Cluster::getPlugByNet ( Instance* instance, Net* cellNet )
  {
    for ( Plug* plug : instance->getPlugs() ) {
      if (plug->getNet() == cellNet)
        return plug;
    }
    return NULL;
  }
  

  Plug* Cluster::raddTransPlug ( Net* topNet, Path path )
  {
    if (path.isEmpty()) return NULL;
    if (topNet->getCell() != path.getOwnerCell() ) {
      throw Error( "Cluster::raddTransPlug(): \"topNet\" and \"path\" must belong to the same cell.\n"
                   "        * \"topNet\" is owned by %s\n"
                   "        * \"path\" is owned by %s"
                 , getString(topNet->getCell()).c_str()
                 , getString(path.getOwnerCell()).c_str()
                 );
    }

    Path      tailPath     = path.getTailPath();
    Instance* headInstance = path.getHeadInstance();
    Plug*     headPlug     = getPlugByNet( headInstance, topNet );
    Net*      masterNet    = NULL;

    if (not headPlug) {
      Cell* masterCell = headInstance->getMasterCell();
      masterNet = masterCell->getNet( topNet->getName() );
      if (not masterNet) {
        masterNet = Net::create( masterCell, topNet->getName() );
        masterNet->setType     ( topNet->getType() );
        masterNet->setDirection( Net::Direction::IN );
      }

      masterNet->setExternal( true );
      headPlug = headInstance->getPlug( masterNet );
      if (not headPlug)
        throw Error( "Cluster::raddTransPlug(): Plug not created for \"%s\" on instance \"%s\" of \"%s\""
                   , getString(topNet->getName()).c_str()
                   , getString(headInstance->getName()).c_str()
                   , getString(masterCell->getName()).c_str() );
      headPlug->setNet( topNet );
    } else {
      masterNet = headPlug->getMasterNet();
    }

    if (not tailPath.isEmpty())
      headPlug = raddTransPlug( masterNet, tailPath );

    return headPlug;
  }
  

  Net* Cluster::raddTransNet ( Net* topNet, Path path )
  {
    if (path.isEmpty()) return topNet;
    return raddTransPlug( topNet, path )->getMasterNet();
  }


  void  Cluster::createInput ( Net* upDriver )
  {
    Cell*        topCell     = _etesian->getCell();
    Cell*        cellPnR     = _etesian->getBlockCell();
    Instance*    instancePnR = _etesian->getBlockInstance();
    BufferDatas* bufferDatas = _etesian->getBufferCells().getBiggestBuffer();
    Net*         blockNet    = upDriver;
    Path         inputPath   = Path();

    if (topCell != cellPnR) {
      inputPath = Path( instancePnR );
      blockNet = raddTransNet( upDriver, inputPath );
    }
    Plug* inputPlug = bufferDatas->getInput( _buffer );
    inputPlug->setNet( blockNet );
    RoutingPad::create( upDriver, Occurrence(inputPlug,inputPath), RoutingPad::BiggestArea );
  }


  void  Cluster::createOutput ()
  {
    Cell*        topCell     = _etesian->getCell();
    Cell*        cellPnR     = _etesian->getBlockCell();
    Instance*    instancePnR = _etesian->getBlockInstance();
    BufferDatas* bufferDatas = _etesian->getBufferCells().getBiggestBuffer();
    string       driverName  = getSubNetNames()->getSubNetName();
    Net*         blockNet    = NULL;
    Path         outputPath  = Path();

    _buffer = Instance::create( cellPnR, driverName, bufferDatas->getCell() );

    getSubNetNames()->nextSubNet();
    _driverNet = Net::create( topCell, driverName );
    if (topCell == cellPnR) blockNet = _driverNet;
    else {
      outputPath = Path( instancePnR );
      blockNet = raddTransNet( _driverNet, outputPath );
    }
    Plug* outputPlug = bufferDatas->getOutput( _buffer );
    outputPlug->setNet( blockNet );
    RoutingPad::create( _driverNet, Occurrence(outputPlug,outputPath), RoutingPad::BiggestArea );
  }


  void  Cluster::splitNet ()
  {
    createOutput();
    for ( Cluster* cluster : _clusters ) {
      cluster->createInput( _driverNet );
    }
    for ( RoutingPad* rp : _rps ) {
      Occurrence  plugOcc = rp->getPlugOccurrence();
      Net*        deepNet = raddTransNet( _driverNet, plugOcc.getPath() );
      if (dynamic_cast<Pin*>(plugOcc.getEntity())) {
        continue;
      }
      Plug* deepSinkPlug = dynamic_cast<Plug*>( plugOcc.getEntity() );
      deepSinkPlug->setNet( deepNet );
      rp->destroy();
      RoutingPad::create( _driverNet, plugOcc, RoutingPad::BiggestArea );
    }
  }

  
  string  Cluster::_getTypeName () const
  { return "Cluster"; }

    
  string  Cluster::_getString () const
  {
    string s = "<" + _getTypeName() + " ";
    s += getString(_rps.size()) + "+" + getString(_clusters.size()) + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "::BufferTree".

  class BufferTree : public Cluster {
    public:
                           BufferTree     ( EtesianEngine*, Net* );
      virtual             ~BufferTree     ();
      virtual Cluster*     getParent      () const;
      virtual SubNetNames* getSubNetNames ();
      virtual void         splitNet       ();
              void         rpartition     ();
              uint32_t     build          ();
              string       _getTypeName   () const;
    private:
      SubNetNames  _subNetNames;
      bool         _isDeepNet;
      Net*         _rootNet;
      RoutingPad*  _rpDriver;
      vector< vector<Cluster*> >  _clustersStack;
  };


  BufferTree::BufferTree ( EtesianEngine* etesian, Net* rootNet )
    : Cluster(etesian)
    , _subNetNames  ()
    , _isDeepNet    (true)
    , _rootNet      (rootNet)
    , _rpDriver     (NULL)
    , _clustersStack()
  {
    _subNetNames.match( getString(rootNet->getName()) );
    cdebug_log(123,0) << "BufferTree CTOR _clustersStack.size()=" << _clustersStack.size() << endl;
  }

  
  BufferTree::~BufferTree ()
  {
    for ( vector<Cluster*>& clusters : _clustersStack ) {
      for ( Cluster* cluster : clusters ) {
        if (dynamic_cast<BufferTree*>(cluster)) continue;
        delete cluster;
      }
    }
  }

  
  Cluster*     BufferTree::getParent      () const { return NULL; }
  SubNetNames* BufferTree::getSubNetNames () { return &_subNetNames; }


  void  BufferTree::splitNet ()
  {
    if (not _rpDriver)
      throw Error( "BufferTree::splitNet(): Missing driver on %s.", getString(_rootNet).c_str() );

    Cluster::splitNet();
    if (_isDeepNet) {
      Cell*      topCell     = getEtesian()->getCell();
      Name       topNetName  = _rootNet->getName();
      Occurrence driverRpOcc = _rpDriver->getPlugOccurrence();
      _rootNet->destroy();
      _rootNet = Net::create( topCell, topNetName );
      Net* deepDriverNet = raddTransNet( _rootNet, driverRpOcc.getPath() );
      dynamic_cast<Plug*>( driverRpOcc.getEntity() )->setNet( deepDriverNet );
      RoutingPad::create( _rootNet, driverRpOcc, RoutingPad::BiggestArea );
    }
    createInput( _rootNet );
  }


  void  BufferTree::rpartition ()
  {
    cdebug_log(123,1) << "BufferTree::rpartition()" << endl;
    _clustersStack.push_back( vector<Cluster*>() );
    cdebug_log(123,0) << "_clustersStack.size()=" << _clustersStack.size() << endl;
    _clustersStack.back().push_back( new Cluster(getEtesian()) );
    cdebug_log(123,0) << "_clustersStack[0].size()=" << _clustersStack.back().size() << endl;

    BufferDatas* bufferDatas = getEtesian()->getBufferCells().getBiggestBuffer();
    RoutingPad* rpPin = NULL;
    for ( RoutingPad* rp : _rootNet->getRoutingPads() ) {
      Occurrence rpOccurrence = rp->getPlugOccurrence();
      if (rpOccurrence.getPath().isEmpty())
        _isDeepNet = false;
      Pin* pin = dynamic_cast<Pin* >( rpOccurrence.getEntity() );
      if (pin) {
        if (not rpPin) {
          if (dynamic_cast<Pin* >( rpOccurrence.getEntity() )) {
            rpPin = rp;
          }
        }
        cdebug_log(123,0) << "Excluded: " << pin << endl;
        continue;
      }
      Plug* rpPlug = dynamic_cast<Plug*>( rpOccurrence.getEntity() );
      Net* masterNet = rpPlug->getMasterNet();
      if (masterNet->getDirection() & Net::Direction::DirIn) {
        if(_clustersStack[0].back()->getSize() >= bufferDatas->getMaxSinks()) {
          _clustersStack[0].push_back( new Cluster(getEtesian()) );
          cdebug_log(123,0) << "_clustersStack[0].size()=" << _clustersStack[0].size() << endl;
        }
        cdebug_log(123,0) << "merge: " << _clustersStack[0].back()->getSize() << " " << rp << endl;
        _clustersStack[0].back()->merge( rp );
      } else {
        _rpDriver = rp;
      }
    }

    if (rpPin) {
      if (not _rpDriver) {
        _rpDriver = rpPin;
      } else {
        _clustersStack[0].back()->merge( rpPin );
      }
    }

    if (_clustersStack[0].size() == 1) {
      _clustersStack[0].back()->swapRps( this );
      _clustersStack[0].clear();
      _clustersStack[0].push_back( this );
      cdebug_log(123,0) << "One cluster special case." << endl;
      cdebug_tabw(123,-1);
      return;
    }
    
    size_t depth = 0;
    while ( _clustersStack[depth].size() > 1) {
      cdebug_log(123,0) << "New depth " << (depth+1) << endl;
      _clustersStack.push_back( vector<Cluster*>() );
      if (_clustersStack[depth].size() <= bufferDatas->getMaxSinks()) {
        _clustersStack[depth+1].push_back( this );
        cdebug_log(123,0) << "TOP cluster" << endl;
      }
      else
        _clustersStack[depth+1].push_back( new Cluster(getEtesian()) );
      cdebug_log(123,0) << "CLUSTER _clustersStack[" << depth << "].size()=" << _clustersStack[depth].size() << endl;
      for ( Cluster* cluster : _clustersStack[depth] ) {
        if(_clustersStack[depth+1].back()->getSize() >= bufferDatas->getMaxSinks()) {
          _clustersStack[depth+1].push_back( new Cluster(getEtesian()) );
          cdebug_log(123,0) << "_clustersStack[" << (depth+1) << "].size()=" << _clustersStack[depth+1].size() << endl;
        }
        cdebug_log(123,0) << "merge: " << _clustersStack[depth+1].back()->getSize() << " " << cluster << endl;
        _clustersStack[depth+1].back()->merge( cluster );
      }
      ++depth;
    }

    cdebug_tabw(123,-1);
  }


  uint32_t  BufferTree::build ()
  {
    uint32_t bufferCount = 0;
    rpartition();
    for ( vector<Cluster*>& clusters : _clustersStack ) {
      for ( Cluster* cluster : clusters ) {
        cluster->splitNet();
        ++bufferCount;
      }
    }
    return bufferCount;
  }

  
  string  BufferTree::_getTypeName () const
  { return "BufferTree"; }


  uint32_t  EtesianEngine::doHFNS ()
  {
    cmess2 << "     - High Fanout Net Synthesis (HFNS)." << endl;
    startMeasures();

    BufferDatas* bufferDatas = getBufferCells().getBiggestBuffer();
    vector< tuple<Net*,uint32_t> > netDatas;
    for ( Net* net : getCell()->getNets() ) {
      uint32_t rpCount = 0;
      for ( RoutingPad* rp : net->getRoutingPads() ) {
        Occurrence rpOcc = rp->getPlugOccurrence();
        Pin*       pin   = dynamic_cast<Pin*>( rpOcc.getEntity() ); 
        if (pin) {
          continue;
        }

        if (getBlockInstance()) {
          if (rpOcc.getPath().getHeadInstance() != getBlockInstance()) {
            continue;
          }
        }

        Plug* rpPlug    = dynamic_cast<Plug*>( rpOcc.getEntity() );
        Net*  masterNet = rpPlug->getMasterNet();
        if (masterNet->getDirection() & Net::Direction::DirIn) {
          ++rpCount;
        }
      }

      if (rpCount > bufferDatas->getMaxSinks()) {
        netDatas.push_back( make_tuple(net,rpCount) );
      }
    }

    UpdateSession::open();
    Go::disableAutoMaterialization();
    _bufferCount = 0;
    for ( size_t i=0 ; i<netDatas.size() ; ++i ) {
      if (i) {
        if ((i%10) == 0) cmess2 << "\n       ";
        else             cmess2 << " ";
      } else {
        cmess2 << "       ";
      }
      cmess2 << "[" << std::get<1>( netDatas[i] ) << "]";
      Net* net = std::get<0>( netDatas[i] );
      BufferTree tree ( this, net );
      _bufferCount += tree.build();
    }
    cmess2 << endl;
    // for ( tuple<Net*,uint32_t>& netData : netDatas ) {
    //   Net* net = std::get<0>(netData);
    //   cmess2 << "       Net \"" << net->getName() << "\" has " << std::get<1>(netData) << " sinks." << endl;
    //   BufferTree tree ( this, net );
    //   _bufferCount += tree.build();
    // }
    Go::enableAutoMaterialization();
    UpdateSession::close();

    stopMeasures();
    printMeasures();
    cmess2 << "     - Total added buffers " << _bufferCount << endl;
    return _bufferCount;
  }
  

}  // Etesian namespace.


GETSTRING_POINTER_SUPPORT(Etesian::Cluster);
GETSTRING_POINTER_SUPPORT(Etesian::BufferTree);
