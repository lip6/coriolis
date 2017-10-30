
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                Christophe Alexandre              |
// |  E-mail      :        Christophe.Alexandre@lip6.fr              |
// |                                                                 |
// |  Modify By   :                       Damien Dupuis              |
// |  E-mail      :               Damien.Dupuis@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./DefParser.cpp"                          |
// +-----------------------------------------------------------------+



#include  "hurricane/Warning.h"
#include  "hurricane/Technology.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Component.h"
#include  "hurricane/Plug.h"
#include  "hurricane/Contact.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Pin.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Instance.h"

#include  "crlcore/Utilities.h"
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/NetExtension.h"
#include  "crlcore/LefDefExtension.h"
#include  "LefDef.h"

#if defined(HAVE_LEFDEF)
#  include  "defrReader.hpp"
#endif




namespace {

  using namespace std;

  using Hurricane::Warning;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Transformation;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Technology;
  using Hurricane::DataBase;
  using Hurricane::Horizontal;
  using Hurricane::Component;
  using Hurricane::Vertical;
  using Hurricane::Contact;
  using Hurricane::Plug;
  using Hurricane::Pin;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::tab;

  using namespace CRL;


  // ---------------------------------------------------------------
  // Module Global Variables.


  AllianceFramework* __framework        = NULL;
  Technology*        __technology       = NULL;
  Cell*              __design;
  long               __defUserData      = 0;
  unsigned int       __defConvertFactor = 1;
  unsigned int       __lefConvertFactor = 1;


#if defined(HAVE_LEFDEF)

  // ---------------------------------------------------------------
  // Module Global Functions.


  Cell* verifyDesign ()
  {
    if ( !__design )
      throw Error ( "Empty DEF Design." );
    return __design;
  }


  Instance* verifyInstance ( const Name& name )
  {
    Instance* instance = verifyDesign()->getInstance(name);
    if ( !instance )
      throw Error ( "No instance " + getString(name) + " in DEF design." );
    return instance;
  }


  Plug* verifyPlug ( const Name& instanceName, const Name& netName )
  {
    Instance* instance = verifyInstance ( instanceName );
    Cell*     model    = instance->getMasterCell ();
    Plug*     plug     = instance->getPlug ( model->getNet(netName) );
    if ( !plug )
      throw Error ( "No Plug " + getString(netName) + " in instance " + getString(instanceName) );
    return plug;
  }


  Net* verifyNet ( const Name& instanceName, const Name& netName )
  {
    Instance* instance = verifyInstance ( instanceName );
    Cell*     model    = instance->getMasterCell ();
    Net*      modelNet = model->getNet ( netName );
    if ( !modelNet )
      throw Error ( "No net " + getString(netName) + " in cell " + getString(model->getName()) );
    return modelNet;
  }


  Transformation::Orientation  def2Transformation ( int orient )
  {
  // Note : the codes between DEF & Hurricane matches.
  // This function is just to be clear.
    switch ( orient ) {
      default:
      case 0: break; // N, default.
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


  Transformation  getTransformation ( const Box& abox
                                    , const DbU::Unit& x
                                    , const DbU::Unit& y
                                    , const Transformation::Orientation& orientation
                                    )
  {
	switch (orientation) {
      case Transformation::Orientation::ID:
        return Transformation ( x, y, orientation );
      case Transformation::Orientation::R1:
        return Transformation ( x, y+abox.getWidth(), orientation );
      case Transformation::Orientation::R2:
        return Transformation ( x+abox.getWidth(), y+abox.getHeight(), orientation );
      case Transformation::Orientation::R3:
        return Transformation ( x+abox.getHeight(), y, orientation );
      case Transformation::Orientation::MX:
        return Transformation ( x+abox.getWidth(), y, orientation );
      case Transformation::Orientation::XR:
        return Transformation ( x+abox.getHeight(), y+abox.getWidth(), orientation );
      case Transformation::Orientation::MY:
        return Transformation ( x, y+abox.getHeight(), orientation );
      case Transformation::Orientation::YR:
        return Transformation ( x+abox.getHeight(), y+abox.getWidth(), orientation );
	}
	return Transformation ();
  }


  void defUserDataError ()
  {
    cerr << Warning("defParser(): returned DEF user data is not correct.") << endl;
  }


  void checkCbType ( defrCallbackType_e cbType )
  {
    if ( (cbType < 0) || (cbType > defrDesignEndCbkType) )
      cerr << Warning("defParser(): callback type is out of bounds.") << endl;
  }


  int  cbDesignName ( defrCallbackType_e cbType, const char* name, defiUserData ud )
  {
    checkCbType ( cbType );
    if ( (long)ud != __defUserData ) defUserDataError ();

    if ( !name || (name[0] == (char)0) ) {
      cerr << Warning("defParser(): design name empty, aborting.") << endl;
      return 1;
    }

    return 0;
  }


  int cbDone ( defrCallbackType_e cbType, void* dummy, defiUserData ud )
  {
    checkCbType ( cbType );
    if ( (long)ud != __defUserData ) defUserDataError ();

    return 0;
  }


  int  cbUnits ( defrCallbackType_e cbType, double value, defiUserData ud )
  {
    checkCbType ( cbType );
    if ( (long)ud != __defUserData ) defUserDataError ();

    __defConvertFactor = (unsigned)value;
    LefDefExtension::setDefDbuPerMicron ( verifyDesign(), __defConvertFactor );

    return 0;
  }


  int  cbComponent ( defrCallbackType_e cbType, defiComponent* component, defiUserData ud )
  {
    checkCbType ( cbType );
    if ( (long)ud != __defUserData ) defUserDataError ();
  
    Cell* model = __framework->getCell ( component->defiComponent::name(), Catalog::State::Views );
    if ( !model )
      throw Error ( "Unkown cell: " + string(component->defiComponent::name()) );
  
    Transformation            instancePosition;
    Instance::PlacementStatus placementStatus ( Instance::PlacementStatus::UNPLACED );
    if ( component->defiComponent::isPlaced() || component->defiComponent::isFixed() ) {
      if ( component->defiComponent::isPlaced() )
        placementStatus = Instance::PlacementStatus::PLACED;
      else
        placementStatus = Instance::PlacementStatus::FIXED;
      instancePosition = getTransformation
        ( model->getAbutmentBox()
        , DbU::lambda((double)component->defiComponent::placementX() * (double)__lefConvertFactor / (double)__defConvertFactor)
        , DbU::lambda((double)component->defiComponent::placementY() * (double)__lefConvertFactor / (double)__defConvertFactor)
        , def2Transformation ( component->defiComponent::placementOrient() )
        );
    }

    Instance::create ( verifyDesign()
                     , Name(component->defiComponent::id())
                     , model, instancePosition
                     , placementStatus
                     );

    return 0;
  }


  int  cbNet ( defrCallbackType_e cbType, defiNet* defnet, defiUserData ud )
  {
    checkCbType ( cbType );
    if ( (long)ud != __defUserData ) defUserDataError ();
    if ( cbType != defrNetCbkType ) {
      cerr << Warning("defParser::cbNet(): Bogus net type.") << endl;
      return 1;
    }

    Net* net;
    Net* tryNet = verifyDesign()->getNet ( Name(defnet->defiNet::name()) );
    if ( tryNet && !tryNet->isExternal() ) {
      cerr << Warning("defParser(): Net \"%s\" already exist.",defnet->defiNet::name()) << endl;
      return 1;
    }
    if ( !tryNet ) {
      net = Net::create ( verifyDesign(), defnet->defiNet::name() );
      net->setExternal ( false );
      net->setType     ( Net::Type::LOGICAL );
    } else
      net = tryNet;
    
    for ( int i=0 ; i < defnet->numConnections() ; i++ ) {
      if ( !strcmp(defnet->instance(i),"PIN") ) continue;
      Plug* plug = verifyPlug ( Name(defnet->instance(i)), Name(defnet->pin(i)) );
      plug->setNet ( net );
    }

    for ( int i=0 ; i < defnet->numWires() ; i++ ) {
      defiWire* wire = defnet->wire(i);
      for ( int j=0 ; j < wire->numPaths() ; j++ ) {
        defiPath* p = wire->path(j);
        p->initTraverse();
        int        path          = 0;
        int        x0,y0,x1,y1   = 0;
        bool       hasFirstPoint = false;
        Layer*     layer         = NULL;
        Component* source        = NULL;
        Component* target        = NULL;
        Component* lastComponent = NULL;
        DbU::Unit  layerMinWidth = 0;

        while ( (path = static_cast<int>(p->next())) != DEFIPATH_DONE ) {
          switch ( path ) {
            case DEFIPATH_LAYER:
              layer = __technology->getLayer( Name(p->getLayer()) );
              if ( !layer )
                throw Error ( "defParser(): Unknown layer \"%s\".", p->getLayer() );
              layerMinWidth = layer->getMinimalSize();
              break;
            case DEFIPATH_VIA:
#if 0
              {
                Contact* lastContact = NULL;
                layer = __technology()->getLayer ( Name(p->getVia()) );
                if ( !layer )
                  throw Error ( "defParser(): Unknown layer \"%s\".", p->getLayer() );
                if ( !lastComponent )
                  throw Error( "defParser(): Missing last path component." );
                lastContact = dynamic_cast<Contact*>(lastComponent);
                if ( !lastContact )
                  throw Error( "defParser(): Missing last path contact." );
                lastContact->setLayer ( layer );
              }
#endif
              break;
            case DEFIPATH_POINT:
              {
                if ( !hasFirstPoint ) {
                  p->getPoint ( &x0, &y0 );
                  lastComponent = Contact::create
                    ( net, layer
                    , DbU::lambda((double)x0 * (double)__lefConvertFactor / (double)__defConvertFactor)
                    , DbU::lambda((double)y0 * (double)__lefConvertFactor / (double)__defConvertFactor)
                    ); 
                  source = lastComponent;
                  hasFirstPoint = true;
                } else {
                  p->getPoint ( &x1, &y1 );
                  lastComponent = Contact::create
                    ( net, layer
                    , DbU::lambda((double)x1 * (double)__lefConvertFactor / (double)__defConvertFactor)
                    , DbU::lambda((double)y1 * (double)__lefConvertFactor / (double)__defConvertFactor)
                    ); 
                  target = lastComponent;
                  if ( x0 == x1 ) {
                    lastComponent = Vertical::create
                      ( source, target, layer, source->getX(), layerMinWidth );
                    source = lastComponent;
                  } else {
                    if (y0 == y1) {
                      lastComponent = Horizontal::create
                        ( source, target, layer, source->getY(), layerMinWidth );
                      source = lastComponent;
                    } else {
                      throw Error ( "defParser(): Diagonal segments are not supporteds." );
                    }
                  }
                }
              }
              break;
          }
        }
      }
    }

    return 0;
  }


  int  cbSpecialNet ( defrCallbackType_e cbType, defiNet* defnet, defiUserData ud ) 
  {
    checkCbType ( cbType );
    if ( (long)ud != __defUserData ) defUserDataError ();
    
    if ( !defnet->hasUse() )
      throw Error ( "defParser(): Special net with no use precised." );

    bool  netCreated = false;
    Net*  net        = verifyDesign()->getNet ( Name(defnet->defiNet::name()) );
    if ( !net ) {
      net = Net::create ( verifyDesign(), defnet->defiNet::name() );
      if ( defnet->defiNet::hasUse() ) {
        if ( !strcmp(defnet->defiNet::use(),"GROUND") ) {
          net->setGlobal ( true );
          net->setType   ( Net::Type::GROUND );
        }
        if ( !strcmp(defnet->defiNet::use(),"POWER") ) {
          net->setGlobal ( true );
          net->setType   ( Net::Type::POWER );
        }
        if ( !strcmp(defnet->defiNet::use(),"CLOCK") ) {
          net->setType ( Net::Type::CLOCK );
        }
      }
      netCreated = true;
    }
    
    if ( !strcasecmp(defnet->use(), "POWER") ) {
      net->setGlobal ( true );
      if ( !net->isPower() )
        throw Error ( "defParser(): Net use discrepency, %s is not power."
                    , getString(net).c_str() );
        
      for ( int i = 0 ; i < defnet->numConnections() ; i++ ) {
        Net* modelNet = verifyNet ( Name(defnet->instance(i)), Name(defnet->pin(i)) );
        if ( !modelNet->isGlobal() )
          throw Error( "defReader(): %s is not global.", getString(modelNet).c_str() );

        if ( !modelNet->isPower() )
          throw Error ( "defParser(): Net use discrepency, %s is not power."
                      , getString(modelNet).c_str() );
      }
      return 0;
    }

    if ( !strcasecmp(defnet->use(), "GROUND") ) {
      net->setGlobal ( true );
      if ( !net->isGround() )
        throw Error ( "defParser(): Net use discrepency, %s is not ground."
                    , getString(net).c_str() );
      
      for ( int i = 0 ; i < defnet->numConnections() ; i++ ) {
        Net* modelNet = verifyNet ( Name(defnet->instance(i)), Name(defnet->pin(i)) );
        if ( !modelNet->isGlobal() )
          throw Error( "defReader(): %s is not global.", getString(modelNet).c_str() );
        
        if ( !modelNet->isGround() )
          throw Error ( "defParser(): Net use discrepency, %s is not ground."
                      , getString(modelNet).c_str() );
      }
      return 0;
    }

    if ( !strcasecmp(defnet->use(), "CLOCK") ) {
      if ( netCreated ) {
        net->setType ( Net::Type::CLOCK );
      } else if ( !net->isClock() )
        throw Error ( "defParser(): Net use discrepency, %s is not clock."
                    , getString(net).c_str() );

      for ( int i=0 ; i < defnet->numConnections() ; i++ ) {
        Plug* plug = verifyPlug ( Name(defnet->instance(i)), Name(defnet->pin(i)) );
        plug->setNet ( net );
      }
      return 0;
    }

    cerr << Warning("defReader(): Special net with unrecognized use.") << endl;
    return 1;
  }


  int cls ( defrCallbackType_e cbType, void* cl, defiUserData ud )
  {
    defiBox* box;
    defiPin* pin;
    defiRow* row;

    checkCbType ( cbType );
    if ( (long)ud != __defUserData ) defUserDataError ();

    switch ( cbType ) {
      case defrDieAreaCbkType :
        {
          box = (defiBox*)cl;
            
          verifyDesign()->setAbutmentBox
            ( Box ( DbU::lambda((double)box->xl() * (double)__lefConvertFactor / (double)__defConvertFactor)
                  , DbU::lambda((double)box->yl() * (double)__lefConvertFactor / (double)__defConvertFactor)
                  , DbU::lambda((double)box->xh() * (double)__lefConvertFactor / (double)__defConvertFactor)
                  , DbU::lambda((double)box->yh() * (double)__lefConvertFactor / (double)__defConvertFactor)
                  ));
          break;
        }
      case defrPinCbkType :
        {
          pin = (defiPin*)cl;
            
          Net::Direction pinDirection;
          if ( pin->defiPin::hasDirection() ) {
            if ( !strcmp(pin->defiPin::direction(), "INPUT") )
              pinDirection = Net::Direction::IN;
            else if ( !strcmp(pin->defiPin::direction(), "OUTPUT") )
              pinDirection = Net::Direction::OUT;
            else if ( !strcmp(pin->defiPin::direction(), "INOUT") )
              pinDirection = Net::Direction::INOUT;
            else
              pinDirection = Net::Direction::UNDEFINED;
          }

          Net* net = verifyDesign()->getNet ( Name ( pin->defiPin::netName() ) );
          if ( net ) {
            if ( !net->isExternal() )
              throw Error("defReader(): External and Internal Net");
            if ( pin->defiPin::hasUse() ) {
              if ( !strcmp(pin->defiPin::use(),"GROUND") && !net->isGround() )
                throw Error ( "defReader(): Discrepency in Supply Net %s."
                            , getString(net->getName()).c_str() );
              if ( !strcmp(pin->defiPin::use(),"POWER") && !net->isPower() )
                throw Error ( "defReader(): Discrepency in Supply Net %s"
                            , getString(net->getName()).c_str() );
              if ( pinDirection != net->getDirection() )
                throw Error ( "defReader(): Discrepency in Net direction %s."
                            , getString(net->getName()).c_str() );
            }
            NetExtension::addPort ( net, Name ( pin->defiPin::pinName() ) );
          } else {
            net = Net::create ( verifyDesign(), Name ( pin->defiPin::netName() ) );
            if ( pin->defiPin::hasUse() ) {
              if ( !strcmp(pin->defiPin::use(),"GROUND") ) {
                net->setGlobal ( true );
                net->setType   ( Net::Type::GROUND );
              }
              if ( !strcmp(pin->defiPin::use(),"POWER") ) {
                net->setGlobal ( true );
                net->setType   ( Net::Type::POWER );
              }
              if ( !strcmp(pin->defiPin::use(),"CLOCK") ) {
                net->setType ( Net::Type::CLOCK );
              }
              net->setExternal  ( true );
              net->setDirection ( pinDirection );
              NetExtension::addPort ( net, Name ( pin->defiPin::pinName() ) );
            } else {
              net->setType      ( Net::Type::LOGICAL );
              net->setExternal  ( true );
              net->setDirection ( pinDirection );
              NetExtension::addPort ( net, Name ( pin->defiPin::pinName() ) );
            }
          }
          if ( pin->hasLayer() ) {
            int                  xl, yl, xh, yh;
            Layer*               layer              = NULL;
            DbU::Unit            width              = 0;
            DbU::Unit            height             = 0;
            DbU::Unit            pinX               = 0;
            DbU::Unit            pinY               = 0;
            int                  layerId            = 0;
            Pin::PlacementStatus pinPlacementStatus = Pin::PlacementStatus::UNPLACED;

            if ( pin->numLayer() == 1 )
              layer = __technology->getLayer( Name(pin->layer(layerId)) );
            else
              throw Error ( "defReader(): LEF/DEF 5.6 multi-layer pins are not supported." );
            if ( !layer )
              throw Error("defReader(): Unknown Layer %s.",pin->layer(layerId));

            pin->bounds ( layerId, &xl, &yl, &xh, &yh );
            
            if ( pin->hasPlacement() ) {
              if ( pin->isPlaced() || pin->isFixed() ) {
                if ( pin->isPlaced() )
                  pinPlacementStatus = Pin::PlacementStatus::PLACED;
                else
                  pinPlacementStatus = Pin::PlacementStatus::FIXED;
                
                pinX = DbU::lambda ( (double)pin->placementX() * (double)__lefConvertFactor / (double)__defConvertFactor );
                pinY = DbU::lambda ( (double)pin->placementY() * (double)__lefConvertFactor / (double)__defConvertFactor ); 
                switch ( pin->orient() ) {
                  case 0:
                  case 4:
                    pinX  += DbU::lambda ( ( xl + xh ) * (double)__lefConvertFactor / (double)__defConvertFactor ) / 2;
                    pinY  += DbU::lambda ( ( yl + yh ) * (double)__lefConvertFactor / (double)__defConvertFactor ) / 2;
                    width  = abs(xl) + abs(xh);
                    height = abs(yl) + abs(yh);
                    break;
                  case 1:
                  case 5:
                    pinX  -= DbU::lambda ( ( yl + yh ) * (double)__lefConvertFactor / (double)__defConvertFactor ) / 2;
                    pinY  += DbU::lambda ( ( xl + xh ) * (double)__lefConvertFactor / (double)__defConvertFactor ) / 2;
                    width  = abs(yl) + abs(yh);
                    height = abs(xl) + abs(xh);
                    break;
                  case 2:
                  case 6:
                    pinX  += DbU::lambda ( ( xl + xh ) * (double)__lefConvertFactor / (double)__defConvertFactor ) / 2;
                    pinY  -= DbU::lambda ( ( yl + yh ) * (double)__lefConvertFactor / (double)__defConvertFactor ) / 2;
                    width  = abs(xl) + abs(xh);
                    height = abs(yl) + abs(yh);
                    break;
                  case 3:
                  case 7:
                    pinX  += DbU::lambda ( ( yl + yh ) * (double)__lefConvertFactor / (double)__defConvertFactor ) / 2;
                    pinY  += DbU::lambda ( ( xl + xh ) * (double)__lefConvertFactor / (double)__defConvertFactor ) / 2;
                    width  = abs(yl) + abs(yh);
                    height = abs(xl) + abs(xh);
                    break;
                  default:
                    throw Error ("defReader(): Undefined orientation %i for pin %s"
                                , pin->orient(), pin->defiPin::netName() );
                    break;
                }
              }
            }
            Name pinName ( pin->defiPin::pinName() );
            Pin::create ( net
                        , pinName
                        , Pin::AccessDirection::UNDEFINED
                        , pinPlacementStatus
                        , layer
                        , pinX, pinY
                        , width, height
                        );
          }
          break;
        }
      case defrRowCbkType :
        {
          row = (defiRow*)cl;
          
          Box rowBox
            ( DbU::lambda((double)(row->x())                          * (double)__lefConvertFactor / (double)__defConvertFactor)
            , DbU::lambda((double)(row->y())                          * (double)__lefConvertFactor / (double)__defConvertFactor)
            , DbU::lambda((double)(row->x()+row->xNum()*row->xStep()) * (double)__lefConvertFactor / (double)__defConvertFactor)
            , DbU::lambda((double)(row->y()+row->yNum()*row->yStep()) * (double)__lefConvertFactor / (double)__defConvertFactor)
            );

          LefDefExtension::addRowsBox ( verifyDesign(), rowBox );
          break;
        }

      default:
        throw Error ( "defReader(): Unknown callback type %i for cls().", cbType );
    }
    return 0;
  }

#endif // HAVE_LEFDEF


} // End of anonymous namespace.


namespace CRL {


  void  defParser ( const string cellPath, Cell *cell )
  {
    static unsigned int callNumber = 0;

    cmess2 << "     " << tab << "+ " << cellPath << endl;

#if defined(HAVE_LEFDEF)
    DataBase* db = DataBase::getDB ();
    if ( !db ) throw Error ( NullDataBase, "defParser()" );

    __technology = db->getTechnology ();
    if ( !__technology ) throw Error ( NullTechnology, "defParser()" );

    __framework = AllianceFramework::get ();
    if ( !__framework ) throw Error ( "defParser(): NULL Alliance Framework." );
    

    if ( !cell ) throw Error ( NullCell, "defParser()" );

    __lefConvertFactor = LefDefExtension::getLefDbuPerMicron ( cell );
    __design = cell;
    __design->setTerminal ( false );

    CatalogProperty* property
      = (CatalogProperty*)__design->getProperty ( CatalogProperty::getPropertyName() );
    if ( property == NULL )
      throw Error ( "defParser(): Missing Catalog Property on Cell %s."
                  , getString(__design->getName()).c_str() );

    Catalog::State* state = property->getState ();

    state->setPhysical ( true );
    state->setLogical  ( true );

    defrInit();
    if (callNumber == 0) {
      defrSetDesignCbk    ( cbDesignName );
      defrSetDesignEndCbk ( cbDone );
      defrSetUnitsCbk     ( cbUnits );
      defrSetComponentCbk ( cbComponent );
      defrSetNetCbk       ( cbNet );
      defrSetSNetCbk      ( cbSpecialNet );

      defrSetDieAreaCbk   ( (defrBoxCbkFnType)cls );
      defrSetPinCbk       ( (defrPinCbkFnType)cls );
      defrSetRowCbk       ( (defrRowCbkFnType)cls );
    
    //defrSetTechnologyCbk ( TechnoName );
      defrSetAddPathToNet ();
    }

    IoFile  ccell ( cellPath );
    ccell.open ( "r" );
    defrRead ( ccell.getFile(), getString(&ccell).c_str(), (void*)__defUserData, 1 );
    ccell.close();

#else // HAVE_LEFDEF

    cerr << "\nDummy DEF parser call for \"" << cellPath << "\"." << endl;

#endif // HAVE_LEFDEF

    callNumber++;
  }


} // End of CRL namespace.

