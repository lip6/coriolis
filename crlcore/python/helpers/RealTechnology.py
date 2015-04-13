
# -*- mode:Python -*-

import sys
import os.path
import string
import traceback
import Hurricane
from   Hurricane import DbU
from   Hurricane import DataBase
from   Hurricane import BasicLayer
from   helpers   import ErrorMessage


realFile = '<No real file specified>'


def loadGdsLayers ( realLayersTable, confFile ):
    realFile   = confFile
    technology = DataBase.getDB().getTechnology()

    entryNo = 0
    for entry in realLayersTable:
        entryNo += 1

        try:
            if len(entry) != 3:
                raise ErrorMessage(1,['Malformed entry in <realLayersTable>.'
                                     ,'Must have exactly three fields: (symb_name,real_name,GDSII_extnb).'
                                     ,str(entry)
                                     ])
            symbName, realName, gdsiiExtractNumber = entry
            if not isinstance(gdsiiExtractNumber,int):
                raise ErrorMessage(1,['Incoherency in <realLayersTable> entry.'
                                     ,'GDSII exctract number is not of int type (%s).' \
                                          % helpers.stype(gdsiiExtractNumber)
                                     ,str(entry)
                                     ])

            basicLayer = technology.getBasicLayer(symbName)
            if not basicLayer:
                raise ErrorMessage(1,['Incoherency in <realLayersTable> entry.'
                                     ,'The real layer "%s" associated to the GDSII "%s" do not exists.' \
                                          % (symbName,realName)
                                     ,str(entry)
                                     ])
            basicLayer.setRealName     ( realName )
            basicLayer.setExtractNumber( gdsiiExtractNumber )

        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<realLayersTable> at index %d.' % (realFile,entryNo))
    return


def loadTechnoConfig (  technoConfig, confFile ):
    realFile   = confFile
    technology = DataBase.getDB().getTechnology()

    gridValue = 1
    gridUnit  = DbU.UnitPowerMicro
    for key in [ 'gridUnit', 'gridValue', 'gridsPerLambda' ]:
        try:
            if key == 'gridUnit':
                if technoConfig.has_key(key):
                    gridUnit = technoConfig[key]
                    if gridUnit != DbU.UnitPowerPico  and \
                       gridUnit != DbU.UnitPowerNano  and \
                       gridUnit != DbU.UnitPowerMicro and \
                       gridUnit != DbU.UnitPowerMilli and \
                       gridUnit != DbU.UnitPowerUnity and \
                       gridUnit != DbU.UnitPowerKilo:
                        raise ErrorMessage(1,'In <technoConfig>, invalid DbU unit power for gridUnit, reseting to Micro.')
                else:
                    raise ErrorMessage(1,'<technoConfig> has no <gridUnit> defined, assuming Micro.')

            elif key == 'gridValue':
                if technoConfig.has_key('gridValue'):
                    gridValue = technoConfig['gridValue']
                    if not isinstance(gridUnit,float) and not isinstance(gridUnit,int):
                        raise ErrorMessage(1,['In <technoConfig>, <gridValue> must be of type float (and not: %s).'
                                              % helpers.stype(gridValue)
                                             ])
                    DbU.setPhysicalsPerGrid(gridValue,gridUnit)
                else:
                    raise ErrorMessage(1,'<technoConfig> has no <gridValue> defined.')
    
            elif key == 'gridsPerLambda':
                if technoConfig.has_key('gridsPerLambda'):
                    gridsPerLambda = technoConfig['gridsPerLambda']
                    if not isinstance(gridsPerLambda,int):
                        raise ErrorMessage(1,['In <technoConfig>, <gridsPerLambda> must be of type int (and not: %s).'
                                            % helpers.stype(gridsPerLambda)
                                           ])
                    DbU.setGridsPerLambda(gridsPerLambda)
    
        except Exception, e:
            ErrorMessage.wrapPrint(e)
    return
