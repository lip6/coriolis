
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2023-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/constants.py"              |
# +-----------------------------------------------------------------+


North      = 0x0001
South      = 0x0002
East       = 0x0004
West       = 0x0008
SouthWest  = South|West
SouthEast  = South|East
NorthWest  = North|West
NorthEast  = North|East
HORIZONTAL = North|South
VERTICAL   = East |West

Superior          = 0x0010
Inferior          = 0x0020
Inwards           = 0x0040
OnHorizontalPitch = 0x0080
OnVerticalPitch   = 0x0100


def importConstants ( symbols ):
    """
    Import chip module symbol constants in another module dictionnary
    (i.e. globals()).
    """
    if not isinstance(symbols,dict):
        print( WarningMessage( 'plugins.chip.__init__.importConstants(), argument is not a symbol table.' ))
        return
    for symbol in globals().items():
        if isinstance(symbol[1],int):
            if not symbol[0] in symbols:
                symbols[ symbol[0] ] = symbol[1]
