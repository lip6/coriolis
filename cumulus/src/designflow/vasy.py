
import os
import re
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


def removePowerSupplies ( verilogFile ):
    patModuleDecl = re.compile( r'^\s*module\s+(?P<name>\S+)\s*\((?P<signals>[^)]*)\)' )
    patInputVdd   = re.compile( r'^\s*input\s+vdd\s*;\s*$' )
    patInputVss   = re.compile( r'^\s*input\s+vss\s*;\s*$' )
    patConnectVdd = re.compile( r'\s*\.vdd\s*\(\s*vdd\s*\)(?P<endline>.*)' )
    patConnectVss = re.compile( r'\s*\.vss\s*\(\s*vss\s*\)(?P<endline>.*)' )
    patEndComma   = re.compile( r'(?P<beginline>.*),\s*' )

    with open( verilogFile, 'r' ) as fd:
        lines = fd.readlines()

    newLines = []
    for line in lines:
        if patInputVdd.match(line): continue
        if patInputVss.match(line): continue
        m = patConnectVdd.match(line)
        if m:
            if len(m.group('endline')) and m.group('endline')[0] == ',':
                m = patEndComma.match( newLines[-1] )
                if m:
                    newLines[-1] = '{}\n'.format( m.group( 'beginline' ))
            continue
        m = patConnectVss.match(line)
        if m:
            if len(m.group('endline')) and m.group('endline')[0] == ',':
                m = patEndComma.match( newLines[-1] )
                if m:
                    newLines[-1] = '{}\n'.format( m.group( 'beginline' ))
            continue
        m = patModuleDecl.match(line)
        if m:
            newSignals = ''
            signals = m.group('signals').split(',')
            for signal in signals:
                signal = signal.strip()
                if signal == 'vdd': continue
                if signal == 'vss': continue
                if len(newSignals): newSignals += ', '
                newSignals += signal
            newLines.append( 'module {} ({});\n'.format( m.group('name'), newSignals ))
            continue
        newLines.append( line )
    fd = open( verilogFile, 'w' )
    fd.writelines( newLines )
    fd.close()


class UnsupportedVHdlSuffix ( Exception ): pass


class Vasy ( FlowTask ):

    FlagMask            = 0x00000011
    Verbose             = 0x00000001
    UseStdLogic         = 0x00000002
    AddPowerSupplies    = 0x00000004
    RemovePowerSupplies = 0x00000008
    Overwrite           = 0x00000010
    ToVerilog           = 0x00000020

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return Vasy( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.flags      = flags
        self.vhdlFile   = self.file_depend(0)
        if len(self.targets) > 1:
            self.outputFile = self.targets[1].with_suffix('.vbe')
        else:
            self.outputFile = self.targets[0]
        self.command = [ 'vasy' ]
        if flags & Vasy.Verbose:          self.command.append( '-V' )
        if flags & Vasy.UseStdLogic:      self.command.append( '-S' )
        if flags & Vasy.AddPowerSupplies: self.command.append( '-p' )
        if flags & Vasy.Overwrite:        self.command.append( '-o' )

        if self.outputFile.suffix in ('.v', '.vlog'):
            self.command.append( '-v' )
            self.flags |= Vasy.ToVerilog
        elif self.outputFile.suffix in ('.vbe', '.vst'):
            self.command.append( '-a' )
        elif self.outputFile.suffix ('.vhd',):
            self.command.append( '-s' )
        if not (self.vhdlFile.suffix in ('.vbe', '.vst', '.vhd', '.vhdl')):
            raise UnsupportedVHdlSuffix( 'Vasy.__init__(): File "{}" has unsupported VHDL suffix (not .vbe, .vst, .vhd, .vhdl).' )
        self.command += [ '-I', self.vhdlFile.suffix[1:] ]
        self.command   += [ self.vhdlFile.stem, self.outputFile.stem ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage, WarningMessage

        shellEnv = ShellEnv()
        shellEnv.export()
        print( '   -> Running "{}" ...'.format( ' '.join(self.command) ))
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Vasy.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        if self.flags & Vasy.RemovePowerSupplies:
            if self.flags & Vasy.ToVerilog:
                print( '   -> Removing power supplies "vdd" & "vss".' )
                removePowerSupplies( self.outputFile )
            else:
                print( WarningMessage( 'Vasy.doTask(): Removing power supplies only supported for Verilog.' ))
        return self.checkTargets( 'Vasy.doTask' )

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
