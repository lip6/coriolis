
from pathlib         import Path
from doit.exceptions import TaskFailed
from .task           import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class PnR ( FlowTask ):
    """
    Rule to call a Coriolis ``scriptMain()`` script. If no script is provided,
    just launch an empty graphical interface.

    This class provides the expected ``doit`` function ``create_doit_tasks()``
    so it is recognized as a task.

    The action to be called by ``doit`` is provided by the ``doTask()`` function.

    To create a new instance (task) of this rule, call the static method
    ``mkRule()``.
    """

    textMode = True

    @staticmethod
    def mkRule ( rule, targets=[], depends=[], script=None ):
        """
        Creates a new rule instance (``doit`` task).

        :param rule:    The name of the rule (``basename`` for ``doit``).
        :param targets: A scalar or a list of either files or ``pathlib.Path`` instances.
        :param depends: A scalar or a list of file, ``pathlib.Path`` or other rule
                        instances. In the later case all the *targets* of the rules are
                        considered as dependencies.
        :param script:  A callable, typically a ``scriptMain()`` function. The only
                        requirement is that it should accept one keyworded argument (``**kw``).
        """
        return PnR( rule, targets, depends, script )

    def __init__ ( self, rule, targets, depends, script ):
        super().__init__( rule, targets, depends )
        self.script  = script
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<pnr {} depends=[{}]>' \
               .format( self.design, ','.join([f.as_posix() for f in self.file_dep]) )

    @property
    def design ( self ):
        if len(self.targets): return self.targets[0]
        return None

    def _setCgtBanner ( self, banner ):
        banner.setName( 'cgt' )
        banner.setPurpose( 'Coriolis Graphical Tool' )
        return banner

    def doTask ( self ):
        from ..helpers.io import ErrorMessage
        if self.design:
            print( 'PnR.doTask() on "{}"'.format( self.design ))
        else:
            print( 'PnR.doTask() run in interactive CGT mode.' )
            PnR.textMode = False
        from .. import Etesian, Anabatic, Katana, Bora, Tramontana, Tutorial, Viewer, Unicorn

        ShellEnv().export()
        if self.script and not callable(self.script):
            e = ErrorMessage( 1, 'PnR.doTask(): "script" argument is *not* callable.' )
            return TaskFailed( e )
        if self.script:
            self.script( **{} )
        if not PnR.textMode:
           # Run in graphic mode.
            ha = Viewer.HApplication.create( [] )
            Viewer.Graphics.enable()
            
            unicorn = Unicorn.UnicornGui.create()
            unicorn.setApplicationName  ( 'cgt')
            unicorn.registerTool        ( Etesian.GraphicEtesianEngine.grab() )
            unicorn.registerTool        ( Katana.GraphicKatanaEngine.grab() )
            unicorn.registerTool        ( Tramontana.GraphicTramontanaEngine.grab() )
            unicorn.registerTool        ( Bora.GraphicBoraEngine.grab() )
            unicorn.registerTool        ( Tutorial.GraphicTutorialEngine.grab() )
           #unicorn.setAnonNetSelectable(False)
            unicorn.setLayerVisible     ( "grid"          , False );
            unicorn.setLayerVisible     ( "text.instance" , False );
            unicorn.setLayerVisible     ( "text.component", False );
            self._setCgtBanner(unicorn.getBanner())
            unicorn.show()
            ha.qtExec()

        return self.checkTargets( 'PnR.doTask' )

    def create_doit_tasks ( self ):
        if self.design: doc = 'Run {}.'.format( self )
        else:           doc = 'Run plain CGT (no loaded design)'
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : doc
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        
