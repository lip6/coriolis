
# -*- mode:Python -*-

import sys
import os.path
import string
import traceback
import Hurricane
from   Hurricane import DbU
from   Hurricane import BasicLayer
from   helpers   import ErrorMessage


realFile = '<No real file specified>'


def loadRealLayers ( technology, basicLayersData ):
    entryNo = 0
    for entry in realLayersTable:
        entryNo += 1

        try:
            if len(entry) != 3:
                raise ErrorMessage(['Malformed entry in <realLayersTable>.'
                                   ,'Must have exactly three fields: (symb_name,real_name,GDSII_extnb).'
                                   ,str(entry)
                                   ])
            symbName, realName, gdsiiExtractNumber = entry
            if not isinstance(gdsiiExtractNumber,int):
                raise ErrorMessage(['Incoherency in <realLayersTable> entry.'
                                   ,'GDSII exctract number is not of int type (%s).' \
                                        % helpers.stype(gdsiiExtractNumber)
                                   ,str(entry)
                                   ])

            basicLayer = technology.getBasicLayer(symbName)
            if not basicLayer:
                raise ErrorMessage(['Incoherency in <realLayersTable> entry.'
                                   ,'The real layer "%s" associated to the GDSII "%s" do not exists.' \
                                        % (symbName,realName)
                                   ,str(entry)
                                   ])
            basicLayer.setRealName     ( realName )
            basicLayer.setExtractNumber( gdsiiExtractNumber )

        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<realLayersTable> at index %d.' % (realFile,entryNo))
    return


def loadTechnoConfig ( technology, technoConfig ):
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
                        raise ErrorMessage('In <technoConfig>, invalid DbU unit power for gridUnit, reseting to Micro.')
                else:
                    raise ErrorMessage('<technoConfig> has no <gridUnit> defined, assuming Micro.')

            elif key == 'gridValue':
                if technoConfig.has_key('gridValue'):
                    gridValue = technoConfig['gridValue']
                    if not isinstance(gridUnit,float) and not isinstance(gridUnit,int):
                        raise ErrorMessage(['In <technoConfig>, <gridValue> must be of type float (and not: %s).'
                                            % helpers.stype(gridValue)
                                           ])
                    DbU.setPhysicalsPerGrid(gridValue,gridUnit)
                else:
                    raise ErrorMessage('<technoConfig> has no <gridValue> defined.')
    
            elif key == 'gridsPerLambda':
                if technoConfig.has_key('gridsPerLambda'):
                    gridsPerLambda = technoConfig['gridsPerLambda']
                    if not isinstance(gridsPerLambda,int):
                        raise ErrorMessage(['In <technoConfig>, <gridsPerLambda> must be of type int (and not: %s).'
                                            % helpers.stype(gridsPerLambda)
                                           ])
                    DbU.setGridsPerLambda(gridsPerLambda)
    
        except Exception, e:
            ErrorMessage.wrapPrint(e)
    return


def load ( realPath ):
    tables = ( ('technoConfig'   , loadTechnoConfig  )
             , ('realLayersTable', loadRealLayers    )
             )

    global realFile
    realFile = os.path.basename(realPath)

    confGlobals = globals()
    execfile(realPath,confGlobals)

    technology = Hurricane.getDataBase().getTechnology()

    for symbol, loader in tables:
        if not confGlobals.has_key(symbol):
            print '[ERROR] The <%s> table is missing in the configuration file.' % symbol
            print '        <%s>' % realFile
            print '        Attempting to continue anyway.'
        else:
            loader( technology, confGlobals[ symbol ])

    return


