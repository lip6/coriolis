#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/vchannels.py"              |
# +-----------------------------------------------------------------+


import sys
from   Hurricane  import DbU
from   Hurricane  import Point
from   Hurricane  import Transformation
from   Hurricane  import Box
from   Hurricane  import Interval
from   Hurricane  import Path
from   Hurricane  import Occurrence
from   Hurricane  import UpdateSession
from   Hurricane  import Net
from   Hurricane  import Contact
from   Hurricane  import Horizontal
from   Hurricane  import Vertical
from   Hurricane  import Query
import CRL
import helpers
from   helpers    import trace
from   helpers.io import ErrorMessage
from   helpers.io import WarningMessage
from   helpers.io import vprint
from   helpers    import l, u, n
import plugins
import chip


class VChannels ( object ):

    def __init__ ( self, cell ):
        self.gauge     = chip.Configuration.GaugeConf()
        self.cell      = cell
        self.vchannels = [ ]

        topAb     = self.cell.getAbutmentBox()
        topTransf = Transformation()
        for occurrence in cell.getNonTerminalNetlistInstanceOccurrences():
          transf = occurrence.getPath().getTransformation()
          if transf != topTransf:
            raise ErrorMessage( 1, [ 'VChannels.__init__(): Transformation of non-terminal instances must be (0,0,ID),'
                                   , 'instead of %s.' % str(transf)
                                   , 'on %s'          % str(occurrence) ] )

          ab = occurrence.getEntity().getMasterCell().getAbutmentBox()
          if ab != topAb:
            raise ErrorMessage( 1, [ 'VChannels.__init__(): Abutment box of non-terminal instances must be equal to the top level %s.' % str(topAb)
                                   , 'on %s' % str(occurrence) ] )
        return


    def addChannelAt ( self, x, width ):
        xMin = self.cell.getAbutmentBox().getXMin()
        xMax = self.cell.getAbutmentBox().getXMax()
        if x < xMin or x >= xMax:
          print ErrorMessage( 1, [ 'VChannels.addChannelAt(): Attempt to add a channel outside abutment box, ignored.'
                                 , '(%s must be included in [%s..%s]' % ( DbU.getValueString(x)
                                                                        , DbU.getValueString(xMin)
                                                                        , DbU.getValueString(xMax) ) ] ),
          return False
            
        for i in range(len(self.vchannels)):
          if self.vchannels[i][0] == x:
            print ErrorMessage( 1, 'VChannels.addChannelAt(): Attempt to add a channel twice at position %s, ignored.' % DbU.getValueString(x) ),
            return False
          if self.vchannels[i][0] > x:
            self.vchannels.insert( i, (x,width) )
            return True

        self.vchannels.append( (x,width) )
        return True


    def getDeltaWidth ( self ):
        deltaWidth = 0
        for x,w in self.vchannels: deltaWidth += w
        return deltaWidth
        
    
    def expandChannels ( self ):
        UpdateSession.open()
        
        dw = self.getDeltaWidth()

        ab = self.cell.getAbutmentBox()
        ab.inflate( 0, 0, dw, 0 )
        self.cell.setAbutmentBox( ab )

        for occurrence in self.cell.getNonTerminalNetlistInstanceOccurrences():
          masterCell = occurrence.getEntity().getMasterCell()
          ab         = masterCell.getAbutmentBox()
          ab.inflate( 0, 0, dw, 0 )
          masterCell.setAbutmentBox( ab )

        for occurrence in self.cell.getTerminalNetlistInstanceOccurrences():
          instance = occurrence.getEntity()
          transf   = occurrence.getPath().getTransformation()
          instance.getTransformation().applyOn( transf )
          xcenter  = transf.getTx() + instance.getMasterCell().getAbutmentBox().getXCenter()

          xshift = 0
          for x,w in self.vchannels:
            if x < xcenter: xshift += w
            else:           break

          if xshift:
            baseTransf = instance.getTransformation()
            baseTransf = Transformation( baseTransf.getTx() + xshift
                                       , baseTransf.getTy()
                                       , baseTransf.getOrientation() )
            instance.setTransformation( baseTransf )

        UpdateSession.close()

        spaceMargin = (float(dw) * 100.0) / float(ab.getWidth())
        vprint( 1, '     - V-Channels space margin: %d%%.' % spaceMargin )
        return
