
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
# |  Python      :       "./etc/common/__init__.py"                 |
# +-----------------------------------------------------------------+


from   Hurricane  import DataBase
import helpers.io
from   helpers.io import ErrorMessage


def loadGdsLayers ( gdsLayersTable ):
    technology = DataBase.getDB().getTechnology()

    for entry in gdsLayersTable:
      try:
        if len(entry) != 4:
          raise ErrorMessage( 1, ['Malformed entry in <gdsLayersTable>.'
                                 ,'Must have exactly four fields: (symb_name,real_name,GDSII_layer,GDSII_datatype).'
                                 ,str(entry)
                                 ] )
        symbName, realName, gdsiiLayer,gdsiiDatatype = entry
        if not isinstance(gdsiiLayer,int):
          raise ErrorMessage( 1, ['Incoherency in <gdsLayersTable> entry.'
                                 ,'GDSII layer number is not of int type (%s).' \
                                    % helpers.stype(gdsiiLayer)
                                 ,str(entry)
                                 ] )
        if not isinstance(gdsiiDatatype,int):
          raise ErrorMessage( 1, ['Incoherency in <gdsLayersTable> entry.'
                                 ,'GDSII layer Datatype is not of int type (%s).' \
                                    % helpers.stype(gdsiiDatatype)
                                 ,str(entry)
                                 ] )

        basicLayer = technology.getBasicLayer( symbName )
        if not basicLayer:
          raise ErrorMessage( 1, ['Incoherency in <gdsLayersTable> entry.'
                                 ,'The real layer "%s" associated to the GDSII "%s" do not exists.' \
                                    % (symbName,realName)
                                 ,str(entry)
                                 ] )

        basicLayer.setRealName    ( realName )
        basicLayer.setGds2Layer   ( gdsiiLayer )
        basicLayer.setGds2Datatype( gdsiiDatatype )

      except Exception as e:
        helpers.io.catch( e )
    return
