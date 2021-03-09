
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
import Cfg
from   Hurricane import Breakpoint, DbU, Box, Transformation, Point, \
                        Box, Path, Layer, Occurrence, Net,           \
                        NetExternalComponents, RoutingPad, Pad,      \
                        Horizontal, Vertical, Contact, Pin, Plug,    \
                        Cell, Instance
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

    LUT = {}

    @staticmethod
    def lookup ( macroCell ):
        if Macro.LUT.has_key(macroCell): return True
        return False

    @staticmethod
    def getPPitch ( rg, metal ):
        depth = rg.getLayerDepth( metal )
        if depth < rg.getDepth(): pdepth = depth + 1
        else:                     pdepth = depth - 1
        return rg.getLayerPitch( pdepth )

    @staticmethod
    def getWireWidth ( rg, metal ):
        return rg.getWireWidth( metal )

    @staticmethod
    def place ( instance, transf, status ):
        ab      = instance.getMasterCell().getAbutmentBox()
        abShift = Transformation( -ab.getXMin(), -ab.getYMin(), Transformation.Orientation.ID )
        abShift.applyOn( transf )
        instance.setTransformation( transf )
        instance.setPlacementStatus( status )

    @staticmethod
    def wrap ( macroCell, gaugeName, hMargin, vMargin ):
        """
        Encapsulate the macro cell, if not already done. Returns True
        if the encapsulation actually takes place.

        :param macroCell: The macro cell to be encapsulated.
        :param gaugeName: The name of the RoutingGauge to use.
        :param xMargin:   The horizontal margin, expressed in pitchs of the
                          lowest vertical routing metal (usually METAL3).
        :param yMargin:   The vertical margin, expressed in pitchs of the
                          lowesthorizontal routing metal (usually METAL2).

        The abutment box of the block is enlarged so both it's dimensions
        are a multiple of the sliceHeight. It is then enlarged of one  more
        sliceHeight on each side. The resulting block will:

        * Exactly fit the GCell grid.
        * Eternal pins will be isolated in fully empty GCells on the side.
          Due to some quirks in the P&R algorithm, putting them in GCells
          that are half free and half occluded by the block itself may
          cause (stupid) deadlock to appear.
        """
        if Macro.lookup(macroCell): return False

        af = CRL.AllianceFramework.get()
        rg = af.getRoutingGauge( gaugeName )
        ab = macroCell.getAbutmentBox()
        sliceHeight = af.getCellGauge( gaugeName ).getSliceHeight()
        westPins  = []
        eastPins  = []
        northPins = []
        southPins = []
        for net in macroCell.getNets():
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
        innerAb = ab
        xAdjust = 0
        yAdjust = 0
        if ab.getWidth () % sliceHeight: xAdjust = sliceHeight - ab.getWidth () % sliceHeight
        if ab.getHeight() % sliceHeight: yAdjust = sliceHeight - ab.getHeight() % sliceHeight
        innerAb.inflate( 0, 0, xAdjust, yAdjust )
        outerAb = innerAb
        outerAb.inflate( sliceHeight )
                            
        with UpdateSession():
            for component in westPins:
                NetExternalComponents.setInternal( component )
                pitch  = rg.getPitch( component.getLayer() )
                ppitch = Macro.getPPitch( rg, component.getLayer() )
                wwidth = Macro.getWireWidth( rg, component.getLayer() )
                bb     = component.getBoundingBox()
                yAxis  = bb.getYCenter()
                xMax   = bb.getXMin()
                xMin   = xMax - hMargin*ppitch
                width  = bb.getHeight()
                horizontal = Horizontal.create( component.getNet()
                                              , component.getLayer()
                                              , yAxis
                                              , width
                                              , xMin
                                              , xMax
                                              )
                horizontal = Horizontal.create( component.getNet()
                                              , component.getLayer()
                                              , yAxis
                                              , pitch + wwidth
                                              , xMin
                                              , xMax - (hMargin-1) * ppitch
                                              )
                NetExternalComponents.setExternal( horizontal )
            for component in eastPins:
                NetExternalComponents.setInternal( component )
                pitch  = rg.getPitch( component.getLayer() )
                ppitch = Macro.getPPitch( rg, component.getLayer() )
                wwidth = Macro.getWireWidth( rg, component.getLayer() )
                bb     = component.getBoundingBox()
                yAxis  = bb.getYCenter()
                xMin   = innerAb.getXMax()
                xMax   = xMin + hMargin*ppitch
                width  = bb.getHeight()
                horizontal = Horizontal.create( component.getNet()
                                              , component.getLayer()
                                              , yAxis
                                              , width
                                              , xMin
                                              , xMax
                                              )
                horizontal = Horizontal.create( component.getNet()
                                              , component.getLayer()
                                              , yAxis
                                              , pitch + wwidth
                                              , xMin + (hMargin-1) * ppitch
                                              , xMax
                                              )
                NetExternalComponents.setExternal( horizontal )
            for component in southPins:
                NetExternalComponents.setInternal( component )
                pitch  = rg.getPitch( component.getLayer() )
                ppitch = Macro.getPPitch( rg, component.getLayer() )
                wwidth = Macro.getWireWidth( rg, component.getLayer() )
                bb     = component.getBoundingBox()
                xAxis  = bb.getXCenter()
                yMax   = bb.getYMin()
                yMin   = xMax - vMargin*ppitch
                width  = bb.getWidth()
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , xAxis
                                          , width
                                          , yMin
                                          , yMax
                                          )
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , xAxis
                                          , pitch + wwidth
                                          , yMin
                                          , yMax - (vMargin-1) * ppitch
                                          )
                NetExternalComponents.setExternal( vertical )
            for component in northPins:
                NetExternalComponents.setInternal( component )
                pitch  = rg.getPitch( component.getLayer() )
                ppitch = Macro.getPPitch( rg, component.getLayer() )
                wwidth = Macro.getWireWidth( rg, component.getLayer() )
                bb     = component.getBoundingBox()
                xAxis  = bb.getXCenter()
                yMin   = innerAb.getYMax()
                yMax   = xMin + vMargin*ppitch
                width  = bb.getWidth()
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , xAxis
                                          , width
                                          , yMin
                                          , yMax
                                          )
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , xAxis
                                          , pitch + wwidth
                                          , yMin + (vMargin-1) * ppitch
                                          , yMax
                                          )
                NetExternalComponents.setExternal( vertical )
            macroCell.setAbutmentBox( outerAb )
