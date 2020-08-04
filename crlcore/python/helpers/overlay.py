# -*- mode:Python; explicit-buffer-name: "__init__.py<crlcore/helpers>" -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2012-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                    Jean-Paul Chaput              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :   "./crlcore/helpers/overlay.py"                 |
# +-----------------------------------------------------------------+
#
# Those classes are based on the work of Jock Tanner from Libre-SOC.


"""
Overlay to make some C++ objects provide a more Pythonic interface.
Contains:

* ``overlay.UpdateSession`` : to be used in ``with`` construct.
* ``overlay.Configuration`` : to be used in ``with`` construct.
* ``overlay.CfgCache``      : A cache for Cfg parameters.
"""

from   __future__ import print_function
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
            p.setInt( val )
        elif isinstance(val, float):
            p = Cfg.getParamDouble( attr ).setDouble( val )
        elif '%' in val:
            Cfg.getParamPercentage(attr).setPercentage( float(val[:-1]) )
        else:
            Cfg.getParamString(attr).setString( val )

    def __exit__( self, *args ):
        if self._priority is not None:
            Cfg.Configuration.popDefaultPriority()

            
class CfgCache ( object ):
    """
    CgfCache cache a set of configuration parameters. The values of the
    parameters are not set in the system *until* the ``apply()`` function
    is called.

    If a parameter do not exists in the ``Cfg`` module, it is created
    when ``apply()`` is called. Be aware that it is not able to guess
    the right type between Double and Percentage or Int and Enumerate.
    It will, by default, only create Double or Int. So, when setting
    Percentage or Enumerate, be sure that they exists beforehand in
    the ``Cfg`` module.

    The attributes of CfgCache exactly mimic the behavior of the
    ``Cfg`` parameter string identifiers. For example:

    .. code-block:: python

       # Direct access to a Cfg parameter.
       p = Cfg.getParamInt('katana.eventsLimit').setInt( 4000000 )

       # Setup of a CfgCache parameter.
       cache = CfgCache('')
       cache.katana.eventsLimit = 4000000

       # ...
       # Effective setting of the Cfg parameter.
       cache.apply()


    This is done by overloading ``__setattr__()`` and ``__getattr__()``
    which recursively create CfgCache objects for intermediate levels
    attributes (in the previous example, a CfgCache for ``katana``
    will automatically be created). To separate between attributes
    that are part of configuration parameters and attributes belonging
    to CfgCache itself, we prepend a '_' to the laters.

    .. note:: It is important to understand the difference of behavior
              with ``Configuration``, the former set the parameters
              at once, it directly act on the ``Cfg`` settings.
              The later keep a state and set the ``Cfg`` parameters
              *only* when ``apply()`` is called.
    """

    @staticmethod
    def setCfgParameter ( paramPath, value ):
        """"
        Set the value of parameter ``paramPath`` to ``value`` in Cfg.
        Percentage are set as Double and Enumerate as Int.
        """
        if Cfg.hasParameter(paramPath):
            confDb = Cfg.Configuration.get()
            p      = confDb.getParameter( paramPath )
        else:
            if   isinstance(value,bool ): p = Cfg.getParamBool  ( paramPath )
            elif isinstance(value,int  ): p = Cfg.getParamInt   ( paramPath )
            elif isinstance(value,long ): p = Cfg.getParamInt   ( paramPath )
            elif isinstance(value,float): p = Cfg.getParamDouble( paramPath )
            else:                         p = Cfg.getParamString( paramPath )

        if   p.type == Cfg.Parameter.Type.Enumerate:  p.setInt   ( value )
        elif p.type == Cfg.Parameter.Type.Int:        p.setInt   ( value )
        elif p.type == Cfg.Parameter.Type.Bool:       p.setBool  ( value )
        elif p.type == Cfg.Parameter.Type.Double:     p.setDouble( value )
        elif p.type == Cfg.Parameter.Type.Percentage: p.setDouble( value*100.0 )
        else:                                         p.setString( str(value) )

    @staticmethod
    def getDefaultCfgParameter ( paramPath ):
        """"Get the value of parameter ``paramPath`` from Cfg."""
        if not Cfg.hasParameter(paramPath):
            raise AttributeError( 'CfgCache.getDefaultCfgParameter(): Undefined "{}"'.format(paramPath) )
        confDb = Cfg.Configuration.get()
        p      = confDb.getParameter( paramPath )
        if p:
            if p.type == Cfg.Parameter.Type.Enumerate:  return p.asInt()
            if p.type == Cfg.Parameter.Type.Int:        return p.asInt()
            if p.type == Cfg.Parameter.Type.Bool:       return p.asBool()
            if p.type == Cfg.Parameter.Type.String:     return p.asString()
            if p.type == Cfg.Parameter.Type.Double:     return p.asDouble()
            if p.type == Cfg.Parameter.Type.Percentage: return p.asDouble()/100.0
            return p.asString()

    def __enter__( self ):
        return self

    def __exit__( self, *args ):
        self.apply()
        self.display()

    def __init__ ( self, path='', priority=None ):
        """Create a new CfgCache with a ``path`` as parent path."""
        self._priority = priority
        self._path     = path
        self._rattr    = {}
        return

    def __setattr__ ( self, attr, v ):
        """
        Recursively set an attribute. Attributes names starting by an '_' are
        treated as belonging to *this* object (self).

        How does the recursive attributes/CfgCache works? Assumes that we
        are doing:

        .. code-block:: python

           # Setup of a CfgCache parameter.
           cache = CfgCache('')
           cache.katana.eventsLimit = 4000000

        The explicit call sequence will be:

        .. code-block:: python

           cache.__getattr__('katana').__setattr__( 'eventsLimit', 4000000 )

        1. For the intermediate hierarchy level ``katana``, it is __getattr__()
           which is called, if the attribute do not exists, we create a new
           CfgCache().

        2. Second, and only then, __setattr__() is called, which will create a
           parameter entry named ``eventsLimit``.

        The decision of whether create a parameter entry *or* a CfgCache
        intermediate level will always be correctly handled because prior
        to any access, an attribute needs to be set. So we always have
        first a call chain of __getattr__() with one final __setattr__().
        For any subsequent access to ``cache.katana.eventsLimit``, as
        the attribute already exists, there is no type creation problem.
        """
        if attr[0] == '_':
            object.__setattr__( self, attr, v )
            return
        if v is None:
            v = CfgCache.getDefaultCfgParameter( self._path+'.'+attr )
        self._rattr[ attr ] = v
        return

    def __getattr__ ( self, attr ):
        """
        Get an attribute, if it doesn't exists, then we are in an intermediate
        level like ``katana``, so create a new sub CfgCache for that attribute.
        """
        if not self._rattr.has_key(attr):
            path = self._path+'.'+attr if len(self._path) else attr
            self._rattr[attr] = CfgCache( path, self._priority )
        return self._rattr[attr]

    def apply ( self, priority=None ):
        """Apply the parameters values stored in the cache to the ``Cfg`` database."""
        if priority is None: priority = self._priority
        if not len(self._path) and priority is not None:
            Cfg.Configuration.pushDefaultPriority( priority )
        for attrName in self._rattr.keys():
            if isinstance(self._rattr[attrName],CfgCache):
                self._rattr[attrName].apply()
                continue
            CfgCache.setCfgParameter( self._path+'.'+attrName,self._rattr[attrName] )
        if not len(self._path) and priority is not None:
            Cfg.Configuration.popDefaultPriority()

    def display ( self ):
        """Print all the parameters stored in that CfgCache."""
        if not len(self._path):
            print( '  o  Applying configuration (CfgCache):' )
        for attrName in self._rattr.keys():
            if isinstance(self._rattr[attrName],CfgCache):
                self._rattr[attrName].display()
                continue
            print( '     - {}.{} = {}'.format(self._path,attrName,self._rattr[attrName]) )

