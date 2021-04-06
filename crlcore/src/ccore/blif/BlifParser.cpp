// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        Yosys & Blif / Hurricane  Interface                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./blif/BlifParser.cpp"                        |
// +-----------------------------------------------------------------+


#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/Warning.h"
#include "hurricane/Plug.h"
#include "hurricane/Net.h"
#include "hurricane/Instance.h"
#include "hurricane/Cell.h"
#include "hurricane/Library.h"
#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/Catalog.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/NetExtension.h"
#include "crlcore/ToolBox.h"
#include "crlcore/Blif.h"
using namespace CRL;


namespace {

  using namespace std;
  using CRL::NamingScheme;


//inline bool  isAbcAutomaticName ( string name )
//{ return (name.substr(0,5) == "$abc$"); }


// -------------------------------------------------------------------
// Class  :  "::Tokenize".


  class Tokenize {
    public:
      enum State { Init       = 0x00000001
                 , Unknown    = 0x00000002
                 , Model      = 0x00000004
                 , End        = 0x00000008
                 , Subckt     = 0x00000010
                 , Gate       = 0x00000020
                 , MLatch     = 0x00000040
                 , Latch      = 0x00000080
                 , Inputs     = 0x00000100
                 , Outputs    = 0x00000200
                 , Clock      = 0x00000400
                 , Names      = 0x00000800
                 , CoverZero  = 0x00001000
                 , CoverOne   = 0x00002000
                 , CoverLogic = 0x00004000
                 , CoverAlias = 0x00008000
                 };
      typedef  vector< pair<string,string> >  CoverTable;
    public:
                                   Tokenize   ( string blifFile );
      inline size_t                lineno     () const;
      inline unsigned int          state      () const;
             const vector<string>& blifLine   () const;
             const CoverTable&     coverTable () const;
             bool                  readEntry  ();
    private:                                  
             bool                  _readline  ();
    private:
      size_t          _lineno;
      unsigned int    _state;
      ifstream        _stream;
      vector<string>  _tokens;
      vector<string>  _blifLine;
      CoverTable      _coverTable;
  };


  Tokenize::Tokenize ( string blifFile )
    : _lineno    (0)
    , _state     (Init)
    , _stream    ()
    , _tokens    ()
    , _blifLine  ()
    , _coverTable()
  { 
    _stream.open( blifFile+".blif" );
    if (_stream.fail())
      throw Error( "Unable to open BLIF file %s.blif\n", blifFile.c_str() );

    _readline();
  }


  inline size_t                      Tokenize::lineno    () const { return (_lineno) ? _lineno-1 : 0; }
  inline unsigned int                Tokenize::state     () const { return _state; }
  inline const vector<string>&       Tokenize::blifLine  () const { return _blifLine; }
  inline const Tokenize::CoverTable& Tokenize::coverTable() const { return _coverTable; }


  bool  Tokenize::readEntry ()
  {
    _blifLine  .clear();
    _coverTable.clear();
    _blifLine = _tokens;
    _state    = 0;

    if (_stream.eof()) return false;

    if (_tokens.front() == ".model"  ) { _state = Model;   }
    if (_tokens.front() == ".end"    ) { _state = End;     }
    if (_tokens.front() == ".inputs" ) { _state = Inputs;  }
    if (_tokens.front() == ".outputs") { _state = Outputs; }
    if (_tokens.front() == ".clock"  ) { _state = Clock;   }
    if (_tokens.front() == ".subckt" ) { _state = Subckt;  }
    if (_tokens.front() == ".gate"   ) { _state = Gate;    }
    if (_tokens.front() == ".latch"  ) { _state = Latch;   }
    if (_tokens.front() == ".mlatch" ) { _state = MLatch;  }
    if (_tokens.front() == ".names"  ) {
      _state = Names;

      while ( _readline() and (_tokens.front()[0] != '.')) {
        switch ( _tokens.size() ) {
          case 0: break;
          case 1: _coverTable.push_back( make_pair(_tokens[0],string()) ); break;
          default: 
          case 2: _coverTable.push_back( make_pair(_tokens[0],_tokens[1]) ); break;
        }
      }

      if      (_coverTable.empty()) _state |= CoverZero;
      else if (_coverTable.size () == 1) {
        if      ( (_coverTable[0].first == "1") and (_coverTable[0].second.empty()) ) _state |= CoverOne;
        else if ( (_coverTable[0].first == "1") and (_coverTable[0].second == "1")  ) _state |= CoverAlias;
      } else {
        _state |= CoverLogic;
      }
    } else
      _readline();

    return true;
  }


  bool  Tokenize::_readline ()
  {
    _tokens.clear();

    bool nextLine = true;

    while ( nextLine ) {
      if (_stream.eof()) return false;

      nextLine = false;
      ++_lineno;

      string line;
      getline( _stream, line );

      bool   comment  = false;
      size_t tokstart = 0;
      for ( size_t i=0 ; i<line.size() ; ++i ) {
        size_t nextTokstart = i+1;
  
        switch ( line[i] ) {
          case '\\':
            if (i == line.size()-1) nextLine = true;
          default:   continue;
          case ' ':
          case '\t': break;
          case '#':  comment  = true; break;
        }
  
        if (i > tokstart)
          _tokens.push_back( line.substr(tokstart,i-tokstart) );
  
        tokstart = nextTokstart;

        if (comment) break;
      }

      if ( (not comment) and (tokstart < line.size()) )
        _tokens.push_back( line.substr(tokstart) );

      if (_tokens.empty())
        nextLine = true;
    }

    return not _tokens.empty();
  }


// -------------------------------------------------------------------
// Class  :  "::Subckt" (declaration).


  class Model;


  class Subckt {
    public:
      typedef  vector< pair<string,string> >  Connections;
    public:
                                Subckt          ( string modelName, string instanceName );
      static Model*             createModel     ( string modelName );
      inline string             getModelName    () const;
      inline string             getInstanceName () const;
      inline const Connections& getConnections  () const;
      inline size_t             getDepth        () const;
      inline Model*             getModel        () const;
      inline void               setModel        ( Model* );
      inline void               addConnection   ( const pair<string,string>& );
             void               connectSubckts  ();
    private:
      string       _modelName;
      string       _instanceName;
      Connections  _connections;
      Model*       _model;
  };

  typedef  vector<Subckt*>  Subckts;


// -------------------------------------------------------------------
// Class  :  "::Model" (declaration).


  class Model {
    public:
      typedef unordered_map<string,Model*>  Lut; 
      static  Lut                           _blifLut; 
      static  vector<Model*>                _blifOrder; 
      static  bool                          _staticInit;
      static  string                        _groundName;
      static  string                        _powerName;
      static  Cell*                         _zeroCell;
      static  Cell*                         _oneCell;
      static  Net*                          _masterNetZero;
      static  Net*                          _masterNetOne;
    public:
      static  void          staticInit     ();
      static  string        getGroundName  ();
      static  string        getPowerName   ();
      static  Model*        find           ( string modelName );
      static  void          orderModels    ();
      static  void          connectModels  ();
      static  void          toVhdlModels   ();
      static  void          clearStatic    ();
      static  const Lut&    getLut         ();
    public:                                
                            Model          ( Cell* );
      inline               ~Model          ();
      inline Cell*          getCell        () const;
             Net*           newOne         ();
             Net*           newZero        ();
      inline size_t         getDepth       () const;
      inline const Subckts& getSubckts     () const;
             Subckt*        addSubckt      ( string modelName );
             size_t         computeDepth   ();
             void           connectSubckts ();
             Net*           mergeNet       ( string name, bool isExternal, unsigned int );
             Net*           mergeAlias     ( string name1, string name2 );
    private:
      Cell*     _cell;
      Subckts   _subckts;
      size_t    _depth;
      size_t    _supplyCount;
      Instance* _oneInstance;
      Instance* _zeroInstance;
  };


// -------------------------------------------------------------------
// Class  :  "::Subckt" (implementation).


  Model*  Subckt::createModel ( string modelName )
  {
    Cell*  cell  = NULL;
    Model* model = NULL;
    if (Blif::getLibraries().empty()) {
      AllianceFramework* af = AllianceFramework::get();
      if (af->isInCatalog(modelName)) {
        model = Model::find( modelName );
        if (not model) {
          model = new Model ( af->getCell( modelName, Catalog::State::Views, 0 ) );
        }
      }
    } else {
      for ( Library* library : Blif::getLibraries() ) {
        cell = library->getCell( modelName );
        if (cell) {
          model = new Model ( cell );
          break;
        }
      }
    }

    return model;
  }


  Subckt::Subckt ( string modelName, string instanceName )
    : _modelName   (modelName)
    , _instanceName(instanceName)
    , _connections ()
    , _model       (createModel(modelName))
  { }


  inline Model*     Subckt::getModel        () const { return _model; }
  inline string     Subckt::getModelName    () const { return _modelName; }
  inline string     Subckt::getInstanceName () const { return _instanceName; }
  inline const Subckt::Connections&
                    Subckt::getConnections  () const { return _connections; }
  inline size_t     Subckt::getDepth        () const { return (_model) ? _model->getDepth() : 0; }
  inline void       Subckt::setModel        ( Model* model ) { _model = model; }
  inline void       Subckt::addConnection   ( const pair<string,string>& connection ) { _connections.push_back(connection); }


// -------------------------------------------------------------------
// Class  :  "::Model" (implementation).


  Model::Lut      Model::_blifLut;
  vector<Model*>  Model::_blifOrder;
  bool            Model::_staticInit     = false;
  string          Model::_groundName     = "vss";
  string          Model::_powerName      = "vdd";
  Cell*           Model::_zeroCell       = NULL;
  Cell*           Model::_oneCell        = NULL;
  Net*            Model::_masterNetZero  = NULL;
  Net*            Model::_masterNetOne   = NULL;


  struct CompareByDepth {
      inline bool  operator() ( const Model* lhs, const Model* rhs )
      {
        if (lhs->getDepth() != rhs->getDepth()) return lhs->getDepth() < rhs->getDepth();
        return lhs->getCell()->getId() < rhs->getCell()->getId();
      }
  };


  void  Model::staticInit ()
  {
    _staticInit = true;

    auto framework = AllianceFramework::get();

    static string zeroName = Cfg::getParamString("etesian.cell.zero","zero_x0")->asString();
    static string  oneName = Cfg::getParamString("etesian.cell.one" , "one_x0")->asString();

    _zeroCell     = framework->getCell( zeroName, Catalog::State::Views|Catalog::State::Foreign );
    _oneCell      = framework->getCell(  oneName, Catalog::State::Views|Catalog::State::Foreign );
    _groundName   = Cfg::getParamString("crlcore.groundName","vss")->asString();
    _powerName    = Cfg::getParamString("crlcore.powerName" ,"vdd")->asString();

    if (_zeroCell) {
      for ( Net* net : _zeroCell->getNets() ) {
        if (   not net->isSupply   ()
           and not net->isAutomatic()
           and not net->isBlockage () ) {
          if (getString(net->getName()).find(_powerName) != string::npos) {
            cerr << Error( "BlifParser::Model::staticInit(): Output \"%s\" of zero (tie low) cell \"%s\" match power supply name."
                         , getString(net->getName()).c_str(), zeroName.c_str() ) << endl;
          }
          if (getString(net->getName()).find(_groundName) != string::npos) {
            cerr << Error( "BlifParser::Model::staticInit(): Output \"%s\" of zero (tie low) cell \"%s\" match ground name."
                         , getString(net->getName()).c_str(), zeroName.c_str() ) << endl;
          }
          _masterNetZero = net;
          break;
        }
      }
    } else
      cerr << Warning( "BlifParser::Model::staticInit(): The zero (tie low) cell \"%s\" has not been found."
                     , zeroName.c_str() ) << endl;

    if (_oneCell) {
      for ( Net* net : _oneCell->getNets() )
        if (   not net->isSupply   ()
           and not net->isAutomatic()
           and not net->isBlockage () ) {
          if (getString(net->getName()).find(_powerName) != string::npos) {
            cerr << Error( "BlifParser::Model::staticInit(): Output \"%s\" of one (tie high) cell \"%s\" match power supply name."
                         , getString(net->getName()).c_str(), zeroName.c_str() ) << endl;
          }
          if (getString(net->getName()).find(_groundName) != string::npos) {
            cerr << Error( "BlifParser::Model::staticInit(): Output \"%s\" of one (tie high) cell \"%s\" match ground name."
                         , getString(net->getName()).c_str(), zeroName.c_str() ) << endl;
          }
          _masterNetOne = net;
          break;
        }
    } else
      cerr << Warning( "BlifParser::Model::staticInit(): The one (tie high) cell \"%s\" has not been found."
                     , oneName.c_str() ) << endl;
  }


  string  Model::getGroundName () { return _groundName; }
  string  Model::getPowerName  () { return _powerName; }


  Model* Model::find ( string modelName )
  {
    Lut::iterator ibcell = _blifLut.find( modelName );
    if (ibcell == _blifLut.end()) return NULL;

    return ibcell->second;
  }


  void  Model::orderModels ()
  {
    for ( auto element : _blifLut ) {
      element.second->computeDepth();
      _blifOrder.push_back( element.second );
    }
    sort( _blifOrder.begin(), _blifOrder.end(), CompareByDepth() );
  }


  void  Model::connectModels ()
  {
    for ( Model* blifModel : _blifOrder ){
    //cmess2 << "Handling model <" << blifModel->getCell()->getName() << ">" << endl;
      blifModel->connectSubckts();
    }
  }


  void  Model::toVhdlModels ()
  {
    for ( Model* model : _blifOrder )
      CRL::NamingScheme::toVhdl( model->getCell()
                               , CRL::NamingScheme::Recursive
                               | CRL::NamingScheme::FromVerilog
                               | CRL::NamingScheme::NoLowerCase );
  }


  void  Model::clearStatic ()
  {
    for ( auto ibcell : _blifLut ) delete ibcell.second;
    _blifLut.clear();
    _blifOrder.clear();
  }


  Model::Model ( Cell* cell )
    : _cell        (cell)
    , _subckts     ()
    , _depth       (0)
    , _supplyCount (0)
    , _oneInstance (NULL)
    , _zeroInstance(NULL)
  {
    if (not _staticInit) staticInit();
    
    _blifLut.insert( make_pair(getString(_cell->getName()), this) );
    if (_cell->isTerminalNetlist())
      _depth = 1;
    else {
      cmess2 << "     " << tab++ << "+ " << cell->getName() << " [.model]" << endl;

      if (not cell->getNet(_groundName)) {
        Net* vss = Net::create ( _cell, _groundName );
        vss->setExternal( true );
        vss->setGlobal  ( true );
        vss->setType    ( Net::Type::GROUND );
      }
  
      if (not cell->getNet(_powerName)) {
        Net* vdd = Net::create ( _cell, _powerName );
        vdd->setExternal( true );
        vdd->setGlobal  ( true );
        vdd->setType    ( Net::Type::POWER );
      }
    }
  }


  inline Model::~Model ()
  { for ( auto subckt : _subckts ) delete subckt; }


  inline const Model::Lut& Model::getLut     () { return _blifLut; }
  inline Cell*             Model::getCell    () const { return _cell; }
  inline size_t            Model::getDepth   () const { return _depth; }
  inline const Subckts&    Model::getSubckts () const { return _subckts; }


  Net* Model::newOne ()
  {
    if (not _masterNetOne) return NULL; 
    // if (_oneInstance) return _oneInstance->getPlug( _masterNetOne )->getNet();
    ostringstream sigName; sigName <<      "one_" << _supplyCount;
    ostringstream insName; insName << "cmpt_one_" << _supplyCount++;
    _oneInstance = Instance::create( _cell, insName.str(), _oneCell );
    Net* one = Net::create( _cell, sigName.str() );
    _oneInstance->getPlug( _masterNetOne )->setNet( one );
    return one;
  }


  Net* Model::newZero ()
  {
    if (not _masterNetZero) return NULL;
    // if (_zeroInstance) return _zeroInstance->getPlug( _masterNetZero )->getNet();
    ostringstream sigName; sigName <<      "zero_" << _supplyCount;
    ostringstream insName; insName << "cmpt_zero_" << _supplyCount++;
    _zeroInstance = Instance::create( _cell, insName.str(), _zeroCell );
    Net* zero = Net::create( _cell, sigName.str() );
    _zeroInstance->getPlug( _masterNetZero )->setNet( zero );
    return zero;
  }


  Net* Model::mergeNet ( string name, bool isExternal, unsigned int direction )
  {
    bool isClock = AllianceFramework::get()->isCLOCK( name );

    Net* net = _cell->getNet( name );
    if (not net) {
      net = Net::create( _cell, name );
      net->setExternal ( isExternal );
      net->setDirection( (Net::Direction::Code)direction );
      if (isClock) net->setType( Net::Type::CLOCK );

      // if (_cell->getName() == "CR_dec19")
      //   cerr << "CR_dec19 net create:" << direction << " " << net << endl;
    } else {
      net->addAlias( name );
      if (isExternal) net->setExternal( true );
      direction &= ~Net::Direction::UNDEFINED;
      direction |= net->getDirection();
      net->setDirection( (Net::Direction::Code)direction );
      if (isClock) net->setType( Net::Type::CLOCK );

      // if (_cell->getName() == "CR_dec19")
      //   cerr << "CR_dec19 net merge:" << direction << " " << net << endl;
    }
    return net;
  }


  Net* Model::mergeAlias ( string name1, string name2 )
  {
    Net* net1 = _cell->getNet( name1 );
    Net* net2 = _cell->getNet( name2 );

    if (net1 and (net1 == net2)) return net1;
    if (net1 and net2) {
      if (net1->isSupply() and (net2->isExternal() and not net2->isSupply())) {
        ostringstream message;

        if (net1->isPower()) {
          message << "In model " << _cell->getName() << "\n          "
                  << "Terminal " << net2->getName()
                  << " is connected to POWER " << net1->getName()
                  << " through the alias " << name1 << ".";
                     
          Net* one = newOne();
          if (one) net2->merge( one );
          else
            message << "\n          (no tie high, connexion has been LEFT OPEN)";
        }

        if (net1->isGround()) {
          message << "In model " << _cell->getName() << "\n          "
                  << "Terminal " << net2->getName()
                  << " is connected to GROUND " << net1->getName()
                  << " through the alias " << name1 << ".";
                     
          Net* zero = newZero();
          if (zero) net2->merge( zero );
          else
            message << "\n          (no tie low, connexion has been LEFT OPEN)";
        }

        if (not message.str().empty()) cerr << Warning( message.str() ) << endl;
        return net2;
      }
        
      if (  (not net1->isExternal() and net2->isExternal()) 
         or (    net1->isExternal() and net2->isExternal() and (net1->getId() > net2->getId()) ) ) {
        std::swap( net1 , net2  );
        std::swap( name1, name2 );
      }

      // if (_cell->getName() == "CR_dec19")
      //   cerr << "CR_dec19 alias net merge:" << net2 << " -> " << net1 << endl;

      net1->merge( net2 ); return net1;
    }

    if (net2) {
      std::swap( net1 , net2  );
      std::swap( name1, name2 );
    }

    if (not net1) {
      net1 = Net::create( _cell, name1 );
      net1->setExternal ( false );

      // if (_cell->getName() == "CR_dec19")
      //   cerr << "CR_dec19 alias net create:" << net1 << endl;
    }

    net1->addAlias( name2 );
    return net1;
  }


  Subckt* Model::addSubckt ( string modelName )
  {
    string instanceName = "subckt_" + getString(_subckts.size()) + "_" + modelName;
    _subckts.push_back( new Subckt( modelName, instanceName ) );

    return _subckts.back();
  }


  size_t  Model::computeDepth ()
  {
    if (_depth) return _depth;

    for ( auto subckt : _subckts ) {
      Model* subcktModel = subckt->getModel();
      if (not subcktModel) {
        subcktModel = Model::find( subckt->getModelName() );
        subckt->setModel( subcktModel );
      }

      size_t subcktDepth = subckt->getDepth();
      if (not subcktDepth and subckt->getModel())
        subcktDepth = subckt->getModel()->computeDepth();

      _depth = std::max( _depth, subcktDepth );
    }

    return _depth;
  }


  void  Model::connectSubckts ()
  {
    for ( Subckt* subckt : _subckts ) {
      if(not subckt->getModel())
        throw Error( "No .model or cell named <%s> has been found.\n"
                   , subckt->getModelName().c_str() );

      Instance* instance = Instance::create( _cell
                                           , subckt->getInstanceName()
                                           , subckt->getModel()->getCell()
                                           );

      for ( auto connection : subckt->getConnections() ) {
        string masterNetName = connection.first;
        string netName       = connection.second;
        //cparanoid << "\tConnection "
        //          << "plug: <" << masterNetName << ">, "
        //          << "external: <" << netName << ">."
        //          << endl;
        Net* net       = _cell->getNet( netName );
        Net* masterNet = instance->getMasterCell()->getNet(masterNetName,false);
        if(not masterNet) {
          Name vlogMasterNetName = NamingScheme::vlogToVhdl( masterNetName, NamingScheme::NoLowerCase );
          masterNet = instance->getMasterCell()->getNet(vlogMasterNetName);
          if(not masterNet) {
            ostringstream tmes;
            tmes << "The master net <" << masterNetName << "> hasn't been found "
                 << "for instance <" << subckt->getInstanceName() << "> "
                 << "of model <" << subckt->getModelName() << ">"
                 << "in model <" << getCell()->getName() << ">"
                 << endl;
            throw Error(tmes.str());
          }
        }

        Plug* plug = instance->getPlug( masterNet );
        if(not plug) {
          ostringstream tmes;
          tmes << "The plug in net \"" << netName << "\" "
               << "for master net \"" << masterNetName << "\" hasn't been found.\n        "
               << "(instance \"" << subckt->getInstanceName() << "\" "
               << "of model \"" << subckt->getModelName() << "\" "
               << "in model \"" << getCell()->getName() << "\")"
               << endl;
          throw Error(tmes.str());
        }

        Net* plugNet = plug->getNet();

        if (not plugNet) { // Plug not connected yet
          if (not net) net = Net::create( _cell, netName );
          plug->setNet( net );
          plugNet = net;
        }
        else if (not net) { // Net doesn't exist yet
          plugNet->addAlias( netName );
        }
        else if (plugNet != net){ // Plus already connected to another net
          if (not plugNet->isExternal()) {
            net->merge( plugNet );
          }
          else plugNet->merge( net );
        }

        // if (subckt->getModel()->getCell()->getName() == "sm0") {
        //   cerr << "sm0 plug:" << plug->getMasterNet()->getName() << " => net:" << net->getName() << endl;
        // }

        if (plugNet->isSupply() and not plug->getMasterNet()->isSupply()) {
          ostringstream message;
          message << "In " << instance << "\n          "
                  << "Terminal " << plug->getMasterNet()->getName()
                  << " is connected to POWER/GROUND " << plugNet->getName()
                  << " through the alias " << netName
                  << ".";

          if (_masterNetOne) {
            if (plugNet->isPower()) plug->setNet( newOne() );
          } else
            message << "\n          (no tie high, connexion has been LEFT OPEN)";

          if (_masterNetZero) {             
            if (plugNet->isGround()) plug->setNet( newZero() );
          } else
            message << "\n          (no tie low, connexion has been LEFT OPEN)";

          if (not message.str().empty()) cerr << Warning( message.str() ) << endl;
        }
      }
    }
  }


}  // Anonymous namespace.


namespace CRL {


  vector<Library*>  Blif::_libraries;


  void  Blif::add ( Library* library )
  { if (library) _libraries.push_back( library ); }


  Cell* Blif::load ( string cellPath, bool enforceVhdl )
  {
    using namespace std;

    string mainName;
    string blifFile  = cellPath;
    size_t dot       = cellPath.find_first_of('.');
    if (dot != string::npos) {
      blifFile = cellPath.substr( 0, dot );
      mainName = cellPath.substr( dot+1 );
    }
  
    auto framework = AllianceFramework::get();
  
    cmess2 << "     " << tab++ << "+ " << blifFile << " [blif]" << endl;

    Cell*                 mainModel = NULL;
    Model*                blifModel = NULL;
    Tokenize              tokenize  ( blifFile );
    const vector<string>& blifLine  = tokenize.blifLine();

    UpdateSession::open();
    while ( tokenize.readEntry() ) {
      if (tokenize.state() == Tokenize::Model) {
        if (blifModel) {
          cerr << Error( "Blif::load() Previous \".model\" %s not closed (missing \".end\"?).\n"
                         "                    File %s.blif at line %u."
                       , getString(blifModel->getCell()->getName()).c_str()
                       , blifFile.c_str()
                       , tokenize.lineno()
                       ) << endl;
          blifModel = NULL;
          --tab;
        }

        Cell* cell = framework->createCell( blifLine[1] );
        cell->setTerminalNetlist( false );
        blifModel = new Model ( cell );

        if (not mainModel or (blifLine[1] == mainName))
          mainModel = blifModel->getCell();
      } 

      if (tokenize.state() == Tokenize::End) {
        if (blifModel) { blifModel = NULL; --tab; continue; }
      }

      if (tokenize.state() == Tokenize::Clock) {
        cerr << Error( "Blif::load() \".clock\" command is not supported.\n"
                       "                    File %s.blif at line %u."
                     , blifFile.c_str()
                     , tokenize.lineno()
                     ) << endl;
        continue;
      }

      if (tokenize.state() == Tokenize::Latch) {
        cerr << Error( "Blif::load() \".latch\" command is not supported.\n"
                       "                    File %s.blif at line %u."
                     , blifFile.c_str()
                     , tokenize.lineno()
                     ) << endl;
        continue;
      }

      if (tokenize.state() == Tokenize::MLatch) {
        cerr << Error( "Blif::load() \".mlatch\" command is not supported.\n"
                       "                    File %s.blif at line %u."
                     , blifFile.c_str()
                     , tokenize.lineno()
                     ) << endl;
        continue;
      }

      if (not blifModel) {
        cerr << Error( "Blif::load() Unexpected command \"%s\" outside of .model definition.\n"
                       "                    File %s.blif at line %u."
                     , blifLine[0].c_str()
                     , blifFile.c_str()
                     , tokenize.lineno()
                     ) << endl;
        continue;
      }

      if (tokenize.state() == Tokenize::Inputs) {
      //cerr << "Reading .inputs of " << blifModel->getCell() << endl;
        for ( size_t i=1 ; i<blifLine.size() ; ++i ) {
          blifModel->mergeNet( blifLine[i], true, Net::Direction::IN );
        }
      //cerr << "Reading .inputs of " << blifModel->getCell() << " DONE" << endl;
      }

      if (tokenize.state() == Tokenize::Outputs) {
      //cerr << "Reading .outputs of " << blifModel->getCell() << endl;
        for ( size_t i=1 ; i<blifLine.size() ; ++i ) {
          blifModel->mergeNet( blifLine[i], true, Net::Direction::OUT );
        }
      //cerr << "Reading .outputs of " << blifModel->getCell() << " DONE" << endl;
      }

      if (tokenize.state() & Tokenize::Names) {
        if (tokenize.state() & Tokenize::CoverAlias) {
          blifModel->mergeAlias( blifLine[1], blifLine[2] );
        } else if (tokenize.state() & Tokenize::CoverZero) {
          cparanoid << Warning( "Blif::load() Definition of an alias <%s> of VSS in a \".names\". Maybe you should use tie cells?\n"
                                "          File \"%s.blif\" at line %u."
                              , blifLine[1].c_str()
                              , blifFile.c_str()
                              , tokenize.lineno()
                              ) << endl;
          //blifModel->mergeAlias( blifLine[1], "vss" );
          blifModel->getCell()->getNet( blifModel->getGroundName() )->addAlias( blifLine[1] );
        } else if (tokenize.state() & Tokenize::CoverOne ) {
          cparanoid << Warning( "Blif::load() Definition of an alias <%s> of VDD in a \".names\". Maybe you should use tie cells?\n"
                                "          File \"%s.blif\" at line %u."
                              , blifLine[1].c_str()
                              , blifFile.c_str()
                              , tokenize.lineno()
                              ) << endl;
          //blifModel->mergeAlias( blifLine[1], "vdd" );
          blifModel->getCell()->getNet( blifModel->getPowerName() )->addAlias( blifLine[1] );
        } else {
          cerr << Error( "Blif::load() Unsupported \".names\" cover construct.\n"
                         "          File \"%s.blif\" at line %u."
                       , blifFile.c_str()
                       , tokenize.lineno()
                       ) << endl;
          continue;
        }
      }

      if (tokenize.state() == Tokenize::Subckt or tokenize.state() == Tokenize::Gate) {
        Subckt* subckt = blifModel->addSubckt( blifLine[1] );
        for ( size_t i=2 ; i<blifLine.size() ; ++i ) {
          size_t equal = blifLine[i].find('=');
          if (equal == string::npos) {
            cerr << Error( "Blif::load() Bad affectation in \".subckt\": %s.\n"
                          "                    File %s.blif at line %u."
                         , blifLine[i].c_str()
                         , blifFile.c_str()
                         , tokenize.lineno()
                         ) << endl;
            continue;
          }
          subckt->addConnection( make_pair(blifLine[i].substr(0,equal)
                                          ,blifLine[i].substr(  equal+1)) );

        }
      }
    }

    if (blifModel) {
      cerr << Error( "Blif::load() Last \".model\" %s was not closed (missing \".end\"?).\n"
                     "                    File %s.blif at line %u."
                   , getString(blifModel->getCell()->getName()).c_str()
                   , blifFile.c_str()
                   , tokenize.lineno()
                   ) << endl;
      tab--;
    }
    tab--;

    Model::orderModels();
    Model::connectModels();
    if (enforceVhdl) Model::toVhdlModels();
    Model::clearStatic();
    UpdateSession::close();

    --tab;

    if (not mainModel)
      cerr << Warning( "Blif::load(): File %s.blif doesn't contains any \".model\".\n"
                     , blifFile.c_str()
                     );

    return mainModel;
  }


}  // CRL namespace.


