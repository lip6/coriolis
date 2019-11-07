# -*- coding: utf-8 -*-

from   Hurricane import DataBase
from   Hurricane import UpdateSession
from   Hurricane import DbU
from   Hurricane import Box
import helpers
import helpers.io
from   helpers   import trace

helpers.setTraceLevel( 1000 )

import Analog
import ParamsMatrix
import oroshi
from   CapacitorUnit     import CapacitorUnit
from   CapacitorMatrix   import CapacitorStack
from   CapacitorVRTracks import VerticalRoutingTracks
from   CapacitorRouted   import RoutMatchedCapacitor


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
    message = 'CapacitorMatrix.checkCoherency():\n'
    
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
        print '    [',
        for column in range(columns):
          capaIndex =  pmatrix.getValue(row,column)
          print '%2d' % capaIndex,

          if capaIndex >= capacities.getCount():
            valid    = False
            message += '        element [%d,%d] == %d is out of range, must be in [0..%d]\n' \
                       % (row,column,capaIndex,capacities.getCount()-1)
        print ']'

    if not valid: return (False, message)
    
    return (True, "")


def layout ( device, bbMode ):

    trace( 100, ',+', '\tCapacitorMatrix.layout() called.\n' )

    paramsMatrix = ParamsMatrix.ParamsMatrix()

    try:
      capacities = device.getParameter( 'capacities' )
      print str(device)
      print 'Capacities = ', capacities

      capValuesArg  = []
      vTrackNetsArg = []
      for i in range(capacities.getCount()):
        capValuesArg .append( capacities.getValue(i) )
        vTrackNetsArg.append( [ device.getNet('t%d'%i), device.getNet("b%d"%i) ] )

      matrixSizeArg, matchingSchemeArg = toMatrixArgs( device.getParameter('matrix') )

      typeArg = 'UnknownType'
      if device.isPIP(): typeArg = 'PIPCap'
      if device.isMIM(): typeArg = 'MIMCap'
      if device.isMOM(): typeArg = 'MOMCap'

      capaGenerator = CapacitorStack( device
                                    , capValuesArg
                                    , typeArg
                                    , matrixSizeArg
                                    , vTrackNetsArg
                                    , matrixDim     =matrixSizeArg
                                    , matchingMode  =True
                                    , matchingScheme=matchingSchemeArg
                                    , dummyRing     =False
                                    )
      capaMatrix = capaGenerator.create()
      capaTracks = VerticalRoutingTracks( capaGenerator, capaMatrix, True )
      capaTracks.create()
      
      capaRouted = RoutMatchedCapacitor( capaTracks )
      capaRouted.route()

      paramsMatrix.setGlobalCapacitorParams( device.getAbutmentBox() )
      trace( 100, '++' )
     #paramsMatrix.trace()

    except Exception, e:
      helpers.io.catch( e )

    trace( 100, '---' )
    
    return paramsMatrix.getMatrix()
