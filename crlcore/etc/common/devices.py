
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2019-2019, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./common/devices.py"                      |
# +-----------------------------------------------------------------+


from   Hurricane  import DataBase
import helpers.io
from   helpers.io import ErrorMessage
from   helpers.io import WarningMessage


tech = DataBase.getDB().getTechnology()


def addDevice ( **kw ):
    global tech
    
    try:
      if kw.has_key('name'):
        devDesc = tech.addDeviceDescriptor( kw['name'] )

        if kw.has_key('spice'):
          devDesc.setSpiceFilePath( kw['spice'] )
        
        if kw.has_key('connectors'):
          for connector in kw['connectors']:
            devDesc.addConnector( connector )
        else:
          print WarningMessage( 'common.addDevice(): Missing connectors on device "%s".' % kw['name' ])
        
        if kw.has_key('layouts'):
          for layout in kw['layouts']:
            devDesc.addLayout( layout[0], layout[1] )
        else:
          print WarningMessage( 'common.addDevice(): Missing layouts on device "%s".' % kw['name' ])
    except Exception, e:
      helpers.io.catch( e )
    return
