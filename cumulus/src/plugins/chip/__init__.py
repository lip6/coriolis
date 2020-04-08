# -*- explicit-buffer-name: "__init__.py<cumulus/plugins/chip>" -*-
#
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


from helpers.io import WarningMessage


# Common constants used through all <chip> modules.

# For Corona's sides.
North     = 0x0001
South     = 0x0002
East      = 0x0004
West      = 0x0008

# For Corona's corners.
SouthWest = South|West
SouthEast = South|East
NorthWest = North|West
NorthEast = North|East

# For rounding functions.
Superior          = 0x0010
Inferior          = 0x0020
Inwards           = 0x0040
OnHorizontalPitch = 0x0080
OnVerticalPitch   = 0x0100


def importConstants ( symbols ):
    if not isinstance(symbols,dict):
      print WarningMessage( 'plugins.chip.__init__.importConstants(), argument is not a symbol table.' )
      return

    for symbol in globals().items():
      if isinstance(symbol[1],int) or  isinstance(symbol[1],long):
        if not symbols.has_key(symbol[0]):
          symbols[ symbol[0] ] = symbol[1]
  
    return
