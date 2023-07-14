
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2019-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./etc/symbolic/cmos/display.py"           |
# +-----------------------------------------------------------------+


from coriolis.helpers    import truncPath
from coriolis.helpers.io import vprint
vprint( 2, '     - "%s".' % truncPath(__file__) )

from ...common import display

display.createStyles( scale=1.0 )
