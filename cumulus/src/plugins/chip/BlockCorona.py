#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2014, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/BlockCorona.py"            |
# +-----------------------------------------------------------------+


import bisect
from   operator  import methodcaller  
import Cfg
from   Hurricane import DbU
from   Hurricane import Point
from   Hurricane import Transformation
from   Hurricane import Box
from   Hurricane import Path
from   Hurricane import Occurrence
from   Hurricane import UpdateSession
from   Hurricane import Net
from   Hurricane import Contact
from   Hurricane import Horizontal
from   Hurricane import Vertical
import CRL
from   CRL import RoutingLayerGauge
from   helpers   import ErrorMessage
from   helpers   import WarningMessage
from   plugins   import StackedVia
import chip.BlockPower


class Rail ( object ):

    def __init__ ( self, side, order, axis ):
        self.side  = side
        self.order = order
        self.axis  = axis
        self.vias  = { }    # Key:pos Element: [pos,railContact,blockContact]
        return

    @property
    def net ( self ): return self.side.getRailNet(self.order)


class HorizontalRail ( Rail ):

    def __init__ ( self, side, order, axis ):
        Rail.__init__( self, side, order, axis )
        return

    def connect ( self, contact ):
        contactBb = contact.getBoundingBox()
        if    contactBb.getXMin() < self.side.innerBb.getXMin() \
           or contactBb.getXMax() > self.side.innerBb.getXMax():
            raise ErrorMessage( 1, [ '%s is outside rail/corona X range,' % str(contact)
                                   , 'power pad is likely to be to far off west or east.'
                                   , '(corona:%s)'                        % str(self.side.innerBb) ] ) 

       #print '  HorizontalRail.connect() net:%s contact:%s' % (self.net.getName(),str(contact))
        if self.net != contact.getNet(): return False
        if self.vias.has_key(contact.getX()): return False
        
        keys        = self.vias.keys()
        keys.sort()
        insertIndex = bisect.bisect_left( keys, contact.getX() )

        if len(keys) > 0:
            if insertIndex < len(keys):
                insertPosition = keys[ insertIndex ]
                if contactBb.getXMax() >= self.vias[insertPosition][2].getBoundingBox().getXMin():
                   #print 'Reject contact %s intersect NEXT' % str(contact)
                    return False
            if insertIndex > 0:
                if self.vias[keys[insertIndex-1]][2].getBoundingBox().getXMax() >= contactBb.getXMin():
                   #print 'Reject contact %s intersect PREVIOUS' % str(contact)
                    return False

        self.vias[ contact.getX() ] = [ contact.getX()
                                      , StackedVia( self.net
                                                  , self.side.getLayerDepth(self.side.getHLayer())
                                                  , contact.getX()
                                                  , self.axis
                                                  , contact.getWidth()
                                                  , self.side.hRailWidth
                                                  )
                                      , contact ]
       #print '    ADD contact @ [%d %d]' % (DbU.toLambda(contact.getX()), DbU.toLambda(self.axis))
        self.vias[ contact.getX() ][1].mergeDepth( self.side.getLayerDepth(contact.getLayer()) )
        return True

    def doLayout ( self ):
        for via in self.vias.values():
          via[1].doLayout()

        Horizontal.create( self.side.corner0(self.order)
                         , self.side.corner1(self.order)
                         , self.side.getHLayer()
                         , self.axis
                         , self.side.hRailWidth
                         )
        return

    def doLayout ( self ):
       #print 'HorizontalRail[%s] @%d' % (self.order,DbU.toLambda(self.axis))

        railVias = [ self.side.corner0(self.order)
                   , self.side.corner1(self.order) ]

        for via in self.vias.values():
          via[1].doLayout()
         #print '  Connect:', via[2], via[1].getVia( via[2].getLayer() )
          Vertical.create( via[1].getVia( via[2].getLayer() )
                         , via[2]
                         , via[2].getLayer()
                         , via[2].getX()
                         , via[2].getWidth()
                         )
          railVias.append( via[1].getVia( self.side.getVLayer()) )

        railVias.sort( key=methodcaller('getY') )

        for i in range(1,len(railVias)):
          Horizontal.create( railVias[i-1]
                           , railVias[i]
                           , self.side.getHLayer()
                           , self.axis
                           , self.side.hRailWidth
                           )
        return


class VerticalRail ( Rail ):

    def __init__ ( self, side, order, axis ):
        Rail.__init__( self, side, order, axis )
        return

    def doLayout ( self ):
       #print 'VerticalRail[%s] @%d' % (self.order,DbU.toLambda(self.axis))

        railVias = [ self.side.corner0(self.order)
                   , self.side.corner1(self.order) ]

        for via in self.vias.values():
         #print '  Connect: ', via[2]
          via[1].doLayout()
          Horizontal.create( via[1].getVia( via[2].getLayer() )
                           , via[2]
                           , via[2].getLayer()
                           , via[2].getY()
                           , via[2].getHeight()
                           )
          railVias.append( via[1].getVia( self.side.getVLayer()) )

        railVias.sort( key=methodcaller('getY') )

        for i in range(1,len(railVias)):
          Vertical.create( railVias[i-1]
                         , railVias[i]
                         , self.side.getVLayer()
                         , self.axis
                         , self.side.vRailWidth
                         )
        return

    def connect ( self, contact ):
        contactBb = contact.getBoundingBox()
        if    contactBb.getYMin() < self.side.innerBb.getYMin() \
           or contactBb.getYMax() > self.side.innerBb.getYMax():
            raise ErrorMessage( 1, [ '%s is outside rail/corona Y range' % str(contact)
                                   , 'power pad is likely to be to far off north or south.'
                                   , '(corona:%s)'                       % str(self.side.innerBb) ] ) 

        if self.net != contact.getNet(): return False
        if self.vias.has_key(contact.getY()): return False
        
        keys        = self.vias.keys()
        keys.sort()
        insertIndex = bisect.bisect_left( keys, contact.getY() )
       #print 'keys:', keys

        if len(keys) > 0:
            if insertIndex < len(keys):
                insertPosition = keys[ insertIndex ]
               #print 'insertIndex:%d' % insertIndex
               #print 'Check NEXT contactBb:%s via:%s' \
               #    % ( str(contactBb)
               #      , str(self.vias[insertPosition][2].getBoundingBox()) )
                if contactBb.getYMax() >= self.vias[insertPosition][2].getBoundingBox().getYMin():
               #    print 'Reject %s intersect NEXT' % str(contact)
                    return False
            if insertIndex > 0:
               #print 'check PREVIOUS contactBb:%s via:%s' \
               #    % ( str(contactBb)
               #      , str(self.vias[keys[insertIndex-1]][2].getBoundingBox()) )
                if self.vias[keys[insertIndex-1]][2].getBoundingBox().getYMax() >= contactBb.getYMin():
               #    print 'Reject %s intersect PREVIOUS' % str(contact)
                    return False

        self.vias[ contact.getY() ] = [ contact.getY()
                                      , StackedVia( self.net
                                                  , self.side.getLayerDepth(self.side.getVLayer())
                                                  , self.axis
                                                  , contact.getY()
                                                  , self.side.vRailWidth
                                                  , contact.getHeight()
                                                  )
                                      , contact ]
       #print 'ADD %s' % str(contact)
        self.vias[ contact.getY() ][1].mergeDepth( self.side.getLayerDepth(contact.getLayer()) )
        return True


class Side ( object ):

    def __init__ ( self, corona ):
        self._corona = corona
        return

    @property
    def railsNb       ( self ): return self._corona._railsNb
    @property         
    def innerBb       ( self ): return self._corona._innerBb
    @property         
    def hRailWidth    ( self ): return self._corona._hRailWidth
    @property         
    def hRailSpace    ( self ): return self._corona._hRailSpace
    @property         
    def vRailWidth    ( self ): return self._corona._vRailWidth
    @property         
    def vRailSpace    ( self ): return self._corona._vRailSpace
    @property         
    def corners       ( self ): return self._corona._corners

    def getLayerDepth ( self, metal ): return self._corona.getLayerDepth(metal)
    def getRail       ( self, i ):     return self._rails[i]
    def getRailNet    ( self, i ):     return self._corona.getRailNet(i)
    def getHLayer     ( self ):        return self._corona.getHLayer()
    def getVLayer     ( self ):        return self._corona.getVLayer()

    def getRailAxis ( self, i ):
        raise ErrorMessage( 1, 'Side.getRailAxis(): Must never be called on base class.' )

    def getInnerRail ( self, i ):
        if i >= len(self._rails):
            raise ErrorMessage( 1, 'Side.getInnerRail(): no rail %d (only: %d).' % (i,len(self._rails)) )
        return self._rails[i]

    def getOuterRail ( self, i ):
        if i >= len(self._rails):
            raise ErrorMessage( 1, 'Side.getOuterRail(): no rail %d (only: %d).' % (i,len(self._rails)) )
        return self._rails[-(i+1)]

    def connect ( self, blockSide ):
        for terminal in blockSide.terminals:
          for rail in self._rails:
            rail.connect( terminal[1] )
        return

    def connectPads ( self, padSide ):
        halfRails = len(self._rails)/2
        for terminal in padSide._powerContacts:
         #print 'Connect pad terminal ', terminal
          for i in range(halfRails):
           #print '  Connect to [-%i] @%d' % (i, DbU.toLambda(self.getOuterRail(i).axis))
            self.getOuterRail(i).connect( terminal )
        return

    def doLayout ( self ):
        for rail in self._rails: rail.doLayout()
        return


class HorizontalSide ( Side ):

    def __init__ ( self, corona ):
       #print 'HorizontalSide.__init__()'
        Side.__init__( self, corona )

        self._rails = []
        for i in range(self.railsNb):
            self._rails.append( HorizontalRail(self,i,self.getRailAxis(i)) )
           #print '  Rail [%i] @%d' % (i,DbU.toLambda(self._rails[-1].axis))
        return


class SouthSide ( HorizontalSide ):

    def __init__ ( self, corona ):
        HorizontalSide.__init__( self, corona )
        return

    def getRailAxis ( self, i ):
        return self.innerBb.getYMin() -    self.hRailWidth/2 - self.hRailSpace \
                                      - i*(self.hRailWidth   + self.hRailSpace)

    def corner0 ( self, i ): return self.corners[chip.SouthWest ][i]
    def corner1 ( self, i ): return self.corners[chip.SouthEast][i]


class NorthSide ( HorizontalSide ):

    def __init__ ( self, corona ):
        HorizontalSide.__init__( self, corona )
        return

    def getRailAxis ( self, i ):
        return self.innerBb.getYMax() +    self.hRailWidth/2 + self.hRailSpace \
                                      + i*(self.hRailWidth   + self.hRailSpace)

    def corner0 ( self, i ): return self.corners[chip.NorthWest ][i]
    def corner1 ( self, i ): return self.corners[chip.NorthEast][i]


class VerticalSide ( Side ):

    def __init__ ( self, corona ):
        Side.__init__( self, corona )

        self._rails = []
        for i in range(self.railsNb):
            self._rails.append( VerticalRail(self,i,self.getRailAxis(i)) )
        return


class WestSide ( VerticalSide ):

    def __init__ ( self, corona ):
        VerticalSide.__init__( self, corona )
        return

    def getRailAxis ( self, i ):
        return self.innerBb.getXMin() -    self.vRailWidth/2 - self.vRailSpace \
                                      - i*(self.vRailWidth   + self.vRailSpace)

    def corner0 ( self, i ): return self.corners[chip.SouthWest][i]
    def corner1 ( self, i ): return self.corners[chip.NorthWest   ][i]


class EastSide ( VerticalSide ):

    def __init__ ( self, corona ):
        VerticalSide.__init__( self, corona )
        return

    def getRailAxis ( self, i ):
        return self.innerBb.getXMax() +    self.vRailWidth/2 + self.vRailSpace \
                                      + i*(self.vRailWidth   + self.vRailSpace)

    def corner0 ( self, i ): return self.corners[chip.SouthEast][i]
    def corner1 ( self, i ): return self.corners[chip.NorthEast   ][i]


class Corona ( object ):

    def __init__ ( self, block ):
        if not isinstance(block,chip.BlockPower.Block):
            raise ErrorMessage( 1, 'Attempt to create a Corona on a non-Block object.' )

        self._framework    = CRL.AllianceFramework.get()
        self._routingGauge = self._framework.getRoutingGauge()
        self._block        = block
        self._innerBb      = self._block.bb
        self._block.path.getTransformation().applyOn( self._innerBb )

        self._railsNb         = 4
        self._hRailWidth      = DbU.fromLambda( 12.0 )
        self._vRailWidth      = DbU.fromLambda( 12.0 )
        self._hRailSpace      = DbU.fromLambda(  6.0 )
        self._vRailSpace      = DbU.fromLambda(  6.0 )
        self._topLayerDepth   = 0
        self._horizontalDepth = 0
        self._verticalDepth   = 0

        self._southSide  = SouthSide( self )
        self._northSide  = NorthSide( self )
        self._westSide   = WestSide ( self )
        self._eastSide   = EastSide ( self )

        self._guessHvLayers()
        return

    def _guessHvLayers ( self ):
        topLayer = Cfg.getParamString('katabatic.topRoutingLayer').asString()
        self._topLayerDepth = 0
        for layerGauge in self._routingGauge.getLayerGauges():
          if layerGauge.getLayer().getName() == topLayer:
            self._topLayerDepth = layerGauge.getDepth()
            break
        if not self._topLayerDepth:
          print WarningMessage( 'Gauge top layer not defined, using top of gauge (%d).' \
                                % self._routingGauge.getDepth() )
          self._topLayerDepth = self._routingGauge.getDepth()
        
        for depth in range(0,self._topLayerDepth+1):
          if self._routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
            self._horizontalDepth = depth
          if self._routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Vertical:
            self._verticalDepth = depth
        return

    def getLayerDepth ( self, metal ):
        return self._routingGauge.getLayerDepth( metal )

    def getRailNet ( self, i ):
        if i % 2: return self._block.vssi
        return self._block.vddi

    def getHLayer ( self ):
        return self._routingGauge.getLayerGauge( self._horizontalDepth ).getLayer()

    def getVLayer ( self ):
        return self._routingGauge.getLayerGauge( self._verticalDepth ).getLayer()

    def connectBlock ( self ):
        for plane in self._block.planes.values():
          for side in plane.sides.values():
            self._southSide.connect( side[chip.South] )
            self._northSide.connect( side[chip.North] )
            self._westSide .connect( side[chip.West ] )
            self._eastSide .connect( side[chip.East ] )
        return

    def connectPads ( self, padsCorona ):
        self._southSide.connectPads( padsCorona.southSide )
        self._northSide.connectPads( padsCorona.northSide )
        self._eastSide.connectPads( padsCorona.eastSide )
        self._westSide.connectPads( padsCorona.westSide )
        return

    def doLayout ( self ):
        self._corners = { chip.SouthWest  : []
                        , chip.SouthEast : []
                        , chip.NorthWest     : []
                        , chip.NorthEast    : []
                        }

        contactDepth = self._horizontalDepth
        if self._horizontalDepth > self._verticalDepth:
            contactDepth = self._verticalDepth

        for i in range(self._railsNb):
            xBL = self._westSide .getRail(i).axis
            yBL = self._southSide.getRail(i).axis
            xTR = self._eastSide .getRail(i).axis
            yTR = self._northSide.getRail(i).axis
            net = self.getRailNet( i )

            self._corners[chip.SouthWest].append( 
                Contact.create( net
                              , self._routingGauge.getContactLayer(contactDepth)
                              , xBL, yBL
                              , self._hRailWidth
                              , self._vRailWidth
                              ) )
            self._corners[chip.NorthWest].append( 
                Contact.create( net
                              , self._routingGauge.getContactLayer(contactDepth)
                              , xBL, yTR
                              , self._hRailWidth
                              , self._vRailWidth
                              ) )
            self._corners[chip.SouthEast].append( 
                Contact.create( net
                              , self._routingGauge.getContactLayer(contactDepth)
                              , xTR, yBL
                              , self._hRailWidth
                              , self._vRailWidth
                              ) )
            self._corners[chip.NorthEast].append( 
                Contact.create( net
                              , self._routingGauge.getContactLayer(contactDepth)
                              , xTR, yTR
                              , self._hRailWidth
                              , self._vRailWidth
                              ) )

        self._southSide.doLayout()
        self._northSide.doLayout()
        self._westSide .doLayout()
        self._eastSide .doLayout()
        return
