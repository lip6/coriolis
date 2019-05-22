
# -*- mode:Python -*-

import sys
import math
from   helpers.io import ErrorMessage
from   helpers.io import WarningMessage


confFile    = '<confFile has not been set>'
patternsLUT = {}


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
        for i in range(len(hexasLSB)/2):
            self._hexa += hexasLSB[i*2+1] + hexasLSB[i*2]
        return self._hexa

    def _fromHexa ( self ):
        self._bits = []
    
        side = math.sqrt(4*len(self._hexa))
        if pow(side,2) != 4*len(self._hexa):
            print '[ERROR] The pattern is not square (%d self._bits).' % (4*len(self._hexa))
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


def loadPatterns ( patternsData, fromFile ):
    global patternsLUT
    global confFile
    confFile = fromFile

    entryNo = 0
    for entry in patternsData:
        entryNo += 1

        try:
            if not isinstance(entry,dict):
                raise ErrorMessage(1,['Malformed entry in <patternsTable>.'
                                     ,'Not a dictionary {\'key1\':\'value1\', ...}.'
                                     ,str(entry)
                                     ])

            if not entry.has_key('name'):
                raise ErrorMessage(1,['Malformed entry in <patternsTable>.'
                                     ,'Pattern has no \'name\' key.'
                                     ,str(entry)
                                     ])
            arguments = { 'name':entry['name'] }

            if entry.has_key('bits'):
                arguments['bits'] = entry['bits']

            if entry.has_key('hexa'):
                if entry.has_key('bits'):
                    w = WarningMessage( 'Pattern <%s> has both bits & hexa, ignoring hexa.' % entry['name'] )
                    print w
                else:
                    arguments['hexa'] = entry['hexa']

            patternsLUT[ entry['name'] ] = Pattern( **arguments ) 


        except Exception, e:
            e = ErrorMessage( e )
            e.addMessage( 'In %s:<patternsTable> at index %d.' % (confFile,entryNo) )
            print e
    return


#patternsLUT = { 'slash.8' : Pattern( name='slash.8'
#                                   , bits=[ '   X   X'
#                                          , '  X   X '
#                                          , ' X   X  '
#                                          , 'X   X   '
#                                          , '   X   X'
#                                          , '  X   X '
#                                          , ' X   X  '
#                                          , 'X   X   ' ])
#              , 'hash.8'  : Pattern( name='hash.8'
#                                   , bits=[ 'XXX XXX '
#                                          , 'XX XXX X'
#                                          , 'X XXX XX'
#                                          , ' XXX XXX'
#                                          , 'XXX XXX '
#                                          , 'XX XXX X'
#                                          , 'X XXX XX'
#                                          , ' XXX XXX' ])
#              , 'urgo.8'             : Pattern( name='urgo.8'            , hexa='feffffffefffffff')
#              , 'antihash0.8'        : Pattern( name='antihash0.8'       , hexa='77bbddee77bbddee')
#              , 'antihash1.8'        : Pattern( name='antihash1.8'       , hexa='bbddee77bbddee77')
#              , 'poids2.8'           : Pattern( name='poids2.8'          , hexa='aa55aa55aa55aa55')
#              , 'poids4.8'           : Pattern( name='poids4.8'          , hexa='8800220088002200')
#              , 'antipoids2.8'       : Pattern( name='antipoids2.8'      , hexa='55aa55aa55aa55aa')
#              , 'light_antihash0.8'  : Pattern( name='light_antihash0.8' , hexa='8822882288228822')
#              , 'light_antihash1.8'  : Pattern( name='light_antihash1.8' , hexa='4411441144114411')
#              , 'light_antihash2.8'  : Pattern( name='light_antihash2.8' , hexa='2288228822882288')
#              , 'light_antislash0.8' : Pattern( name='light_antislash0.8', hexa='8844221188442211')
#              , 'urgo.32'            : Pattern( name='urgo.32'
#                                              , bits=[ 'XXXXXXXXXXXXX  XXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXX      XXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXX      XXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXX  XXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXX  XXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXX  X'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXX  X'
#                                                     , ' XXXXXXXXXXXXXXXXXXXXXXXXXX     '
#                                                     , ' XXXXXXXXXXXXXXXXXXXXXXXXXX     '
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXX  X'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXX  X'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
#                                                     , 'XXXXXXXXXXXXX  XXXXXXXXXXXXXXXXX' ])
#              , 'slash.32'           : Pattern( name='slash.32'
#                                              , bits=[ '               X               X'
#                                                     , '              X               X '
#                                                     , '             X               X  '
#                                                     , '            X               X   '
#                                                     , '           X               X    '
#                                                     , '          X               X     '
#                                                     , '         X               X      '
#                                                     , '        X               X       '
#                                                     , '       X               X        '
#                                                     , '      X               X         '
#                                                     , '     X               X          '
#                                                     , '    X               X           '
#                                                     , '   X               X            '
#                                                     , '  X               X             '
#                                                     , ' X               X              '
#                                                     , 'X               X               '
#                                                     , '               X               X'
#                                                     , '              X               X '
#                                                     , '             X               X  '
#                                                     , '            X               X   '
#                                                     , '           X               X    '
#                                                     , '          X               X     '
#                                                     , '         X               X      '
#                                                     , '        X               X       '
#                                                     , '       X               X        '
#                                                     , '      X               X         '
#                                                     , '     X               X          '
#                                                     , '    X               X           '
#                                                     , '   X               X            '
#                                                     , '  X               X             '
#                                                     , ' X               X              '
#                                                     , 'X               X               ' ])
#              , 'antihash0.32'       : Pattern( name='antihash0.32'
#                                              , bits=[ ' XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX'
#                                                     , 'X XXXXXXXXXXXXXXX XXXXXXXXXXXXXX'
#                                                     , 'XX XXXXXXXXXXXXXXX XXXXXXXXXXXXX'
#                                                     , 'XXX XXXXXXXXXXXXXXX XXXXXXXXXXXX'
#                                                     , 'XXXX XXXXXXXXXXXXXXX XXXXXXXXXXX'
#                                                     , 'XXXXX XXXXXXXXXXXXXXX XXXXXXXXXX'
#                                                     , 'XXXXXX XXXXXXXXXXXXXXX XXXXXXXXX'
#                                                     , 'XXXXXXX XXXXXXXXXXXXXXX XXXXXXXX'
#                                                     , 'XXXXXXXX XXXXXXXXXXXXXXX XXXXXXX'
#                                                     , 'XXXXXXXXX XXXXXXXXXXXXXXX XXXXXX'
#                                                     , 'XXXXXXXXXX XXXXXXXXXXXXXXX XXXXX'
#                                                     , 'XXXXXXXXXXX XXXXXXXXXXXXXXX XXXX'
#                                                     , 'XXXXXXXXXXXX XXXXXXXXXXXXXXX XXX'
#                                                     , 'XXXXXXXXXXXXX XXXXXXXXXXXXXXX XX'
#                                                     , 'XXXXXXXXXXXXXX XXXXXXXXXXXXXXX X'
#                                                     , 'XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX '
#                                                     , ' XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX'
#                                                     , 'X XXXXXXXXXXXXXXX XXXXXXXXXXXXXX'
#                                                     , 'XX XXXXXXXXXXXXXXX XXXXXXXXXXXXX'
#                                                     , 'XXX XXXXXXXXXXXXXXX XXXXXXXXXXXX'
#                                                     , 'XXXX XXXXXXXXXXXXXXX XXXXXXXXXXX'
#                                                     , 'XXXXX XXXXXXXXXXXXXXX XXXXXXXXXX'
#                                                     , 'XXXXXX XXXXXXXXXXXXXXX XXXXXXXXX'
#                                                     , 'XXXXXXX XXXXXXXXXXXXXXX XXXXXXXX'
#                                                     , 'XXXXXXXX XXXXXXXXXXXXXXX XXXXXXX'
#                                                     , 'XXXXXXXXX XXXXXXXXXXXXXXX XXXXXX'
#                                                     , 'XXXXXXXXXX XXXXXXXXXXXXXXX XXXXX'
#                                                     , 'XXXXXXXXXXX XXXXXXXXXXXXXXX XXXX'
#                                                     , 'XXXXXXXXXXXX XXXXXXXXXXXXXXX XXX'
#                                                     , 'XXXXXXXXXXXXX XXXXXXXXXXXXXXX XX'
#                                                     , 'XXXXXXXXXXXXXX XXXXXXXXXXXXXXX X'
#                                                     , 'XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX ' ])
#              , 'antihash1.32'       : Pattern( name='antihash1.32'
#                                              , bits=[ 'XXXX XXXXXXXXXXXXXXX XXXXXXXXXXX'
#                                                     , 'XXXXX XXXXXXXXXXXXXXX XXXXXXXXXX'
#                                                     , 'XXXXXX XXXXXXXXXXXXXXX XXXXXXXXX'
#                                                     , 'XXXXXXX XXXXXXXXXXXXXXX XXXXXXXX'
#                                                     , 'XXXXXXXX XXXXXXXXXXXXXXX XXXXXXX'
#                                                     , 'XXXXXXXXX XXXXXXXXXXXXXXX XXXXXX'
#                                                     , 'XXXXXXXXXX XXXXXXXXXXXXXXX XXXXX'
#                                                     , 'XXXXXXXXXXX XXXXXXXXXXXXXXX XXXX'
#                                                     , 'XXXXXXXXXXXX XXXXXXXXXXXXXXX XXX'
#                                                     , 'XXXXXXXXXXXXX XXXXXXXXXXXXXXX XX'
#                                                     , 'XXXXXXXXXXXXXX XXXXXXXXXXXXXXX X'
#                                                     , 'XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX '
#                                                     , ' XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX'
#                                                     , 'X XXXXXXXXXXXXXXX XXXXXXXXXXXXXX'
#                                                     , 'XX XXXXXXXXXXXXXXX XXXXXXXXXXXXX'
#                                                     , 'XXX XXXXXXXXXXXXXXX XXXXXXXXXXXX'
#                                                     , 'XXXX XXXXXXXXXXXXXXX XXXXXXXXXXX'
#                                                     , 'XXXXX XXXXXXXXXXXXXXX XXXXXXXXXX'
#                                                     , 'XXXXXX XXXXXXXXXXXXXXX XXXXXXXXX'
#                                                     , 'XXXXXXX XXXXXXXXXXXXXXX XXXXXXXX'
#                                                     , 'XXXXXXXX XXXXXXXXXXXXXXX XXXXXXX'
#                                                     , 'XXXXXXXXX XXXXXXXXXXXXXXX XXXXXX'
#                                                     , 'XXXXXXXXXX XXXXXXXXXXXXXXX XXXXX'
#                                                     , 'XXXXXXXXXXX XXXXXXXXXXXXXXX XXXX'
#                                                     , 'XXXXXXXXXXXX XXXXXXXXXXXXXXX XXX'
#                                                     , 'XXXXXXXXXXXXX XXXXXXXXXXXXXXX XX'
#                                                     , 'XXXXXXXXXXXXXX XXXXXXXXXXXXXXX X'
#                                                     , 'XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX '
#                                                     , ' XXXXXXXXXXXXXXX XXXXXXXXXXXXXXX'
#                                                     , 'X XXXXXXXXXXXXXXX XXXXXXXXXXXXXX'
#                                                     , 'XX XXXXXXXXXXXXXXX XXXXXXXXXXXXX'
#                                                     , 'XXX XXXXXXXXXXXXXXX XXXXXXXXXXXX' ])
#              , 'poids2.32'                   : Pattern( name='poids2.32'
#                                              , bits=[ '                                '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '                                ' ])
#              , 'poids4.32'                   : Pattern( name='poids2.32'
#                                              , bits=[ '                                '
#                                                     , ' XX              XX             '
#                                                     , ' XX              XX             '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '         XX              XX     '
#                                                     , '         XX              XX     '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , ' XX              XX             '
#                                                     , ' XX              XX             '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '         XX              XX     '
#                                                     , '         XX              XX     '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , ' XX              XX             '
#                                                     , ' XX              XX             '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '         XX              XX     '
#                                                     , '         XX              XX     '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , ' XX              XX             '
#                                                     , ' XX              XX             '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '         XX              XX     '
#                                                     , '         XX              XX     '
#                                                     , '                                ' ])
#              , 'antipoids2.32'               : Pattern( name='antipoids2.32'
#                                              , bits=[ '                                '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '     XX      XX      XX      XX '
#                                                     , '                                '
#                                                     , '                                '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , ' XX      XX      XX      XX     '
#                                                     , '                                ' ])
#              , 'antislash.32'       : Pattern( name='antislash.32'
#                                              , bits=[ 'X               X               '
#                                                     , ' X               X              '
#                                                     , '  X               X             '
#                                                     , '   X               X            '
#                                                     , '    X               X           '
#                                                     , '     X               X          '
#                                                     , '      X               X         '
#                                                     , '       X               X        '
#                                                     , '        X               X       '
#                                                     , '         X               X      '
#                                                     , '          X               X     '
#                                                     , '           X               X    '
#                                                     , '            X               X   '
#                                                     , '             X               X  '
#                                                     , '              X               X '
#                                                     , '               X               X'
#                                                     , 'X               X               '
#                                                     , ' X               X              '
#                                                     , '  X               X             '
#                                                     , '   X               X            '
#                                                     , '    X               X           '
#                                                     , '     X               X          '
#                                                     , '      X               X         '
#                                                     , '       X               X        '
#                                                     , '        X               X       '
#                                                     , '         X               X      '
#                                                     , '          X               X     '
#                                                     , '           X               X    '
#                                                     , '            X               X   '
#                                                     , '             X               X  '
#                                                     , '              X               X '
#                                                     , '               X               X' ])
#              , 'antislash2.32'      : Pattern( name='antislash2.32'
#                                              , bits=[ 'X               X               '
#                                                     , '  X               X             '
#                                                     , '    X               X           '
#                                                     , '      X               X         '
#                                                     , '        X               X       '
#                                                     , '          X               X     '
#                                                     , '            X               X   '
#                                                     , '              X               X '
#                                                     , 'X               X               '
#                                                     , '  X               X             '
#                                                     , '    X               X           '
#                                                     , '      X               X         '
#                                                     , '        X               X       '
#                                                     , '          X               X     '
#                                                     , '            X               X   '
#                                                     , '              X               X '
#                                                     , 'X               X               '
#                                                     , '  X               X             '
#                                                     , '    X               X           '
#                                                     , '      X               X         '
#                                                     , '        X               X       '
#                                                     , '          X               X     '
#                                                     , '            X               X   '
#                                                     , '              X               X '
#                                                     , 'X               X               '
#                                                     , '  X               X             '
#                                                     , '    X               X           '
#                                                     , '      X               X         '
#                                                     , '        X               X       '
#                                                     , '          X               X     '
#                                                     , '            X               X   '
#                                                     , '              X               X ' ])
#              , 'diffusion.32'       : Pattern( name='diffusion.32'
#                                              , bits=[ 'XXXXXXXXXX      XXXXXXXXXX      '
#                                                     , ' XXXXXXXXXX      XXXXXXXXXX     '
#                                                     , '  XXXXXXXXXX      XXXXXXXXXX    '
#                                                     , '   XXXXXXXXXX      XXXXXXXXXX   '
#                                                     , '    XXXXXXXXXX      XXXXXXXXXX  '
#                                                     , '     XXXXXXXXXX      XXXXXXXXXX '
#                                                     , '      XXXXXXXXXX      XXXXXXXXXX'
#                                                     , 'X      XXXXXXXXXX      XXXXXXXXX'
#                                                     , 'XX      XXXXXXXXXX      XXXXXXXX'
#                                                     , 'XXX      XXXXXXXXXX      XXXXXXX'
#                                                     , 'XXXX      XXXXXXXXXX      XXXXXX'
#                                                     , 'XXXXX      XXXXXXXXXX      XXXXX'
#                                                     , 'XXXXXX      XXXXXXXXXX      XXXX'
#                                                     , 'XXXXXXX      XXXXXXXXXX      XXX'
#                                                     , 'XXXXXXXX      XXXXXXXXXX      XX'
#                                                     , 'XXXXXXXXX      XXXXXXXXXX      X'
#                                                     , 'XXXXXXXXXX      XXXXXXXXXX      '
#                                                     , ' XXXXXXXXXX      XXXXXXXXXX     '
#                                                     , '  XXXXXXXXXX      XXXXXXXXXX    '
#                                                     , '   XXXXXXXXXX      XXXXXXXXXX   '
#                                                     , '    XXXXXXXXXX      XXXXXXXXXX  '
#                                                     , '     XXXXXXXXXX      XXXXXXXXXX '
#                                                     , '      XXXXXXXXXX      XXXXXXXXXX'
#                                                     , 'X      XXXXXXXXXX      XXXXXXXXX'
#                                                     , 'XX      XXXXXXXXXX      XXXXXXXX'
#                                                     , 'XXX      XXXXXXXXXX      XXXXXXX'
#                                                     , 'XXXX      XXXXXXXXXX      XXXXXX'
#                                                     , 'XXXXX      XXXXXXXXXX      XXXXX'
#                                                     , 'XXXXXX      XXXXXXXXXX      XXXX'
#                                                     , 'XXXXXXX      XXXXXXXXXX      XXX'
#                                                     , 'XXXXXXXX      XXXXXXXXXX      XX'
#                                                     , 'XXXXXXXXX      XXXXXXXXXX      X' ])
#              , 'active.32'          : Pattern( name='active.32'
#                                              , bits=[ '   XXXXXXXXXXXXX   XXXXXXXXXXXXX'
#                                                     , 'X   XXXXXXXXXXXXX   XXXXXXXXXXXX'
#                                                     , 'XX   XXXXXXXXXXXXX   XXXXXXXXXXX'
#                                                     , 'XXX   XXXXXXXXXXXXX   XXXXXXXXXX'
#                                                     , 'XXXX   XXXXXXXXXXXXX   XXXXXXXXX'
#                                                     , 'XXXXX   XXXXXXXXXXXXX   XXXXXXXX'
#                                                     , 'XXXXXX   XXXXXXXXXXXXX   XXXXXXX'
#                                                     , 'XXXXXXX   XXXXXXXXXXXXX   XXXXXX'
#                                                     , 'XXXXXXXX   XXXXXXXXXXXXX   XXXXX'
#                                                     , 'XXXXXXXXX   XXXXXXXXXXXXX   XXXX'
#                                                     , 'XXXXXXXXXX   XXXXXXXXXXXXX   XXX'
#                                                     , 'XXXXXXXXXXX   XXXXXXXXXXXXX   XX'
#                                                     , 'XXXXXXXXXXXX   XXXXXXXXXXXXX   X'
#                                                     , 'XXXXXXXXXXXXX   XXXXXXXXXXXXX   '
#                                                     , ' XXXXXXXXXXXXX   XXXXXXXXXXXXX  '
#                                                     , '  XXXXXXXXXXXXX   XXXXXXXXXXXXX '
#                                                     , '   XXXXXXXXXXXXX   XXXXXXXXXXXXX'
#                                                     , 'X   XXXXXXXXXXXXX   XXXXXXXXXXXX'
#                                                     , 'XX   XXXXXXXXXXXXX   XXXXXXXXXXX'
#                                                     , 'XXX   XXXXXXXXXXXXX   XXXXXXXXXX'
#                                                     , 'XXXX   XXXXXXXXXXXXX   XXXXXXXXX'
#                                                     , 'XXXXX   XXXXXXXXXXXXX   XXXXXXXX'
#                                                     , 'XXXXXX   XXXXXXXXXXXXX   XXXXXXX'
#                                                     , 'XXXXXXX   XXXXXXXXXXXXX   XXXXXX'
#                                                     , 'XXXXXXXX   XXXXXXXXXXXXX   XXXXX'
#                                                     , 'XXXXXXXXX   XXXXXXXXXXXXX   XXXX'
#                                                     , 'XXXXXXXXXX   XXXXXXXXXXXXX   XXX'
#                                                     , 'XXXXXXXXXXX   XXXXXXXXXXXXX   XX'
#                                                     , 'XXXXXXXXXXXX   XXXXXXXXXXXXX   X'
#                                                     , 'XXXXXXXXXXXXX   XXXXXXXXXXXXX   '
#                                                     , ' XXXXXXXXXXXXX   XXXXXXXXXXXXX  '
#                                                     , '  XXXXXXXXXXXXX   XXXXXXXXXXXXX ' ])
#              }

