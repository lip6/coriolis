# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                    Jean-Paul Chaput              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :   "./crlcore/helpers/utils.py"                   |
# +-----------------------------------------------------------------+

"""
Miscellaeous utilities. Contains:

* ``classdecorator`` : A class decorator, wrap a new class instance
  around and exsiting one and create proxies for all it's attributes
  and methods.
"""

import types
import inspect
import functools


def classdecorator ( cls ):
    """
    Decorate an instance of a class. It is not an implementation of the Design
    Pattern, instead of using Concrete/Abstract classes and inheritance, it just
    create proxies for the base class attributes & methods in the derived one.

    Example:

    .. code-block:: python

       class Base ( object ):

           def __init__ ( self, param1 ):
               self.param1 = param1

           def func1 ( self ):
               return self.param1

       @classdecorator
       class DecoratedClass ( object ):

             def __init__ ( self, param2 )
                 self.param2 = param2

             def func2 ( self ):
                 return self.param2

       base      = Base( 'Value1' )
       decorated = Decorated( 'Value2' )    
       print( 'decorated.param1 = {}'.format(decorated.param1) )
       print( 'decorated.param2 = {}'.format(decorated.param2) )
       print( 'decorated.func1  = {}'.format(decorated.func1()) )
       print( 'decorated.func2  = {}'.format(decorated.func2()) )


    Technical note
    ==============

    A hidden attribute ``_baseClass`` is added that contains an *instance* of
    the base class (that is, not the class type itself).  Then the
    ``__setattr__()`` and ``__getattr__()`` of the decorated class instance
    (``cls``) are replaced by versions that tries first to find attributes in
    the base class.  The ``__init__()`` of the decorated class is also
    redefined so that it *imports* all the methods of the base class in the
    decorated one at runtime.

    Methods binding: the base class methods importeds into the decorated one
    stay bound the the *base* class. As far as I understand, this should cause
    no problem as long as the base class instance exists, which should always
    be the case.
    """

    def isprop ( attr ): return isinstance( attr, property )

    def wrappedSetattr ( self, attr, v ):
        if attr != '_baseClass' and (attr in self._baseClass.__dict__):
            self._baseClass.__setattr__( attr, v )
        object.__setattr__( self, attr, v )
    
    def wrappedGetattr ( self, attr ):
        if attr == '_baseClass': return self.__dict__['_baseClass']
        if attr in self.__dict__: return self.__dict__[attr]
        selfClass = type( self )
        if attr in selfClass.__dict__:
            prop = selfClass.__dict__[attr]
            if isprop(prop):
                return prop.__get__(self)
            return prop
        if not hasattr(self,'_baseClass'):
            raise AttributeError( '\'{}\' object has no attribute or method named \'{}\'' \
                                  .format(self.__class__.__name__,attr) )
        return wrappedGetattr( self._baseClass, attr )

    classInit = cls.__init__

    @functools.wraps(classInit)
    def wrappedInit ( self, baseClass, *args, **kwargs ):
        self._baseClass = baseClass
        for method in inspect.getmembers(self._baseClass, predicate=inspect.ismethod):
            if method[0] == '__init__': continue
            self.__setattr__( method[0], method[1] )
        classInit( self, *args, **kwargs )

    cls.__init__    = wrappedInit
    cls.__setattr__ = wrappedSetattr
    cls.__getattr__ = wrappedGetattr
    return cls

