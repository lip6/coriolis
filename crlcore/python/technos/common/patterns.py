
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Universit 2019-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./etc/common/patterns.py"                 |
# +-----------------------------------------------------------------+


import sys
import math
from   coriolis.helpers    import irange
from   coriolis.helpers.io import ErrorMessage, WarningMessage


class Pattern ( object ):

    hexaToPatternLUT = { '0':'    '
                       , '1':'   X'
                       , '2':'  X '
                       , '3':'  XX'
                       , '4':' X  '
                       , '5':' X X'
                       , '6':' XX '
                       , '7':' XXX'
                       , '8':'X   '
                       , '9':'X  X'
                       , 'a':'X X '
                       , 'b':'X XX'
                       , 'c':'XX  '
                       , 'd':'XX X'
                       , 'e':'XXX '
                       , 'f':'XXXX'}

    def __init__ ( self, name='noname', bits=[], hexa='' ):
        self._name = name
        self._bits = []
        self._hexa = ''
        self._side = 0
    
        if bits: self._setFromBits(bits)
        if hexa: self._setFromHexa(hexa)
        return

    def empty ( self ):
        if not self._bits and not self._hexa: return True
        return False

    def _setName ( self, name ): self._name = name
    def _getName ( self ): return self._name

    def _toHexa ( self ):
        hexasLSB = []
        for line in self._bits:
            byte = 0
            for i in range(len(line)):
                if i != 0 and i%4 == 0:
                    hexasLSB  += [ hex(byte)[2:] ]
                    byte  = 0
                byte = byte << 1
                if line[i] != ' ': byte += 1
            hexasLSB += [ hex(byte)[2:] ]
       # Convert in MSB mode. Invert the bytes by pairs.
        self._hexa = ''
        for i in irange(len(hexasLSB)/2):
            self._hexa += hexasLSB[i*2+1] + hexasLSB[i*2]
        return self._hexa

    def _fromHexa ( self ):
        self._bits = []
    
        side = math.sqrt(4*len(self._hexa))
        if pow(side,2) != 4*len(self._hexa):
            print( '[ERROR] The pattern is not square ({} self._bits).'.format(4*len(self._hexa)))
            return None
    
        side /= 4

       # Convert from MSB mode. Invert the bytes by pairs.
        hexasLSB = ''
        for i in range(len(self._hexa)/2):
            hexasLSB += self._hexa[i*2+1] + self._hexa[i*2]
        
        line = ''
        for i in range(len(hexasLSB)):
            if i != 0 and i%side == 0:
                self._bits += [ line ]
                line = ''
            line += Pattern.hexaToPatternLUT[hexasLSB[i].lower()]
        self._bits += [ line ]
       #self._bits.reverse()
        return self._bits

    def _setFromHexa ( self, hexa ):
        self._hexa = hexa
        self._side = math.sqrt(4*len(self._hexa))

        if pow(self._side,2) != 4*len(self._hexa):
            raise ErrorMessage( 1, 'The pattern is not square (%d bits).'
                                   % (4*len(self._hexa)) )
            return

        self._fromHexa()
        return

    def _setFromBits ( self, bits ):
        self._bits = bits
        self._side = len(bits)

        for line in bits:
            if self._side != len(line):
                raise ErrorMessage( 1, 'The pattern is not square (%dx%d bits).'
                                       % (len(line),len(self._bits)) )

        self._toHexa()
        return

    def _getHexa ( self ): return self._hexa
    def _getBits ( self ): return self._bits

    def printBits ( self ):
        s    = ''
        side = len(self._bits[0])
    
        s += '+%s+\n' % ('-'*side)
        for line in self._bits:
            s += '|%s|\n' % line
        s += '+%s+' % ('-'*side)
        return s

    def printHexa ( self ):
        return self._hexa

    def __str__ ( self ):
        return self._hexa

    hexa = property(_getName,_setName)
    hexa = property(_getHexa,_setFromHexa)
    bits = property(_getBits,_setFromBits)


LUT = {}


def add ( **keywords ):
    global LUT

    try:
      if not 'name' in keywords:
        raise ErrorMessage(1,['patterns.add(): Malformed pattern, missing "name" argument.', str(keywords) ])

      if ('bits' in keywords) and ('hexa' in keywords):
        w = WarningMessage( 'patterns.add(): Pattern "%s" has both bits & hexa, ignoring hexa.' % keywords['name'] )
        print( w )
        del keywords['hexa']

      LUT[ keywords['name'] ] = Pattern( **keywords ) 

    except Exception as e:
      helpers.io.catch( e )

    return


def toHexa ( key ):
    global LUT

    if isinstance(key,int) or not (key in LUT): return key
    return LUT[key].hexa


add( name='crux.8'
   , bits=[ '        '
          , '   X    '
          , '   X    '
          , ' XXXXX  '
          , '   X    '
          , '   X    '
          , '        '
          , '        ' ] )

add( name='slash.8'
   , bits=[ '   X   X'
          , '  X   X '
          , ' X   X  '
          , 'X   X   '
          , '   X   X'
          , '  X   X '
          , ' X   X  '
          , 'X   X   ' ] )

add( name='hash.8'
   , bits=[ 'XXX XXX '
          , 'XX XXX X'
          , 'X XXX XX'
          , ' XXX XXX'
          , 'XXX XXX '
          , 'XX XXX X'
          , 'X XXX XX'
          , ' XXX XXX' ] )

add( name='urgo.8'
   , bits=[ 'XXX XXXX'      # feffffffefffffff
          , 'XXXXXXXX'
          , 'XXXXXXXX'
          , 'XXXXXXXX'
          , 'XXXXXXX '
          , 'XXXXXXXX'
          , 'XXXXXXXX'
          , 'XXXXXXXX' ] )

add( name='antihash0.8'
   , bits=[ ' XXX XXX'      # 77bbddee77bbddee
          , 'X XXX XX'
          , 'XX XXX X'
          , 'XXX XXX '
          , ' XXX XXX'
          , 'X XXX XX'
          , 'XX XXX X'
          , 'XXX XXX ' ] )

add( name='antihash1.8'
   , bits=[ 'X XXX XX'      # bbddee77bbddee77
          , 'XX XXX X'
          , 'XXX XXX '
          , ' XXX XXX'
          , 'X XXX XX'
          , 'XX XXX X'
          , 'XXX XXX '
          , ' XXX XXX' ] )

add( name='poids2.8'
   , bits=[ 'X X X X '      # aa55aa55aa55aa55
          , ' X X X X'
          , 'X X X X '
          , ' X X X X'
          , 'X X X X '
          , ' X X X X'
          , 'X X X X '
          , ' X X X X' ] )

add( name='poids4.8'
   , bits=[ 'X   X   '      # 8800220088002200
          , '        '
          , '  X   X '
          , '        '
          , 'X   X   '
          , '        '
          , '  X   X '
          , '        ' ] )

add( name='antipoids2.8'
   , bits=[ ' # # # #'      # 55aa55aa55aa55aa
          , '# # # # '
          , ' # # # #'
          , '# # # # '
          , ' # # # #'
          , '# # # # '
          , ' # # # #'
          , '# # # # ' ] )

add( name='light_antihash0.8'
   , bits=[ 'X   X   '      # 8822882288228822
          , '  X   X '
          , 'X   X   '
          , '  X   X '
          , 'X   X   '
          , '  X   X '
          , 'X   X   '
          , '  X   X ' ] )

add( name='light_antihash1.8'
   , bits=[ ' X   X  '      # 4411441144114411
          , '   X   X'
          , ' X   X  '
          , '   X   X'
          , ' X   X  '
          , '   X   X'
          , ' X   X  '
          , '   X   X' ] )

add( name='light_antihash2.8'
   , bits=[ '  X   X '      # 2288228822882288
          , 'X   X   '
          , '  X   X '
          , 'X   X   '
          , '  X   X '
          , 'X   X   '
          , '  X   X '
          , 'X   X   ' ] )

add( name='light_antislash0.8'
   , bits=[ 'X   X   '      # 8844221188442211
          , ' X   X  '
          , '  X   X '
          , '   X   X'
          , 'X   X   '
          , ' X   X  '
          , '  X   X '
          , '   X   X' ] )

add( name='urgo.32'
   , bits=[ 'XXXXXXXXXXXX    XXXXXXXXXXXXXXXX'
          , 'XXXXXXXXX          XXXXXXXXXXXXX'
          , 'XXXXXXXXX          XXXXXXXXXXXXX'
          , 'XXXXXXXXXXXX    XXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXX    XXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXX    XXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXX    '
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXX    '
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXX    '
          , '   XXXXXXXXXXXXXXXXXXXXXX       '
          , '   XXXXXXXXXXXXXXXXXXXXXX       '
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXX    '
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXX    '
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXX    '
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXX    XXXXXXXXXXXXXXXX'
          , 'XXXXXXXXXXXX    XXXXXXXXXXXXXXXX' ] )

add( name='slash.32'
   , bits=[ '             XXXX            XXX'
          , '            XXXX            XXXX'
          , '           XXXX            XXXX '
          , '          XXXX            XXXX  '
          , '         XXXX            XXXX   '
          , '        XXXX            XXXX    '
          , '       XXXX            XXXX     '
          , '      XXXX            XXXX      '
          , '     XXXX            XXXX       '
          , '    XXXX            XXXX        '
          , '   XXXX            XXXX         '
          , '  XXXX            XXXX          '
          , ' XXXX            XXXX           '
          , 'XXXX            XXXX            '
          , 'XXX            XXXX            X'
          , 'XX            XXXX            XX'
          , 'X            XXXX            XXX'
          , '            XXXX            XXXX'
          , '           XXXX            XXXX '
          , '          XXXX            XXXX  '
          , '         XXXX            XXXX   '
          , '        XXXX            XXXX    '
          , '       XXXX            XXXX     '
          , '      XXXX            XXXX      '
          , '     XXXX            XXXX       '
          , '    XXXX            XXXX        '
          , '   XXXX            XXXX         '
          , '  XXXX            XXXX          '
          , ' XXXX            XXXX           '
          , 'XXXX            XXXX            '
          , 'XXX            XXXX             '
          , 'XX            XXXX              ' ] )

add( name='antihash0.32'
   , bits=[ ' XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX'
          , 'X XXXXXXXXXXXXXXX XXXXXXXXXXXXXX'
          , 'XX XXXXXXXXXXXXXXX XXXXXXXXXXXXX'
          , 'XXX XXXXXXXXXXXXXXX XXXXXXXXXXXX'
          , 'XXXX XXXXXXXXXXXXXXX XXXXXXXXXXX'
          , 'XXXXX XXXXXXXXXXXXXXX XXXXXXXXXX'
          , 'XXXXXX XXXXXXXXXXXXXXX XXXXXXXXX'
          , 'XXXXXXX XXXXXXXXXXXXXXX XXXXXXXX'
          , 'XXXXXXXX XXXXXXXXXXXXXXX XXXXXXX'
          , 'XXXXXXXXX XXXXXXXXXXXXXXX XXXXXX'
          , 'XXXXXXXXXX XXXXXXXXXXXXXXX XXXXX'
          , 'XXXXXXXXXXX XXXXXXXXXXXXXXX XXXX'
          , 'XXXXXXXXXXXX XXXXXXXXXXXXXXX XXX'
          , 'XXXXXXXXXXXXX XXXXXXXXXXXXXXX XX'
          , 'XXXXXXXXXXXXXX XXXXXXXXXXXXXXX X'
          , 'XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX '
          , ' XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX'
          , 'X XXXXXXXXXXXXXXX XXXXXXXXXXXXXX'
          , 'XX XXXXXXXXXXXXXXX XXXXXXXXXXXXX'
          , 'XXX XXXXXXXXXXXXXXX XXXXXXXXXXXX'
          , 'XXXX XXXXXXXXXXXXXXX XXXXXXXXXXX'
          , 'XXXXX XXXXXXXXXXXXXXX XXXXXXXXXX'
          , 'XXXXXX XXXXXXXXXXXXXXX XXXXXXXXX'
          , 'XXXXXXX XXXXXXXXXXXXXXX XXXXXXXX'
          , 'XXXXXXXX XXXXXXXXXXXXXXX XXXXXXX'
          , 'XXXXXXXXX XXXXXXXXXXXXXXX XXXXXX'
          , 'XXXXXXXXXX XXXXXXXXXXXXXXX XXXXX'
          , 'XXXXXXXXXXX XXXXXXXXXXXXXXX XXXX'
          , 'XXXXXXXXXXXX XXXXXXXXXXXXXXX XXX'
          , 'XXXXXXXXXXXXX XXXXXXXXXXXXXXX XX'
          , 'XXXXXXXXXXXXXX XXXXXXXXXXXXXXX X'
          , 'XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX ' ] )

add( name='antihash1.32'
   , bits=[ 'XXXX XXXXXXXXXXXXXXX XXXXXXXXXXX'
          , 'XXXXX XXXXXXXXXXXXXXX XXXXXXXXXX'
          , 'XXXXXX XXXXXXXXXXXXXXX XXXXXXXXX'
          , 'XXXXXXX XXXXXXXXXXXXXXX XXXXXXXX'
          , 'XXXXXXXX XXXXXXXXXXXXXXX XXXXXXX'
          , 'XXXXXXXXX XXXXXXXXXXXXXXX XXXXXX'
          , 'XXXXXXXXXX XXXXXXXXXXXXXXX XXXXX'
          , 'XXXXXXXXXXX XXXXXXXXXXXXXXX XXXX'
          , 'XXXXXXXXXXXX XXXXXXXXXXXXXXX XXX'
          , 'XXXXXXXXXXXXX XXXXXXXXXXXXXXX XX'
          , 'XXXXXXXXXXXXXX XXXXXXXXXXXXXXX X'
          , 'XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX '
          , ' XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX'
          , 'X XXXXXXXXXXXXXXX XXXXXXXXXXXXXX'
          , 'XX XXXXXXXXXXXXXXX XXXXXXXXXXXXX'
          , 'XXX XXXXXXXXXXXXXXX XXXXXXXXXXXX'
          , 'XXXX XXXXXXXXXXXXXXX XXXXXXXXXXX'
          , 'XXXXX XXXXXXXXXXXXXXX XXXXXXXXXX'
          , 'XXXXXX XXXXXXXXXXXXXXX XXXXXXXXX'
          , 'XXXXXXX XXXXXXXXXXXXXXX XXXXXXXX'
          , 'XXXXXXXX XXXXXXXXXXXXXXX XXXXXXX'
          , 'XXXXXXXXX XXXXXXXXXXXXXXX XXXXXX'
          , 'XXXXXXXXXX XXXXXXXXXXXXXXX XXXXX'
          , 'XXXXXXXXXXX XXXXXXXXXXXXXXX XXXX'
          , 'XXXXXXXXXXXX XXXXXXXXXXXXXXX XXX'
          , 'XXXXXXXXXXXXX XXXXXXXXXXXXXXX XX'
          , 'XXXXXXXXXXXXXX XXXXXXXXXXXXXXX X'
          , 'XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX '
          , ' XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX'
          , 'X XXXXXXXXXXXXXXX XXXXXXXXXXXXXX'
          , 'XX XXXXXXXXXXXXXXX XXXXXXXXXXXXX'
          , 'XXX XXXXXXXXXXXXXXX XXXXXXXXXXXX' ] )

add( name='poids2.32'
   , bits=[ '                                '
          , ' XX      XX      XX      XX     '
          , ' XX      XX      XX      XX     '
          , '                                '
          , '                                '
          , '     XX      XX      XX      XX '
          , '     XX      XX      XX      XX '
          , '                                '
          , '                                '
          , ' XX      XX      XX      XX     '
          , ' XX      XX      XX      XX     '
          , '                                '
          , '                                '
          , '     XX      XX      XX      XX '
          , '     XX      XX      XX      XX '
          , '                                '
          , '                                '
          , ' XX      XX      XX      XX     '
          , ' XX      XX      XX      XX     '
          , '                                '
          , '                                '
          , '     XX      XX      XX      XX '
          , '     XX      XX      XX      XX '
          , '                                '
          , '                                '
          , ' XX      XX      XX      XX     '
          , ' XX      XX      XX      XX     '
          , '                                '
          , '                                '
          , '     XX      XX      XX      XX '
          , '     XX      XX      XX      XX '
          , '                                ' ] )

add( name='poids4.32'
   , bits=[ '                                '
          , ' XX              XX             '
          , ' XX              XX             '
          , '                                '
          , '                                '
          , '         XX              XX     '
          , '         XX              XX     '
          , '                                '
          , '                                '
          , ' XX              XX             '
          , ' XX              XX             '
          , '                                '
          , '                                '
          , '         XX              XX     '
          , '         XX              XX     '
          , '                                '
          , '                                '
          , ' XX              XX             '
          , ' XX              XX             '
          , '                                '
          , '                                '
          , '         XX              XX     '
          , '         XX              XX     '
          , '                                '
          , '                                '
          , ' XX              XX             '
          , ' XX              XX             '
          , '                                '
          , '                                '
          , '         XX              XX     '
          , '         XX              XX     '
          , '                                ' ] )

add( name='antipoids2.32'
   , bits=[ '                                '
          , '     XX      XX      XX      XX '
          , '     XX      XX      XX      XX '
          , '                                '
          , '                                '
          , ' XX      XX      XX      XX     '
          , ' XX      XX      XX      XX     '
          , '                                '
          , '                                '
          , '     XX      XX      XX      XX '
          , '     XX      XX      XX      XX '
          , '                                '
          , '                                '
          , ' XX      XX      XX      XX     '
          , ' XX      XX      XX      XX     '
          , '                                '
          , '                                '
          , '     XX      XX      XX      XX '
          , '     XX      XX      XX      XX '
          , '                                '
          , '                                '
          , ' XX      XX      XX      XX     '
          , ' XX      XX      XX      XX     '
          , '                                '
          , '                                '
          , '     XX      XX      XX      XX '
          , '     XX      XX      XX      XX '
          , '                                '
          , '                                '
          , ' XX      XX      XX      XX     '
          , ' XX      XX      XX      XX     '
          , '                                ' ] )

add( name='antislash.32'
   , bits=[ 'X               X               '
          , ' X               X              '
          , '  X               X             '
          , '   X               X            '
          , '    X               X           '
          , '     X               X          '
          , '      X               X         '
          , '       X               X        '
          , '        X               X       '
          , '         X               X      '
          , '          X               X     '
          , '           X               X    '
          , '            X               X   '
          , '             X               X  '
          , '              X               X '
          , '               X               X'
          , 'X               X               '
          , ' X               X              '
          , '  X               X             '
          , '   X               X            '
          , '    X               X           '
          , '     X               X          '
          , '      X               X         '
          , '       X               X        '
          , '        X               X       '
          , '         X               X      '
          , '          X               X     '
          , '           X               X    '
          , '            X               X   '
          , '             X               X  '
          , '              X               X '
          , '               X               X' ] )

add( name='antislash2.32'
   , bits=[ 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X ' ] )

add( name='antislash3.32'
   , bits=[ '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             ' ] )

add( name='antislash4.32'
   , bits=[ '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       ' ] )

add( name='antislash5.32'
   , bits=[ '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               '
          , '  X               X             '
          , '    X               X           '
          , '      X               X         '
          , '        X               X       '
          , '          X               X     '
          , '            X               X   '
          , '              X               X '
          , 'X               X               ' ] )

add( name='diffusion.32'
   , bits=[ 'XXXXXXXXXX      XXXXXXXXXX      '
          , ' XXXXXXXXXX      XXXXXXXXXX     '
          , '  XXXXXXXXXX      XXXXXXXXXX    '
          , '   XXXXXXXXXX      XXXXXXXXXX   '
          , '    XXXXXXXXXX      XXXXXXXXXX  '
          , '     XXXXXXXXXX      XXXXXXXXXX '
          , '      XXXXXXXXXX      XXXXXXXXXX'
          , 'X      XXXXXXXXXX      XXXXXXXXX'
          , 'XX      XXXXXXXXXX      XXXXXXXX'
          , 'XXX      XXXXXXXXXX      XXXXXXX'
          , 'XXXX      XXXXXXXXXX      XXXXXX'
          , 'XXXXX      XXXXXXXXXX      XXXXX'
          , 'XXXXXX      XXXXXXXXXX      XXXX'
          , 'XXXXXXX      XXXXXXXXXX      XXX'
          , 'XXXXXXXX      XXXXXXXXXX      XX'
          , 'XXXXXXXXX      XXXXXXXXXX      X'
          , 'XXXXXXXXXX      XXXXXXXXXX      '
          , ' XXXXXXXXXX      XXXXXXXXXX     '
          , '  XXXXXXXXXX      XXXXXXXXXX    '
          , '   XXXXXXXXXX      XXXXXXXXXX   '
          , '    XXXXXXXXXX      XXXXXXXXXX  '
          , '     XXXXXXXXXX      XXXXXXXXXX '
          , '      XXXXXXXXXX      XXXXXXXXXX'
          , 'X      XXXXXXXXXX      XXXXXXXXX'
          , 'XX      XXXXXXXXXX      XXXXXXXX'
          , 'XXX      XXXXXXXXXX      XXXXXXX'
          , 'XXXX      XXXXXXXXXX      XXXXXX'
          , 'XXXXX      XXXXXXXXXX      XXXXX'
          , 'XXXXXX      XXXXXXXXXX      XXXX'
          , 'XXXXXXX      XXXXXXXXXX      XXX'
          , 'XXXXXXXX      XXXXXXXXXX      XX'
          , 'XXXXXXXXX      XXXXXXXXXX      X' ] )

add( name='active.32'
   , bits=[ '   XXXXXXXXXXXXX   XXXXXXXXXXXXX'
          , 'X   XXXXXXXXXXXXX   XXXXXXXXXXXX'
          , 'XX   XXXXXXXXXXXXX   XXXXXXXXXXX'
          , 'XXX   XXXXXXXXXXXXX   XXXXXXXXXX'
          , 'XXXX   XXXXXXXXXXXXX   XXXXXXXXX'
          , 'XXXXX   XXXXXXXXXXXXX   XXXXXXXX'
          , 'XXXXXX   XXXXXXXXXXXXX   XXXXXXX'
          , 'XXXXXXX   XXXXXXXXXXXXX   XXXXXX'
          , 'XXXXXXXX   XXXXXXXXXXXXX   XXXXX'
          , 'XXXXXXXXX   XXXXXXXXXXXXX   XXXX'
          , 'XXXXXXXXXX   XXXXXXXXXXXXX   XXX'
          , 'XXXXXXXXXXX   XXXXXXXXXXXXX   XX'
          , 'XXXXXXXXXXXX   XXXXXXXXXXXXX   X'
          , 'XXXXXXXXXXXXX   XXXXXXXXXXXXX   '
          , ' XXXXXXXXXXXXX   XXXXXXXXXXXXX  '
          , '  XXXXXXXXXXXXX   XXXXXXXXXXXXX '
          , '   XXXXXXXXXXXXX   XXXXXXXXXXXXX'
          , 'X   XXXXXXXXXXXXX   XXXXXXXXXXXX'
          , 'XX   XXXXXXXXXXXXX   XXXXXXXXXXX'
          , 'XXX   XXXXXXXXXXXXX   XXXXXXXXXX'
          , 'XXXX   XXXXXXXXXXXXX   XXXXXXXXX'
          , 'XXXXX   XXXXXXXXXXXXX   XXXXXXXX'
          , 'XXXXXX   XXXXXXXXXXXXX   XXXXXXX'
          , 'XXXXXXX   XXXXXXXXXXXXX   XXXXXX'
          , 'XXXXXXXX   XXXXXXXXXXXXX   XXXXX'
          , 'XXXXXXXXX   XXXXXXXXXXXXX   XXXX'
          , 'XXXXXXXXXX   XXXXXXXXXXXXX   XXX'
          , 'XXXXXXXXXXX   XXXXXXXXXXXXX   XX'
          , 'XXXXXXXXXXXX   XXXXXXXXXXXXX   X'
          , 'XXXXXXXXXXXXX   XXXXXXXXXXXXX   '
          , ' XXXXXXXXXXXXX   XXXXXXXXXXXXX  '
          , '  XXXXXXXXXXXXX   XXXXXXXXXXXXX ' ] )
