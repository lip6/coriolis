#!/usr/bin/env python
#
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
# |  Python      :       "./plugins/checks.py"                      |
# +-----------------------------------------------------------------+


from   __future__ import print_function
import sys
import traceback
import os.path

try:
    import Cfg
    import CRL
    import helpers
    from   helpers.io import ErrorMessage
    from   helpers.io import WarningMessage
    import plugins
    from   Hurricane  import Net, Plug
except Exception, e:
    helpers.io.catch( e )
    sys.exit(2)


def oneDriver ( cell ):
    """
    Checks that all nets have *one and only one* driver.
    """
    print( '  o  Checks that all nets have exactly one drivers on "{}"'.format(cell.getName()) )
    for net in cell.getNets():
        if net.isGlobal() or net.isSupply():
            continue
        drivers = []
        if net.isExternal():
            if net.getDirection() & Net.Direction.IN:
                drivers.append( net )
            elif net.getDirection() & Net.Direction.OUT:
                pass
            else:
                print( WarningMessage( [ 'checks.oneDriver(): External net without direction.'
                                       , '{}'.format(net) ] ))
        for plug in net.getPlugs():
            masterNet = plug.getMasterNet()
            if masterNet.getDirection() & Net.Direction.OUT:
                drivers.append( plug )
        #for pin in net.getPins():
        #    if len(drivers) == 0:
        #        drivers.append( pin )
        if len(drivers) == 0:
            print( WarningMessage( 'checks.oneDriver(): "{}" has no driver.'.format(net.getName()) ) )
        elif len(drivers) > 1:
            message = [ 'checks.oneDriver(): "{}" has more than one drivers.'.format(net.getName()) ]
            for driver in drivers:
                if isinstance(driver,Plug):
                    message.append( '* {} {}'.format(driver,driver.getMasterNet()) )
                else:
                    message.append( '* {}'.format(driver) )
            print( WarningMessage( message ))
    return


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    """Hook up rsave plugin into Unicorn/CGT menu tree."""
    plugins.kwUnicornHook( 'tools.checks'
                         , 'Netlist Checks'
                         , 'Perform coherency checks on the netlist'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
    """Called when run as a stand alone script through Unicorn/CGT."""
    try:
       #helpers.setTraceLevel( 550 )
        cell, editor = plugins.kwParseMain( **kw )
        if not cell:
            print( WarningMessage( 'No Cell loaded in the editor (yet), nothing done.' ) )
            return 0
        oneDriver( cell )
    except Exception, e:
      helpers.io.catch( e )
    sys.stdout.flush()
    sys.stderr.flush()
    return 0
