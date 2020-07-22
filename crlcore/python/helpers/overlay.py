
"""
Overlay to make some C++ objects provide a more Pythonic interface.
Contains:

* ``overlay.UpdateSession`` : to be used in ``with`` construct.
* ``overlay.Cfg`` : to be used in ``with`` construct.
"""


import Cfg
import Hurricane


class UpdateSession ( object ):
    """
    Context manager for a GO update session. See Hurricane reference manual
    for an info on Hurricane::UpdateSession class.
    """

    def __enter__ ( self ):
        Hurricane.UpdateSession.open()

    def __exit__( self, *args ):
        Hurricane.UpdateSession.close()


class Configuration:
    """
    Allow access to Cfg parameter as attributes. For attribute syntax,
    the dot (.) used in C++ or raw access is replaced by an underscore (_)
    in Python mode.

    Also provides a context manager.
    """

    PRIORITY_USE_DEFAULT         = Cfg.Parameter.Priority.UseDefault
    PRIORITY_APPLICATION_BUILTIN = Cfg.Parameter.Priority.ApplicationBuiltin
    PRIORITY_CONFIGURATION_FILE  = Cfg.Parameter.Priority.ConfigurationFile
    PRIORITY_USER_FILE           = Cfg.Parameter.Priority.UserFile
    PRIORITY_COMMAND_LINE        = Cfg.Parameter.Priority.CommandLine
    PRIORITY_INTERACTIVE         = Cfg.Parameter.Priority.Interactive

    def __init__ ( self, priority=None ):
        self._priority = priority

    def __enter__( self ):
        if self._priority is not None:
            Cfg.Configuration.pushDefaultPriority( self._priority )
        return self

    def __setattr__( self, attr, val ):
        if attr.startswith("_"):
            self.__dict__[attr] = val
            return
        attr = attr.replace("_", ".")
        if isinstance(val, bool):
            Cfg.getParamBool(attr).setBool( val )
        elif isinstance(val, int):
            p = Cfg.getParamInt( attr ) # all params have a type
            if p.type == 'Enumerate':
                Cfg.getParamEnumerate(attr).setInt( val )
            else:
                Cfg.getParamInt(attr).setInt( val )
        elif isinstance(val, long):
            p = Cfg.getParamInt( attr ) # all params have a type
        elif isinstance(val, float):
            p = Cfg.getParamDouble( attr ).setDouble( val )
        elif '%' in val:
            Cfg.getParamPercentage(attr).setPercentage( float(val[:-1]) )
        else:
            Cfg.getParamString(attr).setString( val )

    def __exit__( self, *args ):
        if self._priority is not None:
            Cfg.Configuration.popDefaultPriority()

