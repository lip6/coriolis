
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id: CParsLEF.cpp,v 1.28 2007/07/29 15:27:25 jpc Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                Christophe Alexandre              |
// |  E-mail      :        Christophe.Alexandre@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LefParser.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



# include  "hurricane/Warning.h"
# include  "hurricane/DataBase.h"
# include  "hurricane/BasicLayer.h"
# include  "hurricane/ViaLayer.h"
# include  "hurricane/Net.h"
# include  "hurricane/Pad.h"
# include  "hurricane/Vertical.h"
# include  "hurricane/Horizontal.h"
# include  "hurricane/NetExternalComponents.h"
# include  "hurricane/Cell.h"

# include  "crlcore/AllianceFramework.h"
# include  "LefDef.h"

# if  HAVE_LEFDEF && defined(LEF_ENABLED)
#   include  "lefrReader.hpp"
# endif


# define       ONLY_PAD_PINS        1


namespace {

  using namespace std;

  using Hurricane::Warning;
  using Hurricane::Error;
  using Hurricane::Box;
  using Hurricane::Layer;
  using Hurricane::Technology;
  using Hurricane::Pad;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Library;

  using namespace CRL;
    

//unsigned int       __lefConvertFactor = 1;
//Library*           __library          = NULL;
//AllianceFramework* __framework        = NULL;
//Technology*        __technology       = NULL;
//Cell*              __cell             = NULL;
//Net*               __net              = NULL;
//long               __userData         = 0;
//Catalog*           __catalog          = NULL; 


  // ---------------------------------------------------------------
  // Function  :  "Obstruction()".

  void  Obstruction ( Cell* cell, Layer* layer, const Box& bb )
  {
    Net* blockageNet = cell->getNet ( "blockagenet" );

    if (!layer)
      throw Error("No layer for blockage");

    if ( blockageNet == NULL )
      blockageNet = Net::create ( cell, "blockagenet" );

    Pad::create ( blockageNet, layer, bb );
  }
  

} // End of anonymous namespace.


namespace CRL {


  using Hurricane::Box;
  using Hurricane::Layer;
  using Hurricane::Technology;
  using Hurricane::Vertical;
  using Hurricane::Horizontal;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::Library;


// x-----------------------------------------------------------------x
// |                     Functions Definitions                       |
// x-----------------------------------------------------------------x


# if HAVE_LEFDEF && defined(LEF_ENABLED)

// -------------------------------------------------------------------
// Function  :  "badOpenTechno()".

// inline string  badOpenTechno ( const string &header, string file )
// {
//   ostringstream  m;

//   if ( header.size() > 0 )
//     m << header << " :";
//   m << "\n\n";

//   m << "    Unable to open LEF technological file : \n"
//     << "      \"" << file << "\".\n\n"
//     << "    Please check LEF_TECHNO_NAME environment variable.\n";

//   return ( m.str() );
// }


  int propDefCB(lefrCallbackType_e type, lefiProp* propInfo, lefiUserData ud)
  {
  //FIXME:checktype
    return 0;
  }




  int layerCB(lefrCallbackType_e type, lefiLayer* layer, lefiUserData ud)
  {
    DbU::Unit width      = 0;
    DbU::Unit pitch      = 0;
    DbU::Unit spacing    = 0;
    bool      hasWidth   = false;
    bool      hasPitch   = false;
    bool      hasSpacing = false;

    if ( type != lefrLayerCbkType )
      throw Error("layerCB: Invalid Callback");
    
    if ( layer->hasWidth() ) {
      hasWidth = true;
      width = DbU::getUnit(layer->width() * (double)__lefConvertFactor);
    } 
    if ( layer->hasPitch() ) {
      hasPitch = true;
      pitch = DbU::getUnit(layer->pitch() * (double)__lefConvertFactor);
    }
    for ( int i=0; i < layer->numSpacing(); i++ ) {
      if ( layer->hasSpacingRange(i) ) continue;
      if (hasSpacing)
        spacing = std::min ( spacing, DbU::getUnit(layer->spacing(i) * (double)__lefConvertFactor) );
      else {
        hasSpacing = true;
        spacing = DbU::getUnit(layer->spacing(i) * (double)__lefConvertFactor);
      }
    }
    Layer* hurricaneLayer = __technology->getLayer(Name(layer->name()));
    if ( !hurricaneLayer ) {
      hurricaneLayer = BasicLayer::create( __technology
                                         , Name(layer->name())
                                         , BasicLayer::Type::UNDEFINED
                                         , width
                                         , spacing
                                         , pitch);
    } else {
      if (hasWidth  ) hurricaneLayer->setMinimalSize(width);
      if (hasSpacing) hurricaneLayer->setMinimalSpacing(spacing);
      if (hasPitch  ) hurricaneLayer->setPitch(pitch);
    }

    if (layer->hasType()) {
      if (layer->type() == "ROUTING") {
        if (layer->hasDirection()) {
          if (layer->direction() == "VERTICAL") {
          // routing layer, direction vertical
          //setVerticalProperty(hurricaneLayer);
          //hurricaneLayer->Put(StandardSharedProperty<TRoutingLayerDirection>(Name("ROUTING"), TRoutingLayerDirection(TRoutingLayerDirection::VERTICAL)));
          }
          else if (layer->direction() == "HORIZONTAL") {
          // routing layer, direction horizontal
          //setHorizontalProperty(hurricaneLayer);
          } else {
          // routing layer, direction unknown
          }
        } else {
        // routing layer, no direction
        }
      }
    }
    
    return 0;
  }


  int viaCB(lefrCallbackType_e type, lefiVia* viaInfo, lefiUserData ud)
  {
    if ( type != lefrViaCbkType )
      throw Error("error");

    CompositeLayer* layer = ViaLayer::create(__technology, Name (viaInfo->name()) ) ;

    if ( viaInfo->numLayers() > 0 ) {
      vector<BasicLayer*>  basicLayers;
      vector<DbU::Unit>    enclosures;
      for ( int i = 0 ; i < viaInfo->numLayers() ; i++ ) {
        DbU::Unit enclosure = 0;
        for (int j = 0; j < viaInfo->numRects(i); j++) {
          enclosure = DbU::getUnit ( ( viaInfo->xh(i,j) * (double)__lefConvertFactor ) - ( viaInfo->xl(i,j) * (double)__lefConvertFactor ) );
        }
        BasicLayer* basicLayer = __technology->getBasicLayer(viaInfo->layerName(i)); 
        if ( basicLayer ) {
          basicLayers.push_back ( basicLayer );
          enclosures.push_back ( enclosure );
        }
      }
      if ( basicLayers.size() == 3 ) {
        ViaLayer::create ( __technology
                         , viaInfo->name()
                         , basicLayers[0]
                         , basicLayers[1]
                         , basicLayers[2]
                         );
      }
    }

    return 0;
  }


  int macroObsCB (lefrCallbackType_e type, lefiObstruction *obsInfo, lefiUserData ud)
  {
    lefiGeometries* geom;
    lefiGeomRect*   rect;
    lefiGeomPath*   path;
    Layer*          layer;
    double          width = 0.0;
    
    
    if (type != lefrObstructionCbkType) abort ();

    geom = obsInfo->geometries();
    for (int i = 0 ; i < geom->numItems() ; i++) {
      switch (geom->itemType(i)) {
        case lefiGeomClassE: break;
        case lefiGeomLayerE:
          {
            layer = __technology->getLayer(geom->getLayer(i));
            if (!layer)
              throw Error("Unknown Layer: " + string(geom->getLayer(i)));
            BasicLayer* basicLayer = dynamic_cast<BasicLayer*>(layer);
            const Name* blockageLayerName = getBlockageLayerName(basicLayer);
            if (blockageLayerName)
              layer = technology->getLayer(*blockageLayerName);
            if (!layer)
              if (blockageLayerName)
                throw Error("Unknown layer: " + blockageLayerName->_getString());
              else
                throw Error("Unknow Layer: (null) Name");
            break;
          }
        case lefiGeomWidthE:
          width = (geom->getWidth (i) * (double)__lefConvertFactor);
          break;
        case lefiGeomPathE:
          path = geom->getPath(i);

          if (path->numPoints < 2) {
            cerr << "Un seul point sur le path ???" << endl;
            abort ();
          }

          for (int j = 0 ; j < (path->numPoints - 1) ; j++) {
            if (path->x[j] == path->x[j + 1]) {
            // horizontal
              Obstruction ( __cell
                          , layer
                          , Box ( DbU::getUnit(path->x[j] * (double)__lefConvertFactor - width / 2)
                                , DbU::getUnit(path->y[j] * (double)__lefConvertFactor)
                                , DbU::getUnit(path->x[j + 1] * (double)__lefConvertFactor + width / 2)
                                , DbU::getUnit(path->y[j + 1] * (double)__lefConvertFactor)
                                )
                          );
            } else if (path->y[j] == path->y[j + 1]) {
            // vertical
              Obstruction ( __cell
                          , layer
                          , Box ( DbU::getUnit(path->x[j] * (double)__lefConvertFactor)
                                , DbU::getUnit(path->y[j] * (double)__lefConvertFactor - width / 2)
                                , DbU::getUnit(path->x[j + 1] * (double)__lefConvertFactor)
                                , DbU::getUnit(path->y[j + 1] * (double)__lefConvertFactor + width / 2)
                                )
                          );
            } else {
            // curiosité
              cerr << "Invalid PATH..." << endl;
              abort ();
            }
          } // for
          break;
        case lefiGeomPathIterE:
        // pas glop
          break;
        case lefiGeomRectE:
        // FIXME
        // un segment.....
        // quel net de quel cell ????
        // utiliser la var globale "cell"
          rect = geom->getRect (i);

          Obstruction ( __cell
                      , layer
                      , Box ( DbU::getUnit(rect->xl * (double)__lefConvertFactor)
                            , DbU::getUnit(rect->yl * (double)__lefConvertFactor)
                            , DbU::getUnit(rect->xh * (double)__lefConvertFactor)
                            , DbU::getUnit(rect->yh * (double)__lefConvertFactor)
                            )
                      );
          break;
        case lefiGeomRectIterE:
        // pas glop...
          break;
        case lefiGeomPolygonE:
        case lefiGeomPolygonIterE:
        // pas encore de polygones dans Hurricane
          cerr << "Polygons are not yet supported" << endl;
          abort ();
          break;
        case lefiGeomViaE:
        case lefiGeomViaIterE:
        // pas glop...
          break;
        default:
        // y'en a pas
          abort ();
          break;
      }
    }

    return 0;
  }

            
  int arrayBeginCB(lefrCallbackType_e type, const char* array_name, lefiUserData ud)
  {
    if (type != lefrArrayBeginCbkType)
      throw Error("arrayBeginCB: Invalid Callback");

    return 0;
  }


  int macroBeginCB(lefrCallbackType_e type, const char* macroName, lefiUserData ud)
  {
    if (type != lefrMacroBeginCbkType)
      throw Error("macroBeginCB: Invalid Callback");

    if (!macroName || !*macroName)
      return 1;

    string  cellName = macroName;
    cmess2 << "     - \"" << cellName << "\"..." << endl;

    __cell = Cell::create ( __library, cellName );
    __cell->setTerminal(false);
    
    Catalog::State* state = __catalog->getState ( cellName );
    if ( state == NULL )
      state = __catalog->getState ( cellName, true );
    // Should check here when a cell is redefined (from another library).

    __cell->put ( CatalogProperty::create(state) );
    state->setLogical  ( true );
    state->setPhysical ( true );
    state->setCell     ( __cell );

    return 0;
  }


  int macroCB(lefrCallbackType_e type, lefiMacro* macroInfo, lefiUserData ud)
  {
    if (type != lefrMacroCbkType)
      throw Error("macroCB: Invalid Callback");

    DbU::Unit originX;
    DbU::Unit originY;
    if (macroInfo->hasOrigin()) {
      originX = DbU::getUnit(macroInfo->originX() * (double)__lefConvertFactor);
      originY = DbU::getUnit(macroInfo->originY() * (double)__lefConvertFactor);
    }

    DbU::Unit sizeX;
    DbU::Unit sizeY;
    if (macroInfo->hasSize()) {
      sizeX = DbU::getUnit(macroInfo->sizeX() * (double)__lefConvertFactor);
      sizeY = DbU::getUnit(macroInfo->sizeY() * (double)__lefConvertFactor);
    }
    Box abox ( originX, originY, originX + sizeX, originY + sizeY );
    __cell->setAbutmentBox(abox);
    for_each_net(net, cell->getNets()) {
      net->setPosition(Point(abox.getHalfWidth(), abox.getHalfHeight()));
      end_for;
    }

    return 0;
  }


  int unitsCB(lefrCallbackType_e type, lefiUnits* units, lefiUserData ud)
  {
    if (type != lefrUnitsCbkType)
      throw Error("unitsCB: Invalid Callback");

    if (units->hasDatabase()) {
      __lefConvertFactor = (unsigned)units->databaseNumber();
      setLefConvertFactor(__technology, __lefConvertFactor);
    }

    return 0;
  }


  int siteCB(lefrCallbackType_e type, lefiSite* site, lefiUserData ud)
  {
    if (type != lefrSiteCbkType)
      throw Error("siteCB: Invalid Callback");

    assert(site->lefiSite::hasClass());
    assert(site->lefiSite::hasSize());
    /*BasicLayer::create(technology, Name(site->lefiSite::name()),
	    DbU::getUnit(site->lefiSite::sizeX()), DbU::getUnit(site->lefiSite::sizeX()));
    */
    //set_grid_step(getUnit(site->lefiSite::sizeX()));
    return 0;
  }


  int pinCB(lefrCallbackType_e type, lefiPin* pin, lefiUserData ud)
  {
    if (type != lefrPinCbkType)
      throw Error("pinCB: Invalid Callback");
    
    net = Net::create(cell, Name(pin->lefiPin::name()));
    net->setExternal(true);
    if (pin->hasUse()) {
      if (!strcmp(pin->use(), "POWER")) {
	    net->setGlobal(true);
        net->setType(Net::Type::POWER);
      } else if (!strcmp(pin->use(), "GROUND")) {
	    net->setGlobal(true);
        net->setType(Net::Type::GROUND);
      } else if (!strcmp(pin->use(), "CLOCK")) {
        net->setType(Net::Type::CLOCK);
      }
      else
	    net->setType(Net::Type::UNDEFINED);
    } else
      net->setType(Net::Type::LOGICAL);

    for (int prop = 0; prop < pin->numProperties(); prop++) {
      if (!strcmp(pin->propName(prop), "REGISTERPIN")) {
        setRegisterNet(*net);
      }
    }

    if (pin->hasDirection()) {
      if (!strcmp(pin->direction(), "INPUT"))
        net->setDirection(Net::Direction::IN);
      else if (!strcmp(pin->direction(), "OUTPUT"))
        net->setDirection(Net::Direction::OUT);
      else if (!strcmp(pin->direction(), "OUTPUT TRISTATE")) {
        net->setDirection(Net::Direction::TRISTATE);
      } else if (!strcmp(pin->direction(), "INOUT"))
        net->setDirection(Net::Direction::INOUT);
      else
        net->setDirection(Net::Direction::UNDEFINED);
    } else
      net->setDirection(Net::Direction::UNDEFINED);

    // la physique du pin
    for (int j=0; j<pin->numPorts(); j++) {
      lefiGeometries* geom = pin->port(j);
      Layer* layer = NULL;
      DbU::Unit width = 0;
      for (int i=0; i<geom->numItems(); i++) {
        switch (geom->itemType(i)) {
          case lefiGeomLayerE:
            {
              layer = technology->getLayer(geom->getLayer(i));
              if (!layer)
                throw Error("Unknown Layer: " + string(geom->getLayer(i)));
              BasicLayer* basicLayer = dynamic_cast<BasicLayer*>(layer);
              const Name* connectorLayerName = getConnectorLayerName(basicLayer);
              if (connectorLayerName)
                layer = technology->getLayer(*connectorLayerName);
              if (!layer)
                if (connectorLayerName)
                  throw Error("Unknown layer: " + connectorLayerName->_getString());
                else
                  throw Error("Unknow Layer: (null) Name");
              break;
            }
          case lefiGeomRectE:
            {
              lefiGeomRect* rect = geom->getRect(i);
              if (!layer) {
                throw Error("Unknown Layer: " + string(pin->lefiPin::name()));
              }
# ifdef  ONLY_PAD_PINS
              Pad* pad=Pad::create ( net
                                   , layer
                                   , Box ( DbU::getUnit(rect->xl * (double)__lefConvertFactor)
                                         , DbU::getUnit(rect->yl * (double)__lefConvertFactor)
                                         , DbU::getUnit(rect->xh * (double)__lefConvertFactor)
                                         , DbU::getUnit(rect->yh * (double)__lefConvertFactor) ) );
              setExternal(pad);
# else
              DbU::Unit width  = DbU::getUnit( (rect->xh - rect->xl) * (double)__lefConvertFactor);
              DbU::Unit height = DbU::getUnit( (rect->yh - rect->yl) * (double)__lefConvertFactor);
              DbU::Unit cx = DbU::getUnit(rect->xl * (double)__lefConvertFactor) + width/2;
              DbU::Unit cy = DbU::getUnit(rect->yl * (double)__lefConvertFactor) + height/2;
              if (width > height) {
                Horizontal* horizontal=Horizontal::create(net, layer, cy, height, DbU::getUnit(rect->xl * (double)__lefConvertFactor), DbU::getUnit(rect->xh * (double)__lefConvertFactor));
                setExternal(horizontal);
              } else {
                Vertical* vertical= Vertical::create(net, layer, cx, width, DbU::getUnit(rect->yl * (double)__lefConvertFactor), DbU::getUnit(rect->yh * (double)__lefConvertFactor));
                setExternal(vertical);
              }
# endif
            }
            break;
          case lefiGeomWidthE:
            width = DbU::getUnit(geom->getWidth(i) * (double)__lefConvertFactor);
            break;
          case lefiGeomPathE:
            {
              lefiGeomPath* path = geom->getPath(i);
              if (path->numPoints == 1) {
                Pad* pad=Pad::create( net
                                    , layer
                                    , Box( DbU::getUnit(path->x[0] * (double)__lefConvertFactor) - width/2
                                         , DbU::getUnit(path->y[0] * (double)__lefConvertFactor) - width/2
                                         , DbU::getUnit(path->x[0] * (double)__lefConvertFactor) + width/2
                                         , DbU::getUnit(path->y[0] * (double)__lefConvertFactor) + width/2)
                                    );
                        setExternal(pad);
                        break;
              }
              for (int point = 0 ; point < (path->numPoints - 1) ; point++) {
                if (path->x[point] == path->x[point + 1]) {
                // horizontal
                  Pad* pad=Pad::create ( net
                                       , layer
                                       , Box ( DbU::getUnit(path->x[point] * (double)__lefConvertFactor) - width/2
                                             , DbU::getUnit(path->y[point] * (double)__lefConvertFactor) - width/2
                                             , DbU::getUnit(path->x[point+1] * (double)__lefConvertFactor) + width/2
                                             , DbU::getUnit(path->y[point+1] * (double)__lefConvertFactor) + width/2
                                             )
                                       );
                  setExternal(pad);
                } else if (path->y[point] == path->y[point + 1]) {
                // vertical
                  Pad* pad=Pad::create ( net
                                       , layer
                                       , Box ( DbU::getUnit(path->x[point] * (double)__lefConvertFactor) - width/2
                                             , DbU::getUnit(path->y[point] * (double)__lefConvertFactor) - width/2
                                             , DbU::getUnit(path->x[point+1] * (double)__lefConvertFactor) + width/2
                                             , DbU::getUnit(path->y[point+1] * (double)__lefConvertFactor) + width/2
                                             )
                                       );
                  setExternal(pad);
                } else {
                // curiosité
                  cerr << "Invalid PATH..." << endl;
                  abort ();
                }
              }
            }
            break;
          default:
            cerr << "geometry not supported for pin ... "
                 << pin->lefiPin::name()
                 << " type is " << geom->itemType(i)
                 << endl;
            exit(1);
        }
      }
    }

    return 0;
  }

# endif




// -------------------------------------------------------------------
// Function  :  "lefParser()".

void  lefParser ( const string libPath, Library* library, Catalog& catalog )
{
  static int callNumber = 0;

# if HAVE_LEFDEF && defined(LEF_ENABLED)
  if ( !library )
    throw Error ( "lefParser() : Library argument is NULL." );

//cmess1 << "\n  o  Loading library \"" << lib->getName() << "\"." << endl;

  __library    = lib;
  __framework  = AllianceFrameWork::get ();
  __technology = DataBase::getDB()->getTechnology ();
  __catalog    = _framework->getCatalog ();

  lefrInit ();

  if ( !callNumber ) {
    lefrSetMacroBeginCbk  ( macroBeginCB );
    lefrSetMacroCbk       ( macroCB );
    lefrSetObstructionCbk ( macroObsCB );
    lefrSetPinCbk         ( pinCB );
  }

  IoFile  clibrary ( libPath );
  clibrary.open ( "r" );
  lefrRead ( clibrary.getFile(), getString(&clibrary).c_str(), (void*)userData );
  clibrary.close ();
# else // HAVE_LEFDEF
  cerr << Warning("Dummy LEF parser call for %s.",libPath.c_str()) << endl;
# endif // HAVE_LEFDEF

  callNumber++;
}




void  CParsLEFTechno ( string fileTechno )
{
  static int callNumber = 0;

# if HAVE_LEFDEF && defined(LEF_ENABLED)
  string  nameTechno = fileTechno;

  nameTechno.erase ( 0, nameTechno.find_last_of ("/")+1 );
  nameTechno.erase (    nameTechno.find_first_of("."), string::npos );

  assert ( nameTechno.size() > 1 );
            
  DataBase* db = DataBase::getDB();
  if ( !db )
    throw Error ( "CParsLEFTechno(): NULL DataBase." );

  __technology = db->getTechnology();
  if ( !__technology )
    __technology = Technology::create ( db, nameTechno );
  else
    __technology->setName ( nameTechno );

  lefrInit();
  if ( !callNumber ) {
    lefrSetUnitsCbk(unitsCB);
    lefrSetSiteCbk(siteCB);
    lefrSetArrayBeginCbk(arrayBeginCB);
  //lefrSetArrayCbk(arrayCB);
    lefrSetLayerCbk(layerCB);
  //lefrSetObstructionCbk(macroObsCB);
    lefrSetViaCbk(viaCB);
  }

  FILE* inLefFile = fopen ( fileTechno.c_str(), "r" );

  if ( inLefFile  == NULL )
    throw ( Error("CParsLEFTechno() : Can't open file %s", fileTechno.c_str()) );

  lefrRead(inLefFile, fileTechno.c_str(), (void*)(long)userData);
# else // HAVE_LEFDEF
  cerr << Warning("Dummy LEF technology parser call for \"%s\".",fileTechno.c_str())  << endl;
# endif // HAVE_LEFDEF

  callNumber++;
}


} // End of CRL namespace.
