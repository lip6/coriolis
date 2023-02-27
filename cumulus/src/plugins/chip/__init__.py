
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2014-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/__init__.py"               |
# +-----------------------------------------------------------------+

"""
Define constants to be used through all "chip" plugin modules, along
with the importConstants() function to import then directly in the
dictionnary of other modules.

* For corona sides: North, South, East & West.
* For corona corners: SouthWest, SouthEast, NorthWest & NorthEast.
* For rounding operation: Superior, Inferior, Inwards,
  OnHorizontalPitch & OnVerticalPitch.
"""

from ...Hurricane       import DbU, DataBase, UpdateSession, Breakpoint,  \
                               Box, Transformation , Instance , Net, Pin, \
                               Contact, Horizontal, Vertical, BasicLayer, \
                               Layer
from ...CRL             import RoutingGauge, RoutingLayerGauge
from ...helpers         import trace, l, u, n, onFGrid
from ...helpers.io      import ErrorMessage, WarningMessage
from ...helpers.overlay import UpdateSession
from ..block.bigvia     import BigVia
from .constants         import importConstants


importConstants( globals() )


# --------------------------------------------------------------------
# Class  :  "chip.CoreWire"

class CoreWire ( object ):
    """
    Draw wires to connect an external (chip/harness) terminal belonging
    to a ``chipNet`` (the *internal* one) to an on-grid Pin on the edge
    of the corona instance.

    Wires are created in *two* Cells:

    1. At the chip/harness level, where they are using real layout
       connecting to I/O external pads or the harness pins.

    2. At the corona level, *outside* the corona abutment box, so
       they overlap with the part draw at chip level, as we know the
       placement of the corona inside the chip.
         This part of the wires are *on grid*. And they may also be
       symbolic if the corona+core is in symbolic layout. This just
       on the outside of the corona that we make the transition
       between the real outer chip and the internal symbolic core.

    In the case a complete chip with I/O pad, we can also request the
    insertion of a jumper to protect against antenna effect. This is
    not working yet in the case of a harness.
    """

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
        self.addJumper     = False
        self.preferredDir  = preferredDir
        self.inCoronaRange = True
        self.arraySize     = None
        self.count         = count
        self.viaPitch      = DbU.fromLambda( 4.0 )
        self.gapWidth      = 0
        self._computeCoreLayers()
        if self.conf.routingGauge.getName() == 'FlexLib':
            self.addJumper = True

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
            trace( 550, '\tTrying: {}\n'.format(layerGauge) )
            if layerGauge.getType() == RoutingLayerGauge.Unusable: continue
            if layerGauge.getType() == RoutingLayerGauge.BottomPowerSupply: continue
            if layerGauge.getDepth() > self.conf.topLayerDepth: break
            if layerGauge.getLayer().getMask() == mask:
                trace( 550, '\tUsing: {}\n'.format(layerGauge) )
                self.symSegmentLayer = layerGauge.getLayer()
                if self.preferredDir:
                    self.symContactLayer = self.symSegmentLayer
                    if self.side & (West|East):
                        self.symContactSize = ( layerGauge.getWireWidth(), self.bbSegment.getHeight() )
                    else:
                        self.symContactSize = ( self.bbSegment.getWidth(), layerGauge.getWireWidth() )
                    trace( 550, '\tsymContactSize=( {}, {} )\n' \
                                .format( DbU.getValueString(self.symContactSize[0])
                                       , DbU.getValueString(self.symContactSize[1]) ))
                else:
                    depth = layerGauge.getDepth()
                    trace( 550, '\tChoosing PP layer, {}+1 > top={}\n' \
                                .format(layerGauge.getDepth(),self.conf.topLayerDepth) )
                    if layerGauge.getDepth() + 1 > self.conf.topLayerDepth:
                        self.symSegmentLayer = rg.getLayerGauge( depth-1 ).getLayer()
                        depth -= 1
                        trace( 550, '\tUsing below layer {}\n'.format( self.symSegmentLayer ))
                    else:
                        self.symSegmentLayer = rg.getLayerGauge( depth+1 ).getLayer()
                        trace( 550, '\tUsing above layer {}\n'.format( self.symSegmentLayer ))
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
                    arrayCount = (arrayWidth - contactMinSize) // self.viaPitch
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
        coronaAb     = self.conf.getInstanceAb( self.conf.icorona )
        coronaTransf = self.conf.icorona.getTransformation()
        self._computeCoreLayers()
        trace( 550, '\tbbSegment: {}\n'.format(self.bbSegment) )
        padLayer = self.padSegment.getLayer()
        if not isinstance(padLayer,BasicLayer):
            padLayer = padLayer.getBasicLayer()
            trace( 550, '\tpadLayer={}\n'.format( padLayer ))
        if self.side == West or self.side == East:
            flags  = OnHorizontalPitch
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
                trace( 550, '\txPadMin: {}\n'.format(DbU.getValueString( xPadMin )))
               #if not self.preferredDir:
               #   #xPadMax += self.bbSegment.getHeight()//2
               #    xPadMin += 3*vPitch
               #    trace( 550, '\txPadMin: {}\n'.format(DbU.getValueString( xPadMin )))
            else:
                accessDirection = Pin.Direction.EAST
                xPadMax         = self.bbSegment.getXMax()
                xContact        = self.corona.coreSymBb.getXMax() + self.offset * 2*vPitch
                xPadMin         = xContact
                xCore           = coronaAb.getXMax() - vPitch
               #if not self.preferredDir:
               #   #xPadMin -= self.bbSegment.getHeight()//2
               #    xPadMin -= 3*vPitch
            if self.addJumper:
                rg        = self.conf.routingGauge
                gaugeM5   = rg.getLayerGauge( 4 )
                wwidthM5  = gaugeM5.getWireWidth()
                jumperGap = 3*gaugeM5.getPitch()
                if self.side == East:
                    gapCenter = xPadMin + 5*gaugeM5.getPitch()
                else:
                    gapCenter = xPadMax - 5*gaugeM5.getPitch()
                xJumpMin  = gapCenter - jumperGap//2
                xJumpMax  = gapCenter + jumperGap//2
                hChip1 = Horizontal.create( self.chipNet
                                          , self.padSegment.getLayer()
                                          , self.bbSegment.getCenter().getY()
                                          , self.bbSegment.getHeight()
                                          , xPadMin
                                          , xJumpMin
                                          )
                trace( 550, '\thChip1: %s\n' % str(hChip1) )
                hChip2 = Horizontal.create( self.chipNet
                                          , self.padSegment.getLayer()
                                          , self.bbSegment.getCenter().getY()
                                          , self.bbSegment.getHeight()
                                          , xJumpMax
                                          , xPadMax
                                          )
                trace( 550, '\thChip2: %s\n' % str(hChip2) )
                hChip = hChip2 if self.side == West else hChip1
                bvia1 = BigVia( self.chipNet
                              , rg.getLayerDepth( self.padSegment.getLayer() )
                              , xJumpMin
                              , self.bbSegment.getCenter().getY()
                              , wwidthM5
                              , 2*wwidthM5
                              , flags=BigVia.AllowAllExpand )
                bvia1.mergeDepth( gaugeM5.getDepth() )
                trace( 550, '\tbvia1: %s\n' % str(bvia1) )
                bvia1.doLayout()
                bvia2 = BigVia( self.chipNet
                              , rg.getLayerDepth( self.padSegment.getLayer() )
                              , xJumpMax
                              , self.bbSegment.getCenter().getY()
                              , wwidthM5
                              , 2*wwidthM5
                              , flags=BigVia.AllowAllExpand )
                bvia2.mergeDepth( gaugeM5.getDepth() )
                bvia2.doLayout()
                trace( 550, '\tbvia2: %s\n' % str(bvia2) )
                Horizontal.create( bvia1.getPlate( gaugeM5.getLayer() )
                                 , bvia2.getPlate( gaugeM5.getLayer() )
                                 , gaugeM5.getLayer()
                                 , self.bbSegment.getCenter().getY()
                                 , wwidthM5
                                 )
            else:
                hChip = Horizontal.create( self.chipNet
                                         , self.padSegment.getLayer()
                                         , self.bbSegment.getCenter().getY()
                                         , self.bbSegment.getHeight()
                                         , xPadMin
                                         , xPadMax
                                         )
                trace( 550, '\thChip={}\n'.format( hChip ))
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
                    bb = contact.getBoundingBox()
                    coronaTransf.applyOn( bb )
                    vStrapBb.merge( bb )
            else:
                contact = self.conf.coronaContact( self.chipNet
                                                 , self.symContactLayer
                                                 , xContact
                                                 , yCore
                                                 , self.symContactSize[0]
                                                 , self.symContactSize[1]
                                                 , flags
                                                 )
                vStrapBb = contact.getBoundingBox( padLayer )
                coronaTransf.applyOn( vStrapBb )
            if self.arraySize:
                if self.side == West: xContact = min( xContact, vStrapBb.getXMin() )
                else:                 xContact = max( xContact, vStrapBb.getXMax() )
            hCorona = self.conf.coronaHorizontal( self.chipNet
                                                , self.symSegmentLayer
                                                , yCore
                                                , self.bbSegment.getHeight()
                                                , xContact
                                                , xCore
                                                )
            trace( 550, '\thCorona={}\n'.format( hCorona ))
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
            hChipBb = hChip.getBoundingBox( padLayer )
            trace( 550, '\thChipBb={}\n'.format( hChipBb ))
            vStrapBb.merge( vStrapBb.getXMin(), hChipBb.getYMin() )
            vStrapBb.merge( vStrapBb.getXMin(), hChipBb.getYMax() )
           #hCoronaBb = hCorona.getBoundingBox( padLayer )
            hCoronaBb = hCorona.getBoundingBox()
            self.conf.icorona.getTransformation().applyOn( hCoronaBb )
            trace( 550, '\thCoronaBb={}\n'.format( hCoronaBb ))
            vStrapBb.merge( vStrapBb.getXMin(), hCoronaBb.getYMin() )
            vStrapBb.merge( vStrapBb.getXMin(), hCoronaBb.getYMax() )
            if self.padSegment.getLayer().isSymbolic():
                vStrapBb.inflate( 0, -self.padSegment.getLayer().getExtentionCap()
                                , 0, -self.padSegment.getLayer().getExtentionCap() )
            v = Vertical.create( self.chipNet
                               , self.padSegment.getLayer()
                               , vStrapBb.getCenter().getX()
                               , vStrapBb.getWidth()
                               , vStrapBb.getYMin()
                               , vStrapBb.getYMax()
                               )
            trace( 550, '\tvChip={}\n'.format( v ))
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
                trace( 550, '\tSouth pad: yPadMin={}\n'.format(DbU.getValueString(yPadMin) ))
               #if not self.preferredDir:
               #    yPadMax += self.bbSegment.getWidth()//2
               #    yPadMin += 3*hPitch
            else:
                accessDirection = Pin.Direction.NORTH
                yPadMax         = self.bbSegment.getYMax()
                yPadMin         = self.corona.coreSymBb.getYMax() + self.offset * 2*hPitch
                yContact        = yPadMin
                yCore           = coronaAb.getYMax() - vPitch
               #if not self.preferredDir:
               #    yPadMin -= self.bbSegment.getWidth()//2
               #    yPadMin -= 3*hPitch
            vChip = Vertical.create( self.chipNet
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
                    bb = contact.getBoundingBox()
                    coronaTransf.applyOn( bb )
                    hStrapBb.merge( bb )
                    trace( 550, '\thStrapBb={} contact={}\n'.format(hStrapBb,contact) )
            else:
                contact   = self.conf.coronaContact( self.chipNet
                                                   , self.symContactLayer
                                                   , self.bbSegment.getCenter().getX()
                                                   , yContact
                                                   , self.symContactSize[0]
                                                   , self.symContactSize[1]
                                                   , flags
                                                   )
                hStrapBb = contact.getBoundingBox( padLayer )
                coronaTransf.applyOn( hStrapBb )
            trace( 550, '\thStrapBb={}\n'.format(hStrapBb) )
            if self.arraySize:
                if self.side == South: yContact = min( yContact, hStrapBb.getYMin() )
                else:                  yContact = max( yContact, hStrapBb.getYMax() )
            trace( 550, '\txCore:    {:<.1f}L {:<}\n'.format(DbU.toLambda(xCore   ), DbU.getValueString(xCore   )) )
            trace( 550, '\tyContact: {:<.1f}L {:<}\n'.format(DbU.toLambda(yContact), DbU.getValueString(yContact)) )
            trace( 550, '\tyCore:    {:<.1f}L {:<}\n'.format(DbU.toLambda(yCore   ), DbU.getValueString(yCore   )) )
            vCorona = self.conf.coronaVertical( self.chipNet
                                              , self.symSegmentLayer
                                              , xCore
                                              , self.bbSegment.getWidth()
                                              , yContact
                                              , yCore
                                              )
            trace( 550, '\tvCorona={}\n'.format(vCorona) )
            pin = self.conf.coronaPin( self.chipNet
                                     , self.count
                                     , accessDirection
                                     , self.symSegmentLayer
                                     , xCore
                                     , yCore
                                     , self.bbSegment.getWidth()
                                     , DbU.fromLambda( 1.0 )
                                     )
            vChipBb = vChip.getBoundingBox()
            hStrapBb.merge( vChipBb.getXMin(), hStrapBb.getYMin() )
            hStrapBb.merge( vChipBb.getXMax(), hStrapBb.getYMin() )
            vCoronaBb = vCorona.getBoundingBox()
            self.conf.icorona.getTransformation().applyOn( vCoronaBb )
            hStrapBb.merge( vCoronaBb.getXMin(), hStrapBb.getYMin() )
            hStrapBb.merge( vCoronaBb.getXMax(), hStrapBb.getYMin() )
            if self.padSegment.getLayer().isSymbolic():
                hStrapBb.inflate( -self.padSegment.getLayer().getExtentionCap(), 0
                                , -self.padSegment.getLayer().getExtentionCap(), 0 )
            hStrap = Horizontal.create( self.chipNet
                                      , self.padSegment.getLayer()
                                      , hStrapBb.getCenter().getY()
                                      , hStrapBb.getHeight()
                                      , hStrapBb.getXMin()
                                      , hStrapBb.getXMax()
                                      )
            trace( 550, '\thStrap={}\n'.format(hStrap) )
        if self.side & North: self.corona.northSide.pins.append( pin )
        if self.side & South: self.corona.southSide.pins.append( pin )
        if self.side & East : self.corona.eastSide .pins.append( pin )
        if self.side & West : self.corona.westSide .pins.append( pin )
        trace( 550, '-' )
