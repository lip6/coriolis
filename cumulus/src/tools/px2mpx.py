#!/usr/bin/env python

try:
  import sys
  import traceback
  import os.path
  import optparse
  import math
  import Cfg
  import Hurricane
  from   Hurricane  import DataBase
  from   Hurricane  import DbU
  from   Hurricane  import Transformation
  from   Hurricane  import Box
  from   Hurricane  import UpdateSession
  from   Hurricane  import Breakpoint
  from   Hurricane  import Net
  from   Hurricane  import NetExternalComponents
  from   Hurricane  import BasicLayer
  from   Hurricane  import ContactLayer
  from   Hurricane  import ViaLayer
  from   Hurricane  import RegularLayer
  from   Hurricane  import TransistorLayer
  from   Hurricane  import DiffusionLayer
  from   Hurricane  import Cell
  from   Hurricane  import Instance
  from   Hurricane  import Net
  from   Hurricane  import Contact
  from   Hurricane  import Horizontal
  from   Hurricane  import Vertical
  import Viewer
  import CRL
  from   CRL        import RoutingLayerGauge
  import helpers
  from   helpers    import trace
  from   helpers.io import ErrorMessage
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


framework   = CRL.AllianceFramework.get()


def getDeltas ( layer ):
 #deltas = { 'L_METAL1'   : DbU.fromLambda( -1.0 )
 #         , 'L_METAL2'   : DbU.fromLambda(  3.0 )
 #         , 'L_blockage2': DbU.fromLambda( -1.0 )
 #         , 'L_blockage4': DbU.fromLambda( -1.0 )
 #         , 'L_xWell'    : DbU.fromLambda(  6.0 )
 #         , 'L_Trans'    : DbU.fromLambda( -3.0 )
 #         , 'L_Diff'     : DbU.fromLambda( -1.0 )
 #         , 'L_Tie'      : DbU.fromLambda(  1.2 )
 #         , 'W_Diff'     : DbU.fromLambda(  0.2 )
 #         , 'W_xWell'    : DbU.fromLambda( 12.0 )
 #         , 'mW_METAL1'  : DbU.fromLambda(  4.0 )
 #         , 'mW_METAL2'  : DbU.fromLambda(  4.0 )
 #         }
  deltas = { 'L_METAL1'   : DbU.fromLambda( -1.0 )
           , 'L_METAL2'   : DbU.fromLambda(  1.5 )
           , 'L_blockage2': DbU.fromLambda( -0.5 )
           , 'L_blockage4': DbU.fromLambda( -0.5 )
           , 'L_xWell'    : DbU.fromLambda(  6.0 )
           , 'L_Trans'    : DbU.fromLambda( -3.0 )
           , 'L_Diff'     : DbU.fromLambda( -1.0 )
           , 'L_Tie'      : DbU.fromLambda(  1.2 )
           , 'W_Diff'     : DbU.fromLambda(  0.2 )
           , 'W_xWell'    : DbU.fromLambda( 12.0 )
           , 'W_blockage2': DbU.fromLambda( -1.0 )
           , 'mW_METAL1'  : DbU.fromLambda(  4.0 )
           , 'mW_METAL2'  : DbU.fromLambda(  6.0 )
           }

  dL = 0
  dW = 0
  mW = 0

  if isinstance(layer,TransistorLayer):
    dL = deltas[ 'L_Trans'  ]
  elif isinstance(layer,RegularLayer):
    if   layer.getName() == 'METAL1':
      dL = deltas[  'L_METAL1' ]
      mW = deltas[ 'mW_METAL1' ]
    elif layer.getName() == 'METAL2' or layer.getName() == 'METAL3':
      dL = deltas[  'L_METAL2' ]
      mW = deltas[ 'mW_METAL2' ]
    elif layer.getName() == 'BLOCKAGE2':
      dL = deltas[  'L_blockage2' ]
    elif layer.getName() == 'BLOCKAGE4':
      dL = deltas[  'L_blockage4' ]
    elif layer.getName().endswith('WELL'):
      dL = deltas[ 'L_xWell'  ]
      dW = deltas[ 'W_xWell'  ]
  elif isinstance(layer,DiffusionLayer):
    if layer.getName().endswith('DIF'):
      dL = deltas[ 'L_Diff' ]
      dW = deltas[ 'W_Diff' ]
    elif layer.getName().endswith('TIE'):
      dL = deltas[ 'L_Tie'  ]
  return dL, dW, mW


def px2mpx ( editor, pxCell ):
  global framework

  if pxCell == None:
    raise ErrorMessage( 3, 'px2mpx.px2mpx(): Mandatory pxCell argument is None.' )
  mpxCell = None

  print '\nProcessing', pxCell

  UpdateSession.open()
  try:
    if pxCell.getName() != 'padreal':
      mpxCellName = pxCell.getName()[:-2]+'mpx'
    else:
      mpxCellName = pxCell.getName()+'_mpx'
    mpxCell = framework.createCell( mpxCellName )

    if editor:
      editor.setCell( mpxCell )

    Left    = 0x0001
    Right   = 0x0002
    Middle  = 0x0000
    AllSpan = Left|Right

    ab = pxCell.getAbutmentBox()
    mpxCell.setAbutmentBox( Box( ab.getXMin()*2, ab.getYMin()*2, ab.getXMax()*2, ab.getYMax()*2 ) )

    for instance in pxCell.getInstances():
      masterCell = instance.getMasterCell()
      if masterCell.getName() == 'padreal':
        masterCell = framework.getCell( 'padreal_mpx', CRL.Catalog.State.Physical )
      
      originTransf = instance.getTransformation()
      mpxInstance = Instance.create( mpxCell
                                   , instance.getName()
                                   , masterCell
                                   , Transformation( originTransf.getTx()*2
                                                   , originTransf.getTy()*2
                                                   , originTransf.getOrientation() )
                                   )
      mpxInstance.setPlacementStatus( Instance.PlacementStatus.PLACED )

    for net in pxCell.getNets():
      mpxNet = Net.create( mpxCell, net.getName() )
      if net.isExternal(): mpxNet.setExternal( True )
      if net.isGlobal  (): mpxNet.setGlobal( True )
      mpxNet.setType     ( net.getType     () )
      mpxNet.setDirection( net.getDirection() )

      for component in net.getComponents():
        layer        = component.getLayer()
        dupComponent = None

        print '  Processing', component

        if isinstance(component,Contact):
          dupComponent = Contact.create( mpxNet
                                       , layer
                                       , component.getX     ()*2
                                       , component.getY     ()*2
                                       , component.getWidth ()*2
                                       , component.getHeight()*2
                                       )
        elif isinstance(component,Horizontal):
          dL, dW, mW = getDeltas( layer )
          dLLeft        = dL
          dLRight       = dL
          skipComponent = False

          bb = component.getBoundingBox()
          if component.getSourceX() > component.getTargetX(): component.invert()
          if isinstance(layer,RegularLayer):
            if layer.getBasicLayer().getMaterial().getCode() == BasicLayer.Material.blockage:
              print '    Blockage BB:%s vs. AB:%s' % (bb, ab)
              if layer.getName()[-1] == '2' or layer.getName()[-1] == '4':
                state = 0
                if bb.getXMin() <= ab.getXMin(): state |= Left
                if bb.getXMax() >= ab.getXMax(): state |= Right

                if not (state&Left):
                  print '      Shrink left.'
                  dLLeft  = dL - DbU.fromLambda( 1.5 )
                if not(state&Right):
                  print '      Shrink right.'
                  dLRight = dL - DbU.fromLambda( 1.5 )

                if layer.getName()[-1] == '4' and state == AllSpan:
                  print '      Skipping component.'
                  skipComponent = True

          width = mW
          if component.getWidth() > mW:
            width = component.getWidth()*2 + dW

         #print DbU.toLambda(bb.getWidth()), DbU.toLambda( dLLeft-dLRight)
          if bb.getWidth()*2 > abs(dLLeft+dLRight) and not skipComponent:
            dupComponent = Horizontal.create( mpxNet
                                            , layer
                                            , component.getY       ()*2
                                            , width
                                            , component.getDxSource()*2 - dLLeft
                                            , component.getDxTarget()*2 + dLRight
                                            )
            print '    Copy:', dupComponent
          else:
            print '    Horizontal component too small *or* skipped, not converted'

        elif isinstance(component,Vertical):
          dL, dW, mW = getDeltas( component.getLayer() )
          dLTop         = dL
          dLBottom      = dL
          dX            = 0
          skipComponent = False

          if component.getSourceY() > component.getTargetY(): component.invert()
          if isinstance(layer,RegularLayer):
            if layer.getBasicLayer().getMaterial().getCode() == BasicLayer.Material.blockage:
              if layer.getName()[-1] == '3' or layer.getName()[-1] == '5':
                state = 0
                bb    = component.getBoundingBox()
                if bb.getXMin() <= ab.getXMin(): state |= Left
                if bb.getXMax() >= ab.getXMax(): state |= Right

                if state == Left:
                  dX  = DbU.fromLambda( -2.0 )
                  dW += DbU.fromLambda( -2.0 )
                elif state == Right:
                  dX  = DbU.fromLambda(  2.0 )
                  dW += DbU.fromLambda( -2.0 )
                elif state == 0:
                  dX  = 0
                  dW += DbU.fromLambda( -4.0 )

                if layer.getName()[-1] == '5':
                  if state == AllSpan:
                    print '      Skipping component.'
                    skipComponent = True
                  else:
                    dLTop = DbU.fromLambda(120.0) - component.getDyTarget()*2

          if dW < component.getWidth() and not skipComponent:
            width = mW
            if component.getWidth() > mW:
              width = component.getWidth()*2 + dW
  
            dupComponent = Vertical.create( mpxNet
                                          , layer
                                          , component.getX       ()*2 + dX
                                          , width
                                          , component.getDySource()*2 - dLBottom
                                          , component.getDyTarget()*2 + dLTop
                                          )
          else:
            print '    Vertical component too small *or* skipped, not converted'

        else:
          print '[WARNING] Unchanged component:', component

        if dupComponent and NetExternalComponents.isExternal( component ):
          NetExternalComponents.setExternal( dupComponent )
          
    if editor: editor.fit()

  except ErrorMessage, e:
      print e; errorCode = e.code
  except Exception, e:
      print '\n\n', e; errorCode = 1
      traceback.print_tb(sys.exc_info()[2])

  UpdateSession.close()
  return mpxCell


def scriptMain ( **kw ):
  global framework
  global padrealCell

  helpers.staticInitialization( quiet=True )
 #helpers.setTraceLevel( 550 )

  pxCell = None
  if kw.has_key('cell') and kw['cell']:
    pxCell = kw['cell']

  editor = None
  if kw.has_key('editor') and kw['editor']:
    editor = kw['editor']
    print '  o  Editor detected, running in graphic mode.'
    if pxCell == None: pxCell = editor.getCell()

  if pxCell:
    mpxCell = px2mpx( editor, pxCell )
  else:
    pxlibDir  = '/dsk/l1/jpc/alliance/Linux.slsoc6x/install/cells/pxlib'
    
    if os.path.isdir(pxlibDir):
      print '  o  <%s> found.' % pxlibDir
      for viewFile in os.listdir( pxlibDir ):
        if viewFile == 'padreal.ap':
          pxCell      = framework.getCell( viewFile[:-3], CRL.Catalog.State.Views )
          padrealCell = px2mpx( editor, pxCell )
          framework.saveCell( padrealCell, CRL.Catalog.State.Physical )
          break

      for viewFile in os.listdir( pxlibDir ):
        if viewFile == 'padreal.ap': continue
        if viewFile.endswith('_sp.ap'): continue
       #if viewFile != 'pot_px.ap': continue
        if viewFile.endswith('.ap'):
          pxCell = framework.getCell( viewFile[:-3], CRL.Catalog.State.Views )
          mpxCell = px2mpx( editor, pxCell )
          framework.saveCell( mpxCell, CRL.Catalog.State.Physical )
    else:
      print '[WARNING] <%s> not found.' % pxlibDir
      
  return 0


if __name__ == '__main__':
  parser = optparse.OptionParser()
  parser.add_option( '-c', '--cell', type='string',                      dest='cell'       , help='The name of the chip to build, without extension.')
  parser.add_option( '-v', '--verbose'            , action='store_true', dest='verbose'    , help='First level of verbosity.')
  parser.add_option( '-V', '--very-verbose'       , action='store_true', dest='veryVerbose', help='Second level of verbosity.')
  (options, args) = parser.parse_args()

  kw = {}
  if options.cell:
    kw['cell'] = framework.getCell( options.cell, CRL.Catalog.State.Logical )
  if options.verbose:     Cfg.getParamBool('misc.verboseLevel1').setBool(True)
  if options.veryVerbose: Cfg.getParamBool('misc.verboseLevel2').setBool(True)

  success = scriptMain( **kw )
  shellSuccess = 0
  if not success: shellSuccess = 1

  sys.exit( shellSuccess )
