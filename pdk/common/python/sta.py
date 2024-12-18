import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   coriolis.designflow.task import FlowTask, ShellEnv

CalcCPathBin = Path(os.path.dirname(__file__)) / '..' / '..' / '..' / 'bin' / 'calcCPath.tcl'



class MissingTarget ( Exception ): pass

class STA ( FlowTask ):
    flags               = 0
    Transistor          = 0x0001
    VddSupply           = 1.8
    ClockName           = 'm_clock'
    SpiceType           = 'hspice'
    SpiceTrModel        = 'scn6_deep.hsp'
    MBK_CATA_LIB        = '.'
    VddName             = 'vdd'
    VssName             = 'vss'
    Temperature         = 25.0

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return STA( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )

        self.flags      = flags
        self.inputFile  = self.file_depend(0)
        self.outputFile = self.targets[0]
        if isinstance(self.SpiceTrModel, list):
          model = ' '.join(self.SpiceTrModel)
        else:
          model = self.SpiceTrModel
        self.command    = [ 'avt_shell' , str(CalcCPathBin ), '-Target', self.inputFile.stem, '-SpiceModel', model,  '-SpiceType', self.SpiceType, '-VddVoltage', str(self.VddSupply), '-ClockSignal', self.ClockName, '-VddName', self.VddName, '-VssName', self.VssName, '-Temperature', str(self.Temperature)]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from coriolis.CRL        import AllianceFramework
        from coriolis.helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv[ 'MBK_CATA_LIB'   ] = self.MBK_CATA_LIB
        shellEnv[ 'MBK_OUT_LO' ] = 'spi'
        shellEnv[ 'MBK_IN_PH' ] = 'ap'
        shellEnv.export()
        state = subprocess.run( self.command )
        # state = subprocess.run('pwd')
        if state.returncode:
            e = ErrorMessage( 1, 'STA.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'STA.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        
