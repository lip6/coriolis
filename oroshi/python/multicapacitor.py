# -*- coding: utf-8 -*-

from   Hurricane import DataBase
from   Hurricane import UpdateSession
from   Hurricane import DbU
from   Hurricane import Box
from   Hurricane import Net
import helpers
import helpers.io
from   helpers   import trace

#helpers.setTraceLevel( 100 )

import Analog
from   Analog import Device
import oroshi
import oroshi.paramsmatrix
from   oroshi.capacitorunit         import CapacitorUnit
from   oroshi.capacitormatrix       import CapacitorStack
from   oroshi.capacitorvrtracks     import VerticalRoutingTracks
from   oroshi.capacitorrouted       import RoutMatchedCapacitor
from   oroshi.capacitorroutedsingle import RouteCapacitorSingle


def toMatrixArgs ( matrix ):
    valid   = True
    rows    = matrix.getRows   ()
    columns = matrix.getColumns()
    lol     = []

    for row in range(rows):
        lol.append( [] )
        for column in range(columns):
            lol[-1].append( matrix.getValue(row,column) )

    return [rows,columns], lol



def checkCoherency ( device, bbMode ):
    message = 'CapacitorMatrix.checkCoherency(): device "%s".\n' % device.getName()
    
    techno = DataBase.getDB().getTechnology()
    rules  = oroshi.getRules()

    capacities = device.getParameter( 'capacities' )
    if capacities is None:
      message += '        Missing "capacities" parameter on %s' % str(device)
      return (False, message)
      
    pmatrix = device.getParameter( 'matrix' )
    if pmatrix is None:
      message += '        Missing "matrix" parameter on %s' % str(device)
      return (False, message)
      
    valid = True
    if pmatrix:
      rows    = pmatrix.getRows   ()
      columns = pmatrix.getColumns()

      for row in range(rows):
       #print '    [',
        for column in range(columns):
          capaIndex =  pmatrix.getValue(row,column)
         #print '%2d' % capaIndex,

          if capaIndex >= capacities.getCount():
            valid    = False
            message += '        element [%d,%d] == %d is out of range, must be in [0..%d]\n' \
                       % (row,column,capaIndex,capacities.getCount()-1)
       #print ']'

    if not valid: return (False, message)
    
    return (True, "")


def addDummyNets ( device ):
    if not device.hasDummy(): return
    
    i       = device.getParameter( 'capacities' ).getCount()
    topName = 't%d'%i
    botName = 'b%d'%i

    topNet  = device.getNet( topName )
    if not topNet:
      topNet = Net.create( device, topName )
      topNet.setExternal( True )

    botNet  = device.getNet( botName )
    if not botNet:
      botNet = Net.create( device, botName )
      botNet.setExternal( True )
    return


def layout ( device, bbMode ):

    trace( 100, ',+', '\tMultiCapacitor.layout() called for "%s".\n' % device.getName())

    paramsMatrix = oroshi.paramsmatrix.ParamsMatrix()

    try:
        capacities    = device.getParameter( 'capacities' )
        dummyArg      = device.hasDummy()
        capaValuesArg = []
        vTrackNetsArg = []
        
        addDummyNets( device )
        
        for i in range(capacities.getCount()):
            capaValuesArg.append( capacities.getValue(i)*1000.0 )
            vTrackNetsArg.append( [ device.getNet('t%d'%i), device.getNet("b%d"%i) ] )
        
        if dummyArg:
            i = device.getParameter( 'capacities' ).getCount()
            vTrackNetsArg.append( [ device.getNet('t%d'%i), device.getNet("b%d"%i) ] )
        
        matrixSizeArg, matchingSchemeArg = toMatrixArgs( device.getParameter('matrix') )
        
        typeArg = 'UnknownType'
        if device.isPIP(): typeArg = 'PIPCap'
        if device.isMIM(): typeArg = 'MIMCap'
        if device.isMOM(): typeArg = 'MOMCap'
        
        if len(capaValuesArg) == 1:
         # One capa, multiple units.
            capaGenerator = CapacitorStack( device
                                          , capaValuesArg # (fF).
                                          , typeArg
                                          , [0,0]         # AB position.
                                          , vTrackNetsArg
                                          , matrixDim     =matrixSizeArg
                                          , dummyRing     =dummyArg
                                          )
            capaMatrix = capaGenerator.create()
            capaSingle = RouteCapacitorSingle( capaGenerator
                                             , capaMatrix
                                             ,    topPlateWSpec=[1,0]
                                             , bottomPlateWSpec=[0,1] )
            capaSingle.route()
        else:
           # Two capa, multiple units.
            capaGenerator = CapacitorStack( device
                                          , capaValuesArg # [ 240, 720 ]  # capaValuesArg (fF).
                                          , typeArg
                                          , [0,0]         # AB position.
                                          , vTrackNetsArg
                                          , matrixDim     =matrixSizeArg
                                          , matchingMode  =True
                                          , matchingScheme=matchingSchemeArg
                                          , dummyRing     =dummyArg
                                          )
            capaMatrix = capaGenerator.create()
            capaTracks = VerticalRoutingTracks( capaGenerator, capaMatrix, True )
            capaTracks.create()
          
            capaRouted = RoutMatchedCapacitor( capaTracks )
            capaRouted.route()
        
        for pair in vTrackNetsArg:
            for net in pair:
                device.setRestrictions( net, Device.NorthBlocked|Device.SouthBlocked )
        
        paramsMatrix.setGlobalCapacitorParams( device.getAbutmentBox() )
        trace( 100, '++' )
       #paramsMatrix.trace()

    except Exception as e:
        helpers.io.catch( e )

    trace( 100, '---' )
    
    return paramsMatrix.getMatrix()
