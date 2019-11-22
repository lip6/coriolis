// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      B o r a   -  A n a l o g   S l i c i n g   T r e e         |
// |                                                                 |
// |  Authors     :                         Eric LAO                 |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./NodeSets.cpp"                                |
// +-----------------------------------------------------------------+


#include "bora/NodeSets.h"
#include "hurricane/Warning.h"
#include "hurricane/analog/Device.h"
#include "hurricane/analog/TransistorFamily.h"
#include "hurricane/analog/MultiCapacitor.h"
#include "hurricane/analog/Resistor.h"
#include "hurricane/analog/LayoutGenerator.h"
#include "crlcore/RoutingGauge.h"


namespace Bora {

  using namespace Hurricane;
  using namespace Analog;


  NodeSets::NodeSets ( ParameterRange* range )
    : _boxSets()
    , _range  (NULL)
  {
    if (range) {
      _range = range->clone();
      _range->setNested();
    }
  }
  

  NodeSets::NodeSets ( const NodeSets* from )
    : _boxSets( from->getBoxSets() )
    , _range  (NULL)
  {
    if (from->_range) {
      _range = from->_range->clone();
      _range->setNested();
    }
  }


  NodeSets::~NodeSets ()
  {
    if (_range) delete _range;
  }

  
  NodeSets* NodeSets::create ( Cell*              cell
                             , ParameterRange*    range
                             , CRL::RoutingGauge* rg )
  {
    NodeSets* nodeset = new NodeSets( range );
    if (not cell) return nodeset;

    unique_ptr<LayoutGenerator> layoutGenerator ( new LayoutGenerator() );

    TransistorFamily*   device    = dynamic_cast<TransistorFamily  *>( cell  );
    StepParameterRange* stepRange = dynamic_cast<StepParameterRange*>( nodeset->getRange() );
    if (device) {
    //cdebug_log(536,0) << "createNodeSets for an Analog Device" << endl;
      if (not stepRange) {
        throw Error( "NodeSets::create(): Device \"%s\" must be associated with a StepParameterRange argument instead of %s."
                   , getString(device->getName()).c_str()
                   , getString(stepRange).c_str()
                   );
      }

      stepRange->reset();
      do {
        device->setNfing( stepRange->getValue() ); 
      
        layoutGenerator->setDevice( device );
        layoutGenerator->drawLayout(); 
      
        nodeset->push_back( DBoxSet::create( device, stepRange->getIndex(), rg ) );

        stepRange->progress();
      } while ( stepRange->isValid() );
    } else {
      MultiCapacitor*       mcapacitor  = dynamic_cast<MultiCapacitor      *>( cell  );
      MatrixParameterRange* matrixRange = dynamic_cast<MatrixParameterRange*>( nodeset->getRange() );

      if (mcapacitor) {
        if (not matrixRange) {
          throw Error( "NodeSets::create(): Device \"%s\" must be associated with a MatrixParameterRange argument instead of %s."
                     , getString(mcapacitor->getName()).c_str()
                     , getString(stepRange).c_str()
                     );

          matrixRange->reset();
          do {
            MatrixParameter* mp = NULL;
            if ( (mp = dynamic_cast<MatrixParameter*>(mcapacitor->getParameter("matrix"))) != NULL ) 
              mp->setMatrix( &matrixRange->getValue() );
      
            layoutGenerator->setDevice( mcapacitor );
            layoutGenerator->drawLayout(); 

            nodeset->push_back( DBoxSet::create( mcapacitor, matrixRange->getIndex(), rg ) );

            matrixRange->progress();
          } while ( matrixRange->isValid() );
        }
      } else {
        ResistorFamily*     device    = dynamic_cast<ResistorFamily    *>( cell  );
        StepParameterRange* stepRange = dynamic_cast<StepParameterRange*>( nodeset->getRange() );
        if (device) {
          if (not stepRange) {
            throw Error( "NodeSets::create(): Device \"%s\" must be associated with a StepParameterRange argument instead of %s."
                       , getString(device->getName()).c_str()
                       , getString(stepRange).c_str()
                       );
          }

          stepRange->reset();
          do {
            device->setBends( stepRange->getValue() ); 
      
            layoutGenerator->setDevice( device );
            layoutGenerator->drawLayout(); 
      
            nodeset->push_back( DBoxSet::create( device, stepRange->getIndex(), rg ) );

            stepRange->progress();
          } while ( stepRange->isValid() );
        } else {
          nodeset->push_back( DBoxSet::create( cell, 0, rg ) );
        }
      }
    }

    return nodeset;
  }


  // NodeSets* NodeSets::create ()
  // {
  //   return new NodeSets();
  // }


  BoxSet* NodeSets::operator[] ( size_t i ) 
  {
    if (i < _boxSets.size()) return _boxSets[i];
    return NULL;
  }


  BoxSet* NodeSets::at ( size_t i ) 
  {
    if (i < _boxSets.size()) return _boxSets[i];
    return NULL;
  }


  void  NodeSets::clear ()
  {
    if (not _boxSets.empty()) {
      for ( BoxSet* bs : _boxSets ) bs->destroy();
    }
    _boxSets.clear();
  }


  BoxSet* NodeSets::getPairH ( DbU::Unit height ) const 
  {
    DbU::Unit w        = 0;
    DbU::Unit hclosest = 0;
    BoxSet*   boxSet   = (*_boxSets.begin());

    for ( BoxSet* bs : _boxSets ) {
      if ( (bs->getHeight() > hclosest) and (height >= bs->getHeight()) ) {
        hclosest = bs->getHeight();
        w        = bs->getWidth ();
        boxSet   = bs;
      }
    }
    
    if ( (w == 0) and (hclosest == 0) )
      cerr << Warning( "NodeSets::getPairH(): No solution for h=%s has been found."
                     , DbU::getValueString(height).c_str() ) << endl;

    return boxSet;
  }
 

  BoxSet* NodeSets::getPairHW ( DbU::Unit height, DbU::Unit width ) const 
  {
    vector<BoxSet*>::const_iterator it = _boxSets.begin();
    DbU::Unit bestH    = (*it)->getHeight();
    DbU::Unit bestW    = (*it)->getWidth();
    DbU::Unit currentH = 0;
    DbU::Unit currentW = 0;
    BoxSet*   boxSet   = (*it);

    while ( it != _boxSets.end() ) {
      currentH = (*it)->getHeight();
      currentW = (*it)->getWidth ();

      if (currentH <= height) {
        if (currentH > bestH) {
          bestH  = currentH;
          bestW  = currentW;
          boxSet = (*it);
        } else if (currentH == bestH) {
          if ( (currentW > bestW) and (currentW <= width) ) { 
            bestH = currentH;
            bestW = currentW;
            boxSet = (*it);
          }
          else if ( (currentW < bestW) and (bestW > width) ) { 
            bestH  = currentH;
            bestW  = currentW;
            boxSet = (*it);
          }
        }
        ++it;
      } else {
        it = _boxSets.end();
      }
    }

    return boxSet;
  }
 

  BoxSet* NodeSets::getPairHW ( unsigned int index ) const 
  {
    if (index < _boxSets.size()) return _boxSets[index];
    return NULL;
  }


  size_t  NodeSets::findIndex ( DbU::Unit height, DbU::Unit width ) const
  {
    for ( size_t i=0 ; i<_boxSets.size() ; ++i ) {
      if (   (_boxSets[i]->getHeight() == height)
         and (_boxSets[i]->getWidth () == width ) )
        return i;
    }

    return NotFound;
  }


  BoxSet* NodeSets::find ( DbU::Unit height, DbU::Unit width )
  {
    size_t i = 0;
    for ( ; i<_boxSets.size() ; ++i ) {
      if (   (_boxSets[i]->getHeight() == height)
         and (_boxSets[i]->getWidth () == width ) ) 
        return _boxSets[i];
    }

    return NULL;
  }


  BoxSet* NodeSets::find ( BoxSet* boxSet )
  {
    return find( boxSet->getHeight(), boxSet->getWidth() );
  }


  BoxSet* NodeSets::find ( size_t index )
  {
    size_t i = 0;
    for ( ; i<_boxSets.size() ; ++i ) {
      if (_boxSets[i]->getIndex() == index)
        return _boxSets[i];
    }

    return NULL;
  }


  void NodeSets::print() const 
  {
    cerr << "Print - NodeSets:" << endl;
    int index = 0;
    if (not size()) {
      cerr << "--- EMPTY ---" << endl;
    } else {
      for ( size_t i=0 ; _boxSets.size() ; ++i ) {
        cerr << "[" << setw(3) << index << "] ";
        _boxSets[i]->print();
      }
      cerr << endl;
    }
  }


  bool NodeSets::compare ( NodeSets nodeSets2, unsigned int flags ) const
  {
    if (_boxSets.size() != nodeSets2.size()) {
      if (flags & ShowDiff)
        cerr << "NodeSets::compare() size differs: " << _boxSets.size()
             << " vs. " << nodeSets2.size() << endl;
      return false;
    }

    bool isSame = true;
    for ( size_t iset = 0 ; iset<_boxSets.size() ; ++iset ) {
      if (  (_boxSets[iset]->getHeight() != nodeSets2[iset]->getHeight())
         or (_boxSets[iset]->getWidth () != nodeSets2[iset]->getWidth ()) ) {
        isSame = false;
        if (flags & ShowDiff)
          cerr << "NodeSets::compare() Box [" << iset << "] differs "
               << " (" << DbU::getValueString(_boxSets[iset]->getWidth()) << "," << DbU::getValueString(_boxSets[iset]->getHeight()) << ") vs."
               << " (" << DbU::getValueString(nodeSets2[iset]->getWidth()) << "," << DbU::getValueString(nodeSets2[iset]->getHeight()) << ")"
               << endl;
      }
    }

    return isSame;
  }


  void NodeSets::push_back ( BoxSet* boxSet )
  {
    if (find(boxSet) == NULL) _boxSets.push_back( boxSet );
    else { 
      find( boxSet )->incrementCpt(); 
      cerr << "NodeSets::push_back(): do not add current BoxSet, already exists." << endl;
      boxSet->destroy();
    }
  }


  void NodeSets::push_back ( vector<BoxSet*> vect
                           , DbU::Unit       height
                           , DbU::Unit       width
                           , unsigned int    type )
  {
    if (find(height,width) == NULL) { 
      if (type == HorizontalSNode) {
        _boxSets.push_back( HBoxSet::create( vect, height, width ) );
      } else if (type == VerticalSNode) {
        _boxSets.push_back( VBoxSet::create( vect, height, width ) );   
      }    
    } else
      find( height, width )->incrementCpt(); 
  }


  NodeSets* NodeSets::clone() 
  {
    NodeSets* nodesets = new NodeSets( _range );
    for ( BoxSet* bs : _boxSets ) _boxSets.push_back( bs->clone() );
    return nodesets;
  }


}  // Bora namespace.
