# -*- Mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2016-2023, All Rights Reserved
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


from   ..Hurricane  import *
from   ..Hurricane  import DataBase
from   ..           import CRL
from   ..helpers    import isderived, setTraceLevel, trace
from   ..helpers.io import ErrorMessage as Error
from   ..Analog     import Device, TransistorFamily, Transistor,             \
                           CommonDrain, CommonGatePair, CommonSourcePair,    \
                           CrossCoupledPair, DifferentialPair, LevelShifter, \
                           SimpleCurrentMirror, CapacitorFamily,             \
                           MultiCapacitor, CapacitorFamily, MultiCapacitor,  \
                           ResistorFamily, Resistor, LayoutGenerator,        \
                           Matrix
from   ..Bora       import ParameterRange, StepParameterRange,               \
                           MatrixParameterRange, SlicingNode, HSlicingNode,  \
                           VSlicingNode, DSlicingNode, RHSlicingNode,        \
                           RVSlicingNode
from .              import oceane
from ..             import Anabatic, Katana, Bora


#setTraceLevel( 100 )


NMOS     = Transistor.NMOS
PMOS     = Transistor.PMOS
PIP      = CapacitorFamily.PIP
MIM      = CapacitorFamily.MIM
MOM      = CapacitorFamily.MOM
LOWRES   = ResistorFamily.LOWRES
HIRES    = ResistorFamily.HIRES
RPOLYH   = ResistorFamily.RPOLYH
RPOLY2PH = ResistorFamily.RPOLY2PH
Center   = SlicingNode.AlignCenter
Left     = SlicingNode.AlignLeft
Right    = SlicingNode.AlignRight
Top      = SlicingNode.AlignTop
Bottom   = SlicingNode.AlignBottom
Unknown  = SlicingNode.AlignBottom
VNode    = 1
HNode    = 2
DNode    = 3


def toDbU    ( value ): return DbU.fromPhysical( value, DbU.UnitPowerMicro )
def toLength ( value ): return float(value) * 1e+6


def readMatrix ( rows ):
    if not isinstance(rows,list):
        print( '[ERROR] readMatrix(): First level is not a list.' )
        sys.exit( 1 )
    rowCount = len(rows)
    for row in range(len(rows)):
        column = rows[row]
        if not isinstance(column,list):
            print( '[ERROR] readMatrix(): Column {} is not a list.'.format(row) )
            sys.exit( 1 )
        if row == 0:
            columnCount = len(column)
            matrix      = Matrix( rowCount, columnCount )
        else:
            if columnCount != len(column):
              print( '[ERROR] readMatrix(): Column {} size discrepency (sould be {}).'.format(len(column),columnCount))
              sys.exit( 1 )
        for column in range(len(column)):
            matrix.setValue( row, column, rows[row][column] )
    return matrix      


class AnalogDesign ( object ):

    SPEC_CLASS           =  0
    SPEC_INSTANCE        =  1
    SPEC_STYLE           =  2
    SPEC_TYPE            =  3
    SPEC_TRANS_W         =  4
    SPEC_TRANS_L         =  5
    SPEC_TRANS_M         =  6
    SPEC_TRANS_MINT      =  7
    SPEC_TRANS_DUMMY     =  8
    SPEC_TRANS_SFIRST    =  9
    SPEC_TRANS_BULK      = 10
    SPEC_TRANS_BULK_CONN = 11
    SPEC_CAPA_C          =  4
    SPEC_CAPA_MATRIX     =  5
    SPEC_CAPA_DUMMY      =  6

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
        self.parameters      = oceane.Parameters()
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
        if netName in self.netCache:
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
            if 'isExternal' in netType: isExternal = netType['isExternal']

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
        elif isderived(dspec[0], CapacitorFamily): specSize = 7
        elif isderived(dspec[0],  ResistorFamily): specSize = 8
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

        if isderived(dspec[0],TransistorFamily):
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
        elif isderived(dspec[0], CapacitorFamily):
            if dspec[3] not in [PIP, MIM, MOM]:
                raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [3] (type) must be either PIP, MIM or MOM.' % count
                                , '%s' % str(dspec) ])
            if   isinstance(dspec[4],float): pass
            elif isinstance(dspec[4],tuple): pass
            else:
                raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [4] (Cs) should either be *one* float or a *list* of floats.' % count
                                , '%s' % str(dspec) ])
        elif isderived(dspec[0],ResistorFamily):
            if dspec[3] not in [RPOLYH, RPOLY2PH]:
                raise Error( 3, [ 'AnalogDesign.doDevices(): \"self.devicesSpecs\" entry [%d], field [3] (type) must be either RPOLYH or RPOLY2PH.' % count
                                , '%s' % str(dspec) ])
            if   isinstance(dspec[5],float): pass
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

    def getCommonDSpec ( self, instanceName, specIndex ):
        for dspec in self.devicesSpecs:
            if dspec[AnalogDesign.SPEC_INSTANCE] == instanceName:
                if specIndex < len(dspec):
                    return dspec[specIndex]
                raise Error( 3, [ 'AnalogDesign.getDSpec(): Instance "{}" has not entry index {}' \
                                  .format( instanceName, specIndex )
                                , '%s' % str(dspec) ])
        raise Error( 3, [ 'AnalogDesign.getDSpec(): No instance "{}"'.format( instanceName ) ])

    def getTransDSpec ( self, instanceName, specIndex ):
        for dspec in self.devicesSpecs:
            if dspec[AnalogDesign.SPEC_INSTANCE] == instanceName:
                if not isderived(dspec[0],TransistorFamily): 
                    raise Error( 3, [ 'AnalogDesign.getTransDSpec(): Instance "{}" is *not* a transistor ({})' \
                                      .format( instanceName, type(dspec[0]).__name__ ) ])
                if specIndex < len(dspec):
                    return dspec[specIndex]
                raise Error( 3, [ 'AnalogDesign.getTransDSpec(): Instance "{}" has not entry index {}' \
                                  .format( instanceName, specIndex )
                                , '%s' % str(dspec) ])
        raise Error( 3, [ 'AnalogDesign.getTransDSpec(): No instance "{}"'.format( instanceName ) ])

    def getCapasDSpec ( self, instanceName, specIndex ):
        for dspec in self.devicesSpecs:
            if dspec[AnalogDesign.SPEC_INSTANCE] == instanceName:
                if not isderived(dspec[0],CapacitorFamily): 
                    raise Error( 3, [ 'AnalogDesign.getCapaDSpec(): Instance "{}" is *not* a capacitor ({})' \
                                      .format( instanceName, type(dspec[0]).__name__ ) ])
                if specIndex < len(dspec):
                    return dspec[specIndex]
                raise Error( 3, [ 'AnalogDesign.getCapaDSpec(): Instance "{}" has not entry index {}' \
                                  .format( instanceName, specIndex )
                                , '%s' % str(dspec) ])
        raise Error( 3, [ 'AnalogDesign.getCapaDSpec(): No instance "{}"'.format( instanceName ) ])

    def getClass          ( self, instName ): return self.getCommonDSpec( instName, AnalogDesign.SPEC_CLASS )
    def getInstance       ( self, instName ): return self.getCommonDSpec( instName, AnalogDesign.SPEC_INSTANCE )
    def getStyle          ( self, instName ): return self.getCommonDSpec( instName, AnalogDesign.SPEC_STYLE )
    def getType           ( self, instName ): return self.getCommonDSpec( instName, AnalogDesign.SPEC_TYPE )
    def getTransW         ( self, instName ): return self.getTransDSpec ( instName, AnalogDesign.SPEC_TRANS_W )
    def getTransL         ( self, instName ): return self.getTransDSpec ( instName, AnalogDesign.SPEC_TRANS_L )
    def getTransM         ( self, instName ): return self.getTransDSpec ( instName, AnalogDesign.SPEC_TRANS_M )
    def getTransMInt      ( self, instName ): return self.getTransDSpec ( instName, AnalogDesign.SPEC_TRANS_MINT )
    def getTransDummy     ( self, instName ): return self.getTransDSpec ( instName, AnalogDesign.SPEC_TRANS_DUMMY )
    def getTransSFirst    ( self, instName ): return self.getTransDSpec ( instName, AnalogDesign.SPEC_TRANS_SFIRST )
    def getTransBulk      ( self, instName ): return self.getTransDSpec ( instName, AnalogDesign.SPEC_TRANS_BULK )
    def getTransBulk_Conn ( self, instName ): return self.getTransDSpec ( instName, AnalogDesign.SPEC_TRANS_BULK_CONN )
    def getCapaC          ( self, instName ): return self.getCapaDSpec  ( instName, AnalogDesign.SPEC_CAPA_C )
    def getCapaMatrix     ( self, instName ): return self.getCapaDSpec  ( instName, AnalogDesign.SPEC_CAPA_MATRIX )
    def getCapaDummy      ( self, instName ): return self.getCapaDSpec  ( instName, AnalogDesign.SPEC_CAPA_DUMMY )

    def getDTransParam ( self, instName, paramName ):
        inst = self.cell.getInstance( instName )
        if not inst:
            raise Error( 3, [ 'AnalogDesign.getDTransParam(): No device "{}"'.format( instName ) ])
        device = inst.getMasterCell()
        if not issubclass(type(device),TransistorFamily): 
            raise Error( 3, [ 'AnalogDesign.getDTransParam(): Device "{}" is *not* a transistor ({})' \
                              .format( instName, type(device).__name__ ) ])
        param = device.getParameter( paramName )
        if not param:
            raise Error( 3, [ 'AnalogDesign.getDTransParam(): Device "{}" has no parameter "{}"' \
                              .format( instName, paramName ) ])
        return param

    def getDCapaParam ( self, instName, paramName ):
        inst = self.cell.getInstance( instName )
        if not inst:
            raise Error( 3, [ 'AnalogDesign.getDCapaParam(): No device "{}"'.format( instName ) ])
        device = inst.getMasterCell()
        if not issubclass(type(device),CapacitorFamily): 
            raise Error( 3, [ 'AnalogDesign.getDCapaParam(): Device "{}" is *not* a capaitor ({})' \
                              .format( instName, type(device).__name__ ) ])
        param = device.getParameter( paramName )
        if not param:
            raise Error( 3, [ 'AnalogDesign.getDCapaParam(): Device "{}" has no parameter "{}"' \
                              .format( instName, paramName ) ])
        return param

    def getDTransW     ( self, instName ): return self.getDTransParam( instName, 'W' )
    def getDTransL     ( self, instName ): return self.getDTransParam( instName, 'L' )
    def getDTransM     ( self, instName ): return self.getDTransParam( instName, 'M' )
    def getDCapaC      ( self, instName ): return self.getDCapaParam( instName, 'capacities' )
    def getDCapaMatrix ( self, instName ): return self.getDCapaParam( instName, 'matrix' )

    def getDevice ( self, instanceName ):
        inst = self.cell.getInstance( instanceName )
        if not inst:
            raise Error( 3, [ 'AnalogDesign.getDevice(): No instance "{}"'.format( instanceName ) ])
        return inst.getMasterCell()

    def readParameters ( self, path ):
        trace( 110, ',+', '\tReading Oceane parameters from \"%s\"\n' % path )

        if not path: return
        self.parameters.read( path );

        for dspec in self.devicesSpecs:
            if isinstance(dspec[0],Cell):
                pass
            elif issubclass(dspec[0],TransistorFamily):
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
            elif issubclass(dspec[0],CapacitorFamily):
                Cname       = dspec[1]
                Cparameters = self.parameters.getCapacitor( Cname )
                if not Cparameters:
                    raise Error( 3, [ 'AnalogDesign.readParameters(): Missing parameters for capacity \"%s\".' % Cname ] )
                    continue
                dspec[4] = Cparameters.C * 1e+12
                trace( 110, '\t- \"%s\" : C:%fpF\n' % (Cname ,dspec[4]) )
            elif issubclass(dspec[0],ResistorFamily):
                print( WarningMessage( 'Resistor devices are not supported yet by Oceane parser (instance:"{}").'.format(dspec[1]) ))
            else:
                print( WarningMessage( 'Unsupported analog device type {0} (instance:"{1}").'.format(dspec[0],dspec[1]) ))
        trace( 110, '-,' )
        return


    def doDevice ( self, count, dspec ):
        self.checkBeginCell( 'AnalogDesign.doDevice()' )
        if len(dspec) == 2:
            self.checkDSpecDigital( count, dspec )
            if isinstance( dspec[0], str ):
                masterCell = CRL.AllianceFramework.get().getCell( dspec[0], CRL.Catalog.State.Views )
                instance   = Instance.create( self.cell
                                            , dspec[1]
                                            , masterCell
                                            , Transformation()
                                            , Instance.PlacementStatus.UNPLACED )
                self.__dict__[ dspec[1] ] = instance
            else:
                masterCell = dspec[0]
                instance   = Instance.create( self.cell
                                            , dspec[1]
                                            , masterCell
                                            , Transformation()
                                            , Instance.PlacementStatus.UNPLACED )
                self.__dict__[ dspec[1] ] = instance
        else:
            self.checkDSpec( count, dspec )

            trace( 110, '\t==============================================================\n' )
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
                if (len(dspec) > 14):
                    for wiringSpec in dspec[14].split(' '):
                        fields = wiringSpec.split('.')
                        if len(fields) > 1:
                            device.getParameter( fields[0]+'.t' ).setValue( fields[1] )
                
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
                device.getParameter( 'matrix'  ).setMatrix(     dspec[5]  )
                device.setDummy( dspec[6] )
                for i in range(len(capaValues)):
                    device.getParameter( 'capacities' ).setValue( i, capaValues[i]  )

            elif isderived(dspec[0],ResistorFamily):
                print( dspec )
                device = dspec[0].create( self.library, dspec[1], dspec[3] )
                device.getParameter( 'R'     ).setValue(       dspec[4]  )
                device.getParameter( 'W'     ).setValue( toDbU(dspec[5]) )
                device.getParameter( 'L'     ).setValue( toDbU(dspec[6]) )
                device.getParameter( 'bends' ).setValue(       dspec[7]  )
                trace( 100, '\tW:{0}\n'.format(dspec[5]) )
                trace( 100, '\tpW:{0}\n'.format(device.getParameter('W')) )
                trace( 100, '\tbends:{0}\n'.format(dspec[7]) )
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
            trace( 100, '\tAdd Instance:{0}\n'.format(dspec[1]) )
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
        if not 'type' in node:
            raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"type\" key/element.'
                            ] + self.showNode(node) )
        nodeType = node['type']
        if nodeType not in [VNode, HNode, DNode]:
            raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"type\" must be one of VNode, HNode or DNode.'
                            ] + self.showNode(node) )

        if nodeType == DNode:
            if not 'device' in node:
                raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"device\" key/element.'
                                ] + self.showNode(node) )
            if not isinstance(node['device'],str):
                raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"device\" value *must* be of type str.'
                                ] + self.showNode(node) )
            if not 'span' in node:
                raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"span\" key/element.'
                                ] + self.showNode(node) )
            if    not isinstance(node['span'],tuple)    \
               or len(node['span']) != 3                \
               or not isinstance(node['span'][0],float) \
               or not isinstance(node['span'][1],float) \
               or not isinstance(node['span'][2],float):
                raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"span\" value *must* be a tuple of 3 floats.'
                                ] + self.showNode(node) )
            if not 'NF' in node:
                raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"NF\" key/element.'
                                ] + self.showNode(node) )
            if not isinstance(node['NF'],int):
                raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"NF\" value *must* be of type int.'
                                ] + self.showNode(node) )
        else:
            if isRoot:
              if not 'toleranceRatioH' in node:
                  raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"toleranceRationH\" key/element in root node.'
                                  ] + self.showNode(node) )
              if not 'toleranceRatioW' in node:
                  raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"toleranceRationW\" key/element in root node.'
                                  ] + self.showNode(node) )
              if not 'toleranceBandH' in node:
                  raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"toleranceBandH\" key/element in root node.'
                                  ] + self.showNode(node) )
              if not 'toleranceBandW' in node:
                  raise Error( 3, [ 'AnalogDesign.doSlicingTree(): Missing mandatory \"toleranceBandW\" key/element in root node.'
                                  ] + self.showNode(node) )
              if not 'children' in node:
                  print( Error( 3, [ 'AnalogDesign.doSlicingTree(): Suspicious root node without children.'
                                   ] + self.showNode(node) ))
            if 'children' in node:
                if not isinstance(node['children'],list):
                    raise Error( 3, [ 'AnalogDesign.doSlicingTree(): \"children\" value *must* be of type list.' ]
                                    + self.showNode(node) )

        if 'symmetries' in node:
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

    def addDevice ( self, name, align, parameter=None, index=0 ):
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
