# -*- explicit-buffer-name: "__init__.py<cumulus/plugins>" -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/__init__.py"                    |
# +-----------------------------------------------------------------+


import Cfg
from   helpers.io import ErrorMessage
from   helpers.io import WarningMessage
from   Hurricane  import Contact
from   Hurricane  import Path
from   Hurricane  import Occurrence
from   Hurricane  import Instance
import Viewer
import CRL
from   CRL        import RoutingLayerGauge


NoFlags           = 0000
ShowWarnings      = 0001
WarningsAreErrors = 0002



def  kwParseMain ( **kw ):
    cell = None
    if kw.has_key('cell') and kw['cell']:
      cell = kw['cell']

    editor = None
    if kw.has_key('editor') and kw['editor']:
      editor = kw['editor']
      if cell == None: cell = editor.getCell()

   #if cell == None:
   #  raise ErrorMessage( 3, 'Chip: No cell loaded yet.' )

    return cell, editor


def kwAddMenu ( menuPath, menuName, **kw ):
    editor = kw['editor']
    if menuPath.find('.') >= 0: flags = Viewer.CellViewer.NoFlags
    else:                       flags = Viewer.CellViewer.TopMenu
    if not editor.hasMenu(menuPath):
        editor.addMenu( menuPath, menuName, flags )
    return


def kwUnicornHook ( menuPath, menuName, menuTip, moduleFile, **kw ):
    editor = kw['editor']
    if moduleFile.endswith('.pyc') or moduleFile.endswith('.pyo'):
      moduleFile = moduleFile[:-1]

    if kw.has_key('beforeAction'):
      editor.addToMenu( menuPath, menuName, menuTip, moduleFile, kw['beforeAction'] )
    else:  
      editor.addToMenu( menuPath, menuName, menuTip, moduleFile )
    return


def getParameter ( pluginName, key ):
    if not Cfg.hasParameter(key):
      raise ErrorMessage( 1, 'Mandatory parameter <%s> for for plugin <%s> is missing in configuration.'
                             % (key,pluginName) )
    return Cfg.Configuration.get().getParameter(key)
    

class CheckUnplaced ( object ):

  def __init__ ( self, cell, flags=NoFlags ):
    self.cell      = cell
    self.flags     = flags
    self.unplaceds = []
    return


  def _rcheckUnplaced ( self, path, cell ):
    for instance in cell.getInstances():
      if instance.getPlacementStatus() == Instance.PlacementStatus.UNPLACED:
        self.unplaceds.append( Occurrence(instance,path) )

      rpath = Path( path, instance )
      self._rcheckUnplaced( rpath, instance.getMasterCell() )
    return


  def check ( self ):
    self._rcheckUnplaced( Path(), self.cell )
      
    if self.unplaceds:
      if self.flags & (ShowWarnings | WarningsAreErrors):
        message = [ 'Some instances are unplaceds:' ]
        for occurrence in self.unplaceds:
          message += [ '%s' % (occurrence.getCompactString()) ]
        error = ErrorMessage( 3, message )
      
        if self.flags & WarningsAreErrors: raise error
        else:                              print error
    return self.unplaceds


class StackedVia ( object ):

    def __init__ ( self, net, depth, x, y, width, height ):
      self._hasLayout   = False
      self._net         = net
      self._bottomDepth = depth
      self._topDepth    = depth
      self._x           = x
      self._y           = y
      self._width       = width
      self._height      = height
      self._vias        = []
      return

    @property
    def topDepth    ( self ): return self._topDepth
    @property
    def bottomDepth ( self ): return self._bottomDepth

    def getNet ( self ): return self._net

    def mergeDepth ( self, depth ):
      if self._hasLayout:
        print WarningMessage( 'StackedVia.mergeDepth(): Cannot be called *after* StackVia.doLayout()' )
        return
      if depth < self._bottomDepth: self._bottomDepth = depth
      if depth > self._topDepth:    self._topDepth    = depth
      return

    def getLayer ( self, fromTop ):
      if self._topDepth-fromTop >= self._bottomDepth:
        routingGauge = CRL.AllianceFramework.get().getRoutingGauge()
        return routingGauge.getRoutingLayer(self._topDepth-fromTop)
      return None

    def getBlockageLayer ( self, fromTop ):
      if self._topDepth-fromTop >= self._bottomDepth:
        routingGauge = CRL.AllianceFramework.get().getRoutingGauge()
        return routingGauge.getRoutingLayer(self._topDepth-fromTop).getBlockageLayer()
      return None

    def getVia ( self, metal ):
      if not self._hasLayout: return None
      for via in self._vias:
          if via.getLayer().contains(metal): return via
      return None

    def doLayout ( self ):
      if self._hasLayout: return
      self._hasLayout = True

      routingGauge = CRL.AllianceFramework.get().getRoutingGauge()

      if self._bottomDepth == self._topDepth:
        self._vias.append( Contact.create( self._net
                                         , routingGauge.getRoutingLayer(self._bottomDepth)
                                         , self._x    , self._y
                                         , self._width, self._height
                                         ) )
      else:
        for depth in range(self._bottomDepth,self._topDepth):
          if depth == self._bottomDepth:
            self._vias.append( Contact.create( self._net
                                             , routingGauge.getContactLayer(depth)
                                             , self._x    , self._y
                                             , self._width, self._height
                                             ) )
          else:
            self._vias.append( Contact.create( self._vias[-1]
                                             , routingGauge.getContactLayer(depth)
                                             , 0          , 0
                                             , self._width, self._height
                                             ) )
         #print '    Sub-via: ', self._vias[-1]
      return
