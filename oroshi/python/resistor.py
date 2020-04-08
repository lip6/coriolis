# -*- coding: utf-8 -*-

from   Hurricane import DataBase
from   Hurricane import UpdateSession
from   Hurricane import DbU
from   Hurricane import Box
from   Hurricane import Net
import helpers
import helpers.io
from   helpers   import trace

helpers.setTraceLevel( 100 )

import Analog
from   Analog import Device
import oroshi
import oroshi.paramsmatrix
from   oroshi.resistorsnake import Resistor



def checkCoherency ( device, bbMode ):
    message = 'Resistor.checkCoherency(): device "%s".\n' % device.getName()
    
    techno = DataBase.getDB().getTechnology()
    rules  = oroshi.getRules()

    resistance = device.getParameter( 'R' )
    if resistance is None:
        message += '        Missing "resistance" parameter on %s' % str(device)
        return (False, message)
    
    return (True, "")


def layout ( device, bbMode ):

    trace( 100, ',+', '\tResistor.layout() called for "%s".\n' % device.getName())

    paramsMatrix = oroshi.paramsmatrix.ParamsMatrix()

    try:
        resistance = device.getParameter( 'R' ).getValue()
        width      = device.getParameter( 'W' ).getValue()
        length     = device.getParameter( 'L' ).getValue()
        bends      = device.getParameter( 'bends' ).getValue()
  
        trace( 100, '\tpW:{0}\n'.format(device.getParameter('W')) )
        trace( 100, '\tresistance:{0}, width:{1}, length:{2}\n'.format(resistance,width,length) )
  
        typeArg = 'UnknownType'
        if device.isRPOLYH():   typeArg = 'RPOLYH'
        if device.isRPOLY2PH(): typeArg = 'RPOLY2PH'
  
        netsArg = [ device.getNet('t1'), device.getNet('t2') ]
  
        resistor = Resistor( device
                           , netsArg
                           , typeArg
                           , resistance
                           , resDim={ 'width':width, 'length':length }
                           , direction='vertical'
                           , bends=bends
                           , shape=135
                           )
        resistor.create()
  
        for net in netsArg:
            device.setRestrictions( net, Device.SouthBlocked|Device.NorthBlocked )
  
        paramsMatrix.setGlobalCapacitorParams( device.getAbutmentBox() )
        trace( 100, '++' )
       #paramsMatrix.trace()

    except Exception, e:
        helpers.io.catch( e )

    trace( 100, '---' )
    
    return paramsMatrix.getMatrix()
