# -*- coding: utf-8 -*-

from   Hurricane import DataBase
from   Hurricane import UpdateSession
from   Hurricane import DbU
from   Hurricane import Box
import helpers
import helpers.io
from   helpers   import trace

helpers.staticInitialization( quiet=True )
helpers.setTraceLevel( 1000 )

import Analog
import ParamsMatrix
import Stack
import oroshi


def checkCoherency ( device, bbMode ):
    message = '[ERROR] Transistor.checkCoherency():\n'
    
    techno = DataBase.getDB().getTechnology()
    rules  = oroshi.getRules()

    w    = device.getParameter( 'W' ).getValue()
    M    = device.getParameter( 'M' ).getValue()
    mMax = w / rules.transistorMinW
    if M > mMax:
      message += \
        '        W/M ratio must be greater than transistor minimal width (%s)\n' \
        '        Please increase W or decrease M.' \
        % DbU.getValueString(rules.transistorMinW)
      return (False, message)
    
    return (True, "")


def layout ( device, bbMode ):

    trace( 100, ',+', '\tWIP_Transistor.layout() called.\n' )

    paramsMatrix = ParamsMatrix.ParamsMatrix()

    try:
      nerc = device.getParameter( 'NERC' ).getValue()
      nirc = device.getParameter( 'NIRC' ).getValue()
      
      stack = Stack.Stack( device, nerc, nirc )
      
      bw  = str(device.getParameter( 'B.w' ).getValue())
      dw  = str(device.getParameter( 'D.w' ).getValue())
      gw  = str(device.getParameter( 'G.w' ).getValue())
      sw  = str(device.getParameter( 'S.w' ).getValue())
      
      diffMap = { 'D':'D', 'S':'S', 'Dw':dw, 'Sw':sw, 'Gw':gw, 'Bw':bw }
      if device.isSourceFirst():
        trace( 100, '\tUse sourceFirst.\n' )
        diffMap = { 'D':'S', 'S':'D', 'Dw':sw, 'Sw':dw, 'Gw':gw, 'Bw':bw }
      
      wirings = '{D}.b0.{Dw}  G.t0.{Gw}  {S}.b1.{Sw}'
      for i in range(device.getM() - 1):
        if i%2: wirings += ' G.t0.{Gw} {S}.b1.{Sw}'
        else:   wirings += ' G.t0.{Gw} {D}.b0.{Dw}'
      for i in range(device.getExternalDummy()):
        wirings  = 'B.bX.{Bw}  B.bX.{Bw}  ' + wirings
        wirings += '  B.bX.{Bw}  B.bX.{Bw}'
      
      stack.setWirings( wirings.format( **diffMap ) )
      stack.doLayout  ( bbMode )
      
      paramsMatrix.setGlobalTransistorParams( oroshi.toUnity(stack.w)
                                            , oroshi.toUnity(stack.L)
                                            , device.getM()
                                            , stack.boundingBox
                                            )
      paramsMatrix.setStacks( [ stack ] )
      trace( 100, '++' )
      paramsMatrix.trace()

    except Exception, e:
      helpers.io.catch( e )

    trace( 100, '---' )
    
    return paramsMatrix.getMatrix()
