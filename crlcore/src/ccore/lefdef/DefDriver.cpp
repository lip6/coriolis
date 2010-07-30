

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

// -*- C++ -*-
//
// $Id: CDrivDEF.cpp,v 1.15 2006/02/23 14:13:53 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                Christophe Alexandre              |
// |  E-mail      :        Christophe.Alexandre@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./CDrivDEF.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# include  <iostream>
# include  <functional>

# include  "hurricane/SharedName.h"
# include  "hurricane/DataBase.h"
# include  "hurricane/Technology.h"
# include  "hurricane/Warning.h"
# include  "hurricane/Cell.h"
# include  "hurricane/Instance.h"
# include  "hurricane/Net.h"
# include  "hurricane/Plug.h"
# include  "hurricane/Segment.h"
# include  "hurricane/Segments.h"
# include  "hurricane/Vertical.h"
# include  "hurricane/Verticals.h"
# include  "hurricane/Horizontal.h"
# include  "hurricane/Horizontals.h"

# include  "hurricane/DataBase.h"
# include  "LefDef.h"

# if HAVE_LEDEF
#   include  "hurricane/defwWriter.hpp"
# endif




namespace CRL {


  using namespace std;


// x-----------------------------------------------------------------x
// |                       Local Definitions                         |
// x-----------------------------------------------------------------x


# if HAVE_LEFDEF && defined(LEF_ENABLED)
#define CHECK_STATUS(status)		\
	if (status) {			\
	    defwPrintError(status);	\
	    exit(status);		\
	}



typedef  list<Net*>  NetList;


struct LessNet : public binary_function<Net*,Net*,bool> {
    bool operator () ( Net* net1, Net* net2 );
};




// Painstakingly slow, but quick to write.
// Implement net1 < net2 (strictly inferior).
bool  LessNet::operator () ( Net* net1, Net* net2 ) {
  if ( net1->GetName() == net2->GetName() ) return false;

  const string& name1 = (net1->GetName()._GetSharedName())->_GetSString ();
  const string& name2 = (net2->GetName()._GetSharedName())->_GetSString ();

  size_t length1  = name1.size ();
  size_t length2  = name2.size ();
  size_t index1   = 0;
  size_t index2   = 0;
  int    number1  = 0;
  int    number2  = 0;

  while ( true ) {
    if ( isdigit(name1[index1]) ) {
      for ( number1 = 0 ; isdigit(name1[index1]) && (index1<length1) ; index1++ )
        number1 = ( number1 * 10 ) + (int)( name1[index1] - '0' );

      if ( isdigit(name2[index2]) ) {
        for ( number2 = 0 ; isdigit(name2[index2]) && (index2<length2) ; index2++ )
          number2 = ( number2 * 10 ) + (int)( name2[index2] - '0' );
      } else
        return true;

      if ( number1 == number2 ) continue;
      if ( number1 <  number2 ) return true;
      return false;
    }

    if ( isdigit(name2[index2]) ) return true;
    
    if ( name1[index1] == name2[index2] ) {
      index1++;
      index2++;
      if ( (index1<length1) && (index2<length2) )
        continue;

      if (index1>=length1) return true;
      return false;
    }

    if ( name1[index1] < name2[index2] ) return true;

    break;
  }

  return false;
}

# endif // HAVE_LEFDEF




void  defDriver ( const string cellPath, Cell *cell, unsigned int &saveState )
{
# if HAVE_LEFDEF && defined(LEF_ENABLED)
    CDataBase* dataBase = GetCDataBase();
    if ( dataBase == NULL )
      throw CError ( NullDataBase, "CDrivDEF()" );

    saveState |= CCatal::State::VIEWS;


    int   status;
    IOFILE  ccell ( cellPath );
    ccell.Open ( "w" );
    FILE* fileDefOut = ccell.GetFile ();

    unsigned defConvertFactor = GetDefConvertFactor(*cell);
    unsigned lefConvertFactor = GetLefConvertFactor(*(dataBase->GetTechnology()));
    status = defwInit(fileDefOut, 5, 2, "ON", ":", "[]"
            , GetString ( cell->GetName() ).c_str ()
            , GetString(dataBase->GetTechnology()->GetName()).c_str()
            , NULL, NULL, 
            defConvertFactor);
    CHECK_STATUS(status);
    Box abutmentBox(cell->GetAbutmentBox());
    if (!abutmentBox.IsEmpty())
    {
        int xMin = (int)GetValue(abutmentBox.GetXMin()) * defConvertFactor / lefConvertFactor;
        int yMin = (int)GetValue(abutmentBox.GetYMin()) * defConvertFactor / lefConvertFactor;
        int xMax = (int)GetValue(abutmentBox.GetXMax()) * defConvertFactor / lefConvertFactor;
        int yMax = (int)GetValue(abutmentBox.GetYMax()) * defConvertFactor / lefConvertFactor;
        status = defwDieArea(xMin, yMin, xMax, yMax);
        CHECK_STATUS(status);
    }
    status = defwStartComponents(cell->GetInstances().GetSize());
    CHECK_STATUS(status);

    for_each_instance(instance, cell->GetInstances())
    {
        int x = (int)GetValue(instance->GetAbutmentBox().GetXMin()) * defConvertFactor / lefConvertFactor; 
        int y = (int)GetValue(instance->GetAbutmentBox().GetYMin()) * defConvertFactor / lefConvertFactor;
        char componentStatus[128];


        switch (instance->GetPlacementStatus())
        {
            case Instance::PlacementStatus::UNPLACED:
                strcpy(componentStatus, "UNPLACED");
                break;
            case Instance::PlacementStatus::PLACED:
                strcpy(componentStatus, "PLACED");
                break;
            case Instance::PlacementStatus::FIXED:
                strcpy(componentStatus, "FIXED");
		break;
            default:
                throw Error ("Unknown instance placement status");
        }
        status = defwComponent(GetString(instance->GetName()).c_str(),
            GetString(instance->GetMasterCell()->GetName()).c_str(),
            0, NULL, NULL, NULL, NULL, NULL,
            0, NULL, NULL, NULL, NULL,
            componentStatus,
            x, y, 0, 0.0, NULL,
            0, 0, 0, 0);
        CHECK_STATUS(status);
        end_for;
    }
    status = defwEndComponents();
    CHECK_STATUS(status);
    
    //PINS
    // le nombre de pins est egal au nombre de pins de la cellule
    unsigned numPins = cell->GetPins().GetSize();
    // + le nombre de nets externes et les nets POWER/GROUND/CLOCK sans pins :
    for_each_net ( net, cell->GetNets() )
    {
        if (  ( ( net->GetType() == Net::Type::POWER ) || ( net->GetType() == Net::Type::GROUND ) || ( net->IsExternal() ) )
           && ( net->GetPins().GetSize() == 0 ) )
            numPins++;
        end_for
    }
    status = defwStartPins(numPins);
    CHECK_STATUS(status);

    for_each_pin( pin, cell->GetPins() )
    {
        Net *net = pin->GetNet();
        if ( ! net->IsGlobal() &&  ! net->IsExternal() )
            throw Warning ( "Pin on non global and non external net : " + GetString ( net) );
        string dir;
        string use;
        string place;
        string layer;
        const char* pinName = GetString ( pin->GetName() ).c_str();
        const char* netName = GetString ( net->GetName() ).c_str();

        switch ( net->GetDirection() )
        {
            case Net::Direction::UNDEFINED:
                break;
            case Net::Direction::IN:
                dir = string ( "INPUT" );
                break;
            case Net::Direction::OUT:
                dir  = string ( "OUTPUT" );
                break;
            case Net::Direction::INOUT:
                dir = string ( "INOUT" );
                break;
            default:
                throw Error ( "Wrong net Direction" );
        }

        switch ( net->GetType() )
        {
            case Net::Type::UNDEFINED:
                break;
            case Net::Type::LOGICAL:
                use = string ( "SIGNAL" );
                break;
            case Net::Type::POWER:
                use = string ( "POWER" );
                break;
            case Net::Type::GROUND:
                use = string ( "GROUND" );
                break;
            case Net::Type::CLOCK:
                use = string ( "CLOCK" );
                break;
            default:
                throw Error ( "Wrong net Type" );
        }

        switch ( pin->GetPlacementStatus() )
        {
            case Pin::PlacementStatus::UNPLACED:
                break;
            case Pin::PlacementStatus::PLACED:
                place = string ( "PLACED" );
                layer = GetString ( pin->GetLayer()->GetName() );
                break;
            case Pin::PlacementStatus::FIXED:
                place = string ( "FIXED" );
                layer = GetString ( pin->GetLayer()->GetName() );
                break;
            default:
                throw Error ( "Wrong pin PlacementStatus" );
        }
        
        status = defwPin ( pinName, netName,
               ( net->IsGlobal() )                                             ?    1 : 0,
               ( net->GetDirection()       == Net::Direction::UNDEFINED )      ? NULL : dir.c_str(),
               ( net->GetType()            == Net::Type::UNDEFINED )           ? NULL : use.c_str(),
               ( pin->GetPlacementStatus() == Pin::PlacementStatus::UNPLACED ) ? NULL : place.c_str(),
               ( pin->GetPlacementStatus() == Pin::PlacementStatus::UNPLACED ) ?    0 : (int) pin->GetX(),
               ( pin->GetPlacementStatus() == Pin::PlacementStatus::UNPLACED ) ?    0 : (int) pin->GetY(),
               0,
               ( pin->GetPlacementStatus() == Pin::PlacementStatus::UNPLACED ) ? NULL : layer.c_str(),
               ( pin->GetPlacementStatus() == Pin::PlacementStatus::UNPLACED ) ?    0 : - (int) pin->GetHalfWidth(),
               ( pin->GetPlacementStatus() == Pin::PlacementStatus::UNPLACED ) ?    0 : - (int) pin->GetHalfHeight(),
               ( pin->GetPlacementStatus() == Pin::PlacementStatus::UNPLACED ) ?    0 :   (int) pin->GetHalfWidth(),
               ( pin->GetPlacementStatus() == Pin::PlacementStatus::UNPLACED ) ?    0 :   (int) pin->GetHalfHeight()
                         );
        CHECK_STATUS(status);
        
        end_for
    }

    for_each_net ( net, cell->GetNets() )
    {
        if ( ( net->GetType() == Net::Type::POWER ) && ( net->GetPins().GetSize() == 0 ) )
        {
            string dir;
            string use;
            const char* pinName = GetString ( GetPort(*net) ).c_str();
            const char* netName = GetString ( net->GetName() ).c_str();
            switch ( net->GetDirection() )
            {
                case Net::Direction::UNDEFINED:
                    break;
                case Net::Direction::IN:
                    dir = string ( "INPUT" );
                    break;
                case Net::Direction::OUT:
                    dir = string ( "OUTPUT" );
                    break;
                case Net::Direction::INOUT:
                    dir = string ( "INOUT" );
                    break;
                default:
                    throw Error ( "Wrong net direction" );
            }
            use = string ( "POWER" );
            status = defwPin ( pinName, netName,
                net->IsGlobal() ? 1 : 0, ( net->GetDirection() == Net::Direction::UNDEFINED ) ? NULL : dir.c_str(),
                use.c_str(), NULL, 0, 0, 0, NULL, 0, 0, 0, 0);
            CHECK_STATUS(status);
        }
        else if ( ( net->GetType() == Net::Type::GROUND ) && ( net->GetPins().GetSize() == 0 ) )
        {
            string dir;
            string use;
            const char* pinName = GetString ( GetPort(*net) ).c_str();
            const char* netName = GetString ( net->GetName() ).c_str();
            switch ( net->GetDirection() )
            {
                case Net::Direction::UNDEFINED:
                    break;
                case Net::Direction::IN:
                    dir = string ( "INPUT" );
                    break;
                case Net::Direction::OUT:
                    dir = string ( "OUTPUT" );
                    break;
                case Net::Direction::INOUT:
                    dir = string ( "INOUT" );
                    break;
                default:
                    throw Error ( "Wrong net direction" );
            }
            use = string ( "GROUND" );
            status = defwPin ( pinName, netName,
                net->IsGlobal() ? 1 : 0, ( net->GetDirection() == Net::Direction::UNDEFINED ) ? NULL : dir.c_str(),
                use.c_str(), NULL, 0, 0, 0, NULL, 0, 0, 0, 0);
            CHECK_STATUS(status);
        }
        else if ( net->IsExternal() && ( net->GetPins().GetSize() == 0 ) )
        {
            string dir;
            string use;
            const char* pinName = GetString ( GetPort(*net) ).c_str();
            const char* netName = GetString ( net->GetName() ).c_str();
            switch ( net->GetDirection() )
            {
                case Net::Direction::UNDEFINED:
                    break;
                case Net::Direction::IN:
                    dir = string ( "INPUT" );
                    break;
                case Net::Direction::OUT:
                    dir = string ( "OUTPUT" );
                    break;
                case Net::Direction::INOUT:
                    dir = string ( "INOUT" );
                    break;
                default:
                    throw Error ( "Wrong net direction" );
            }
            switch ( net->GetType() )
            {
                case Net::Type::UNDEFINED:
                    break;
                case Net::Type::LOGICAL:
                    use = string ( "SIGNAL" );
                    break;
                case Net::Type::CLOCK:
                    use = string ( "CLOCK" );
                    break;
                default:
                    throw Error ( "Wrong type or shouldn't be that type" );
            }
            status = defwPin ( pinName, netName,
                net->IsGlobal() ? 1 : 0, ( net->GetDirection() == Net::Direction::UNDEFINED ) ? NULL : dir.c_str(),
                ( net->GetType() == Net::Type::UNDEFINED ) ? NULL : use.c_str(), NULL, 0, 0, 0, NULL, 0, 0, 0, 0);
            CHECK_STATUS(status);
        }
        end_for
    }
    
    status = defwEndPins();
    CHECK_STATUS(status);
    
    unsigned netCpt = 0;
    unsigned specialNetCpt = 0;
    for_each_net(net, cell->GetNets())
    {
        if (net->IsGlobal()) 
        {
            if (net->IsSupply())
                ++specialNetCpt;
            else
            {
                cerr << "Strange Net:" << net
                    << "Global net but no type ...."
                    << endl;
                cerr << net->GetType() << endl;
            }
        }
        else
        {
            if (net->IsClock())
                ++specialNetCpt;
            else
                ++netCpt;
        }
        end_for;
    }

    if (specialNetCpt > 0)
    {
        status = defwStartSpecialNets(specialNetCpt);
        CHECK_STATUS(status);
        for_each_net(net, cell->GetNets())
        {
            if (net->IsClock())
            {
                status = defwSpecialNet(GetString(net->GetName()).c_str());
                CHECK_STATUS(status);
                for_each_plug(plug, net->GetPlugs())
                {
                    Instance* instance = plug->GetInstance();
                    Net* masterNet = plug->GetMasterNet();
                    const char* instanceName  = GetString(instance->GetName()).c_str();
                    status = defwSpecialNetConnection(instanceName,
                            GetString(masterNet->GetName()).c_str(), 0);
                    CHECK_STATUS(status);
                    end_for;
                }
                if (!net->GetSegments().IsEmpty())
                {
                    status = defwSpecialNetPathStart("ROUTED");
                    CHECK_STATUS(status);
                    Segments segments = net->GetSegments();
                    SegmentLocator segmentLocator = segments.GetLocator();
                    Segment* segment = segmentLocator.GetElement();
                    status =
                        defwSpecialNetPathLayer(GetString(segment->GetLayer()->GetName()).c_str());
                    CHECK_STATUS(status);
                    const char* coorX[2];
                    const char* coorY[2];
                    coorX[0] = strdup((GetString(segment->GetSourceX())).c_str());
                    coorY[0] = strdup((GetString(segment->GetSourceY())).c_str());
                    coorX[1] = strdup((GetString(segment->GetTargetX())).c_str());
                    coorY[1] = strdup((GetString(segment->GetTargetY())).c_str());
                    status = defwSpecialNetPathPoint(2, coorX, coorY); 
                    CHECK_STATUS(status);
                    free((char*)coorX[0]);
                    free((char*)coorX[1]);
                    free((char*)coorY[0]);
                    free((char*)coorY[1]);
                    status = defwSpecialNetPathEnd();
                    CHECK_STATUS(status);
                    segmentLocator.Progress();
                    while (segmentLocator.IsValid())
                    {
                        status = defwSpecialNetPathStart("ROUTED");
                        CHECK_STATUS(status);
                        Segment* segment = segmentLocator.GetElement();
                        status =
                            defwSpecialNetPathLayer(GetString(segment->GetLayer()->GetName()).c_str());
                        const char* coorX[2];
                        const char* coorY[2];
                        coorX[0] = strdup((GetString(segment->GetSourceX())).c_str());
                        coorY[0] = strdup((GetString(segment->GetSourceY())).c_str());
                        coorX[1] = strdup((GetString(segment->GetTargetX())).c_str());
                        coorY[1] = strdup((GetString(segment->GetTargetY())).c_str());
                        status = defwSpecialNetPathPoint(2, coorX, coorY); 
                        CHECK_STATUS(status);
                        free((char*)coorX[0]);
                        free((char*)coorX[1]);
                        free((char*)coorY[0]);
                        free((char*)coorY[1]);
                        status = defwSpecialNetPathEnd();
                        CHECK_STATUS(status);
                        segmentLocator.Progress();
                    }
                }
                status = defwSpecialNetUse("CLOCK");
                CHECK_STATUS(status);
                status = defwSpecialNetEndOneNet();
                CHECK_STATUS(status);
            }
            if (net->IsGlobal() && (net->IsSupply()))
            {
                status = defwSpecialNet(GetString(net->GetName()).c_str());
                CHECK_STATUS(status);
                for_each_instance(instance, cell->GetInstances())
                {
                    Cell* model = instance->GetMasterCell();
                    for_each_net(modelnet, model->GetExternalNets())
                    {
                        if (modelnet->IsGlobal() &&
                                ((modelnet->IsGround() && net->IsGround())
                                 || (modelnet->IsPower() && net->IsPower()))
                           )
                        {
                            status = defwSpecialNetConnection(
                                GetString(instance->GetName()).c_str(),
                                GetString(modelnet->GetName()).c_str(),
                                0);
                            CHECK_STATUS(status);
                        }
                        end_for;
                    }
                end_for;
                }
                if (net->IsGround())
                    status = defwSpecialNetUse("GROUND");
                else if (net->IsPower())
                    status = defwSpecialNetUse("POWER");
                else
                    throw Error("No Supply type");
                CHECK_STATUS(status);
                status = defwSpecialNetEndOneNet();
                CHECK_STATUS(status);
            }
            end_for;
        }
        status = defwEndSpecialNets();
        CHECK_STATUS(status);
    }
    status = defwStartNets(netCpt);
    CHECK_STATUS(status);
    for_each_net(net, cell->GetNets())
    {
        if (!net->IsGlobal() && !net->IsClock())
        {
            status = defwNet(GetString(net->GetName()).c_str());
            CHECK_STATUS(status);
            for_each_plug(plug, net->GetPlugs())
            {
                Instance* instance = plug->GetInstance();
                Net* masterNet = plug->GetMasterNet();
                const char* instanceName  = GetString(instance->GetName()).c_str();
                status = defwNetConnection(instanceName,
                    GetString(masterNet->GetName()).c_str(), 0);
                CHECK_STATUS(status);
                end_for;
            }
            if (!net->GetSegments().IsEmpty())
            {
                status = defwNetPathStart("ROUTED");
                CHECK_STATUS(status);
                Segments segments = net->GetSegments();
                SegmentLocator segmentLocator = segments.GetLocator();
                Segment* segment = segmentLocator.GetElement();
                status =
                    defwNetPathLayer(GetString(segment->GetLayer()->GetName()).c_str(), 1, NULL);
                CHECK_STATUS(status);
                const char* coorX[2];
                const char* coorY[2];
                const char* coorValue[2];
                coorX[0] = strdup((GetString(segment->GetSourceX())).c_str());
                coorY[0] = strdup((GetString(segment->GetSourceY())).c_str());
                coorValue[0] = NULL;
                coorX[1] = strdup((GetString(segment->GetTargetX())).c_str());
                coorY[1] = strdup((GetString(segment->GetTargetY())).c_str());
                coorValue[1] = NULL;
                status = defwNetPathPoint(2, coorX, coorY, coorValue); 
                CHECK_STATUS(status);
                free((char*)coorX[0]);
                free((char*)coorX[1]);
                free((char*)coorY[0]);
                free((char*)coorY[1]);
                status = defwNetPathEnd();
                CHECK_STATUS(status);
                segmentLocator.Progress();
                while (segmentLocator.IsValid())
                {
                    status = defwNetPathStart("ROUTED");
                    CHECK_STATUS(status);
                    Segment* segment = segmentLocator.GetElement();
                    status =
                        defwNetPathLayer(GetString(segment->GetLayer()->GetName()).c_str(), 1, NULL);
                    const char* coorX[2];
                    const char* coorY[2];
                    const char* coorValue[2];
                    coorX[0] = strdup((GetString(segment->GetSourceX())).c_str());
                    coorY[0] = strdup((GetString(segment->GetSourceY())).c_str());
                    coorValue[0] = NULL;
                    coorX[1] = strdup((GetString(segment->GetTargetX())).c_str());
                    coorY[1] = strdup((GetString(segment->GetTargetY())).c_str());
                    coorValue[1] = NULL;
                    status = defwNetPathPoint(2, coorX, coorY, coorValue); 
                    CHECK_STATUS(status);
                    free((char*)coorX[0]);
                    free((char*)coorX[1]);
                    free((char*)coorY[0]);
                    free((char*)coorY[1]);
                    status = defwNetPathEnd();
                    CHECK_STATUS(status);
                    segmentLocator.Progress();
                }
            }
            status = defwNetEndOneNet();
            CHECK_STATUS(status);
        }
        end_for;
    }
    status = defwEndNets();
    CHECK_STATUS(status);
    
    status = defwEnd();
    CHECK_STATUS(status);

    ccell.Close ();
# else // HAVE_LEFDEF
    cerr << "\nDummy LEF driver call for \"" << cellPath << "\"." << endl;
# endif // HAVE_LEFDEF
}


} // End of CRL namespace.

