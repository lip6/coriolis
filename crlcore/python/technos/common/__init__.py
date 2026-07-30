
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
# |  Python      :       "./etc/common/__init__.py"                 |
# +-----------------------------------------------------------------+


import re
from   coriolis            import Viewer
from   coriolis.Hurricane  import DataBase
from   coriolis.helpers    import stype
from   coriolis.helpers.io import catch, ErrorMessage


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
                                    % stype(gdsiiLayer)
                                 ,str(entry)
                                 ] )
        if not isinstance(gdsiiDatatype,int):
          raise ErrorMessage( 1, ['Incoherency in <gdsLayersTable> entry.'
                                 ,'GDSII layer Datatype is not of int type (%s).' \
                                    % stype(gdsiiDatatype)
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
        catch( e )
    return


def addStyle ( datas, metalNb ):
    """
    Build a compete display style from structured set of datas.
    """
    reMetal = re.compile( r'metal(?P<nb>\d+)' )
    reCut   = re.compile( r'cut(?P<nb>\d+)' )

    style = Viewer.DisplayStyle( datas['Name'] )
    style.setDescription( datas['Description'] )
    if 'Darkening' in datas and datas['Darkening']: style.setDarkening( datas['Darkening'] )
    if 'Inherit'   in datas and datas['Inherit'  ]: style.inheritFrom ( datas['Inherit'  ] )

    for groupName, group in datas.items():
        if not isinstance(group,list): continue
        for layerStyle in group:
            m = reMetal.match( layerStyle[0] )
            if m and int(m.group('nb')) > metalNb: continue
            m = reCut.match( layerStyle[0] )
            if m and int(m.group('nb')) >= metalNb: continue
            
            arguments = { 'group' : groupName
                        , 'name'  : layerStyle[0]
                        }
            if not (layerStyle[1] is None): arguments[ 'color'     ] = layerStyle[1]
            if not (layerStyle[2] is None): arguments[ 'border'    ] = layerStyle[2]
            if not (layerStyle[3] is None): arguments[ 'pattern'   ] = layerStyle[3]
            if not (layerStyle[4] is None): arguments[ 'threshold' ] = layerStyle[4]
            style.addDrawingStyle( **arguments )
    Viewer.Graphics.addStyle( style )
