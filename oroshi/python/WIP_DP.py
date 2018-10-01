# -*- coding: utf-8 -*-

from   Hurricane import DataBase
from   Hurricane import UpdateSession
from   Hurricane import DbU
from   Hurricane import Box
import helpers
from   helpers   import trace

helpers.staticInitialization( quiet=True )
helpers.setTraceLevel( 1000 )

import Analog
import ParamsMatrix
import Stack
import oroshi


def checkCoherency ( args ):
    message = '<b>[ERROR] DifferentialPair.checkCoherency():</b>\n'
    
    techno = DataBase.getDB().getTechnology()
    rules  = Analog.getRules( techno, Analog.Flag.StandardRules )

    M    = args.getM()
    mMax = args.getW() / rules['transistorMinW']
    if M > mMax:
      message += \
        '        W/M ratio must be greater than transistor minimal width (%d)\n' \
        '        Please increase W or decrease M.' \
        % rules['transistorMinW']
      return False, message

    mint = args.getMInt()
    if mint != 2:
      message += '        For interdigitaded layout style, Mint *must* be equal to 2 (not %d)\n' % mint
      return False, message
    
    return True, ''


def layout ( device, args, bbMode ):

    trace( 100, ',+', '\tWIP_DP.layout() called.\n' )
    nerc = device.getParameter( 'NERC' ).getValue()
    nirc = device.getParameter( 'NIRC' ).getValue()

    stack = Stack.Stack( device, nerc, nirc )

    bw  = str(device.getParameter( 'B.w'  ).getValue())
    d1w = str(device.getParameter( 'D1.w' ).getValue())
    d2w = str(device.getParameter( 'D2.w' ).getValue())
    g1w = str(device.getParameter( 'G1.w' ).getValue())
    g2w = str(device.getParameter( 'G2.w' ).getValue())
    sw  = str(device.getParameter( 'S.w'  ).getValue())

    diffMap = { 'D1w':str(device.getParameter( 'D1.w' ).getValue())
              , 'D2w':str(device.getParameter( 'D2.w' ).getValue())
              , 'Sw' :str(device.getParameter( 'S.w'  ).getValue())
              , 'G1w':str(device.getParameter( 'G1.w' ).getValue())
              , 'G2w':str(device.getParameter( 'G2.w' ).getValue())
              , 'Bw' :str(device.getParameter( 'B.w'  ).getValue())
              }

    M           = device.getM()
    remain      = M % device.getMint()
    westWirings = ''
    eastWirings = ''
    for i in range(device.getExternalDummy()):
      westWirings = westWirings + 'B.bX.{Bw}  B.bX.{Bw}  '
      eastWirings = '  B.bX.{Bw}  B.bX.{Bw}' + eastWirings
    if remain:
      westWirings = westWirings + 'D1.b1.{D1w}  G1.b0.{G1w}  '
      eastWirings = '  G2.t0.{G2w}  D2.t1.{D2w}' + eastWirings
    westWirings = westWirings + 'S.b2.{Sw}  '
    

    mintWirings = ''
    for i in range( (M / device.getMint())*2 ):
      if (i + remain) % 2: mintWirings += 'G2.t0.{G2w}  D2.t1.{D2w} G2.t0.{G2w} S.b2.{Sw}  '
      else:                mintWirings += 'G1.b0.{G1w}  D1.b1.{D1w} G1.b0.{G1w} S.b2.{Sw}  '

    wirings = westWirings + mintWirings + eastWirings

    stack.setWirings( wirings.format( **diffMap ) )
    stack.doLayout  ( bbMode )

    paramsMatrix = ParamsMatrix.ParamsMatrix()
    paramsMatrix.setGlobalParams( oroshi.toUnity(stack.w)
                                , oroshi.toUnity(stack.L)
                                , device.getM()
                                , stack.boundingBox
                                )
    paramsMatrix.setStacks( [ stack ] )
    trace( 100, '++' )
    paramsMatrix.trace()
    trace( 100, '---' )
    
    return paramsMatrix.getMatrix()
