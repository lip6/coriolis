
# -*- coding: utf-8 -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2019-2023, All Rights Reserved
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

#from   exceptions      import NotImplementedError
import re
from   ...Hurricane          import DbU, UpdateSession, Net, Instance, Transformation, Box
from   ...CRL                import Catalog, AllianceFramework, Spice, DefImport
from   ...helpers            import trace, netDirectionToStr
from   ...helpers.overlay    import UpdateSession
from   ...helpers.io         import ErrorMessage, WarningMessage
from   ..rsave               import rsave
from   ..checks              import oneDriver
from   ..utils               import getPlugByName
from   ..block.block         import Block
from   ..block.configuration import BlockConf, IoPadConf, ConstantsConf
            

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
    IsAnalog       = 0x0004
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
        if self.coreToChip.useHarness(): self._flags |= IoNet.PadPassthrough
        self._type = self.coreToChip.getNetType( self._name )
        trace( 550, '\tIoNet.__init__(): {}\n'.format(self) )
    
    def __repr__ ( self ):
        return '<IoNet "{}" ext:"{}" int:"{}">'.format( self.coreNet.getName()
                                                      , self.chipExtNet
                                                      , self.chipIntNet )
    
    def isElem     ( self ): return self._flags & IoNet.IsElem
    def isEnable   ( self ): return self._flags & IoNet.IsEnable
    def isAnalog   ( self ): return self._flags & IoNet.IsAnalog
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
        if self.coreNet.isSupply(): return s
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
        if self._flags & IoNet.IsEnable:
            if self.coreToChip.useHarness():
                return 'io_oeb({})'.format( self._index )
            return 'None'
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

    @property
    def enableNetName ( self ):
        if self.coreToChip.useHarness():
            m = IoNet.reVHDLVector.match(self.chipExtNetName)
            if m:
                return 'io_oeb({})'.format( m.group('index') )
        return '{}_enable'.format(self.padInstanceName)   

    def setEnable ( self, state ):
        if state == True: self._flags |=  IoNet.IsEnable
        else:             self._flags &= ~IoNet.IsEnable
    
    def setAnalog ( self, state ):
        if state == True: self._flags |=  IoNet.IsAnalog
        else:             self._flags &= ~IoNet.IsAnalog
    
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
        # In case of harness, the chip external net should already exists.
        if self.coreToChip.useHarness():
            self.chipExtNet = self.coreToChip.chip.getNet( self.chipExtNetName )
            self.chipIntNet = self.chipExtNet
            if not self.chipExtNet:
                raise ErrorMessage( 1, 'IoNet.buildNets(): No harness net "{}" to connect core net "{}".' \
                                       .format( self.chipExtNetName, self.coreNet.getName() ))
        # Chip "internal" net, connect Corona instance net to I/O inside the chip.
        if not self.chipIntNet:
            chipIntNetName = "internal_" + self.coronaNetName
           #if self._flags & IoNet.IsAnalog:
           #    chipIntNetName = self.coronaNetName
            self.chipIntNet = Net.create( self.coreToChip.chip, chipIntNetName )
            if netType != Net.Type.LOGICAL:
                self.chipIntNet.setType( netType )
        coronaNetPlug = self.coreToChip.icorona.getPlug( self.coronaNet )
        if not coronaNetPlug.getNet():
            coronaNetPlug.setNet( self.chipIntNet )
        # Chip "external" net, connected to the pad I/O to the outside world.
        #if self._flags & (IoNet.PadPassthrough | IoNet.IsAnalog):
        if self._flags & IoNet.PadPassthrough:
            self.chipExtNet = self.chipIntNet
        elif not self.chipExtNet and (self._flags & IoNet.DoExtNet):
            self.chipExtNet = self.coreToChip.chip.getNet( self.chipExtNetName )
            if not self.chipExtNet:
                self.chipExtNet = Net.create( self.coreToChip.chip, self.chipExtNetName )
        if self.chipExtNet:
            self.chipExtNet.setExternal ( True )
            self.chipExtNet.setDirection( self.coreNet.getDirection() )
        trace( 550, '\tIoNet.buildNets() {}\n'.format( self ))


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
    ANALOG      = 0x0010
    UNSUPPORTED = 0x0020
    FILLER      = 0x0040
    CORNER      = 0x0080
    NON_CONNECT = 0x0100
  
    @staticmethod
    def directionToStr ( direction ):
        if direction == IoPad.IN:          return "IN"
        if direction == IoPad.OUT:         return "OUT"
        if direction == IoPad.BIDIR:       return "BIDIR"
        if direction == IoPad.TRI_OUT:     return "TRI_OUT"
        if direction == IoPad.ANALOG:      return "ANALOG"
        if direction == IoPad.UNSUPPORTED: return "UNSUPPORTED"
        if direction == IoPad.NON_CONNECT: return "NON_CONNECT"
        return "Invalid value"
  
    def __init__ ( self, coreToChip, ioPadConf ):
        self.coreToChip = coreToChip
        self.ioPadConf  = ioPadConf
        self.direction  = IoPad.NON_CONNECT
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
            s += ' "{}" <-> "{}"'.format(ioNet.coreNetName,ioNet.chipExtNetName)
        s += ' direction={}'.format(self.direction)
        s += '>'
        return s
  
    def addNet ( self, ioNet ):
        """
        Add an IoNet. Between one and three IoNets are allowed. Corresponding
        respectively to simple input/output pad, tristate output pad and
        bidirectional pad. See the class definition.
        """
        trace( 550, '\tIoPad.addNet() net={} iopad={}\n'.format(ioNet,self))
        self.nets.append( ioNet )
        if len(self.nets) == 1:
            if   self.nets[0].isAnalog():                                  self.direction = IoPad.ANALOG
            elif self.nets[0].coreNet.getDirection() == Net.Direction.IN:  self.direction = IoPad.IN
            elif self.nets[0].coreNet.getDirection() == Net.Direction.OUT: self.direction = IoPad.OUT
            elif self.nets[0].coreNet.getName()      == 'scout':           self.direction = IoPad.OUT
            else:
                raise ErrorMessage( 1, 'IoPad.addNet(): Unsupported direction {} ({}) for core net "{}" in I/O pad \"{}\".' \
                                       .format( self.nets[0].coreNet.getDirection()
                                              , netDirectionToStr(self.nets[0].coreNet.getDirection())
                                              , self.nets[0].coreNet.getName()
                                              , self.padInstanceName ))
            if self.coreToChip.getPadInfo(self.direction) is None:
                if     (self.direction == IoPad.OUT) \
                   and (self.coreToChip.getPadInfo(IoPad.TRI_OUT) is not None):
                    if not self.coreToChip.useHarness():
                        print( WarningMessage( 'IoPad.addNet(): No simple pad in direction {} for "{}", fallback to output tristate.' \
                                               .format(IoPad.directionToStr(self.direction),ioNet.padInstanceName)) )
                    self.direction = IoPad.TRI_OUT
                else:
                    if not self.coreToChip.useHarness():
                        print( WarningMessage( 'IoPad.addNet(): No simple pad in direction {} for "{}", fallback to bi-directional.' \
                                               .format(IoPad.directionToStr(self.direction),ioNet.padInstanceName)) )
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
        trace( 550, '\tIoPad.createPad() {}\n'.format( self ))
        padInfo = self.coreToChip.getPadInfo( self.direction )
        if padInfo is None:
            if len(self.nets) == 0:
                raise ErrorMessage( 1, [ 'IoPad.createPad(): Cannot find net(s) connected(s) to pad "{}".' \
                                         .format( self.padInstanceName )
                                       , '({})'.format( self.ioPadConf ) ] )
                    
            raise ErrorMessage( 1, 'IoPad.createPad(): Unsupported direction {} ({}) for pad "{}".' \
                                   .format( self.direction
                                          , IoPad.directionToStr(self.direction)
                                          , self.padInstanceName ))
        connexions = []
        if (self.direction == IoPad.ANALOG):
            self.nets[0].setFlags( IoNet.DoExtNet )
            self.nets[0].buildNets()
            connexions.append( ( self.nets[0].chipExtNet, padInfo.padNet ) )
            connexions.append( ( self.nets[0].chipIntNet, padInfo.coreNets[1] ) )
        elif (self.direction == IoPad.NON_CONNECT):
            pass
        elif (self.direction == IoPad.BIDIR) and (len(self.nets) < 3):
            # Case of BIDIR as fallback for simple IN/OUT.
            self.nets[0].setFlags( IoNet.DoExtNet )
            self.nets[0].buildNets()
            hasEnable = not self.coreToChip.useHarness() \
                        or self.nets[0].chipExtNetName.startswith('io_in') \
                        or self.nets[0].chipExtNetName.startswith('io_out')
            if hasEnable:
                if len(self.nets) < 2:
                    enableNet = self.coreToChip.newEnableForNet( self.nets[0] )
                    self.nets.append( self.coreToChip.getIoNet( enableNet ) )
                self.nets[1].buildNets()
            connexions.append( ( self.nets[0].chipExtNet, padInfo.padNet ) )
            if self.nets[0].coreNet.getDirection() == Net.Direction.IN:
                connexions.append( ( self.nets[0].chipIntNet      , padInfo.outputNet ) )
                connexions.append( ( self.coreToChip.newDummyNet(), padInfo.inputNet  ) )
            else:
                connexions.append( ( self.nets[0].chipIntNet      , padInfo.inputNet  ) )
                connexions.append( ( self.coreToChip.newDummyNet(), padInfo.outputNet ) )
            if hasEnable:
                connexions.append( ( self.nets[1].chipIntNet, padInfo.enableNet  ) )
        elif (self.direction == IoPad.TRI_OUT) and (len(self.nets) < 2):
            self.nets[0].setFlags( IoNet.DoExtNet )
            self.nets[0].buildNets()
            if len(self.nets) < 2:
                enableNet = self.coreToChip.newEnableForNet( self.nets[0] )
                self.nets.append( self.coreToChip.getIoNet( enableNet ) )
            self.nets[1].buildNets()
            connexions.append( ( self.nets[0].chipExtNet, padInfo.padNet    ) )
            connexions.append( ( self.nets[0].chipIntNet, padInfo.inputNet  ) )
            connexions.append( ( self.nets[1].chipIntNet, padInfo.enableNet ) )
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
            elif self.direction == IoPad.BIDIR:
                connexions.append( ( self.nets[0].chipIntNet, padInfo.inputNet  ) )
                connexions.append( ( self.nets[1].chipIntNet, padInfo.outputNet ) )
                connexions.append( ( self.nets[2].chipIntNet, padInfo.enableNet ) )
        if not self.coreToChip.useHarness():
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
    ``CoreToChip`` expect configuration informations to be present in the
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
        raise NotImplementedError( 'coreToChip.getNetType(): This method must be overloaded.' )

    def isGlobal ( self, netName ):
        raise NotImplementedError( 'coreToChip.isGlobal(): This method must be overloaded.' )

    def getCell ( self, masterCellName ):
        raise NotImplementedError( 'coreToChip.getCell(): This method must be overloaded.' )

    @staticmethod
    def _connect ( instance, netO, masterNetO=None ):
        """
        Connect ``netO`` to the plug of ``masterNetO`` on ``instance``.

        :param instance:   The instance to work on.
        :param netO:       The signal to connect to, could be either the net *name*
                           or the net object itself.
        :param masterNetO: The master net (internal net) of the plug. Could be either
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
            conf = core.state
        elif isinstance(core,BlockConf):
            conf = core
        else:
            block = Block.lookup( core )
            if not block:
                raise ErrorMessage( 1, [ 'CoreToChip.__init__(): Core cell "{}" has no Block defined.' \
                                         .format( core.getName() )
                                       ] )
            conf = block.state
       #conf._postInit()
        self.conf            = conf
        self.conf.useHarness = False
        self.ringNetNames    = []
        self.ioPadInfos      = []
        self.chipPads        = []
        self.padLib          = None
        self.corona          = None
        self._ioNets         = {}
        self.powerPadCount   = 0
        self.groundPadCount  = 0
        self.clockPadCount   = 0
        self.dummyNetCount   = 0
        return

    @property
    def core ( self ): return self.conf.cell

    @property
    def icore ( self ): return self.conf.icore

    @property
    def icorona ( self ): return self.conf.icorona

    @property
    def chip ( self ): return self.conf.chip

    def useHarness ( self ): return self.conf.useHarness

    def getPadInfo ( self, padType ):
        for ioPadInfo in self.ioPadInfos:
            if ioPadInfo.flags & padType:
                return ioPadInfo
        return None

    def hasIoNet ( self, netName ):
        """
        Look for an IoNet associated to *core* net ``netName``.
        """
        if netName in self._ioNets: return True
        return False

    def newDummyNet ( self ):
        """Create a dummy net in *chip* cell (for unconnected terminals)."""
        dummy = Net.create( self.chip, '{}_dummy_{}'.format(self.chip.getName(),self.dummyNetCount) )
        self.dummyNetCount += 1
        return dummy

    def newEnableForNet ( self, ioNet ):
        """
        Create a new enable signal, in *core* cell, to control the associated I/O pad.
        This is to be used in the case of bi-directional I/O pads used as simples inputs
        or outputs.
        """
        if   ioNet.coreNet.getDirection() & Net.Direction.IN:  constantType = ConstantsConf.ZERO
        elif ioNet.coreNet.getDirection() & Net.Direction.OUT: constantType = ConstantsConf.ONE
        else:
            raise ErrorMessage( 2, 'CoreToChip.newEnableForNet(): Net "{}" is neither IN nor OUT.' \
                                   .format(ioNet.coreNet.getName()) )
        instance = self.conf.constantsConf.createInstance( self.core, constantType )
        enable   = Net.create( self.core, ioNet.enableNetName )
        enable.setExternal ( True )
        enable.setDirection( Net.Direction.OUT )
        getPlugByName( instance, self.conf.constantsConf.output(constantType) ).setNet( enable )
        self.conf.addClonedCell( self.conf.core )
        return enable

    def getIoNet ( self, coreNet ):
        """
        Lookup, and create if it doesn't exist, for an IoNet associated to *core*
        net ``coreNet``.

        :param coreNet: The core net signal, could be either the net *name*
                        or the net object itself.
        """
        if isinstance(coreNet,str):
            if coreNet in self._ioNets: return self._ioNet[ coreNet ]
            raise ErrorMessage( 1, 'CoreToChip.getIoNet(): Cannot lookup net "%s" by name.' % coreNet )
        if not coreNet.getName() in self._ioNets:
            self._ioNets[ coreNet.getName() ] = IoNet( self, coreNet )
        return self._ioNets[ coreNet.getName() ]

    def _connectPadRing ( self, padInstance ):
        """Connect ring signals to the I/O pad."""
        for masterNetName, netName in self.ringNetNames.items():
            if masterNetName == 'iovss':
                netName = 'vss'
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
        raise NotImplementedError( 'coreToChip._buildPowerPads(): This method must be overloaded, on {}' \
                                   .format(ioPadConf) )

    def _buildClockPads ( self, ioPadConf ):
        """Build I/O pad relateds to clock signals."""
        if self.useHarness(): return
        raise NotImplementedError( 'coreToChip._buildClockPads(): This method must be overloaded.' )

    def _connectClocks ( self ):
        """Connect inner clocks signal to the corona (towards the core) ."""
        raise NotImplementedError( 'coreToChip._connectClocks(): This method must be overloaded.' )

    def _loadHarness ( self ):
        """Load the DEF file containing the reference harness layout."""
        raise NotImplementedError( 'coreToChip._loadHarness(): This method must be overloaded.' )

    def hasCornerCell ( self ):
        """Is there a special cell for corners. This method may be overloaded."""
        return False

    def hasFillerCells ( self ):
        """Is there a special cell(s) for filling gaps. This method may be overloaded."""
        return False

    def getCornerCell ( self, instanceName=None ):
        """Return the model of corner cell."""
        raise NotImplementedError( 'coreToChip.getCornerCell(): This method must be overloaded.' )

    def createSpacer ( self, gapWidth ):
        """Return a new instance of spacer cell."""
        raise NotImplementedError( 'coreToChip.createSpacer(): This method must be overloaded.' )

    def createCorner ( self ):
        """Return a new instance of corner cell."""
        raise NotImplementedError( 'coreToChip.createCorner(): This method must be overloaded' )

    def buildChip ( self ):
        """
        Build the whole chip+corona hierarchical structure (netlist only)
        from the core cell.
        """
        af = AllianceFramework.get()
        self.conf.cfg.apply()
        chipName = self.conf.chipConf.name
        if self.useHarness():
            self.conf.chip = self._loadHarness()
        else:
            self.conf.chip = af.createCell( self.conf.chipConf.name )
        with UpdateSession():
            print( '  o  Build Chip from Core.' )
            print( '     - Core:    "{}".'.format(self.conf.cell.getName()) )
            print( '     - Corona:  "{}".'.format('corona') )
            print( '     - Chip:    "{}".'.format(self.conf.chip.getName()) )
            self.corona       = af.createCell( 'corona' )
            self.conf.icore   = Instance.create( self.corona   , 'core'  , self.conf.cell )
            self.conf.icorona = Instance.create( self.conf.chip, 'corona', self.corona    )
            if self.useHarness():
                harnessBb = self.conf.chip.getBoundingBox()
                harnessAb = self.conf.chip.getAbutmentBox()
                self.conf.chip.setAbutmentBox( harnessBb )
                self.conf.corona.setAbutmentBox( Box( 0, 0, harnessAb.getWidth(), harnessAb.getHeight() ))
                self.conf.icorona.setTransformation( Transformation( harnessAb.getXMin()
                                                                   , harnessAb.getYMin()
                                                                   , Transformation.Orientation.ID ))
                self.conf.icorona.setPlacementStatus( Instance.PlacementStatus.FIXED )
            ioPads      = []
            clockIoNets = []
            for ioPadConf in self.conf.chipConf.padInstances:
                trace( 550, ',+', '\tioPadConf={}\n'.format( ioPadConf ))
                if ioPadConf.isAllPower():
                    self._buildAllPowerPads( ioPadConf )
                    trace( 550, '-' )
                    continue
                if ioPadConf.isCorePower():
                    self._buildCorePowerPads( ioPadConf )
                    trace( 550, '-' )
                    continue
                if ioPadConf.isIoPower():
                    self._buildIoPowerPads( ioPadConf )
                    trace( 550, '-' )
                    continue
                if ioPadConf.isAllGround():
                    self._buildAllGroundPads( ioPadConf )
                    trace( 550, '-' )
                    continue
                if ioPadConf.isCoreGround():
                    self._buildCoreGroundPads( ioPadConf )
                    trace( 550, '-' )
                    continue
                if ioPadConf.isIoGround():
                    self._buildIoGroundPads( ioPadConf )
                    trace( 550, '-' )
                    continue
                if ioPadConf.isClock():
                    self._buildClockPads( ioPadConf )
                    trace( 550, '-' )
                    continue
                if self.useHarness():
                    ioPadConf.flags = IoPadConf.BIDIR
                ioPadConf.udata = IoPad( self, ioPadConf )
                for netName in ioPadConf.nets:
                    if netName is None: continue
                    coreNet = self.core.getNet( netName )
                    if not coreNet:
                        print( ErrorMessage( 1, 'CoreToChip.buildChip(): Pad "{}" refer net "{}" which do not exist in core "{}".' \
                                                .format(ioPadConf.instanceName,netName,self.core.getName()) ))
                        continue
                    ioNet = self.getIoNet( coreNet )
                    if ioPadConf.isAnalog():
                        ioNet.setAnalog( True )
                    else:
                        if ioPadConf.isBidir() or ioPadConf.isTristate():
                            if coreNet.getName() == ioPadConf.enableNetName:
                                trace( 550, '\tFlag core net name "{}" as enable\n'.format( coreNet.getName() ))
                                ioNet.setEnable( True )
                        if not ioNet.isEnable() and not self.conf.useHarness:
                            trace( 550, '\tForce pad net name to "{}"\n'.format( ioPadConf.padNetName ))
                            ioNet.chipExtNetName = ioPadConf.padNetName
                    ioPadConf.udata.addNet( ioNet )
                ioPads.append( ioPadConf )
                trace( 550, '-' )
            trace( 550, '\tProcessed all IoPadConf, looking for orphaned core nets...\n' )
            for coreNet in self.core.getNets():
                if   not coreNet.isExternal(): continue
                elif coreNet.isPower(): continue
                elif coreNet.isGround(): continue
                elif coreNet.isClock(): continue
                elif self.hasIoNet(coreNet.getName()): continue
                # Remaining non configured Standard I/O pads.
                print( ErrorMessage( 1, 'CoreToChip.buildChip(): No pad configured for core signal "{}".' \
                                        .format(coreNet.getName()) ) )
                continue
               #ioNet     = IoNet( self, coreNet )
               #directPad = IoPadConf( ( 0      # Unkown side.
               #                       , None   # Unknow position.
               #                       , ioNet.padInstanceName
               #                       , ioNet.chipExtNetName
               #                       , ioNet.coreNetName
               #                       ) )
               #directPad.udata = IoPad( self, directPad )
               #directPad.udata.addNet( ioNet )
               #ioPads.append( directPad )
               #trace( 550, '\tNon-configured core net {}, adding {}\n'.format(coreNet,directPad) )
            for ioPad in ioPads:
                ioPad.udata.createPad()
            else:
                self._connectRing()
                self._connectClocks()
        oneDriver( self.chip )
        rsave( self.chip, views=Catalog.State.Logical, enableSpice=True )
        Spice.clearProperties()
