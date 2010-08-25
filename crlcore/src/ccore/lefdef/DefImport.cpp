
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |        C a d e n c e   D E F   I m p o r t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./DefImport.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstdio>
#include  <cstring>
#include  <memory>
#if HAVE_LEFDEF
#  include  "lefrReader.hpp"
#  include  "defrReader.hpp"
#endif
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Net.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/Contact.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Library.h"
#include  "hurricane/UpdateSession.h"
#include  "crlcore/Utilities.h"
#include  "crlcore/ToolBox.h"
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/DefImport.h"


#if HAVE_LEFDEF

namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;


  void  addSupplyNets ( Cell* cell )
  {
    Net* vss = Net::create ( cell, "vss" );
    vss->setExternal ( true );
    vss->setGlobal   ( true );
    vss->setType     ( Net::Type::GROUND );

    Net* vdd = Net::create ( cell, "vdd" );
    vdd->setExternal ( true );
    vdd->setGlobal   ( true );
    vdd->setType     ( Net::Type::POWER );
  }


  class DefParser {
    public:
      static AllianceFramework* getFramework             ();
      static DbU::Unit          fromDefUnits             ( int );
      static Transformation::Orientation                 
                                fromDefOrientation       ( int orient );
      static Transformation     getTransformation        ( const Box&
                                                         , const DbU::Unit x
                                                         , const DbU::Unit y
                                                         , const Transformation::Orientation
                                                         );
      static Cell*              parse                    ( string file, unsigned int flags );
                                DefParser                ( string& file, AllianceLibrary*, unsigned int flags );
                               ~DefParser                ();
      inline bool               hasErrors                ();
      inline unsigned int       getFlags                 () const;
      inline AllianceLibrary*   getLibrary               ();
      inline Cell*              getCell                  ();
      inline size_t             getPitchs                () const;
      inline size_t             getSlices                () const;
      inline const Box&         getFitOnCellsDieArea     () const;
      inline Net*               getPrebuildNet             () const;
             Net*               lookupNet                ( const string& );
      inline vector<string>&    getErrors                ();
      inline void               pushError                ( const string& );
             int                flushErrors              ();
      inline void               clearErrors              ();
      inline void               setPitchs                ( size_t );
      inline void               setSlices                ( size_t );
      inline void               setPrebuildNet             ( Net* );
             void               addNetLookup             ( const string& netName, Net* );
      inline void               mergeToFitOnCellsDieArea ( const Box& );
    private:                                         
      static int                _designEndCbk            ( defrCallbackType_e, void*         , defiUserData );
      static int                _dieAreaCbk              ( defrCallbackType_e, defiBox*      , defiUserData );
      static int                _pinCbk                  ( defrCallbackType_e, defiPin*      , defiUserData );
      static int                _componentCbk            ( defrCallbackType_e, defiComponent*, defiUserData );
      static int                _componentEndCbk         ( defrCallbackType_e, void*         , defiUserData );
      static int                _netCbk                  ( defrCallbackType_e, defiNet*      , defiUserData );
      static int                _netEndCbk               ( defrCallbackType_e, void*         , defiUserData );
      static int                _pathCbk                 ( defrCallbackType_e, defiPath*     , defiUserData );
             Cell*              _createCell              ( const char* name );
    private:
      static double             _defUnits;
      static AllianceFramework* _framework;
             string             _file;
             unsigned int       _flags;
             AllianceLibrary*   _library;
             Cell*              _cell;
             size_t             _pitchs;
             size_t             _slices;
             Box                _fitOnCellsDieArea;
             Net*               _prebuildNet;
             map<string,Net*>   _netsLookup;
             vector<string>     _errors;
  };


  double             DefParser::_defUnits  = 0.01;
  AllianceFramework* DefParser::_framework = NULL;


  DefParser::DefParser ( string& file, AllianceLibrary* library, unsigned int flags )
    : _file             (file)
    , _flags            (flags)
    , _library          (library)
    , _cell             (NULL)
    , _pitchs           (0)
    , _slices           (0)
    , _fitOnCellsDieArea()
    , _prebuildNet      (NULL)
    , _netsLookup       ()
    , _errors           ()
  {
    defrInit               ();
    defrSetDesignEndCbk    ( _designEndCbk );
    defrSetDieAreaCbk      ( _dieAreaCbk );
    defrSetPinCbk          ( _pinCbk );
    defrSetComponentCbk    ( _componentCbk );
    defrSetComponentEndCbk ( _componentEndCbk );
    defrSetNetCbk          ( _netCbk );
    defrSetNetEndCbk       ( _netEndCbk );
    defrSetPathCbk         ( _pathCbk );
  }


  DefParser::~DefParser ()
  {
    defrReset ();
  }


         AllianceFramework* DefParser::getFramework             () { return _framework; }
  inline DbU::Unit          DefParser::fromDefUnits             ( int u ) { return DbU::lambda(_defUnits*(double)u); }
  inline bool               DefParser::hasErrors                () { return not _errors.empty(); }
  inline unsigned int       DefParser::getFlags                 () const { return _flags; }
  inline AllianceLibrary*   DefParser::getLibrary               () { return _library; }
  inline Cell*              DefParser::getCell                  () { return _cell; }
  inline size_t             DefParser::getPitchs                () const { return _pitchs; }
  inline size_t             DefParser::getSlices                () const { return _slices; }
  inline const Box&         DefParser::getFitOnCellsDieArea     () const { return _fitOnCellsDieArea; }
  inline Net*               DefParser::getPrebuildNet           () const { return _prebuildNet; }
  inline vector<string>&    DefParser::getErrors                () { return _errors; }
  inline void               DefParser::pushError                ( const string& error ) { _errors.push_back(error); }
  inline void               DefParser::clearErrors              () { return _errors.clear(); }
  inline void               DefParser::setPitchs                ( size_t pitchs ) { _pitchs=pitchs; }
  inline void               DefParser::setSlices                ( size_t slices ) { _slices=slices; }
  inline void               DefParser::setPrebuildNet           ( Net* net ) { _prebuildNet=net; }
  inline void               DefParser::mergeToFitOnCellsDieArea ( const Box& box ) { _fitOnCellsDieArea.merge(box); }


  Transformation::Orientation  DefParser::fromDefOrientation ( int orient )
  {
  // Note : the codes between DEF & Hurricane matches.
  // This function is just to be clear.
    switch ( orient ) {
      default:
      case 0: break;                                  // N, default.
      case 1: return Transformation::Orientation::R1; // W
      case 2: return Transformation::Orientation::R2; // S
      case 3: return Transformation::Orientation::R3; // E
      case 4: return Transformation::Orientation::MX; // FN
      case 5: return Transformation::Orientation::XR; // FW
      case 6: return Transformation::Orientation::MY; // FS
      case 7: return Transformation::Orientation::YR; // FE
    }
    return Transformation::Orientation::ID;
  }


  Transformation  DefParser::getTransformation ( const Box& abox
                                               , DbU::Unit  x
                                               , DbU::Unit  y
                                               , const Transformation::Orientation orientation
                                               )
  {
	switch (orientation) {
      default:
      case Transformation::Orientation::ID: return Transformation ( x, y, orientation );
      case Transformation::Orientation::R1: return Transformation ( x, y+abox.getWidth(), orientation );
      case Transformation::Orientation::R2: return Transformation ( x+abox.getWidth (), y+abox.getHeight(), orientation );
      case Transformation::Orientation::R3: return Transformation ( x+abox.getHeight(), y, orientation );
      case Transformation::Orientation::MX: return Transformation ( x+abox.getWidth (), y, orientation );
      case Transformation::Orientation::XR: return Transformation ( x+abox.getHeight(), y+abox.getWidth(), orientation );
      case Transformation::Orientation::MY: return Transformation ( x, y+abox.getHeight(), orientation );
      case Transformation::Orientation::YR: return Transformation ( x+abox.getHeight(), y+abox.getWidth(), orientation );
	}
	return Transformation ();
  }


  Cell* DefParser::_createCell ( const char* name )
  {
    _cell = DefParser::getFramework()->createCell ( name, NULL );
    addSupplyNets ( _cell );
    return _cell;
  }


  int  DefParser::flushErrors ()
  {
    int code = (hasErrors()) ? 1 : 0;

    for ( size_t ierror=0 ; ierror < _errors.size() ; ++ierror ) {
        string message = "DefImport::load(): " + _errors[ierror];
        cerr << Error(message.c_str(),getString(_cell->getName()).c_str()) << endl;
    }
    clearErrors ();

    return code;
  }


  Net* DefParser::lookupNet ( const string& netName )
  {
    map<string,Net*>::iterator imap = _netsLookup.find(netName);
    if ( imap == _netsLookup.end() ) return NULL;

    return (*imap).second;
  }


  void  DefParser::addNetLookup ( const string& netName, Net* net )
  {
    if ( lookupNet(netName) == NULL ) _netsLookup.insert ( make_pair(netName,net) );
  }


  int  DefParser::_designEndCbk ( defrCallbackType_e c, void*, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;

    if (      (parser->getFlags() & DefImport::FitAbOnCells)
       and not parser->getFitOnCellsDieArea().isEmpty() ) {
      parser->getCell()->setAbutmentBox ( parser->getFitOnCellsDieArea() );
    }
      
    return 0;
  }


  int  DefParser::_dieAreaCbk ( defrCallbackType_e c, defiBox* box, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;

    parser->getCell()->setAbutmentBox
      ( Box ( fromDefUnits(box->xl())
            , fromDefUnits(box->yl())
            , fromDefUnits(box->xh())
            , fromDefUnits(box->yh()) ) );

    return 0;
  }


  int  DefParser::_pinCbk ( defrCallbackType_e c, defiPin* pin, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;

  //cout << "     - Pin " << pin->pinName() << ":" << pin->netName() << endl;

    Net* hnet = parser->getCell()->getNet ( pin->netName() );
    if ( hnet == NULL ) {
      hnet = Net::create ( parser->getCell(), pin->netName() );
      parser->addNetLookup ( pin->netName(), hnet );
      if ( string(pin->netName()).compare(pin->pinName()) != 0 )
         parser->addNetLookup ( pin->pinName(), hnet );
    }

    return 0;
  }


  int  DefParser::_componentCbk ( defrCallbackType_e c, defiComponent* component, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;

    string componentName = component->name();
    string componentId   = component->id();

    Cell* masterCell = DefParser::getFramework()->getCell ( componentName, Catalog::State::Views );
    if ( masterCell == NULL ) {
      ostringstream message;
      message << "Unknown model/Cell (LEF MACRO) " << componentName << " in <%s>.";
      parser->pushError ( message.str() );
      return 0;
    }

    Transformation            placement;
    Instance::PlacementStatus state     ( Instance::PlacementStatus::UNPLACED );
    if ( component->isPlaced() or component->isFixed() ) {
      state = (component->isPlaced()) ? Instance::PlacementStatus::PLACED
                                      : Instance::PlacementStatus::FIXED;

      placement = getTransformation ( masterCell->getAbutmentBox()
                                    , fromDefUnits(component->placementX())
                                    , fromDefUnits(component->placementY())
                                    , fromDefOrientation ( component->placementOrient() )
                                    );
    }

    Instance* instance = Instance::create ( parser->getCell()
                                          , componentId
                                          , masterCell
                                          , placement
                                          , state
                                          );
    if ( state != Instance::PlacementStatus::UNPLACED ) {
      parser->mergeToFitOnCellsDieArea ( instance->getAbutmentBox() );
    }

    return 0;
  }


  int  DefParser::_componentEndCbk ( defrCallbackType_e c, void*, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;
    return parser->flushErrors ();
  }


  int  DefParser::_netCbk ( defrCallbackType_e c, defiNet* net, lefiUserData ud )
  {
    static size_t netCount = 0;

    DefParser* parser = (DefParser*)ud;

  //cout << "     - Net " << net->name() << endl;
    
    Net* hnet = parser->lookupNet ( net->name() );
    if ( hnet == NULL )
      hnet = Net::create ( parser->getCell(), net->name() );

    if ( parser->getPrebuildNet() != NULL ) {
      hnet->merge ( parser->getPrebuildNet() );
      parser->setPrebuildNet ( NULL );
    }

    if (tty::enabled()) {
      cmess2 << "       " << tty::bold << setw(7) << setfill('0') << ++netCount << ":" << setfill(' ')
             << tty::reset << setw(40) << "<" << net->name() << ">  " << tty::cr;
      cmess2.flush ();
    }

    int numConnections = net->numConnections();
    for ( int icon=0 ; icon<numConnections ; ++icon ) {
      string instanceName = net->instance(icon);
      string pinName      = net->pin(icon);

    // Connect to an external pin.
      if ( instanceName.compare("PIN") == 0 ) continue;

      Instance* instance = parser->getCell()->getInstance ( instanceName );
      if ( instance == NULL ) {
        ostringstream message;
        message << "Unknown instance (DEF COMPONENT) <" << instanceName << "> in <%s>.";
        parser->pushError ( message.str() );
        continue;
      }

      Net* masterNet = instance->getMasterCell()->getNet ( pinName );
      if ( masterNet == NULL ) {
        ostringstream message;
        message << "Unknown PIN <" << pinName << "> in instance <"
                << instanceName << "> (LEF MACRO) in <%s>.";
        parser->pushError ( message.str() );
        continue;
      }

      instance->getPlug(masterNet)->setNet(hnet);
    }

    return 0;
  }


  int  DefParser::_netEndCbk ( defrCallbackType_e c, void*, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;
    return parser->flushErrors ();
  }


  int  DefParser::_pathCbk ( defrCallbackType_e c, defiPath* path, lefiUserData ud )
  {

    DefParser*  parser       = (DefParser*)ud;
    Technology* technology   = DataBase::getDB()->getTechnology();
    Net*        hnet         = parser->getPrebuildNet();

    if ( hnet == NULL ) {
      hnet = Net::create ( parser->getCell(), "__prebuild__" );
      parser->setPrebuildNet ( hnet );
    }

    Contact*     source      = NULL;
    Contact*     target      = NULL;
    const Layer* layer       = NULL;
    const Layer* viaLayer    = NULL;
    DbU::Unit    width       = DbU::lambda(2.0);
    DbU::Unit    x, y;
    int          defx, defy, defext;
    int          elementType;

    path->initTraverse ();
    while ( (elementType = path->next()) != DEFIPATH_DONE ) {
      bool createSegment = false;
      bool createVia     = false;

      switch ( elementType ) {
        case DEFIPATH_LAYER:
          layer = technology->getLayer ( path->getLayer() );
          break;
        case DEFIPATH_WIDTH:
          width = fromDefUnits(path->getWidth());
          break;
        case DEFIPATH_POINT:
          path->getPoint ( &defx, &defy );
          x = fromDefUnits ( defx );
          y = fromDefUnits ( defy );
          createSegment = true;
          break;
        case DEFIPATH_FLUSHPOINT:
          path->getFlushPoint ( &defx, &defy, &defext );
          x = fromDefUnits ( defx );
          y = fromDefUnits ( defy );
          target = NULL;
          createSegment = true;
          break;
        case DEFIPATH_VIA:
          viaLayer  = technology->getLayer ( path->getVia() );
          createVia = true;
          break;
      }

      if ( createSegment ) {
        source = target;
        target = Contact::create ( hnet, layer, x, y );
        if ( source != NULL ) {
          if ( source->getX() == x ) {
            Vertical::create ( source, target, layer, x, width );
          } else if ( source->getY() == y ) {
            Horizontal::create ( source, target, layer, y, width );
          } else {
            ostringstream message;
            message << "Non-manhattan segment in net <" << hnet->getName() << ">.";
            parser->pushError ( message.str() );
          }
        }
      }
      
      if ( createVia ) {
        if ( target != NULL ) {
          target = Contact::create ( target, viaLayer, 0, 0 );
        } else {
          target = Contact::create ( hnet, viaLayer, x, y, 0, 0 );
        }
      }
    }

    return 0;
  }


  Cell* DefParser::parse ( string file, unsigned int flags )
  {
    cmess1 << "  o  DEF: <" << file << ">" << endl;

    size_t iext = file.rfind ( '.' );
    if ( file.compare(iext,4,".def") != 0 )
      throw Error ("DefImport::load(): DEF files must have  \".def\" extension <%s>.",file.c_str());

    _framework = AllianceFramework::get ();

    size_t              islash     = file.rfind  ( '/' );
    string              designName = file.substr ( ((islash == string::npos) ? 0 : islash), file.size()-4 );
    AllianceLibrary*    library    = _framework->getAllianceLibrary ( (unsigned int)0 );
    auto_ptr<DefParser> parser     ( new DefParser(file,library,flags) );

    FILE* defStream = fopen ( file.c_str(), "r" );
    if ( defStream == NULL )
      throw Error ("DefImport::load(): Cannot open DEF file <%s>.",file.c_str());

    parser->_createCell ( designName.c_str() );
    defrRead  ( defStream, file.c_str(), (defiUserData)parser.get(), 1 );

    fclose ( defStream );

    return parser->getCell();
  }


}  // End of anonymous namespace.

#endif  // HAVE_LEFDEF


namespace CRL {

  using std::cerr;
  using std::endl;
  using std::string;
  using Hurricane::UpdateSession;


  Cell* DefImport::load ( string design, unsigned int flags )
  {
    UpdateSession::open ();

    Cell* cell = NULL;
#if HAVE_LEFDEF
    cell = DefParser::parse ( design+".def", flags );
#else
    cerr << "[ERROR] CRL::DefImport::load(): \n"
         << "  Coriolis2 hasn't been compiled with LEF/DEF support. To enable LEF/DEF\n"
         << "  support, you may obtain parser/driver from Si2 (www.si2.org) then recompile."
         << endl;
#endif

    UpdateSession::close ();

    return cell;
  }


}  // End of CRL namespace.
