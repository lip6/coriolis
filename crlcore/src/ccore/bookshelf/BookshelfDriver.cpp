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
//
// Date   : 08/11/2005
// Author : Damien Dupuis  <Damien.Dupuis@lip6.fr>
//
// Authors-Tag

#include <time.h>
using namespace std;

#include  "hurricane/Pin.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Instance.h"
#include  "hurricane/Net.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Warning.h"
using namespace Hurricane;


namespace {
    
struct PinSort {
    bool operator()(const Pin* pin1, const Pin* pin2) const
    {
        if (pin1->getY() > pin2->getY())
            return true;
        else if (pin2->getY() > pin1->getY())
            return false;
        else if (pin1->getX() > pin2->getX())
            return true;
        else
            return false;
    }
};

void DumpMasters ( ofstream& ccell, Cell *cell )
{
    unsigned i = 0;
    for_each_cell ( masterCell, cell->getSubCells() )
    {
        cerr << "found : " << masterCell << endl;
        i++;
        end_for;
    }

    cerr << "Dumped " << i << " masterCell" << endl;
}

void DumpSegments(ofstream& ccell, Cell* cell)
{
    for_each_net(net, cell->getNets())
    {
        for_each_component(component, net->getComponents())
        {
            if (Horizontal* horizontal = dynamic_cast<Horizontal*>(component))
            {
                DbU::Unit minX = (horizontal->getSourceX() < horizontal->getTargetX())?
                    horizontal->getSourceX() : horizontal->getTargetX();
                DbU::Unit maxX = (horizontal->getSourceX() > horizontal->getTargetX())?
                    horizontal->getSourceX() : horizontal->getTargetX();
                DbU::Unit y = horizontal->getY();
                DbU::Unit width = horizontal->getWidth();
                
                ccell << "S "
                    << minX << ","
                    << y << ","
                    << maxX << ","
                    << y << ","
                    << width << ","
                    << horizontal->getNet()->getName() << ",RIGHT," 
                    << horizontal->getLayer()->getName()
                    << endl;
            }
            else if (Vertical* vertical = dynamic_cast<Vertical*>(component))
            {
                DbU::Unit minY = (vertical->getSourceY() < vertical->getTargetY())?
                    vertical->getSourceY() : vertical->getTargetY();
                DbU::Unit maxY = (vertical->getSourceY() > vertical->getTargetY())?
                    vertical->getSourceY() : vertical->getTargetY();
                DbU::Unit x = vertical->getX();
                DbU::Unit width = vertical->getWidth();
                
                ccell << "S "
                    << x << ","
                    << minY << ","
                    << x << ","
                    << maxY << ","
                    << width << ","
                    << vertical->getNet()->getName() << ",LEFT,"
                    << vertical->getLayer()->getName()
                    << endl;
            }
            end_for;
        }
        end_for;
    }
}

unsigned getPinIndex(Name& pinname) {
    string pinNameString = getString(pinname);
    string::size_type pointString = pinNameString.find('.');
    if (pointString == string::npos) {
        return 0;
    } else {
        string indexString(pinNameString, pointString + 1, pinNameString.size() - 1);
        pinNameString = string(pinNameString, 0, pointString);
        pinname = Name(pinNameString);
        return atoi(indexString.c_str());
    }
}

void DumpPins(ofstream &ccell, Cell* cell) {
    for_each_net(net, cell->getExternalNets()) {
        typedef vector<Pin*> PinVector;
        PinVector pinVector;
        for_each_pin(pin, net->getPins()) {
            pinVector.push_back(pin);
            end_for;
        }
        sort (pinVector.begin(), pinVector.end(), PinSort()); 
        set<unsigned> indexesSet;
        for (PinVector::const_iterator pvit = pinVector.begin();
                pvit != pinVector.end();
                pvit++) {
            Pin* pin = *pvit;
            Name pinName(pin->getName());
            unsigned index = getPinIndex(pinName);
            if (pinName != net->getName()) {
                throw Error("Pin name (" + getString(pinName)
                        + ") must look like "
                        + getString(net->getName())
                        + "[.index] to be driven in AP format");
            }
            if (indexesSet.find(index) != indexesSet.end()) {
                throw Error("Two pins on same net with same index for net : "
                            + getString(net->getName()));
            }
            indexesSet.insert(index);
            if (pin->getWidth() != pin->getHeight()) {
                throw Warning(getString(pin->getName()) + " of "
                        + getString(net->getName())
                        + " will be incompletely saved ... : AP format is only able to save square pins ...");
            ccell << "C "
                << pin->getX()
                << "," << pin->getY()
                << "," << pin->getWidth()
                << "," << pinName
                << "," << index
                << ",";
            }
            switch (pin->getAccessDirection()) {
                case Pin::AccessDirection::NORTH:
                    ccell << "NORTH";
                    break;
                case Pin::AccessDirection::SOUTH:
                    ccell << "SOUTH";
                    break;
                case Pin::AccessDirection::EAST:
                    ccell << "EAST";
                    break;
                case Pin::AccessDirection::WEST:
                    ccell << "WEST";
                    break;
                case Pin::AccessDirection::UNDEFINED:
                default:
                    throw Error("Pins must a have direction to generate a .ap");
            }
            ccell << ",";
            ccell << pin->getLayer()->getName();
            ccell << endl;
        }
        end_for;
    }
}

void DumpDate(ofstream &ccell)
{
    time_t tim;
    time(&tim);
    struct tm* rest = localtime(&tim);
    ccell << rest->tm_mday
        << "/" << rest->tm_mon+1
        << "/" << rest->tm_year+1900;
}

void DumpInstances(ofstream &ccell, Cell* cell) {
    for_each_instance(instance, cell->getNotUnplacedInstances()) {
        ccell << "I "
            << instance->getAbutmentBox().getXMin()
            << "," << instance->getAbutmentBox().getYMin()
            << "," << instance->getMasterCell()->getName()
            << "," << instance->getName()
            << ",";

        const Transformation& transformation = instance->getTransformation();
        switch (transformation.getOrientation()) {
            case Transformation::Orientation::ID:
                ccell << "NOSYM";
                break;
            case Transformation::Orientation::R1:
                ccell << "ROT_P";
                break;
            case Transformation::Orientation::R2:
                ccell << "SYMXY";
                break;
            case Transformation::Orientation::R3:
                ccell << "ROT_M";
                break;
            case Transformation::Orientation::MX:
                ccell << "SYM_X"; 
                break;
            case Transformation::Orientation::XR:
                ccell << "SY_RM";
                break;
            case Transformation::Orientation::MY:
                ccell << "SYM_Y"; 
                break;
            case Transformation::Orientation::YR:
                ccell << "SY_RP";
                break;
            default:
                throw Error("Unrecognized orientation in transformation");
        }
        ccell << endl;
        end_for;
    }
}

}

namespace CRL {
    
void  bookshelfDriver( const string& cellPath, Cell *cell, unsigned int &saveState)
{
    ::std::ofstream auxFile ( cellPath.c_str() );

    // .aux file generation :
    string str_name = getString ( cell->getName() ) + ".test";
    auxFile << "STDCellPlacement :  " 
            << str_name << ".masters  "
            << str_name << ".nodes  "
            << str_name << ".nets  "
            << str_name << ".pl" << endl;

    //DumpMasters ( ccell, cell );

    //ccell << "UCLA nodes 1.0" << endl;
    
    //DumpDate(ccell);
    //ccell << "," << scaleX << endl;
    //const Box& abutmentBox = cell->getAbutmentBox();
    //ccell << "A "
    //    << abutmentBox.getXMin()
    //    << "," << abutmentBox.getYMin()
    //    << "," << abutmentBox.getXMax()
    //    << "," << abutmentBox.getYMax()
    //    << endl;
    //DumpPins(ccell, cell);
    //DumpInstances(ccell, cell);
    //DumpSegments(ccell, cell);
    //DumpContacts(ccell, cell);
    //ccell << "EOF" << endl;
    auxFile.close ();
}

}
