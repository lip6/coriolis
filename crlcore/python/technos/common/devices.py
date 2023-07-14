
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
# |  Python      :       "./common/devices.py"                      |
# +-----------------------------------------------------------------+


from coriolis.Hurricane  import DataBase
from coriolis.helpers.io import catch, ErrorMessage, WarningMessage


tech = DataBase.getDB().getTechnology()


def addDevice ( **kw ):
    global tech
    
    try:
        if 'name' in kw:
            devDesc = tech.addDeviceDescriptor( kw['name'] )
            if 'spice' in kw: devDesc.setSpiceFilePath( kw['spice'] )
            if 'connectors' in kw:
                for connector in kw['connectors']:
                    devDesc.addConnector( connector )
            else:
                print( WarningMessage( 'common.addDevice(): Missing connectors on device "{}".' \
                                       .format(kw['name'])))
            if 'layouts' in kw:
                for layout in kw['layouts']:
                    devDesc.addLayout( layout[0], layout[1] )
            else:
                print( WarningMessage( 'common.addDevice(): Missing layouts on device "{}".' \
                                       .format( kw['name'] )))
    except Exception as e:
        catch( e )
    return
