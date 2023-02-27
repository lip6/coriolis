
# -*- Mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2015-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :  "./crlcore/python/helpers/analogtechno.py"      |
# +-----------------------------------------------------------------+


import os
import os.path
import sys
from   ..Hurricane import DbU, DataBase, Layer
from   .io         import catch, ErrorMessage, WarningMessage


tech       = None
technoFile = '<technoFile has not been set>'
Length     = 0x0001
Area       = 0x0002
Asymmetric = 0x0004
Unit       = 0x0008


def valueToDbU ( value, unit, lengthType ):
    length = DbU.fromPhysical( value, unit )
    if lengthType & Length: return length

    area = DbU.fromPhysical( float(length), unit )
    return area


def isStepped ( entry ):
    if isinstance(entry,tuple):
        for item in entry:
            if not isinstance(item,tuple) or len(item) != 2:
                return False
        return True
    return False


def isNonIsotropic ( entry ):
    if not isinstance(entry,tuple) or len(entry) != 2: return False
    if not isinstance(entry[0],int) and not isinstance(entry[0],float): return False
    if not isinstance(entry[1],int) and not isinstance(entry[1],float): return False
    return True


def checkEntry ( entry, entryNo ):
    if not isinstance(entry,tuple):
        raise ErrorMessage( 1, [ 'Entry %d is malformed in <analogTechnologyTable>.' % entryNo
                               , 'Not a tuple (a, b, c, ...) or (a,).'
                               , str(entry)
                               ] )
    if not len(entry) in (4, 5, 6):
        raise ErrorMessage( 1, [ 'Entry %d is malformed in <analogTechnologyTable>.' % entryNo
                               , 'Tuple must have *4*, *5* or *6* items only.'
                               , str(entry)
                               ] )
    if not entry[-2] in (Length, Length|Asymmetric, Area, Unit):
        raise ErrorMessage( 1, [ 'Entry %d is malformed in <analogTechnologyTable>.' % entryNo
                               , 'Beforelast item is neither Length, Length|Asymmetric nor Area.'
                               , str(entry)
                               ] )
    return


def _loadAnalogTechno ( techno, ruleTable ):
    unit    = None
    entryNo = 0
    for entry in ruleTable:
        entryNo += 1
        
        try:
            if entryNo > 1: checkEntry( entry, entryNo )
          
            if entry[0] == 'Header':
                unit = entry[2]
                techno.setName( entry[1] )
                continue
          
           # Zero-layer rule.
            if len(entry) == 4:
                if entry[2] & Unit:
                    rule = techno.addUnitRule( entry[0], entry[3] )
                    rule.addValue( entry[1] )
                else:
                    rule = techno.addPhysicalRule( entry[0], entry[3] )
                    rule.addValue( valueToDbU(entry[1], unit, entry[2]), 0 )
           # One-layer rule.
            if len(entry) == 5:
                rule = techno.addPhysicalRule( entry[0], entry[1], entry[4] )
                if entry[3] & Unit:
                    rule.addValue( entry[2] )
                else:
                    if isStepped(entry[2]):
                        for step in entry[2]:
                            rule.addValue( valueToDbU(step[0], unit, entry[3])
                                         , valueToDbU(step[1], unit, entry[3]) )
                    elif isNonIsotropic(entry[2]):
                        rule.addValue( valueToDbU(entry[2][0], unit, entry[3])
                                     , valueToDbU(entry[2][1], unit, entry[3])
                                     , 0 )
                    else:
                        rule.addValue( valueToDbU(entry[2], unit, entry[3]), 0 )
           # Two-layer rule.
            if len(entry) == 6:
                symmetric = True
                if entry[4] & Asymmetric: symmetric = False
                rule = techno.addPhysicalRule( entry[0], entry[1], entry[2], entry[5] )
                rule.setSymmetric( symmetric )
                
                if isNonIsotropic(entry[3]):
                    rule.addValue( valueToDbU(entry[3][0], unit, entry[4])
                                 , valueToDbU(entry[3][1], unit, entry[4])
                                 , 0 )
                else:
                    rule.addValue( valueToDbU(entry[3], unit, entry[4]), 0 )
        except Exception as e:
            e = ErrorMessage( 1, e )
            e.addMessage( 'In {}:<analogTechnologyTable> at index {}.'.format(technoFile,entryNo) )
            print( str(e) )
    return


def loadAnalogTechno ( table, fromFile ):
    global technoFile
    global tech
    if not tech:
        tech = DataBase.getDB().getTechnology() 
    technoFile = fromFile
    _loadAnalogTechno( tech, table )
    return


def addDevice ( **kw ):
    global tech
    if not tech:
        tech = DataBase.getDB().getTechnology() 
    
    try:
        if 'name' in kw: 
            devDesc = tech.addDeviceDescriptor( kw['name'] )
            if 'spice' in kw: devDesc.setSpiceFilePath( kw['spice'] )
            if 'connectors' in kw: 
                for connector in kw['connectors']:
                    devDesc.addConnector( connector )
            else:
                print( WarningMessage( 'common.addDevice(): Missing connectors on device "{}".' \
                                       .format(kw['name'])))
            if 'layouts' in kw: 
                for layout in kw['layouts']:
                    devDesc.addLayout( layout[0], layout[1] )
            else:
                print( WarningMessage( 'common.addDevice(): Missing layouts on device "{}".' \
                                       .format( kw['name'] ))) 
    except Exception as e:
        catch( e ) 
    return

