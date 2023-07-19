# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2012-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
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

from ..  import Cfg
from ..  import Hurricane
from .io import isVL


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
        elif isinstance(val, float):
            p = Cfg.getParamDouble( attr ).setDouble( val )
        elif '%' in val:
            Cfg.getParamPercentage(attr).setPercentage( float(val[:-1]) )
        else:
            Cfg.getParamString(attr).setString( val )

    def __exit__( self, *args ):
        if self._priority is not None:
            Cfg.Configuration.popDefaultPriority()


class CfgDefault ( object ):
    """
    Define a builtin default value. This value will be used as default
    if the associated parameter do not have a value either set on disk
    nor through progamming.

    It is used as a very last ditch to get a value for a parameter.
    """

    def __init__ ( self, v ):
        self.vDefault = v

    def __str__ ( self ):
        return 'CfgDefault({})'.format( self.vDefault )


class CachedParameter ( object ):

    def __init__ ( self, path, v ):
        self.path      = path
        self._v        = None
        self.vRange    = [ None, None ]
        self.vEnum     = []
        self.create    = True
        if not isinstance(v,CfgDefault):
            self.v = v
        self.cacheRead()

    @property
    def v ( self ): return self._v

    @v.setter
    def v ( self, value ):
        if value is None: return
        if isinstance(value,CfgDefault):
            if self._v is None:
                self._v = value.vDefault
            return
        self._v = value

    def __str__ ( self ):
        if isinstance(self.v,str): s = '"{}"'.format(self.v)
        else: s = '{}'.format(self.v)
        if self.vRange[0] is not None or self.vRange[1] is not None:
            s += ' [{}:{}]'.format(self.vRange[0],self.vRange[1])
        if self.vEnum:
            s += ' ('
            for i in range(len(self.vEnum)):
                if i: s += ', '
                s += '{}:"{}"'.format(self.vEnum[i][1],self.vEnum[i][0])
            s += ')'
        return s

    def cacheWrite ( self ):
        """"
        Commit the value of parameter ``self.path`` to ``self.v`` in Cfg.
        Percentage are set as Double and Enumerate as Int.
        """
        if Cfg.hasParameter(self.path):
            confDb = Cfg.Configuration.get()
            p      = confDb.getParameter( self.path )
        else:
            if   len(self.vEnum):          p = Cfg.getParamEnumerate( self.path )
            elif isinstance(self.v,bool ): p = Cfg.getParamBool     ( self.path )
            elif isinstance(self.v,int  ): p = Cfg.getParamInt      ( self.path )
            elif isinstance(self.v,float): p = Cfg.getParamDouble   ( self.path )
            else:                          p = Cfg.getParamString   ( self.path )
        if   p.type == Cfg.Parameter.Type.Enumerate:  p.setInt      ( self.v )
        elif p.type == Cfg.Parameter.Type.Int:        p.setInt      ( self.v )
        elif p.type == Cfg.Parameter.Type.Bool:       p.setBool     ( self.v )
        elif p.type == Cfg.Parameter.Type.Double:     p.setDouble   ( self.v )
        elif p.type == Cfg.Parameter.Type.Percentage: p.setDouble   ( self.v*100.0 )
        else:                                         p.setString   ( str(self.v) )
        if self.create:
            if len(self.vEnum):
                for item in self.vEnum:
                    p.addValue( item[0], item[1] )
        if self.vRange[0] is not None: p.setMin( self.vRange[0] )
        if self.vRange[1] is not None: p.setMax( self.vRange[1] )

    def cacheRead ( self ):
        """"Get the value of parameter ``self.path`` from Cfg."""
        if not Cfg.hasParameter(self.path):
            self.create = True
            return
        if self.v is not None: return
        confDb = Cfg.Configuration.get()
        p      = confDb.getParameter( self.path )
        if p:
            if   p.type == Cfg.Parameter.Type.Enumerate:  self.v = p.asInt()
            elif p.type == Cfg.Parameter.Type.Int:
                self.v = p.asInt()
            elif p.type == Cfg.Parameter.Type.Bool:       self.v = p.asBool()
            elif p.type == Cfg.Parameter.Type.String:     self.v = p.asString()
            elif p.type == Cfg.Parameter.Type.Double:     self.v = p.asDouble()
            elif p.type == Cfg.Parameter.Type.Percentage: self.v = p.asDouble()/100.0
            else: self.v = p.asString()

            
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

    If a cache parameter is assigned to ``None``, it triggers the
    loading of the value from the disk, it it exists.

    .. code-block:: python

       # Setup of a CfgCache parameter.
       cache = CfgCache('')
       cache.katana.eventsLimit = None
       # The parameter will read it's value from the disk (4000000).


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

    def __enter__( self ):
        return self

    def __exit__( self, *args ):
        self.apply()
        if isVL(2): self.display()

    def __init__ ( self, path='', priority=None ):
        """Create a new CfgCache with a ``path`` as parent path."""
        self._priority = priority
        self._path     = path
        self._rattr    = {}

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
        vRange = None
        vEnum  = None
        if isinstance(v,list ): vRange = v; v = None
        if isinstance(v,tuple): vEnum  = v; v = None
        if not attr in self._rattr:
            self._rattr[ attr ] = CachedParameter( self._path+'.'+attr, v )
        if   vRange is not None: self._rattr[ attr ].vRange = vRange
        elif vEnum  is not None: self._rattr[ attr ].vEnum  = vEnum
        else: self._rattr[ attr ].v = v

    def __getattr__ ( self, attr ):
        """
        Get an attribute, if it doesn't exists, then we are in an intermediate
        level like ``katana``, so create a new sub CfgCache for that attribute.
        """
        if not attr in self._rattr:
            path = self._path+'.'+attr if len(self._path) else attr
            self._rattr[attr] = CfgCache( path, self._priority )
        if isinstance(self._rattr[attr],CachedParameter):
            return self._rattr[attr].v
        return self._rattr[attr]

    def _hasCachedParam ( self, elements ):
        if not elements[0] in self._rattr:
            return False
        if len(elements) == 1:
            return True
        rattr = self._rattr[ elements[0] ]
        if not isinstance(rattr,CfgCache):
            return False
        return rattr._hasCachedParam( elements[1:] )

    def hasCachedParam ( self, attr ):
        return self._hasCachedParam( attr.split('.') )

    def apply ( self, priority=None ):
        """Apply the parameters values stored in the cache to the ``Cfg`` database."""
        if priority is None: priority = self._priority
        if not len(self._path) and priority is not None:
            Cfg.Configuration.pushDefaultPriority( priority )
        for attrName in self._rattr.keys():
            if isinstance(self._rattr[attrName],CfgCache):
                self._rattr[attrName].apply()
                continue
            self._rattr[attrName].cacheWrite()
        if not len(self._path) and priority is not None:
            Cfg.Configuration.popDefaultPriority()
       #self.display()

    def display ( self ):
        """Print all the parameters stored in that CfgCache."""
        if not len(self._path):
            print( '  o  Configuration contents (CfgCache):' )
        for attrName in self._rattr.keys():
            if isinstance(self._rattr[attrName],CfgCache):
                self._rattr[attrName].display()
                continue
            print( '     - {}.{} = {}'.format(self._path,attrName,self._rattr[attrName]) )

