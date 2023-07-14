# -*- coding: utf-8 -*-

from ..Hurricane  import DataBase, UpdateSession, DbU, Box
from ..helpers.io import catch
from ..helpers    import trace

#helpers.setTraceLevel( 1000 )

try:
    from ..     import Analog
    from .      import getRules, toUnity
    from .      import paramsmatrix
    from .stack import Stack
except Exception as e:
    catch( e )


def checkCoherency ( device, bbMode ):
    try:
        message = '[ERROR] Transistor.checkCoherency():\n'
        
        techno = DataBase.getDB().getTechnology()
        rules  = getRules()
        
        w    = device.getParameter( 'W' ).getValue()
        M    = device.getParameter( 'M' ).getValue()
        mMax = w // rules.transistorMinW
        if M > mMax:
            message += \
              '        W/M ratio must be greater than transistor minimal width (%s)\n' \
              '        Please increase W or decrease M.' \
              % DbU.getValueString(rules.transistorMinW)
            return (False, message)

    except Exception as e:
        catch( e )
        return (False, message)
    
    return (True, "")


def layout ( device, bbMode ):

    trace( 100, ',+', '\tWIP_Transistor.layout() called.\n' )

    paramsMatrix = paramsmatrix.ParamsMatrix()

    try:
      nerc = device.getParameter( 'NERC' ).getValue()
      nirc = device.getParameter( 'NIRC' ).getValue()
      
      stack = Stack( device, nerc, nirc )
      
      bw  = str(device.getParameter( 'B.w' ).getValue())
      dw  = str(device.getParameter( 'D.w' ).getValue())
      gw  = str(device.getParameter( 'G.w' ).getValue())
      sw  = str(device.getParameter( 'S.w' ).getValue())
      
      bt  = device.getParameter( 'B.t' ).getValue()
      dt  = device.getParameter( 'D.t' ).getValue()
      gt  = device.getParameter( 'G.t' ).getValue()
      st  = device.getParameter( 'S.t' ).getValue()
      
      diffMap = { 'D':'D', 'S':'S'
                , 'Dw':dw, 'Sw':sw, 'Gw':gw, 'Bw':bw
                , 'Dt':dt, 'St':st, 'Gt':gt, 'Bt':bt
                }
      if device.isSourceFirst():
        trace( 100, '\tUse sourceFirst.\n' )
        diffMap = { 'D':'S', 'S':'D'
                  , 'Dw':sw, 'Sw':dw, 'Gw':gw, 'Bw':bw
                  , 'Dt':dt, 'St':st, 'Gt':gt, 'Bt':bt
                  }
      
      wirings = '{D}.{Dt}.{Dw}  G.{Gt}.{Gw}  {S}.{St}.{Sw}'
      for i in range(device.getM() - 1):
        if i%2: wirings += ' G.{Gt}.{Gw} {S}.{St}.{Sw}'
        else:   wirings += ' G.{Gt}.{Gw} {D}.{Dt}.{Dw}'
      for i in range(device.getExternalDummy()):
        wirings  = 'B.{Bt}.{Bw}  B.{Bt}.{Bw}  ' + wirings
        wirings += '  B.{Bt}.{Bw}  B.{Bt}.{Bw}'
      
      stack.setWirings( wirings.format( **diffMap ) )
      stack.doLayout  ( bbMode )
      
      paramsMatrix.setGlobalTransistorParams( toUnity(stack.w)
                                            , toUnity(stack.L)
                                            , device.getM()
                                            , stack.boundingBox
                                            )
      paramsMatrix.setStacks( [ stack ] )
      trace( 100, '++' )
      paramsMatrix.trace()

    except Exception as e:
      catch( e )

    trace( 100, '---' )
    
    return paramsMatrix.getMatrix()
