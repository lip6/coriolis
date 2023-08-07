
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
# |  Python      :       "./plugins/chip/corona.py"                 |
# +-----------------------------------------------------------------+


import sys
import bisect
from   operator           import methodcaller, xor
from   ...                import Cfg        
from   ...Hurricane       import DbU, Point, Interval, Box, Transformation,   \
                                 Path, Occurrence, Net, Contact, Horizontal,  \
                                 Vertical, Pad
from   ...CRL             import RoutingLayerGauge
from   ...helpers         import trace
from   ...helpers.io      import ErrorMessage, WarningMessage
from   ...helpers.overlay import UpdateSession
from   ..block.bigvia     import BigVia
from   .constants         import importConstants


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


# --------------------------------------------------------------------
# Class  :  "corona.Rail"

class Rail ( object ):

    def __init__ ( self, side, order, axis ):
        self.side  = side
        self.order = order
        self.axis  = axis
        self.vias  = {}    # Key:pos Element:[pos,railContact,blockContact]

    @property
    def net ( self ): return self.side.getRailNet(self.order)

    @property
    def conf ( self ): return self.side.corona.conf


# --------------------------------------------------------------------
# Class  :  "corona.HorizontalRail"

class HorizontalRail ( Rail ):

    def __init__ ( self, side, order, axis ):
        Rail.__init__( self, side, order, axis )
        trace( 550, '\t{}\n'.format(self) )

    def __str__ ( self ):
        return '<HorizontalRail "{}" ({}) @{}>'.format( self.side.getRailNet(self.order).getName()
                                                      , self.order
                                                      , DbU.getValueString(self.axis) )
    def isReachable ( self, contact ):
        trace( 550, ',+', '\tTry to connect to: {}\n'.format(self) )
        trace( 550, '\tContact {}\n'.format(contact) )
        contactBb = contact.getBoundingBox()
        if    contactBb.getXMin() < self.side.innerBb.getXMin() \
           or contactBb.getXMax() > self.side.innerBb.getXMax():
            trace( 550, '-' )
            # XXX turn this into a non-fatal case. ERROR is still printed,
            # but it continues, allowing testing to continue
            print (ErrorMessage( 1, [ '{} is outside rail/corona X range,'.format(contact)
                                  , 'power pad is likely to be to far off west or east.'
                                  , '(core:{})'.format(self.side.innerBb) ] ) )
        if contact.getX() in self.vias: return False
        trace( 550, '\tvias:{}\n'.format(self.vias) )
        keys = list( self.vias.keys() )
        keys.sort()
        insertIndex = bisect.bisect_left( keys, contact.getX() )
        if len(keys) > 0:
            trace( 550, '\tinsertIndex:{}\n'.format(insertIndex) )
            if insertIndex < len(keys):
                insertPosition = keys[ insertIndex ]
                trace( 550, '\tinsertPosition:{}\n'.format(insertPosition) )
                if contactBb.getXMax() >= self.vias[insertPosition][2].getBoundingBox().getXMin():
                    trace( 550, ',-', '\tFailed: overlap with existing contact @{}.\n' \
                                      .format(self.vias[insertPosition][2]) )
                    return False
            if insertIndex > 0:
                if self.vias[keys[insertIndex-1]][2].getBoundingBox().getXMax() >= contactBb.getXMin():
                    trace( 550, ',-', '\tFailed: overlap with existing contact @{}.\n' \
                                      .format(self.vias[keys[insertIndex-1]][2]) )
                    return False
        trace( 550, ',-' )
        return True

    def connect ( self, contact ):
        trace( 550, '\tHorizontalRail.connect() {}\n'.format(contact) )
        viaWidth  = contact.getWidth()
        viaHeight = self.side.hRailWidth
        viaFlags  = BigVia.AllowAllExpand
        if self.conf.routingGauge.isSymbolic():
            viaWidth  -= DbU.fromLambda( 1.0 )
            viaHeight -= DbU.fromLambda( 1.0 )
        if self.conf.cfg.katana.disableStackedVias:
            viaFlags |= BigVia.StackVias
        self.vias[ contact.getX() ] = [ contact.getX()
                                      , BigVia( self.net
                                              , self.side.getLayerDepth(self.side.getHLayer())
                                              , contact.getX()
                                              , self.axis
                                              , viaWidth
                                              , viaHeight
                                              , flags=viaFlags )
                                      , contact ]
        trace( 550, '\tADD "{}" contact "{}" @ [{} {}]\n' \
               .format( contact.getNet().getName()
                      , contact.getLayer().getName()
                      , DbU.getValueString(contact.getX())
                      , DbU.getValueString(self.axis)) )
        self.vias[ contact.getX() ][1].mergeDepth( self.side.getLayerDepth(contact.getLayer()) )
        return True

    def doLayout ( self ):
        trace( 550, ',+', '\tHorizontalRail.doLayout() order:{} @{}\n' \
                          .format( self.order,DbU.getValueString(self.axis )))
        railVias = [ self.side.corner0(self.order).getPlate( self.side.getHLayer() )
                   , self.side.corner1(self.order).getPlate( self.side.getHLayer() ) ]
        for via in self.vias.values():
            if via[1].getNet() != via[2].getNet(): continue
            via[1].mergeDepth( self.side.getLayerDepth(self.side.getVLayer()) )
            via[1].doLayout()
            Vertical.create( via[1].getPlate( via[2].getLayer() )
                           , via[2]
                           , via[2].getLayer()
                           , via[2].getX()
                           , via[2].getWidth()
                           )
            railVias.append( via[1].getPlate( self.side.getVLayer()) )
        for i in range(1,len(railVias)):
            h = Horizontal.create( railVias[i-1]
                                 , railVias[i]
                                 , self.side.getHLayer()
                                 , self.axis
                                 , self.side.hRailWidth
                                 )
            trace( 550, '\t| {}\n'.format(h) )
        trace( 550, '-' )


# --------------------------------------------------------------------
# Class  :  "corona.VerticalRail"

class VerticalRail ( Rail ):

    def __init__ ( self, side, order, axis ):
        Rail.__init__( self, side, order, axis )
        trace( 550, '\t{}\n'.format(self) )

    def __str__ ( self ):
        return '<VerticalRail "{}" ({}) @{}>'.format( self.side.getRailNet(self.order).getName()
                                                    , self.order
                                                    , DbU.getValueString(self.axis) )

    def doLayout ( self ):
        railVias = [ self.side.corner0(self.order).getPlate( self.side.getVLayer() )
                   , self.side.corner1(self.order).getPlate( self.side.getVLayer() ) ]
        for via in self.vias.values():
            if via[1].getNet() != via[2].getNet(): continue
            via[1].doLayout()
            Horizontal.create( via[1].getPlate( via[2].getLayer() )
                             , via[2]
                             , via[2].getLayer()
                             , via[2].getY()
                             , via[2].getHeight()
                             )
            railVias.append( via[1].getPlate(self.side.getVLayer()) )
        railVias.sort( key=methodcaller('getY') )
        for i in range(1,len(railVias)):
            Vertical.create( railVias[i-1]
                           , railVias[i]
                           , self.side.getVLayer()
                           , self.axis
                           , self.side.vRailWidth
                           )

    def isReachable ( self, contact ):
        railDepth    = self.conf.routingGauge.getLayerDepth( self.side.getVLayer() )
        contactDepth = self.conf.routingGauge.getLayerDepth( contact.getLayer() )
        contactBb    = contact.getBoundingBox()
        contactAbove = True if contactDepth > railDepth else False
        if    contactBb.getYMin() < self.side.innerBb.getYMin() \
           or contactBb.getYMax() > self.side.innerBb.getYMax():
            # XXX turn this into a non-fatal case. ERROR is still printed,
            # but it continues, allowing testing to continue
            print (ErrorMessage( 1, [ '{} is outside rail/corona Y range'.format(contact)
                                   , 'power pad is likely to be to far off north or south.'
                                   , '(core:{})'.format(self.side.innerBb) ] ) )
        if contact.getY() in self.vias: return False
        trace( 550, ',+', '\tVerticalRail.isReachable() [{}] @{}\n'.format(self.order,DbU.getValueString(self.axis)) )
        trace( 550, '\t{}\n'.format(contact) )
        keys = list( self.vias.keys() )
        keys.sort()
        insertIndex = bisect.bisect_left( keys, contact.getY() )
        trace( 550, ',+', '\tkeys:' )
        for key in keys:
            trace( 550, ' {}'.format(DbU.getValueString(key)) )
        trace( 550, '\n' )

        if len(keys) > 0:
            if insertIndex < len(keys):
                insertPosition = keys[ insertIndex ]
                trace( 550, '\tinsertIndex:{}\n'.format(insertIndex) )
                trace( 550, '\tCheck NEXT contactBb:{} via:{}\n' \
                            .format( contactBb
                                   , self.vias[insertPosition][1].getBoundingBox(railDepth)) )
                if   self.vias[insertPosition][1].bottomDepth < railDepth: bigviaAbove = False
                elif self.vias[insertPosition][1].topDepth    > railDepth: bigviaAbove = True
                else:
                    print( ErrorMessage( 1, [ '{} neither above nor below' \
                                              .format( self.vias[insertPosition][1] ) ] ))
                trace( 550, '\tcontactAbove={} bigviaAbove={}\n'.format( contactAbove, bigviaAbove ))
                if contactBb.getYMax() >= self.vias[insertPosition][1].getBoundingBox(railDepth).getYMin() \
                   and not xor(contactAbove,bigviaAbove):
                    trace( 550, ',--', '\tReject {} intersect NEXT\n'.format(contact) )
                    return False
            if insertIndex > 0:
                insertPosition = keys[ insertIndex-1 ]
                trace( 550, '\tcheck PREVIOUS contactBb:{} via:{}\n' \
                            .format( contactBb
                                   , self.vias[insertPosition][1].getBoundingBox(railDepth)) )
                if   self.vias[insertPosition][1].bottomDepth < railDepth: bigviaAbove = False
                elif self.vias[insertPosition][1].topDepth    > railDepth: bigviaAbove = True
                else:
                    print( ErrorMessage( 1, [ '{} neither above nor below' \
                                              .format( self.vias[insertPosition][1] ) ] ))
                trace( 550, '\tcontactAbove={} bigviaAbove={}\n'.format( contactAbove, bigviaAbove ))
                if self.vias[insertPosition][1].getBoundingBox(railDepth).getYMax() >= contactBb.getYMin() \
                   and not xor(contactAbove,bigviaAbove):
                    trace( 550, ',--', '\tReject {} intersect PREVIOUS\n'.format(contact) )
                    return False
        trace( 550, ',--' )
        return True

    def connect ( self, contact ):
        trace( 550, '\tVerticalRail.connect() {}\n'.format(contact) )
        if self.net != contact.getNet():
            trace( 550, '\tReject {} vs. {}\n'.format( self.net, contact.getNet() ))
            return False
        viaWidth  = self.side.vRailWidth
        viaHeight = contact.getHeight()
        viaFlags  = BigVia.AllowAllExpand
        if self.conf.routingGauge.isSymbolic():
            viaWidth  -= DbU.fromLambda( 1.0 )
            viaHeight -= DbU.fromLambda( 1.0 )
        if self.conf.cfg.katana.disableStackedVias:
            viaFlags |= BigVia.StackVias
        self.vias[ contact.getY() ] = [ contact.getY()
                                      , BigVia( self.net
                                              , self.side.getLayerDepth(self.side.getVLayer())
                                              , self.axis
                                              , contact.getY()
                                              , self.side.vRailWidth - DbU.fromLambda(1.0)
                                              , contact.getHeight()  - DbU.fromLambda(1.0)
                                              , flags=viaFlags )
                                      , contact ]
        self.vias[ contact.getY() ][1].mergeDepth( self.side.getLayerDepth(contact.getLayer()) )
        trace( 550, '\t-> BigVIA {}\n'.format(self.vias[ contact.getY() ][1]) )
        return True


# --------------------------------------------------------------------
# Class  :  "corona.Side"

class Side ( object ):

    def __init__ ( self, corona ):
        self.corona = corona

    @property
    def side ( self ):
        raise NotImplementedError('Side.side is not implemented in base class.')

    @property
    def railsCount ( self ): return self.corona.railsCount

    @property
    def coronaCks ( self ):
        if self.corona.conf.useClockTree: return self.corona.conf.coronaCks
        return []

    @property           
    def innerBb ( self ): return self.corona.innerBb

    @property           
    def hRailWidth ( self ): return self.corona.hRailWidth

    @property           
    def hRailSpace ( self ): return self.corona.hRailSpace

    @property           
    def vRailWidth ( self ): return self.corona.vRailWidth

    @property           
    def vRailSpace ( self ): return self.corona.vRailSpace

    @property           
    def corners ( self ): return self.corona.corners

    @property
    def horizontalDepth ( self ): return self.corona.horizontalDepth

    @property
    def verticalDepth ( self ): return self.corona.verticalDepth

    @property
    def blockageNet ( self ): return self.corona.blockageNet

    def isHorizontal    ( self ):        return (self.side & HORIZONTAL) == HORIZONTAL
    def getLayerDepth   ( self, metal ): return self.corona.getLayerDepth(metal)
    def getRail         ( self, i ):     return self.rails[i]
    def getRailNet      ( self, i ):     return self.corona.getRailNet(i)
    def getHLayer       ( self ):        return self.corona.getHLayer()
    def getVLayer       ( self ):        return self.corona.getVLayer()

    def getRailAxis ( self, i ):
        raise ErrorMessage( 1, 'Side.getRailAxis(): Must never be called on base class.' )

    def getInnerRail ( self, i ):
        if i >= len(self.rails):
            raise ErrorMessage( 1, 'Side.getInnerRail(): no rail {} (only:{}).' \
                                   .format(i,len(self.rails)) )
        return self.rails[i]

    def getOuterRail ( self, i ):
        if i >= len(self.rails):
            raise ErrorMessage( 1, 'Side.getOuterRail(): no rail {} (only: {}).' \
                                   .format(i,len(self.rails)) )
        return self.rails[-(i+1)]

    def connect ( self, blockSide ):
        trace( 550, '\tSide.connect()\n' )
        for terminal in blockSide.terminals:
            trace( 550, '\tterminal:{}\n'.format(terminal) )
            for rail in self.rails:
                if not rail.isReachable( terminal[1] ):
                    break
                rail.connect( terminal[1] )

    def getRailRange ( self, net ):
        if net.isClock(): return range(len(self.rails))
        if not net.isSupply(): return []
        if self.side & HORIZONTAL:
            trace( 550, '\tHorizontal rail.\n' )
            return range( len(self.coronaCks), len(self.rails) )
        else:
            trace( 550, '\tVertical rail.\n' )
            trace( 550, '\t{} > {}\n'.format(self.horizontalDepth,self.verticalDepth) )
            if self.horizontalDepth > self.verticalDepth:
                return range( len(self.coronaCks), len(self.rails) )
        trace( 550, '\tUsing half rails only.\n' )
        return range( len(self.coronaCks) + len(self.rails)//2 - 2, len(self.rails) )

    def connectPads ( self, padSide ):
       #for contact in padSide.pins:
       #    if contact.getNet().isClock():
       #        for i in range(len(self.rails)):
       #            trace( 550, '\tConnect to [-{}] @{}\n'.format(i, DbU.getValueString(self.getOuterRail(i).axis)) )
       #            self.getOuterRail(i).connect( contact )
       #    elif contact.getNet().isSupply():
       #        self.getOuterRail( 0 ).connect( contact )
        for contact in padSide.pins:
            if not contact.getNet().isSupply() and not contact.getNet().isClock(): continue
            trace( 550, ',+', '\tConnect pad contact {}\n'.format(contact) )
            railRange = self.getRailRange( contact.getNet() )
            trace( 550, '\trailRange:{}\n'.format(railRange) )
            for i in railRange:
                trace( 550, '\tConnect to [-{}] @{}\n'.format(i, DbU.getValueString(self.getInnerRail(i).axis)) )
                self.getInnerRail(i).connect( contact )
            trace( 550, '-' )

    def doLayout ( self ):
        for rail in self.rails: rail.doLayout()


# --------------------------------------------------------------------
# Class  :  "corona.HorizontalSide"

class HorizontalSide ( Side ):

    def __init__ ( self, corona ):
        Side.__init__( self, corona )
        self.rails = []
        for i in range(self.railsCount):
            self.rails.append( HorizontalRail(self,i,self.getRailAxis(i)) )
        return


# --------------------------------------------------------------------
# Class  :  "corona.SouthSide"

class SouthSide ( HorizontalSide ):

    def __init__ ( self, corona ):
        HorizontalSide.__init__( self, corona )
        return

    @property
    def side ( self ): return South

    def getRailAxis ( self, i ):
        return self.innerBb.getYMin() -    self.hRailWidth//2 - self.hRailSpace \
                                      - i*(self.hRailWidth    + self.hRailSpace)

    def corner0 ( self, i ): return self.corners[SouthWest][i]
    def corner1 ( self, i ): return self.corners[SouthEast][i]


# --------------------------------------------------------------------
# Class  :  "corona.NorthSide"

class NorthSide ( HorizontalSide ):

    def __init__ ( self, corona ):
        HorizontalSide.__init__( self, corona )
        return

    @property
    def side ( self ): return North

    def getRailAxis ( self, i ):
        return self.innerBb.getYMax() +    self.hRailWidth//2 + self.hRailSpace \
                                      + i*(self.hRailWidth    + self.hRailSpace)

    def corner0 ( self, i ): return self.corners[NorthWest][i]
    def corner1 ( self, i ): return self.corners[NorthEast][i]


# --------------------------------------------------------------------
# Class  :  "corona.VerticalSide"

class VerticalSide ( Side ):

    def __init__ ( self, corona ):
        Side.__init__( self, corona )

        self.rails = []
        for i in range(self.railsCount):
            self.rails.append( VerticalRail(self,i,self.getRailAxis(i)) )
        return

    def addBlockages ( self, sideXMin, sideXMax ):
        spans = IntervalSet()
        for rail in self.rails:
            for via in rail.vias.values():
                if via[1].getNet() != via[2].getNet(): continue
                spans.merge( via[1].y - via[1].height//2, via[1].y + via[1].height//2 )
        routingGauge = self.corona.routingGauge
        for depth in range(next(iter(self.getOuterRail(0).vias.values()))[1].bottomDepth
                          ,next(iter(self.getOuterRail(0).vias.values()))[1].topDepth ):
            blockageLayer = routingGauge.getRoutingLayer(depth).getBlockageLayer()
            pitch         = routingGauge.getLayerPitch(depth)
            for chunk in spans.chunks:
                Horizontal.create( self.blockageNet
                                 , blockageLayer
                                 , (chunk.getVMax() + chunk.getVMin())//2
                                 ,  chunk.getVMax() - chunk.getVMin() + pitch*2
                                 , sideXMin
                                 , sideXMax
                                 )
            depth -= 2
            if depth > 0:
                blockageLayer = routingGauge.getRoutingLayer(depth).getBlockageLayer()
                pitch         = routingGauge.getLayerPitch(depth)
                for chunk in spans.chunks:
                    Horizontal.create( self.blockageNet
                                     , blockageLayer
                                     , (chunk.getVMax() + chunk.getVMin())//2
                                     ,  chunk.getVMax() - chunk.getVMin() + pitch*2
                                     , sideXMin
                                     , sideXMax
                                     )


# --------------------------------------------------------------------
# Class  :  "corona.WestSide"

class WestSide ( VerticalSide ):

    def __init__ ( self, corona ):
        VerticalSide.__init__( self, corona )

    @property
    def side ( self ): return West

    def getRailAxis ( self, i ):
        return self.innerBb.getXMin() -    self.vRailWidth//2 - self.vRailSpace \
                                      - i*(self.vRailWidth    + self.vRailSpace)

    def corner0 ( self, i ): return self.corners[SouthWest][i]
    def corner1 ( self, i ): return self.corners[NorthWest][i]

    def addBlockages ( self ):
        sideXMin = self.getOuterRail(0).axis - self.vRailWidth
        sideXMax = self.getInnerRail(0).axis + self.vRailWidth
        VerticalSide.addBlockages( self, sideXMin, sideXMax )


# --------------------------------------------------------------------
# Class  :  "corona.EastSide"

class EastSide ( VerticalSide ):

    def __init__ ( self, corona ):
        VerticalSide.__init__( self, corona )

    @property
    def side ( self ): return East

    def getRailAxis ( self, i ):
        return self.innerBb.getXMax() +    self.vRailWidth//2 + self.vRailSpace \
                                      + i*(self.vRailWidth    + self.vRailSpace)

    def corner0 ( self, i ): return self.corners[SouthEast][i]
    def corner1 ( self, i ): return self.corners[NorthEast][i]

    def addBlockages ( self ):
        sideXMin = self.getInnerRail(0).axis - self.vRailWidth
        sideXMax = self.getOuterRail(0).axis + self.vRailWidth
        VerticalSide.addBlockages( self, sideXMin, sideXMax )


# --------------------------------------------------------------------
# Class  :  "corona.Builder"

class Builder ( object ):

    def __init__ ( self, block ):
        self.block      = block
        self.innerBb    = self.block.icoreAb
        self.block.path.getTransformation().applyOn( self.innerBb )
        self.innerBb.inflate( self.hRailSpace//2, self.vRailSpace//2 )
        self.southSide  = SouthSide( self )
        self.northSide  = NorthSide( self )
        self.westSide   = WestSide ( self )
        self.eastSide   = EastSide ( self )

    @property
    def conf ( self ): return self.block.conf

    @property
    def routingGauge ( self ): return self.conf.routingGauge

    @property
    def topLayerDepth ( self ): return self.conf.topLayerDepth

    @property
    def horizontalDepth ( self ): return self.conf.horizontalDepth

    @property
    def verticalDepth ( self ): return self.conf.verticalDepth

    @property
    def blockageNet ( self ): return self.conf.blockageNet

    @property
    def railsCount ( self ): return self.conf.railsCount

    @property
    def hRailWidth ( self ): return self.conf.cfg.chip.block.rails.hWidth

    @property
    def vRailWidth ( self ): return self.conf.cfg.chip.block.rails.vWidth

    @property
    def hRailSpace ( self ): return self.conf.cfg.chip.block.rails.hSpacing

    @property
    def vRailSpace ( self ): return self.conf.cfg.chip.block.rails.vSpacing

    def getLayerDepth ( self, metal ):
        return self.conf.routingGauge.getLayerDepth( metal )

    def getRailNet ( self, i ):
        if self.conf.useClockTree and i < len(self.conf.coronaCks):
            return self.conf.coronaCks[i]
        if i % 2: return self.conf.coronaVss
        return self.conf.coronaVdd

    def getHLayer ( self ):
        return self.routingGauge.getLayerGauge( self.horizontalDepth ).getLayer()

    def getVLayer ( self ):
        return self.routingGauge.getLayerGauge( self.verticalDepth ).getLayer()

    def connectCore ( self ):
        for plane in self.block.planes.values():
            for side in plane.sides.values():
                self.southSide.connect( side[South] )
                self.northSide.connect( side[North] )
                self.westSide .connect( side[West ] )
                self.eastSide .connect( side[East ] )

    def connectPads ( self, padsCorona ):
        self.southSide.connectPads( padsCorona.southSide )
        self.northSide.connectPads( padsCorona.northSide )
        self.eastSide .connectPads( padsCorona.eastSide  )
        self.westSide .connectPads( padsCorona.westSide  )
        return

    def doLayout ( self ):
        self.corners = { SouthWest : []
                       , SouthEast : []
                       , NorthWest : []
                       , NorthEast : []
                       }
        contactDepth = self.horizontalDepth
        if self.horizontalDepth > self.verticalDepth:
            contactDepth = self.verticalDepth
        with UpdateSession():
            blBox = Box()
            brBox = Box()
            tlBox = Box()
            trBox = Box()
            for i in range(self.railsCount):
                xBL = self.westSide .getRail(i).axis
                yBL = self.southSide.getRail(i).axis
                xTR = self.eastSide .getRail(i).axis
                yTR = self.northSide.getRail(i).axis
                net = self.getRailNet( i )
                blBox.merge( xBL, yBL )
                brBox.merge( xTR, yBL )
                tlBox.merge( xBL, yTR )
                trBox.merge( xTR, yTR )
                self.routingGauge.getContactLayer(contactDepth)
                self.corners[SouthWest].append( 
                    BigVia( net, contactDepth, xBL, yBL, self.hRailWidth, self.vRailWidth , flags=BigVia.AllowAllExpand ) )
                self.corners[SouthWest][-1].mergeDepth( contactDepth+1 )
                self.corners[SouthWest][-1].doLayout()
                self.corners[NorthWest].append( 
                    BigVia( net, contactDepth, xBL, yTR, self.hRailWidth, self.vRailWidth , flags=BigVia.AllowAllExpand ) )
                self.corners[NorthWest][-1].mergeDepth( contactDepth+1 )
                self.corners[NorthWest][-1].doLayout()
                self.corners[SouthEast].append( 
                    BigVia( net, contactDepth, xTR, yBL, self.hRailWidth, self.vRailWidth , flags=BigVia.AllowAllExpand ) )
                self.corners[SouthEast][-1].mergeDepth( contactDepth+1 )
                self.corners[SouthEast][-1].doLayout()
                self.corners[NorthEast].append( 
                    BigVia( net, contactDepth, xTR, yTR, self.hRailWidth, self.vRailWidth , flags=BigVia.AllowAllExpand ) )
                self.corners[NorthEast][-1].mergeDepth( contactDepth+1 )
                self.corners[NorthEast][-1].doLayout()
            self.southSide.doLayout()
            self.northSide.doLayout()
            self.westSide .doLayout()
            self.eastSide .doLayout()
            self.westSide.addBlockages()
            self.eastSide.addBlockages()
            blBox.inflate( self.hRailWidth, self.vRailWidth )
            brBox.inflate( self.hRailWidth, self.vRailWidth )
            tlBox.inflate( self.hRailWidth, self.vRailWidth )
            trBox.inflate( self.hRailWidth, self.vRailWidth )
            for depth in range( 1, self.conf.topLayerDepth + 1 ):
                blockageLayer = self.routingGauge.getRoutingLayer(depth).getBlockageLayer()
                Pad.create( self.blockageNet, blockageLayer, blBox )
                Pad.create( self.blockageNet, blockageLayer, brBox )
                Pad.create( self.blockageNet, blockageLayer, tlBox )
                Pad.create( self.blockageNet, blockageLayer, trBox )
