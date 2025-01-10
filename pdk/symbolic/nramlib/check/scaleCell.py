#!/usr/bin/env python
# -*- coding: utf-8; explicit-buffer-name: "scaleCell.py<nramlib>" -*-

try:
  import sys
  import traceback
  import os.path
  import shutil
  import optparse
  import math
  import Cfg
  import Hurricane
  from   Hurricane import DataBase
  from   Hurricane import DbU
  from   Hurricane import Transformation
  from   Hurricane import Box
  from   Hurricane import UpdateSession
  from   Hurricane import Breakpoint
  from   Hurricane import Net
  from   Hurricane import NetExternalComponents
  from   Hurricane import BasicLayer
  from   Hurricane import ContactLayer
  from   Hurricane import ViaLayer
  from   Hurricane import RegularLayer
  from   Hurricane import TransistorLayer
  from   Hurricane import DiffusionLayer
  from   Hurricane import Cell
  from   Hurricane import Instance
  from   Hurricane import Net
  from   Hurricane import Contact
  from   Hurricane import Horizontal
  from   Hurricane import Vertical
  import Viewer
  import CRL
  from   CRL import RoutingLayerGauge
  import helpers
  from   helpers import trace
  from   helpers import ErrorMessage
except ImportError, e:
  serror = str(e)
  if serror.startswith('No module named'):
    module = serror.split()[-1]
    print '[ERROR] The <%s> python module or symbol cannot be loaded.' % module
    print '        Please check the integrity of the <coriolis> package.'
  if str(e).find('cannot open shared object file'):
    library = serror.split(':')[0]
    print '[ERROR] The <%s> shared library cannot be loaded.' % library
    print '        Under RHEL 6, you must be under devtoolset-2.'
    print '        (scl enable devtoolset-2 bash)'
  sys.exit(1)
except Exception, e:
  print '[ERROR] A strange exception occurred while loading the basic Coriolis/Python'
  print '        modules. Something may be wrong at Python/C API level.\n'
  print '        %s' % e
  sys.exit(2)


framework = CRL.AllianceFramework.get()
scale     = 2


def getDeltas ( layer ):
 #         |  Layer        | Min W | Delta W | Delta L |
 #         +---------------+-------+---------+---------+
  deltas = { 'NWELL'     : ( 999.0 ,    12.0 ,     6.0 )
           , 'PWELL'     : ( 999.0 ,    12.0 ,     6.0 )
           , 'NDIF'      : ( 999.0 ,     0.0 ,    -1.0 )
           , 'PDIF'      : ( 999.0 ,     0.0 ,    -1.0 )
           , 'NTIE'      : ( 999.0 ,     0.0 ,     1.2 )
           , 'PTIE'      : ( 999.0 ,     0.0 ,     1.2 )
           , 'NTRANS'    : ( 999.0 ,     0.0 ,    -3.0 )
           , 'PTRANS'    : ( 999.0 ,     0.0 ,    -3.0 )
           , 'POLY'      : ( 999.0 ,     0.0 ,     0.0 )
           , 'METAL1'    : (   1.5 ,     1.0 ,    -1.0 )
           , 'METAL2'    : ( 999.0 ,     2.0 ,     1.5 )
          #, 'METAL3'    : (   2.0 ,     2.0 ,     1.5 )
           , 'METAL3'    : ( 999.0 ,     0.0 ,     1.5 )
           , 'BLOCKAGE1' : ( 999.0 ,     0.0 ,     0.0 )
           , 'BLOCKAGE2' : ( 999.0 ,    -1.0 ,    -0.5 )
           , 'BLOCKAGE3' : ( 999.0 ,    -1.0 ,    -0.5 )
           , 'BLOCKAGE4' : ( 999.0 ,    -1.0 ,    -0.5 )
           }

  dbuLayerDeltas = ( 0, 0, 0 )
  if deltas.has_key(layer.getName()):
    deltas         = deltas[ layer.getName() ]
    dbuLayerDeltas = DbU.fromLambda(deltas[0]), DbU.fromLambda(deltas[1]), DbU.fromLambda(deltas[2]) 
  else:
    print '[WARNING] Layer \"%s\" has no deltas corrections.' % layer.getName()

  return dbuLayerDeltas


def scaleCell ( editor, sourceCell ):
  global framework
  global scale

  if sourceCell == None:
    raise ErrorMessage( 3, 'scaleCell.scaleCell(): Mandatory sourceCell argument is None.' )
  scaledCell = None

  print '\n  o  Processing', sourceCell

  UpdateSession.open()
  try:
    library    = framework.getLibrary( 0 )
    scaledCell = Cell.create( library, sourceCell.getName() )

    ab = sourceCell.getAbutmentBox()
    scaledCell.setAbutmentBox( Box( ab.getXMin()*scale, ab.getYMin()*scale
                                  , ab.getXMax()*scale, ab.getYMax()*scale ) )

    for net in sourceCell.getNets():
      scaledNet = Net.create( scaledCell, net.getName() )
      if net.isExternal(): scaledNet.setExternal( True )
      if net.isGlobal  (): scaledNet.setGlobal  ( True )
      scaledNet.setType     ( net.getType     () )
      scaledNet.setDirection( net.getDirection() )

      for component in net.getComponents():
        layer        = component.getLayer()
        dupComponent = None

        if isinstance(component,Contact):
         # Default VIAs get their size through the parser. So they
         # are already scaled (to the minimal size).
          dupComponent = Contact.create( scaledNet
                                       , layer
                                       , component.getX     ()*scale
                                       , component.getY     ()*scale
                                       , component.getWidth () # *scale
                                       , component.getHeight() # *scale
                                       )
          print '     |', dupComponent
        elif isinstance(component,Horizontal):
          mW, dW, dL = getDeltas( layer )

          if     component.getLayer().getName() == 'METAL1' \
             and component.getWidth() == DbU.fromLambda(2.0):
            print '[INFO] Shrinking METAL1 of 2l width to default contact width.'
            print '       %s' % component
            mW = DbU.fromLambda( 2.0)
            dW = DbU.fromLambda(-1.0)

          if     component.getLayer().getName() == 'METAL3' \
             and component.getSourceX() == component.getTargetX():
            print '[WARNING] Rotating badly oriented METAL3 terminal (H -> V).'
            print '          %s' % component

            width = component.getWidth()*scale
           #if component.getWidth() <= mW: width += dW

            dupComponent = Vertical.create( scaledNet
                                          , layer
                                          , component.getSourceX()*scale
                                          , width
                                          , component.getY()*scale - dL
                                          , component.getY()*scale + dL
                                          )
            print '     |', dupComponent

          else:
            if component.getSourceX() > component.getTargetX(): component.invert()
            bb = component.getBoundingBox()
            
            width = component.getWidth()*scale
            if component.getWidth() <= mW: width += dW
            
            if width > 0 and ( (dL > 0) or (bb.getWidth()*scale > abs(2*dL)) ):
              dupComponent = Horizontal.create( scaledNet
                                              , layer
                                              , component.getY       ()*scale
                                              , width
                                              , component.getDxSource()*scale - dL
                                              , component.getDxTarget()*scale + dL
                                              )
              print '     |', dupComponent
            else:
              print '[WARNING] Horizontal component too small *or* skipped, not converted.'

        elif isinstance(component,Vertical):
          custom_dX = 0
          if sourceCell.getName() == 'ram_sense_decad12':
            if component.getLayer().getName() == 'NDIF':
              if     component.getSourceY() == DbU.fromLambda(  3.0) \
                 and component.getTargetY() == DbU.fromLambda( 12.0) \
                 and component.getX()       == DbU.fromLambda(153.0) \
                 and component.getWidth()   == DbU.fromLambda(  5.0):
                print '[INFO] Shifted left NDIF segment %s.' % component
                custom_dX = DbU.fromLambda(-1.0)
            if component.getLayer().getName() == 'PDIF':
              if     component.getSourceY() == DbU.fromLambda( 28.0) \
                 and component.getTargetY() == DbU.fromLambda( 47.0) \
                 and component.getX()       == DbU.fromLambda(153.0) \
                 and component.getWidth()   == DbU.fromLambda(  5.0):
                print '[INFO] Shifted left PDIF segment %s.' % component
                custom_dX = DbU.fromLambda(-1.0)

          mW, dW, dL = getDeltas( component.getLayer() )

          if     component.getLayer().getName() == 'METAL1' \
             and component.getWidth() == DbU.fromLambda(2.0):
            print '[INFO] Shrinking METAL1 of 2l width to default contact width.'
            print '       %s' % component
            mW = DbU.fromLambda( 2.0)
            dW = DbU.fromLambda(-1.0)

          if component.getSourceY() > component.getTargetY(): component.invert()
          bb = component.getBoundingBox()

          width = component.getWidth()*scale
          if component.getWidth() <= mW: width += dW

          if width > 0 and ( (dL > 0) or (bb.getHeight()*scale > abs(2*dL)) ):
            dupComponent = Vertical.create( scaledNet
                                          , layer
                                          , component.getX       ()*scale + custom_dX
                                          , width
                                          , component.getDySource()*scale - dL
                                          , component.getDyTarget()*scale + dL
                                          )
            print '     |', dupComponent
          else:
            print '[WARNING] Vertical component too small *or* skipped, not converted.'

        else:
          print '[WARNING] Unchanged component:', component

        if dupComponent and NetExternalComponents.isExternal( component ):
          NetExternalComponents.setExternal( dupComponent )

  except ErrorMessage, e:
      print e; errorCode = e.code
  except Exception, e:
      print '\n\n', e; errorCode = 1
      traceback.print_tb(sys.exc_info()[2])

  UpdateSession.close()
  if scaledCell:
    framework.saveCell( scaledCell, CRL.Catalog.State.Physical )
    if editor:
      editor.setCell( scaledCell )
      editor.fit()

  return scaledCell


def scriptMain ( **kw ):
  global framework

  helpers.staticInitialization( quiet=True )
 #helpers.setTraceLevel( 550 )

  scaledDir = framework.getAllianceLibrary(0).getPath()
  alibrary  = framework.getAllianceLibrary(1)
  if not alibrary:
    print '[ERROR] No Library at index 1, please check SYSTEM_LIBRARY in settings.py.'
    return 1

  hasCatal = False
  apCount  = 0
  vbeCount = 0
  for file in os.listdir( scaledDir ):
    if file      == 'CATAL': hasCatal = True
    if file[-4:] ==  '.vbe': vbeCount += 1
    if file[-3:] ==   '.ap': apCount  += 1

  if hasCatal or vbeCount or apCount:
    print '[ERROR] Target directory already contains CATAL/.vbe/.ap files.'
    print '        You must remove them before proceeding with this script.'
    print '        (%s)' % scaledDir
    return 1

  sourceCell = None
  if kw.has_key('cell') and kw['cell']:
    sourceCell = kw['cell']

  editor = None
  if kw.has_key('editor') and kw['editor']:
    editor = kw['editor']
    print '  o  Editor detected, running in graphic mode.'
    if sourceCell == None: sourceCell = editor.getCell()

  if sourceCell:
    scaledCell = scaleCell( editor, sourceCell )
  else:
    print '  o  Processing library "%s".' % alibrary.getLibrary().getName() 
    print '     (path:%s)'                % alibrary.getPath()
    framework.loadLibraryCells( alibrary.getLibrary() )
    for sourceCell in alibrary.getLibrary().getCells():
      scaledCell = scaleCell( editor, sourceCell )

    print ''
    print '  o  Direct copy of ".vbe" & CATAL files.'
    for file in os.listdir( alibrary.getPath() ):
      if file == 'CATAL' or file[-4:] == '.vbe':
        print '     | %s' % file
        shutil.copy( alibrary.getPath()+'/'+file, scaledDir )
      
  return 0


if __name__ == '__main__':
  parser = optparse.OptionParser()
  parser.add_option( '-c', '--cell', type='string',                      dest='cell'       , help='The name of the chip to build, whithout extension.')
  parser.add_option( '-v', '--verbose'            , action='store_true', dest='verbose'    , help='First level of verbosity.')
  parser.add_option( '-V', '--very-verbose'       , action='store_true', dest='veryVerbose', help='Second level of verbosity.')
  (options, args) = parser.parse_args()

  kw = {}
  if options.cell:
    kw['cell'] = framework.getCell( options.cell, CRL.Catalog.State.Views )
  if options.verbose:     Cfg.getParamBool('misc.verboseLevel1').setBool(True)
  if options.veryVerbose: Cfg.getParamBool('misc.verboseLevel2').setBool(True)

  print framework.getEnvironment().getPrint()

  success = scriptMain( **kw )
  shellSuccess = 0
  if not success: shellSuccess = 1

  sys.exit( shellSuccess )
