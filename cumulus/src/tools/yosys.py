#!/usr/bin/env python3

import sys
import os
import os.path
import subprocess
import optparse
from   helpers.io import ErrorMessage
from   helpers.io import catch


class Yosys ( object ):

    ReadRTLIL   = 0x0001
    ReadVerilog = 0x0002
    Flatten     = 0x0004
    KeepTcl     = 0x0008
    ReadMask    = ReadRTLIL | ReadVerilog

    def __init__ ( self ):
        self.flags       = Yosys.ReadRTLIL
        self.libertyFile = None
        return

    def setFlags ( self, mask, value ):
        if value: self.flags |=  mask
        else:     self.flags &= ~mask
        return

    def setFlatten ( self, value ):       self.setFlags( Yosys.Flatten, True )
    def setKeepTcl ( self, value ):       self.setFlags( Yosys.KeepTcl, True )

    def setLiberty ( self, libertyFile ):
        if not os.path.isfile(libertyFile):
          raise ErrorMessage( 1, [ 'Yosys.setLiberty(): Can\'t find liberty file.'
                                 , '"%s"' % libertyFile ] )
            
        self.libertyFile = libertyFile
        return

    def setInputRTLIL ( self ):
        self.setFlags( Yosys.ReadMask , False )
        self.setFlags( Yosys.ReadRTLIL, True  )
        return

    def setInputVerilog ( self ):
        self.setFlags( Yosys.ReadMask   , False )
        self.setFlags( Yosys.ReadVerilog, True  )
        return

    def run ( self, designName, top='top' ):
        if self.flags & Yosys.ReadRTLIL:
          if not os.path.isfile(designName+'.il'):
            raise ErrorMessage( 1, 'Yosys.run(): Can\'t find RTLIL design file "%s.il".' % designName )
          tclScript = 'yosys read_ilang   %(designName)s.il\n'
        else:
          if not os.path.isfile(designName+".v"):
            raise ErrorMessage( 1, 'Yosys.run(): Can\'t find Verilog design file "%s.v".' % designName )
          tclScript = 'yosys read_verilog %(designName)s.v\n'

        tclScript += 'yosys hierarchy -check -top %(designTop)s\n' \
                     'yosys synth            -top %(designTop)s\n'

        if self.flags & Yosys.Flatten:
          tclScript += 'yosys flatten               %(designTop)s\n' \
                       'yosys hierarchy        -top %(designTop)s\n'

        tclScript += 'yosys dfflibmap -liberty    %(libertyFile)s\n' \
                     'yosys memorydff\n'                             \
                     'yosys abc       -liberty    %(libertyFile)s\n' \
                     'yosys clean\n'                                 \
                     'yosys write_blif %(designName)s.blif\n'

        tclFile = designName + '.tcl'
        tclFd   = open( tclFile, 'w' )
        tclFd.write( tclScript % { 'designName' : designName
                                 , 'designTop'  : top
                                 , 'libertyFile': self.libertyFile
                                 } )
        tclFd.close()

        status = subprocess.call( [ 'yosys', '-c', tclFile ] )

        if not (self.flags & Yosys.KeepTcl): os.unlink( tclFile )
        return status


if __name__ == '__main__':

   parser = optparse.OptionParser()
   parser.add_option ( '-d', '--design'    , action='store'     , type='string', dest='design'   , help='The name of the design file, without extension.' )
   parser.add_option ( '-t', '--top'       , action='store'     , type='string', dest='top'
                                                                               , default='top'   , help='The hierarchy top level name model.' )
   parser.add_option ( '-f', '--flatten'   , action='store_true',                dest='flatten'  , help='Flatten the design hierarchy.' )
   parser.add_option ( '-k', '--keep-tcl'  , action='store_true',                dest='keepTcl'  , help='Keep the Yosys TCL command script.' )
   parser.add_option ( '-i', '--input-lang', action='store'     , type='string', dest='inputLang', help='Set the input description language (RTLIL or Verilog).' )
   parser.add_option ( '-l', '--liberty'   , action='store'     , type='string', dest='liberty'  , help='Set the path to the liberty file (.lib).' )
   (options, args) = parser.parse_args()

   try:
     yosys = Yosys()
     if options.inputLang == 'Verilog': yosys.setInputVerilog()
     if options.inputLang == 'RTLIL'  : yosys.setInputRTLIL()
     if options.flatten: yosys.setFlatten( True )
     if options.keepTcl: yosys.setKeepTcl( True )
     if options.liberty: yosys.setLiberty( options.liberty )
     
     rcode = yosys.run( options.design, top=options.top )

   except Exception, e:
     catch( e )
     sys.exit(2)

   sys.exit( rcode )
