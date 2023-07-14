
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne University 2020-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/block/iospecs.py"               |
# +-----------------------------------------------------------------+

import sys
import re
import os.path
import json
from   operator             import itemgetter
from   ...                  import Cfg
from   ...Hurricane         import Breakpoint, DbU, Box, Transformation
from   ...                  import CRL
from   ...helpers           import trace, l, u, n
from   ...helpers.utils     import classdecorator
from   ...helpers.io        import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay   import CfgCache
from   .block.configuration import IoPin

__all__ = [ 'IoSpecs' ]


# ----------------------------------------------------------------------------
# Function  :  "iospecs.utf8toStr".
def utf8toStr ( data, ignoreDicts=False ):
    """
    Translate UTF-8 strings into basic str ones in dict & list datas.
    Based on _byteify() by Mirec Miskuf on Stackoverflow.

    reference: https://stackoverflow.com/questions/956867/
               how-to-get-string-objects-instead-of-unicode-from-json/
               33571117#33571117
    """
    if isinstance(data,unicode):
        return data.encode('utf-8')
    if isinstance(data,list):
        return [ utf8toStr( item, ignoreDicts=True ) for item in data ]
    if isinstance(data,dict) and not ignoreDicts:
        return dict( ( utf8toStr(key  , ignoreDicts=True )
                     , utf8toStr(value, ignoreDicts=True ))
                     for key, value in data.iteritems() )
    return data


# ----------------------------------------------------------------------------
# Class  :  "iospecs.IoPadSpec".

class IoPadSpec ( object ):

    id = 0

    def __init__ ( self, instance, side, position=None, nets=[] ):
        self.instance = instance
        self.side     = side
        self.position = position
        self.nets     = list(nets)
        self._id      = IoPadSpec.id
        IoPadSpec.id += 1

    def setAnalog ( self ):
        self.side |= IoPin.ANALOG

    def addNets ( self, nets ):
        self.nets += nets

    def asList ( self ):
        return tuple( [ self.side, self.position, self.instance ] + self.nets )

    def __str__ ( self ):
        s  = '({:<11}'.format( IoPin.toStr(self.side) )
        s += ', {}'.format( self.position )
        s += ', "{}"'.format( self.instance )
        for net in self.nets:
            s += ', "{}"'.format(net)
        s += ')'
        return s


# ----------------------------------------------------------------------------
# Class  :  "iospecs.IoSpecs".

class IoSpecs ( object ):
    """
    Load and generate IoPadsSpec from a variety of sources.
    """

    def __init__ ( self ):
        self._ioPadsLUT  = {}
        self._ioPadsSpec = []

    def addIoPadSpec ( self, instance, side ):
        if instance in self._ioPadsLUT:
            print( ErrorMessage( 2, 'IoSpecs.addIoPadSpec(): Duplicate pad specification for "{}" (ignored).' \
                                    .format(instance) ) )
            return self._ioPadsLUT[ instance ]
        spec = IoPadSpec( instance, side )
        self._ioPadsLUT[ instance ] = spec
        self._ioPadsSpec.append( spec )
        trace( 560, '\tIoSpecs.addIoPadSpec() side={} spec={}\n'.format(side,spec) )
        return spec

    def addIoPadSpecs(self, names, side):
        for i, padName in enumerate(names):
            actual_side = side
            # add start and end
            if i == 0:
                actual_side |= IoPin.A_BEGIN
            elif i == len(names)-1:
                actual_side |= IoPin.A_END
            self.addIoPadSpec( padName, actual_side)

    def loadFromPinmux ( self, fileName , cheat_dont_do_analog=False):
        """
        Load ioPadsSpec from a LibreSOC generated pinmux file in JSON format.
        The cheat_dont_do_analog is there, sigh, because nsxlib doesn't
        have analog pads.  it's a terrible hack.
        """
        print( '  o  Loading I/O pad specifications from "%s".' % fileName )
        if not os.path.isfile(fileName):
            raise ErrorMessage( 2, [ 'IoSpecs.loadFromPinmux(): '
                                     'JSON pinmux file not found.'
                                   , '("{}")'.format(fileName) ] )
        with open(fileName) as fd:
           #datas = utf8toStr( json.loads( fd.read(), object_hook=utf8toStr )
           #                 , ignoreDicts=True )
            datas = json.loads( fd.read() )
        self.addIoPadSpecs(datas['pads.east' ], IoPin.EAST  )
        self.addIoPadSpecs(datas['pads.west' ], IoPin.WEST  )
        self.addIoPadSpecs(datas['pads.north'], IoPin.NORTH )
        self.addIoPadSpecs(datas['pads.south'], IoPin.SOUTH )

        for padDatas in datas['pads.instances']:
            padName = padDatas[0]
            if not padName in self._ioPadsLUT:
                print( WarningMessage('IoSpecs.loadFromPinmux(): ' \
                                      'Pad "{}" is not on any side, ignored.' \
                                      .format(padName) ))
                continue
            end = None 
            # remove the direction info: + output - input * bi-directional
            if padDatas[-1][-1] in '+-*': end = -1
            # check if pad is analog or not: last spec item starts with "A"
            if padDatas[-1][0] == 'A' and not cheat_dont_do_analog:
                self._ioPadsLUT[padName].setAnalog()
            # add the nets to the pad
            self._ioPadsLUT[padName].addNets( padDatas[1:end] )
        trace( 560, '-' )

    @property
    def ioPadsSpec ( self ):
        specs = []
        for spec in self._ioPadsSpec:
            specs.append( spec.asList() )
        return specs

    def __str__ ( self ):
        if len(self._ioPadsSpec):
            s = 'ioPadsSpec = [ '
            for spec in self._ioPadsSpec:
                if len(s) > 15: s += '\n             , '
                s += '{}'.format( spec )
                s += ')'
        s += ' ]'
        return s
            
            
