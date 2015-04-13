# -*- Mode:Python; explicit-buffer-name: "Alliance.py<crlcore/helpers>" -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2012-2015, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./crlcore/helpers/Alliance.py"            |
# +-----------------------------------------------------------------+


import os
import os.path
import sys
import Hurricane
from   Hurricane import DbU
from   Hurricane import DataBase
from   Hurricane import Layer
import CRL
from   CRL       import Environment
from   CRL       import AllianceFramework
from   CRL       import RoutingGauge
from   CRL       import RoutingLayerGauge
from   CRL       import CellGauge
from   helpers   import ErrorMessage
from   helpers   import Debug
import helpers.RealTechnology
import helpers.SymbolicTechnology


allianceFile = '<allianceFile has not been set>'


class AddMode ( object ):

  Append  = 1
  Prepend = 2
  Replace = 3

  @staticmethod
  def toEnvironment ( mode ):
    if mode == AddMode.Prepend: return Environment.Prepend
    if mode == AddMode.Replace: return Environment.Replace
    return Environment.Append


class Gauge ( object ):

  Vertical   = 1
  Horizontal = 2
  PinOnly    = 3
  Default    = 4

  @staticmethod
  def toRlGauge ( value ):
    if value == Gauge.Vertical:   return RoutingLayerGauge.Vertical
    if value == Gauge.Horizontal: return RoutingLayerGauge.Horizontal
    if value == Gauge.PinOnly:    return RoutingLayerGauge.PinOnly
    if value == Gauge.Default:    return RoutingLayerGauge.Default
    return None


def xmlToConf ( xmlPath ):
    hasExtention = False
    components   = xmlPath.split(os.sep)
    filename     = components[-1]
    if filename.endswith('.xml'):
        hasExtention = True
        filename     = filename[:-4]
    if filename.endswith('.conf'):
        hasExtention = True
        filename     = filename[:-5]
    filename = filename.replace('.','_')
    confPath = os.sep.join(components[:-1] + [filename])
    if hasExtention:
        confPath += '.conf'
    return confPath


def _loadAllianceConfig ( af, allianceConfig ):
    env = af.getEnvironment()

    entryNo = 0
    for entry in allianceConfig:
        entryNo += 1

        try:
            if len(entry) != 2:
                raise ErrorMessage(1,['Malformed entry in <allianceConfig>.'
                                     ,'Must have exactly two fields ("key", <value>).'
                                     ,str(entry)
                                     ])

            key, value = entry
            if key == 'DISPLAY':             env.setDISPLAY(value)
            if key == 'CATALOG':             env.setCATALOG(value)
            if key == 'SCALE_X':             env.setSCALE_X(value)
            if key == 'IN_LO':               env.setIN_LO(value)
            if key == 'IN_PH':               env.setIN_PH(value)
            if key == 'OUT_PH':              env.setOUT_PH(value)
            if key == 'OUT_LO':              env.setOUT_LO(value)
            if key == 'POWER':               env.setPOWER(value)
            if key == 'GROUND':              env.setGROUND(value)
            if key == 'CLOCK':               env.setCLOCK(value)
            if key == 'BLOCKAGE':            env.setBLOCKAGE(value)
            if key == 'PAD':                 env.setPad(value)
            if key == 'WORKING_LIBRARY':     env.setWORKING_LIBRARY(value)
            if key == 'SYSTEM_LIBRARY':
                for libraryEntry in value:
                    if len(libraryEntry) != 2:
                        raise ErrorMessage(1,['Malformed system library entry in <allianceConfig>.'
                                             ,'Must have exactly two fields ("path", <mode>).'
                                             ,str(libraryEntry)
                                             ])
                    libPath, mode = libraryEntry
                    env.addSYSTEM_LIBRARY(library=libPath,mode=AddMode.toEnvironment(mode))

        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<Alliance> at index %d.' % (allianceFile,entryNo))
    try:
        env.validate()
    except Exception, e:
        ErrorMessage.wrapPrint(e,'In %s:<Alliance>.' % (allianceFile))
        sys.exit(1)
    return


def loadRoutingGaugesTable ( routingGaugesTable, fromFile ):
    global allianceFile
    allianceFile = fromFile

    af = AllianceFramework.get()

    for gaugeName in routingGaugesTable.keys():
        gaugeDatas = routingGaugesTable[gaugeName]

        technology = DataBase.getDB().getTechnology()
        gauge      = RoutingGauge.create(gaugeName)
        
        entryNo = 0
        for entry in gaugeDatas:
            entryNo += 1

            try:
                if len(entry) != 2:
                    raise ErrorMessage(1,['Malformed entry in <routingGaugesTable[%s]>.' % gaugeName
                                         ,'Must have exactly two fields ("METAL_LAYER", (parameters_list)).'
                                         ,str(entry)
                                         ])
                if len(entry[1]) != 8:
                    raise ErrorMessage(1,['Malformed entry in <routingGaugesTable[%s]>.' % gaugeName
                                         ,'Parameters list must have exactly eight fields:'
                                         ,'  (direction, type, depth, density, offset, pitch, wire_width, via_width)'
                                         ,str(entry)
                                         ])

                gauge.addLayerGauge( RoutingLayerGauge.create( technology.getLayer(entry[0])
                                                             , Gauge.toRlGauge(entry[1][0]) # Direction. 
                                                             , Gauge.toRlGauge(entry[1][1]) # Type.
                                                             , entry[1][2]                  # Depth.
                                                             , entry[1][3]                  # Density.
                                                             , DbU.fromLambda(entry[1][4])  # Offset.
                                                             , DbU.fromLambda(entry[1][5])  # Pitch.
                                                             , DbU.fromLambda(entry[1][6])  # Wire width.
                                                             , DbU.fromLambda(entry[1][7])  # Via width.
                                                             ) )

            except Exception, e:
                ErrorMessage.wrapPrint(e,'In %s:<routingGaugesTable> at index %d.' % (allianceFile,entryNo))

        af.addRoutingGauge(gauge)
    return


def loadCellGaugesTable ( cellGaugesTable, fromFile ):
    global allianceFile
    allianceFile = fromFile

    af = AllianceFramework.get()

    for gaugeName in cellGaugesTable.keys():
        gaugeDatas = cellGaugesTable[gaugeName]
        gauge      = None

        try:
            if len(gaugeDatas) != 4:
                raise ErrorMessage(1,['Malformed gaugeDatas in <cellGaugesTable[%s]>.' % gaugeName
                                    ,'Parameters list must have exactly four fields:'
                                    ,'  (terminal_metal, xy_common_pitch, slice_height, slice_step)'
                                    ,str(gaugeDatas)
                                    ])
            gauge = CellGauge.create( gaugeName
                                    , gaugeDatas[0]                 # pinLayerName.
                                    , DbU.fromLambda(gaugeDatas[1]) # pitch.
                                    , DbU.fromLambda(gaugeDatas[2]) # sliceHeight.
                                    , DbU.fromLambda(gaugeDatas[3]) # sliceStep.
                                    )
        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<cellGaugesTable> at index %d.' % (allianceFile,gaugeDatasNo))

        if gauge: af.addCellGauge(gauge)
    return


def loadAllianceConfig ( table, fromFile ):
    global allianceFile
    allianceFile = fromFile

    af         = AllianceFramework.get()
    db         = DataBase.getDB()
    technology = db.getTechnology()
    if not technology:
      technology = Hurricane.Technology.create(db,'Alliance')

    _loadAllianceConfig( af, table )
    env = af.getEnvironment()
    return
