// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DSlicingNode.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/analog/Device.h"
#include "hurricane/analog/FormFactorParameter.h"
#include "hurricane/analog/TransistorFamily.h"
#include "hurricane/analog/Transistor.h"
#include "hurricane/analog/CommonSourcePair.h"
#include "hurricane/analog/DifferentialPair.h"
#include "hurricane/analog/LayoutGenerator.h"
#include "hurricane/analog/MultiCapacitor.h"
#include "anabatic/Session.h"
#include "bora/DSlicingNode.h"


namespace Bora {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Transformation;
  using Analog::Device;
  using Analog::FormFactorParameter;
  using Analog::MatrixParameter;
  using Analog::TransistorFamily;
  using Analog::Transistor;
  using Analog::CommonSourcePair;
  using Analog::DifferentialPair;
  using Analog::MultiCapacitor;
  using Analog::LayoutGenerator;
  using Anabatic::Session;


// -------------------------------------------------------------------
// Class  :  "Bora::DSlicingNode".


  int  DSlicingNode::_count    = 0;
  int  DSlicingNode::_countAll = 0;


  DSlicingNode::DSlicingNode ( unsigned int type
                             , NodeSets*    nodeSets
                             , unsigned int alignment
                             , Instance*    instance
                             , BoxSet*      boxSet 
                             )
    : SlicingNode( type, nodeSets, alignment, boxSet )
    , _instance(instance)
  { ++_count; }


  DSlicingNode::~DSlicingNode ()
  { --_count; }
  

  DSlicingNode* DSlicingNode::create ( NodeSets*    nodeSets
                                     , unsigned int alignment
                                     , Instance*    instance 
                                     , BoxSet*      boxSet 
                                     )
  {
    _countAll++;

    return new DSlicingNode( DeviceSNode, nodeSets, alignment, instance, boxSet ); 
  }


  void  DSlicingNode::print () const
  {
    cerr << "- Print from Slicing Node - " << endl;
    cerr << "SlicingType: Device Node" << endl; 
    if      (isAlignLeft  ()) cerr << "Alignment  : Left"    << endl;
    else if (isAlignCenter()) cerr << "Alignment  : Middle"  << endl;
    else if (isAlignRight ()) cerr << "Alignment  : Right"   << endl;
    else if (isAlignTop   ()) cerr << "Alignment  : Top"     << endl;
    else if (isAlignBottom()) cerr << "Alignment  : Bottom"  << endl;
    else                      cerr << "Alignment  : Unknown" << endl;
    cerr <<   "NFingers   : " << getNFing() << endl;
    if (_instance) cerr << "Instance   : " << _instance << endl;
    else           cerr << "Instance   : None" <<  endl;
    SlicingNode::print();
  }


  DSlicingNode* DSlicingNode::clone ( unsigned int tr )
  {
    DSlicingNode* node = DSlicingNode::create( _nodeSets->clone() 
                                             , getAlignment()
                                             , getInstance()
                                             , getBoxSet()
                                             );
    node->setPreset( getPreset() );
    node->setSet   ( getSet()    );
    node->setPlaced( getPlaced() );
    return node;
  }


  void  DSlicingNode::setNFing ( int nfing )
  { _boxSet = _nodeSets->find( nfing ); }


  int  DSlicingNode::getNFing () const
  { return (_boxSet) ? _boxSet->getNFing() : 1 ; }


  void DSlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )
  {
    cdebug_log(536,1) << "DSlicingNode::_place(DbU::Unit,DbU::Unit,bool replace)" << endl;

    if (replace) {
      SlicingNode::place( x, y );
      if (_instance) {
        Cell* model = _instance->getMasterCell();  
        _instance->setTransformation ( Transformation( _x - model->getAbutmentBox().getXMin()
                                                     , _y - model->getAbutmentBox().getYMin() ) );
        _instance->setPlacementStatus( Instance::PlacementStatus::PLACED ); 
      }
    } else {
      if (isSet()) {
        SlicingNode::place( x, y );

        if (_instance) {
          Cell*   model  = _instance->getMasterCell();   
          Device* device = dynamic_cast<Device*>( model );

          if (device) {     
            unique_ptr<LayoutGenerator> layoutGenerator ( new LayoutGenerator() );

            TransistorFamily* tf = dynamic_cast<TransistorFamily*>( device );
            if (tf) {
              StepParameterRange* stepRange = dynamic_cast<StepParameterRange*>( getNodeSets()->getRange() );
              if (not stepRange) {
                throw Error( "DSlicingNode::_place(): Device \"%s\" must be associated with a StepParameterRange argument instead of %s."
                           , getString(device->getName()).c_str()
                           , getString(stepRange).c_str()
                           );
              }
              stepRange->setIndex( getBoxSet()->getIndex() );
              int nfingers = stepRange->getValue();

              tf->setNfing( nfingers ); 
              layoutGenerator->setDevice ( device );
              layoutGenerator->drawLayout(); 
            } else {
              MultiCapacitor*       mcapacitor  = dynamic_cast<MultiCapacitor      *>( device );
              MatrixParameterRange* matrixRange = dynamic_cast<MatrixParameterRange*>( getNodeSets()->getRange() );

              if (mcapacitor) {
                if (not matrixRange) {
                  throw Error( "DSlicingNode::create(): Device \"%s\" must be associated with a MatrixParameterRange argument instead of %s."
                             , getString(mcapacitor->getName()).c_str()
                             , getString(matrixRange).c_str()
                             );
                }

                matrixRange->setIndex( getBoxSet()->getIndex() );
                MatrixParameter* mp = NULL;
                if ( (mp = dynamic_cast<MatrixParameter*>(mcapacitor->getParameter("Matrix"))) != NULL ) 
                  mp->setMatrix( &matrixRange->getValue() );
      
                layoutGenerator->setDevice( mcapacitor );
                layoutGenerator->drawLayout(); 
              }
            }
          }
          _instance->setTransformation ( Transformation( _x - model->getAbutmentBox().getXMin()
                                                       , _y - model->getAbutmentBox().getYMin() ) );
          _instance->setPlacementStatus( Instance::PlacementStatus::PLACED );
        }
      }
    }

    cdebug_tabw(536,-1);
  }


  void  DSlicingNode::place ( DbU::Unit x, DbU::Unit y )
  { DSlicingNode::_place(x, y); }


  void DSlicingNode::preDestroy ()
  {
    SlicingNode::preDestroy();
  }


  void DSlicingNode::destroy()
  {
    DSlicingNode::preDestroy();
    delete this;
  }


  void  DSlicingNode::preRecursiveDestroy ()
  {
    SlicingNode::preRecursiveDestroy();
  }


  void DSlicingNode::recursiveDestroy ()
  {
    DSlicingNode::preRecursiveDestroy();
    delete this;
  }


  bool  DSlicingNode::checkInitialPlacement ( int& cpt ) const
  {
  // Notes: Initial placement criteria is having at least 2 devices
  //        placed at (x,y) = (0 0)
  //
  // jpc: Very suspicious code, have to check how it is called.

    bool initialPlacement = false;
    if (cpt < 2) {
      if ( (_x == 0) and (_y == 0) ) {
        cpt++;
        if (cpt < 2) initialPlacement = true;
      } else
        initialPlacement = true;
    } 
    return initialPlacement;
  }


  void  DSlicingNode::setGCell ( Anabatic::GCell* gcell )
  {
    cdebug_log(535,0) << "DSlicingNode::setGCell(Anabatic::GCell*) " << gcell << endl;

    if (getParent()) {
      if (getParent()->getType() == HorizontalSNode) {
        if (getAlignment() == AlignLeft) {
          Anabatic::GCell* strut = gcell->vcut( getX() + getWidth() );
          _gcell = gcell;
          strut->setType( Anabatic::Flags::StrutGCell );
        } else if (getAlignment() == AlignCenter) {
          _gcell = gcell->vcut( getX() );
          Anabatic::GCell* strut = _gcell->vcut( getX() + getWidth() );
          gcell->setType( Anabatic::Flags::StrutGCell );
          strut->setType( Anabatic::Flags::StrutGCell );
        } else if (getAlignment() == AlignRight ){
          _gcell = gcell->vcut( getX() );
          gcell->setType( Anabatic::Flags::StrutGCell );
        }
      } else if (getParent()->getType() == VerticalSNode) {
        if (getAlignment() == AlignBottom){
          Anabatic::GCell* strut = gcell->hcut( getY() + getHeight() );
          _gcell = gcell;
          strut->setType( Anabatic::Flags::StrutGCell );
        } else if (getAlignment() == AlignCenter) {
          _gcell = gcell->hcut( getY() );
          Anabatic::GCell* strut = _gcell->hcut( getY() + getHeight() );
          gcell->setType( Anabatic::Flags::StrutGCell );
          strut->setType( Anabatic::Flags::StrutGCell );
        } else if (getAlignment() == AlignTop) {
          _gcell = gcell->hcut( getY() );
          gcell->setType( Anabatic::Flags::StrutGCell );
        }
      } else
        _gcell = gcell;
    } else
      _gcell = gcell;

    if (_gcell) {
      if (dynamic_cast<Device*>(_instance->getMasterCell())) {
        _gcell->setType( Anabatic::Flags::DeviceGCell );
      } else {
        _gcell->setType( Anabatic::Flags::MatrixGCell ); 
        if(   (_gcell->getWidth()  > Anabatic::Session::getSliceHeight())
          and (_gcell->getHeight() > Anabatic::Session::getSliceHeight()) ) {
          _gcell->doGrid();
        }
      }
    }

    cdebug_log(535,0) << "> Device:" << _instance << endl;
    cdebug_log(535,0) << "> GCell: " << _gcell << endl;
  }


  bool  DSlicingNode::isSame ( SlicingNode* node, unsigned int flags ) const
  {
    bool isSame = _nodeSets->compare( node->getNodeSets(), flags );
    if ( not isSame and (flags & ShowDiff) )
      cerr << "DSlicingNode::isSame() Check " << this << " vs. " << node << endl;
    return isSame;
  }
  

  bool  DSlicingNode::isSymmetric ( SlicingNode* node, unsigned int symmetryType, unsigned int flags ) const
  {
    bool isSame = _nodeSets->compare( node->getNodeSets(), flags );
    if (not isSame and (flags & ShowDiff))
      cerr << Warning( "DSlicingNode::isSymmetric(): Different nodesets Check %s vs. %s."
                     , getString(this).c_str(), getString(node).c_str() ) << endl;

    if (_instance) {
      Cell*   model1    = _instance->getMasterCell();   
      Cell*   model2    = node->getInstance()->getMasterCell();  
      Device* device1   = dynamic_cast<Device*>( model1 );
      Device* device2   = dynamic_cast<Device*>( model2 );

      if (device1 and device2) {
        CommonSourcePair* csp1 = dynamic_cast<CommonSourcePair*>( model1 );
        DifferentialPair* dp1  = dynamic_cast<DifferentialPair*>( model1 );
        Transistor*       ts1  = dynamic_cast<Transistor*      >( model1 );

        CommonSourcePair* csp2 = dynamic_cast<CommonSourcePair*>( model2 );
        DifferentialPair* dp2  = dynamic_cast<DifferentialPair*>( model2 );
        Transistor*       ts2  = dynamic_cast<Transistor*      >( model2 );

        if      (csp1 and csp2) isSame = csp1->isSame( csp2 );
        else if (dp1  and dp2 ) isSame = dp1 ->isSame( dp2  );
        else if (ts1  and ts2 ) isSame = ts1 ->isSame( ts2  );
        else                    isSame = false;
      }

      if (not isSame)
        cerr << Warning( "DSlicingNode::isSymmetric(): Different devices. Check %s vs. %s."
                       , getString(this).c_str(), getString(node).c_str() ) << endl;
    }

    return isSame;
  }


  void  DSlicingNode::updateMatrixGCellPosition ()
  {
    DbU::Unit        tx     = getX() -_gcell->getXMin();
    DbU::Unit        ty     = getY() -_gcell->getYMin();
    Anabatic::GCell* ygcell = _gcell;
    Anabatic::GCell* xgcell = ygcell;

    while ( ygcell ) {
      while ( xgcell ) {
        xgcell->setSouthWestCorner( xgcell->getXMin() + tx, xgcell->getYMin() + ty ); 
        if (xgcell->getEast()->isMatrix()) xgcell = xgcell->getEast();
        else                               xgcell = NULL;
      }
      if (ygcell->getNorth()->isMatrix()) ygcell = ygcell->getNorth();
      else                                ygcell = NULL;
      xgcell = ygcell;
    }
  }


  void  DSlicingNode::updateMatrixGContacts ()
  {
    Anabatic::GCell* ygcell = _gcell;
    Anabatic::GCell* xgcell = ygcell;

    while ( ygcell ) {
      while ( xgcell ) {
        xgcell->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
        if (xgcell->getEast()->isMatrix()) xgcell = xgcell->getEast();
        else                               xgcell = NULL;
      }
      if (ygcell->getNorth()->isMatrix()) ygcell = ygcell->getNorth();
      else                                ygcell = NULL;
      xgcell = ygcell;
    }
  }


  void  DSlicingNode::updateGCellPosition ()
  {
    cdebug_log(535,0) << "DSlicingNode::updateGCellPosition() " << _gcell << endl;

    if (_gcell) {
      if (_parent) {
        if (_parent->getType() == HorizontalSNode) {
          DbU::Unit hpitch = _rg->getHorizontalPitch();

          if (getAlignment() == AlignLeft) {
            _gcell->getEastNMatrix()->setSouthWestCorner( getX()+getWidth(), getY() );
            _gcell->getEastNMatrix()->forceEdgesCapacities( floor((_parent->getWidth()-getWidth())/hpitch)
                                                          , floor(getHeight()/hpitch)
                                                          );
          } else if (getAlignment() == AlignCenter) {
            _gcell->getWest()->setSouthWestCorner( _parent->getX(), getY() );
            _gcell->getWest()->forceEdgesCapacities( floor((_parent->getWidth()-getWidth())/(2*hpitch))
                                                   , floor(getHeight()/(2*hpitch))
                                                   );

            _gcell->getEastNMatrix()->setSouthWestCorner( getX()+getWidth(), getY() );
            _gcell->getEastNMatrix()->forceEdgesCapacities( floor((_parent->getWidth()-getWidth())/(2*hpitch))
                                                          , floor(getHeight()/(2*hpitch))
                                                          );
          } else if (getAlignment() == AlignRight) {
            _gcell->getWest()->setSouthWestCorner( _parent->getX(), getY() );
            _gcell->getWest()->forceEdgesCapacities( floor((_parent->getWidth()-getWidth())/hpitch)
                                                   , floor(getHeight()/hpitch)
                                                   );
          
          }
        } else if (_parent->getType() == VerticalSNode) {
          DbU::Unit vpitch = _rg->getVerticalPitch();

          if (getAlignment() == AlignBottom) {
            _gcell->getNorthNMatrix()->setSouthWestCorner( getX(), getY()+getHeight() );
            _gcell->getNorthNMatrix()->forceEdgesCapacities( floor((_parent->getHeight()-getHeight())/vpitch)
                                                           , floor(getWidth()/vpitch)
                                                           );
          } else if (getAlignment() == AlignCenter) {
            _gcell->getSouth()->setSouthWestCorner( getX(), _parent->getY()    );
            _gcell->getSouth()->forceEdgesCapacities( floor((_parent->getHeight()-getHeight())/(2*vpitch))
                                                    , floor(getWidth()/(2*vpitch))
                                                    );

            _gcell->getNorthNMatrix()->setSouthWestCorner( getX(), getY()+getHeight() );
            _gcell->getNorthNMatrix()->forceEdgesCapacities( floor((_parent->getHeight()-getHeight())/(2*vpitch))
                                                           , floor(getWidth()/(2*vpitch))
                                                           );
          } else if (getAlignment() == AlignTop) {
            _gcell->getSouth()->setSouthWestCorner( getX(), _parent->getY()    );
            _gcell->getSouth()->forceEdgesCapacities( floor((_parent->getHeight()-getHeight())/vpitch)
                                                    , floor(getWidth()/vpitch)
                                                    );
          }
        }
      }

      if (_gcell->isMatrix()) updateMatrixGCellPosition();
      else                    _gcell->setSouthWestCorner( getX(), getY() );
      cdebug_log(535,0) << "| Updated:" << _gcell << endl;
    }
  }


  void  DSlicingNode::updateGContacts ()
  {
    cdebug_log(535,0) << "DSlicingNode::updateGContacts() " << _gcell << endl;

    if (_gcell) {
      if (_parent) {
        if (_parent->getType() == HorizontalSNode) {
          if (getAlignment() == AlignLeft) {
            _gcell->getEastNMatrix()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
          } else if (getAlignment() == AlignCenter) {
            _gcell->getWest()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
            _gcell->getEastNMatrix()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
          } else if (getAlignment() == AlignRight) {
            _gcell->getWest()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
          }
        } else if (_parent->getType() == VerticalSNode) {
          if (getAlignment() == AlignBottom) {
            _gcell->getNorthNMatrix()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
          } else if (getAlignment() == AlignCenter) {
            _gcell->getSouth()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
            _gcell->getNorthNMatrix()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
          } else if (getAlignment() == AlignTop) {
            _gcell->getSouth()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
          }
        }
      }

      if (_gcell->isMatrix()) updateMatrixGContacts();
      else                    _gcell->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );

      cdebug_log(535,0) << "| Updated:" << _gcell << endl;
    }
  }


  void  DSlicingNode::restrictDevices ()
  {
    if (_gcell) {
      Anabatic::Vertex* vertex = _gcell->getObserver< Anabatic::Vertex>( Anabatic::GCell::Observable::Vertex );
      vertex->setRestricted();
    
      if (not vertex->hasValidStamp()) {
        if (  (_gcell->getWidth() >  _gcell->getHeight())
           or (_gcell->getWidth() == _gcell->getHeight()) ) {
          vertex->unsetFlags( Anabatic::Vertex::iVertical   );
          vertex->setFlags  ( Anabatic::Vertex::iHorizontal );
        } else {
          vertex->unsetFlags( Anabatic::Vertex::iHorizontal );
          vertex->setFlags  ( Anabatic::Vertex::iVertical   );
        }
      }
    }
  }


  void  DSlicingNode::printLine () const
  {
    cerr << "Instance: " << _instance << ", ";
    SlicingNode::printLine();
  }


  string  DSlicingNode::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " "+getString(_instance->getName()));
    return s;
  }


  string  DSlicingNode::_getTypeName () const
  { return "DSlicingNode"; }


  bool  DSlicingNode::isAnalog () const
  {
    if (not _instance) return false;
    return dynamic_cast<Device*>(_instance->getMasterCell() );
  }


  bool  DSlicingNode::isDigital () const
  { return not isAnalog(); }


  bool  DSlicingNode::checkCellInstances ( Cell* cell )
  {
    if (not _instance) return false;
    return cell->getInstance(_instance->getName());
  }


  void  DSlicingNode::resetWireOccupation ()
  { }
  

}  // Bora namespace.
