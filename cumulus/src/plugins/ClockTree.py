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
# |  Python      :       "./plugins/ClockTree.cpp"                  |
# +-----------------------------------------------------------------+

try:
  import sys
  import traceback
  import os.path
  import optparse
  import math
  import Cfg
  import Hurricane
  from   Hurricane import DbU
  from   Hurricane import Transformation
  from   Hurricane import Box
  from   Hurricane import Path
  from   Hurricane import Occurrence
  from   Hurricane import UpdateSession
  from   Hurricane import Breakpoint
  from   Hurricane import Net
  from   Hurricane import RoutingPad
  from   Hurricane import Contact
  from   Hurricane import Horizontal
  from   Hurricane import Vertical
  from   Hurricane import Instance
  from   Hurricane import HyperNet
  import Viewer
  import CRL
  from   CRL import RoutingLayerGauge
  from   helpers   import ErrorMessage
  import Nimbus
  import Metis
  import Mauka
  import Katabatic
  import Kite
  import Unicorn
  import plugins
except ImportError, e:
  module = str(e).split()[-1]

  print '[ERROR] The <%s> python module or symbol cannot be loaded.' % module
  print '        Please check the integrity of the <coriolis> package.'
  sys.exit(1)
except Exception, e:
  print '[ERROR] A strange exception occurred while loading the basic Coriolis/Python'
  print '        modules. Something may be wrong at Python/C API level.\n'
  print '        %s' % e
  sys.exit(2)


# Will be moved away from the students eyes in the future.
def breakpoint ( editor, level, message ):
  if editor:
    editor.fit()
    editor.refresh()
    Breakpoint.stop( level, message )
  return


def getPlugByName ( instance, netName ):
  masterCell = instance.getMasterCell()
  masterNet  = masterCell.getNet( netName )
  if masterNet:
    return instance.getPlug( masterNet )
  return None


def getPlugByNet ( instance, net ):
 #print 'getPlugByNet:', net, 'connected to', instance
  for plug in net.getPlugs():
   #print '|', plug
    if plug.getInstance() == instance:
     #print '| Found.'
      return plug
 #print '| NOT Found'
  return None


def showNet ( cell, netName ):
  net = cell.getNet(netName)
  if not net:
    print ErrorMessage( 3, 'Cell %s doesn\'t have net %s' % cell.getName(), netName ) 
    return
  
  print 'Components of', netName
  for component in net.getComponents():
    print '| ', component, component.getBoundingBox() 
  return


class HTree ( object ):

  HAccess = 0x0001

  @staticmethod
  def create ( cell, clockBox ):
    if clockBox.isEmpty(): raise ErrorMessage( 3, 'ClockTree: The clock area is empty.' )

    aspectRatio = DbU.toLambda( clockBox.getWidth() ) / DbU.toLambda( clockBox.getHeight() )
    if aspectRatio > 1.5 or aspectRatio < 0.5:
      raise ErrorMessage( 3, 'ClockTree: aspect ratio %f is disproportionate, must be between 0.5 and 1.5.' \
                             % aspectRatio )

    ht = HTree( cell, clockBox )
    print '  o  Creating Clock H-Tree for <%s>.' % cell.getName()
    ht.build()
    ht.place()
    ht.route()
    print '     - H-Tree depth: %d' % ht.getTreeDepth()
    return ht

  def __init__ ( self, cell, area ):
    self.minSide = DbU.fromLambda( Cfg.getParamInt('clockTree.minimumSide').asInt() )
    if self.minSide < DbU.fromLambda(100.0):
      raise ErrorMessage( 3, 'ClockTree: clockTree.minimumSide (%g) is less than 100 lambda.' \
                             % DbU.toLambda(self.minSide) )

    self.framework    = CRL.AllianceFramework.get()
    self.cell         = cell
    self.area         = area
    self.childs       = []
    self.bufferCell   = self.framework.getCell( 'buf_x2', CRL.Catalog.State.Logical )
    self.cellGauge    = self.framework.getCellGauge()
    self.routingGauge = self.framework.getRoutingGauge()
    self.topBuffer    = Instance.create( self.cell, 'ck_htree', self.bufferCell )
    self.cloneds      = [ self.cell ]
    self.plugToRp     = {}
    self._createChildNet( self.topBuffer, 'ck_htree' )

    topLayer = Cfg.getParamString('katabatic.topRoutingLayer').asString()
    self.topLayerDepth = 0
    for layerGauge in self.routingGauge.getLayerGauges():
      if layerGauge.getLayer().getName() == topLayer:
        self.topLayerDepth = layerGauge.getDepth()
        break
    if not self.topLayerDepth:
      print '[WARNING] Gauge top layer not defined, using top of gauge (%d).' \
             % self.routingGauge.getDepth()
      self.topLayerDepth = self.routingGauge.getDepth()

    self.horizontalDepth = 0
    self.verticalDepth   = 0
    for depth in range(0,self.topLayerDepth+1):
      if self.routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
        self.horizontalDepth = depth
      if self.routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Vertical:
        self.verticalDepth = depth

    self.masterClock  = None
    for net in cell.getNets():
      if net.isClock():
        self.masterClock = net
        break
    if not self.masterClock:
      print '[WARNING] Cell %s has no clock net.' % cell.getName()

    return

  def _createChildNet ( self, ibuffer, tag ):
    childNet = Net.create( self.cell, tag )
    childNet.setType( Net.Type.CLOCK )
    getPlugByName(ibuffer, 'q').setNet( childNet )
    return

  def _createContact ( self, net, x, y ):
    return Contact.create( net
                         , self.routingGauge.getContactLayer(self.horizontalDepth)
                         , x, y
                         , self.routingGauge.getLayerGauge(self.horizontalDepth).getViaWidth()
                         , self.routingGauge.getLayerGauge(self.horizontalDepth).getViaWidth()
                         )

  def _createHorizontal ( self, source, target, y ):
    return Horizontal.create( source
                            , target
                            , self.routingGauge.getRoutingLayer(self.horizontalDepth)
                            , y
                            , self.routingGauge.getLayerGauge(self.horizontalDepth).getWireWidth()
                            )

  def _createVertical ( self, source, target, x ):
    return Vertical.create( source
                          , target
                          , self.routingGauge.getRoutingLayer(self.verticalDepth)
                          , x
                          , self.routingGauge.getLayerGauge(self.verticalDepth).getWireWidth()
                          )

  def _rpAccess ( self, rp, net, flags=0 ):
    contact1  = Contact.create( rp, self.routingGauge.getContactLayer(0), 0, 0 )

    if flags & HTree.HAccess: stopDepth = self.horizontalDepth
    else:                     stopDepth = self.verticalDepth

    for depth in range(1,stopDepth):
      contact2 = Contact.create( net
                               , self.routingGauge.getContactLayer(depth)
                               , contact1.getX()
                               , contact1.getY()
                               , self.routingGauge.getLayerGauge(depth).getViaWidth()
                               , self.routingGauge.getLayerGauge(depth).getViaWidth()
                               )
      if self.routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
        Horizontal.create( contact1
                         , contact2
                         , self.routingGauge.getRoutingLayer(depth)
                         , contact1.getY()
                         , self.routingGauge.getLayerGauge(depth).getWireWidth()
                         )
      else:
        Vertical.create( contact1
                       , contact2
                       , self.routingGauge.getRoutingLayer(depth)
                       , contact1.getX()
                       , self.routingGauge.getLayerGauge(depth).getWireWidth()
                       )
      contact1 = contact2

    return contact1

  def _rpAccessByOccurrence ( self, occurrence, net, flags=0 ):
    plug = occurrence.getEntity()
    if self.plugToRp.has_key(plug):
      rp = self.plugToRp[plug]
    else:
      rp = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
      self.plugToRp[plug] = rp

    return self._rpAccess( rp, net, flags )

  def _rpAccessByPlug ( self, plug, net, flags=0 ):
    if self.plugToRp.has_key(plug):
      rp = self.plugToRp[plug]
    else:
      occurrence = Occurrence( plug, Path(self.cell,'') )
      rp         = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
      self.plugToRp[plug] = rp

    return self._rpAccess( rp, net, flags )

  def _rpAccessByPlugName ( self, instance, plugName, net, flags=0 ):
    return self._rpAccessByPlug( getPlugByName(instance,plugName), net, flags )

  def toXCellGrid ( self, x ): return x - (x % self.cellGauge.getSliceStep ())
  def toYCellGrid ( self, y ): return y - (y % self.cellGauge.getSliceHeight())

  def placeInstance ( self, instance, x, y ):
    xslice = self.toXCellGrid(x)
    yslice = self.toYCellGrid(y)

    transformation = Transformation.Orientation.ID
    if (yslice / self.cellGauge.getSliceHeight()) % 2 != 0:
      transformation = Transformation.Orientation.MY
      yslice        += self.cellGauge.getSliceHeight()

    instance.setTransformation ( Transformation(xslice, yslice, transformation) )
    instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
    return

  def getTreeDepth ( self ):
    return self.childs[0].getTreeDepth()

  def getLeafBufferUnder ( self, point ):
    return self.childs[0].getLeafBufferUnder( point )

  def build ( self ):
    self.childs.append( HTreeNode( self, self.topBuffer, self.area, '', HTreeNode.RootBranch ) )
    return

  def place ( self ):
    UpdateSession.open()
    center = self.area.getCenter()
    self.placeInstance( self.topBuffer, center.getX(), center.getY() )
    self.childs[0].place()
    UpdateSession.close()
    return

  def route ( self ):
    UpdateSession.open()
    self.childs[0].route()
    UpdateSession.close()
    return

  def addDeepPlug ( self, topNet, path ):
    if path.isEmpty(): return None

    tailPath     = path.getTailPath()
    headInstance = path.getHeadInstance()
    headPlug     = getPlugByNet(headInstance,topNet)
    if headPlug:
      if tailPath.isEmpty(): return headPlug
      return self.addDeepPlug( headPlug.getMasterNet(), tailPath )

    masterCell = headInstance.getMasterCell()
    masterNet  = Net.create( masterCell, topNet.getName() )
    masterNet.setExternal ( True )
    masterNet.setType     ( Net.Type.CLOCK )
    masterNet.setDirection( Net.Direction.IN )
    headPlug   = headInstance.getPlug( masterNet )
    if not headPlug:
      raise ErrorMessage( 3, 'Plug not created for %s on instance %s of %s' \
                             % (topNet.getName(),headInstance.getName(),masterCell.getName()) )
    headPlug.setNet( topNet )
    if not masterCell in self.cloneds: self.cloneds.append( masterCell )

    if tailPath.isEmpty(): return headPlug
    return self.addDeepPlug( masterNet, tailPath )

  def connectLeaf ( self ):
    UpdateSession.open()

    leafConnects     = []
    hyperMasterClock = HyperNet.create( Occurrence(self.masterClock) )
    for plugOccurrence in hyperMasterClock.getLeafPlugOccurrences():
      position   = plugOccurrence.getBoundingBox().getCenter()
      leafBuffer = self.getLeafBufferUnder( position )
      leafCk     = getPlugByName(leafBuffer,'q').getNet()
      deepPlug   = self.addDeepPlug( leafCk, plugOccurrence.getPath() )
      leafConnects.append( (deepPlug,plugOccurrence,leafCk,leafBuffer) )

    for deepPlug, plugOccurrence, leafCk, leafBuffer in leafConnects:
      plugOccurrence.getEntity().setNet( deepPlug.getMasterNet() )

      bufferContact   = self._rpAccessByPlugName( leafBuffer, 'q', leafCk , HTree.HAccess )
      registerContact = self._rpAccessByOccurrence( plugOccurrence, leafCk, 0 )
      turn            = self._createContact( leafCk, registerContact.getX(), bufferContact.getY() )
      self._createVertical  ( registerContact, turn, registerContact.getX() )
      self._createHorizontal( turn,   bufferContact, bufferContact.getY()   )

    getPlugByName( self.topBuffer, 'i' ).setNet( self.masterClock )
    UpdateSession.close()

    return

  def _rsave ( self, cell ):
    flags = CRL.Catalog.State.Physical
    if cell.getName().endswith('_clocked'):
      flags = flags | CRL.Catalog.State.Logical
    self.framework.saveCell( cell, flags )

    for instance in  cell.getInstances():
      masterCell = instance.getMasterCell()
      if not masterCell.isTerminal():
        self._rsave( masterCell )

  def save ( self ):
    for cell in self.cloneds:
      cell.setName( cell.getName()+'_clocked' )

    self._rsave( self.cell )
    return


class HTreeNode ( object ):

  RootBranch   = 0x0001
  LeftBranch   = 0x0002
  RightBranch  = 0x0004
  UpBranch     = 0x0008
  DownBranch   = 0x0010

  def __init__ ( self, topTree, sourceBuffer, area, prefix, flags ):
    self.topTree      = topTree
    self.childs       = []
    self.flags        = flags
    self.sourceBuffer = sourceBuffer
    self.area         = area
    self.prefix       = prefix

    self.blBuffer     = Instance.create( self.topTree.cell, 'ck_htree'+self.prefix+'_bl_ins', self.topTree.bufferCell )
    self.brBuffer     = Instance.create( self.topTree.cell, 'ck_htree'+self.prefix+'_br_ins', self.topTree.bufferCell )
    self.tlBuffer     = Instance.create( self.topTree.cell, 'ck_htree'+self.prefix+'_tl_ins', self.topTree.bufferCell )
    self.trBuffer     = Instance.create( self.topTree.cell, 'ck_htree'+self.prefix+'_tr_ins', self.topTree.bufferCell )
    self.ckNet        = getPlugByName(self.sourceBuffer, 'q').getNet()
    getPlugByName(self.blBuffer, 'i').setNet( self.ckNet )
    getPlugByName(self.brBuffer, 'i').setNet( self.ckNet )
    getPlugByName(self.tlBuffer, 'i').setNet( self.ckNet )
    getPlugByName(self.trBuffer, 'i').setNet( self.ckNet )

    self.topTree._createChildNet( self.blBuffer, 'ck_htree'+self.prefix+'_bl' )
    self.topTree._createChildNet( self.brBuffer, 'ck_htree'+self.prefix+'_br' )
    self.topTree._createChildNet( self.tlBuffer, 'ck_htree'+self.prefix+'_tl' )
    self.topTree._createChildNet( self.trBuffer, 'ck_htree'+self.prefix+'_tr' )

    halfWidth  = self.area.getHalfWidth ()
    halfHeight = self.area.getHalfHeight()
    if halfWidth >= self.topTree.minSide and halfHeight >= self.topTree.minSide:
     # Recursive call.
      self.childs.append( HTreeNode( self.topTree, self.blBuffer, self.blArea(), self.prefix+'_bl', 0 ) )
      self.childs.append( HTreeNode( self.topTree, self.brBuffer, self.brArea(), self.prefix+'_br', 0 ) )
      self.childs.append( HTreeNode( self.topTree, self.tlBuffer, self.tlArea(), self.prefix+'_tl', 0 ) )
      self.childs.append( HTreeNode( self.topTree, self.trBuffer, self.trArea(), self.prefix+'_tr', 0 ) )

    return

  def xmin      ( self ): return self.area.getXMin()
  def xmax      ( self ): return self.area.getXMax()
  def ymin      ( self ): return self.area.getYMin()
  def ymax      ( self ): return self.area.getYMax()
  def halfWidth ( self ): return self.area.getHalfWidth()
  def halfHeight( self ): return self.area.getHalfHeight()

  def blArea ( self ):
    return Box( self.xmin()                 , self.ymin()
              , self.xmin()+self.halfWidth(), self.ymin()+self.halfHeight() )

  def brArea ( self ):
    return Box( self.xmin()+self.halfWidth(), self.ymin()
              , self.xmax()                 , self.ymin()+self.halfHeight() )

  def tlArea ( self ):
    return Box( self.xmin()                 , self.ymin()+self.halfHeight()
              , self.xmin()+self.halfWidth(), self.ymax() )

  def trArea ( self ):
    return Box( self.xmin()+self.halfWidth(), self.ymin()+self.halfHeight()
              , self.xmax()                 , self.ymax() )

  def getTreeDepth ( self ):
    if self.childs: return self.childs[0].getTreeDepth()+1
    return 1

  def getLeafBufferUnder ( self, point ):
    if self.childs:
      if self.blArea().contains(point): return self.childs[0].getLeafBufferUnder(point)
      if self.brArea().contains(point): return self.childs[1].getLeafBufferUnder(point)
      if self.tlArea().contains(point): return self.childs[2].getLeafBufferUnder(point)
      if self.trArea().contains(point): return self.childs[3].getLeafBufferUnder(point)

    if self.blArea().contains(point): return self.blBuffer
    if self.brArea().contains(point): return self.brBuffer
    if self.tlArea().contains(point): return self.tlBuffer
    return self.trBuffer

  def place ( self ):
    x          = self.area.getXMin() + self.area.getWidth ()/4
    y          = self.area.getYMin() + self.area.getHeight()/4
    halfWidth  = self.area.getHalfWidth ()
    halfHeight = self.area.getHalfHeight()

    self.topTree.placeInstance( self.blBuffer, x          , y )
    self.topTree.placeInstance( self.brBuffer, x+halfWidth, y )
    self.topTree.placeInstance( self.tlBuffer, x          , y+halfHeight )
    self.topTree.placeInstance( self.trBuffer, x+halfWidth, y+halfHeight )

    for child in self.childs: child.place()
    return

  def route ( self ):
    leftSourceContact  = self.topTree._rpAccessByPlugName( self.sourceBuffer, 'q', self.ckNet , HTree.HAccess )
    rightSourceContact = self.topTree._rpAccessByPlugName( self.sourceBuffer, 'q', self.ckNet , HTree.HAccess )
    blContact          = self.topTree._rpAccessByPlugName( self.blBuffer    , 'i', self.ckNet )
    brContact          = self.topTree._rpAccessByPlugName( self.brBuffer    , 'i', self.ckNet )
    tlContact          = self.topTree._rpAccessByPlugName( self.tlBuffer    , 'i', self.ckNet )
    trContact          = self.topTree._rpAccessByPlugName( self.trBuffer    , 'i', self.ckNet )
    leftContact        = self.topTree._createContact( self.ckNet, blContact.getX(),  leftSourceContact.getY() )
    rightContact       = self.topTree._createContact( self.ckNet, brContact.getX(), rightSourceContact.getY() )
    self.topTree._createHorizontal( leftContact       , leftSourceContact, leftSourceContact.getY() )
    self.topTree._createHorizontal( rightSourceContact, rightContact     , rightSourceContact.getY() )
    self.topTree._createVertical  ( leftContact       , blContact        , leftContact.getX()   )
    self.topTree._createVertical  ( tlContact         , leftContact      , leftContact.getX()   )
    self.topTree._createVertical  ( rightContact      , brContact        , rightContact.getX()  )
    self.topTree._createVertical  ( trContact         , rightContact     , rightContact.getX()  )

    for child in self.childs: child.route()
    return

def computeAbutmentBox ( cell, spaceMargin, aspectRatio, cellGauge ):
  sliceHeight = DbU.toLambda( cellGauge.getSliceHeight() ) 

  instancesNb = 0
  cellLength  = 0
  for occurrence in cell.getLeafInstanceOccurrences():
    instance     = occurrence.getEntity()
    instancesNb += 1
    cellLength  += int( DbU.toLambda(instance.getMasterCell().getAbutmentBox().getWidth()) )

 # ar = x/y    S = x*y = spaceMargin*SH    x=S/y    ar = S/y^2
 # y = sqrt(S/AR)
  gcellLength = float(cellLength)*(1+spaceMargin) / sliceHeight
  rows = math.sqrt( gcellLength/aspectRatio )
  if math.trunc(rows) != rows: rows = math.trunc(rows) + 1
  else:                        rows = math.trunc(rows)
  columns = gcellLength / rows
  if math.trunc(columns) != columns: columns = math.trunc(columns) + 1
  else:                              columns = math.trunc(columns)

  print '  o  Creating abutment box (margin:%.1f%%, aspect ratio:%.1f%%, g-length:%.1fl)' \
      % (spaceMargin*100.0,aspectRatio*100.0,(cellLength/sliceHeight))
  print '     - GCell grid: [%dx%d]' % (columns,rows)

  UpdateSession.open()
  abutmentBox =  Box( DbU.fromLambda(0)
                    , DbU.fromLambda(0)
                    , DbU.fromLambda(columns*sliceHeight)
                    , DbU.fromLambda(rows   *sliceHeight)
                    )
  cell.setAbutmentBox( abutmentBox )
  UpdateSession.close()

  return abutmentBox


def unicornHook ( **kw ):
    plugins.kwUnicornHook( 'plugins.clockTree'
                         , 'ClockTree'
                         , 'Build a buffered H-Tree for the clock'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def ScriptMain ( **kw ):
  try:
    errorCode = 0

    print '  o  Cleaning up any previous run.'
    for fileName in os.listdir('.'):
      if fileName.endswith('.ap'):
        print '      - <%s>' % fileName
        os.unlink(fileName)

    cell = None
    if kw.has_key('cell') and kw['cell']:
      cell = kw['cell']

    editor = None
    if kw.has_key('editor') and kw['editor']:
      editor = kw['editor']
      print '  o  Editor detected, running in graphic mode.'
      if cell == None: cell = editor.getCell()

    if cell == None:
      raise ErrorMessage( 3, 'ClockTree: No cell loaded yet.' )

    framework = CRL.AllianceFramework.get()
    cellGauge = framework.getCellGauge()

    if cell.getAbutmentBox().isEmpty():
      spaceMargin = Cfg.getParamPercentage('nimbus.spaceMargin').asPercentage() / 100.0 + 0.02
      aspectRatio = Cfg.getParamPercentage('nimbus.aspectRatio').asPercentage() / 100.0
      computeAbutmentBox( cell, spaceMargin, aspectRatio, cellGauge )
      if editor: editor.fit()

    ht = HTree.create( cell, cell.getAbutmentBox() )
    if editor: editor.refresh()
    mauka = Mauka.MaukaEngine.create( cell )
    mauka.run()
    mauka.destroy()
    ht.connectLeaf()
    ht.save()

   #showNet( cell, 'ck_htree_bl_bl_bl' )
   #showNet( cell, 'ck_htree_bl_bl_br' )
   #showNet( cell, 'ck_htree_bl_bl_tl' )
   #showNet( cell, 'ck_htree_bl_bl_tr' )

  except ErrorMessage, e:
      print e; errorCode = e.code
  except Exception, e:
      print '\n\n', e; errorCode = 1
      traceback.print_tb(sys.exc_info()[2])
      
  return 0


if __name__ == '__main__':
  ScriptMain()

  sys.exit(0)
