
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
# |  Python      :       "./plugins/stats.py"                       |
# +-----------------------------------------------------------------+

"""
This script hook the Stats plugin inside GCT/Unicorn.

Compute some statistics on a hierarchical block.
"""

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


class Stats ( object ):

    LUT = {}

    @staticmethod
    def lookup ( p ):
        if isinstance(p,Cell): cell = p
        else:                  cell = p.getMasterCell()
        if cell in Stats.LUT: return Stats.LUT[ cell ]
        return None

    @staticmethod
    def isInstanceTerminal ( p ):
        if isinstance(p,Cell): cell = p
        else:                  cell = p.getMasterCell()
        for instance in cell.getInstances():
            return False
        return True

    @staticmethod
    def consolidate ():
        ordereds = {}
        for item in Stats.LUT.values():
            if not item.depth in ordereds:
                ordereds[item.depth] = []
            ordereds[item.depth].append( item )
        rkeys = ordereds.keys()
        rkeys.reverse()
        for key in rkeys:
            for cellStat in ordereds[key]:
                cellStat._consolidate()
        return
        
    def __init__ ( self, cell, depth ):
        self.cell          = cell
        self.consolidated  = False
        self.depth         = depth
        self.directCells   = 0
        self.indirectCells = 0
        self.blocks        = 0
        Stats.LUT[ cell ] = self

        for instance in self.cell.getInstances():
            if Stats.isInstanceTerminal(instance):
                self.directCells += 1
            else:
                self.blocks += 1 
                blockStat = Stats.lookup( instance )
                if blockStat is None:
                    blockStat = Stats( instance.getMasterCell(), depth+1 )
                else:
                    blockStat.updateDepth( depth+1 )
        if self.depth == 0:
            Stats.consolidate()
        return

    @property
    def allCells ( self ): return self.directCells+self.indirectCells

    def updateDepth ( self, depth ):
        if self.depth < depth: self.depth = depth

    def display ( self ):
        if self.depth == 0:
            print( '{:>8} |{:>8} |{:>8} | {}'.format( 'Cumul.', 'Own', 'Blocks', 'MasterCell' ) )
        print( '{:8} |{:8} |{:8} | {}+ {}'.format( self.allCells
                                              , self.directCells
                                              , self.blocks
                                              , ' '*(self.depth*2)
                                              , self.cell.getName() ))
        for instance in self.cell.getInstances():
            if Stats.isInstanceTerminal(instance): continue
            blockStat = Stats.lookup( instance )
            if blockStat is None: continue
            blockStat.display()
        return

    def _consolidate ( self ):
        if self.consolidated: return
        for instance in self.cell.getInstances():
            if Stats.isInstanceTerminal(instance): continue
            blockStat = Stats.lookup( instance )
            if blockStat is None:
                continue
            self.indirectCells += blockStat.allCells
        self.consolidated = True
        return
        


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    kw['beforeAction'] = 'misc.alpha'

    plugins.kwUnicornHook( 'misc.alpha.stats'
                         , 'Statistics'
                         , 'Compute some statistics on a hierarchy of Cells '
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
    """The mandatory function that Coriolis CGT/Unicorn will look for."""
    rvalue = True
    try:
        helpers.setTraceLevel( 550 )
        cell, editor = plugins.kwParseMain( **kw )

        stats = Stats( cell, 0 )
        stats.display()
    except Exception as e:
        helpers.io.catch( e )
        rvalue = False

    sys.stdout.flush()
    sys.stderr.flush()
        
    return rvalue
