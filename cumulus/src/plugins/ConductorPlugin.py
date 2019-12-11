#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2019-2019, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/ConductorPlugin.py"             |
# +-----------------------------------------------------------------+

try:
  import sys
  import traceback
  import os.path
  import math
  import Cfg
  import Hurricane
  from   Hurricane  import Breakpoint
  from   Hurricane  import UpdateSession
  import Viewer
  import CRL
  from   CRL        import RoutingLayerGauge
  import helpers
  from   helpers    import trace
  from   helpers.io import ErrorMessage, catch
  from   helpers    import l, u, n
  import Anabatic
  import Etesian
  import Katana
  import Unicorn
  import plugins
except Exception, e:
  catch( e )
  sys.exit(2)


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    kw['beforeAction'] = 'placeAndRoute.conductor'

    plugins.kwAddMenu    ( 'placeAndRoute', 'P&&R', **kw )
    plugins.kwUnicornHook( 'misc.beta.conductor'
                         , 'P&&R Conductor'
                         , 'Perform a placement driven by global routing, then detailed routing'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def ScriptMain ( **kw ):
  try:
   #helpers.setTraceLevel( 550 )

    errorCode = 0

    stopLevel = 1
    if Cfg.hasParameter('conductor.stopLevel'):
      stopLevel = Cfg.getParamInt('conductor.stopLevel').asInt()
    Breakpoint.setStopLevel( stopLevel )

    maxPlaceIterations = 2
    if Cfg.hasParameter('conductor.maxPlaceIterations'):
      maxPlaceIterations = Cfg.getParamInt('conductor.maxPlaceIterations').asInt()

    cell = None
    if kw.has_key('cell') and kw['cell']:
      cell = kw['cell']

    editor = None
    if kw.has_key('editor') and kw['editor']:
      editor = kw['editor']
      print '  o  Editor found, running in graphic mode.'
      if cell == None: cell = editor.getCell()

    if cell == None:
      raise ErrorMessage( 3, 'Conductor: No cell loaded yet.' )

    success   = False
    etesian   = None
    katana    = None
    iteration = 0

    while iteration < maxPlaceIterations:
      print '\n  o  P&R Conductor iteration: %d (max:%s)' % (iteration,maxPlaceIterations)

      if not (katana is None):
        print '  o  Global routing has failed, re-place design.'
        katana.resetRouting()
        katana.destroy     ()
        katana = None
        if editor:
          editor.setShowSelection( False )
    
      etesian = Etesian.EtesianEngine.create( cell )
      etesian.setPassNumber( iteration )
      if editor:    etesian.setViewer( editor )
      if iteration: etesian.resetPlacement()
      etesian.place()
      etesian.destroy()
      etesian = None
      if editor:
        editor.refresh()
        editor.fit()

      katana = Katana.KatanaEngine.create( cell )
      katana.setPassNumber( iteration )
      if editor: katana.setViewer( editor )
      katana.digitalInit    ()
      katana.runGlobalRouter( Katana.Flags.ShowBloatedInstances
                            | Katana.Flags.ShowOverloadedEdges
                            | Katana.Flags.ShowOverloadedGCells
                            )
                           #| Katana.Flags.ShowFailedNets 
      Breakpoint.stop( 1, 'After routing iteration %d' % iteration )

      if katana.isGlobalRoutingSuccess(): break
      iteration += 1

    if not (katana is None):
      katana.loadGlobalRouting( Anabatic.EngineLoadGrByNet )
      katana.layerAssign      ( Anabatic.EngineNoNetLayerAssign )
      katana.runNegociate     ( Katana.Flags.NoFlags )
      success = katana.isDetailedRoutingSuccess()

      Breakpoint.stop( 1, 'Before finalizing & destroying Katana.' )
      katana.finalizeLayout()
      katana.dumpMeasures()
      katana.destroy()
      katana = None
      
   #plugins.RSavePlugin.ScriptMain( **kw )

  except Exception, e:
    catch( e )
      
  return 0
