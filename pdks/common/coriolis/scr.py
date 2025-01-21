
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   coriolis.designflow.task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class SCR ( FlowTask ):

    Route               = 0x0001
    Place               = 0x0002
    Iterations          = 0x0004
    Iterations_value    = 0
    SliceNumber         = 0x0008
    Slice_value         = 0
    SupplyNumber        = 0x0010
    Supply_value        = 0
    ChannelName         = 0x0020
    Channel_str         = 'c'
    RandSeed            = 0x0040
    RandSeed_value      = 0
    MaxRetry            = 0x0080
    MaxRetry_value      = 0
    MBK_CATA_LIB        = '.'

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return SCR( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.flags      = flags
        self.inputFile  = self.file_depend(0)
        self.outputFile = self.targets[0]
        self.command    = [ 'scr' ]
        if flags & SCR.Route:        self.command.append( '-r' )
        if flags & SCR.Place:        self.command.append( '-p' )
        if flags & SCR.Iterations:   self.command.extend( ['-i' ,str(SCR.Iterations_value)])
        if flags & SCR.SliceNumber : self.command.extend( ['-l' ,str(SCR.Slice_value)])
        if flags & SCR.SupplyNumber: self.command.extend( ['-a' ,str(SCR.Supply_value)])
        if flags & SCR.ChannelName:  self.command.extend( ['-c' ,SCR.Channel_str ])
        if flags & SCR.RandSeed   :  self.command.extend( ['-s' ,str(SCR.RandSeed_value) ])
        if flags & SCR.MaxRetry   :  self.command.extend( ['-M' ,str(SCR.MaxRetry_value) ])
        self.command   += [ '-o',  self.outputFile.stem, self.inputFile.stem ]
        self.commandvst    = [ 'x2y' ]
        self.commandvst.extend( ['vst','vst', self.inputFile.stem, self.outputFile.stem] )
        self.commandspi    = [ 'x2y' ]
        self.commandspi.extend( ['vst','spi', self.inputFile.stem, self.outputFile.stem] )
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from coriolis.CRL        import AllianceFramework
        from coriolis.helpers.io import ErrorMessage
        shellEnv = ShellEnv()
        shellEnv[ 'MBK_CATA_LIB'   ] = self.MBK_CATA_LIB
        shellEnv.export()

        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'SCR.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        statevst = subprocess.run( self.commandvst )
        if statevst.returncode:
            e = ErrorMessage( 1, 'SCR.doTask(): UNIX command failed ({}).' \
                                 .format( statevst.returncode ))
            return TaskFailed( e )
        statespi = subprocess.run( self.commandspi )
        if statespi.returncode:
            e = ErrorMessage( 1, 'SCR.doTask(): UNIX command failed ({}).' \
                                 .format( statespi.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'SCR.doTask' )

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        
