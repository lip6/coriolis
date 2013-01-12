
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                    Jean-Paul Chaput              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/Project.py"                         |
# +-----------------------------------------------------------------+


class Project ( object ):

    def __init__ ( self, name, tools, repository ):
        self._name       = name
        self._tools      = tools
        self._repository = repository
        self._actives    = []
        return

    def getName       ( self ): return self._name
    def getTools      ( self ): return self._tools
    def getRepository ( self ): return self._repository
    def getActives    ( self ): return self._actives
    def hasTool       ( self, tool ): return tool in self._tools

    def desactivate ( self ):
        self._active = []
        return

    def activateAll ( self ):
        self._actives = self._tools
        return
    
    def activate ( self, tools ):
       # Build the ordered list.
        for tool in self._tools:
            if (tool in tools) and not (tool in self._actives):
                self._actives += [ tool ]
       # Find the tools not part of the project.
        rejecteds = []
        for tool in tools:
            if not (tool in self._tools) and (not tool in rejecteds):
                rejecteds += [ tool ]
        return rejecteds
