
# This file is part of the Coriolis Software.
# Copyright (c) SU 2021-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/macro/macro.py"                 |
# +-----------------------------------------------------------------+


from   __future__ import print_function
import sys
import os.path
from   operator  import itemgetter, attrgetter, methodcaller
import Cfg
from   Hurricane import Breakpoint, DbU, Box, Transformation, Point, \
                        Box, Path, Layer, Occurrence, Net,           \
                        NetExternalComponents, RoutingPad, Pad,      \
                        Horizontal, Vertical, Contact, Pin, Plug,    \
                        Cell, Instance, Rectilinear
import CRL
from   CRL             import RoutingLayerGauge
from   helpers         import trace, dots
from   helpers.io      import ErrorMessage, WarningMessage, catch
from   helpers.overlay import UpdateSession


# ----------------------------------------------------------------------------
# Class  :  "macro.Macro".

class Macro ( object ):
    """
    Encapsulate the layout of a user-defined block so it can be used
    in the Coriolis P&R flow. Maintain a lookup table of already
    processeds macros so they are modified only once.
    """

    trace( 550, '\tStatic init of Macro\n' )
    LUT = {}

    @staticmethod
    def lookup ( macroCell ):
        trace( 550, '\tMacro.lookup() on {}\n'.format(macroCell) )
        if Macro.LUT.has_key(macroCell): return Macro.LUT[ macroCell ]
        return None

    @staticmethod
    def place ( topCell, instance, transf, status ):
        macro   = Macro.lookup( instance.getMasterCell() )
        ab      = instance.getMasterCell().getAbutmentBox()
        abShift = Transformation( -ab.getXMin(), -ab.getYMin(), Transformation.Orientation.ID )
        abShift.applyOn( transf )
        instance.setTransformation( transf )
        instance.setPlacementStatus( status )

    @staticmethod
    def wrap ( macroCell, gaugeName, hMargin, vMargin ):
        """
        Adjust the interface of a macro block cell to better fit a
        given routing gauge. Keep a table of the already processed
        blocks (must be run only once for a block).

        For an explanation of the parameters, see Macro.__init__().
        """

        macro = Macro.lookup( macroCell )
        if macro is not None:
            trace( 550, '\tReusing macro wrapper {}\n'.format(macroCell) )
            return macro
        return Macro( macroCell, gaugeName, hMargin, vMargin )

    def getPPitch ( self, metal ):
        depth = self.rg.getLayerDepth( metal )
        if depth < self.rg.getDepth(): pdepth = depth + 1
        else:                          pdepth = depth - 1
        return self.rg.getLayerPitch( pdepth )

    def getWireWidth ( self, metal ):
        return self.rg.getWireWidth( metal )

    def getNearestTrackAxis ( self, metal, axis ):
        lg = self.rg.getLayerGauge( metal )
        if lg.getDirection() == RoutingLayerGauge.Horizontal:
            axisMin = self.outerAb.getYMin()
            axisMax = self.outerAb.getYMax()
        else:
            axisMin = self.outerAb.getXMin()
            axisMax = self.outerAb.getXMax()
        trackIndex = lg.getTrackIndex( axisMin, axisMax, axis, RoutingLayerGauge.Nearest )
        return axisMin + lg.getOffset() + trackIndex * lg.getPitch()

    def __init__ ( self, macroCell, gaugeName, hMargin, vMargin ):
        """
        Encapsulate a macro cell.to better fit the routing gauge.

        :param macroCell: The macro cell to be encapsulated.
        :param gaugeName: The name of the RoutingGauge to use.
        :param hMargin:   The number of pitchs the horizontal terminals
                          will stick out of the original abutment box.
        :param vMargin:   The number of pitchs the vertical terminals
                          will stick out of the original abutment box.

        The abutment box of the block is enlarged so both it's dimensions
        are a multiple of the sliceHeight. It is then enlarged of one  more
        sliceHeight on each side. The resulting block will:

        * Exactly fit the GCell grid.
        * Eternal pins will be isolated in fully empty GCells on the side.
          Due to some quirks in the P&R algorithm, putting them in GCells
          that are half free and half occluded by the block itself may
          cause (stupid) deadlock to appear.
        """
        trace( 550, '\tMacro.__init__() {}\n'.format(macroCell) )
        self.cell = macroCell
        Macro.LUT[ self.cell ] = self

        af = CRL.AllianceFramework.get()
        ab = self.cell.getAbutmentBox()
        self.rg = af.getRoutingGauge( gaugeName )
        gaugeMetal2      = self.rg.getLayerGauge( 1 )
        gaugeMetal3      = self.rg.getLayerGauge( 2 )
        gaugeMetal4      = self.rg.getLayerGauge( 3 )
        blockageMetal2   = gaugeMetal2.getBlockageLayer()
        blockageMetal3   = gaugeMetal3.getBlockageLayer()
        blockageMetal4   = gaugeMetal4.getBlockageLayer()
        minSpacingMetal2 = gaugeMetal2.getLayer().getMinimalSpacing()
        minSpacingMetal3 = gaugeMetal3.getLayer().getMinimalSpacing()
        minSpacingMetal4 = gaugeMetal4.getLayer().getMinimalSpacing()
        if self.cell.getName() == 'SPBlock_512W64B8W':
            print( '  o  Ad-hoc blockage patch for "{}".'.format(self.cell.getName()) )
            for net in self.cell.getNets():
                for component in net.getComponents():
                    if isinstance(component,Rectilinear) and component.getLayer() == blockageMetal2:
                        bb = component.getBoundingBox()
                        bb.inflate( minSpacingMetal2 )
                        Horizontal.create( component.getNet()
                                         , blockageMetal2
                                         , bb.getYCenter()
                                         , bb.getHeight()
                                         , bb.getXMin()
                                         , bb.getXMax() )
                    elif isinstance(component,Rectilinear) and component.getLayer() == blockageMetal3:
                        bb = component.getBoundingBox()
                        bb.inflate( 2*minSpacingMetal3, minSpacingMetal3/2 )
                        Vertical.create( component.getNet()
                                       , blockageMetal3
                                       , bb.getXCenter()
                                       , bb.getWidth()
                                       , bb.getYMin()
                                       , bb.getYMax() )
                    elif isinstance(component,Rectilinear) and component.getLayer() == blockageMetal4:
                        bb = component.getBoundingBox()
                        bb.inflate( minSpacingMetal4 )
                        Horizontal.create( component.getNet()
                                         , blockageMetal4
                                         , bb.getYCenter()
                                         , bb.getHeight()
                                         , bb.getXMin()
                                         , bb.getXMax() )
        self.innerAb = ab
        sliceHeight = af.getCellGauge( gaugeName ).getSliceHeight()
        westPins  = []
        eastPins  = []
        northPins = []
        southPins = []
        for net in self.cell.getNets():
            if not net.isExternal() or net.isSupply() or net.isClock():
                continue
            for component in net.getComponents():
                if not NetExternalComponents.isExternal(component):
                    continue
                bb = component.getBoundingBox()
                if not ab.isConstrainedBy(bb):
                    continue
                if   ab.getXMax() == bb.getXMax():  eastPins.append( component )
                elif ab.getXMin() == bb.getXMin():  westPins.append( component )
                elif ab.getYMax() == bb.getYMax(): northPins.append( component )
                elif ab.getYMin() == bb.getYMin(): southPins.append( component )
        xAdjust = 0
        yAdjust = 0
        if ab.getWidth () % sliceHeight: xAdjust = sliceHeight - ab.getWidth () % sliceHeight
        if ab.getHeight() % sliceHeight: yAdjust = sliceHeight - ab.getHeight() % sliceHeight
        self.innerAb.inflate( 0, 0, xAdjust, yAdjust )
        self.outerAb = self.innerAb
        self.outerAb.inflate( sliceHeight )
        westPins .sort( key=lambda k: k.getBoundingBox().getYCenter() )
        eastPins .sort( key=lambda k: k.getBoundingBox().getYCenter() )
        northPins.sort( key=lambda k: k.getBoundingBox().getXCenter() )
        southPins.sort( key=lambda k: k.getBoundingBox().getXCenter() )
                            
        with UpdateSession():
            for component in westPins:
                NetExternalComponents.setInternal( component )
                pitch     = self.rg.getPitch( component.getLayer() )
                ppitch    = self.getPPitch( component.getLayer() )
                wwidth    = self.getWireWidth( component.getLayer() )
                bb        = component.getBoundingBox()
                yAxis     = bb.getYCenter()
                yOngrid   = self.getNearestTrackAxis( component.getLayer(), yAxis )
                xMax      = bb.getXMin()
                xMin      = xMax - hMargin*ppitch
                width     = bb.getHeight()
                ppYAxis   = yAxis
                ppYOngrid = yOngrid
                if not self.rg.isSymbolic():
                    if ppYAxis < ppYOngrid:
                        ppYAxis   -= width/2
                        ppYOngrid += wwidth/2
                    else:
                        ppYAxis   += width/2
                        ppYOngrid -= wwidth/2
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , bb.getXMin()
                                          , width
                                          , ppYAxis
                                          , ppYOngrid
                                          )
                horizontal = Horizontal.create( component.getNet()
                                              , component.getLayer()
                                              , yOngrid
                                              , wwidth
                                              , xMin
                                              , xMax
                                              )
                horizontal = Horizontal.create( component.getNet()
                                              , component.getLayer()
                                              , yOngrid
                                              , wwidth
                                              , xMin
                                              , xMax - ppitch
                                              )
                NetExternalComponents.setExternal( horizontal )
            for component in eastPins:
                NetExternalComponents.setInternal( component )
                pitch     = self.rg.getPitch( component.getLayer() )
                ppitch    = self.getPPitch( component.getLayer() )
                wwidth    = self.getWireWidth( component.getLayer() )
                bb        = component.getBoundingBox()
                yAxis     = bb.getYCenter()
                yOngrid   = self.getNearestTrackAxis( component.getLayer(), yAxis )
                xMin      = self.innerAb.getXMax()
                xMax      = xMin + hMargin*ppitch
                width     = bb.getHeight()
                ppYAxis   = yAxis
                ppYOngrid = yOngrid
                if not self.rg.isSymbolic():
                    if ppYAxis < ppYOngrid:
                        ppYAxis   -= width/2
                        ppYOngrid += wwidth/2
                    else:
                        ppYAxis   += width/2
                        ppYOngrid -= wwidth/2
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , bb.getXMax()
                                          , width
                                          , ppYAxis
                                          , ppYOngrid
                                          )
                horizontal = Horizontal.create( component.getNet()
                                              , component.getLayer()
                                              , yOngrid
                                              , pitch + wwidth
                                              , xMin
                                              , xMax
                                              )
                horizontal = Horizontal.create( component.getNet()
                                              , component.getLayer()
                                              , yOngrid
                                              , pitch + wwidth
                                              , xMin + ppitch
                                              , xMax
                                              )
                NetExternalComponents.setExternal( horizontal )
            for component in southPins:
                NetExternalComponents.setInternal( component )
                pitch     = self.rg.getPitch( component.getLayer() )
                ppitch    = self.getPPitch( component.getLayer() )
                wwidth    = self.getWireWidth( component.getLayer() )
                bb        = component.getBoundingBox()
                xAxis     = bb.getXCenter()
                xOngrid   = self.getNearestTrackAxis( component.getLayer(), xAxis )
                yMax      = bb.getYMin()
                yMin      = yMax - vMargin*ppitch
                width     = bb.getWidth()
                ppXAxis   = xAxis
                ppXOngrid = xOngrid
                if not self.rg.isSymbolic():
                    if ppXAxis < ppXOngrid:
                        ppXAxis   -= width/2
                        ppXOngrid += wwidth/2
                    else:
                        ppXAxis   += width/2
                        ppXOngrid -= wwidth/2
                horizontal = Horizontal.create( component.getNet()
                                              , component.getLayer()
                                              , bb.getYMin()
                                              , width
                                              , ppXAxis
                                              , ppXOngrid
                                              )
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , xOngrid
                                          , wwidth
                                          , yMin
                                          , yMax
                                          )
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , xOngrid
                                          , wwidth
                                          , yMin
                                          , yMax - ppitch
                                          )
                NetExternalComponents.setExternal( vertical )
            for component in northPins:
                NetExternalComponents.setInternal( component )
                pitch     = self.rg.getPitch( component.getLayer() )
                ppitch    = self.getPPitch( component.getLayer() )
                wwidth    = self.getWireWidth( component.getLayer() )
                bb        = component.getBoundingBox()
                xAxis     = bb.getXCenter()
                xOngrid   = self.getNearestTrackAxis( component.getLayer(), xAxis )
                yMin      = self.innerAb.getYMax()
                yMax      = yMin + vMargin*ppitch
                width     = bb.getWidth()
                ppXAxis   = xAxis
                ppXOngrid = xOngrid
                if not self.rg.isSymbolic():
                    if ppXAxis < ppXOngrid:
                        ppXAxis   -= width/2
                        ppXOngrid += wwidth/2
                    else:
                        ppXAxis   += width/2
                        ppXOngrid -= wwidth/2
                horizontal = Horizontal.create( component.getNet()
                                              , component.getLayer()
                                              , bb.getYMax()
                                              , width
                                              , ppXAxis
                                              , ppXOngrid
                                              )
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , xOngrid
                                          , pitch + wwidth
                                          , yMin
                                          , yMax
                                          )
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , xOngrid
                                          , pitch + wwidth
                                          , yMin + ppitch
                                          , yMax
                                          )
                NetExternalComponents.setExternal( vertical )
        self.cell.setAbutmentBox( self.outerAb )
