# -*- coding: utf-8 -*-

from ..Hurricane  import DataBase, UpdateSession, DbU, Box
from ..helpers.io import catch
from ..helpers    import setTraceLevel, trace

#setTraceLevel( 1000 )

try:
    from ..     import Analog
    from .      import getRules, toUnity
    from .      import paramsmatrix
    from .stack import Stack
except Exception as e:
    helpers.io.catch( e )


def checkCoherency ( device, bbMode ):
    message = '[ERROR] CommonSourcePair.checkCoherency():\n'
    
    techno = DataBase.getDB().getTechnology()
    rules  = getRules()

    W    = device.getParameter( 'W' ).getValue()
    M    = device.getParameter( 'M' ).getValue()
    mMax = W // rules.transistorMinW
    if M > mMax:
      message += \
        '        W/M ratio must be greater than transistor minimal width (%s)\n' \
        '        Please increase W or decrease M.' \
        % DbU.getValueString(rules.transistorMinW)
      return False, message

    Mint = device.getParameter( 'Mint' ).getValue()
    if Mint != 2:
      message += '        For interdigitaded layout style, Mint *must* be equal to 2 (not %d)\n' % Mint
      return False, message
    
    return True, ''


def layout ( device, bbMode ):

    trace( 100, ',+', '\tWIP_CSP.layout() called.\n' )
    nerc = device.getParameter( 'NERC' ).getValue()
    nirc = device.getParameter( 'NIRC' ).getValue()

    stack = Stack( device, nerc, nirc )

    bw  = str(device.getParameter( 'B.w'  ).getValue())
    d1w = str(device.getParameter( 'D1.w' ).getValue())
    d2w = str(device.getParameter( 'D2.w' ).getValue())
    gw  = str(device.getParameter( 'G.w'  ).getValue())
    sw  = str(device.getParameter( 'S.w'  ).getValue())

    diffMap = { 'D1w':str(device.getParameter( 'D1.w' ).getValue())
              , 'D2w':str(device.getParameter( 'D2.w' ).getValue())
              , 'Sw' :str(device.getParameter( 'S.w'  ).getValue())
              , 'Gw' :str(device.getParameter( 'G.w'  ).getValue())
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
      westWirings = westWirings + 'D1.t0.{D1w}  G.b0.{Gw}  '
      eastWirings = '  G.b0.{Gw}  D2.t3.{D2w}' + eastWirings
    westWirings = westWirings + 'S.b1.{Sw}  '

    mintWirings = ''
    for i in range( (M // device.getMint())*2 ):
      if (i + remain) % 2: mintWirings += 'G.b0.{Gw}  D2.t1.{D2w}  G.b0.{Gw}  S.b1.{Sw}  '
      else:                mintWirings += 'G.b0.{Gw}  D1.t0.{D1w}  G.b0.{Gw}  S.b1.{Sw}  '

    wirings = westWirings + mintWirings + eastWirings

    stack.setWirings( wirings.format( **diffMap ) )
    stack.doLayout  ( bbMode )

    paramsMatrix = paramsmatrix.ParamsMatrix()
    paramsMatrix.setGlobalTransistorParams( toUnity(stack.w)
                                          , toUnity(stack.L)
                                          , device.getM()
                                          , stack.boundingBox
                                          )
    paramsMatrix.setStacks( [ stack ] )
    trace( 100, '++' )
    paramsMatrix.trace()
    trace( 100, '---' )
    
    return paramsMatrix.getMatrix()
