
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2021-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/block/matrix.py"                |
# +-----------------------------------------------------------------+


import re
import sys
import os.path
from   operator           import itemgetter
from   ...                import Cfg
from   ...Hurricane       import Breakpoint, DbU, Box, Transformation, Box, \
                                 Path, Layer, Occurrence, Net, RoutingPad,  \
                                 Horizontal, Vertical, Contact, Pin, Plug,  \
                                 Instance
from   ...CRL             import RoutingLayerGauge, AllianceFramework, Catalog
from   ...helpers         import trace, l, u, n
from   ...helpers         import dots
from   ...helpers.io      import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay import UpdateSession
from   ..                 import getParameter, utils


af = AllianceFramework.get()


class RegisterMatrix ( object ):

    registerCell = af.getCell( 'sff1_x4', Catalog.State.Views )
    
    def __init__ ( self, conf, cell, pattern ):
        if not RegisterMatrix.registerCell:
            raise ErrorMessage( 1, 'Matrix.__init__(): DFF Cell "sff1_x4" has not been found in the libraries.' )
        self.conf        = conf
        self.pattern     = re.compile( pattern )
        self.matrix      = [ [], ]
        self.abutmentBox = Box()
        for instance in cell.getInstances():
            if instance.getMasterCell() != RegisterMatrix.registerCell:
                continue
            output = instance.getPlug( instance.getMasterCell().getNet( 'q' ) )
            m = self.pattern.match( output.getNet().getName() )
            if not m:
                continue
            i = int( m.group('i') )
            j = int( m.group('j') )
            if len(self.matrix) <= i:
                while len(self.matrix) <= i:
                    self.matrix.append( [] )
            if len(self.matrix[i]) <= j:
                while len(self.matrix[i]) <= j:
                    self.matrix[i].append( None )
            if self.matrix[i][j] is not None:
                print( ErrorMessage( 1, 'Matrix.__init__(): Duplicated matrix element [{},{}]' \
                                        .format(i,j) ) )
                continue
            self.matrix[i][j] = instance
        columns = len( self.matrix[0] )
        for i in range(1,self.rows):
            if len(self.matrix[i]) != self.columns:
                print( ErrorMessage( 1, 'Matrix.__init__(): Uneven matrix, column {} has a different size ({})' \
                                        .format(i,len(self.matrix[i])) ) )
            for j in range(len(self.matrix[i])):
                if self.matrix[i][j] is None:
                    print( ErrorMessage( 1, '[ERROR] Holed matrix @[{},{}]' \
                                            .format(i,j) ) )
        regAb = RegisterMatrix.registerCell.getAbutmentBox()
        print( '   o  Matched matrix block on pattern "{}"'.format(self.pattern.pattern) )
        print( '      - Size {} x {}, {} x {} ' \
               .format( self.rows
                      , self.columns
                      , DbU.getValueString(regAb.getWidth ()*self.rows)
                      , DbU.getValueString(regAb.getHeight()*self.columns)
                      ))

    @property
    def rows ( self ):
        return len(self.matrix)

    @property
    def columns ( self ):
        return len(self.matrix[0])
        
    def toXPitch ( self, x ):
        """
        Returns the coordinate of the pitch immediately inferior to X.
        If we are in a chip, compute coordinate in the *corona* system.
        """
        offset = 0
        area   = self.conf.coreAb
        if self.conf.isCoreBlock:
            offset = self.conf.icore.getTransformation().getTx()
            self.conf.icore.getTransformation().applyOn( area )
        modulo = (x - offset - area.getXMin()) % self.conf.sliceStep 
        return x - modulo

    def toYSlice ( self, y ):
        """
        Returns the coordinate of the slice immediately inferior to Y.
        If we are in a chip, compute coordinate in the *corona* system.
        """
        offset = 0
        area   = self.conf.coreAb
        if self.conf.isCoreBlock:
            offset = self.conf.icore.getTransformation().getTy()
            self.conf.icore.getTransformation().applyOn( area )
        #trace( 540, '\toffset:{}\n'.format(DbU.getValueString(offset)) )
        modulo = (y - offset - area.getYMin()) % self.conf.sliceHeight 
        return y - modulo

    def _getTransformation ( self, originX, originY ):
        """Transform (originX,originY) into sliced coordinates relatives to the corona."""
        yoffset = 0
        if self.conf.isCoreBlock:
            yoffset = self.conf.icore.getTransformation().getTy()
        sliceHeight = self.conf.sliceHeight 
        x           = self.toXPitch( originX )
        y           = self.toYSlice( originY )
        slice       = (y - yoffset) // sliceHeight
        orientation = Transformation.Orientation.ID
        y = slice * sliceHeight + yoffset
        if slice % 2:
            orientation = Transformation.Orientation.MY
            y          += sliceHeight
        transf = Transformation( x, y, orientation )
        return transf

    def getWidth ( self ):
        width = RegisterMatrix.registerCell.getAbutmentBox().getWidth()
        return width * self.rows

    def getHeight ( self ):
        height = RegisterMatrix.registerCell.getAbutmentBox().getHeight()
        return height * self.columns

    def getAbutmentBox ( self ):
        return self.abutmentBox

    def place ( self, origin ):
        regAb       = RegisterMatrix.registerCell.getAbutmentBox()
        sliceHeight = self.conf.sliceHeight 
        regWidth    = regAb.getWidth()
        x = origin.getX()
        for i in range(self.rows):
            y = origin.getY()
            for j in range(self.columns):
                instance = self.matrix[i][j]
                instance.setTransformation ( self._getTransformation(x,y) )
                instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
                #print( '{} placed @{}'.format(instance,instance.getTransformation()) )
                y += sliceHeight
            x += regWidth
        self.abutmentBox = Box( origin.getX()
                              , origin.getY()
                              , origin.getX() + self.getWidth()
                              , origin.getY() + self.getHeight() )
