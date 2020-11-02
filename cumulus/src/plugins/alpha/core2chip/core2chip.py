# -*- coding: utf-8 -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2019-2020, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/core2chip/core2chip.py"         |
# +-----------------------------------------------------------------+

"""
Automatically generates the chip+corona hierarchical netlists from
the *core* of a design.

* *corona* will contains:

  1. Exactly **one** instance of the *core* Cell.
  2. All the complex wiring to the pads, typically the *ouput enable*
     (``oe``) signal connected to the I/O pads of all bits of a bus.

* *chip* will contains:

  1. Exactly **one** instance of the *corona* Cell.
  2. All the I/O pads.

The double level chip+corona serves two purpose:

1. The standard router will be run on the *corona* only, removing
   the need for it to be able to manages the I/O pads. The routing at
   chip level, that is connecting the external pins from the *corona*
   to the inner sides of the pads, is straightforward. Mostly a straight
   wire.

2. In cases were the *corona* is symbolic layout, but we are required to
   use the I/O pads from the foundry (so they are *real* layout), the
   boundary between symbolic and real is much easier to manage at
   corona/chip level. In that case, the connexion may look like a
   small dogleg instead of a straight wire. 
"""

from   __future__      import print_function
from   exceptions      import NotImplementedError
import re
from   Hurricane       import UpdateSession, Net, Instance
from   CRL             import Catalog, AllianceFramework
from   helpers         import trace, netDirectionToStr
from   helpers.overlay import UpdateSession
from   helpers.io      import ErrorMessage, WarningMessage
import plugins.chip
from   plugins.alpha.block.block         import Block
from   plugins.alpha.block.configuration import BlockConf, IoPadConf


# -------------------------------------------------------------------
# Class : "IoNet".

class IoNet ( object ):
    """
    Handle/create the four following nets:

    1. ``self.coreNet``    : The original net from the *core*.
    2. ``self.coronaNet``  : The same net at *corona* level.
    3. ``self.chipIntNet`` : The same net at *chip* level (connected to the
       *internal* terminal of the I/O pad.
    4. ``self.chipExtNet`` : The corresponding net on the *outside* of the
       I/O pad, that is the bonding area. They make the interface of the
       chip itself.

    From the name of the core signal we derives the names of those four
    signals and the instance name of the I/O pad itself.

    It also check:

    * If the signal is a bit from a vector (flag ``IsElem``), to keep
      vectorization across hierarchical level and index the I/O pad name.
    * If the signal is an *output enable*, in that case, no I/O pad
      nor chip external signal should be generated.
    """

    IsElem         = 0x0001
    IsEnable       = 0x0002
    DoExtNet       = 0x0008
    PadPassthrough = 0x0010
    reVHDLVector   = re.compile( r'(?P<name>[^(]*)\((?P<index>[\d]+)\)$' )
    
    def __init__ ( self, coreToChip, coreNet ):
        self.coreToChip      = coreToChip
        self._flags          = 0
        self._chipExtNetName = None    # In the case of bidir pad, force external net name.
        self.coreNet         = coreNet
        self.coronaNet       = None    # Corona net going from core to corona.
        self.chipIntNet      = None    # Chip net going from corona to I/O pad.
        self.chipExtNet      = None    # Chip net going from I/O pad to the outside world.
        m = IoNet.reVHDLVector.match( self.coreNet.getName() )
        if m:
            self._flags |= IoNet.IsElem
            self._name   = m.group('name')
            self._index  = m.group('index')
        else:
            self._name   = self.coreNet.getName()
            self._index  = 0
        self._type = self.coreToChip.getNetType( self._name )
        trace( 550, '\tIoNet.__init__(): {}\n'.format(self) )
    
    def __repr__ ( self ):
        return '<IoNet "{}" ext:"{}" int:"{}">'.format( self.coreNet.getName()
                                                      , self.chipExtNet
                                                      , self.chipIntNet )
    
    def isElem     ( self ): return self._flags & IoNet.IsElem
    def isEnable   ( self ): return self._flags & IoNet.IsEnable
    def isGlobal   ( self ): return self.isGlobal( self._name )
    def isSpecial  ( self ): return self._type != Net.Type.LOGICAL
    def setFlags   ( self, flags ): self._flags |=  flags
    def resetFlags ( self, flags ): self._flags &= ~flags
    
    @property
    def name ( self ):
        if self.isSpecial() and self._chipExtNetName:
            return self._chipExtNetName
        return self._name
    
    @property
    def index ( self ): return self._index
    
    @property
    def coreNetName ( self ): return self.coreNet.getName()
    
    @property
    def coronaNetName ( self ):
        s = self._name
        if self.coreNet.getDirection() & Net.Direction.IN:
          s += '_from_pad'
        elif self.coreNet.getDirection() & Net.Direction.OUT:
          s += '_to_pad'
        if self._flags & IoNet.IsElem: s  += '({})'.format(self._index)
        return s
    
    @property
    def chipIntNetName ( self ):
        return self.chipIntNet.getName() if self.chipIntNet else self.coronaNetName
    
    @property
    def chipExtNetName ( self ):
        if self._chipExtNetName is not None: return self._chipExtNetName
        if self._flags & IoNet.IsEnable: return 'None'
        if self.chipExtNet: return self.chipExtNet.getName()
        s = self._name
        if self._flags & IoNet.IsElem: s  += '({})'.format(self._index)
        return s
    
    @chipExtNetName.setter
    def chipExtNetName ( self, name ): self._chipExtNetName = name
    
    @property
    def padInstanceName ( self ):
        s = self._name
        if self._flags & IoNet.IsElem: s  += '_{}'.format(self._index)
        return s
    
    def setEnable ( self, state ):
        if state == True: self._flags |=  IoNet.IsEnable
        else:             self._flags &= ~IoNet.IsEnable
    
    def buildNets ( self ):
        """
        Creates the signals in corona and chip Cells, then connect them
        together.
        """
        netType = Net.Type.LOGICAL
        if self.coreNet.isPower (): netType = Net.Type.POWER
        if self.coreNet.isGround(): netType = Net.Type.GROUND
        if self.coreNet.isClock (): netType = Net.Type.CLOCK
        # Corona net, connect to Core instance net.
        if not self.coronaNet:
            self.coronaNet  = Net.create( self.coreToChip.corona, self.coronaNetName )
            self.coronaNet.setExternal ( True )
            self.coronaNet.setDirection( self.coreNet.getDirection() )
            if netType != Net.Type.LOGICAL:
                self.coronaNet.setType  ( netType )
                self.coronaNet.setGlobal( True )
            self.coreToChip.icore.getPlug( self.coreNet ).setNet( self.coronaNet  )
        # Chip "internal" net, connect Corona instance net to I/O inside the chip.
        if not self.chipIntNet:
            self.chipIntNet = Net.create( self.coreToChip.chip, self.coronaNetName )
            if netType != Net.Type.LOGICAL:
                self.chipIntNet.setType( netType )
            self.coreToChip.icorona.getPlug( self.coronaNet ).setNet( self.chipIntNet )
        # Chip "external" net, connected to the pad I/O to the outside world.
        if self._flags & IoNet.PadPassthrough:
            self.chipExtNet = self.chipIntNet
        elif not self.chipExtNet and (self._flags & IoNet.DoExtNet):
            self.chipExtNet = self.coreToChip.chip.getNet( self.chipExtNetName )
            if not self.chipExtNet:
                self.chipExtNet = Net.create( self.coreToChip.chip, self.chipExtNetName )
        if self.chipExtNet:
            self.chipExtNet.setExternal ( True )
            self.chipExtNet.setDirection( self.coreNet.getDirection() )


# -------------------------------------------------------------------
# Class : "IoPad".

class IoPad ( object ):

    """
    Manage I/O pad instanciation. Gather between one and three IoNet.
    The number of IoNet implies the kind of I/O pad to be used.

    1. One IoNet mean either an input pad or an output pad.
    2. Two IoNets mean an tristate output pad (the signal and it's
       output enable).
    3. Three IoNets mean a bidirectional I/O pad (the signal as input,
       the signal as output and it's direction).
    """

    IN          = 0x0001
    OUT         = 0x0002
    BIDIR       = 0x0004
    TRI_OUT     = 0x0008
    UNSUPPORTED = 0x0010
  
    @staticmethod
    def directionToStr ( direction ):
        if direction == IoPad.IN:          return "IN"
        if direction == IoPad.OUT:         return "OUT"
        if direction == IoPad.BIDIR:       return "BIDIR"
        if direction == IoPad.TRI_OUT:     return "TRI_OUT"
        if direction == IoPad.UNSUPPORTED: return "UNSUPPORTED"
        return "Invalid value"
  
    def __init__ ( self, coreToChip, ioPadConf ):
        self.coreToChip = coreToChip
        self.ioPadConf  = ioPadConf
        self.direction  = 0
        self.nets       = []
        return

    @property
    def padInstanceName ( self ): return self.ioPadConf.instanceName

    @property
    def pads ( self ):
        return self.ioPadConf.pads
  
    def __str__ ( self ):
        s = '<IoPad "{}" '.format(self.padInstanceName)
        for ioNet in self.nets:
            s += ' {}'.format(ioNet.coreNetName)
        s += '>'
        return s
  
    def addNet ( self, ioNet ):
        """
        Add an IoNet. Between one and three IoNets are allowed. Corresponding
        respectively to simple input/output pad, tristate output pad and
        bidirectional pad. See the class definition.
        """
        self.nets.append( ioNet )
        if len(self.nets) == 1:
            if   self.nets[0].coreNet.getDirection() == Net.Direction.IN:  self.direction = IoPad.IN
            elif self.nets[0].coreNet.getDirection() == Net.Direction.OUT: self.direction = IoPad.OUT
            elif self.nets[0].coreNet.getName()      == 'scout':           self.direction = IoPad.OUT
            else:
                raise ErrorMessage( 1, 'IoPad.addNet(): Unsupported direction {} ({}) for core net "{}" in I/O pad \"{}\".' \
                                       .format( self.nets[0].coreNet.getDirection()
                                              , netDirectionToStr(self.nets[0].coreNet.getDirection())
                                              , self.nets[0].coreNet.getName()
                                              , self.padInstanceName ))
            if self.coreToChip.getPadInfo(self.direction) is None:
                print( WarningMessage( 'IoPad.addNet(): No simple pad in direction {} for "{}", fallback to bi-directional.' \
                                       .format(netDirectionToStr(self.direction),ioNet.padInstanceName)) )
                self.direction = IoPad.BIDIR
        elif len(self.nets) == 2:
            if self.direction != IoPad.BIDIR:
                self.direction = IoPad.TRI_OUT
        elif len(self.nets) == 3:
            self.direction = IoPad.BIDIR
        else:
            message = [ 'IoPad.addNet(): More than three core nets ({}) on I/O pad "{}".' \
                        .format(len(self.nets),self.padInstanceName) ]
            for ioNet in self.nets:
                message += [ "{}".format(ioNet.coreNet) ]
            raise ErrorMessage( 1, message )
        return
  
    def createPad ( self ):
        """
        Actually perform the I/O pad instanciation and triggers the IoNet
        signal creations according to context. For example, the output
        enable signal (if any) do not need an external chip signal
        (it is *not* connected to the outside world).
        """
        padInfo = self.coreToChip.getPadInfo( self.direction )
        if padInfo is None:
            raise ErrorMessage( 1, 'IoPad.createPad(): Unsupported direction {} ({}) for pad "{}".' \
                                   .format( self.direction
                                          , IoPad.directionToStr(self.direction)
                                          , self.padInstanceName ))
        connexions = []
        # Case of BIDIR as fallback for simple IN/OUT.
        if (self.direction == IoPad.BIDIR) and (len(self.nets) < 3):
            self.nets[0].setFlags( IoNet.DoExtNet )
            self.nets[0].buildNets()
            self.nets[1].buildNets()
            connexions.append( ( self.nets[0].chipExtNet, padInfo.padNet ) )
            if self.nets[0].coreNet.getDirection() == Net.Direction.IN:
                connexions.append( ( self.nets[0].chipIntNet      , padInfo.inputNet  ) )
                connexions.append( ( self.coreToChip.newDummyNet(), padInfo.outputNet ) )
            else:
                connexions.append( ( self.nets[0].chipIntNet      , padInfo.outputNet ) )
                connexions.append( ( self.coreToChip.newDummyNet(), padInfo.inputNet  ) )
            connexions.append( ( self.nets[1].chipIntNet, padInfo.enableNet  ) )
        else:
            for ioNet in self.nets:
                if not ioNet.isEnable():
                    ioNet.setFlags( IoNet.DoExtNet )
                ioNet.buildNets()
            connexions.append( ( self.nets[0].chipExtNet, padInfo.padNet ) )
            if self.direction == IoPad.IN:
                connexions.append( ( self.nets[0].chipIntNet, padInfo.inputNet  ) )
            elif self.direction == IoPad.OUT:
                connexions.append( ( self.nets[0].chipIntNet, padInfo.outputNet ) )
            elif self.direction == IoPad.TRI_OUT:
                connexions.append( ( self.nets[0].chipIntNet, padInfo.inputNet  ) )
                connexions.append( ( self.nets[1].chipIntNet, padInfo.enableNet ) )
            elif self.direction == IoPad.BUDIR:
                connexions.append( ( self.nets[0].chipIntNet, padInfo.inputNet  ) )
                connexions.append( ( self.nets[1].chipIntNet, padInfo.outputNet ) )
                connexions.append( ( self.nets[2].chipIntNet, padInfo.enableNet ) )
        self.pads.append( Instance.create( self.coreToChip.chip
                                         , self.padInstanceName
                                         , self.coreToChip.getCell(padInfo.name) ) )
        for connexion in connexions:
            CoreToChip._connect( self.pads[0], connexion[0], connexion[1] )
        self.coreToChip.chipPads += self.pads


# -------------------------------------------------------------------
# Class : "CoreToChip".

class CoreToChip ( object ):
    """
    Semi-automated build of a complete chip with I/O pad around a Cell core.
    ``Core2Chip`` expect configuration informations to be present in the
    state attribute of the Block class. So a Block class must be created
    before calling it.

    At chip level we distinguish bewteen two kind of signals connecting the
    I/O pads:

    1. *Ring signals*, power supplies and optionally clock(s) that have a
       special way of connecting to the pads. They go *through* the I/O pad
       in east/west and therefore constitute a complete ring around the
       chip (and over the pad). In certain cases there can be some
       disruption into that ring for analog pads or different clock
       domains.
   
    2. *Normal signals*, that is, digital/analog signals coming and going
       to the core actually bearing information.
    """

    class IoPadInfo ( object ):

        def __init__ ( self, flags, padName, padNet, coreNets ):
            self.flags    = flags
            self.name     = padName
            self.padNet   = padNet
            self.coreNets = coreNets
            return

        @property
        def inputNet ( self ):
            if self.flags & (IoPad.IN|IoPad.BIDIR|IoPad.TRI_OUT):
                return self.coreNets[0]
            return None

        @property
        def outputNet ( self ):
            if self.flags & (IoPad.IN|IoPad.BIDIR|IoPad.TRI_OUT):
                return self.coreNets[1]
            elif self.flags & IoPad.OUT:
                return self.coreNets[0]
            return None

        @property
        def enableNet ( self ):
            if self.flags & IoPad.BIDIR:
                return self.coreNets[2]
            elif self.flags & IoPad.TRI_OUT:
                return self.coreNets[1]
            return None


    def getNetType ( self, netName ):
        raise ErrorMessage( 1, 'coreToChip.getNetType(): This method must be overloaded.' )

    def isGlobal ( self, netName ):
        raise ErrorMessage( 1, 'coreToChip.isGlobal(): This method must be overloaded.' )

    def getCell ( self, masterCellName ):
        raise ErrorMessage( 1, 'coreToChip.getCell(): This method must be overloaded.' )

    @staticmethod
    def _connect ( instance, netO, masterNetO=None ):
        """
        Connect ``netO`` to the plug of ``masterNetO`` on ``instance``.

        :param instance:   The instance to work on.
        :param netO:       The signal to connect to, could be either the net *name*
                           or the net object itself.
        :param masterNetO: The master net (interal net) of the plug. Could be either
                           the net *name* or the net object. If not supplied,
                           ``_connect()`` will look for a master net of the same
                           name as ``netO``
        """
        if isinstance(netO,Net): chipNet = netO
        else:                    chipNet = instance.getCell().getNet( netO )
        
        if   not masterNetO:             masterNet = instance.getMasterCell().getNet( chipNet.getName() )
        elif isinstance(masterNetO,Net): masterNet = masterNetO
        else:                            masterNet = instance.getMasterCell().getNet( masterNetO )
        
        instance.getPlug( masterNet ).setNet( chipNet )
        return

    def __init__ ( self, core ):
        if isinstance(core,Block):
            state = core.state
        elif isinstance(core,BlockConf):
            state = core
        else:
            block = Block.lookup( core )
            if not block:
                raise ErrorMessage( 1, [ 'Core2Chip.__init__(): Core cell "{}" has no Block defined.' \
                                         .format( core.getName() )
                                       ] )
            state = block.state
        self.state          = state
        self.ringNetNames   = []
        self.ioPadInfos     = []
        self.chipPads       = []
        self.padLib         = None
        self.corona         = None
        self._ioNets        = {}
        self.powerPadCount  = 0
        self.groundPadCount = 0
        self.clockPadCount  = 0
        self.dummyNetCount  = 0
        return

    @property
    def core ( self ): return self.state.cell

    @property
    def icore ( self ): return self.state.icore

    @property
    def icorona ( self ): return self.state.icorona

    @property
    def chip ( self ): return self.state.chip

    def getPadInfo ( self, padType ):
        for ioPadInfo in self.ioPadInfos:
            if ioPadInfo.flags & padType:
                return ioPadInfo
        return None

    def hasIoNet ( self, netName ):
        """
        Look for an IoNet associated to *core* net ``netName``.
        """
        if self._ioNets.has_key(netName): return True
        return False

    def newDummyNet ( self ):
        dummy = Net.create( self.chip, '{}_dummy_{}'.format(self.chip.getName(),self.dummyNetCount) )
        self.dummyNetCount += 1
        return dummy

    def getIoNet ( self, coreNet ):
        """
        Lookup, and create if it doesn't exist, for an IoNet associated to *core*
        net ``coreNet``.

        :param coreNet: The core net signal, could be either the net *name*
                        or the net object itself.
        """
        if isinstance(coreNet,str):
            if self._ioNets.has_key(coreNet): return self._ioNet[ coreNet ]
            raise ErrorMessage( 1, 'CoreToChip.getIoNet(): Cannot lookup net "%s" by name.' % coreNet )
        if not self._ioNets.has_key(coreNet.getName()):
            self._ioNets[ coreNet.getName() ] = IoNet( self, coreNet )
        return self._ioNets[ coreNet.getName() ]

    def _connectPadRing ( self, padInstance ):
        """Connect ring signals to the I/O pad."""
        for masterNetName, netName in self.ringNetNames.items():
            trace( 550, '\tCoreToChip._connectPadRing(): master:{} net:{}\n'.format(masterNetName,netName) )
            CoreToChip._connect( padInstance, netName, masterNetName )

    def _connectRing ( self ):
        """Create the pad ring signals and connect them to all pad instances"""
        for pad in self.chipPads:
            self._connectPadRing( pad )

    def _buildStandardPad ( self, ioNet ):
        """
        Build all the I/O pads for normal signals (that are not related to
        power or clock).
        """
        padInfo = self.getPadInfo( ioNet.coreNet.getDirection() )
        if not padInfo:
            raise ErrorMessage( 1, 'CoreToChip._buildStandardPad(): Unsupported direction %d (%s) for core net "%s".' \
                                   % (ioNet.coreNet.getDirection()
                                     ,netDirectionToStr(ioNet.coreNet.getDirection())
                                     ,ioNet.coreNet.getName()) )
        ioNet.pads.append( Instance.create( self.chip
                                          , ioNet.padInstanceName
                                          , self.getCell(padInfo.name) ) )
        CoreToChip._connect( ioNet.pads[0], ioNet.chipExtNet, padInfo.padNet )
        CoreToChip._connect( ioNet.pads[0], ioNet.chipIntNet, padInfo.coreNets[0] )
        self.chipPads += ioNet.pads

    def _buildCoreGroundPads ( self, ioPadConf ):
        """Build I/O pad relateds to core ground signals."""
        raise NotImplementedError( 'coreToChip._buildGroundPads(): This method must be overloaded.' )

    def _buildIoGroundPads ( self, ioPadConf ):
        """Build I/O pad relateds to pad internal ground signals."""
        raise NotImplementedError( 'coreToChip._buildGroundPads(): This method must be overloaded.' )

    def _buildAllGroundPads ( self, ioPadConf ):
        """Build I/O pad relateds to ground signals for core and pads."""
        raise NotImplementedError( 'coreToChip._buildGroundPads(): This method must be overloaded.' )

    def _buildCorePowerPads ( self, ioPadConf ):
        """Build I/O pad relateds to core power signals."""
        raise NotImplementedError( 'coreToChip._buildPowerPads(): This method must be overloaded.' )

    def _buildIoPowerPads ( self, ioPadConf ):
        """Build I/O pad relateds to pad internal power signals."""
        raise NotImplementedError( 'coreToChip._buildPowerPads(): This method must be overloaded.' )

    def _buildAllPowerPads ( self, ioPadConf ):
        """Build I/O pad relateds to power signals for core and pads."""
        raise NotImplementedError( 'coreToChip._buildPowerPads(): This method must be overloaded.' )

    def _buildClockPads ( self, ioPadConf ):
        """Build I/O pad relateds to clock signals."""
        raise NotImplementedError( 'coreToChip._buildClockPads(): This method must be overloaded.' )

    def _connectClocks ( self ):
        """Connect inner clocks signal to the corona (towards the core) ."""
        raise NotImplementedError( 'coreToChip._connectClocks(): This method must be overloaded.' )

    def buildChip ( self ):
        """
        Build the whole chip+corona hierarchical structure (netlist only)
        from the core cell.
        """
        af = AllianceFramework.get()
        self.state.cfg.apply()
        with UpdateSession():
            print( '  o  Build Chip from Core.' )
            print( '     - Core:   "{}".'.format(self.state.cell.getName()) )
            print( '     - Corona: "{}".'.format('corona') )
            print( '     - Chip:   "{}".'.format(self.state.chipConf.name) )
            self.state.chip = af.createCell( self.state.chipConf.name )
            self.corona     = af.createCell( 'corona' )
            self.state.icore   = Instance.create( self.corona    , 'core'  , self.state.cell )
            self.state.icorona = Instance.create( self.state.chip, 'corona', self.corona     )
            ioPads      = []
            clockIoNets = []
            for ioPadConf in self.state.chipConf.padInstances:
                if ioPadConf.isAllPower():
                    self._buildAllPowerPads( ioPadConf )
                    continue
                if ioPadConf.isCorePower():
                    self._buildCorePowerPads( ioPadConf )
                    continue
                if ioPadConf.isIoPower():
                    self._buildIoPowerPads( ioPadConf )
                    continue
                if ioPadConf.isAllGround():
                    self._buildAllGroundPads( ioPadConf )
                    continue
                if ioPadConf.isCoreGround():
                    self._buildCoreGroundPads( ioPadConf )
                    continue
                if ioPadConf.isIoGround():
                    self._buildIoGroundPads( ioPadConf )
                    continue
                if ioPadConf.isClock():
                    self._buildClockPads( ioPadConf )
                    continue
                ioPadConf.udata = IoPad( self, ioPadConf )
                for netName in ioPadConf.nets:
                    if netName is None: continue
                    coreNet = self.core.getNet( netName )
                    if not coreNet:
                        raise ErrorMessage( 1, 'CoreToChip.buildChip(): "{}" doesn\'t have a "{}" net.' \
                                               .format(self.core.getName(),netName) )
                    ioNet = self.getIoNet( coreNet )
                    if ioPadConf.isBidir() or ioPadConf.isTristate():
                        if coreNet.getName() == ioPadConf.enableNetName:
                            ioNet.setEnable( True )
                    if not ioNet.isEnable():
                        ioNet.chipExtNetName = ioPadConf.padNetName
                    ioPadConf.udata.addNet( ioNet )
                ioPads.append( ioPadConf )
            trace( 550, '\tProcessed all IoPadConf, looking for orphaned core nets...\n' )
            for coreNet in self.core.getNets():
                if   not coreNet.isExternal(): continue
                elif coreNet.isPower(): continue
                elif coreNet.isGround(): continue
                elif coreNet.isClock(): continue
                elif self.hasIoNet(coreNet.getName()): continue
                # Remaining non configured Standard I/O pads.
                ioNet     = IoNet( self, coreNet )
                directPad = IoPadConf( ( 0      # Unkown side.
                                       , None   # Unknow position.
                                       , ioNet.padInstanceName
                                       , ioNet.chipExtNetName
                                       , ioNet.coreNetName
                                       ) )
                directPad.udata = IoPad( self, directPad.instanceName )
                directPad.udata.addNet( ioNet )
                ioPads.append( directPad )
                trace( 550, '\tNon-configured core net {}, adding {}\n'.format(coreNet,directPad) )
            for ioPad in ioPads:
                ioPad.udata.createPad()
            self._connectRing()
            self._connectClocks()
        af.saveCell( self.chip  , Catalog.State.Logical )
        af.saveCell( self.corona, Catalog.State.Logical )
