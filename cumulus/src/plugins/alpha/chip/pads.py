
# This file is part of the Coriolis Software.
# Copyright (c) SU 2014-2020, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/pads.py"                   |
# +-----------------------------------------------------------------+


from   __future__   import print_function
import sys
import re
from   operator     import itemgetter  
import Cfg          
from   Hurricane    import DbU, Point, Transformation, Interval, Box,  \
                           Path, Occurrence, UpdateSession, Layer,     \
                           BasicLayer, Net, Pin, Contact, Segment,     \
                           Horizontal, Vertical, Diagonal, RoutingPad, \
                           Instance
import CRL          
from   CRL          import RoutingLayerGauge
import helpers      
from   helpers         import trace
from   helpers.io      import ErrorMessage, WarningMessage
from   helpers.overlay import UpdateSession
import plugins.alpha.chip

plugins.alpha.chip.importConstants( globals() )


# --------------------------------------------------------------------
# Class  :  "pads.Corner"

class Corner ( object ):

    def __init__ ( self, corona, cornerType ):
        self.type      = cornerType
        self.corona    = corona
        self.padCorner = None

    @property
    def conf ( self ): return self.corona.conf

    def _getPoints ( self, axis ):
        """
        Compute the various coordinates to draw the corner for a wire
        positioned at ``axis`` distance from the *border* of the chip.
        xExtend and yExtend are used only in case of 45 degree corners,
        the 0.5 ratio is an approximate of ``sqrt(2) - 1``.
        """
        if self.type == SouthWest:
            xCorner = self.conf.chipAb.getXMin() + axis
            yCorner = self.conf.chipAb.getYMin() + axis
            xBb     = self.conf.chipAb.getXMin() + self.conf.ioPadHeight
            yBb     = self.conf.chipAb.getYMin() + self.conf.ioPadHeight
            xExtend = - long( 0.5 * float(self.conf.ioPadHeight - axis) )
            yExtend = xExtend
        elif self.type == SouthEast:
            xCorner = self.conf.chipAb.getXMax() - axis
            yCorner = self.conf.chipAb.getYMin() + axis
            xBb     = self.conf.chipAb.getXMax() - self.conf.ioPadHeight
            yBb     = self.conf.chipAb.getYMin() + self.conf.ioPadHeight
            xExtend = long( 0.5 * float(self.conf.ioPadHeight - axis) )
            yExtend = - xExtend
        elif self.type == NorthEast:
            xCorner = self.conf.chipAb.getXMax() - axis
            yCorner = self.conf.chipAb.getYMax() - axis
            xBb     = self.conf.chipAb.getXMax() - self.conf.ioPadHeight
            yBb     = self.conf.chipAb.getYMax() - self.conf.ioPadHeight
            xExtend = long( 0.5 * float(self.conf.ioPadHeight - axis) )
            yExtend = xExtend
        elif self.type == NorthWest:
            xCorner = self.conf.chipAb.getXMin() + axis
            yCorner = self.conf.chipAb.getYMax() - axis
            xBb     = self.conf.chipAb.getXMin() + self.conf.ioPadHeight
            yBb     = self.conf.chipAb.getYMax() - self.conf.ioPadHeight
            xExtend = - long( 0.5 * float(self.conf.ioPadHeight - axis) )
            yExtend = - xExtend
        return xCorner, yCorner, xBb, yBb, xExtend, yExtend

    def _createCorner ( self ):
        self.corona.conf.cfg.chip.use45corners = None
        for rail in self.corona.padRails:
            net   = rail[0]
            layer = rail[1]
            axis  = rail[2]
            width = rail[3]
            xCorner, yCorner, xBb, yBb, xExtend, yExtend = self._getPoints( axis )
            if self.corona.conf.cfg.chip.use45corners:
                Diagonal.create( net, layer, Point(xBb+xExtend,yCorner), Point(xCorner,yBb+yExtend), width )
                Horizontal.create( net, layer, yCorner, width, xBb+xExtend, xBb )
                Vertical  .create( net, layer, xCorner, width, yBb+yExtend, yBb )
            else:
                Contact   .create( net, layer, xCorner, yCorner, width, width )
                Horizontal.create( net, layer, yCorner, width, xCorner, xBb )
                Vertical  .create( net, layer, xCorner, width, yCorner, yBb )

    def _getTransformation ( self ):
        if self.type == SouthWest:
            name = 'padcorner_sw'
            x    = self.conf.chipAb.getXMin()
            y    = self.conf.chipAb.getYMin()
            if self.corona.padOrient == Transformation.Orientation.ID:
                orientation =  Transformation.Orientation.ID
            else:
                orientation  = Transformation.Orientation.R1
                x           += self.padCorner.getAbutmentBox().getWidth()
        elif self.type == SouthEast:
            name = 'padcorner_se'
            x    = self.conf.chipAb.getXMax()
            y    = self.conf.chipAb.getYMin()
            if self.corona.padOrient == Transformation.Orientation.ID:
                orientation  = Transformation.Orientation.R1
            else:
                orientation  = Transformation.Orientation.R2
                x           += self.padCorner.getAbutmentBox().getWidth()
                y           += self.padCorner.getAbutmentBox().getHeight()
        elif self.type == NorthEast:
            name = 'padcorner_ne'
            x    = self.conf.chipAb.getXMax()
            y    = self.conf.chipAb.getYMax()
            if self.corona.padOrient == Transformation.Orientation.ID:
                orientation  = Transformation.Orientation.R2
            else:
                orientation  = Transformation.Orientation.R3
                x           -= self.padCorner.getAbutmentBox().getwidth()
                y           -= self.padCorner.getAbutmentBox().getHeight()
        elif self.type == NorthWest:
            name = 'padcorner_nw'
            x    = self.conf.chipAb.getXMin()
            y    = self.conf.chipAb.getYMax()
            if self.corona.padOrient == Transformation.Orientation.ID:
                orientation  = Transformation.Orientation.R3
            else:
                orientation  = Transformation.Orientation.ID
                y           -= self.padCorner.getAbutmentBox().getHeight()
        return name, Transformation( self.corona.toGrid(x), self.corona.toGrid(y), orientation ) 

    def _instanciateCorner ( self ):
        name, transformation = self._getTransformation()
        corner = Instance.create( self.conf.cell
                                , name, self.corona.padCorner
                                , transformation
                                , Instance.PlacementStatus.FIXED
                                )

    def doLayout ( self ):
        if self.corona.padCorner: self._instanciateCorner()
        else:                     self._createCorner()


# --------------------------------------------------------------------
# Class  :  "pads.Side"

class Side ( object ):

    def __init__ ( self, corona, sideType ):
        self.type          = sideType
        self.corona        = corona
        self.pins          = []
        self.u             = self.conf.ioPadHeight
        self.spacerCount   = 0
        self.gap           = 0
        self.coreWires     = []
        if self.type == North:
            self.pads       = self.corona.northPads
            self.sideName   = 'north'
            self.sideLength = self.conf.chipAb.getWidth()
        elif self.type == South:
            self.pads       = self.corona.southPads
            self.sideName   = 'south'
            self.sideLength = self.conf.chipAb.getWidth()
        elif self.type == East:
            self.pads       = self.corona.eastPads
            self.sideName   = 'east'
            self.sideLength = self.conf.chipAb.getHeight()
        elif self.type == West:
            self.pads       = self.corona.westPads
            self.sideName   = 'west'
            self.sideLength = self.conf.chipAb.getHeight()
        else:
            raise ErrorMessage( 1, 'Pads.Side.__init__(): Invalid value for sideType ({})'.format(sideType))
        self.spacerNames = 'padspacer_' + self.sideName + '_%d'

    @property
    def conf ( self ): return self.corona.conf

    def toGrid ( self, u ): return self.corona.toGrid( u )

    def getAxis ( self, i ):
        if   self.type == North: return self.conf.chipAb.getYMax() - self.conf.ioPadHeight + self.corona.powerRails[i][2]
        elif self.type == South: return self.conf.chipAb.getYMin() + self.conf.ioPadHeight - self.corona.powerRails[i][2]
        elif self.type == East:  return self.conf.chipAb.getXMax() - self.conf.ioPadHeight + self.corona.powerRails[i][2]
        elif self.type == West:  return self.conf.chipAb.getXMin() + self.conf.ioPadHeight - self.corona.powerRails[i][2]
        else:
            raise ErrorMessage( 1, 'Pads.Side.__init__(): Invalid value for sideType ({})'.format(sideType))
        return 0

    def hasPad ( self, padInstance ):
        for pad in self.pads:
            if pad[1] == padInstance:
                return True
        return False

    def addCoreWire ( self, coreWire ):
        self.coreWires.append( coreWire )
        return

    def updateGap ( self, gapWidth ):
        self.gap = max( self.gap, gapWidth )
        return

    def _check ( self, checkSize, checkName ):
        sideName = 'unknown'
        chipSize = 0
        if self.type == North or self.type == South:
            chipSize = self.conf.chipAb.getWidth()
            sideName = 'wide'
        elif self.type == East or self.type == West:
            chipSize = self.conf.chipAb.getHeight()
            sideName = 'tall'
        if checkSize > chipSize:
            sliceHeight = self.conf.sliceHeight
            if checkSize % sliceHeight != 0:
                checkSize += sliceHeight - (checkSize % sliceHeight)
            raise ErrorMessage( 1, [ 'Chip is not {} enought to accomodate the {},' \
                                     .format(sideName,checkName)
                                   , 'needs {}, but only has {}.' \
                                     .format( DbU.getValueString(checkSize), DbU.getValueString(chipSize) )
                                   ] )
            return False
        return True

    def check ( self ):
        self.validated = True
        if self.type == North:
            self.conf.validated = self._check \
                ( self.conf.coronaAb.getWidth() + 2*self.conf.ioPadHeight, 'core' )
            checkName = 'north pads'
        elif self.type == East:
            self.conf.validated = self._check \
                ( self.conf.coronaAb.getHeight() + 2*self.conf.ioPadHeight, 'core' )
            checkName = 'east pads'
        elif self.type == South: checkName = 'south pads'
        elif self.type == West:  checkName = 'west pads'
        padLength = 2*self.conf.ioPadHeight
        for pad in self.pads:
            padLength += pad[1].getMasterCell().getAbutmentBox().getWidth()
        self.conf.validated = self._check( padLength, checkName ) and self.conf.validated
        return self.conf.validated

    def _fillPadSpacing ( self, gapWidth ):

        def _getWidth   ( spacer ): return spacer.getAbutmentBox().getWidth()

        def _nextSpacer ( iPadSpacer ):
            while     iPadSpacer < len(self.corona.padSpacers) \
                  and gapWidth < _getWidth(self.corona.padSpacers[iPadSpacer]):
                iPadSpacer += 1
            return iPadSpacer
        
        if not self.corona.padSpacers:
            self.u += gapWidth
            return
        iPadSpacer = 0
        iPadSpacer = _nextSpacer( iPadSpacer )
        while iPadSpacer < len(self.corona.padSpacers) and gapWidth > 0:
            gapWidth -= _getWidth( self.corona.padSpacers[iPadSpacer] )
            spacer    = Instance.create( self.conf.cell
                                       , self.spacerNames % self.spacerCount
                                       , self.corona.padSpacers[iPadSpacer])
            self.spacerCount += 1
            self._placePad( spacer )
            if gapWidth < _getWidth(self.corona.padSpacers[iPadSpacer]):
                iPadSpacer = _nextSpacer( iPadSpacer )
        if gapWidth != 0:
            raise ErrorMessage( 1, 'PadsCorona.Side._placePads(): Pad fillers cannot close the gap between pads on {} side, {} remains.' \
                                   .format(self.sideName,DbU.getValueString(gapWidth)) )
        self.u += gapWidth

    def _placePad ( self, padInstance ):
        if self.type == North:
            x = self.conf.chipAb.getXMin() + self.u
            y = self.conf.chipAb.getYMax()
            if self.corona.padOrient == Transformation.Orientation.ID:
                orientation = Transformation.Orientation.MY
            else:
                orientation = Transformation.Orientation.ID
                y          -= self.conf.ioPadHeight
        elif self.type == South:
            x = self.conf.chipAb.getXMin() + self.u
            y = self.conf.chipAb.getYMin()
            if self.corona.padOrient == Transformation.Orientation.ID:
                orientation = Transformation.Orientation.ID
            else:
                orientation = Transformation.Orientation.MY
                y          += self.conf.ioPadHeight
        elif self.type == West:
            x = self.conf.chipAb.getXMin()
            y = self.conf.chipAb.getYMin() + self.u
            if self.corona.padOrient == Transformation.Orientation.ID:
                orientation = Transformation.Orientation.R3
                y          += padInstance.getMasterCell().getAbutmentBox().getWidth()
            else:
                orientation = Transformation.Orientation.R1
                x          += padInstance.getMasterCell().getAbutmentBox().getHeight()
        elif self.type == East:
            x = self.conf.chipAb.getXMax()
            y = self.conf.chipAb.getYMin() + self.u
            if self.corona.padOrient == Transformation.Orientation.ID:
                orientation = Transformation.Orientation.R1
            else:
                orientation = Transformation.Orientation.R3
                x          -= padInstance.getMasterCell().getAbutmentBox().getHeight()
                y          += padInstance.getMasterCell().getAbutmentBox().getWidth()
        padInstance.setTransformation ( Transformation( self.toGrid(x), self.toGrid(y), orientation ) )
        padInstance.setPlacementStatus( Instance.PlacementStatus.FIXED )
        self.u += padInstance.getMasterCell().getAbutmentBox().getWidth()
        p = None
        if self.conf.ioPadGauge.getName() == 'pxlib':
            p = re.compile( r'p(?P<power>v[sd]{2}[ei])ck_px' )
        if self.conf.ioPadGauge.getName().startswith('phlib'):
            p = re.compile( r'p(?P<power>v[sd]{2})ck2_sp' )
        if self.conf.ioPadGauge.getName() == 'niolib':
            p = re.compile( r'(?P<power>(io)?v[sd]{2})' )
        if p:
            m = p.match( padInstance.getMasterCell().getName() )
            padName = 'pad'
            if m: padName = m.group( 'power' )
            padNet = padInstance.getMasterCell().getNet( padName )
            trace( 550, '\tpadName:{} padNet:{}\n'.format(padName,padNet) )
            if padNet:
                plug    = padInstance.getPlug( padNet )
                chipNet = plug.getNet()
                if not chipNet and padNet.isGlobal():
                    chipNet = padInstance.getCell().getNet( padNet.getName() )
                if chipNet:
                    rp = RoutingPad.create( chipNet, Occurrence(plug), RoutingPad.BiggestArea )
        return

    def _placePads ( self ):
        padLength = 0
        for pad in self.pads: padLength += pad[1].getMasterCell().getAbutmentBox().getWidth() 
        padSpacing = (self.sideLength - 2*self.conf.ioPadHeight - padLength) / (len(self.pads) + 1)
        if self.conf.padsHavePosition:
            position = self.u
            for pad in self.pads:
                trace( 550, '\tPlace pad {} @{}\n'.format(pad[1],DbU.getValueString(pad[0])) )
                if pad[0] < position:
                    pad[0] = position
                    trace( 550, '\tShift position @{}\n'.format(DbU.getValueString(pad[0])) )
                pad[0] = self.toGrid( pad[0] )
                position = pad[0] + pad[1].getMasterCell().getAbutmentBox().getWidth()  
        else:
            position = self.u
            for i in range(len(self.pads)):
                position += padSpacing
                self.pads[i][0] = self.toGrid( position )
                position += self.pads[i][1].getMasterCell().getAbutmentBox().getWidth()
        for pad in self.pads:
            self._fillPadSpacing( pad[0] - self.u )
            self._placePad( pad[1] )
        self._fillPadSpacing( self.sideLength - self.conf.ioPadHeight - self.u )

    def _getUMin ( self, box ):
        if self.type == North or self.type == South:
            return box.getXMin()
        return box.getYMin()

    def _getUMax ( self, box ):
        if self.type == North or self.type == South:
            return box.getXMax()
        return box.getYMax()

    def _createSegment ( self, rail, uMin, uMax ):
        net, layer, axis, width = rail
        if self.conf.ioPadGauge.getName() == 'pxlib':
            if net.isClock():
                uMin -= DbU.fromLambda(5.0)
                uMax += DbU.fromLambda(5.0)
            else:
                uMin -= width/2
                uMax += width/2
        if self.type == North or self.type == South:
            if self.type == North:
                axis = self.conf.chipAb.getYMax() - axis
            Horizontal.create( net, layer, axis, width, uMin, uMax )
        else:
            if self.type == East:
                axis = self.conf.chipAb.getXMax() - axis
            Vertical.create( net, layer, axis, width, uMin, uMax )

    def _routePads ( self ):
        if self.type == South or self.type == North:
            startCorner = self.conf.chipAb.getXMin()
            stopCorner  = self.conf.chipAb.getXMax()
        elif self.type == West or self.type == East:
            startCorner = self.conf.chipAb.getYMin()
            stopCorner  = self.conf.chipAb.getYMax()
        else:
          return
        startCorner += self.conf.ioPadHeight
        stopCorner  -= self.conf.ioPadHeight
        if len(self.pads) == 0:
          return
        padAb = self.conf.getInstanceAb( self.pads[0][1] )
        for irail in range(len(self.corona.padRails)):
            self._createSegment( self.corona.padRails[ irail ]
                               , startCorner
                               , self._getUMin(padAb) )
        padAb = self.conf.getInstanceAb( self.pads[-1][1] )
        for irail in range(len(self.corona.padRails)):
            self._createSegment( self.corona.padRails[ irail ]
                               , self._getUMax(padAb)
                               , stopCorner )
        for i in range(len(self.pads)-1):
            padAb1 = self.conf.getInstanceAb( self.pads[i  ][1] )
            padAb2 = self.conf.getInstanceAb( self.pads[i+1][1] )
            for rail in self.corona.padRails:
                self._createSegment( rail, self._getUMax(padAb1), self._getUMin(padAb2) )

    def doLayout ( self ):
        self._placePads()
        if not self.corona.padSpacers:
          self._routePads()

    def drawCoreWires ( self ):
        trace( 550, ',+', '\tSide.drawCoreWire()\n' )
        if self.type == West or self.type == East:
            trace( 550, '\tSort East/West\n' )
            self.coreWires.sort( key=lambda wire: wire.bbSegment.getCenter().getY() )
        if self.type == North or self.type == South:
            trace( 550, '\tSort North/South\n' )
            self.coreWires.sort( key=lambda wire: wire.bbSegment.getCenter().getX() )
        for wire in self.coreWires:
            wire.updateInCorona()
        size   = len(self.coreWires)
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
# Class  :  "pads.CoreWire"

class CoreWire ( object ):

    # Should be read from the symbolic technology rules.
  
    NoOffset = 0x0000
    AtBegin  = 0x0001
    AtEnd    = 0x0002
  
    def __init__ ( self, corona, chipNet, padSegment, bbSegment, side, preferredDir, count ):
        self.corona        = corona
        self.chipNet       = chipNet
        self.padSegment    = padSegment
        self.bbSegment     = bbSegment
        self.offset        = 0
        self.offsetType    = CoreWire.NoOffset
        self.side          = side
        self.preferredDir  = preferredDir
        self.inCoronaRange = True
        self.arraySize     = None
        self.count         = count
        self.viaPitch      = DbU.fromLambda( 4.0 )
        self.gapWidth      = 0
        self._computeCoreLayers()

    @property
    def conf ( self ): return self.corona.conf
  
    def updateInCorona ( self ):
        coronaAb = self.conf.getInstanceAb( self.conf.icorona )
        if self.side == South or self.side == North:
            xCoronaPin = self.bbSegment.getCenter().getX()
            if   xCoronaPin <= coronaAb.getXMin(): self.inCoronaRange = False
            elif xCoronaPin >= coronaAb.getXMax(): self.inCoronaRange = False
        if self.side == East or self.side == West:
            yCoronaPin = self.bbSegment.getCenter().getY()
            if   yCoronaPin <= coronaAb.getYMin(): self.inCoronaRange = False
            elif yCoronaPin >= coronaAb.getYMax(): self.inCoronaRange = False
  
    def setOffset ( self, offset, offsetType ):
        self.offset     = offset
        self.offsetType = offsetType
  
    def _computeCoreLayers ( self ):
        rg   = self.conf.routingGauge
        mask = self.padSegment.getLayer().getMask()
        trace( 550, ',+', '\tCoreWire._computeCoreLayers()\n' )
        trace( 550, '\tbbSegment: {}\n'.format(self.bbSegment) )
        self.symSegmentLayer = None
        for layerGauge in rg.getLayerGauges():
            if layerGauge.getDepth() > self.conf.topLayerDepth: break
            if layerGauge.getLayer().getMask() == mask:
                self.symSegmentLayer = layerGauge.getLayer()
                if self.preferredDir:
                    self.symContactLayer = self.symSegmentLayer
                    if self.side & (West|East):
                        self.symContactSize = ( layerGauge.getWireWidth(), self.bbSegment.getHeight() )
                    else:
                        self.symContactSize = ( self.bbSegment.getWidth(), layerGauge.getWireWidth() )
                else:
                    depth = layerGauge.getDepth()
                    if layerGauge.getDepth() + 1 > self.conf.topLayerDepth:
                        self.symSegmentLayer = rg.getLayerGauge( depth-1 ).getLayer()
                        depth -= 1
                    else:
                        self.symSegmentLayer = rg.getLayerGauge( depth+1 ).getLayer()
                    self.symContactLayer = rg.getContactLayer( depth )
                    if self.side & (West|East):
                        self.symContactSize  = ( self.bbSegment.getHeight(), self.bbSegment.getHeight() )
                    else:
                        self.symContactSize  = ( self.bbSegment.getWidth(), self.bbSegment.getWidth() )
                    self.viaPitch = self.conf.getViaPitch( self.symContactLayer )
                    basicLayer = self.symSegmentLayer
                    if basicLayer.isSymbolic():
                        basicLayer = basicLayer.getBasicLayer()
                    contactMinSize = 2*self.symContactLayer.getEnclosure( basicLayer
                                                                        , Layer.EnclosureH|Layer.EnclosureV ) \
                                   +   self.symContactLayer.getMinimalSize()
                    arrayWidth = self.symContactSize[0]
                    arrayCount = (arrayWidth - contactMinSize) / self.viaPitch
                    trace( 550, '\tcontactMinSize: {}, arrayWidth: {}, arrayCount: {}\n' \
                                .format(DbU.getValueString(contactMinSize),DbU.getValueString(arrayWidth),arrayCount) )
                    if arrayCount < 0: arrayCount = 0
                   #if arrayCount < 3:
                    if self.side & (North|South):
                        self.arraySize = ( arrayCount+1, 2 )
                    else:
                        self.arraySize = ( 2, arrayCount+1 )
                    trace( 550, '\tarraySize = ({},{})\n'.format(self.arraySize[0], self.arraySize[1]) )
                    self.gapWidth = 4*self.viaPitch
                trace( 550, ',-' )
                return
        raise ErrorMessage( 1, 'CoreWire._computeCoreLayers(): Layer of IO pad segment "%s" is not in routing gauge.' \
                               .format(self.chipNet.getName()) )
        trace( 550, ',-' )
  
    def drawWire ( self ):
        trace( 550, ',+', '\tCoreWire.drawWire(): chip:"{}"\n'.format(self.chipNet.getName()) )
        coronaAb = self.conf.getInstanceAb( self.conf.icorona )
        self._computeCoreLayers()
        padLayer = self.padSegment.getLayer()
        if not isinstance(padLayer,BasicLayer):
            padLayer = padLayer.getBasicLayer()
        if self.side == West or self.side == East:
            flags = OnHorizontalPitch
            hPitch = self.conf.getPitch( self.symSegmentLayer )
            vPitch = self.conf.getPitch( self.padSegment.getLayer() )
            yCore  = self.conf.toCoronaPitchInChip( self.bbSegment.getCenter().getY(), self.symSegmentLayer )
            if self.offset:
                if self.offsetType == CoreWire.AtBegin:
                    yCore += 2*hPitch*self.offset
                else:
                    yCore -= 2*hPitch*self.offset
            if self.side == West:
                accessDirection = Pin.Direction.WEST
                xPadMin         = self.bbSegment.getXMin()
                xContact        = self.corona.coreSymBb.getXMin() - self.offset * 2*vPitch
                xPadMax         = xContact
                xCore           = coronaAb.getXMin()
                if not self.preferredDir:
                   #xPadMax += self.bbSegment.getHeight()/2
                    xPadMin += 3*vPitch
            else:
                accessDirection = Pin.Direction.EAST
                xPadMax         = self.bbSegment.getXMax()
                xContact        = self.corona.coreSymBb.getXMax() + self.offset * 2*vPitch
                xPadMin         = xContact
                xCore           = coronaAb.getXMax()
                if not self.preferredDir:
                   #xPadMin -= self.bbSegment.getHeight()/2
                    xPadMin -= 3*vPitch
            hReal = Horizontal.create( self.chipNet
                                     , self.padSegment.getLayer()
                                     , self.bbSegment.getCenter().getY()
                                     , self.bbSegment.getHeight()
                                     , xPadMin
                                     , xPadMax
                                     )
            trace( 550, '\tself.arraySize: %s\n' % str(self.arraySize) )
            if self.arraySize:
                contacts = self.conf.coronaContactArray( self.chipNet
                                                       , self.symContactLayer
                                                       , xContact
                                                       , yCore
                                                       , self.arraySize
                                                       , flags
                                                       )
                vStrapBb = Box()
                for contact in contacts:
                    vStrapBb.merge( contact.getBoundingBox() )
            else:
                contact = self.conf.coronaContact( self.chipNet
                                                 , self.symContactLayer
                                                 , xContact
                                                 , yCore
                                                 , self.symContactSize[0]
                                                 , self.symContactSize[1]
                                                 , flags
                                                 )
                vStrapBb  = contact.getBoundingBox( padLayer )
            hRealBb = hReal.getBoundingBox( padLayer )
            self.conf.icorona.getTransformation().applyOn( vStrapBb )
            vStrapBb.merge( vStrapBb.getXMin(), hRealBb.getYMin() )
            vStrapBb.merge( vStrapBb.getXMin(), hRealBb.getYMax() )
            if self.padSegment.getLayer().isSymbolic():
                vStrapBb.inflate( 0, -self.padSegment.getLayer().getExtentionCap()
                                , 0, -self.padSegment.getLayer().getExtentionCap() )
            Vertical.create( self.chipNet
                           , self.padSegment.getLayer()
                           , vStrapBb.getCenter().getX()
                           , vStrapBb.getWidth()
                           , vStrapBb.getYMin()
                           , vStrapBb.getYMax()
                           )
            if self.arraySize:
                if self.side == West: xContact = min( xContact, vStrapBb.getXMin() )
                else:                 xContact = max( xContact, vStrapBb.getXMax() )
            self.conf.coronaHorizontal( self.chipNet
                                      , self.symSegmentLayer
                                      , yCore
                                      , self.bbSegment.getHeight()
                                      , xContact
                                      , xCore
                                      )
            trace( 550, '\tCORONA PIN: {} {}\n'.format(self.chipNet, self.count) )
            pin = self.conf.coronaPin( self.chipNet
                                     , self.count
                                     , accessDirection
                                     , self.symSegmentLayer
                                     , xCore
                                     , yCore
                                     , DbU.fromLambda( 1.0 )
                                     , self.bbSegment.getHeight()
                                     )
        else:
            flags  = OnVerticalPitch
            hPitch = self.conf.getPitch( self.padSegment.getLayer() )
            vPitch = self.conf.getPitch( self.symSegmentLayer )
            trace( 550, '\t%s translated of %s\n' % (self.symSegmentLayer, DbU.getValueString( 2*vPitch*self.offset )) )
            xCore = self.conf.toCoronaPitchInChip( self.bbSegment.getCenter().getX(), self.symSegmentLayer )
            if self.offset:
                if self.offsetType == CoreWire.AtBegin:
                    xCore += 2*vPitch*self.offset
                else:
                    xCore -= 2*vPitch*self.offset
            if self.side == South:
                accessDirection = Pin.Direction.SOUTH
                yPadMin         = self.bbSegment.getYMin()
                yPadMax         = self.corona.coreSymBb.getYMin() - self.offset * 2*hPitch
                yContact        = yPadMax
                yCore           = coronaAb.getYMin()
               #if not self.preferredDir:
               #    yPadMax += self.bbSegment.getWidth()/2
               #    yPadMin += 3*hPitch
            else:
                accessDirection = Pin.Direction.NORTH
                yPadMax         = self.bbSegment.getYMax()
                yPadMin         = self.corona.coreSymBb.getYMax() + self.offset * 2*hPitch
                yContact        = yPadMin
                yCore           = coronaAb.getYMax()
               #if not self.preferredDir:
               #    yPadMin -= self.bbSegment.getWidth()/2
               #    yPadMin -= 3*hPitch
            vReal = Vertical.create( self.chipNet
                                   , self.padSegment.getLayer()
                                   , self.bbSegment.getCenter().getX()
                                   , self.bbSegment.getWidth()
                                   , yPadMin
                                   , yPadMax
                                   )
            trace( 550, '\tself.arraySize: %s\n' % str(self.arraySize) )
            if self.arraySize:
                contacts = self.conf.coronaContactArray( self.chipNet
                                                       , self.symContactLayer
                                                       , xCore
                                                       , yContact
                                                       , self.arraySize
                                                       , flags
                                                       )
                hStrapBb = Box()
                for contact in contacts:
                    hStrapBb.merge( contact.getBoundingBox() )
            else:
                contact   = self.conf.coronaContact( self.chipNet
                                                   , self.symContactLayer
                                                   , self.bbSegment.getCenter().getX()
                                                   , yContact
                                                   , self.symContactSize[0]
                                                   , self.symContactSize[1]
                                                   , flags
                                                   )
                hStrapBb  = contact.getBoundingBox( padLayer )
            vRealBb = vReal.getBoundingBox()
            self.conf.icorona.getTransformation().applyOn( hStrapBb )
            hStrapBb.merge( vRealBb.getXMin(), hStrapBb.getYMin() )
            hStrapBb.merge( vRealBb.getXMax(), hStrapBb.getYMin() )
            if self.padSegment.getLayer().isSymbolic():
                hStrapBb.inflate( -self.padSegment.getLayer().getExtentionCap(), 0
                                , -self.padSegment.getLayer().getExtentionCap(), 0 )
            Horizontal.create( self.chipNet
                             , self.padSegment.getLayer()
                             , hStrapBb.getCenter().getY()
                             , hStrapBb.getHeight()
                             , hStrapBb.getXMin()
                             , hStrapBb.getXMax()
                             )
            if self.arraySize:
                if self.side == South: yContact = min( yContact, hStrapBb.getYMin() )
                else:                  yContact = max( yContact, hStrapBb.getYMax() )
            trace( 550, '\txCore:  {:.1f}L {}\n'.format(DbU.toLambda(xCore), DbU.getValueString(xCore)) )
            self.conf.coronaVertical( self.chipNet
                                    , self.symSegmentLayer
                                    , xCore
                                    , self.bbSegment.getWidth()
                                    , yContact
                                    , yCore
                                    )
            pin = self.conf.coronaPin( self.chipNet
                                     , self.count
                                     , accessDirection
                                     , self.symSegmentLayer
                                     , xCore
                                     , yCore
                                     , self.bbSegment.getWidth()
                                     , DbU.fromLambda( 1.0 )
                                     )
        if self.side & North: self.corona.northSide.pins.append( pin )
        if self.side & South: self.corona.southSide.pins.append( pin )
        if self.side & East : self.corona.eastSide .pins.append( pin )
        if self.side & West : self.corona.westSide .pins.append( pin )
        trace( 550, '-' )


# --------------------------------------------------------------------
# Class  :  "pads.Corona"

class Corona ( object ):

    def __init__ ( self, conf ):

        def _cmpPad ( pad1, pad2):
            width1 = pad1.getAbutmentBox().getWidth()
            width2 = pad2.getAbutmentBox().getWidth()
            if width1 == width2: return 0
            if width1 >  width2: return  -1
            return 1

        def _dupPads ( padsConf ):
            duplicateds = []
            for ioPadConf in padsConf:
                for padInstance in ioPadConf.pads:
                    position = 0 if ioPadConf.position is None else ioPadConf.position
                    duplicateds.append( [ position, padInstance ] )
            return duplicateds
        
        self.conf            = conf  
        self.conf.validated  = False
        self.northPads       = _dupPads( self.conf.chipConf.northPads )
        self.southPads       = _dupPads( self.conf.chipConf.southPads )
        self.eastPads        = _dupPads( self.conf.chipConf.eastPads )
        self.westPads        = _dupPads( self.conf.chipConf.westPads )
        self.northSide       = Side( self, North )
        self.southSide       = Side( self, South )
        self.eastSide        = Side( self, East  )
        self.westSide        = Side( self, West  )
        self.corners         = { SouthWest : Corner( self, SouthWest )
                               , SouthEast : Corner( self, SouthEast )
                               , NorthWest : Corner( self, NorthWest )
                               , NorthEast : Corner( self, NorthEast )
                               }
        self.padLib          = None
        self.padOrient       = Transformation.Orientation.ID
        self.padSpacers      = []
        self.padCorner       = []
        self.padRails        = []  # [ , [net, layer, axis, width] ]
        self.conf.cfg.chip.padCoreSide = None
        if self.conf.cfg.chip.padCoreSide.lower() == 'south':
            self.padOrient = Transformation.Orientation.MY
        trace( 550, '\tchip.padCoreSide: {}\n'.format(self.conf.cfg.chip.padCoreSide) )
        self._allPadsAnalysis()
        if Cfg.hasParameter('chip.padSpacers'):
            for spacerName in Cfg.getParamString('chip.padSpacers').asString().split(','):
                spacerCell = self.padLib.getCell( spacerName )
                if spacerCell: self.padSpacers.append( spacerCell )
                else:
                  raise ErrorMessage( 1, 'Corona.__init__(): Missing spacer cell "{}"'.format(spacerName) )
                self.padSpacers.sort( _cmpPad )
        if Cfg.hasParameter('chip.padCorner'):
            self.padCorner = self.padLib.getCell( Cfg.getParamString('chip.padCorner').asString() )
  
    def toGrid ( self, u ): return u - (u % self.conf.ioPadPitch)
  
    def validate ( self ):
        self._allPadsAnalysis()
        self.conf.validated = self.northSide.check()
        self.conf.validated = self.southSide.check() and self.conf.validated
        self.conf.validated = self.eastSide.check()  and self.conf.validated
        self.conf.validated = self.westSide.check()  and self.conf.validated
        return self.conf.validated
  
    def hasNorthPad ( self, padInstance ): return self.northSide.hasPad(padInstance)
    def hasSouthPad ( self, padInstance ): return self.southSide.hasPad(padInstance)
    def hasEastPad  ( self, padInstance ): return self.eastSide.hasPad(padInstance)
    def hasWestPad  ( self, padInstance ): return self.westSide.hasPad(padInstance)
  
    def hasPad ( self, padInstance ):
        if self.hasNorthPad(padInstance): return True
        if self.hasSouthPad(padInstance): return True
        if self.hasEastPad (padInstance): return True
        if self.hasWestPad (padInstance): return True
        return False
  
    def _allPadsAnalysis ( self ):
        for pad in self.southPads:
            self._padAnalysis( pad[1] )
            if self.padRails: return
        for pad in self.northPads:
            self._padAnalysis( pad[1] )
            if self.padRails: return
        for pad in self.eastPads:
            self._padAnalysis( pad[1] )
            if self.padRails: return
        for pad in self.westPads:
            self._padAnalysis( pad[1] )
            if self.padRails: return
  
    def _padAnalysis ( self, padInstance ):
        if self.padRails: return
        padCell = padInstance.getMasterCell()
        ab      = padCell.getAbutmentBox()
        if not self.padLib: self.padLib = padCell.getLibrary()
        for plug in padInstance.getPlugs():
            for component in plug.getMasterNet().getComponents():
                if component is None: continue 
                bb    = component.getBoundingBox()
                hspan = Interval( bb.getXMin(), bb.getXMax() )
                axis  = bb.getCenter().getY()
                if isinstance(component,Horizontal):
                    width = component.getWidth()
                elif isinstance(component,Vertical):
                    width = bb.getHeight()
                else:
                    continue
                # Specific hack for Alliance pxlib pad library.
                if self.conf.ioPadGauge.getName() == 'pxlib':
                    if isinstance(component,Vertical):
                        continue
                    if plug.getMasterNet().isClock():
                        hspan.inflate( DbU.fromLambda(5.0) )
                    else:
                        hspan.inflate( component.getWidth() / 2 )
                if hspan.contains( ab.getXMin() ) or hspan.contains( ab.getXMax() ):
                    duplicate = False
                    if self.padOrient == Transformation.Orientation.ID:
                        pass
                    else:
                        axis = self.conf.ioPadHeight - axis
                    for rail in self.padRails:
                        if     rail[0] == plug.getNet()        \
                           and rail[1] == component.getLayer() \
                           and rail[2] == axis:
                            duplicate = True
                            break
                    if not duplicate:
                        net = plug.getNet()
                        if not net:
                            if plug.getMasterNet().isGlobal():
                                net = self.conf.cell.getNet( plug.getMasterNet().getName() )
                                if not net:
                                    raise ErrorMessage( 1, 'Corona._padAnalysis(): Ring net "%s" is not connected and there is no global net (in pad \"%s").' \
                                                           %  plug.getMasterNet().getName(), padCell.getName() )
                            else:
                                raise ErrorMessage( 1, 'Corona._padAnalysis(): Ring net "%s" is neither connected nor global (in pad \"%s").' \
                                                     %  plug.getMasterNet().getName(), padCell.getName() )
                        if net:
                            self.padRails.append( ( net
                                                  , component.getLayer()
                                                  , axis
                                                  , width ) )
  
    def _createCoreWire ( self, chipIntNet, padNet, padInstance, count ):
        trace( 550, ',+', '\tCorona._createCoreWire()\n' )
        trace( 550, '\tchipIntNet:{}\n'.format(chipIntNet) )
        trace( 550, '\tpadNet:{}\n'.format(padNet) )
        trace( 550, '\tpadInstance:{}\n'.format(padInstance) )
        side = None
        if   self.hasSouthPad(padInstance): side = self.southSide
        elif self.hasNorthPad(padInstance): side = self.northSide
        elif self.hasEastPad (padInstance): side = self.eastSide
        elif self.hasWestPad (padInstance): side = self.westSide
        if not side:
            trace( 550, '-' )
            return count
        if chipIntNet.isPower() and not padInstance.getName().startswith('p_vdd'):
            trace( 550, '-' )
            return count
        if chipIntNet.isGround() and not padInstance.getName().startswith('p_vss'):
            trace( 550, '-' )
            return count
        innerBb   = self.conf.chip.getAbutmentBox().inflate( -self.conf.ioPadHeight )
        rg        = self.conf.routingGauge
        hsegments = {}
        vsegments = {}
        trace( 550, '\tinnerBb: {}\n'.format(innerBb) )
        for component in padNet.getExternalComponents():
            if isinstance(component,Segment) or isinstance(component,Contact):
                bb = component.getBoundingBox()
                padInstance.getTransformation().applyOn( bb )
                trace( 550, '\t| External:{} bb:{}\n'.format(component,bb) )
                if bb.intersect(innerBb):
                    trace( 550, '\t| Accepted.\n' )
                    lg    = rg.getLayerGauge( component.getLayer() )
                    depth = lg.getDepth()
                    if depth > self.conf.topLayerDepth: continue
                    if lg.getDirection() == RoutingLayerGauge.Vertical:
                        if not vsegments.has_key(depth): vsegments[ depth ] = []
                        vsegments[ depth ].append( (component,bb) )
                    else:
                        if not hsegments.has_key(depth): hsegments[ depth ] = []
                        hsegments[ depth ].append( (component,bb) )
        gapWidth       = 0
        segments       = None
        inPreferredDir = False
        if side.type == North or side.type == South:
            if len(vsegments):
                inPreferredDir = True
                segments = vsegments[ min(vsegments.keys()) ]
            elif len(hsegments):
                segments = hsegments[ min(hsegments.keys()) ]
        else:
            if len(hsegments):
                inPreferredDir = True
                segments = hsegments[ min(hsegments.keys()) ]
            elif len(vsegments):
                segments = vsegments[ min(vsegments.keys()) ]
        coreWires = []
        if segments:
            for segment, bb in segments:
                side.addCoreWire( CoreWire( self, chipIntNet, segment, bb, side.type, inPreferredDir, count ) )
                side.updateGap( side.coreWires[-1].gapWidth )
               #if not inPreferredDir:
               #    if side.type == North or side.type == South:
               #        trace( 550, '\tNorth/South "{}" but RDir H, gapWidth: {}\n' \
               #                    .format(chipIntNet.getName(),DbU.getValueString(bb.getWidth()) ) )
               #        side.updateGap( bb.getWidth() )
               #    else:
               #        trace( 550, '\tEast/West "{}" but RDir V, gapWidth: {}\n' \
               #                    .format(chipIntNet.getName(),DbU.getValueString(bb.getHeight())) )
               #        side.updateGap( bb.getHeight() )
                count += 1
        else:
            if not (chipIntNet.isGlobal() or chipIntNet.isSupply() or chipIntNet.isClock()):
                trace( 550, '-' )
                raise ErrorMessage( 1, [ 'Corona._createCoreWire(): In I/O pad "{}" ({},{}),' \
                                         .format( padInstance.getMasterCell().getName()
                                                , padInstance.getName(), padInstance.getTransformation() )
                                       , 'connector "{}" has no suitable segment for net "{}".' \
                                         .format( padNet, chipIntNet.getName() )
                                       ] )
        trace( 550, '-' )
        return count
  
    def _placeInnerCorona ( self ):
        trace( 550, ',+', '\tCorona._placeInnerCorona()\n' )
        rg = self.conf.routingGauge
        coronaSouthGap = 0
        for layerGauge in rg.getLayerGauges():
            self.southSide.gap = max( self.southSide.gap, layerGauge.getPitch() * 6 )
        self.northSide.gap = self.southSide.gap
        self.eastSide.gap  = self.southSide.gap
        self.westSide.gap  = self.southSide.gap
        for coronaPlug in self.conf.icorona.getPlugs():
            chipIntNet = coronaPlug.getNet()
            if not chipIntNet:
                trace( 550, '-' )
                raise ErrorMessage( 1, 'PadsCorona._placeInnerCorona(): Corona net "{}" is not connected to a pad.' \
                                       .format(coronaPlug.getMasterNet().getName()) )
                continue
            padConnected  = 0
            doneInstances = []
            trace( 550, '\tConnexions on chipIntNet: {}\n'.format(chipIntNet) )
            for chipPlug in chipIntNet.getPlugs():
                trace( 550, '\t| chipPlug: {}\n'.format(chipPlug) )
                doneInstances.append( chipPlug.getInstance() )
                padNet       = chipPlug.getMasterNet()
                padConnected = self._createCoreWire( chipIntNet, padNet, doneInstances[-1], padConnected )
            if chipIntNet.isGlobal():
                for instance in self.conf.cell.getInstances():
                    if instance in doneInstances: continue
                    doneInstances.append( instance )
                    padNet = instance.getMasterCell().getNet( chipIntNet.getName() )
                    if not padNet: continue
                    padConnected = self._createCoreWire( chipIntNet, padNet, doneInstances[-1], padConnected )
            if padConnected == 0:
                trace( 550, '-' )
                raise ErrorMessage( 1, 'PadsCorona._placeInnerCorona(): Chip net "{}" is not connected to a pad.' \
                                       .format(chipIntNet.getName()) )
        self.conf.setupCorona( self.westSide.gap, self.southSide.gap, self.eastSide.gap, self.northSide.gap )
        self.coreSymBb = self.conf.getInstanceAb( self.conf.icorona )
        self.coreSymBb.inflate( self.conf.toSymbolic( self.westSide.gap /2, Superior )
                              , self.conf.toSymbolic( self.southSide.gap/2, Superior )
                              , self.conf.toSymbolic( self.eastSide.gap /2, Inferior )
                              , self.conf.toSymbolic( self.northSide.gap/2, Inferior ) )
        self.southSide.drawCoreWires()
        self.northSide.drawCoreWires()
        self.eastSide .drawCoreWires()
        self.westSide .drawCoreWires()
        if     len(self.southSide.coreWires) \
           and len(self.westSide .coreWires) \
           and not self.southSide.coreWires[0].inCoronaRange \
           and not self.westSide .coreWires[0].inCoronaRange:
              print( ErrorMessage( 1, [ 'Corona._placeInnerCorona(): Both pads on south-east corner are outside corona range.'
                                      , 'This will generate a short-circuit between S:"{}" and W:"{}".' \
                                        .format( self.southSide.coreWires[0].chipNet.getName()
                                               , self.westSide .coreWires[0].chipNet.getName()) ] ))
        if     len(self.southSide.coreWires) \
           and len(self.eastSide .coreWires) \
           and not self.southSide.coreWires[-1].inCoronaRange \
           and not self.eastSide .coreWires[ 0].inCoronaRange:
              print( ErrorMessage( 1, [ 'Corona._placeInnerCorona(): Both pads on south-west corner are outside corona range.'
                                      , 'This will generate a short-circuit between S:"{}" and E:"{}.' \
                                        .format( self.southSide.coreWires[-1].chipNet.getName()
                                               , self.eastSide .coreWires[ 0].chipNet.getName()) ] ))
        if     len(self.northSide.coreWires) \
           and len(self.westSide .coreWires) \
           and not self.northSide.coreWires[ 0].inCoronaRange \
           and not self.westSide .coreWires[-1].inCoronaRange:
            print( ErrorMessage( 1, [ 'Corona._placeInnerCorona(): Both pads on north-east corner are outside corona range.'
                                    , 'This will generate a short-circuit between N:"%s" and W:"%s".' % \
                                      ( self.northSide.coreWires[ 0].chipNet.getName()
                                      , self.westSide .coreWires[-1].chipNet.getName()) ] ))
        if     len(self.northSide.coreWires) \
           and len(self.eastSide .coreWires) \
           and not self.northSide.coreWires[-1].inCoronaRange \
           and not self.eastSide .coreWires[-1].inCoronaRange:
            print( ErrorMessage( 1, [ 'Corona._placeInnerCorona(): Both pads on north-west corner are outside corona range.'
                                    , 'This will generate a short-circuit between N:"%s" and E:"%s.' % \
                                      ( self.northSide.coreWires[-1].chipNet.getName()
                                      , self.eastSide .coreWires[-1].chipNet.getName()) ] ))
        trace( 550, '-' )

    def doLayout ( self ):
        if not self.conf.validated: return
        with UpdateSession():
            for corner in self.corners.values():
                corner.doLayout()
            self.northSide.doLayout()
            self.southSide.doLayout()
            self.eastSide.doLayout()
            self.westSide.doLayout()
            self._placeInnerCorona()
            self.conf.chip.setRouted( True )
