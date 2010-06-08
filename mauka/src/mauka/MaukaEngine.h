
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
// Date   : 19/07/2006
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 

#ifndef __MAUKA_H
#define __MAUKA_H

#include "hurricane/Instance.h"
#include "crlcore/ToolEngine.h"
#include "nimbus/GCell.h"

#include "mauka/Configuration.h"


namespace Mauka {

using Hurricane::Record;
using Hurricane::Name;
using Hurricane::DbU;
using Hurricane::Box;
using Hurricane::Net;
using Hurricane::Cell;
using Hurricane::Instance;
using Hurricane::Occurrence;
using CRL::ToolEngine;
using Nimbus::GCell;

class Surface;
class SimAnnealingPlacer;
class BBPlacer;

class MaukaEngine: public ToolEngine 
// *********************************
{
    //Mauka: a cool, light, Hawaiian wind descending from the montains. 
    friend class Surface;
    friend class SimAnnealingPlacer;
    friend class Move;
    friend class Bin;
    friend class SubRow;
    friend class BBPlacer;
// Types
// *****
    public: typedef ToolEngine Inherit;
    public: typedef std::vector<Occurrence> InstanceOccurrencesVector;
    public: typedef std::map<Occurrence, unsigned> InstanceOccurrencesMap;
    public: typedef std::vector<unsigned> UVector;
    public: typedef std::list<unsigned> UList;
    public: typedef std::vector<UVector> UTable;
    public: typedef std::vector<bool> BVector;
    public: typedef std::vector<DbU::Unit> UnitVector;
    public: typedef std::vector<Net*> NetVector;
    public: typedef std::vector<Box> BoxVector;
    public: typedef std::vector<BoxVector> BBoxes;
    public: typedef std::vector< std::vector<double> > Costs; 
    public: typedef std::vector<bool> PrePlaceRow;
    public: typedef std::vector<PrePlaceRow> PrePlaceTab;
    
// Attributes
// **********
    private: static Name                _toolName;
    private: Configuration*             _configuration;
    private: InstanceOccurrencesVector  _instanceOccurrencesVector;
    private: InstanceOccurrencesMap     _instanceOccurrencesMap;
    private: UnitVector                 _instanceWidths;
    private: UTable                     _instanceNets;
    private: NetVector                  _nets;
    private: UTable                     _netInstances;
    private: UVector                    _netInitX;
    private: UVector                    _netInitY;
    private: BVector                    _hasInitX;
    private: BVector                    _hasInitY;
    private: Surface*                   _surface;
    private: SimAnnealingPlacer*        _simAnnealingPlacer;
    private: BBPlacer*                  _bbPlacer;
             

// Constructor
// ***********
    private: MaukaEngine(Cell* cell);
    public: static MaukaEngine* create(Cell* cell, Box placementbox = Box());
            
// Accessors
// *********
  public:
    static  MaukaEngine*   get                           (Cell*);
    static  const Name&    staticGetName                 ();
    virtual const Name&    getName                       () const;
    inline  Configuration* getConfiguration              () const { return _configuration; }
    inline  bool           useStandardSimulatedAnnealing () const { return _configuration->useStandardSimulatedAnnealing(); }
    inline  bool           doIgnorePins                  () const { return _configuration->doIgnorePins(); }
    inline  bool           doPlotBins                    () const { return _configuration->doPlotBins(); }
    inline  double         getSearchRatio                () const { return _configuration->getSearchRatio(); }
    inline  double         getAnnealingNetMult           () const { return _configuration->getAnnealingNetMult(); } 
    inline  double         getAnnealingBinMult           () const { return _configuration->getAnnealingBinMult(); }
    inline  double         getAnnealingRowMult           () const { return _configuration->getAnnealingRowMult(); }
            DbU::Unit      getInstanceIdWidth            (unsigned id) const { return _instanceWidths[id]; }
            unsigned       getRandomInstanceId           () const;
  private:
    inline  Surface*       _getSurface                   () const { return _surface; }

// Others
// ******
    private: void _postCreate(Box& placementbox);
    private: void _preDestroy();
    public: static  MaukaEngine* get ( const Cell* );
    //public: void ReInit();
    public: bool Iterate();
    public: void Run();
    public: void Test();
    public: virtual std::string _getTypeName() const {return "MaukaEngine";};
    public: virtual Record* _getRecord() const;
    public: void Save() const;
    public: void PlotBinsStats() const;
    public: void Plot() const;
    private: Box PlotFixedPointsLabels(std::ofstream& out) const;
    private: void Construct();

};

void setPlacementStatusRecursivelyToPlaced(Instance* instance);
bool TestMaukaConstruction(Cell* cell, GCell* gcell);

}  // Enf of Mauka namespace.

#endif /* __MAUKA_H */
