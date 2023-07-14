
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
# |  Python      :       "./plugins/chip/powerplane.py"             |
# +-----------------------------------------------------------------+


import sys
from   operator              import methodcaller  
from   ...Hurricane          import DbU, Point, Transformation, Box, Interval,  \
                                    Path, Occurrence, UpdateSession, Net,       \
                                    Contact, Horizontal, Vertical, Cell, Query, \
                                    DataBase, Pin, NetExternalComponents, Layer
from   ...helpers            import trace
from   ...helpers.io         import ErrorMessage, WarningMessage
from   ...helpers.overlay    import UpdateSession
from   ..block.bigvia        import BigVia
from   ..block.configuration import GaugeConf
from   .constants            import importConstants

__all__ = [ 'Builder' ]

importConstants( globals() )


# --------------------------------------------------------------------
# Class  :  "corona.IntervalSet"

class IntervalSet ( object ):

    def __init__ ( self ):
        self.chunks = []
        return

    def merge ( self, min, max ):
        toMerge = Interval( min, max )
        imerge  = len(self.chunks)
        length  = len(self.chunks)
        i       = 0
        while i < length:
            if imerge >= length:
                if toMerge.getVMax() < self.chunks[i].getVMin():
                    self.chunks.insert( i, toMerge )
                    length += 1
                    imerge  = 0
                    break
                if toMerge.intersect(self.chunks[i]):
                    imerge = i
                    self.chunks[imerge].merge( toMerge )
            else:
                if toMerge.getVMax() >= self.chunks[i].getVMin():
                    self.chunks[imerge].merge( self.chunks[i] )
                    del self.chunks[ i ]
                    length -= 1
                    continue
                else:
                    break
            i += 1
        if imerge >= length:
            self.chunks.insert( length, toMerge )

    def toStr ( self ):
        s = ''
        for interval in self.chunks:
            if len(s): s += ' '
            s += '[{}:{}]'.format( DbU.getValueString(interval.getVMin())
                                 , DbU.getValueString(interval.getVMax()) )
        return s


# --------------------------------------------------------------------
# Class  :  "power.Rail"

class Rail ( object ):

    @staticmethod
    def create ( bb ):
        rail = None
        if bb.getWidth() >= bb.getHeight():
            rail = HorizontalRail( bb )
            rail.merge( bb )
        else:
            rail = VerticalRail( bb )
            rail.merge( bb )
        return rail

    def __init__ ( self, axis, width ):
        self.axis      = axis
        self.width     = width
        self.intervals = IntervalSet()

    def __str__ ( self ):
        s = '<{} @{} w={} {}>'.format( self.typeName
                                     , DbU.getValueString(self.axis)
                                     , DbU.getValueString(self.width)
                                     , self.intervals.toStr() )
        return s


# --------------------------------------------------------------------
# Class  :  "power.HorizontalRail"

class HorizontalRail ( Rail ):

    def __init__ ( self, bb ):
       super(HorizontalRail,self).__init__( bb.getYCenter(), bb.getHeight() ) 
       self.isHorizontal = True

    @property
    def typeName ( self ): return 'HorizontalRail'

    def merge ( self, bb ):
        self.intervals.merge( bb.getXMin(), bb.getXMax() )

    def computeBigViaBbs ( self, plane, stripe, leftBigViaBbs, rightBigViaBbs ):
        """
        Compute the bounding boxes of the intersection of the stripe (vertical)
        and this horizontal rail. If the stripe is very wide, we put one BigVia
        on the left adge and another one on the right edge. Hence the two lists.
        If the stripe is *narrow*, only the left list is filled.
          The minimal *horizontal* size of the BigVias are such as they contains
        at least one top level power contact (which is usually very large) or
        two, if possible.
        """
        trace( 550, ',+', '\tHorizontalRail.computeBigViaBbs() metal={} {} stipe={}\n' \
                          .format( plane.metal.getName(), self, stripe ))
        viaLayer     = plane.conf.routingGauge.getContactLayer( stripe.getLayerDepth()-1 )
        cutLayer     = viaLayer.getCut()
        cutSide      = cutLayer.getMinimalSize()
        cutSpacing   = cutLayer.getMinimalSpacing()
        enclosure    = viaLayer.getTopEnclosure( Layer.EnclosureH )
        oneCutWidth  = cutSide + 2*enclosure
        twoCutWidth  = oneCutWidth + cutSpacing + cutSide
        viaWidth = plane.conf.vDeepRG.getPitch()*5
        stripeBb = stripe.getBoundingBox()
        for chunk in self.intervals.chunks:
            if chunk.getVMax() <= stripeBb.getXMin(): continue
            if chunk.getVMin() >= stripeBb.getXMax(): break
            trace( 550, '\t| Chunk=[{} {}]\n'.format( DbU.getValueString(chunk.getVMin())
                                                    , DbU.getValueString(chunk.getVMax()) ))
            chunkBb = Box( chunk.getVMin()
                         , self.axis - self.width//2
                         , chunk.getVMax()
                         , self.axis + self.width//2 )
            overlap = stripeBb.getIntersection( chunkBb )
            if overlap.isEmpty(): continue
            if overlap.getWidth() > 2*viaWidth:
                trace( 550, '\t| Large overlap={}\n'.format(overlap) )
                leftBigViaBbs.append( Box( overlap.getXMin()
                                         , overlap.getYMin()
                                         , overlap.getXMin() + twoCutWidth
                                         , overlap.getYMax()
                                         ))
                rightBigViaBbs.append( Box( overlap.getXMax() - twoCutWidth
                                          , overlap.getYMin()
                                          , overlap.getXMax()
                                          , overlap.getYMax()
                                          ))
                trace( 550, '\t| left={} right={}\n'.format(leftBigViaBbs[-1],rightBigViaBbs[-1]) )
            elif overlap.getWidth() > 2*plane.conf.vDeepRG.getPitch():
                trace( 550, '\t| Narrow overlap={}\n'.format(overlap) )
                leftBigViaBbs.append( overlap )
                trace( 550, '\t| left={} right=N/A\n'.format(leftBigViaBbs[-1]) )
            else:
                trace( 550, '\t| Too narrow overlap={}, no BigVia\n'.format(overlap) )
        trace( 550, '-' )


# --------------------------------------------------------------------
# Class  :  "power.VerticalRail"

class VerticalRail ( Rail ):

    def __init__ ( self, bb ):
       super(VerticalRail,self).__init__( bb.getXCenter(), bb.getWidth() ) 
       self.isHorizontal = False

    @property
    def typeName ( self ): return 'VerticalRail'

    def merge ( self, bb ):
        self.intervals.merge( bb.getYMin(), bb.getYMax() )

    def doLayout ( self, plane, stripes ):
        return


# --------------------------------------------------------------------
# Class  :  "power.Rails"

class Rails ( object ):

    def __init__ ( self, net ):
        self.net     = net
        self.axisLut = {}

    def merge ( self, bb ):
        if self.isHorizontal:
            axis  = bb.getYCenter()
            width = bb.getHeight()
        else:
            axis  = bb.getXCenter()
            width = bb.getWidth()
        if not axis in self.axisLut:
            self.axisLut[ axis ] = {}
        if not width in self.axisLut[axis]:
            if self.isHorizontal:
                self.axisLut[ axis ][ width ] = HorizontalRail( bb )
            else:
                self.axisLut[ axis ][ width ] = VerticalRail( bb )
        trace( 550, '\tRails.merge() on {} bb={}\n'.format(type(self.axisLut[ axis ][ width ]),bb) )
        self.axisLut[ axis ][ width ].merge( bb )

    def doLayout ( self, plane, stripes ):
        for stripe in stripes:
            leftBigViaBbs  = []
            rightBigViaBbs = []
            for wrail in self.axisLut.values():
                for rail in wrail.values():
                    rail.computeBigViaBbs( plane, stripe, leftBigViaBbs, rightBigViaBbs )
            stripe.doBigViaLayout( plane,  leftBigViaBbs )
            stripe.doBigViaLayout( plane, rightBigViaBbs )
        return


# --------------------------------------------------------------------
# Class  :  "power.HorizontalRails"

class HorizontalRails ( Rails ):

    def __init__ ( self, net ):
        super(HorizontalRails,self).__init__( net )
        self.isHorizontal = True


# --------------------------------------------------------------------
# Class  :  "power.VerticalRails"

class VerticalRails ( Rails ):

    def __init__ ( self, net ):
        super(VerticalRails,self).__init__( net )
        self.isHorizontal = False


# --------------------------------------------------------------------
# Class  :  "power.Plane"

class Plane ( object ):
  
    Horizontal = 1
    Vertical   = 2
  
    def __init__ ( self, builder, metal ):
        self.builder      = builder
        self.metal        = metal
        self.powerHRails  = HorizontalRails( self.conf.coronaVdd )
        self.powerVRails  = VerticalRails  ( self.conf.coronaVdd )
        self.groundHRails = HorizontalRails( self.conf.coronaVss )
        self.groundVRails = VerticalRails  ( self.conf.coronaVss )

    @property
    def conf ( self ): return self.builder.conf

    def getLayer ( self ): return self.metal

    def getLayerDepth ( self, layer ): return self.conf.getLayerDepth( layer )

    def getPlaneDepth ( self ): return self.conf.getLayerDepth( self.metal )
  
    def addRail ( self, net, bb ):
        if net.isPower():
            if bb.getWidth() >= bb.getHeight(): self.powerHRails.merge( bb )
            else:                               self.powerVRails.merge( bb )
        elif net.isGround():
            if bb.getWidth() >= bb.getHeight(): self.groundHRails.merge( bb )
            else:                               self.groundVRails.merge( bb )
        else:
            trace( 550, '\nPlane.addRail(): Reject, not a supply net:{}\n'.format(net) )
  
    def doLayout ( self, stripes ):
        self.powerHRails .doLayout( self, stripes.powerStripes )
        self.groundHRails.doLayout( self, stripes.groundStripes )


# --------------------------------------------------------------------
# Class  :  "power.Stripe"

class Stripe ( object ):

    def __init__ ( self, builder, southPin, northPin ):
        self.builder     = builder
        self.southPin    = southPin
        self.northPin    = northPin
        self.stripe      = None

    @property
    def conf ( self ): return self.builder.conf

    def __str__ ( self ):
        s = '<Stripe "{}" @{}>'.format( self.getNet().getName()
                                      , DbU.getValueString(self.getX()) )
        return s

    def getNet ( self ):
        if self.southPin: return self.southPin.getNet()
        if self.northPin: return self.northPin.getNet()
        return None

    def getX ( self ):
        if self.southPin: return self.southPin.getX()
        if self.northPin: return self.northPin.getX()
        return None

    def getLayer ( self ):
        if self.southPin: return self.southPin.getLayer()
        if self.northPin: return self.northPin.getLayer()
        return None

    def getLayerDepth ( self ):
        return self.conf.routingGauge.getLayerDepth( self.getLayer() )

    def getBoundingBox ( self ):
        if self.stripe: return self.stripe.getBoundingBox()
        bb = Box()
        if self.southPin: bb.merge( self.southPin.getBoundingBox() )
        if self.northPin: bb.merge( self.northPin.getBoundingBox() )
        return bb
                
    def doLayout ( self ):
        """
        Draw one vertical wire between the north and south Pins of the
        power stripe. For the computation & drawing of the big VIA, see
        ``doBigViaLayout()``.
        """
        if self.southPin is None:
            print( ErrorMessage( 1, [ 'Stripes.doLayout(): Power/ground stripe is missing a south Pin.'
                                    , '(north:{})'.format(self.northPin) ] ))
            return
        if self.northPin is None:
            print( ErrorMessage( 1, [ 'Stripes.doLayout(): Power/ground stripe is missing a north Pin.'
                                    , '(south:{})'.format(self.southPin) ] ))
            return
        self.stripe = Vertical.create( self.southPin
                                     , self.northPin
                                     , self.southPin.getLayer()
                                     , self.southPin.getX()
                                     , self.southPin.getWidth() )

    def doBigViaLayout ( self, plane, bigViaBbs ):
        """
        Draw the layout of the BigVias located at the intersection of the
        stripe (vertical) and the deep (horizontal) power lines. The VIAs
        are given as a list of bounding box. Do a pre-processing to try
        to merge two vertically contiguous vias (with the same width).
        """
        trace( 550, ',+', '\tStripe.doBigViaLayout()\n' )
        bigViaBbs.sort( key=methodcaller('getYMin') )
        i = 0
        while i+1 < len(bigViaBbs):
            if     bigViaBbs[i].getYMax () >= bigViaBbs[i+1].getYMin () \
               and bigViaBbs[i].getWidth() == bigViaBbs[i+1].getWidth():
                bigViaBbs[i].merge( bigViaBbs[i+1] )
                del bigViaBbs[i+1]
                continue
            i += 1
        for viaBb in bigViaBbs:
            via = BigVia( self.getNet()
                        , plane.getLayerDepth(self.getLayer())
                        , viaBb.getXCenter()
                        , viaBb.getYCenter()
                        , viaBb.getWidth()
                        , viaBb.getHeight()
                        , BigVia.AllowTopMetalExpand|BigVia.AllowVerticalExpand
                        )
            via.mergeDepth( plane.getLayerDepth(plane.getLayer()) )
            via.doLayout()
        trace( 550, '-' )


# --------------------------------------------------------------------
# Class  :  "power.Stripes"

class Stripes ( object ):

    def __init__ ( self, builder ):
        self.builder     = builder
        self.powers      = {}
        self.grounds     = {}
        self.supplyLayer = self.conf.routingGauge.getPowerSupplyGauge().getLayer()
        if self.conf.coronaVdd:
            for pin in self.conf.coronaVdd.getPins():
                if pin.getLayer() != self.supplyLayer: continue
                key = pin.getX()
                if pin.getAccessDirection() == Pin.Direction.SOUTH:
                    if not key in self.powers: self.powers[ key ] = Stripe( self, pin, None )
                    else:                      self.powers[ key ].southPin = pin
                elif pin.getAccessDirection() == Pin.Direction.NORTH:
                    if not key in self.powers: self.powers[ key ] = Stripe( self, None, pin )
                    else:                      self.powers[ key ].northPin = pin
        if self.conf.coronaVss:
            for pin in self.conf.coronaVss.getPins():
                if pin.getLayer() != self.supplyLayer: continue
                key = pin.getX()
                if pin.getAccessDirection() == Pin.Direction.SOUTH:
                    if not key in self.grounds: self.grounds[ key ] = Stripe( self, pin, None )
                    else:                       self.grounds[ key ].southPin = pin
                elif pin.getAccessDirection() == Pin.Direction.NORTH:
                    if not key in self.grounds: self.grounds[ key ] = Stripe( self, None, pin )
                    else:                       self.grounds[ key ].northPin = pin

    @property
    def conf ( self ): return self.builder.conf

    @property
    def powerStripes ( self ): return self.powers.values()

    @property
    def groundStripes ( self ): return self.grounds.values()
                
    def doLayout ( self ):
        for stripe in self.powers .values(): stripe.doLayout()
        for stripe in self.grounds.values(): stripe.doLayout()


# --------------------------------------------------------------------
# Class  :  "power.GoCb"

class GoCb ( object ):
  
    def __init__ ( self, builder ):
        self.builder = builder
  
    def __call__ ( self, query, go ):
        managed = False
        if   isinstance(go,Horizontal): managed = True
        elif isinstance(go,Vertical):   managed = True
        if not managed: return
        rootNet = None
        if go.getNet().getType() == int(Net.Type.POWER):  rootNet = self.builder.conf.coronaVdd
        if go.getNet().getType() == int(Net.Type.GROUND): rootNet = self.builder.conf.coronaVss
        if not rootNet: return
        if not NetExternalComponents.isExternal(go): return
        if self.builder.activePlane:
            layer = self.builder.activePlane.metal
            if layer.isSymbolic():
                layer = layer.getBasicLayer()
            bb = go.getBoundingBox( layer )
            query.getPath().getTransformation().applyOn( bb )
            trace( 550, '\tGoCb.__call__(): path={} go={}\n'.format(query.getPath(),go) )
            self.builder.activePlane.addRail( rootNet, bb )
        else:
           print( WarningMessage( 'power.GoCb() callback called without an active plane.' ))
        return


# --------------------------------------------------------------------
# Class  :  "power.Builder"

class Builder ( object ):

    def __init__ ( self, conf ):
        self.conf        = conf
        self.path        = Path()
        self.corona      = self.conf.icorona.getMasterCell()
        self.icoreAb     = self.conf.icore.getAbutmentBox()
        self.planes      = {}
        self.stripes     = Stripes( self )
        self.activePlane = None
        for layerGauge in self.conf.routingGauge.getLayerGauges():
          self.planes[ layerGauge.getLayer().getName() ] = Plane( self, layerGauge.getLayer() )
  
    def connectPower ( self ):
        trace( 550, '\tpower.Builder.connectPower()\n' )
        if not self.conf.coronaVdd or not self.conf.coronaVss:
            print( ErrorMessage( 1, 'Cannot build block power terminals as core vdd and/or vss are not known.' ))
            return
        goCb  = GoCb( self )
        query = Query()
        query.setGoCallback( goCb )
        query.setCell( self.corona )
        query.setArea( self.icoreAb )
        query.setFilter( Query.DoComponents|Query.DoTerminalCells )
        query.setStopCellFlags( Cell.Flags_AbstractedSupply )
        trace( 550, '\tloop over the layer gauge\n' )
        for layerGauge in self.conf.routingGauge.getLayerGauges():
            self.activePlane = self.planes[ layerGauge.getLayer().getName() ]
            layer = layerGauge.getLayer()
            if layer.isSymbolic():
                layer = layer.getBasicLayer()
            query.setBasicLayer( layer )
            trace( 550, ',+', '\tquery.doQuery() {}\n'.format(layer) )
            query.doQuery()
            trace( 550, '-' )
        self.activePlane = None
  
    def _connectHTree ( self, coronaNet, trackNb ):
        trace( 550, '\tpower.Builder._connectHTree() {} on track {}\n'.format(coronaNet,trackNb) )
        coreNet = None
        for plug in self.conf.icore.getPlugs():
            if plug.getNet() == coronaNet:
                coreNet = plug.getMasterNet()
        if not coreNet:
            raise ErrorMessage( 1, 'Block "{}" has no net connected to the H-Tree "{}".' \
                                   .format(self.conf.icore.getName(),coronaNet.getName()) )
            return
        htPlugs = []
        for plug in coronaNet.getPlugs():
            if plug.getInstance().isTerminalNetlist():
                htPlugs.append( plug )
        if len(htPlugs) != 1:
            message = [ 'Net "{}" of block "{}" is not organized as a H-Tree ({} plugs).' \
                        .format( coronaNet.getName()
                               , self.conf.icore.getName()
                               , len(htPlugs)) ]
            for plug in htPlugs:
                message += [ '\n        - {} {}'.format(plug,plug.getInstance()) ]
            raise ErrorMessage( 1, message )
            return
        if not coronaNet.isExternal(): return
        coronaPin = None
        for pin in coronaNet.getPins():
            coronaPin = pin
            break
        if not coronaPin:
            message = [ 'Net "{}" of block "{}" is not connected to a corona Pin.' \
                        .format( coronaNet.getName() , self.conf.icore.getName() ) ]
            raise ErrorMessage( 1, message )
        with UpdateSession():
            coronaAb   = self.conf.cellPnR.getAbutmentBox()
            if coronaPin.getAccessDirection() == Pin.Direction.NORTH:
                isVertical = True
                axis       = coronaAb.getYMax()
                trackNb    = -trackNb
            elif coronaPin.getAccessDirection() == Pin.Direction.SOUTH:
                isVertical = True
                axis       = coronaAb.getYMin()
            elif coronaPin.getAccessDirection() == Pin.Direction.EAST:
                isVertical = False
                axis       = coronaAb.getXMax()
                trackNb    = -trackNb
            elif coronaPin.getAccessDirection() == Pin.Direction.WEST:
                isVertical = False
                axis       = coronaAb.getXMin()
            flags    = 0 if isVertical else GaugeConf.HAccess
            bufferRp = self.conf.rpAccessByOccurrence( Occurrence(htPlugs[0], Path()), coronaNet, flags )
            pinRp    = self.conf.rpAccessByOccurrence( Occurrence(coronaPin , Path()), coronaNet, flags )
            trace( 550, '\tpinRp={}\n'.format(pinRp) )
            if isVertical:
                pitch    = self.conf.hRoutingGauge.getPitch()
                yaxis    = axis + 2 * pitch * trackNb
                trace( 550, '\tyaxis(request)={}\n'.format(DbU.getValueString(yaxis)) )
                yaxis    = self.conf.getNearestHorizontalTrack( yaxis, 0 )
                xaxisRp  = self.conf.getNearestVerticalTrack( bufferRp.getX(), 0 )
                xaxisPin = self.conf.getNearestVerticalTrack( pin.getX(), 0 )
                contact1 = self.conf.createContact( coronaNet, xaxisRp , yaxis, 0 )
                contact2 = self.conf.createContact( coronaNet, xaxisPin, yaxis, 0 )
                self.conf.createVertical  ( bufferRp, contact1, xaxisRp , 0 )
                self.conf.createHorizontal( contact1, contact2, yaxis   , 0 )
                self.conf.createVertical  ( contact2, pinRp   , xaxisPin, 0 )
                trace( 550, '\tyaxis(track)={}\n'.format(DbU.getValueString(yaxis)) )
                trace( 550, '\tcontact1={}\n'.format(contact1) )
                trace( 550, '\tcontact2={}\n'.format(contact2) )
            else:
                pitch    = self.conf.vRoutingGauge.getPitch()
                xaxis    = axis + 2 * pitch * trackNb
                trace( 550, '\txaxis(request)={} vpitch={}\n' \
                            .format(DbU.getValueString(xaxis), DbU.getValueString(pitch)) )
                xaxis    = self.conf.getNearestVerticalTrack( xaxis, 0 )
                yaxisRp  = self.conf.getNearestHorizontalTrack( bufferRp.getY(), 0 )
                yaxisPin = self.conf.getNearestHorizontalTrack( pin.getY(), 0 )
                contact1 = self.conf.createContact( coronaNet, xaxis, yaxisRp , 0 )
                contact2 = self.conf.createContact( coronaNet, xaxis, yaxisPin, 0 )
                self.conf.createHorizontal( bufferRp, contact1, yaxisRp , 0 )
                self.conf.createVertical  ( contact1, contact2, xaxis   , 0 )
                self.conf.createHorizontal( contact2, pinRp   , yaxisPin, 0 )
                trace( 550, '\txaxis(track)={}\n'.format(DbU.getValueString(xaxis)) )
            self.conf.expandMinArea( bufferRp )
            self.conf.expandMinArea( pinRp )
        return
  
    def connectHTrees ( self, hTrees ):
        for i in range(len(hTrees)):
            self._connectHTree( hTrees[i].treeNet, i+2 )
  
    def doLayout ( self ):
        with UpdateSession():
            self.stripes.doLayout()
            for plane in self.planes.values():
                plane.doLayout( self.stripes )
