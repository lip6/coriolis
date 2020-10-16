#
# This file is part of the Coriolis Software.
# Copyright (c) SU 2020-2020, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/block/timing.py"                |
# +-----------------------------------------------------------------+

"""
This module provide a very basic support for timing related features.

For now, the electrical values are taken from the sxlib dummy 0.35um
technology. See ``man sxlib``, **output drive** section.
"""

from   __future__ import print_function
import sys
import os.path
import re
from   operator  import itemgetter, attrgetter, methodcaller
import Cfg
from   Hurricane import Breakpoint
from   Hurricane import DbU
from   Hurricane import Box
from   Hurricane import Transformation
from   Hurricane import Box
from   Hurricane import Path
from   Hurricane import Layer
from   Hurricane import Occurrence
from   Hurricane import Net
from   Hurricane import HyperNet
from   Hurricane import RoutingPad
from   Hurricane import Horizontal
from   Hurricane import Vertical
from   Hurricane import Contact
from   Hurricane import Pin
from   Hurricane import Plug
from   Hurricane import Instance
import CRL
from   CRL             import RoutingLayerGauge
from   helpers         import trace, l, u, n
from   helpers.io      import ErrorMessage
from   helpers.io      import WarningMessage
from   helpers.io      import catch
from   helpers.overlay import UpdateSession
from   plugins         import getParameter
from   plugins         import utils


class CellTimings ( object ):
    """
    Contains the timing data related to a Cell.
    """

    def __init__ ( self, cell ):
        self.cell  = cell
        self.drive = 0.0

    @property
    def name ( self ): return self.cell.getName()

    def __str__ ( self ):
        return '<CellTimings "{}" drive:{}>'.format(self.name, self.drive)


class TechTimings ( object ):
    """
    Timing datas for the technology and the standard cells.
    """

    def __init__ ( self ):
        self.cells         = {}
        self.capaBaseDrive = 0.0
        self.capaAvgFanin  = 0.0
        self.capaPerLambda = 0.0

    def addCell ( self, cellTiming ):
        if self.cells.has_key(cellTiming.name):
            print( ErrorMessage( 1, 'TechTimings.addCell(): Redefinition of timings for "{}"' \
                                    .format(cellTiming.name) ))
        self.cells[ cellTiming.name ] = cellTiming

    def getCapaEstimate ( self, WL, sinks ):
        return WL*self.capaPerLambda + self.capaAvgFanin*sinks

    def getWlEstimate ( self, cellName, sinks ):
        drivingCapa = self.getDrivingCapa( cellName )
       #print( 'sinks:{}, dC:{}, avgFanin:{}, CpL:{}'.format(sinks,drivingCapa,self.capaAvgFanin,self.capaPerLambda) )
       #print( '{}'.format((drivingCapa - self.capaAvgFanin*sinks) / self.capaPerLambda) )
        return DbU.fromLambda( (drivingCapa - self.capaAvgFanin*sinks) / self.capaPerLambda )

    def getOneSinkEqWL ( self ):
        """Return the equivalent wirelength of the sink average capacity."""
        return DbU.fromLambda(self.capaAvgFanin / self.capaPerLambda)

    def getSinksEstimate ( self, cellName ):
        """
        Estimate the number sinks that gate can drive. We assume a 100 lambda
        wire to connect to each sink.
        """
        drivingCapa = self.getDrivingCapa( cellName )
        return drivingCapa / (self.capaAvgFanin + 100.0*self.capaPerLambda)

    def getDrivingCapa ( self, name ):
        if not self.cells.has_key(name):
            print( ErrorMessage( 1, 'TechTimings.getDrivingCapa(): No timings for "{}"' \
                                    .format(name) ))
            return 0.0
        return self.cells[name].drive
            

# -------------------------------------------------------------------
# Module static initialization

tech = None
cellsTimingsDatas = ( ('inv_x1', 1.0)
                    , ('inv_x2', 1.6)
                    , ('inv_x4', 3.6)
                    , ('inv_x8', 8.4)
                    , ('buf_x2', 2.1)
                    , ('buf_x4', 4.3)
                    , ('buf_x8', 8.4)
                    )

def staticInit ():
    """
    Static initialization of the ``timing`` module. Allow to postpone
    initialization until Hurricane & CRL database and evironment are
    properly loaded.

    Capacitance unit is fF (femto Farad).
    """
    global tech
    if tech is not None: return

    af = CRL.AllianceFramework.get()
    tech = TechTimings()
    tech.capaAvgFanin  = 10.0
    tech.capaBaseDrive = 125.0
    tech.capaPerLambda = 0.3
    for cellName, drive in cellsTimingsDatas:
        cell             = af.getCell( cellName, CRL.Catalog.State.Views )
        if not cell:
            print( WarningMessage('timing.staticInit(): Library do not provides "{}", skipped.' \
                                  .format(cellName)) )
            continue
        cellTiming       = CellTimings( cell )
        cellTiming.drive = drive*tech.capaBaseDrive
        tech.addCell( cellTiming )
