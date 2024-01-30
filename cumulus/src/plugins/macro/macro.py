
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2021-2021, All Rights Reserved
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


import sys
import os.path
from   operator           import itemgetter, attrgetter, methodcaller
from   ...                import Cfg
from   ...Hurricane       import Breakpoint, DbU, Box, Transformation, Point, \
                                 Box, Path, Layer, Occurrence, Net,           \
                                 NetExternalComponents, RoutingPad, Pad,      \
                                 Horizontal, Vertical, Contact, Pin, Plug,    \
                                 Cell, Instance, Rectilinear
from   ...CRL             import AllianceFramework, RoutingLayerGauge
from   ...helpers         import trace, dots, l, u, n
from   ...helpers.io      import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay import UpdateSession
from   ..block.bigvia     import BigVia


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
        if macroCell in Macro.LUT: return Macro.LUT[ macroCell ]
        return None

    @staticmethod
    def place ( topCell, instance, transf, status ):
        macro   = Macro.lookup( instance.getMasterCell() )
        instance.setMasterCell(macro.wrapper)
        ab      = instance.getMasterCell().getAbutmentBox()
        if transf.getOrientation() == Transformation.Orientation.ID:
            abShift = Transformation( -ab.getXMin(), -ab.getYMin(), Transformation.Orientation.ID )
        if transf.getOrientation() == Transformation.Orientation.MX:
            abShift = Transformation( ab.getXMin(), -ab.getYMin(), Transformation.Orientation.ID )
        if transf.getOrientation() == Transformation.Orientation.MY:
            abShift = Transformation( -ab.getXMin(), ab.getYMin(), Transformation.Orientation.ID )
        print( 'transf={}'.format(transf) )
        print( 'abShift={}'.format(abShift) )
        abShift.applyOn( transf )
        print( 'transf={}'.format(transf) )
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

    def getLayerDepth ( self, metal ):
        return self.rg.getLayerDepth( metal )

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
        trace( 550, '+,', '\tMacro.__init__() {}\n'.format(macroCell) )
        af = AllianceFramework.get()
        self.cell = macroCell
        self.wrapper = af.createCell(f"{self.cell.getName()}_wrapper_")
        inst = Instance.create(self.wrapper, f"inst", self.cell)
        inst.setTransformation( Transformation() )
        inst.setPlacementStatus( Instance.PlacementStatus.FIXED )
        self.wrapper.setTerminalNetlist( True )
        Macro.LUT[ self.cell ] = self

        ab = self.cell.getAbutmentBox()
        self.rg = af.getRoutingGauge( gaugeName )
        gaugeMetal2      = self.rg.getLayerGauge( 1 )
        gaugeMetal3      = self.rg.getLayerGauge( 2 )
        gaugeMetal4      = self.rg.getLayerGauge( 3 )
        gaugeMetal5      = self.rg.getLayerGauge( 4 )
        blockageMetal2   = gaugeMetal2.getBlockageLayer()
        blockageMetal3   = gaugeMetal3.getBlockageLayer()
        blockageMetal4   = gaugeMetal4.getBlockageLayer()
        minSpacingMetal2 = gaugeMetal2.getLayer().getMinimalSpacing()
        minSpacingMetal3 = gaugeMetal3.getLayer().getMinimalSpacing()
        minSpacingMetal4 = gaugeMetal4.getLayer().getMinimalSpacing()
        pitchMetal2      = gaugeMetal2.getPitch()
        pitchMetal3      = gaugeMetal3.getPitch()
        pitchMetal4      = gaugeMetal4.getPitch()
        pitchMetal5      = gaugeMetal5.getPitch()
        useJumper  = False
        xMinAdjust = 0
        yMinAdjust = 0
        xMaxAdjust = 0
        yMaxAdjust = 0
        for inst_net in self.cell.getNets():
            wrapper_net = None
            for component in inst_net.getComponents():
                if not (component.getLayer().isBlockage() or NetExternalComponents.isExternal(component)):
                    continue
                if wrapper_net is None:
                    wrapper_net = Net.create(self.wrapper, inst_net.getName())
                    wrapper_net.setType(inst_net.getType())
                    wrapper_net.setDirection(inst_net.getDirection())
                    if inst_net.isExternal():
                        wrapper_net.setExternal(True)
                        wrapper_net.setGlobal(inst_net.isGlobal())
                        # Connect a logical plug through the wrapper
                        plug = inst.getPlug(inst_net)
                        plug.setNet(wrapper_net)
                if isinstance(component, Horizontal):
                    inst_comp = Horizontal.create(wrapper_net, component.getLayer(),
                        component.getY(), component.getWidth(), component.getSourceX(), component.getTargetX())
                elif isinstance(component, Vertical):
                    inst_comp = Vertical.create(wrapper_net, component.getLayer(),
                        component.getX(), component.getWidth(), component.getSourceY(), component.getTargetY())
                elif isinstance(component, Pad):
                    inst_comp = Pad.create(wrapper_net, component.getLayer(), component.getBoundingBox())
                elif isinstance(component, Rectilinear):
                    inst_comp = Rectilinear.create(wrapper_net, component.getLayer(), component.getPoints())
                else:
                    assert False, f"unable to copy net component {component}"
                if inst_net.isExternal():
                    NetExternalComponents.setExternal(inst_comp)
        self.wrapper.setRouted(True)

        if self.cell.getName().lower() == 'spblock_512w64b8w':
            print( '  o  Ad-hoc patch for "{}".'.format(self.cell.getName()) )
            useJumper  = True
            xMinAdjust = self.rg.getPitch( gaugeMetal5.getLayer() )
            pitch      = gaugeMetal2.getPitch()
            if xMinAdjust % pitch:
                xMinAdjust += pitch - (xMinAdjust % pitch)
            for net in self.wrapper.getNets():
                for component in net.getComponents():
                    if isinstance(component,Rectilinear) and component.getLayer() == blockageMetal2:
                        bb = Box( component.getBoundingBox() )
                        bb.inflate( 2*pitchMetal5 + xMinAdjust
                                  , pitchMetal2 + u(0.19)
                                  , pitchMetal2
                                  , pitchMetal2 )
                        Horizontal.create( component.getNet()
                                         , blockageMetal2
                                         , bb.getYCenter()
                                         , bb.getHeight()
                                         , bb.getXMin()
                                         , bb.getXMax() )
                        Horizontal.create( component.getNet()
                                         , blockageMetal4
                                         , bb.getYCenter()
                                         , bb.getHeight()
                                         , bb.getXMin()
                                         , bb.getXMax() )
                    elif isinstance(component,Rectilinear) and component.getLayer() == blockageMetal3:
                        bb = Box( component.getBoundingBox() )
                        deltaAbXMin = bb.getXMin() + minSpacingMetal3//2 - ab.getXMin()
                        bb.inflate( pitchMetal3 + deltaAbXMin
                                  , minSpacingMetal3//2
                                  , pitchMetal3
                                  , pitchMetal3
                                  )
                        Vertical.create( component.getNet()
                                       , blockageMetal3
                                       , bb.getXCenter()
                                       , bb.getWidth()
                                       , bb.getYMin()
                                       , bb.getYMax() )
        if self.cell.getName().lower().startswith('gf180mcu_fd_ip_sram'):
            print( '  o  Ad-hoc patch for "{}".'.format(self.cell.getName()) )
            # Extend pins 2um downwards so that they don't conflict with power supply when we add the via
            for net in self.wrapper.getNets():
                if net.isSupply():
                    continue
                for component in net.getComponents():
                    if not NetExternalComponents.isExternal(component):
                        continue
                    bb = component.getBoundingBox()
                    if not ab.isConstrainedBy(bb) or ab.getYMin() != bb.getYMin() or component.getLayer() != gaugeMetal2.getLayer():
                        continue
                    NetExternalComponents.setInternal(component)
                    extension = Vertical.create( net
                                       , gaugeMetal2.getLayer()
                                       , bb.getXCenter()
                                       , bb.getWidth()
                                       , u(-2)
                                       , bb.getYMin())
                    NetExternalComponents.setExternal(extension)
        if self.cell.getName().lower() in [ 'pll', 'gds_pll', 'cmpt_pll' ]:
            print( '  o  Ad-hoc patch for "{}".'.format(self.cell.getName()) )
        self.innerAb = ab
        sliceHeight = af.getCellGauge( gaugeName ).getSliceHeight()
        westPins  = []
        eastPins  = []
        northPins = []
        southPins = []
        for net in self.wrapper.getNets():
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
        if ab.getWidth () % sliceHeight: xMaxAdjust = sliceHeight - (ab.getWidth ()+xMinAdjust) % sliceHeight
        if ab.getHeight() % sliceHeight: yMaxAdjust = sliceHeight - (ab.getHeight()+yMinAdjust) % sliceHeight
        self.innerAb.inflate( xMinAdjust, 0, xMaxAdjust, yMaxAdjust )
        self.outerAb = Box( self.innerAb )
        self.outerAb.inflate( sliceHeight )
        westPins .sort( key=lambda k: k.getBoundingBox().getYCenter() )
        eastPins .sort( key=lambda k: k.getBoundingBox().getYCenter() )
        northPins.sort( key=lambda k: k.getBoundingBox().getXCenter() )
        southPins.sort( key=lambda k: k.getBoundingBox().getXCenter() )
                            
        with UpdateSession():
            for component in westPins:
                trace( 550, '\twestPin {}\n'.format( component ))
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
                        ppYAxis   -= width//2
                        ppYOngrid += wwidth//2
                    else:
                        ppYAxis   += width//2
                        ppYOngrid -= wwidth//2
                if useJumper:
                    jpitch  = self.rg.getPitch    ( gaugeMetal5.getLayer() )
                    jwwidth = self.rg.getWireWidth( gaugeMetal5.getLayer() )
                    xMin   -= 4*jpitch
                    bvia1 = BigVia( component.getNet()
                                  , self.getLayerDepth(component.getLayer())
                                  , xMax
                                  , yOngrid
                                  , wwidth
                                  , 3*wwidth
                                  , flags=BigVia.AllowAllExpand )
                    bvia1.mergeDepth( gaugeMetal5.getDepth() )
                    bvia1.doLayout()
                    bvia2 = BigVia( component.getNet()
                                  , self.getLayerDepth(component.getLayer())
                                  , xMax - 3*jpitch
                                  , yOngrid
                                  , wwidth
                                  , 3*wwidth
                                  , flags=BigVia.AllowAllExpand )
                    bvia2.mergeDepth( gaugeMetal5.getDepth() )
                    bvia2.doLayout()
                    Horizontal.create( bvia1.getPlate( gaugeMetal5.getLayer() )
                                     , bvia2.getPlate( gaugeMetal5.getLayer() )
                                     , gaugeMetal5.getLayer()
                                     , yOngrid
                                     , jwwidth
                                     )
                    horizontal = Horizontal.create( component.getNet()
                                                  , component.getLayer()
                                                  , yOngrid
                                                  , wwidth
                                                  , xMin
                                                  , xMax - 3*jpitch
                                                  )
                    horizontal = Horizontal.create( component.getNet()
                                                  , component.getLayer()
                                                  , yOngrid
                                                  , wwidth
                                                  , xMin
                                                  , xMin + ppitch + ppitch//2
                                                  )
                    blockageNet = self.wrapper.getNet( '*'  )
                    for gauge in [ gaugeMetal3, gaugeMetal3, gaugeMetal4, gaugeMetal5 ]:
                        bb =      bvia1.getPlate( gauge.getLayer() ).getBoundingBox()
                        bb.merge( bvia2.getPlate( gauge.getLayer() ).getBoundingBox() )
                        bb.inflate( gauge.getPitch() )
                        Pad.create( blockageNet
                                  , gauge.getLayer().getBlockageLayer()
                                  , bb )
                else:
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
                                                  , xMin + ppitch + ppitch//2
                                                  )
                NetExternalComponents.setExternal( horizontal )
            for component in eastPins:
                trace( 550, '\teastPin {}\n'.format( component ))
                layer = component.getLayer()
                if layer.getMask() != gaugeMetal2.getLayer().getMask():
                    useBigVia = True
                    layer     = gaugeMetal2.getLayer()
                NetExternalComponents.setInternal( component )
                pitch     = self.rg.getPitch( layer )
                ppitch    = self.getPPitch( layer )
                wwidth    = self.getWireWidth( layer )
                bb        = component.getBoundingBox()
                yAxis     = bb.getYCenter()
                yOngrid   = self.getNearestTrackAxis( layer, yAxis )
                xMin      = self.innerAb.getXMax() - xMaxAdjust
                xMax      = xMin + xMaxAdjust + hMargin*ppitch
                width     = bb.getHeight()
                ppYAxis   = yAxis
                ppYOngrid = yOngrid
                if not self.rg.isSymbolic():
                    if ppYAxis < ppYOngrid:
                        ppYAxis   -= width//2
                        ppYOngrid += wwidth//2
                    else:
                        ppYAxis   += width//2
                        ppYOngrid -= wwidth//2
                vertical = Vertical.create( component.getNet()
                                          , component.getLayer()
                                          , bb.getXMax()
                                          , width
                                          , ppYAxis
                                          , ppYOngrid
                                          )
                horizontal = Horizontal.create( component.getNet()
                                              , layer
                                              , yOngrid
                                              , wwidth
                                              , xMin
                                              , xMax
                                              )
                horizontal = Horizontal.create( component.getNet()
                                              , layer
                                              , yOngrid
                                              , wwidth
                                              , xMin + xMaxAdjust + ppitch
                                              , xMax
                                              )
                NetExternalComponents.setExternal( horizontal )
            for component in southPins:
                trace( 550, '\tsouthPin {}\n'.format( component ))
                NetExternalComponents.setInternal( component )
                innerRg = self.rg.getLayerGauge( component.getLayer() )
                outerRg = innerRg
                vDepth  = innerRg.getDepth()
                if innerRg.isHorizontal(): 
                   #if vDepth+1 < self.rg.getAllowedDepth(): vDepth += 1
                   #else:                                    vDepth -= 1
                    outerRg = self.rg.getLayerGauge( vDepth + 1 )
                if outerRg.getLayer().getMask() != innerRg.getLayer().getMask():
                    useBigVia = True
                pitch     = self.rg.getPitch( outerRg.getLayer() )
                ppitch    = self.getPPitch( component.getLayer() )
                wwidth    = self.getWireWidth( outerRg.getLayer() )
                bb        = component.getBoundingBox()
                xAxis     = bb.getXCenter()
                xOngrid   = self.getNearestTrackAxis( outerRg.getLayer(), xAxis )
                yMax      = bb.getYMin()
                yMin      = yMax - vMargin*ppitch
                width     = bb.getWidth()
                ppXAxis   = xAxis
                ppXOngrid = xOngrid
                if not self.rg.isSymbolic():
                    if ppXAxis < ppXOngrid:
                        ppXAxis   -= width//2
                        ppXOngrid += wwidth//2
                    else:
                        ppXAxis   += width//2
                        ppXOngrid -= wwidth//2
                if useBigVia:
                    bvia = BigVia( component.getNet()
                                 , innerRg.getDepth()
                                 , xOngrid
                                 , bb.getYMin() + wwidth
                                 , wwidth
                                 , 3*wwidth
                                 , flags=BigVia.AllowAllExpand )
                    bvia.mergeDepth( outerRg.getDepth() )
                    bvia.doLayout()
                else:
                    horizontal = Horizontal.create( component.getNet()
                                                  , component.getLayer()
                                                  , bb.getYMin()
                                                  , width
                                                  , ppXAxis
                                                  , ppXOngrid
                                                  )
                vertical = Vertical.create( component.getNet()
                                          , outerRg.getLayer()
                                          , xOngrid
                                          , wwidth
                                          , yMin
                                          , yMax
                                          )
                vertical = Vertical.create( component.getNet()
                                          , outerRg.getLayer()
                                          , xOngrid
                                          , wwidth
                                          , yMin
                                          , yMax - ppitch
                                          )
                NetExternalComponents.setExternal( vertical )
            for component in northPins:
                trace( 550, '\tnorthPin {}\n'.format( component ))
                layer = component.getLayer()
                if layer.getMask() != gaugeMetal3.getLayer().getMask():
                    useBigVia = True
                    layer     = gaugeMetal3.getLayer()
                NetExternalComponents.setInternal( component )
                pitch     = self.rg.getPitch( layer )
                ppitch    = self.getPPitch( layer )
                wwidth    = self.getWireWidth( layer )
                bb        = component.getBoundingBox()
                xAxis     = bb.getXCenter()
                xOngrid   = self.getNearestTrackAxis( layer, xAxis )
                yMin      = self.innerAb.getYMax() - yMaxAdjust
                yMax      = yMin + yMaxAdjust + vMargin*ppitch
                width     = bb.getWidth()
                ppXAxis   = xAxis
                ppXOngrid = xOngrid
                if not self.rg.isSymbolic():
                    if ppXAxis < ppXOngrid:
                        ppXAxis   -= width//2
                        ppXOngrid += wwidth//2
                    else:
                        ppXAxis   += width//2
                        ppXOngrid -= wwidth//2
                if useBigVia:
                    bvia = BigVia( component.getNet()
                                 , self.getLayerDepth(component.getLayer())
                                 , xOngrid
                                 , bb.getYMax() - wwidth
                                 , wwidth
                                 , 3*wwidth
                                 , flags=BigVia.AllowAllExpand )
                    bvia.mergeDepth( self.getLayerDepth(layer) )
                    bvia.doLayout()
                else:
                    horizontal = Horizontal.create( component.getNet()
                                                  , component.getLayer()
                                                  , bb.getYMax()
                                                  , width
                                                  , ppXAxis
                                                  , ppXOngrid
                                                  )
                vertical = Vertical.create( component.getNet()
                                          , layer
                                          , xOngrid
                                          , wwidth
                                          , yMin
                                          , yMax
                                          )
                vertical = Vertical.create( component.getNet()
                                          , layer
                                          , xOngrid
                                          , wwidth
                                          , yMin + ppitch + yMaxAdjust
                                          , yMax
                                          )
                NetExternalComponents.setExternal( vertical )
        self.wrapper.setAbutmentBox( self.outerAb )
        trace( 550, '-' )
