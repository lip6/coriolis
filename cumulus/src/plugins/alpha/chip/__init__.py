
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/__init__.py"               |
# +-----------------------------------------------------------------+

"""
Define constants to be used through all "chip" plugin modules, along
with the importConstants() function to import then directly in the
dictionnary of other modules.

* For corona sides: North, South, East & West.
* For corona corners: SouthWest, SouthEast, NorthWest & NorthEast.
* For rounding operation: Superior, Inferior, Inwards,
  OnHorizontalPitch & OnVerticalPitch.
"""

from __future__ import print_function
from helpers.io import WarningMessage

North     = 0x0001
South     = 0x0002
East      = 0x0004
West      = 0x0008
SouthWest = South|West
SouthEast = South|East
NorthWest = North|West
NorthEast = North|East

Superior          = 0x0010
Inferior          = 0x0020
Inwards           = 0x0040
OnHorizontalPitch = 0x0080
OnVerticalPitch   = 0x0100


def importConstants ( symbols ):
    """
    Import chip module symbol constants in the another module dictionnary
    (i.e. globals()).
    """
    if not isinstance(symbols,dict):
        print( WarningMessage( 'plugins.chip.__init__.importConstants(), argument is not a symbol table.' ))
        return
    for symbol in globals().items():
        if isinstance(symbol[1],int) or  isinstance(symbol[1],long):
            if not symbols.has_key(symbol[0]):
                symbols[ symbol[0] ] = symbol[1]
