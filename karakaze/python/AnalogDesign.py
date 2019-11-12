# -*- Mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2016-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
# |                                                                 |
# |  Author      :                    Jean-Paul Chaput              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :  "./karakaze/AnalogDesign.py"                    |
# +-----------------------------------------------------------------+


from   Hurricane  import *
from   Hurricane  import DataBase
import CRL
import helpers
from   helpers    import isderived
from   helpers    import trace
from   helpers.io import ErrorMessage as Error
from   Analog     import Device
from   Analog     import TransistorFamily
from   Analog     import Transistor
from   Analog     import CommonDrain
from   Analog     import CommonGatePair
from   Analog     import CommonSourcePair
from   Analog     import CrossCoupledPair
from   Analog     import DifferentialPair
from   Analog     import LevelShifter
from   Analog     import SimpleCurrentMirror
from   Analog     import CapacitorFamily
from   Analog     import MultiCapacitor
from   Analog     import CapacitorFamily
from   Analog     import MultiCapacitor
from   Analog     import ResistorFamily
from   Analog     import Resistor
from   Analog     import LayoutGenerator
from   Analog     import Matrix
from   Bora       import ParameterRange
from   Bora       import StepParameterRange
from   Bora       import MatrixParameterRange
from   Bora       import SlicingNode
from   Bora       import HSlicingNode
from   Bora       import VSlicingNode
from   Bora       import DSlicingNode
from   Bora       import RHSlicingNode
from   Bora       import RVSlicingNode
import karakaze.Oceane
import Anabatic
import Katana
import Bora


NMOS    = Transistor.NMOS
PMOS    = Transistor.PMOS
PIP     = CapacitorFamily.PIP
MIM     = CapacitorFamily.MIM
MOM     = CapacitorFamily.MOM
LOWRES  = ResistorFamily.LOWRES
HIRES   = ResistorFamily.HIRES
Center  = SlicingNode.AlignCenter
Left    = SlicingNode.AlignLeft
Right   = SlicingNode.AlignRight
Top     = SlicingNode.AlignTop
Bottom  = SlicingNode.AlignBottom
Unknown = SlicingNode.AlignBottom
VNode   = 1
HNode   = 2
DNode   = 3


def toDbU    ( value ): return DbU.fromPhysical( value, DbU.UnitPowerMicro )
def toLength ( value ): return float(value) * 1e+6


def readMatrix ( rows ):
    if not isinstance(rows,list):
      print '[ERROR] readMatrix(): First level is not a list.'
      sys.exit( 1 )
    rowCount = len(rows)

    for row in range(len(rows)):
      column = rows[row]
      if not isinstance(column,list):
        print '[ERROR] readMatrix(): Column %d is not a list.' % row
        sys.exit( 1 )
      if row == 0:
        columnCount = len(column)
        matrix      = Matrix( rowCount, columnCount )
      else:
        if columnCount != len(column):
          print '[ERROR] readMatrix(): Column %d size discrepency (sould be %d).' % (len(column),columnCount)
          sys.exit( 1 )

      for column in range(len(column)):
        matrix.setValue( row, column, rows[row][column] )
          
    return matrix      



class AnalogDesign ( object ):

    def __init__ ( self ):
        self.cellName        = None
        self.netCache        = {}
        self.rg              = None
        self.library         = None
        self.cell            = None
        self.netCache        = {}
        self.slicingTree     = None
        self.stack           = []
        self.stack2          = []
        self.toleranceRatioH = 0
        self.toleranceRatioW = 0
        self.toleranceBandH  = 0
        self.toleranceBandW  = 0
        self.parameters      = karakaze.Oceane.Parameters()
        return


    def setCellName ( self, name ):
        self.cellName = name
        return


    def beginCell ( self, cellName ):
        self.setCellName( cellName )
        UpdateSession.open()
        self.rg        = CRL.AllianceFramework.get().getRoutingGauge()
        self.cell      = CRL.AllianceFramework.get().createCell( self.cellName )
        self.library   = Library.create( DataBase.getDB().getRootLibrary(), 'AnalogRootLibrary' )
        self.generator = LayoutGenerator()
        return


    def endCell ( self ):
        UpdateSession.close()
        return


    def checkBeginCell ( self, function ):
        if not self.cell:
          raise Error( 3, [ 'AnalogDesign: \"AnalogDevice.beginCell()\" must be called *before* \"%s\".' \
                            % function
                          ] )
        return


    def checkConnexion ( self, count, net, connexion ):
        if not isinstance(connexion,tuple):
          raise Error( 3, [ 'AnalogDesign.doNets(): \"self.netSpecs\" in \"%s\", connexion [%d] is *not* a tuple.' \
                             % (net.getName(),count)
                            , '%s' % str(connexion) ] )
        if len(connexion) != 2:
          raise Error( 3, [ 'AnalogDesign.doNets(): \"self.devicesSpecs\" in \"%s\", connexion [%d] has %d items instead of 2 .' \
                            % (net.getName(),count,len(connexion))
                            , '%s' % str(connexion) ] )
        if not isinstance(connexion[0],str):
          raise Error( 3, [ 'AnalogDesign.doNets(): \"self.devicesSpecs\" in \"%s\", connexion [%d], field [0] (instance) is *not* a string.' \
                            % (net.getName(),count)
                            , '%s' % str(connexion) ] )
        if not isinstance(connexion[1],str):
          raise Error( 3, [ 'AnalogDesign.doNets(): \"self.devicesSpecs\" in \"%s\", connexion [%d], field [1] (terminal) is *not* a string.' \
                            % (net.getName(),count)
                            , '%s' % str(connexion) ] )
        return

    def checkRail( self, net, metal, npitch, cellName, instanceName ):
        #Net verification missing
        if not isinstance(metal,str):
          raise Error( 3, [ 'AnalogDesign.checkRail(): \"metal\" is *not* a string.' ] )
        if not isinstance(npitch,int):
          raise Error( 3, [ 'AnalogDesign.checkRail(): \"NPitch\" is *not* an int.' ] )
        if not isinstance(cellName,str):
          raise Error( 3, [ 'AnalogDesign.checkRail(): \"cellName\" is *not* a string.' ] )
        if not isinstance(instanceName,str):
          raise Error( 3, [ 'AnalogDesign.checkRail(): \"instanceName\" is *not* a string.' ] )
        return
    
    def connect ( self, instanceName, masterNetName, net ):
        instance  = getattr( self, instanceName )
        masterNet = instance.getMasterCell().getNet( masterNetName )
        instance.getPlug( masterNet ).setNet( net )

        state  = NetRoutingExtension.get(net)
        device = instance.getMasterCell()
        if masterNetName=='B':
            device.getParameter('B.w').setValue(int(state.getWPitch()))
        if masterNetName=='G':
            device.getParameter('G.w').setValue(int(state.getWPitch()))
        if masterNetName=='G1':
            device.getParameter('G1.w').setValue(int(state.getWPitch()))
        if masterNetName=='G2':
            device.getParameter('G2.w').setValue(int(state.getWPitch()))
        if masterNetName=='D':
            device.getParameter('D.w').setValue(int(state.getWPitch()))
        if masterNetName=='D1':
            device.getParameter('D1.w').setValue(int(state.getWPitch()))
        if masterNetName=='D2':
            device.getParameter('D2.w').setValue(int(state.getWPitch()))
        if masterNetName=='S':
            device.getParameter('S.w').setValue(int(state.getWPitch()))
        return


    def getNet ( self, netName, create=True ):
        net = None
        if self.netCache.has_key(netName):
          net = self.netCache[netName]
        elif create:
          net = Net.create( self.cell, netName )
          self.netCache[ netName ] = net
        return net


    def doNets ( self ):
        self.checkBeginCell( 'AnalogDesign.doNets()' )

        if not hasattr(self,'netSpecs'):
          raise Error( 3, 'AnalogDesign.doNets(): Mandatory attribute \"self.netSpecs\" has not been defined.' )
        if not isinstance(self.netSpecs,dict):
          raise Error( 3, 'AnalogDesign.doNets(): Attribute \"self.netSpecs\" *must* be a Python dict.' )

        for netName, netType in self.netTypes.items():
          if not isinstance(netName,str):
            raise Error( 3, 'AnalogDesign.doNets(): Dict key (net name) of \"self.netTypes\" *must* be a string (%s).' % str(netName) )

          net        = self.getNet( netName )
          isExternal = False
          if netType.has_key('isExternal'): isExternal = netType['isExternal']

        for netName, connexions in self.netSpecs.items():
          if not isinstance(netName,str):
            raise Error( 3, 'AnalogDesign.doNets(): Dict key (net name) of \"self.netSpecs\" *must* be a string (%s).' % str(netName) )
            
          net   = self.getNet( netName )
          state = NetRoutingExtension.create( net, NetRoutingState.AutomaticGlobalRoute|NetRoutingState.Analog )
          count = 1
          for connexion in connexions:
            if isinstance(connexion,tuple): 
              self.checkConnexion( count, net, connexion ) 
              self.connect( connexion[0], connexion[1], net )
              count += 1
            else:
              if isinstance(connexion,dict): state.setWPitch(long(connexion['W']))
          
        return


    def checkDSpec ( self, count, dspec ):
        if not isinstance(dspec,list):
          raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], is *not* a list.' % count
                          , '%s' % str(dspec) ])
        if not isderived(dspec[0],Device):
          raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [0] is *not* a Device class.' % count
                          , '%s' % str(dspec) ])

        specSize = 0
        if   isderived(dspec[0],TransistorFamily): specSize = 12
        elif isderived(dspec[0], CapacitorFamily): specSize = 6
        elif isderived(dspec[0],  ResistorFamily): specSize = 5
        else:
          raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], has unsupported device type.' \
                            % (count)
                          , '%s' % str(dspec) ])

        if len(dspec) < specSize:
          raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], has %d items instead of 12 .' \
                            % (count,len(dspec))
                          , '%s' % str(dspec) ])
        if not isinstance(dspec[1],str):
          raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [1] (model name) is *not* a string.' % count
                          , '%s' % str(dspec) ])
        if not isinstance(dspec[2],str):
          raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [2] (layout style) is *not* a string.' % count
                          , '%s' % str(dspec) ])

        if specSize == 12:
          if dspec[3] not in [NMOS, PMOS]:
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [3] (type) must be either NMOS or PMOS.' % count
                            , '%s' % str(dspec) ])
          if not isinstance(dspec[4],float):
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [4] (WE) is *not* a float.' % count
                            , '%s' % str(dspec) ])
          if not isinstance(dspec[5],float):
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [5] (LE) is *not* a float.' % count
                            , '%s' % str(dspec) ])
          if not isinstance(dspec[6],int):
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [6] (M) is *not* an int.' % count
                            , '%s' % str(dspec) ])
          if (not dspec[7] is None) and (not isinstance(dspec[7],int)):
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [7] (Mint) is neither an int nor None.' % count
                            , '%s' % str(dspec) ])
          if not isinstance(dspec[8],int):
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [8] (external dummies) is *not* an int.' % count
                            , '%s' % str(dspec) ])
          if not isinstance(dspec[9],bool):
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [9] (source first) is *not* a boolean.' % count
                            , '%s' % str(dspec) ])
          if not isinstance(dspec[10],int):
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [10] (bulk) is *not* an int.' % count
                            , '%s' % str(dspec) ])
          else:
            if dspec[10] > 0xf:
              raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [10] (bulk) is greater than 0xf.' % count
                              , '%s' % str(dspec) ])
          if not isinstance(dspec[11],bool):
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [11] (bulk connected) is *not* a boolean.' % count
                            , '%s' % str(dspec) ])

        elif specSize == 6:
          if dspec[3] not in [PIP, MIM, MOM]:
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [3] (type) must be either PIP, MIM or MOM.' % count
                            , '%s' % str(dspec) ])
          if   isinstance(dspec[4],float): pass
          elif isinstance(dspec[4],tuple): pass
          else:
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [4] (Cs) should either be *one* float or a *list* of floats.' % count
                            , '%s' % str(dspec) ])

        elif specSize == 5:
          if dspec[3] not in [LOWRES, HIRES]:
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [3] (type) must be either LOWRES or HIRES.' % count
                            , '%s' % str(dspec) ])
          if   isinstance(dspec[4],float): pass
          else:
            raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [4] (resistance) must be a float.' % count
                            , '%s' % str(dspec) ])

        else:
          raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], spec list do not match any known pattern.' % count
                          , '%s' % str(dspec) ])
        return

    def checkDSpecDigital ( self, count, dspec ):
        # if not isinstance(dspec[0],str):
        #   raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [0] (model name) is *not* a string.' % count
        #                   , '%s' % str(dspec) ])
        if not isinstance(dspec[1],str):
          raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [1] (model name) is *not* a string.' % count
                          , '%s' % str(dspec) ])
        return

    def readParameters ( self, path ):
        trace( 110, ',+', '\tReading Oceane parameters from \"%s\"\n' % path )

        if not path: return
        self.parameters.read( path );
        for dspec in self.devicesSpecs:
          if len(dspec) > 2:
            Tname       = dspec[1].split('_')[0]
            Tparameters = self.parameters.getTransistor( Tname )
            if not Tparameters:
              raise Error( 3, [ 'AnalogDesign.readParameters(): Missing parameters for \"%s\".' % Tname ] )
              continue
            dspec[4] = toLength( Tparameters.W )
            dspec[5] = toLength( Tparameters.L )
            dspec[6] =           Tparameters.M
            trace( 110, '\t- \"%s\" : W:%f L:%f M:%d\n' % (Tname
                                                          ,dspec[4]
                                                          ,dspec[5]
                                                          ,dspec[6]) )
        trace( 110, '-,' )
        return


    def doDevice ( self, count, dspec ):
        self.checkBeginCell( 'AnalogDesign.doDevice()' )
        if len(dspec) == 2:
            self.checkDSpecDigital( count, dspec )
            if isinstance( dspec[0], str ):
                masterCell = CRL.AllianceFramework.get().getCell( dspec[0], CRL.Catalog.State.Views )
                instance   = Instance.create( self.cell, dspec[1], masterCell, Transformation() )
                instance.setPlacementStatus( Instance.PlacementStatus.UNPLACED )
                self.__dict__[ dspec[1] ] = instance
            else:
                masterCell = dspec[0]
                instance   = Instance.create( self.cell, dspec[1], masterCell, Transformation() )
                instance.setPlacementStatus( Instance.PlacementStatus.UNPLACED )
                self.__dict__[ dspec[1] ] = instance
        else:
            self.checkDSpec( count, dspec )

            trace( 110, '\tBuilding \"%s\"\n' % dspec[1] )
            if isderived(dspec[0],TransistorFamily):
              device = dspec[0].create( self.library, dspec[1], dspec[3], dspec[11] )
              device.getParameter( 'Layout Styles' ).setValue( dspec[2] )
              device.getParameter( 'W' ).setValue( toDbU(dspec[4]) )
              device.getParameter( 'L' ).setValue( toDbU(dspec[5]) )
              device.getParameter( 'M' ).setValue( dspec[6] )
              device.setSourceFirst( dspec[9] )
              device.setBulkType   ( dspec[10] )
            
              if (len(dspec) > 12): device.getParameter( 'NERC' ).setValue(int (dspec[12]))
              if (len(dspec) > 13): device.getParameter( 'NIRC' ).setValue(int (dspec[13]))
            
              if not (dspec[7] is None): device.setMint         ( dspec[7] ) 
              if dspec[8]:               device.setExternalDummy( dspec[8] )

            elif isderived(dspec[0],CapacitorFamily):
              if   isinstance(dspec[4],float): capaValues = (dspec[4],) 
              elif isinstance(dspec[4],tuple): capaValues =  dspec[4]
              else:
                  raise ErrorMessage( 1, 'AnalogDesign.doDevice(): Invalid type for capacities values "%s".' \
                                         % str(dspec[4]) )

              device = dspec[0].create( self.library, dspec[1], dspec[3], len(capaValues) )
              device.getParameter( 'Layout Styles' ).setValue( dspec[2] )
              for i in range(len(capaValues)):
                device.getParameter( 'capacities' ).setValue( i, capaValues[i]  )

            elif isderived(dspec[0],ResistorFamily):
              device = dspec[0].create( self.library, dspec[1], dspec[3] )
              device.getParameter( 'Layout Styles' ).setValue ( dspec[2] )
              device.getParameter( 'Resistance'    ).setMatrix( dspec[4] )
            else:
              raise ErrorMessage( 1, 'AnalogDesign.doDevice(): Unknown/unsupported device "%s".' % str(dspec[0]) )
            
            self.generator.setDevice ( device )
            self.generator.drawLayout()
            instance = Instance.create( self.cell
                                      , dspec[1]
                                      , device
                                      , Transformation()
                                      , Instance.PlacementStatus.UNPLACED )

            self.__dict__[ dspec[1] ] = instance
        return


    def doDevices ( self ):
        trace( 110, ',+', '\tAnalogDesign.doDevices()\n' )

        if not hasattr(self,'devicesSpecs'):
          raise Error( 3, 'AnalogDesign.doDevices(): Mandatory attribute \"self.devicesSpecs\" has not been defined.' )
        if not isinstance(self.devicesSpecs,list):
          raise Error( 3, 'AnalogDesign.doDevices(): Attribute \"self.devicesSpecs\" *must* be a Python list.' )
        
        count = 1
        for dspec in self.devicesSpecs:
          self.doDevice( count, dspec )
          count += 1
        trace( 110, '-,' )
        return


    def showNode ( self, node ):
        lines = [ '{' ]
        for key, value in node.items():
          if key == 'children':
            lines += [ "%20s { ... }" % "'children':" ]
          else:
            skey   = "'%s':" % str(key)
            lines += [ "%20s %s" % (skey,str(value)) ] 
        lines += [ '}' ]
        return lines


    def checkNode ( self, node, isRoot ):
        if not isinstance(node,dict):
          raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Node element is *not* a dict.'
                          ] + self.showNode(node) )
        if not node.has_key('type'):
          raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"type\" key/element.'
                          ] + self.showNode(node) )
        nodeType = node['type']
        if nodeType not in [VNode, HNode, DNode]:
          raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"type\" must be one of VNode, HNode or DNode.'
                          ] + self.showNode(node) )

        if nodeType == DNode:
          if not node.has_key('device'):
            raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"device\" key/element.'
                            ] + self.showNode(node) )
          if not isinstance(node['device'],str):
            raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"device\" value *must* be of type str.'
                            ] + self.showNode(node) )
          if not node.has_key('span'):
            raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"span\" key/element.'
                            ] + self.showNode(node) )
          if    not isinstance(node['span'],tuple)    \
             or len(node['span']) != 3                \
             or not isinstance(node['span'][0],float) \
             or not isinstance(node['span'][1],float) \
             or not isinstance(node['span'][2],float):
            raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"span\" value *must* be a tuple of 3 floats.'
                            ] + self.showNode(node) )
          if not node.has_key('NF'):
            raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"NF\" key/element.'
                            ] + self.showNode(node) )
          if not isinstance(node['NF'],int):
            raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"NF\" value *must* be of type int.'
                            ] + self.showNode(node) )
        else:
          if isRoot:
            if not node.has_key('toleranceRatioH'):
              raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"toleranceRationH\" key/element in root node.'
                              ] + self.showNode(node) )
            if not node.has_key('toleranceRatioW'):
              raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"toleranceRationW\" key/element in root node.'
                              ] + self.showNode(node) )
            if not node.has_key('toleranceBandH'):
              raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"toleranceBandH\" key/element in root node.'
                              ] + self.showNode(node) )
            if not node.has_key('toleranceBandW'):
              raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"toleranceBandW\" key/element in root node.'
                              ] + self.showNode(node) )
            if not node.has_key('children'):
              print Error( 3, [ 'AnalogDesign.doSlicingTree(): Suspicious root node without children.'
                              ] + self.showNode(node) )
          if node.has_key('children'):
            if not isinstance(node['children'],list):
              raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"children\" value *must* be of type list.' ]
                              + self.showNode(node) )

        if node.has_key('symmetries'):
          symmetries = node['symmetries']
          if not isinstance(symmetries,list):
            raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"symmetries\" value *must* be of type list.'
                            ] + self.showNode(node) )
          for i in range(len(symmetries)):
            if    not isinstance(symmetries[i],tuple)  \
               or len(symmetries[i]) != 2              \
               or not isinstance(symmetries[i][0],int) \
               or not isinstance(symmetries[i][1],int):
              raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"symmetries\" entry [%d] *must* be a tuple of 2 int.' % i ]
                              + self.showNode(node) )
        return


    def beginSlicingTree ( self ):
        trace( 110, ',+', '\tAnalogDesign.beginSlicingTree()\n' )
        return

    def topNode          ( self ): return self.stack[-1][0]
    def topSymmetries    ( self ): return self.stack[-1][1]
    def topSymmetriesNet ( self ): return self.stack[-1][2]

    def setToleranceRatioH ( self, u ): self.toleranceRatioH = toDbU(u)
    def setToleranceRatioW ( self, u ): self.toleranceRatioW = toDbU(u)
    def setToleranceBandH  ( self, u ): self.toleranceBandH  = toDbU(u)
    def setToleranceBandW  ( self, u ): self.toleranceBandW  = toDbU(u)

    def dupTolerances ( self, node ):
        node.setToleranceRatioH( self.toleranceRatioH )
        node.setToleranceRatioW( self.toleranceRatioW )
        node.setToleranceBandH ( self.toleranceBandH  )
        node.setToleranceBandW ( self.toleranceBandW  )
        return

    def pushNode ( self, node ):
        trace( 110, ',+', '\tSlicingTree.pushNode() %s ' % str(node) )
        parent = None
        if len(self.stack):
          parent = self.topNode()
          parent.push_back( node )
          trace( 110, '(parent id:%d)\n' % parent.getId() )
        else:
          trace( 110, '(Root)\n' )
          self.slicingTree = node
          node.setCell( self.cell )
        
        self.stack.append( (node,[],[]) )
        self.dupTolerances( node )
        node.setRoutingGauge( self.rg )
       #node.cprint()
        return

    def pushVNode ( self, alignment ):
        self.pushNode( VSlicingNode.create( alignment ) )
        return

    def pushHNode ( self, alignment ):
        self.pushNode( HSlicingNode.create( alignment ) )
        return

    def popNode ( self ):
        for childIndex, copyIndex in self.topSymmetries():
          self.topNode().addSymmetry( childIndex, copyIndex )
        for type, net1, net2 in self.topSymmetriesNet():
          if (net2 == None): 
              self.topNode().addSymmetryNet( type, net1 )
          else:             
              self.topNode().addSymmetryNet( type, net1, net2 )

        trace( 110, '-,', '\tSlicingTree.popNode() %s\n' % str(self.topNode()) )

        if len(self.stack) == 1:
          trace( 110, '\tAnalogDesign.endSlicingTree()\n' )
          trace( 110, '-,', '\tSlicingTree %s stack size:%d\n' % (self.cell.getName(), len(self.stack)) )
          #self.topNode().setCell( self.cell )
          self.topNode().updateNetConstraints()
          self.topNode().updateGlobalSize()
            
        del self.stack[-1]
        return

    def addDevice ( self, name, align, parameter, index=0 ):
        node = DSlicingNode.create( name, self.cell, parameter, self.rg )
        node.setAlignment( align )
        if index != 0: node.setBoxSetIndex( index )
        self.topNode().push_back( node )
        trace( 110, '\tSlicingTree.addDevice() %s (parent id:%d)\n' % (str(node),self.topNode().getId()) )
       #node.cprint()
        return

    def addHRail ( self, net, metal, npitch, cellName, instanceName ):
        self.checkRail( net, metal, npitch, cellName, instanceName )
        node = RHSlicingNode.create( net, DataBase.getDB().getTechnology().getLayer(metal), npitch, cellName, instanceName)
        self.topNode().push_back( node )
        trace( 110, '\tSlicingTree.addHRail() to %s\n' % (str(self.topNode())) )
       #node.cprint()
        return

    def addVRail ( self, net, metal, npitch, cellName, instanceName ):
        self.checkRail( net, metal, npitch, cellName, instanceName )
        node = RVSlicingNode.create( net, DataBase.getDB().getTechnology().getLayer(metal), npitch, cellName, instanceName)
        self.topNode().push_back( node )
        trace( 110, '\tSlicingTree.addVRail() to %s\n' % (str(self.topNode())) )
       #node.cprint()
        return

    def addSymmetry ( self, childIndex, copyIndex ):
        self.topSymmetries().append( (childIndex,copyIndex) )
        return

    def addSymmetryNet ( self, type, net1, net2=None ):
        self.topSymmetriesNet().append( (type, net1, net2) )
        return

    def endSlicingTree ( self ):
        self.slicingTree.updateGlobalSize()
       #bora = Bora.BoraEngine.get( self.cell )
       #if not bora: bora = Bora.BoraEngine.create( self.cell )
       #bora.updateSlicingTree()
        return

    def updatePlacement ( self, *args ):
        if self.slicingTree:
          bora = Bora.BoraEngine.get( self.cell )
          if not bora: bora = Bora.BoraEngine.create( self.cell )

          signatureMatched = True
          if   len(args) == 2: bora.updatePlacement( toDbU(args[0]), toDbU(args[1]) )
          elif len(args) == 1: bora.updatePlacement( args[0] )
          else: signatureMatched = False

         #if signatureMatched:
         #  katana = Katana.KatanaEngine.get( self.cell )
         #  if katana:
         #    katana.loadGlobalRouting( Anabatic.EngineLoadGrByNet )
         #    katana.runNegociate( Katana.Flags.PairSymmetrics );
         #   #katana.destroy()
        return
