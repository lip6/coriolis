
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/block/htree.py"                 |
# +-----------------------------------------------------------------+


import sys
import os.path
import Cfg
from   Hurricane import Breakpoint, DbU, Box, Transformation, Box, \
                        Path, Layer, Occurrence, Net, HyperNet,    \
                        RoutingPad, Horizontal, Vertical, Contact, \
                        Pin, Plug, Instance
import CRL
from   CRL             import RoutingLayerGauge
from   helpers         import trace
from   helpers.io      import ErrorMessage, WarningMessage, catch
from   helpers.overlay import UpdateSession
from   plugins         import getParameter
from   plugins.alpha   import utils
from   plugins.alpha.block.configuration import GaugeConf
from   plugins.alpha.block.spares        import Spares


# ----------------------------------------------------------------------------
# Class  :  "htree.HTree".

class HTree ( object ):
    """
    Build a H-Tree on a net occurrene.
    """

    def __init__ ( self, spares, treeNetOcc, index, flags ):
        self.spares     = spares
        self.treeNetOcc = treeNetOcc
        self.treeIndex  = index
        self.flags      = flags
        self.subNets    = []
       #if not self.treeNetOcc.getEntity().isClock():
       #    print( WarningMessage( 'HTree.__init__(): Net "{}" is not of CLOCK type.' \
       #                           .format(self.treeNet.getEntity().getName()) ))
        if treeNetOcc.getPath().isEmpty():
            self.treeNet = self.treeNetOcc.getEntity()
        else:
            botNet = self.treeNetOcc.getEntity()
            botNet.setExternal( True )
            topNetName = self.treeNetOcc.getName()
            topNet     = Net.create( self.treeNetOcc.getPath().getOwnerCell(), topNetName )
            topNet.setType     ( botNet.getType() )
            topNet.setDirection( botNet.getDirection() )
            path   = self.treeNetOcc.getPath().getHeadPath()
            self.spares.raddTransNet( topNet, path )
            botInstance = self.treeNetOcc.getPath().getTailInstance()
            botInstance.getPlug( botNet ).setNet( botInstance.getCell().getNet( topNetName ))
            self.treeNet = topNet

    def destroy ( self ):
        trace( 550, ',+', '\tHTree.destroy() "{}"\n'.format(self.treeNet.getName()) )
        with UpdateSession():
            for subNet in self.subNets + [ self.treeNet ]:
                components = []
                for comp in subNet.getComponents():
                    if isinstance(comp,RoutingPad): components.append( comp )
                    if isinstance(comp,Pin       ): components.append( comp )
                for comp in components:
                    comp.destroy()
                if subNet != self.treeNet:
                    subNet.destroy()
        trace( 550, '-' )

    def _rconnectHTree ( self, qt ):
        if qt.isLeaf(): return False
        qt.rconnectBuffer()
        driverNet = qt.bOutputPlug(0).getNet()
        driverNet.setType( self.treeNet.getType() )
        for leaf in qt.leafs:
            for plug in leaf.bInputPlugs:
                plug.setNet( driverNet )
            self._rconnectHTree( leaf )
        return True

    def _connectLeaf ( self, leaf, ckNet, forkContact, contact, x, y ):
        gaugeConf  = self.spares.conf
        bufferConf = self.spares.conf.bufferConf
        hLeafDepth = gaugeConf.horizontalDepth
        if gaugeConf.horizontalDepth > 2 and (gaugeConf.horizontalDepth > gaugeConf.verticalDepth):
            hLeafDepth = gaugeConf.horizontalDepth - 2
        gaugeConf.setStackPosition( contact, x, y )
        gaugeConf.createVertical  ( contact, forkContact, x, 0 )
        trace( 550, '\tLeaf contact:{}\n'.format( contact ))
        if len(leaf.buffers) > 1:
            tl1Contact = gaugeConf.rpAccessByPlugName( leaf.buffers[1], bufferConf.input , ckNet, GaugeConf.DeepDepth|GaugeConf.HAccess )
            tl2Contact = gaugeConf.rpAccessByPlugName( leaf.buffers[2], bufferConf.input , ckNet )
            tl2Y       = gaugeConf.getTrack( tl2Contact.getY(), hLeafDepth, 2 )
            left1X     = gaugeConf.getNearestVerticalTrack( tl1Contact.getX(), 0, 0 )
            left1Y     = gaugeConf.getStackY( contact, GaugeConf.DeepDepth )
            dxLeft     = contact.getX() - gaugeConf.getStackX( contact, GaugeConf.DeepDepth )
            gaugeConf.expandMinArea( tl2Contact )
            gaugeConf.setStackPosition( tl1Contact, left1X    , left1Y )
           #gaugeConf.createHorizontal( contact   , tl1Contact, left1Y , GaugeConf.DeepDepth|GaugeConf.SourceExtend )
            gaugeConf.createHorizontal( contact   , tl1Contact, left1Y , GaugeConf.DeepDepth, dxLeft )
            gaugeConf.createVertical  ( contact   , tl2Contact, x, 0 )

    def _rrouteHTree ( self, qt ):
        """
        Recursively build one HTree branch for all non-terminal nodes of the QuadTree.
        """
        trace( 550, ',+', '\tHTree._rrouteHTree() {}\n'.format(qt.bOutputPlug(0).getNet()) )
        trace( 550, '\tOn: {}\n'.format(qt) )
        if qt.isLeaf():
            trace( 550, '-' )
            return False
        gaugeConf  = self.spares.conf
        bufferConf = self.spares.conf.bufferConf
        ckNet      = qt.bOutputPlug(0).getNet()
        self.subNets.append( ckNet )
        hLeafDepth = gaugeConf.horizontalDepth
        if gaugeConf.horizontalDepth > 2 and (gaugeConf.horizontalDepth > gaugeConf.verticalDepth):
            hLeafDepth = gaugeConf.horizontalDepth - 2
        blContact          = None
        brContact          = None
        tlContact          = None
        trContact          = None
        leftContact        = None
        rigthContact       = None
        if qt.bl:
            blContact = gaugeConf.rpAccessByPlugName( qt.bl.buffers[0], bufferConf.input , ckNet )
        if qt.br:
            brContact = gaugeConf.rpAccessByPlugName( qt.br.buffers[0], bufferConf.input , ckNet )
        if qt.tl:
            tlContact = gaugeConf.rpAccessByPlugName( qt.tl.buffers[0], bufferConf.input , ckNet )
        if qt.tr:
            trContact = gaugeConf.rpAccessByPlugName( qt.tr.buffers[0], bufferConf.input , ckNet )
        if qt.bl or qt.tl:
            leafContact       = blContact if brContact else tlContact
            leftSourceContact = gaugeConf.rpAccessByPlugName( qt.buffers[0], bufferConf.output, ckNet , GaugeConf.HAccess|GaugeConf.OffsetBottom1 )
            leftSourceX       = gaugeConf.getNearestVerticalTrack  ( leftSourceContact.getX(), 0 )
            leftSourceY       = gaugeConf.getNearestHorizontalTrack( leftSourceContact.getY(), 0 )
            leftContact       = gaugeConf.createContact( ckNet, leafContact.getX(),  leftSourceContact.getY(), 0 )
            leftX             = gaugeConf.getNearestVerticalTrack( leftContact.getX(), 0 )
            gaugeConf.setStackPosition( leftSourceContact, leftSourceX, leftSourceY )
            leftContact .setX(       leftX )
            leftContact .setY( leftSourceY )
        if qt.br or qt.tr:
            leafContact        = brContact if brContact else trContact
            rightSourceContact = gaugeConf.rpAccessByPlugName( qt.buffers[0], bufferConf.output, ckNet , GaugeConf.HAccess|GaugeConf.OffsetBottom1 )
            rightSourceX       = gaugeConf.getNearestVerticalTrack( rightSourceContact.getX(), 0 )
            rightSourceY       = gaugeConf.getNearestHorizontalTrack( rightSourceContact.getY(), 0 )
            rightContact       = gaugeConf.createContact( ckNet, leafContact.getX(), rightSourceContact.getY(), 0 )
            rightX             = gaugeConf.getNearestVerticalTrack( rightContact.getX(), 0 )
            gaugeConf.setStackPosition( rightSourceContact, rightSourceX, rightSourceY )
            rightContact.setX(       rightX )
            rightContact.setY( rightSourceY )
        if qt.bl or qt.tl:
            gaugeConf.createHorizontal( leftContact, leftSourceContact, leftSourceY , 0 )
        if qt.br or qt.tr:
            gaugeConf.createHorizontal( rightSourceContact, rightContact, rightSourceY, 0 )
        if tlContact:
            tlY = gaugeConf.getTrack( tlContact.getY(), hLeafDepth, 0 )
        elif trContact:
            tlY = gaugeConf.getTrack( trContact.getY(), hLeafDepth, 0 )
        if blContact:
            blY = gaugeConf.getTrack( blContact.getY(), hLeafDepth, 0 )
            trace( 550, '\tblY:{}\n'.format( DbU.getValueString(blY) ))
        elif brContact:
            blY = gaugeConf.getTrack( brContact.getY(), hLeafDepth, 0 )
            trace( 550, '\tblY:{}\n'.format( DbU.getValueString(blY) ))
        if qt.tl:
            self._connectLeaf( qt.tl, ckNet, leftContact, tlContact, leftX, tlY )
        if qt.bl:
            trace( 550, '\tConnect BL leaf, leftX={} blY={}\n'.format( DbU.getValueString(leftX), DbU.getValueString(blY) ))
            self._connectLeaf( qt.bl, ckNet, leftContact, blContact, leftX, blY )
        if qt.tr:
            self._connectLeaf( qt.tr, ckNet, rightContact, trContact, rightX, tlY )
        if qt.br:
            self._connectLeaf( qt.br, ckNet, rightContact, brContact, rightX, blY )
        if qt.isRoot():
            ckNet = self.treeNet
            if not self.spares.conf.isCoreBlock:
                trace( 550, '\tRemoving any previous pin...\n' )
                pins = []
                for pin in ckNet.getPins(): pins.append( pin )
                for pin in pins:
                    print( WarningMessage('HTree._rrouteHTree(): Removing {}.'.format(pin)) )
                    pin.destroy()
                layerGauge  = gaugeConf.vRoutingGauge
                rootContact = gaugeConf.rpAccessByPlugName( qt.buffers[0], bufferConf.input, ckNet, 0 )
                x           = gaugeConf.getNearestVerticalTrack  ( rootContact.getX(), 0, -2 )
                y           = gaugeConf.getNearestHorizontalTrack( rootContact.getY(), 0, -2 )
                rootPin     = Pin.create( ckNet
                                        , ckNet.getName()+'.0'
                                        , Pin.Direction.NORTH
                                        , Pin.PlacementStatus.FIXED
                                        , layerGauge.getLayer()
                                        , x
                                        , qt.area.getYMax() 
                                        , layerGauge.getViaWidth()
                                        , layerGauge.getViaWidth()
                                        )
                gaugeConf.setStackPosition( rootContact, x, y )
                gaugeConf.createVertical( rootContact, rootPin, x, 0 )
        for leaf in qt.leafs:
            self._rrouteHTree( leaf )
        trace( 550, '-' )
        return True
        
    def buildHTree ( self ):
        """
        Create the tree tree netlist in two steps:
        1. Connect the buffers of the spares QuadTree to form a H-Tree.
        2. Detach the all the tree sink point and reconnect them to the
           buffers of the leafs of the QuadTree.
        """
        qt           = self.spares.quadTree
        qt.bufferTag = self.treeNet.getName()
        qt.runselect()
        qt.rselectBuffer( self.treeIndex
                        , self.treeIndex
                        , Spares.CHECK_USED|Spares.MARK_USED|self.flags)
        with UpdateSession():
            self._rconnectHTree( qt )
            self._rrouteHTree  ( qt )

    def splitNet ( self ):
        """
        Disconnect the sinks from the main tree and reconnect them to
        the leaf buffers of the tree tree.
        """
        bufferConf         = self.spares.conf.bufferConf
        quadTree           = self.spares.quadTree
        quadTree.bufferTag = self.treeNet.getName()
        quadTree.runselect()
        quadTree.rselectBuffer( self.treeIndex, self.treeIndex, self.flags)
        with UpdateSession():
            driverPlugs = []
            hyperNet    = HyperNet.create( Occurrence(self.treeNet) )
            for plugOcc in hyperNet.getTerminalNetlistPlugOccurrences():
                trace( 550, '\tReattach "{}"\n'.format(plugOcc) )
                plug = plugOcc.getEntity()
                if not (plug.getMasterNet().getDirection() & Net.Direction.OUT) \
                   and quadTree.isUnderArea(plugOcc):
                    quadTree.attachToLeaf( plugOcc )
                else:
                    driverPlugs.append( plugOcc )
            quadTree.rsplitNetlist()
           #if self.spares.conf.isCoreBlock:
            plug = utils.getPlugByName( quadTree.buffers[0], bufferConf.input )
            plug.setNet( self.treeNet )
            trace( 550, '\tCore mode, setting only root plug "{}"\n'.format(self.treeNet.getName()) )
            trace( 550, '\tPlug of "{}" (Cell:{})\n'.format(self.treeNet.getName()
                                                           ,self.treeNet.getCell()) )
            for plug in self.treeNet.getPlugs():
                trace( 550, '\t| {}\n'.format(plug) )
