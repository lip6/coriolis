# -*- Mode:Python; explicit-buffer-name: "AnalogTechno.py<crlcore/helpers>" -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2015-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :  "./crlcore/python/helpers/AnalogTechno.py"      |
# +-----------------------------------------------------------------+


import os
import os.path
import sys
import Hurricane
from   Hurricane    import DbU
from   Hurricane    import DataBase
from   Hurricane    import Layer
from   helpers.io   import ErrorMessage
from   helpers      import Debug


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
        if entryNo > 1:
          checkEntry( entry, entryNo )

        if entry[0] == 'Header':
          unit = entry[2]
          techno.setName( entry[1] )
          continue

       # Zero-layer rule.
        if len(entry) == 4:
          if entry[2] & Unit:
            techno.addUnitRule( entry[0], entry[1], entry[3] )
          else:
            techno.addPhysicalRule( entry[0]
                                  , valueToDbU( entry[1], unit, entry[2] )
                                  , entry[3]
                                  )
       # One-layer rule.
        if len(entry) == 5:
          techno.addPhysicalRule( entry[0]
                                , entry[1]
                                , valueToDbU( entry[2], unit, entry[3] )
                                , entry[4]
                                )
       # Two-layer rule.
        if len(entry) == 6:
          symmetric = True
          if entry[4] & Asymmetric: symmetric = False

          techno.addPhysicalRule( entry[0]
                                , entry[1]
                                , entry[2]
                                , symmetric
                                , valueToDbU( entry[3], unit, entry[4] )
                                , entry[5]
                                )
      except Exception, e:
        e = ErrorMessage( e )
        e.addMessage( 'In %s:<analogTechnologyTable> at index %d.' % (technoFile,entryNo) )
        print e
    return


def loadAnalogTechno ( table, fromFile ):
    global technoFile
    technoFile = fromFile
    techno     = DataBase.getDB().getTechnology()

    _loadAnalogTechno( techno, table )
    return
