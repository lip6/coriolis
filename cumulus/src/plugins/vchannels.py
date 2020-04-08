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
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/vchannels.py"                   |
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
  import Etesian
  import Unicorn
  import plugins
  import block.vchannels
except Exception, e:
  catch( e )
  sys.exit(2)


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    kw['beforeAction'] = 'placeAndRoute.placeChip'

    plugins.kwAddMenu    ( 'placeAndRoute', 'P&&R', **kw )
    plugins.kwUnicornHook( 'placeAndRoute.vchannels'
                         , 'Add vertical routing channels'
                         , 'Create vertical spacing to increase the vertical routing capacity'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
  try:
   #helpers.setTraceLevel( 550 )

    errorCode = 0

    cell = None
    if kw.has_key('cell') and kw['cell']:
      cell = kw['cell']

    editor = None
    if kw.has_key('editor') and kw['editor']:
      editor = kw['editor']
      print '  o  Editor found, running in graphic mode.'
      if cell == None: cell = editor.getCell()

    if cell == None:
      raise ErrorMessage( 3, 'VChannels: No cell loaded yet.' )

    if cell.getAbutmentBox().isEmpty():
      raise ErrorMessage( 3, 'VChannels: cell "%s" is not placed yet.' % cell.getName() )

    vchannels = block.VChannels.VChannels( cell )
    for i in range(48):
      vchannels.addChannelAt( l(250.0*(i+1)), l(190.0) )
    vchannels.expandChannels()
    if editor: editor.refresh()

  except Exception, e:
    catch( e )
      
  return 0
