# -*- coding: utf-8 -*-

from ..Hurricane  import DataBase, DbU, Box, Net
from ..helpers.io import ErrorMessage as Error
from ..helpers    import trace


# Rows are stacks.
# Columns are metaTransistors.
#
# There must be the exact same number of metaTransistors in each stack.
# Storage is matrix[row][col] ie matrix[stack][metaTransistor]

NoFlags = 0x0000
Top     = 0x0001
Left    = 0x0002

class ParamsMatrix ( object ):

  @staticmethod
  def gateToId ( gateName ):
    if len(gateName) == 1: return None
    return int( gateName[1:] )

  @staticmethod
  def idToGate ( index ):
    if index is None: return 'G'
    return 'G'+str(index)

  @staticmethod
  def idToTran ( index ):
    if index is None: return 'T'
    return 'T'+str(index)


  def __init__ ( self ):
    self.matrix  = [ [ {} ] ]
    self.rows    = 1
    self.columns = 1
    return


  def getMatrix ( self ): return self.matrix


  def setGlobalTransistorParams ( self, w, L, M, boundingBox ):
    self.matrix[0][0]['L'               ] = L
    self.matrix[0][0]['W'               ] = w
    self.matrix[0][0]['M'               ] = M
    self.matrix[0][0]['box'             ] = boundingBox
    self.matrix[0][0]['globalActiveBox' ] = Box()
    self.matrix[0][0]['family'          ] = 'Transistor'
    return


  def setGlobalCapacitorParams ( self, boundingBox ):
    self.matrix[0][0]['box'             ] = boundingBox
    self.matrix[0][0]['family'          ] = 'Capacitor'
    return


  def setGlobalResistorParams ( self, boundingBox ):
    self.matrix[0][0]['box'             ] = boundingBox
    self.matrix[0][0]['family'          ] = 'Resistor'
    return


  def setStacks ( self, stacks ):
    if not isinstance(stacks,list):
      print( Error( 3, 'ParamsMatrix::setGlobalParams(): <stack> argument must be of <list> type.' ))
      return

    if not len(stacks):
      print( Error( 3, 'ParamsMatrix::setGlobalParams(): There must be at least one Stack.' ))
      return

    mtIds = []
    if len(stacks[0].metaTransistors) == 0:
      print( Error( 3, 'ParamsMatrix::setGlobalParams(): Stack without any meta-transistor.' ))
      return
    else:
      for gateName in stacks[0].metaTransistors.keys():
        mtIds.append( ParamsMatrix.gateToId(gateName) )
    mtIds.sort() 

    for i in range(1,len(stacks)):
      if len(stacks[0].metaTransistors) != len(stacks[i].metaTransistors):
        print( Error( 3, 'ParamsMatrix::setGlobalParams(): Stacks {} and {}' \
                         ' have different numbers of meta-transistor.'.format(0,i) ))
        return

      for id in mtIds:
        gateName = ParamsMatrix.idToGate( id )
        if not gateName in stacks[i].metaTransistors:
          print( Error( 3, 'ParamsMatrix::setGlobalParams(): Stack {} ' \
                           ' is missing meta-transistor "{}".'.format(i,gateName) ))
          return

   # Thoses parameters must be the same in all the stacks.
    self.matrix[0][0]['DMCG'] = stacks[0].DMCG
    self.matrix[0][0]['DMCI'] = stacks[0].DMCI
    self.matrix[0][0]['DGG' ] = stacks[0].DGG
    self.matrix[0][0]['DGI' ] = stacks[0].DGI

    for iStack in range(len(stacks)):
      activeBox = stacks[iStack].activeBox
      stackMTs  = [ { 'activeBox':activeBox } ]
      self.matrix[0][0]['globalActiveBox'].merge( activeBox )

      for iMT in range(len(mtIds)):
        gateName = ParamsMatrix.idToGate( mtIds[iMT] )
        tranName = ParamsMatrix.idToTran( mtIds[iMT] )
        mt       = stacks[iStack].metaTransistors[ gateName ]

        stackMTs.append( { 'name':tranName } )
        for key, value in mt.items():
          stackMTs[-1][ key.split('.')[-1] ] = value

        if iStack == 0:
          self.matrix[0].append( { 'name':tranName } )
          for key in stackMTs[-1].keys():
            if key == 'name' or key == 'style.geomod': continue
            if key.startswith('stress.'):
              self.matrix[0][-1][key] = stackMTs[-1][key] / len(stacks)
            else:
              self.matrix[0][-1][key] = stackMTs[-1][key]
        else:
          for key in stackMTs[-1].keys():
            if key == 'name' or key == 'style.geomod': continue
            if key.startswith('stress.'):
              self.matrix[0][-1][key] += stackMTs[-1][key] / len(stacks)
            else:
              self.matrix[0][-1][key] += stackMTs[-1][key]

      self.matrix.append( stackMTs )

    return


  def stackHeader ( self, lines, iStack ):
    for i in range(28):
      if i in [1, 2, 3, 4]:
        if i == 1:
            lines.append( '+-----------------------+' )
            self.toBoxDatasheet( lines, self.matrix[iStack][0]['activeBox'], 'Active Box', Left )
        continue

      if   i ==  0: lines.append( '|{0:^23s}'.format( 'Stack %d'%iStack ) )
      elif i == 27: lines.append( '+-----------------------' )
      else:         lines.append( '|                       ' )

      if i in [1, 14, 21, 27]: lines[-1] += '+'
      else:                    lines[-1] += '|'
    return


  def toMtDatasheet ( self, lines, mt, comment, flags ):
    if flags & Top: lines[-29] += '-----------------------+'

    lines[-28] += '{0:^23s}|'.format( mt['name']+comment )
    lines[-27] += '-----------------------+'
    lines[-26] += '         BSIM4         |'
    lines[-25] += ' NSint     %7s     |'   % mt['NSint']
    lines[-24] += ' NDint     %7s     |'   % mt['NDint']
    lines[-23] += ' NSend     %7s     |'   % mt['NSend']
    lines[-22] += ' NDend     %7s     |'   % mt['NDend']
    lines[-21] += ' SAeff     %7.2g   m |' % mt['SAeff_Bsim4']
    lines[-20] += ' SBeff     %7.2g   m |' % mt['SBeff_Bsim4']
    lines[-19] += ' SAinv     %7.2g 1/m |' % mt['SBinv_Bsim4']
    lines[-18] += ' SBinv     %7.2g 1/m |' % mt['SBinv_Bsim4']
    lines[-17] += ' alpha     %7.2g   m |' % mt['alpha'      ]
    lines[-16] += ' alphaInv  %7.2g 1/m |' % mt['alphaInv'   ]
    lines[-15] += ' LODeffect %7.2g 1/m |' % mt['alphaInv'   ]
    lines[-14] += '-----------------------+'
    lines[-13] += '        Crolles        |'
    lines[-12] += ' SAeff     %7.2g   m |' % mt['SAeff_Crolles']
    lines[-11] += ' SBeff     %7.2g   m |' % mt['SBeff_Crolles']
    lines[-10] += ' SAinv     %7.2g 1/m |' % mt['SBinv_Crolles']
    lines[- 9] += ' SBinv     %7.2g 1/m |' % mt['SBinv_Crolles']
    lines[- 8] += ' po2actEff %7.2g   m |' % mt['po2actEff_Crolles']
    lines[- 7] += '-----------------------+'
    lines[- 6] += '   Layout Parasitics   |'
    lines[- 5] += ' ASeff %11.2g m2  |' % mt['ASeff']
    lines[- 4] += ' PSeff %11.2g m   |' % mt['PSeff']
    lines[- 3] += ' ADeff %11.2g m2  |' % mt['ADeff']
    lines[- 2] += ' PDeff %11.2g m   |' % mt['PDeff']
    lines[- 1] += '-----------------------+'
    return


  def toBoxDatasheet ( self, lines, box, comment, flags ):
    if flags & Left:
      for i in range(3): lines.append( '|' )

    lines[-3] += '{0:^23s}|'.format( comment )
    lines[-2] += ' bl  %8s %8s |' % (DbU.getValueString(box.getXMin()), DbU.getValueString(box.getYMin()))
    lines[-1] += ' tr  %8s %8s |' % (DbU.getValueString(box.getXMax()), DbU.getValueString(box.getYMax()))
    return


  def toDatasheet ( self ):
    lines = []
   # Matrix element [0,0]
    lines.append( '+-----------------------+' )
    lines.append( '|     Device Datas      |' )
    lines.append( '+-----------------------+' )
    lines.append( '| L    %14.2g m |' % self.matrix[0][0]['L'] )
    lines.append( '| wF   %14.2g m |' % self.matrix[0][0]['W'] )
    lines.append( '| M    %14.2g   |' % self.matrix[0][0]['M'] )
    lines.append( '| DMCG %14.2g m |' % self.matrix[0][0]['DMCG'] )
    lines.append( '| DMCI %14.2g m |' % self.matrix[0][0]['DMCI'] )
    lines.append( '| DGG  %14.2g m |' % self.matrix[0][0]['DGG'] )
    lines.append( '| DGI  %14.2g m |' % self.matrix[0][0]['DGI'] )
    lines.append( '+-----------------------+' )
    self.toBoxDatasheet( lines, self.matrix[0][0]['box'], 'Bounding Box', Left )
    lines.append( '+-----------------------+' )
    self.toBoxDatasheet( lines, self.matrix[0][0]['globalActiveBox'], 'Active Box', Left )
    lines[-3] = lines[-3][0:-1] + '+'
    for i in range(len(lines),28):
      lines.append( '|                       ' )
      if i in [0, 2, 15, 22, 28]: lines[-1] += '+'
      else:                       lines[-1] += '|'
    lines.append( '+-----------------------+' )

   # Row 0: Meta-Transistors aggregated parameters.
    mts = self.matrix[0]
    for iMT in range(1,len(mts)):
      self.toMtDatasheet( lines, mts[iMT], ' (meta)', Top )

   # Rows 1+: One stack and its's meta-transistors.
    for iStack in range(1,len(self.matrix)):
      self.stackHeader( lines, iStack )
      for iMT in range(1,len(self.matrix[iStack])):
        self.toMtDatasheet( lines, self.matrix[iStack][iMT], '', NoFlags )

    return lines


  def __str__ ( self ):
    s = ''
    for line in self.toDatasheet(): s += line + '\n'
    return s


  def trace( self ):
    for line in self.toDatasheet():
      trace( 100, '\t%s\n' % line )
    return
