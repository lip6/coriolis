import os
import subprocess
from   pathlib         import Path
from   doit.exceptions import TaskFailed
from   .task           import FlowTask, ShellEnv


def guessAVERTEC_TOP ():
    modulePath = Path( __file__ )
    if modulePath.match('/usr/*'):
        avertectTop = '/usr/share/tasyag'
    else:
        avertectTop = modulePath.parents[5] / 'share' / 'tasyag'
    return avertectTop


def toArgStr ( fileList ):
    if isinstance(fileList,list):
        argStr = ''
        for f in fileList:
            if len(argStr):
                argStr += ' '
            if isinstance(f,Path):
                argStr += f.as_posix()
            else:
                argStr += str(f)
        return argStr
    if isinstance(fileList,Path):
        return fileList.as_posix()
    return str(fileList)



class MissingTarget      ( Exception ): pass
class UndefinedParameter ( Exception ): pass
class ScriptNotFound     ( Exception ): pass

class STA ( FlowTask ):
    flags               = 0
    Transistor          = 0x0001
    AVERTEC_TOP         = guessAVERTEC_TOP()
    SpiceType           = None
    SpiceTrModel        = None
    OSDIdll             = None
    MBK_SPI_MODEL       = None
    MBK_CATA_LIB        = '.'
    Temperature         = None
    VddSupply           = None
    VddName             = None
    VssName             = None
    ClockName           = None

    @staticmethod
    def mkRule ( rule, targets, depends, script, flags=0 ):
        return STA( rule, targets, depends, script, flags )

    def __init__ ( self, rule, targets, depends, script, flags ):
        super().__init__( rule, targets, depends )

        if STA.SpiceType is None:
            e = ErrorMessage( 1, 'STA.__init__(): Parameter <STA.SpiceType> has not been defined.' )
            return UndefinedParameter( e )
        if STA.SpiceTrModel is None:
            e = ErrorMessage( 1, 'STA.__init__(): Parameter <STA.SpiceTrModel> has not been defined.' )
            return UndefinedParameter( e )
        if STA.Temperature is None:
            e = ErrorMessage( 1, 'STA.__init__(): Parameter <STA.Temperature> has not been defined.' )
            return UndefinedParameter( e )
        if STA.VddSupply is None:
            e = ErrorMessage( 1, 'STA.__init__(): Parameter <STA.VddSupply> has not been defined.' )
            return UndefinedParameter( e )
        if STA.VddName is None:
            e = ErrorMessage( 1, 'STA.__init__(): Parameter <STA.VddName> has not been defined.' )
            return UndefinedParameter( e )
        if STA.VssName is None:
            e = ErrorMessage( 1, 'STA.__init__(): Parameter <STA.VssName> has not been defined.' )
            return UndefinedParameter( e )
        if STA.ClockName is None:
            e = ErrorMessage( 1, 'STA.__init__(): Parameter <STA.ClockName> has not been defined.' )
            return UndefinedParameter( e )
        if STA.MBK_SPI_MODEL is None:
            e = ErrorMessage( 1, 'STA.__init__(): Parameter <STA.MBK_SPI_MODEL> has not been defined.' )
            return UndefinedParameter( e )

        self.script = script
        if self.script and not isinstance(self.script,Path):
            self.script = Path( self.script )
        if script:
            depends.append( self.script )
        if not self.script.is_file():
            e = ErrorMessage( 1, [ 'STA.__init__(): Script not found.\n'
                                 , '<{}>'.format( self.script ) ] )
            return ScriptNotFound( e )

        self.flags      = flags
        self.inputFile  = self.file_depend(0)
        self.outputFile = self.targets[0]
        models = toArgStr( self.SpiceTrModel )
        self.command = [ 'avt_shell'
                       , self.script.as_posix()
                       , '-Target'     , self.inputFile.stem
                       , '-SpiceModel' , models
                       , '-SpiceType'  ,     STA.SpiceType
                       , '-VddVoltage' , str(STA.VddSupply)
                       , '-ClockSignal',     STA.ClockName
                       , '-VddName'    ,     STA.VddName
                       , '-VssName'    ,     STA.VssName
                       , '-Temperature', str(STA.Temperature)
                       ]
        if STA.OSDIdll:
            self.command += [ '-OsdiDll', str(STA.OSDIdll) ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from coriolis.CRL        import AllianceFramework
        from coriolis.helpers.io import ErrorMessage

        shellEnv = ShellEnv( 'STA (Tas/Yagle)' )
        shellEnv[ 'AVERTEC_TOP'   ] = STA.AVERTEC_TOP
        shellEnv[ 'MBK_SPI_MODEL' ] = self.MBK_SPI_MODEL
        shellEnv[ 'MBK_CATA_LIB'  ] = self.MBK_CATA_LIB
        shellEnv[ 'MBK_IN_LO'     ] = 'spi'
        shellEnv.export( ShellEnv.Show )
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'STA.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'STA.doTask' )

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        

class FMax ( STA ):

    scriptFMax = Path(__file__).parent / 'scripts' / 'calcCPath.tcl'

    @staticmethod
    def mkRule ( rule, targets, depends, flags=0 ):
        return FMax( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        from ..helpers.io import ErrorMessage

        depends   = FlowTask._normFileList( depends )
        #targets   = [ depends[0].with_suffix('.kdrc-report.txt') ]
        super().__init__( rule, targets, depends, FMax.scriptFMax, flags )
