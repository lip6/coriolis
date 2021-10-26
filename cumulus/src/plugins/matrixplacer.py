
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
# |  Python      :       "./plugins/matrixplacer.py"                |
# +-----------------------------------------------------------------+


import sys
import traceback
import helpers
from   helpers.io      import ErrorMessage
from   helpers.io      import WarningMessage
from   helpers.overlay import UpdateSession
from   helpers         import trace
import plugins
from   Hurricane import Breakpoint
from   Hurricane import DbU
from   Hurricane import Box
from   Hurricane import Net
from   Hurricane import Cell
from   Hurricane import Instance
from   Hurricane import Transformation
from   plugins.chip.configuration import GaugeConf


DIRECT   = 0x0001
REVERSE  = 0x0002
BACKLASH = 0x0004


def dictKeysStr ( dictionary ):
    keys = dictionary.keys()
    keys.sort()
    s = '{'
    for i in range(len(keys)):
        if i: s += ','
        s += '{}'.format(keys[i])
    s += '}'
    return s
        

# --------------------------------------------------------------------
# Class :  Signature.

class Signature ( object ):

    def __init__ ( self, instance ):
        self.instance = instance
        self.rchilds  = []
        for plug in self.instance.getConnectedPlugs():
            masterNet = plug.getMasterNet()
            if masterNet.getDirection() != Net.Direction.OUT: continue
            for outPlug in plug.getNet().getPlugs():
                outInstance = outPlug.getInstance()
                self.rchilds.append( outInstance.getMasterCell().getName() )
        self.rchilds.sort()

    def __cmp__ ( self, other ):
        if self.instance.getMasterCell().getName() < other.instance.getMasterCell().getName(): return -1
        if self.instance.getMasterCell().getName() > other.instance.getMasterCell().getName(): return +1

        if len(self.rchilds) < len(other.rchilds): return -1
        if len(self.rchilds) > len(other.rchilds): return +1
       #print( 'number of childs OK, {}' .format(len(self.rchilds)))
        for i in range(len(self.rchilds)):
            if self.rchilds[i] < other.rchilds[i]: return -1
            if self.rchilds[i] > other.rchilds[i]: return +1
       #    print( 'rchilds[{}] OK'.format(i) )
       #if self.instance.getId() < other.instance.getId(): return -1
       #if self.instance.getId() > other.instance.getId(): return +1
        return 0

    def __str__ ( self ):
        s = self.instance.getName() + ':' + str(self.rchilds)
        return s
        

# --------------------------------------------------------------------
# Class :  NetDatas.

class NetDatas ( object ):
    """
    Datas associated to a Net in a DAG.
    """

    REACHED = 0x0001
    INLOOP  = 0x0002

    def __init__ ( self, net ):
        self.net       = net
        self.flags     = 0
        self.depthSpan = [ -1, -1 ]
        self._dtags    = {}
        self._rtags    = {}
        self.plugCount = 0
        for plug in self.net.getPlugs(): self.plugCount += 1

        trace( 500, '\tNew NetDatas:{}\n'.format(self) )
        return

    @property
    def id ( self ): return self.net.getId()

    @property
    def rtags ( self ): return self._rtags

    @property
    def dtags ( self ): return self._dtags

    @property
    def krtags ( self ): return dictKeysStr(self._rtags)

    @property
    def kdtags ( self ): return dictKeysStr(self._dtags)

    @property
    def minDepth ( self ): return self.depthSpan[0]

    @property
    def maxDepth ( self ): return self.depthSpan[1]

    def updateDepth ( self, depth, direction ):
        i = 0 if direction & DIRECT else 1
        if self.depthSpan[i] < 0:
            self.depthSpan[i] = depth
        else:
            self.depthSpan[i] = min( self.depthSpan[i], depth )

    def consolidateDepth ( self, dagDepth ):
        self.depthSpan[1] = dagDepth-1 - self.depthSpan[1]

    @property
    def reached ( self ): return self.flags & NetDatas.REACHED

    @reached.setter
    def reached ( self, state ):
        if state: self.flags |=  NetDatas.REACHED
        else:     self.flags &= ~NetDatas.REACHED

    @property
    def inloop ( self ): return self.flags & NetDatas.INLOOP

    @inloop.setter
    def inloop ( self, state ):
        if state: self.flags |=  NetDatas.INLOOP
        else:     self.flags &= ~NetDatas.INLOOP

    def mergeTag ( self, tag, direction ):
        if direction & BACKLASH:
            tags = self._dtags if not(direction & DIRECT) else self._rtags
            if not len(tags):
                tags[tag] = 1
        else:
            tags = self._dtags if direction & DIRECT else self._rtags
            if tag in tags: tags[tag] += 1
            else:           tags[tag]  = 1
            return

    def mergeTags ( self, tags, direction ):
        for tag in tags.keys(): self.mergeTag( tag, direction )
        return

    def __str__ ( self ):
        s = '{} {} [{}:{}]'.format( self.net
                                  , self.plugCount
                                  , self.depthSpan[0]
                                  , self.depthSpan[1] )
        return s


# --------------------------------------------------------------------
# Class :  InstanceDatas.

class InstanceDatas ( object ):
    """
    Datas associated to an Instance in a DAG.
    """

    REACHED    = 0x0001
    REGISTER   = 0x0004
    NO_INPUTS  = 0x0008
    NO_OUTPUTS = 0x0010

    def __init__ ( self, instance ):
        self.instance   = instance
        self.flags      = 0
        self._dtags     = {}
        self._rtags     = {}
        self.depthSpan  = [ -1, -1 ]
        self._matchSets = []
        self.signature  = Signature( self.instance )

        masterCell = instance.getMasterCell()
        if masterCell.getName().startswith('sff'):
            self.flags |= InstanceDatas.REGISTER
        else:
            hasInputs  = False
            hasOutputs = False
            for plug in instance.getConnectedPlugs():
                masterNet = plug.getMasterNet()
                if masterNet.isSupply() or masterNet.isClock(): continue
                if masterNet.getDirection() & Net.Direction.DirIn:  hasInputs  = True
                if masterNet.getDirection() & Net.Direction.DirOut: hasOutputs = True
            if not hasInputs:
                self.flags |= InstanceDatas.NO_INPUTS
            if not hasOutputs:
                self.flags |= InstanceDatas.NO_OUTPUTS

        trace( 500, '\tNew InstanceDatas:{}\n'.format(self) )
        return

    @property
    def id ( self ): return self.instance.getId()

    @property
    def dtags ( self ): return self._dtags

    @property
    def rtags ( self ): return self._rtags

    @property
    def krtags ( self ): return dictKeysStr(self._rtags)

    @property
    def kdtags ( self ): return dictKeysStr(self._dtags)

    @property
    def register ( self ): return self.flags & InstanceDatas.REGISTER

    @property
    def noInputs ( self ): return self.flags & InstanceDatas.NO_INPUTS

    @property
    def noOutputs ( self ): return self.flags & InstanceDatas.NO_OUTPUTS

    @property
    def reached ( self ): return self.flags & InstanceDatas.REACHED

    @reached.setter
    def reached ( self, state ):
        if state: self.flags |=  InstanceDatas.REACHED
        else:     self.flags &= ~InstanceDatas.REACHED

    @property
    def inMatchSets ( self ): return len(self._matchSets)

    @property
    def matchSets ( self ): return self._matchSets

    def addToMatchSet ( self, matchSet ):
        self._matchSets.append( matchSet )

    def updateDepth ( self, depth, direction ):
        i = 0 if direction & DIRECT else 1
        if self.depthSpan[i] < 0:
            self.depthSpan[i] = depth
        else:
            self.depthSpan[i] = max( self.depthSpan[i], depth )

    def consolidateDepth ( self, dagDepth ):
        self.depthSpan[1] = dagDepth-1 - self.depthSpan[1]

    def mergeTag ( self, tag, direction ):
        if direction & BACKLASH:
            tags = self._dtags if not(direction & DIRECT) else self._rtags
            if not len(tags):
                tags[tag] = 1
        else:
            tags = self._dtags if direction & DIRECT else self._rtags
            if tag in tags: tags[tag] += 1
            else:           tags[tag]  = 1
            return

    def mergeTags ( self, tags, direction ):
        for tag in tags.keys(): self.mergeTag( tag, direction )
        return

    def __cmp__ ( self, other ):
        if self.depthSpan[0] < other.depthSpan[0]: return -1
        if self.depthSpan[0] > other.depthSpan[0]: return +1
        return cmp( self.signature, other.signature )

    def __str__ ( self ):
        s = '{} [{}:{}]'.format( self.instance
                               , self.depthSpan[0]
                               , self.depthSpan[1] )
        return s


# --------------------------------------------------------------------
# Class :  MatchSet.

class MatchSet ( object ):

    def __init__ ( self, row, col, tags ):
        self.row            = row
        self.col            = col
        self.tags           = tags
        self.instancesDatas = []
        self._length        = 0

    @property
    def length ( self ):
        if not self.instancesDatas or (self._length > 0): return self._length
        for instanceDatas in self.instancesDatas:
            if instanceDatas.inMatchSets > 1: continue
            self._length += instanceDatas.instance.getMasterCell().getAbutmentBox().getWidth()
        return self._length

    def match ( self, cellDag ):
        for instanceDatas in cellDag.directOrdereds:
            matched = True
            for tag in self.tags:
                if     not tag in instanceDatas.dtags \
                   and not tag in instanceDatas.rtags:
                    matched = False
                    break
            if matched:
                self.instancesDatas.append( instanceDatas )
                instanceDatas.addToMatchSet( self )
        self.instancesDatas.sort()

    def isIsomorph ( self, other ):
        count = len( self.instancesDatas )
        trace( 500, '{} vs. {}'.format(count,len(other.instancesDatas)) )
        if count != len(other.instancesDatas): return False

        iself  = 0
        iother = 0
        while iself < len(other.instancesDatas) and iother < len(other.instancesDatas): 
            skip = False
            if self.instancesDatas[iself].inMatchSets > 1:
                iself += 1
                skip   = True
            if other.instancesDatas[iother].inMatchSets > 1:
                iother += 1
                skip    = True
            if skip: continue
            if self.instancesDatas[iself].signature != other.instancesDatas[iother].signature:
                trace( 500, '  [{}]:{}'.format(iself,self.instancesDatas[iself]) )
                trace( 500, '  [{}]:{}'.format(iother,other.instancesDatas[iother]) )
                return False
            iself  += 1
            iother += 1
        return True

    def show ( self ):
        print( 'Match set for tags: {}'.format(self.tags) )
        print( '  {} instances matcheds.'.format(len(self.instancesDatas)) )
        for i in range(len(self.instancesDatas)):
            print( '  | {:02}: {} {}+{}'.format( i
                                               , self.instancesDatas[i]
                                               , self.instancesDatas[i].kdtags
                                               , self.instancesDatas[i].krtags) )
        for i in range(len(self.instancesDatas)):
            if self.instancesDatas[i].inMatchSets > 1:
                print( '[WARNING] {} affectations for {} {}+{}.'.format( self.instancesDatas[i].inMatchSets
                                                                       , self.instancesDatas[i]
                                                                       , self.instancesDatas[i].kdtags
                                                                       , self.instancesDatas[i].krtags ) )


# --------------------------------------------------------------------
# Class :  CellDag.

class CellDag ( object ):
    """
    Build a DAG from the Cell netlist.
    """

    def __init__ ( self, cell ):
        trace( 500, ',+', '\tCellDag.__init__() on {}\n'.format(cell) )
        self.cell             = cell
        self.instancesDatas   = {}
        self.netsDatas        = {}
        self.orderedNets      = []
        self.orderedInstances = []
        self.directOrdereds   = []
        self.reverseOrdereds  = []
        self.depth            = 0
        self.dagDepth         = 0
        self.direction        = DIRECT
        self.matchSets        = []

        for instance in self.cell.getInstances():
            instanceDatas = InstanceDatas( instance )
            self.instancesDatas[ instanceDatas.id ] = instanceDatas

        for net in self.cell.getNets():
            netDatas = NetDatas( net )
            self.netsDatas[ netDatas.id ] = netDatas

            if not net.isExternal() or not net.getDirection() & Net.Direction.DirIn:
                continue
        trace( 500, '-' )
        return

    def consolidateDepth ( self ):
        for datas in self.netsDatas.values():
            datas.consolidateDepth( self.dagDepth )
        for datas in self.instancesDatas.values():
            datas.consolidateDepth( self.dagDepth )
        return

    def initDirect ( self ):
        trace( 500, ',+', '\tCellDag.initDirect() on {}\n'.format(self.cell) )
        self.depth            = 0
        self.direction        = DIRECT
        self.orderedNets      = []
        self.orderedInstances = []

        for instance in self.cell.getInstances():
            instanceDatas = self.lookup( instance )
            instanceDatas.reached = False

        for net in self.cell.getNets():
            netDatas = self.lookup( net )
            if net.getDirection() & Net.Direction.DirIn:
                netDatas.reached = True
                netDatas.updateDepth( 0, self.direction )
                self.addOrdered( netDatas, 0 )
                trace( 500, '\tDAG input:{}\n'.format(netDatas) )
            else:
                netDatas.reached = False

        self.findStartInstances()
        trace( 500, '-' )
        return

    def initReverse ( self ):
        trace( 500, ',+', '\tCellDag.initReverse() on {}\n'.format(self.cell) )
        self.depth            = 0
        self.direction        = REVERSE
        self.orderedNets      = []
        self.orderedInstances = []

        for instance in self.cell.getInstances():
            instanceDatas = self.lookup( instance )
            instanceDatas.reached = False

        for net in self.cell.getNets():
            netDatas = self.lookup( net )
            if net.getDirection() & Net.Direction.DirOut:
                netDatas.reached = True
                netDatas.updateDepth( 0, self.direction )
                self.addOrdered( netDatas, 0 )
                trace( 500, '\tR-DAG input:{}\n'.format(netDatas) )
            else:
                netDatas.reached = False

        self.findStartInstances()
        trace( 500, '-' )
        return

    def isInPropagateDirection ( self, net, direction=None ):
        if direction is None: direction = self.direction
        return not (  (direction & DIRECT  and net.getDirection() != Net.Direction.OUT) \
                   or (direction & REVERSE and net.getDirection() != Net.Direction.IN ))

    def lookup ( self, element ):
        if isinstance(element,Net):
            if element.getId() in self.netsDatas:
                return self.netsDatas[element.getId()]
            raise ErrorMessage( 1, 'CellDag.lookup(): Missing NetDatas for {}.'.format(element) )
        if isinstance(element,Instance):
            if element.getId() in self.instancesDatas:
                return self.instancesDatas[element.getId()]
            raise ErrorMessage( 1, 'CellDag.lookup(): Missing InstanceDatas for {}.'.format(element) )
        raise ErrorMessage( 1, 'CellDag.lookup(): {} has not Datas support.'.format(element) )

    def isOrdered ( self, element ):
        return self.lookup(element).reached

    def addOrdered ( self, datas, depth ):
        if isinstance(datas,NetDatas):
            datas.reached = True
            datas.updateDepth( depth, self.direction )
            while len(self.orderedNets) < depth+1:
                self.orderedNets.append( {} )
            self.orderedNets[depth][datas.id] = datas
        if isinstance(datas,InstanceDatas):
            datas.reached = True
            datas.updateDepth( depth, self.direction )
            while len(self.orderedInstances) < depth+1:
                self.orderedInstances.append( {} )
            self.orderedInstances[depth][datas.id] = datas

            if self.direction & DIRECT:
                self.directOrdereds.append( datas )
            else:
                self.reverseOrdereds.append( datas )
        return

    def addInstanceOutputs ( self, instance, depth, direction=None ):
        for plug in instance.getConnectedPlugs():
            masterNet = plug.getMasterNet()
            if not self.isInPropagateDirection(masterNet,direction): continue
            netDatas = self.lookup( plug.getNet() )
            self.addOrdered( netDatas, depth )
        return

    def findStartInstances ( self ):
        for instance in self.cell.getInstances():
            datas = self.lookup( instance )
            if datas.register:
                trace( 500, '\tREGISTER:{}\n'.format(instance) )
                self.addInstanceOutputs( instance, 0, DIRECT )
            if self.direction & DIRECT and datas.noInputs:
                trace( 500, '\tNO-INPUTS:{}\n'.format(instance) )
                self.addInstanceOutputs( instance, 0 )
            if self.direction & REVERSE and datas.noOutputs:
                trace( 500, '\tNO-R-INPUTS:{}\n'.format(instance) )
                self.addInstanceOutputs( instance, 0 )
        return

    def processNetDatas ( self, netDatas ):
        trace( 500, ',+', '\tCellDag.processNetDatas() {}\n'.format(netDatas) )
        for plug in netDatas.net.getPlugs():
            instance = plug.getInstance()
            if self.isOrdered(instance): continue 
            self.processInstance( instance )
        trace( 500, '-' )
        return

    def processInstance ( self, instance ):
        trace( 500, ',+', '\tCellDag.processInstance() {}\n'.format(instance) )
        datas = self.lookup( instance )
        if datas.reached:
            trace( 500, '-' )
            return

        reached = True
        for plug in instance.getConnectedPlugs():
            masterNet = plug.getMasterNet()
            if self.isInPropagateDirection(masterNet): continue
            net = plug.getNet()
            if not self.isOrdered(net):
                trace( 500, '\tunordered:{}:{}\n'.format(net,masterNet) )
                reached = False            
                break
            else:
                trace( 500, '\tordered:{}:{}\n'.format(net,masterNet) )

        if reached:
            trace( 500, '\tORDERED:{}\n'.format(instance) )
            self.addOrdered( datas, self.depth )

            self.addInstanceOutputs( instance, self.depth+1 )

            for plug in instance.getConnectedPlugs():
                masterNet = plug.getMasterNet()
                if not self.isInPropagateDirection(masterNet): continue
                netDatas = self.lookup( plug.getNet() )
                netDatas.updateDepth( self.depth+1, self.direction )

        trace( 500, '-' )

    def processNets ( self ):
        trace( 500, ',+', '\tCellDag.processNets() @depth:{}\n'.format(self.depth) )
        for depth in range(self.depth+1):
            for id in self.orderedNets[depth].keys():
                trace( 500, '\t| orderedNets[{}][{}] = {}\n'.format(depth,id,self.orderedNets[depth][id]) )
        
        trace( 500, '+' )
        for netDatas in self.orderedNets[self.depth].values():
            self.processNetDatas( netDatas )
        trace( 500, '--' )

    def build ( self ):
        self.initDirect()
        while len(self.orderedNets) > self.depth:
            trace( 500, '+' )
            self.processNets()
            self.depth += 1
        trace( 500, '-' * self.depth )
        self.dagDepth = max( self.dagDepth, self.depth )

        self.initReverse()
        while len(self.orderedNets) > self.depth:
            trace( 500, '+' )
            self.processNets()
            self.depth += 1
        trace( 500, '-' * self.depth )
        self.dagDepth = max( self.dagDepth, self.depth )

        self.consolidateDepth()
        return

    def showReverseCone ( self, reference, maxDepth ):
        instance = None
        if isinstance(reference,str):
            instance = self.cell.getInstance( reference )
        elif isinstance(reference,Instance):
            instance = reference
        elif isinstance(reference,InstanceDatas):
            instance = reference.instance
        if not instance: return

        instanceDatas = self.lookup( instance )
        if not instanceDatas:
            trace( 500, '\t+ No datas for {}\n'.format(instance) )
            return
            
        s  =  'dtags:{}'.format( instanceDatas.dtags )
        s += ' rtags:{}'.format( instanceDatas.rtags )
        trace( 500, ',+', '\t+ {} {}\n'.format(s,instance) )

        for plug in instance.getConnectedPlugs():
            masterNet = plug.getMasterNet()
            if masterNet.getDirection() == Net.Direction.IN \
               and not (masterNet.isSupply() or masterNet.isClock()):
                netDatas = self.lookup(plug.getNet())
                s  =  'dtags:{}'.format( netDatas.dtags )
                s += ' rtags:{}'.format( netDatas.rtags )
                trace( 500, '\t| {} input "{}" {}\n'.format(s,masterNet.getName(),netDatas.net) )

                for driverPlug in plug.getNet().getPlugs():
                    if driverPlug == plug: continue
                    driverMasterNet = driverPlug.getMasterNet()
                    if driverMasterNet.getDirection() == Net.Direction.OUT \
                       and not (driverMasterNet.isSupply() or driverMasterNet.isClock()):
                        driverInstance = driverPlug.getInstance()
                        driverDatas    = self.lookup( driverInstance )
                        if driverDatas.register:
                            trace( 500, '\t| register {} \n'.format(driverInstance) )
                        if maxDepth > 0:
                            self.showReverseCone( driverInstance, maxDepth-1 )
                        else:
                            trace( 500, '\t| Max depth {} \n'.format(driverInstance) )

        trace( 500, '-' )

    def showNetHistogram ( self ):
        histogram = {}
        for netDatas in self.netsDatas.values():
            print( 'netDatas:{}'.format(netDatas) )
            if netDatas.plugCount in histogram:
                histogram[netDatas.plugCount] += 1
            else:
                histogram[netDatas.plugCount] = 1
        orderedKeys = histogram.keys()
        orderedKeys.sort()
        for key in orderedKeys:
            print( '[{}] = {}'.format(key,histogram[key]) )
        return

    def getConnectedInstances ( self, netDatas ):
        instances = []
        if isinstance(datas,NetDatas):
            for plug in datas.net.getPlugs():
                instanceDatas = self.lookup( plug.getInstance() )
                instances.append( instanceDatas )
        return instances

    def show ( self ):
        instancesNb = 0
        for instance in self.cell.getInstances(): instancesNb += 1

        instancesReacheds = 0
        for datas in self.instancesDatas.values():
            if datas.reached == True:
                instancesReacheds += 1
        
        print( 'CellDag.show() {}'.format(self.cell) )
        print( 'instances reacheds: {} / {}'.format(instancesReacheds,instancesNb) )
        depth = 0
        for depthLayer in self.orderedInstances:
            print( 'thickness of depth {}: {}'.format(depth,len(depthLayer)) )
            for datas in self.orderedInstances[depth].values():
                print( '[{0:03d}] {1}'.format(depth,datas.instance) )
            depth += 1

        print( 'nets ordereds:' )
        depth = 0
        for depthLayer in self.orderedNets:
            print( 'thickness of depth {}: {}'.format(depth,len(depthLayer)) )
            for datas in self.orderedNets[depth].values():
                print( '[{0:03d}] {1}'.format(depth,datas) )
            depth += 1

        self.showNetHistogram()
        return

    def addNetTag ( self, netName, tag ):
        net = self.cell.getNet( netName )
        if not net:
            print( ErrorMessage(1, 'CellDag.addNetTag(): No net named "{}", ignored.'.format(netName)) )
            return
        if not net.isExternal():
            print( Warning('CellDag.addNetTag(): Net "{}" is not external.'.format(netName)) )
        direction = DIRECT
        if net.getDirection() & Net.Direction.DirOut: direction = REVERSE
        if net.getDirection() & Net.Direction.DirIn:  direction = DIRECT
        netDatas = self.lookup( net )
        if netDatas: netDatas.mergeTag( tag, direction )
        return

    def directPropagate ( self, count, backlash=False ):
        trace( 500, ',+', '\tCellDag.directPropagate(): count:{}, backlash:{}\n'.format(count,backlash) )
        direction = DIRECT
        if backlash: direction |= BACKLASH
        for iteration in range(count):
            for instanceDatas in self.directOrdereds:
                trace( 500, ',+', '\t+ direct:{}\n'.format(instanceDatas) )
                for plug in instanceDatas.instance.getConnectedPlugs():
                    masterNet = plug.getMasterNet()
                    if masterNet.getDirection() == Net.Direction.IN \
                       and not (masterNet.isSupply() or masterNet.isClock()):
                        netDatas = self.lookup(plug.getNet())
                        netTags  = netDatas.dtags if not backlash else netDatas.rtags
                        instanceDatas.mergeTags( netTags, direction )
                        s  =  'DTAGS:{}'.format( netTags )
                        s += ' rtags:{}'.format( netTags )
                        trace( 500, '\t| {} from {}\n'.format(s,netDatas.net) )
                for plug in instanceDatas.instance.getConnectedPlugs():
                    masterNet = plug.getMasterNet()
                    if masterNet.getDirection() == Net.Direction.OUT \
                       and not (masterNet.isSupply() or masterNet.isClock()):
                        netDatas     = self.lookup(plug.getNet())
                        instanceTags = instanceDatas.dtags if not backlash else instanceDatas.rtags
                        netDatas.mergeTags( instanceTags, direction )
                        s  =  'DTAGS:{}'.format( netTags )
                        s += ' rtags:{}'.format( netTags )
                        trace( 500, '\t> {} from {}\n'.format(s,netDatas.net) )
                trace( 500, ',-' )
        trace( 500, '-' )
        return

    def reversePropagate ( self, count, backlash=False ):
        trace( 500, ',+', '\tCellDag.reversePropagate(): count:{}, backlash:{}\n'.format(count,backlash) )
        direction = REVERSE
        if backlash: direction |= BACKLASH
        for iteration in range(count):
            for instanceDatas in self.reverseOrdereds:
                trace( 500, ',+', '\t+ reverse:{}\n'.format(instanceDatas) )
                for plug in instanceDatas.instance.getConnectedPlugs():
                    masterNet = plug.getMasterNet()
                    if masterNet.getDirection() == Net.Direction.OUT \
                       and not (masterNet.isSupply() or masterNet.isClock()):
                        netDatas = self.lookup(plug.getNet())
                        netTags  = netDatas.rtags if not backlash else netDatas.dtags
                        instanceDatas.mergeTags( netTags, direction )
                        s  =  'dtags:{}'.format( netDatas.dtags )
                        s += ' RTAGS:{}'.format( netDatas.rtags )
                        trace( 500, '\t| {} from {}\n'.format(s,netDatas.net) )
                s  =  'dtags:{}'.format( instanceDatas.dtags )
                s += ' RTAGS:{}'.format( instanceDatas.rtags )
                trace( 500, '\t> {}\n'.format(s) )

                for plug in instanceDatas.instance.getConnectedPlugs():
                    masterNet = plug.getMasterNet()
                    if masterNet.getDirection() == Net.Direction.IN \
                       and not (masterNet.isSupply() or masterNet.isClock()):
                        self.lookup(plug.getNet()).mergeTags( instanceDatas.rtags, direction )
                        netDatas     = self.lookup(plug.getNet())
                        instanceTags = instanceDatas.rtags if not backlash else instanceDatas.dtags
                        netDatas.mergeTags( instanceTags, direction )
                        s  =  'dtags:{}'.format( netDatas.dtags )
                        s += ' RTAGS:{}'.format( netDatas.rtags )
                        trace( 500, '\t| {} on {}\n'.format(s,netDatas.net) )
                trace( 500, '-' )

        trace( 500, '-' )
        return

    def createMatchSet ( self, row, col, tags ):
        self.matchSets.append( MatchSet(row,col,tags) )
        self.matchSets[-1].match( self )
        return self.matchSets[-1]

    def checkIsomorphy ( self ):
        if len(self.matchSets) == 0: return False
        reference  = self.matchSets[0]
        isomorphic = True
        for i in range(len(self.matchSets)):
            if not reference.isIsomorph(self.matchSets[i]):
                print( Warning('CellDag.chackIsomorphy(): MatchSets {} and 0 differs.'.format(i)) )
                isomorphic = False
        return isomorphic

    def showMultimatcheds ( self ):
        count = 0
        print( 'Multimatcheds instances:' )
        for datas in self.directOrdereds:
            if datas.inMatchSets < 2: continue
            print( '  | {} {}+{}'.format(datas, datas.kdtags, datas.krtags) )
            count += 1
        print( 'Total multimatcheds instances: {}'.format(count) )

    def showUnmatcheds ( self ):
        count = 0
        print( 'Unmatcheds instances:' )
        for datas in self.directOrdereds:
            if datas.inMatchSets > 0: continue
            print( '  | {} {}+{}'.format(datas, datas.kdtags, datas.krtags) )
            count += 1
        print( 'Total unmatcheds instances: {}'.format(count) )


# --------------------------------------------------------------------
# Class :  Column.

class Column ( object ):

    def __init__ ( self, size=0 ):
        self.rows        = []
        self.rowCaps     = []
        self.bottomRows  = []
        self.bottomAlign = 0
        self._colWidth   = 0
        self._capWidth   = 0
        self._valid      = False
        self.expand( size )

    def __getitem__ ( self, row ):
        return self.rows[row]

    def __setitem__ ( self, row, element ):
        self.addRow( row, element )

    def _update ( self ):
        if self._valid: return
        for element in self.rows:
            if not (element is None):
                self._colWidth = max( self._colWidth, element.length )
        self._capWidth = 0
        for elements in self.rowCaps:
            capWidth = 0
            for element in elements:
                capWidth += element.instance.getMasterCell().getAbutmentBox().getWidth()
            self._capWidth = max( self._capWidth, capWidth )
        self._valid = True

    @property
    def width ( self ):
        self._update()
        return self._colWidth + self._capWidth

    @property
    def rowCount ( self ): return len(self.rows)

    def expand ( self, size ):
        self._valid = False
        if size:
            while len(self.rows) < size:
                self.rows.append( None )
                self.rowCaps.append( [] )

    def addRow ( self, j, element=None ):
        self._valid = False
        self.expand( j+1 )
        if isinstance(element,MatchSet):
            self.rows[j] = element

    def addRowCap ( self, element ):
        self._valid = False
        if not isinstance(element,InstanceDatas): return
        tags  = element.dtags.keys() + element.rtags.keys()
        vtags = []
        jspan = [ self.rowCount, 0 ]
        for tag in tags:
            if tag[0] == 'v': vtags.append( tag )
            if tag[0] == 'h':
                j        = int(tag[1:])
                jspan[0] = min( jspan[0], j )
                jspan[1] = max( jspan[1], j )
        if len(vtags) != 1:
            print( ErrorMessage( 1, 'Column.addRowCap(): {} has not exactly *one* vertical tag ({}).'.format(element,vtags)) )
            return
        if jspan[0] > jspan[1]:
            jspan = [ 0, self.rowCount ]
        jinsert = jspan[0]
        for j in range(jspan[0],jspan[1]+1):
            if len(self.rowCaps[j]) < len(self.rowCaps[jinsert]):
                jinsert = j
        self.rowCaps[jinsert].append( element )

    def addBottom ( self, element ):
        if not isinstance(element,InstanceDatas): return
        if not len(self.bottomRows): self.bottomRows.append( [] )
        self.bottomRows[0].append( element )

    def balanceBottomRows ( self ):
        trace( 500, ',+', 'Column.balanceBottomrows()' )
        if not len(self.bottomRows):
            trace( 500, '-' )
            return

        bottomWidth = 0
        for element in self.bottomRows[0]:
            bottomWidth += element.instance.getMasterCell().getAbutmentBox().getWidth()
        if bottomWidth > self.width:
            splits             = bottomWidth / self.width + 1
            splitWidth         = bottomWidth / splits #+ bottomWidth / (splits+1)
            rowWidth           = 0
            elements           = self.bottomRows[0]
            trace( 500, '\t| column width: {}\n'.format(DbU.getValueString(self.width)) )
            trace( 500, '\t| bottom width: {}\n'.format(DbU.getValueString(bottomWidth)) )
            trace( 500, '\t| splits:       {}\n'.format(splits) )
            trace( 500, '\t| split width:  {}\n'.format(DbU.getValueString(splitWidth)) )
            self.bottomRows[0] = []
            for element in elements:
                abWidth = element.instance.getMasterCell().getAbutmentBox().getWidth()
                if rowWidth+abWidth > splitWidth:
                    self.bottomRows.append( [] )
                    rowWidth = 0
                self.bottomRows[-1].append( element )
                rowWidth += abWidth
        trace( 500, '-' )

    def place ( self, xorigin, sliceHeight ):
        trace( 500, ',+', '\tColumn.place() @{}\n'.format(DbU.getValueString(xorigin)) )
        self._update()
        regRows = len( self.rows )
        botRows = self.bottomAlign
        allRows = regRows + botRows
        for j in range(regRows):
            trace( 500, ',+', '\tplacing row {} (colW:{}, capW:{})\n' \
                   .format( j
                          , DbU.getValueString(self._colWidth)
                          , DbU.getValueString(self._capWidth)) )
            if (j+botRows)%2:
                y      = (allRows - j) * sliceHeight
                orient = Transformation.Orientation.MY
            else:
                y      = (allRows - j - 1) * sliceHeight
                orient = Transformation.Orientation.ID
            x = xorigin
            trace( 500, '\t+ Regular elements @{}\n'.format(DbU.getValueString(x)) )
            for element in self.rows[j].instancesDatas:
                if element.inMatchSets > 1: continue
                element.instance.setTransformation ( Transformation(x,y,orient) )
                element.instance.setPlacementStatus( Instance.PlacementStatus.PLACED )
                abWidth = element.instance.getMasterCell().getAbutmentBox().getWidth()
                trace( 500, '\t| placed @({},{}) {}\n'.format( DbU.getValueString(x)
                                                             , DbU.getValueString(y)
                                                             , element.instance) )
                x += abWidth

            x = xorigin + self._colWidth
            trace( 500, '\t+ Endcap elements @{}\n'.format(DbU.getValueString(x)) )
            for element in self.rowCaps[j]:
                element.instance.setTransformation ( Transformation(x,y,orient) )
                element.instance.setPlacementStatus( Instance.PlacementStatus.PLACED )
                abWidth = element.instance.getMasterCell().getAbutmentBox().getWidth()
                trace( 500, '\t| placed @({},{}) {}\n'.format( DbU.getValueString(x)
                                                             , DbU.getValueString(y)
                                                             , element.instance) )
                x += abWidth
            trace( 500, '-' )

        for j in range(len(self.bottomRows)):
            trace( 500, ',+', '\tplacing bottom row {} (colW:{}, capW:{})\n' \
                   .format( j
                          , DbU.getValueString(self._colWidth)
                          , DbU.getValueString(self._capWidth)) )
            if j%2:
                y      = (allRows - j - regRows) * sliceHeight
                orient = Transformation.Orientation.MY
            else:
                y      = (allRows - j - regRows - 1) * sliceHeight
                orient = Transformation.Orientation.ID
            x = xorigin
            trace( 500, '\t+ Bottom elements @{}\n'.format(DbU.getValueString(x)) )
            for element in self.bottomRows[j]:
                element.instance.setTransformation ( Transformation(x,y,orient) )
                element.instance.setPlacementStatus( Instance.PlacementStatus.PLACED )
                abWidth = element.instance.getMasterCell().getAbutmentBox().getWidth()
                trace( 500, '\t| placed @({},{}) {}\n'.format( DbU.getValueString(x)
                                                             , DbU.getValueString(y)
                                                             , element.instance) )
                x += abWidth
            x = xorigin + self._colWidth
            trace( 500, '-' )
        trace( 500, '-' )


# --------------------------------------------------------------------
# Class :  MatrixPlacer.

class MatrixPlacer ( object ):

    def __init__ ( self, cell ):
        self.gaugeConf  = GaugeConf()
        self.cell       = cell
        self.columns    = []
        self.dag        = CellDag( cell )
        self.tagRows    = {}
        self.tagColumns = {}
        self.dag.build()

    def tagsToMatrix ( self, propagateSteps=2 ):
        self.dag.directPropagate ( propagateSteps )
        self.dag.reversePropagate( 1, True )
        self.dag.reversePropagate( propagateSteps )

        matchSets = []
        for h in self.tagRows.keys():
            for v in self.tagColumns.keys():
                self.dag.createMatchSet( h, v, ['h{}'.format(h), 'v{}'.format(v)] )
                self.addElement( v, h, self.dag.matchSets[-1] )

        for instanceDatas in self.dag.instancesDatas.values():
            if instanceDatas.inMatchSets < 2: continue
            tags  = instanceDatas.dtags.keys() + instanceDatas.rtags.keys()
            vtags = []
            for tag in tags:
                if tag[0] == 'v': vtags.append( tag )
            if len(vtags) != 1: continue
            v = int( vtags[0][1:] )
            self.addCapElement( v, instanceDatas )

        for instanceDatas in self.dag.instancesDatas.values():
            if instanceDatas.inMatchSets: continue
            tags  = instanceDatas.dtags.keys() + instanceDatas.rtags.keys()
            print( 'Unmatched {} {}'.format(instanceDatas,tags) )
            vtags = []
            for tag in tags:
                if tag[0] == 'v': vtags.append( tag )
            if len(vtags) != 1: continue
            v = int( vtags[0][1:] )
            self.addBottomElement( v, instanceDatas )

        self.finalizeBottom()

    def tagRow ( self, row, nets ):
        if not row in self.tagRows:
            self.tagRows[row] = []
        self.tagRows[row] += nets
        for net in nets:
            self.dag.addNetTag( net, 'h{}'.format(row) )

    def tagColumn ( self, column, nets ):
        if not column in self.tagColumns:
            self.tagColumns[column] = []
        self.tagColumns[column] += nets
        for net in nets:
            self.dag.addNetTag( net, 'v{}'.format(column) )

    def expand ( self, columnSize, rowSize ):
        for column in self.columns:
            column.expand( rowSize )
        while len(self.columns) < columnSize:
            self.columns.append( Column(rowSize) )

    def addElement ( self, i, j, matchSet ):
        self.expand( i+1, j+1 )
        if not (self.columns[i][j] is None):
            print( ErrorMessage(1, 'MatrixPlacer.addElement(): Redefinition of matrix element [{},{}]'.format(i,j)) )
        else:
            self.columns[i][j] = matchSet

    def addCapElement ( self, i, instanceDatas ):
        self.columns[i].addRowCap( instanceDatas )

    def addBottomElement ( self, i, instanceDatas ):
        self.columns[i].addBottom( instanceDatas )

    def finalizeBottom ( self ):
        for i in range(len(self.columns)):
            self.columns[i].balanceBottomRows()

        bottomAlign = 0
        for i in range(len(self.columns)):
            bottomAlign = max( bottomAlign, len(self.columns[i].bottomRows) )
        for i in range(len(self.columns)):
            self.columns[i].bottomAlign = bottomAlign

    def place ( self ):
        with UpdateSession():
            xcolumn     = 0
            sliceHeight = self.gaugeConf.getSliceHeight()
            for i in range(len(self.columns)):
                self.columns[i].place( xcolumn, sliceHeight )
                xcolumn += self.columns[i].width
            self.cell.setAbutmentBox( Box( 0, 0, xcolumn, (self.columns[0].rowCount+self.columns[0].bottomAlign)*sliceHeight) )

    def checkUnplaceds ( self ):
        for instanceDatas in self.dag.instancesDatas.values():
            if instanceDatas.instance.getPlacementStatus() != Instance.PlacementStatus.PLACED:
                print( ErrorMessage(1, 'MatrixPlace.checkUnplaceds(): Unplaced {}.'.format(instanceDatas)) )

    def showTags ( self ):
        print( 'MatrixPlacer on "{}"'.format(self.cell.getName()) )
        print( '  Rows tags/nets: {}'.format(len(self.tagRows)) )
        for item in  self.tagRows.items():
            print( '    | {:03} : {}'.format(item[0],item[1]) )
        print( '  Columns tags/nets: {}'.format(len(self.tagColumns)) )
        for item in  self.tagColumns.items():
            print( '    | {:03} : {}'.format(item[0],item[1]) )

    def showGrid ( self ):
        for ms in self.dag.matchSets:
            ms.show()
        self.dag.showMultimatcheds()
        self.dag.showUnmatcheds()
       #self.dag.checkIsomorphy()


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    kw['beforeAction'] = 'misc.alpha'

   #plugins.kwAddMenu    ( 'placeAndRoute', 'P&&R', **kw )
    plugins.kwUnicornHook( 'misc.alpha.matrixPlacer'
                         , 'Matrix Placer'
                         , 'Look for a Matrix-Like netlist strucure and place it'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
    rvalue = True
    try:
       #helpers.setTraceLevel( 500 )
        cell, editor = plugins.kwParseMain( **kw )

        matrix = MatrixPlacer( cell )

        fuNb   = 30
       #fuNb   = 4
        gorwNb = 3
        for fu in range(fuNb):
            matrix.tagColumn( fu, ['readable_o({})'.format(fu)] )
            matrix.tagColumn( fu, ['writable_o({})'.format(fu)] )
            for gorwi in range(gorwNb):
                matrix.tagRow( (fu*gorwNb + gorwi)*2    , ['gord{}_i({})'.format(gorwi+1,fu)] )
                matrix.tagRow( (fu*gorwNb + gorwi)*2 + 1, ['gowr{}_i({})'.format(gorwi+1,fu)] )
        matrix.showTags()

        matrix.tagsToMatrix( 2 )
        matrix.showGrid()
        matrix.place()
        matrix.checkUnplaceds()
        if editor: editor.fit()
       #Breakpoint.stop( 0, 'Showing cone' )
        matrix.dag.showReverseCone( 'subckt_5_dm13_subckt_332_src3_c_63_subckt_112_sff1_x4', 10 )
      
        return True
    except Exception as e:
        helpers.io.catch( e )
        rvalue = False

    sys.stdout.flush()
    sys.stderr.flush()
        
    return rvalue
