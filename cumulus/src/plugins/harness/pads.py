
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2021-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/harness/pads.py"                |
# +-----------------------------------------------------------------+


import sys
import re
from   ...Hurricane          import DbU, DataBase, UpdateSession, Breakpoint,  \
                                    Box, Transformation , Instance , Net, Pin, \
                                    Contact, Horizontal, Vertical, BasicLayer
from   ...CRL                import Catalog, AllianceFramework, DefImport, \
                                    RoutingGauge, RoutingLayerGauge
from   ...helpers            import trace, l, u, n, onFGrid
from   ...helpers.io         import ErrorMessage, WarningMessage
from   ...helpers.overlay    import CfgCache, UpdateSession
from   ..chip                import CoreWire
from   ..chip.constants      import importConstants
from   ..block.bigvia        import BigVia
from   ..core2chip.core2chip import CoreToChip as BaseCoreToChip, IoNet, IoPad

importConstants( globals() )


# --------------------------------------------------------------------
# Class  :  "harness.pads.Side"

class Side ( object ):

    def __init__ ( self, harness, sideType ):
        self.type      = sideType
        self.harness   = harness
        self.gap       = 0
        self.pins      = []
        self.coreWires = []
        self.padRails  = []
        if self.type == North:
            self.sideName = 'north'
            pins = self.harness.conf.chipConf.northPins
        elif self.type == South:
            self.sideName = 'south'
            pins = self.harness.conf.chipConf.southPins
        elif self.type == East:
            self.sideName = 'east'
            pins = self.harness.conf.chipConf.eastPins
        elif self.type == West:
            self.sideName = 'west'
            pins = self.harness.conf.chipConf.westPins
        else:
            raise ErrorMessage( 1, 'harness.Side.__init__(): Invalid value for sideType ({})'.format(sideType))
        for pin in pins:
            self.pins.append( pin )

    @property
    def conf ( self ): return self.harness.conf

    def addChipPin ( self, pin ):
        if self.hasChipPin(pin): return
        self.pins.append( pin )

    def hasChipPin ( self, pin ):
        if pin in self.pins: return True
        return False

    def addCoreWire ( self, coreWire ):
        self.coreWires.append( coreWire )
        return

    def updateGap ( self, gapWidth ):
        self.gap = max( self.gap, gapWidth )
        return

    def addSupplyRail ( self, component ):
        if self.type == North or self.type == South:
            if not isinstance(component,Horizontal):
                return
            self.padRails.append( ( component.getNet()
                                  , component.getLayer()
                                  , component.getY()
                                  , component.getWidth() ) )
            return
        if self.type == East or self.type == West:
            if not isinstance(component,Vertical):
                return
            self.padRails.append( ( component.getNet()
                                  , component.getLayer()
                                  , component.getX()
                                  , component.getWidth() ) )

    def drawCoreWires ( self ):
        trace( 550, ',+', '\tSide.drawCoreWire()\n' )
        size = len(self.coreWires)
        if not size:
            trace( 550, '-' )
            return
        if self.type == West or self.type == East:
            trace( 550, '\tSort East/West\n' )
            self.coreWires.sort( key=lambda wire: wire.bbSegment.getCenter().getY() )
        if self.type == North or self.type == South:
            trace( 550, '\tSort North/South\n' )
            self.coreWires.sort( key=lambda wire: wire.bbSegment.getCenter().getX() )
        for wire in self.coreWires:
            wire.updateInCorona()
        offset = 0
        for i in range(size):
            if self.coreWires[i].inCoronaRange: break
            offset += 1
        for i in range(offset):
            self.coreWires[i].setOffset( i+1, CoreWire.AtBegin )
        offset = 0
        for i in range(1,size+1):
            if self.coreWires[-i].inCoronaRange: break
            offset += 1
        if offset < size:
            for i in range(offset):
                self.coreWires[ -(offset+1) ].setOffset( i+1, CoreWire.AtEnd )
        for wire in self.coreWires:
            if self.type == West or self.type == East:
                trace( 550, '\t| %s %s %d %s inRange:%s\n' % ( wire.chipNet.getName()
                                                             , DbU.getValueString(wire.bbSegment.getCenter().getY())
                                                             , wire.count
                                                             , wire.padSegment.getLayer()
                                                             , wire.inCoronaRange ) )
            if self.type == North or self.type == South:
                trace( 550, '\t| %s %s %d %s inRange:%s\n' % ( wire.chipNet.getName()
                                                             , DbU.getValueString(wire.bbSegment.getCenter().getX())
                                                             , wire.count
                                                             , wire.padSegment.getLayer()
                                                             , wire.inCoronaRange ) )
            wire.drawWire()
        trace( 550, '-' )


# --------------------------------------------------------------------
# Class  :  "harness.pads.Corona"

class Corona ( object ):
    """
    Perform the placement of the corona instance inside the harness MPRJ
    space (centered by default). Draw connecting wires from the *edge*
    of the corona toward the I/O ports. Also connect the power supplies.

    This is a specialized version of ``core2chip/pads.py``
    """

    def __init__ ( self, chip ):
        self.chip = chip
        self.northSide  = Side( self, North )
        self.southSide  = Side( self, South )
        self.eastSide   = Side( self, East  )
        self.westSide   = Side( self, West  )
        self.powerCount = 0
        self._findPowerRails()

    @property
    def conf ( self ): return self.chip.conf

    @property
    def supplyRailWidth ( self ): return self.conf.cfg.chip.supplyRailWidth

    @property
    def supplyRailPitch ( self ): return self.conf.cfg.chip.supplyRailPitch

    def _findPowerRails ( self ):
        """
        Affect the power rails segments (horizontals & vertical) to their
        relevant side. The side is guessed by the position of the segment
        relative to the harness abutment box, they are assumed to be all
        fully outside of it.
        """
        trace( 550, '+', '\tCorona._findPowerRails()\n' )
        coronaAb = self.conf.icorona.getAbutmentBox()
        for net in self.conf.chip.getNets():
            if not net.isSupply(): continue
            for component in net.getComponents():
                trace( 550, '\tcomponent: {} {}\n'.format(type(component),component) )
                if not isinstance(component,Horizontal) and not isinstance(component,Vertical):
                    continue
                trace( 550, '\t| {}\n'.format(component) )
                bb   = component.getBoundingBox()
                side = None
                if bb.getXMin() < coronaAb.getXMin(): side = self.westSide
                if bb.getXMax() > coronaAb.getXMax(): side = self.eastSide
                if bb.getYMin() < coronaAb.getYMin(): side = self.southSide
                if bb.getYMax() > coronaAb.getYMax(): side = self.northSide
                trace( 550, '\tside: {} {}\n'.format(side,component) )
                if side:
                    trace( 550, '\tAdded on {} side: {}\n'.format(side,component) )
                    side.addSupplyRail( component )
        trace( 550, '-' )
  
    def doLayout ( self ):
        """
        Draw the dogleg wires connecting the harness I/O segments to the nearest
        on grid pin right at the egde of the core.
        """
        trace( 550, ',+', '\tCorona.doLayout()\n' )
        rg = self.conf.routingGauge
        coronaSouthGap = 0
        for layerGauge in rg.getLayerGauges():
            self.southSide.gap = max( self.southSide.gap, layerGauge.getPitch() * 2 )
        self.northSide.gap = self.southSide.gap
        self.eastSide.gap  = self.southSide.gap
        self.westSide.gap  = self.southSide.gap
        for coronaPlug in self.conf.icorona.getPlugs():
            padConnected = 0
            chipIntNet   = coronaPlug.getNet()
            if not chipIntNet: continue
            trace( 550, '\tConnexions on chipIntNet: {}\n'.format(chipIntNet) )
            trace( 550, '\t| coronaPlug: {}\n'.format(coronaPlug) )
            padConnected = self._createCoreWire( chipIntNet, padConnected )
            if padConnected == 0:
                if not (chipIntNet.isSupply() and self.conf.routingGauge.hasPowerSupply()):
                    trace( 550, '-' )
                    raise ErrorMessage( 1, 'ChipToChip._connectCorona(): Harness net "{}" is not connected to the core.' \
                                           .format(chipIntNet.getName()) )
        self.coreSymBb = self.conf.getInstanceAb( self.conf.icorona )
        self.coreSymBb.inflate( self.conf.toSymbolic( self.westSide.gap //2, Superior )
                              , self.conf.toSymbolic( self.southSide.gap//2, Superior )
                              , self.conf.toSymbolic( self.eastSide.gap //2, Inferior )
                              , self.conf.toSymbolic( self.northSide.gap//2, Inferior ) )
        with UpdateSession():
            self.northSide.drawCoreWires()
            self.southSide.drawCoreWires()
            self.eastSide.drawCoreWires()
            self.westSide.drawCoreWires()
        trace( 550, '-' )
  
    def _createCoreWire ( self, chipIntNet, count ):
        """
        Lookup the pin of the chipIntNet and create the CoreWire object.
        """
        trace( 550, ',+', '\tCorona._createCoreWire()\n' )
        trace( 550, '\tchipIntNet:{}\n'.format(chipIntNet) )
        pin = None
        for ipin in chipIntNet.getPins():
            pin = ipin
            bb  = pin.getBoundingBox()
            break
        trace( 550, '\tpin:{}\n'.format(pin) )
        if not pin or chipIntNet.isSupply():
            trace( 550, '-' )
            return count
        side = None
        if   self.hasSouthChipPin(pin): side = self.southSide
        elif self.hasNorthChipPin(pin): side = self.northSide
        elif self.hasEastChipPin (pin): side = self.eastSide
        elif self.hasWestChipPin (pin): side = self.westSide
        if not side:
            trace( 550, '-' )
            return count
        trace( 550, '\tside:{}\n'.format(side.sideName) )
        rg             = self.conf.routingGauge
        lg             = rg.getLayerGauge( pin.getLayer() )
        gapWidth       = 0
        segments       = None
        inPreferredDir = False
        if side.type == North or side.type == South:
            if lg.getDirection() == RoutingLayerGauge.Vertical:
                inPreferredDir = True
        else:
            if lg.getDirection() == RoutingLayerGauge.Horizontal:
                inPreferredDir = True
        side.addCoreWire( CoreWire( self, chipIntNet, pin, bb, side.type, inPreferredDir, count ) )
        side.updateGap( side.coreWires[-1].gapWidth )
        side.coreWires[-1].addJumper = False
        count += 1
        trace( 550, '-' )
        return count
  
    def hasNorthChipPin ( self, pin ): return self.northSide.hasChipPin(pin)
    def hasSouthChipPin ( self, pin ): return self.southSide.hasChipPin(pin)
    def hasEastChipPin  ( self, pin ): return self.eastSide.hasChipPin(pin)
    def hasWestChipPin  ( self, pin ): return self.westSide.hasChipPin(pin)

    def doPowerLayout ( self ):
        if not self.conf.routingGauge.hasPowerSupply(): return
        with UpdateSession():
            capViaWidth   = self.conf.vDeepRG.getPitch()*4
            coreAb        = self.conf.coreAb
            powerNet      = None
            groundNet     = None
            chipPowerNet  = None
            chipGroundNet = None
            corona = self.conf.corona
            for net in corona.getNets():
                if net.isPower (): powerNet  = net
                if net.isGround(): groundNet = net
            if powerNet:
                if powerNet.isGlobal():
                    chipPowerNet = self.conf.chip.getNet( powerNet.getName() )
                if not chipPowerNet:
                    for net in self.conf.chip.getNets():
                        if chipPowerNet: break
                        if not net.isPower(): continue
                        for plug in net.getPlugs():
                            if plug.getMasterNet() == powerNet:
                                chipPowerNet = net
                                break
                if not chipPowerNet:
                    raise ErrorMessage( 1, 'pads.Corona.doPowerLayout(): No core power net not connected in "{}"' \
                                           .format(self.conf.chip.getName()) )
            else:
                raise ErrorMessage( 1, 'pads.Corona.doPowerLayout(): No power net found in "{}"' \
                                       .format(corona.getName()) )
            if groundNet:
                if groundNet.isGlobal():
                    chipGroundNet = self.conf.chip.getNet( groundNet.getName() )
                if not chipGroundNet:
                    for net in self.conf.chip.getNets():
                        if chipGroundNet: break
                        if not net.isGround(): continue
                        for plug in net.getPlugs():
                            if plug.getMasterNet() == groundNet:
                                chipGroundNet = net
                                break
                if not chipGroundNet:
                    raise ErrorMessage( 1, 'pads.Corona.doPowerLayout(): No ground power net not connected in "{}"' \
                                           .format(self.conf.chip.getName()) )
            else:
                raise ErrorMessage( 1, 'pads.Corona.doPowerLayout(): No ground net found in "{}"' \
                                       .format(corona.getName()) )

            icore = self.conf.icore
            xcore = icore.getTransformation().getTx()
            stripeSpecs = []
            stripesNb = int( (coreAb.getWidth() - 8*capViaWidth + self.supplyRailWidth) \
                                 // self.supplyRailPitch - 1 )
            offset    = (coreAb.getWidth() - self.supplyRailPitch*(stripesNb-1)) // 2
            stripeSpecs.append( [ xcore + capViaWidth//2 , capViaWidth ] )
            stripeSpecs.append( [ xcore + 3*capViaWidth + capViaWidth//2 , capViaWidth ] )
            if self.chip.spares and len(self.chip.spares.rleafX) > 1:
                rleafX     = self.chip.spares.rleafX
                spacing    = (rleafX[1] - rleafX[0]) // 2
                stepOffset = 0
                step       = 1
                trace( 550, '\trleafX\n' )
                for i in range(len(rleafX)):
                    trace( 550, '\t| rleafX[{}] = {}\n'.format(i,DbU.getValueString(rleafX[i])))
                if spacing < self.supplyRailPitch:
                    stepOffset = 1
                    step       = 2
                    spacing    = (rleafX[2] - rleafX[0]) // 2
                if step == 1:
                    stripeSpecs.append( [ rleafX[0] - spacing, self.supplyRailWidth ] )
                    trace( 550, '\tstripe[N/A] @{}\n'.format(DbU.getValueString(stripeSpecs[-1][0])))
                else:
                    stripeSpecs.append( [ rleafX[0], self.supplyRailWidth ] )
                    trace( 550, '\tstripe[N/A] @{}\n'.format(DbU.getValueString(stripeSpecs[-1][0])))
                for i in range( stepOffset, len(rleafX)-stepOffset, step ):
                    if step == 1:
                        stripeSpecs.append( [ rleafX[i], self.supplyRailWidth ] )
                        trace( 550, '\tstripe[{}] @{}\n'.format(i,DbU.getValueString(stripeSpecs[-1][0])))
                    stripeSpecs.append( [ rleafX[i] + spacing, self.supplyRailWidth ] )
                    trace( 550, '\tstripe[{}] @{}\n'.format(i,DbU.getValueString(stripeSpecs[-1][0])))
            else:
                for i in range(stripesNb):
                    stripeSpecs.append( [ xcore + offset + i*self.supplyRailPitch
                                        , self.supplyRailWidth
                                        ] )
            stripeSpecs.append( [ xcore + coreAb.getWidth() - 3*capViaWidth - capViaWidth//2 , capViaWidth ] )
            stripeSpecs.append( [ xcore + coreAb.getWidth() - capViaWidth//2 , capViaWidth ] )

            trace( 550, '\ticoreAb={}\n'.format(icore.getAbutmentBox()) )
            trace( 550, '\tcapViaWidth={}\n'.format(DbU.getValueString(capViaWidth)))
            for i in range(len(stripeSpecs)):
                if i % 2:
                    chipNet   = chipPowerNet
                    coronaNet = powerNet
                else:
                    chipNet   = chipGroundNet
                    coronaNet = groundNet
                trace( 550, '\tstripe[{}] @{}\n'.format(i,DbU.getValueString(stripeSpecs[i][0])))
                self._supplyToPad( chipNet, coronaNet, stripeSpecs[i][0], stripeSpecs[i][1], North )
                self._supplyToPad( chipNet, coronaNet, stripeSpecs[i][0], stripeSpecs[i][1], South )

    def _supplyToPad ( self, chipNet, coronaNet, coronaAxis, stripeWidth, side ):
        """
        Draw the wires connecting the north/south corona power pins to the
        harness I/O ring (on both north and south side). This *do not* draw
        the power lines *inside* the corona.

        .. note:: As the current e-fabless harness use preferred routing
                  directions at 90° to our own (VHV instead of HVH), we must
                  make a "duck" in m4 to reach the north/south bottom power
                  line in m5 without short-circuit...
        """
        trace( 550, ',+', '\tCorona.Builder._supplyToPads()\n' )
        supplyLayerDepth = self.conf.routingGauge.getPowerSupplyGauge().getDepth()
        supplyLayer      = self.conf.routingGauge.getPowerSupplyGauge().getLayer()
        supplyBelowLayer = self.conf.routingGauge.getRoutingLayer( supplyLayerDepth-1 )
        chipLayer        = self.conf.getRoutingLayer( self.conf.routingGauge.getPowerSupplyGauge().getDepth() - 1 )
        coronaAb         = self.conf.icorona.getAbutmentBox()
        chipAxis         = coronaAxis + self.conf.icorona.getTransformation().getTx()
        trace( 550, '\tchipNet={}\n'.format(chipNet) )
        trace( 550, '\tchipLayer={}\n'.format(chipLayer) )

        if   side == North: rails = self.northSide.padRails
        elif side == South: rails = self.southSide.padRails
        else:
            trace( 550, '-' )
            return
        trace( 550, '\trails={}\n'.format(rails) )
        for net, layer, railAxis, width in rails:
            trace( 550, '\t| powerNet={}\n'.format(net) )
            if net != chipNet:
                continue
            pinDirection = None
            viaAxis1     = railAxis
            viaAxis2     = None
            if side == North:
                pinDirection  = Pin.Direction.NORTH
                coronaY       = coronaAb.getYMax()
                masterCoronaY = self.conf.icorona.getMasterCell().getAbutmentBox().getYMax()
                if layer.getMask() == supplyLayer.getMask():
                    viaAxis1 = self.coreSymBb.getYMax()
                    viaAxis2 = railAxis
                trace( 550, '\tNorth side supply\n' )
            elif side == South:
                pinDirection  = Pin.Direction.SOUTH
                coronaY       = coronaAb.getYMin()
                masterCoronaY = self.conf.icorona.getMasterCell().getAbutmentBox().getYMin()
                if layer.getMask() == supplyLayer.getMask():
                    viaAxis1 = self.coreSymBb.getYMin()
                    viaAxis2 = railAxis
                trace( 550, '\tSouth side supply\n' )
            if pinDirection is not None:
                trace( 550, '\tcoronaAb={}\n'.format(coronaAb) )
                trace( 550, '\tcoronaAxis={}\n'.format(DbU.getValueString(coronaAxis)) ) 
                trace( 550, '\tchipAxis={}\n'.format(DbU.getValueString(chipAxis)) )
                trace( 550, '\trailNet={} <-> {}\n'.format(net,chipNet) )
                trace( 550, '\trailAxis={}\n'.format(DbU.getValueString(railAxis)) )
                if viaAxis2 is not None:
                    via = BigVia( chipNet
                                , supplyLayerDepth
                                , chipAxis
                                , viaAxis2
                                , stripeWidth
                                , width
                                , BigVia.AllowAllExpand
                                )
                    via.mergeDepth( supplyLayerDepth-1 )
                    via.doLayout()
                    Vertical.create( chipNet
                                   , supplyBelowLayer
                                   , chipAxis
                                   , stripeWidth
                                   , viaAxis2
                                   , viaAxis1
                                   )
                via = BigVia( chipNet
                            , supplyLayerDepth
                            , chipAxis
                            , viaAxis1
                            , stripeWidth
                            , width
                            , BigVia.AllowAllExpand
                            )
                trace( 550, '\tpower depth: {}\n'.format( self.conf.routingGauge.getPowerSupplyGauge().getDepth() ))
                via.mergeDepth( self.conf.routingGauge.getPowerSupplyGauge().getDepth()-1 )
                via.doLayout()
                Vertical.create( chipNet
                               , supplyLayer
                               , chipAxis
                               , stripeWidth
                               , viaAxis1
                               , coronaY
                               )
                pin = Pin.create( coronaNet
                                , '{}.{}'.format(coronaNet.getName(),self.powerCount)
                                , pinDirection
                                , Pin.PlacementStatus.FIXED
                                , supplyLayer
                                , coronaAxis
                                , masterCoronaY
                                , stripeWidth
                                , DbU.fromLambda( 1.0 )
                                )
                trace( 550, '\tpin={}\n'.format(pin) )
                self.powerCount += 1
                break
        trace( 550, '-' )
