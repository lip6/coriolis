# -*- Mode:Python; explicit-buffer-name: "Devices.py<crlcore/helpers>" -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2015-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :  "./crlcore/python/helpers/Devices.py"           |
# +-----------------------------------------------------------------+


import os
import os.path
import sys
from   Hurricane import DataBase
from   helpers   import ErrorMessage
from   helpers   import Debug


devicesFile = '<devicesFile has not been set>'


def checkEntry ( entry, entryNo ):
    if not isinstance(entry,dict):
      raise ErrorMessage( 1, [ 'Entry %d is malformed in <devicesTable>.' % entryNo
                             , 'Not a dictionnary { key1:value1, key2:value2 }.'
                             , str(entry)
                             ] )
    if not entry.has_key('name'):
      raise ErrorMessage( 1, [ 'Entry %d is malformed in <devicesTable>.' % entryNo
                             , 'Dictionnary is missing the <name> key/value pair.'
                             , str(entry)
                             ] )
    if not entry.has_key('spice'):
      raise ErrorMessage( 1, [ 'Entry %d is malformed in <devicesTable>.' % entryNo
                             , 'Dictionnary is missing the <spice> key/value pair.'
                             , str(entry)
                             ] )
    if not entry.has_key('connectors'):
      raise ErrorMessage( 1, [ 'Entry %d is malformed in <devicesTable>.' % entryNo
                             , 'Dictionnary is missing the <connectors> key/value pair.'
                             , str(entry)
                             ] )
    if not entry.has_key('layouts'):
      raise ErrorMessage( 1, [ 'Entry %d is malformed in <devicesTable>.' % entryNo
                             , 'Dictionnary is missing the <layouts> key/value pair.'
                             , str(entry)
                             ] )
    if not isinstance(entry['connectors'],tuple):
      raise ErrorMessage( 1, [ 'Entry %d is malformed in <devicesTable>.' % entryNo
                             , 'The <connectors> value is not a *tuple*.'
                             , str(entry)
                             ] )
    layouts = entry['layouts']
    if not isinstance(layouts,tuple):
      raise ErrorMessage( 1, [ 'Entry %d is malformed in <devicesTable>.' % entryNo
                             , 'The <layout> value is not a *tuple*.'
                             , str(entry)
                             ] )
    for layout in layouts:
        if not isinstance(layout,tuple) or len(layout) != 2:
          raise ErrorMessage( 1, [ 'Entry %d is malformed in <devicesTable>.' % entryNo
                               , 'One <layout> item is not a two element *tuple*.'
                               , str(entry)
                               ] )
    return


def _loadDevices ( techno, ruleTable ):
    unit    = None
    entryNo = 0
    for entry in ruleTable:
      entryNo += 1

      try:
        checkEntry( entry, entryNo )

        devDesc = techno.addDeviceDescriptor( entry['name'] )
        devDesc.setSpiceFilePath( entry['spice'] )

        for connector in entry['connectors']:
          devDesc.addConnector( connector )

        for layout in entry['layouts']:
          devDesc.addLayout( layout[0], layout[1] )
      except Exception, e:
        ErrorMessage.wrapPrint(e,'In %s:<devicesTable> at index %d.' % (devicesFile,entryNo))
    return


def loadDevices ( table, fromFile ):
    global devicesFile
    devicesFile = fromFile
    techno      = DataBase.getDB().getTechnology()

    _loadDevices( techno, table )
    return
