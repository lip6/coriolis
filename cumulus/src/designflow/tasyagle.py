import os
import subprocess
from   pathlib         import Path
from   doit.exceptions import TaskFailed
from   .task           import FlowTask, ShellEnv


def guessAVERTEC_TOP ():
    modulePath = Path( __file__ )
    if modulePath.match('/usr/*'):
        avertecTop = '/usr/share/tasyag'
    else:
        avertecTop = modulePath.parents[5] / 'share' / 'tasyag'
    return avertecTop


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

class TasYagle ( FlowTask ):
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
    def mkRule ( rule, depends, flags=0 ):
        return TasYagle( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        from coriolis.helpers.io import ErrorMessage

        super().__init__( rule, [], depends )
        self.flags     = flags
        self.inputFile = self.file_depend(0)

        if TasYagle.SpiceType is None:
            e = ErrorMessage( 1, 'TasYagle.__init__(): Parameter <TasYagle.SpiceType> has not been defined.' )
            return UndefinedParameter( e )
        if TasYagle.SpiceTrModel is None:
            e = ErrorMessage( 1, 'TasYagle.__init__(): Parameter <TasYagle.SpiceTrModel> has not been defined.' )
            return UndefinedParameter( e )
        if TasYagle.Temperature is None:
            e = ErrorMessage( 1, 'TasYagle.__init__(): Parameter <TasYagle.Temperature> has not been defined.' )
            return UndefinedParameter( e )
        if TasYagle.VddSupply is None:
            e = ErrorMessage( 1, 'TasYagle.__init__(): Parameter <TasYagle.VddSupply> has not been defined.' )
            return UndefinedParameter( e )
        if TasYagle.VddName is None:
            e = ErrorMessage( 1, 'TasYagle.__init__(): Parameter <TasYagle.VddName> has not been defined.' )
            return UndefinedParameter( e )
        if TasYagle.VssName is None:
            e = ErrorMessage( 1, 'TasYagle.__init__(): Parameter <TasYagle.VssName> has not been defined.' )
            return UndefinedParameter( e )
        if TasYagle.ClockName is None:
            e = ErrorMessage( 1, 'TasYagle.__init__(): Parameter <TasYagle.ClockName> has not been defined.' )
            return UndefinedParameter( e )
        if TasYagle.MBK_SPI_MODEL is None:
            e = ErrorMessage( 1, 'TasYagle.__init__(): Parameter <TasYagle.MBK_SPI_MODEL> has not been defined.' )
            return UndefinedParameter( e )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from coriolis.helpers.io import ErrorMessage

        binPath  = os.environ[ 'PATH' ]
        binPath  = (TasYagle.AVERTEC_TOP / 'tcl').as_posix() + ':' + binPath
        shellEnv = ShellEnv( 'TasYagle (Tas/Yagle)' )
        shellEnv[ 'AVERTEC_TOP'   ] = TasYagle.AVERTEC_TOP
        shellEnv[ 'MBK_SPI_MODEL' ] = self.MBK_SPI_MODEL
        shellEnv[ 'MBK_CATA_LIB'  ] = self.MBK_CATA_LIB
        shellEnv[ 'MBK_IN_LO'     ] = 'spi'
        shellEnv[ 'PATH'          ] = binPath
        shellEnv.export( ShellEnv.Show )
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'TasYagle.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'TasYagle.doTask' )

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }


class AvtShell ( TasYagle ):    

    @staticmethod
    def mkRule ( rule, depends, script, flags=0 ):
        return AvtShell( rule, depends, flags )

    def __init__ ( self, rule, depends, script, flags ):
        super().__init__( rule, depends, flags )

        self.script = script
        if self.script and not isinstance(self.script,Path):
            self.script = Path( self.script )
        if script:
            depends.append( self.script )
        if not self.script.is_file():
            e = ErrorMessage( 1, [ 'TasYagle.__init__(): Script not found.\n'
                                 , '<{}>'.format( self.script ) ] )
            return ScriptNotFound( e )

        models = toArgStr( self.SpiceTrModel )
        self.command = [ 'avt_shell'
                       , self.script.as_posix()
                       , '-Target'     , self.inputFile.stem
                       , '-SpiceModel' , models
                       , '-SpiceType'  ,     TasYagle.SpiceType
                       , '-VddVoltage' , str(TasYagle.VddSupply)
                       , '-ClockSignal',     TasYagle.ClockName
                       , '-VddName'    ,     TasYagle.VddName
                       , '-VssName'    ,     TasYagle.VssName
                       , '-Temperature', str(TasYagle.Temperature)
                       ]
        if TasYagle.OSDIdll:
            self.command += [ '-OsdiDll', str(TasYagle.OSDIdll) ]
        self.addClean( self.targets )
        

class STA ( AvtShell ):

    scriptSTA = Path(__file__).parent / 'scripts' / 'calcCPath.tcl'

    @staticmethod
    def mkRule ( rule, depends, flags=0 ):
        return STA( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        depends   = FlowTask._normFileList( depends )
        super().__init__( rule, depends, STA.scriptSTA, flags )
        self.targets = [ Path(self.file_depend(0).stem + '.dtx')
                       , Path(self.file_depend(0).stem + '.cpath.rep')
                       , Path(self.file_depend(0).stem + '.slack.rep')
                       , Path(self.file_depend(0).stem + '.cns')
                       , Path(self.file_depend(0).stem + '.cnv')
                       , Path(self.file_depend(0).stem + '.rcx')
                       , Path(self.file_depend(0).stem + '.rep')
                       , Path(self.file_depend(0).stem + '.stat')
                       , Path(self.file_depend(0).stem + '.stm')
                       , Path(self.file_depend(0).stem + '.sto')
                       , Path(self.file_depend(0).stem + '.str')
                       , Path(self.file_depend(0).stem + '.loop') ]
        self.addClean( self.targets )


class XTas ( TasYagle ):
    flags = 0

    @staticmethod
    def mkRule ( rule, depends, flags=0 ):
        return XTas( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        super().__init__( rule, [], depends )
        self.flags     = flags
        self.inputFile = self.file_depend(0)
        self.command = [ 'xtas'
                       ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )
