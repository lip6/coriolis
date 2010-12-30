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

#include "hurricane/NetExternalComponents.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/HyperNet.h"
#include "hurricane/UpdateSession.h"

#include "crlcore/Hierarchy.h"
#include "crlcore/ToolBox.h"


namespace CRL {

  using namespace Hurricane;


void createRoutingPadsRing(Cell* top_cell)
// ***************************************
{
    //ltrace(61) << "Creating Routing Pads and Pins Ring for top cell " << top_cell << endl; ltracein(61);
    for_each_occurrence(cellOccurrence,getAllUniqueCellOccurrencesFrom(top_cell))
    {
        Cell* cell=static_cast<Cell*>(cellOccurrence.getEntity());
        //ltrace(60) << "Creating Routing Pads and Pins Ring for " << cell << endl;ltracein(60);

        for_each_net ( net, cell->getNets() )
        {
            //getting the net occurrence relative to the top-cell
            //to check if it is the root of an hypernet
            Occurrence netOccurrence ( net, cellOccurrence.getPath() );
            if ( !isHyperNetRootNetOccurrence(netOccurrence) )
                continue;

            //ltrace(59) << "Creating Routing Pads for " << net << endl;ltracein(59);
            
            //getting the net occurrence relative to its cell (null Path)
            //in order to have plug occurrences relative to the RoutingPads-to-be
            netOccurrence = Occurrence(net);
            for_each_occurrence ( plugOccurrence, HyperNet(netOccurrence).getLeafPlugOccurrences() )
            {
                //ltrace(58) << "Creating Routing Pad " << plugOccurrence << endl;ltracein(58);
                RoutingPad::create ( net, plugOccurrence );
                //ltraceout(58);
                end_for;
            }
    
            Component* previousComponent=NULL;
            for_each_component ( component, net->getComponents() )
            {
                if ( dynamic_cast<Plug*>(component) )
                {
                    if ( component->getBodyHook()->isAttached() )
                        component->getBodyHook()->detach();
                }
                else if ( dynamic_cast<RoutingPad*>(component) )
                {
                    if (previousComponent)
                        component->getBodyHook()->attach(previousComponent->getBodyHook());
                    previousComponent=component;
                }
                end_for;
            }

            //ltraceout(59);
            
            end_for;
        }
        
        //ltraceout(60);
        
        end_for;
    }
    
    //ltraceout(61);
}




void createRoutingPadsAndPinsRing ( Cell* top_cell )
// *************************************************
{
    //ltrace(61) << "Creating Routing Pads and Pins Ring for top cell " << top_cell << endl; ltracein(61);
    UpdateSession::open();
    for_each_occurrence(cellOccurrence,getAllUniqueCellOccurrencesFrom(top_cell))
    {
        Cell* cell=static_cast<Cell*>(cellOccurrence.getEntity());
        //ltrace(60) << "Creating Routing Pads and Pins Ring for " << cell << endl;ltracein(60);

        for_each_net ( net, cell->getNets() )
        {
            //getting the net occurrence relative to the top-cell
            //to check if it is the root of an hypernet
            Occurrence netOccurrence ( net, cellOccurrence.getPath() );
            if ( !isHyperNetRootNetOccurrence(netOccurrence) )
                continue;

            //ltrace(59) << "Creating Routing Pads for " << net << endl;ltracein(59);
            
            //getting the net occurrence relative to its cell (null Path)
            //in order to have plug occurrences relative to the RoutingPads-to-be
            netOccurrence = Occurrence(net);
            for_each_occurrence ( plugOccurrence, HyperNet(netOccurrence).getLeafPlugOccurrences() )
            {
                //ltrace(58) << "Creating Routing Pad " << plugOccurrence << endl;ltracein(58);
                RoutingPad::create ( net, plugOccurrence );
                //ltraceout(58);
                end_for;
            }
    
            Component* previousComponent=NULL;
            for_each_component ( component, net->getComponents() )
            {
                if ( dynamic_cast<Plug*>(component) )
                {
                    //ltrace(57) << "detach a plug " << component << endl;
                    if ( component->getBodyHook()->isAttached() )
                        component->getBodyHook()->detach();
                }
                else if ( dynamic_cast<Pin*>(component) )
                {
                    //ltrace(57) << "detach / attach a pin " << component << endl;
                    if ( component->getBodyHook()->isAttached() )
                        component->getBodyHook()->detach();
                    if (previousComponent)
                        component->getBodyHook()->attach(previousComponent->getBodyHook());
                    previousComponent=component;
                }
                else if ( dynamic_cast<RoutingPad*>(component) )
                {
                    //ltrace(57) << "attach a routingPad " << component << endl;
                    if (previousComponent)
                        component->getBodyHook()->attach(previousComponent->getBodyHook());
                    previousComponent=component;
                }
                end_for;
            }

            //ltraceout(59);
            
            end_for;
        }
        
        //ltraceout(60);
        
        end_for;
    }
    
    UpdateSession::close();
    //ltraceout(61);
}




void createPlacedRoutingPadsAndPinsRing ( Cell* top_cell )
// *******************************************************
{
    //ltrace(61) << "Creating Routing Pads and Pins Ring for top cell " << top_cell << endl; ltracein(61);
    UpdateSession::open();
    for_each_occurrence(cellOccurrence,getAllUniqueCellOccurrencesFrom(top_cell))
    {
        Cell* cell = static_cast<Cell*>(cellOccurrence.getEntity());
        //ltrace(60) << "Creating Routing Pads and Pins Ring for " << cell << endl;ltracein(60);

        for_each_net ( net, cell->getNets() )
        {
            //getting the net occurrence relative to the top-cell
            //to check if it is the root of an hypernet
            Occurrence netOccurrence ( net, cellOccurrence.getPath() );
            if ( !isHyperNetRootNetOccurrence(netOccurrence) )
                continue;

            //ltrace(59) << "Creating Routing Pads for " << net << endl;ltracein(59);
            
            //getting the net occurrence relative to its cell (null Path)
            //in order to have plug occurrences relative to the RoutingPads-to-be
            netOccurrence = Occurrence(net);
            for_each_occurrence ( plugOccurrence, HyperNet(netOccurrence).getLeafPlugOccurrences() )
            {
                ltrace(58) << "Creating Routing Pad " << plugOccurrence << endl;ltracein(58);
                cerr << RoutingPad::create ( net, plugOccurrence, RoutingPad::BiggestArea ) << endl;
                //ltraceout(58);
                end_for;
            }
            vector<Pin*> pins;
            for_each_pin ( pin, net->getPins() ) {
                pins.push_back ( pin );
                end_for
            }
            for ( unsigned i = 0 ; i < pins.size() ; i++ ) {
              RoutingPad::create ( pins[i] );
            }
    
            Component* previousComponent=NULL;
            for_each_component ( component, net->getComponents() )
            {
                if ( dynamic_cast<Plug*>(component) )
                {
                    //ltrace(57) << "detach a plug " << component << endl;
                    if ( component->getBodyHook()->isAttached() )
                        component->getBodyHook()->detach();
                }
                else if ( dynamic_cast<Pin*>(component) )
                {
                    //ltrace(57) << "detach / attach a pin " << component << endl;
                    if ( component->getBodyHook()->isAttached() )
                        component->getBodyHook()->detach();
                    //if (previousComponent)
                    //    component->getBodyHook()->attach(previousComponent->getBodyHook());
                    //previousComponent=component;
                }
                else if ( dynamic_cast<RoutingPad*>(component) )
                {
                    //ltrace(57) << "attach a routingPad " << component << endl;
                    if (previousComponent)
                        component->getBodyHook()->attach(previousComponent->getBodyHook());
                    previousComponent=component;
                }
                end_for;
            }

            //ltraceout(59);
            
            end_for;
        }
        
        //ltraceout(60);
        
        end_for;
    }
    
    UpdateSession::close();
    //ltraceout(61);
}


} //namespace CRL

