# -*- coding: utf-8 -*-

from   Hurricane import DataBase
from   Hurricane import UpdateSession
from   Hurricane import DbU
from   Hurricane import Box
import helpers
import helpers.io
from   helpers   import trace, u

helpers.setTraceLevel( 1000 )

import Analog
import ParamsMatrix
import oroshi


def checkCoherency ( device, bbMode ):
    valid   = True
    message = 'ResistorSnake.checkCoherency():\n'
    
    techno = DataBase.getDB().getTechnology()
    rules  = oroshi.getRules()

    bends = device.getParameter( 'bends' )
    if bends is None:
      message += '        Missing "bends" parameter on %s' % str(device)
      print message
      return (False, message)

    if not valid: return (False, message)
    
    return (True, "")


def layout ( device, bbMode ):

    trace( 100, ',+', '\tResistorSnake.layout() called.\n' )

    paramsMatrix = ParamsMatrix.ParamsMatrix()

    try:
      bends = device.getParameter( 'bends' )
      print 'bends = ', bends

      device.setAbutmentBox( Box( u(0.0), u(0.0), u(10.0), u((bends.getValue()+1)*10.0)) )

      paramsMatrix.setGlobalResistorParams( device.getAbutmentBox() )
      trace( 100, '++' )
     #paramsMatrix.trace()

    except Exception, e:
      helpers.io.catch( e )

    trace( 100, '---' )
    
    return paramsMatrix.getMatrix()
