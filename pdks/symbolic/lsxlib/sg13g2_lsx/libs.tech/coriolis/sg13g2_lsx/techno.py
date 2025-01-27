
from coriolis                         import CRL, Hurricane, Viewer, Cfg
from coriolis.Hurricane               import Technology, DataBase, DbU, Library, Layer, BasicLayer,      \
                                             RegularLayer, Cell, Net, Horizontal, Vertical, Rectilinear, \
                                             Box, Point, NetExternalComponents
from coriolis.technos.common.colors   import toRGB
from coriolis.technos.common.patterns import toHexa
from coriolis.helpers                 import u
from coriolis.helpers.technology      import createBL, createVia
from coriolis.helpers.overlay         import CfgCache
from coriolis.helpers.analogtechno    import Length, Area, Unit, Asymmetric, loadAnalogTechno, addDevice
from coriolis.designflow.task         import ShellEnv


__all__ = [ "setup" ]


"""
Coriolis Design Technological Rules (DTR) for IHP 130nm BiCMOS General Purpose
=================================================================================

:Version: rev.LIP6-1
:Date:    May 22, 2024
:Authors: Naohiko Shimizu

Reference documents: 
    SG13G2 Process Specification Rev.1.2
    SG13G2 Open Source Layout Rules Rev.0.2 (2024-03-08)

=====================  =======  ============  ====================================

"""


analogTechnologyTable = \
    ( ('Header', 'Sg13g2', DbU.UnitPowerMicro, 'rev.LIP6-1')
    # ------------------------------------------------------------------------------------
    # ( Rule name          , [Layer1]  , [Layer2]  , Value , Rule flags       , Reference )
    , ('physicalGrid'                              , 0.001 , Length           , 'Grid Rules')
    , ('transistorMinL'                            , 0.13  , Length           , 'Gat.a')
    #, ('transistorMinL'                            , 0.38  , Length           , 'lvtn.1a')
    #, ('transistorMaxL'                            , 38    , Length           , 'rule0002')
    #, ('transistorMinW'                            , 0.42  , Length           , 'activ.2')
    #, ('transistorMinW'                            , 0.36  , Length           , 'activ.2b')
    #, ('transistorMaxW'                            , 4000  , Length           , 'rule0004')

    # N-WELL (nwm)
    , ('minWidth'          , 'nwm'                 , 0.62  , Length           , 'NW.a')
    , ('minSpacing'        , 'nwm'                 , 0.62  , Length           , 'NW.b')
    , ('minArea'           , 'nwm'                 , 0     , Area             , 'N/A')

    # LVTN (lvtn)
    #, ('minWidth'          , 'lvtn'                , 0.38  , Length           , 'lvtn.1a')
    #, ('minSpacing'        , 'lvtn'                , 0.38  , Length           , 'lvtn.2')
    #, ('minArea'           , 'lvtn'                , 0.265 , Area             , 'lvtn.13')
    #, ('minEnclosure'      , 'nwm'     , 'lvtn'    , 0.38  , Length|Asymmetric, 'lvtn.10')

    # ACTIV (activ)
    , ('minWidth'          , 'activ'               , 0.15  , Length           , 'Act.a')
    , ('minSpacing'        , 'activ'               , 0.21  , Length           , 'Act.b')
    , ('minArea'           , 'activ'               , 0.122 , Area             , 'Act.d')
    , ('minEnclosure'      , 'nwm'     , 'activ'   , 0.31  , Length|Asymmetric, 'NW.c')

    # Poly1 (poly)
    , ('minWidth'          , 'poly'                , 0.13  , Length           , 'Gat.a')
    , ('minSpacing'        , 'poly'                , 0.18  , Length           , 'Gat.b')
    , ('minGateSpacing'    , 'poly'                , 0.18  , Length           , 'Gat.b')
    , ('minArea'           , 'poly'                , 0.09  , Area             , 'Gat.e')
    , ('minSpacing'        , 'poly'    , 'activ'   , 0.07  , Length           , 'Gat.d')
    , ('minExtension'      , 'poly'    , 'activ'   , 0.180 , Length|Asymmetric, 'Gat.c')
    , ('minGateExtension'  , 'activ' , 'poly'      , 0.23  , Length|Asymmetric, 'Act.c')
    , ('minExtension'      , 'activ' , 'poly'      , 0.23  , Length|Asymmetric, 'Act.c')

    # 4.1.6 PPLUS (psdm)
    , ('minWidth'          , 'psdm'                , 0.31  , Length           , 'pSD.a')
    , ('minSpacing'        , 'psdm'                , 0.31  , Length           , 'pSD.b')
    , ('minArea'           , 'psdm'                , 0.25  , Area             , 'pSD.k')
    , ('minSpacing'        , 'psdm'    , 'activ'   , 0.180 , Length           , 'pSD.d')
    , ('minGateExtension'  , 'psdm'    , 'poly'    , 0.00  , Length|Asymmetric, 'N/A')
    , ('minOverlap'        , 'psdm'    , 'activ'   , 0.30  , Length           , 'pSD.e')
    , ('minEnclosure'      , 'psdm'    , 'activ'   , 0.180 , Length|Asymmetric, 'pSD.c')
    , ('minStrapEnclosure' , 'psdm'    , 'activ'   , 0.180 , Length           , 'pSD.c')
    , ('minSpacing'        , 'nsdm'    , 'psdm'    , 0.00  , Length           , 'N/A')
    , ('minEnclosure'      , 'psdm'    , 'poly'    , 0.30  , Length|Asymmetric, 'pSD.i')
    , ('minLengthEnclosure', 'psdm'    , 'activ'   , 0     , Length|Asymmetric, 'N/A')
    , ('minWidthEnclosure' , 'psdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'psdm'    , 'activ'   , 0.180 , Length|Asymmetric, 'dup. pSD.c')
    , ('minStrapEnclosure' , 'psdm'                , 0.180 , Length           , 'dup. pSD.c')

    # NPLUS (nsdm) no nSD rules
    #, ('minWidth'          , 'nsdm'                , 0.38  , Length           , 'nsd.1')
    #, ('minSpacing'        , 'nsdm'                , 0.38  , Length           , 'nsd.2')
    #, ('minArea'           , 'nsdm'                , 0.265 , Area             , 'nsd.10a')
    #, ('minSpacing'        , 'nsdm'    , 'activ'   , 0.130 , Length           , 'nsd.7')
    #, ('minGateExtension'  , 'nsdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    #, ('minOverlap'        , 'nsdm'    , 'activ'   , 0     , Length           , 'N/A')
    #, ('minEnclosure'      , 'nsdm'    , 'activ'   , 0.125 , Length|Asymmetric, 'nsd.5a')
    #, ('minStrapEnclosure' , 'nsdm'    , 'activ'   , 0.125 , Length           , 'nsd.5b')
    #, ('minEnclosure'      , 'nsdm'    , 'nwm'     , 0     , Length|Asymmetric, 'N/A')
    #, ('minEnclosure'      , 'nsdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    #, ('minLengthEnclosure', 'nsdm'    , 'activ'   , 0     , Length|Asymmetric, 'N/A')
    #, ('minWidthEnclosure' , 'nsdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    #, ('minGateEnclosure'  , 'nsdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    # Error: duplicated rule, needed by "old Pharos".
    #, ('minExtension'      , 'nsdm'    , 'activ'   , 0.125 , Length|Asymmetric, 'dup. nsd.5a')
    #, ('minStrapEnclosure' , 'nsdm'                , 0.215 , Length           , 'dup. nsd.5b')

    # LICM1 (CONT)
    , ('minWidth'          , 'cont'                , 0.16  , Length           , 'Cnt.a')
    , ('minSpacing'        , 'cont'                , 0.18  , Length           , 'Cnt.b')
    , ('minGateSpacing'    , 'cont'    , 'poly'    , 0.11  , Length|Asymmetric, 'Cnt.f')
    , ('minSpacing'        , 'cont'    , 'poly'    , 0.11  , Length|Asymmetric, 'Cnt.f')
    , ('minSpacing'        , 'cont'    , 'activ'   , 0.14  , Length           , 'Cnt.e')
    #, ('minSpacing'        , 'cont'    , 'activ'   , 0.06  , Length           , 'cont.5b')
    , ('minEnclosure'      , 'activ'   , 'cont'    , 0.07  , Length|Asymmetric, 'Cnt.c')
    , ('minEnclosure'      , 'poly'    , 'cont'    , 0.07  , Length|Asymmetric, 'Cnt.d')
    #, ('minEnclosure'      , 'psdm'    , 'cont'    , 0     , Length|Asymmetric, 'N/A')
    #, ('minEnclosure'      , 'nsdm'    , 'cont'    , 0     , Length|Asymmetric, 'N/A')
    #, ('minGateEnclosure'  , 'psdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'poly'    , 'cont'    , 0.07  , Length|Asymmetric, 'dup. Cnt.d')
    , ('minExtension'      , 'psdm'    , 'cont'    , 0.09  , Length|Asymmetric, 'Cnt.g2')
    #, ('minExtension'      , 'nsdm'    , 'cont'    , 0.25  , Length|Asymmetric, 'dup.')

    # LI1M (M1)
    , ('minWidth'          , 'M1'                  , 0.16  , Length           , 'M1.a')
    , ('minSpacing'        , 'M1'                  , 0.18  , Length           , 'M1.b')
    , ('minArea'           , 'M1'                  , 0.09  , Area             , 'M1.d')
    , ('minEnclosure'      , 'M1'      , 'cont'    , 0.05  , Length|Asymmetric, 'M1.c1')
    , ('minEnclosure'      , 'M1'      , 'via12'   , 0.05  , Length|Asymmetric, 'Mn.c1')

    # CTM1 (via12)
    , ('minWidth'          , 'via12'                , 0.19  , Length           , 'V1.a')
    , ('minSpacing'        , 'via12'                , 0.22  , Length           , 'V1.b')


    # MM1 (M2)
    , ('minWidth'          , 'M2'                  , 0.20  , Length           , 'Mn.a')
    , ('minSpacing'        , 'M2'                  , 0.21  , Length           , 'Mn.b')
    , ('minArea'           , 'M2'                  , 0.144 , Area             , 'Mn.d')
    , ('minEnclosure'      , 'M2'      , 'via12'    , 0.05  , Length|Asymmetric, 'Mn.c1')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'M2'      , 'via12'    , 0.05  , Length|Asymmetric, 'Mn.c1')

    # VIM (via23)
    , ('minWidth'          , 'via23'                 , 0.19  , Length           , 'Vn.a')
    , ('minSpacing'        , 'via23'                 , 0.22  , Length           , 'Vn.b')
    , ('minEnclosure'      , 'M2'      , 'via23'     , 0.05  , Length|Asymmetric, 'Vn.c1')
    # Error: duplicated rule, needed by "old Pharos".
    , ('lineExtension'     , 'M2'      , 'via23'     , 0.05  , Length|Asymmetric, 'dup. Vn.c1')


    # MM1 (M3)
    , ('minWidth'          , 'M3'                  , 0.20  , Length           , 'Mn.a')
    , ('minSpacing'        , 'M3'                  , 0.21  , Length           , 'Mn.b')
    , ('minArea'           , 'M3'                  , 0.144 , Area             , 'Mn.d')
    , ('minEnclosure'      , 'M3'      , 'via23'    , 0.05  , Length|Asymmetric, 'Mn.c1')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'M3'      , 'via23'    , 0.05  , Length|Asymmetric, 'Mn.c1')

    # VIM (via34)
    , ('minWidth'          , 'via34'                 , 0.19  , Length           , 'Vn.a')
    , ('minSpacing'        , 'via34'                 , 0.22  , Length           , 'Vn.b')
    , ('minEnclosure'      , 'M3'      , 'via34'     , 0.05  , Length|Asymmetric, 'Vn.c1')
    # Error: duplicated rule, needed by "old Pharos".
    , ('lineExtension'     , 'M3'      , 'via34'     , 0.05  , Length|Asymmetric, 'dup. Vn.c1')

    # MM1 (M4)
    , ('minWidth'          , 'M4'                  , 0.20  , Length           , 'Mn.a')
    , ('minSpacing'        , 'M4'                  , 0.21  , Length           , 'Mn.b')
    , ('minArea'           , 'M4'                  , 0.144 , Area             , 'Mn.d')
    , ('minEnclosure'      , 'M4'      , 'via34'    , 0.05  , Length|Asymmetric, 'Mn.c1')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'M4'      , 'via34'    , 0.05  , Length|Asymmetric, 'Mn.c1')

    # VIM (via45)
    , ('minWidth'          , 'via45'                 , 0.19  , Length           , 'Vn.a')
    , ('minSpacing'        , 'via45'                 , 0.22  , Length           , 'Vn.b')
    , ('minEnclosure'      , 'M4'      , 'via45'     , 0.05  , Length|Asymmetric, 'Vn.c1')
    # Error: duplicated rule, needed by "old Pharos".
    , ('lineExtension'     , 'M4'      , 'via45'     , 0.05  , Length|Asymmetric, 'dup. Vn.c1')

    # MM5 (M5)
    , ('minWidth'          , 'M5'                  , 0.20  , Length           , 'Mn.a')
    , ('minSpacing'        , 'M5'                  , 0.21  , Length           , 'Mn.b')
    , ('minArea'           , 'M5'                  , 0.144 , Area             , 'Mn.d')
    , ('minEnclosure'      , 'M5'      , 'via45'    , 0.05  , Length|Asymmetric, 'Mn.c1')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'M5'      , 'via45'    , 0.05  , Length|Asymmetric, 'Mn.c1')

    # VIM (via56 TopVia1)
    , ('minWidth'          , 'via56'                 , 0.42  , Length           , 'TV1.a')
    , ('minSpacing'        , 'via56'                 , 0.42  , Length           , 'TV1.b')
    , ('minEnclosure'      , 'M5'      , 'via56'     , 0.1   , Length|Asymmetric, 'TV1.c')
    # Error: duplicated rule, needed by "old Pharos".
    , ('lineExtension'     , 'M5'      , 'via56'     , 0.1   , Length|Asymmetric, 'dup. TV1.c')

    # MM6 (M6) TopMetal1
    , ('minWidth'          , 'M6'                  , 1.64  , Length           , 'TM1.a')
    , ('minSpacing'        , 'M6'                  , 1.64  , Length           , 'TM1.b')
    #, ('minArea'           , 'M6'                  , 0.24  , Area             , 'M5.4a')
    , ('minEnclosure'      , 'M6'      , 'via56'    , 0.10 , Length|Asymmetric, 'TV1.c')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'M6'      , 'via56'    , 0.10 , Length|Asymmetric, 'dup. TV1.c ')

    # VIM (via56 TopVia2)
    , ('minWidth'          , 'via56'                 , 0.90  , Length           , 'TV2.a')
    , ('minSpacing'        , 'via56'                 , 1.06  , Length           , 'TV2.b')
    , ('minEnclosure'      , 'M6'      , 'via56'     , 0.5   , Length|Asymmetric, 'TV2.c')
    # Error: duplicated rule, needed by "old Pharos".
    , ('lineExtension'     , 'M6'      , 'via56'     , 0.5   , Length|Asymmetric, 'dup. TV2.c')

    # MM5 (M7) TopMetal2
    , ('minWidth'          , 'M7'                  , 2.00  , Length           , 'TM2.a')
    , ('minSpacing'        , 'M7'                  , 2.00  , Length           , 'TM2.b')
    #, ('minArea'           , 'M7'                  , 0.24  , Area             , 'M5.4a')
    , ('minEnclosure'      , 'M7'      , 'via56'    , 0.50 , Length|Asymmetric, 'TV1.d')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'M7'      , 'via56'    , 0.50 , Length|Asymmetric, 'dup. TV1.d ')



    #capm
    #, ('minWidth'          , 'metcap'              , 1.0  , Length           , 'capm.1')
    #, ('minWidth'          , 'metcapdum'           , 0.5  , Length           , '')
    #, ('maxWidth'          , 'metcap'              , 300.0 , Length           , '')
    #, ('maxWidth'          , 'metbot'              , 350.0 , Length           , '')
    #, ('minSpacing'        , 'metcap'              , 0.84 , Length           , 'capm.2a')
    #, ('minSpacing'        , 'metbot'              , 0.8  , Length           , 'metcap.2b')
    #, ('minSpacing'        , 'cut1'    , 'metcap'  , 0.50 , Length           , '')
    #, ('minSpacing'        , 'cut2'    , 'metcap'  , 0.50 , Length           , 'capm.5')
    #, ('minSpacingOnMetbot', 'cut2'                , 0.2  , Length           , 'via34.2')
    #, ('minSpacingOnMetbot', 'via34'                , 0.2  , Length           , 'via34.2')
    #, ('minSpacingOnMetcap', 'cut2'                , 0.2  , Length           , 'via34.2')
    #, ('minEnclosure'      , 'M3'      , 'metcap'  , 0.14 , Length|Asymmetric, 'capm.3')
    #, ('minEnclosure'      , 'metbot'  , 'cut1'    , 0.055, Length|Asymmetric, 'via.4a')
    #, ('minEnclosure'      , 'metbot'  , 'cut2'    , 0.04 , Length|Asymmetric, 'via34.14')
    #, ('minEnclosure'      , 'metcap'  , 'cut2'    , 0.14 , Length|Asymmetric, 'capm.4')
    #, ('minArea'           , 'metcap'              , 0    , Area             , 'na')
    #, ('minAreaInMetcap'   , 'cut2'                , 0    , Area             , 'na')
    #, ('MIMCap'                                    , 1.25 , Unit             , 'na')
    #, ('MIMPerimeterCap'                           , 0.17 , Unit             , 'na')

      
    #capm
    , ('minWidth'          , 'capm'                , 1.14  , Length           , 'MIM.a')
    #, ('minWidth'          , 'capmdum'             , 0.5  , Length           , '')
    #, ('maxWidth'          , 'capm'                , 30.0 , Length           , '')
    #, ('maxWidth'          , 'metbot'              , 35.0 , Length           , '')
    , ('minSpacing'        , 'capm'                , 0.60 , Length           , 'MIM.b')
   #, ('minSpacing'        , 'M4'                  , 0.8  , Length           , 'capm.2b')
    , ('minSpacingWide1'   , 'M3'                  , 0.8  , Length           , 'capm.2b')
    , ('minSpacing'        , 'M6'     , 'capm'    , 0.60 , Length           , 'MIM.e')
    , ('minSpacing'        , 'via34'    , 'capm'    , 0.50 , Length           , 'capm.5')
    , ('minSpacingOnMetBot', 'via34'                , 0.2  , Length           , 'via34.2')
    , ('minSpacingOnMetCap', 'via34'                , 0.2  , Length           , 'via34.2')
    , ('minSpacingOnMetBot', 'via23'                 , 0.2  , Length           , 'via34.2 fake')
    , ('minSpacingOnMetCap', 'via23'                 , 0.2  , Length           , 'via34.2 fake')
    , ('minEnclosure'      , 'M5'      , 'capm'    , 0.60 , Length|Asymmetric, 'MIM.c')
    #, ('minEnclosure'      , 'M4'      , 'via23'     , 0.055, Length|Asymmetric, 'via.4a')
   #, ('minEnclosure'      , 'M4'      , 'via34'    , 0.04 , Length|Asymmetric, 'via34.14')
    #, ('minEnclosure'      , 'capm'    , 'via23'     , 0.14 , Length|Asymmetric, 'capm.4 fake')
    , ('minEnclosure'      , 'capm'    , 'via56'    , 0.36 , Length|Asymmetric, 'MIM.d')
    , ('minArea'           , 'capm'                , 1.30    , Area             , 'MIM.f')
    , ('minAreaInMetcap'   , 'via34'                , 0    , Area             , 'na')
    , ('MIMCap'                                    , 1.25 , Unit             , 'na')
    , ('MIMPerimeterCap'                           , 0.17 , Unit             , 'na')
    , ('PIPCap'                                    , 1.25 , Unit             , 'na')
    , ('PIPPerimeterCap'                           , 0.17 , Unit             , 'na')

    )


def _loadDtr ():
    """
    Load design kit physical rules for IHP 130nm.
    """
    loadAnalogTechno( analogTechnologyTable, __file__ )


def _loadDevices ():
    addDevice( name       = 'DifferentialPairBulkConnected'
            #, spice      = spiceDir+'DiffPairBulkConnected.spi'
             , connectors = ( 'D1', 'D2', 'G1', 'G2', 'S' )
             , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.DP_horizontalM2.py'    )   
                            , ('Symmetrical'    , 'coriolis.oroshi.DP_symmetrical.py'     )   
                            , ('Common centroid', 'coriolis.oroshi.DP_2DCommonCentroid.py')
                            , ('Interdigitated' , 'coriolis.oroshi.DP_interdigitated.py'  )
                            , ('WIP DP'         , 'coriolis.oroshi.wip_dp.py'             )
                            )
             )
    addDevice( name       = 'DifferentialPairBulkUnconnected'
            #, spice      = spiceDir+'DiffPairBulkUnconnected.spi'
             , connectors = ( 'D1', 'D2', 'G1', 'G2', 'S', 'B' )
             , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.DP_horizontalM2.py'    )   
                            , ('Symmetrical'    , 'coriolis.oroshi.DP_symmetrical.py'     )   
                            , ('Common centroid', 'coriolis.oroshi.DP_2DCommonCentroid.py')
                            , ('Interdigitated' , 'coriolis.oroshi.DP_interdigitated.py'  )
                            , ('WIP DP'         , 'coriolis.oroshi.wip_dp.py'             )
                            )
             )
    addDevice( name       = 'LevelShifterBulkUnconnected'
            #, spice      = spiceDir+'LevelShifterBulkUnconnected.spi'
             , connectors = ( 'D1', 'D2', 'S1', 'S2', 'B' )
             , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.LS_horizontalM2.py'    )   
                            , ('Symmetrical'    , 'coriolis.oroshi.LS_symmetrical.py'     )   
                            , ('Common centroid', 'coriolis.oroshi.LS_2DCommonCentroid.py')
                            , ('Interdigitated' , 'coriolis.oroshi.LS_interdigitated.py'  )
                            )
             )
    addDevice( name       = 'TransistorBulkConnected'
            #, spice      = spiceDir+'TransistorBulkConnected.spi'
             , connectors = ( 'D', 'G', 'S' )
             , layouts    = ( ('Rotate transistor', 'coriolis.oroshi.Transistor_rotate.py')
                            , ('Common transistor', 'coriolis.oroshi.Transistor_common.py')
                            , ('WIP Transistor'   , 'coriolis.oroshi.wip_transistor.py'   )   
                            )
             )
    addDevice( name       = 'TransistorBulkUnconnected'
            #, spice      = spiceDir+'TransistorBulkUnconnected.spi'
             , connectors = ( 'D', 'G', 'S', 'B' )
             , layouts    = ( ('Rotate transistor', 'coriolis.oroshi.Transistor_rotate.py')
                            , ('Common transistor', 'coriolis.oroshi.Transistor_common.py')
                            , ('WIP Transistor'   , 'coriolis.oroshi.wip_transistor.py'   )
                            )
             )
    addDevice( name       = 'CrossCoupledPairBulkConnected'
            #, spice      = spiceDir+'CCPairBulkConnected.spi'
             , connectors = ( 'D1', 'D2', 'S' )
             , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.CCP_horizontalM2.py'    )
                            , ('Symmetrical'    , 'coriolis.oroshi.CCP_symmetrical.py'     )
                            , ('Common centroid', 'coriolis.oroshi.CCP_2DCommonCentroid.py')
                            , ('Interdigitated' , 'coriolis.oroshi.CCP_interdigitated.py'  )
                            )
             )
    addDevice( name       = 'CrossCoupledPairBulkUnconnected'
            #, spice      = spiceDir+'CCPairBulkUnconnected.spi'
             , connectors = ( 'D1', 'D2', 'S', 'B' )
             , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.CCP_horizontalM2.py'    )
                            , ('Symmetrical'    , 'coriolis.oroshi.CCP_symmetrical.py'     )
                            , ('Common centroid', 'coriolis.oroshi.CCP_2DCommonCentroid.py')
                            , ('Interdigitated' , 'coriolis.oroshi.CCP_interdigitated.py'  )
                            )
             )
    addDevice( name       = 'CommonSourcePairBulkConnected'
            #, spice      = spiceDir+'CommonSourcePairBulkConnected.spi'
             , connectors = ( 'D1', 'D2', 'S', 'G' )
             , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.CSP_horizontalM2.py'    )
                            , ('Symmetrical'    , 'coriolis.oroshi.CSP_symmetrical.py'     )
                            , ('Interdigitated' , 'coriolis.oroshi.CSP_interdigitated.py'  )
                            , ('WIP CSP'        , 'coriolis.oroshi.wip_csp.py'             )
                            )
             )
    addDevice( name       = 'CommonSourcePairBulkUnconnected'
            #, spice      = spiceDir+'CommonSourcePairBulkUnconnected.spi'
             , connectors = ( 'D1', 'D2', 'S', 'G', 'B' )
             , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.CSP_horizontalM2.py'    )
                            , ('Symmetrical'    , 'coriolis.oroshi.CSP_symmetrical.py'     )
                            , ('Interdigitated' , 'coriolis.oroshi.CSP_interdigitated.py'  )
                            , ('WIP CSP'        , 'coriolis.oroshi.wip_csp.py'             )
                            )
             )
    addDevice( name       = 'SimpleCurrentMirrorBulkConnected'
            #, spice      = spiceDir+'CurrMirBulkConnected.spi'
             , connectors = ( 'D1', 'D2', 'S' )
             , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.SCM_horizontalM2.py'    )
                            , ('Symmetrical'    , 'coriolis.oroshi.SCM_symmetrical.py'     )
                            , ('Common centroid', 'coriolis.oroshi.SCM_2DCommonCentroid.py')
                            , ('Interdigitated' , 'coriolis.oroshi.SCM_interdigitated.py'  )
                            )
             )
    addDevice( name       = 'SimpleCurrentMirrorBulkUnconnected'
            #, spice      = spiceDir+'CurrMirBulkUnconnected.spi'
             , connectors = ( 'D1', 'D2', 'S', 'B' )
             , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.SCM_horizontalM2.py'    )
                            , ('Symmetrical'    , 'coriolis.oroshi.SCM_symmetrical.py'     )
                            , ('Common centroid', 'coriolis.oroshi.SCM_2DCommonCentroid.py')
                            , ('Interdigitated' , 'coriolis.oroshi.SCM_interdigitated.py'  )
                            )
             )
    addDevice( name       = 'MultiCapacitor'
            #, spice      = spiceDir+'MIM_OneCapacitor.spi'
             , connectors = ( 'T1', 'B1' )
             , layouts    = ( ('Matrix', 'coriolis.oroshi.multicapacitor.py' ),
                            )
             )
    #addDevice( name       = 'Resistor'
    #        #, spice      = spiceDir+'MIM_OneCapacitor.spi'
    #         , connectors = ( 'PIN1', 'PIN2' )
    #         , layouts    = ( ('Snake', 'coriolis.oroshi.resistorsnake.py' ),
    #                        )
    #         )


def _setup_techno ( coriolisTechDir ):
    ShellEnv.RDS_TECHNO_NAME   = (coriolisTechDir / 'sg13g2_lsx' / 'sg13g2.rds').as_posix()
    ShellEnv.GRAAL_TECHNO_NAME = (coriolisTechDir / 'sg13g2_lsx' / 'symbolic.graal' ).as_posix()
    ShellEnv.DREAL_TECHNO_NAME = (coriolisTechDir / 'sg13g2_lsx' / 'symbolic.dreal' ).as_posix()

    db = DataBase.getDB()
    CRL.System.get()

    tech = Technology.create(db, 'sg13g2_lsx')

    DbU.setPrecision( 2 )
    DbU.setPhysicalsPerGrid( 0.001, DbU.UnitPowerMicro )
    with CfgCache(priority=Cfg.Parameter.Priority.ConfigurationFile) as cfg:
        cfg.gdsDriver.metricDbu = 1e-09
        cfg.gdsDriver.dbuPerUu  = 0.001
    DbU.setGridsPerLambda      ( 30 )
    DbU.setSymbolicSnapGridStep( DbU.fromGrid( 1.0 ))
    DbU.setPolygonStep         ( DbU.fromGrid( 1.0 ))
    DbU.setStringMode          ( DbU.StringModePhysical, DbU.UnitPowerMicro )

    createBL( tech, 'nwm'          , BasicLayer.Material.nWell   , size=u(0.62), spacing=u(0.62),              gds2Layer= 31, gds2DataType= 0 )
    createBL( tech, 'nsdm'         , BasicLayer.Material.nImplant, size=u(0.31), spacing=u(0.31), area=0.25,  gds2Layer= 7, gds2DataType= 0 )
    createBL( tech, 'psdm'         , BasicLayer.Material.pImplant, size=u(0.31), spacing=u(0.31), area=0.25,  gds2Layer= 14, gds2DataType= 0 )
    #createBL( tech, 'hvi'          , BasicLayer.Material.other   ,                                             gds2Layer= 75, gds2DataType= 20 )
    createBL( tech, 'activ.pin'  , BasicLayer.Material.other   ,                                             gds2Layer= 1, gds2DataType= 2 )
    #createBL( tech, 'activ.block', BasicLayer.Material.blockage,                                             gds2Layer=1, gds2DataType= 23 )
    createBL( tech, 'poly.pin'     , BasicLayer.Material.other   ,                                             gds2Layer= 5, gds2DataType= 2 )
    #createBL( tech, 'poly.block'   , BasicLayer.Material.blockage,                                             gds2Layer=5, gds2DataType= 23 )
    createBL( tech, 'M1.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 8, gds2DataType= 2 )
    #createBL( tech, 'M1.block'     , BasicLayer.Material.blockage,                                             gds2Layer=8, gds2DataType= 23 )
    createBL( tech, 'M2.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 10, gds2DataType= 2 )
    #createBL( tech, 'M2.block'     , BasicLayer.Material.blockage,                                             gds2Layer=10, gds2DataType= 23 )
    createBL( tech, 'M3.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 30, gds2DataType= 2 )
    #createBL( tech, 'M3.block'     , BasicLayer.Material.blockage,                                             gds2Layer=30, gds2DataType= 23 )
    createBL( tech, 'M4.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 50, gds2DataType= 2 )
    #createBL( tech, 'M4.block'     , BasicLayer.Material.blockage,                                             gds2Layer=50, gds2DataType=23 )
    createBL( tech, 'M5.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 67, gds2DataType= 2 )
    #createBL( tech, 'M5.block'     , BasicLayer.Material.blockage,                                             gds2Layer=67, gds2DataType=23 )
    createBL( tech, 'M6.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 126, gds2DataType= 2 )
    #createBL( tech, 'M6.block'     , BasicLayer.Material.blockage,                                             gds2Layer=126, gds2DataType=23 )
    #createBL( tech, 'cont.block'  , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 30 )
    #createBL( tech, 'via12.block'   , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 50 )
    #createBL( tech, 'via23.block'    , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 70 )
    #createBL( tech, 'via34.block'   , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 90 )
    #createBL( tech, 'via45.block'   , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType=110 )
    #createBL( tech, 'via56.block'   , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType=130 )
    createBL( tech, 'activ'        , BasicLayer.Material.active  , size=u(0.15), spacing=u(0.21),              gds2Layer= 1, gds2DataType= 0 )
    createBL( tech, 'poly'         , BasicLayer.Material.poly    , size=u(0.13), spacing=u(0.18),              gds2Layer= 5, gds2DataType= 0 )
    createBL( tech, 'cont'         , BasicLayer.Material.cut     , size=u(0.16), spacing=u(0.18),              gds2Layer= 6, gds2DataType= 0 )
    createBL( tech, 'M1'           , BasicLayer.Material.metal   , size=u(0.16), spacing=u(0.18), area=0.009,   gds2Layer= 8, gds2DataType= 0 )
    createBL( tech, 'via12'         , BasicLayer.Material.cut     , size=u(0.19), spacing=u(0.22),              gds2Layer= 19, gds2DataType= 0 )
    createBL( tech, 'M2'           , BasicLayer.Material.metal   , size=u(0.20), spacing=u(0.21), area=0.144,  gds2Layer= 10, gds2DataType= 0 )
    createBL( tech, 'via23'          , BasicLayer.Material.cut     , size=u(0.19), spacing=u(0.22),              gds2Layer= 29, gds2DataType= 0 )
    createBL( tech, 'M3'           , BasicLayer.Material.metal   , size=u(0.19), spacing=u(0.21), area=0.144, gds2Layer= 30, gds2DataType= 0 )
    createBL( tech, 'capm'         , BasicLayer.Material.metal )
    createBL( tech, 'via34'         , BasicLayer.Material.cut     , size=u(0.19 ), spacing=u(0.22),              gds2Layer= 49, gds2DataType= 0 )
    createBL( tech, 'M4'           , BasicLayer.Material.metal   , size=u(0.19 ), spacing=u(0.21 ), area=0.144,   gds2Layer= 50, gds2DataType= 0 )
    createBL( tech, 'via45'         , BasicLayer.Material.cut     , size=u(0.19 ), spacing=u(0.22),              gds2Layer= 49, gds2DataType= 0 )
    createBL( tech, 'M5'           , BasicLayer.Material.metal   , size=u(0.19 ), spacing=u(0.21 ), area=0.144,   gds2Layer=67, gds2DataType= 0 )
    createBL( tech, 'via56'         , BasicLayer.Material.cut     , size=u(0.42 ), spacing=u(0.42 ),              gds2Layer= 125, gds2DataType= 0 )
    createBL( tech, 'M6'           , BasicLayer.Material.metal   , size=u(1.64), spacing=u(1.64),    gds2Layer= 126, gds2DataType= 0 )
    createBL( tech, 'hvtp'         , BasicLayer.Material.other   , size=u(0.38), spacing=u(0.38), area=0.265,  gds2Layer= 78, gds2DataType= 44 )
    createBL( tech, 'lvtn'         , BasicLayer.Material.other   , size=u(0.38), spacing=u(0.38), area=0.265,  gds2Layer=125, gds2DataType= 44 )
    createBL( tech, 'areaid_sc'    , BasicLayer.Material.other   ,                                             gds2Layer= 81, gds2DataType=  4 )
    createBL( tech, 'pad'          , BasicLayer.Material.cut     , size=u(40.0), spacing=u(1.27),              gds2Layer= 76, gds2DataType= 20 )
    createBL( tech, 'areaid_diode' , BasicLayer.Material.other   ,                                             gds2Layer= 81, gds2DataType= 23 )
    createBL( tech, 'pnp'          , BasicLayer.Material.other   ,                                             gds2Layer= 82, gds2DataType= 44 )
    createBL( tech, 'diffres'      , BasicLayer.Material.other   ,                                             gds2Layer= 65, gds2DataType= 13 )
    createBL( tech, 'npn'          , BasicLayer.Material.other   ,                                             gds2Layer= 82, gds2DataType= 20 )
    createBL( tech, 'polyres'      , BasicLayer.Material.other   ,                                             gds2Layer= 66, gds2DataType= 13 )
    createBL( tech, 'prBoundary'   , BasicLayer.Material.other   ,                                             gds2Layer=235, gds2DataType=  4 )

    tech.addLayerAlias( 'M2', 'met1' )
    tech.addLayerAlias( 'M3', 'met2' )
    tech.addLayerAlias( 'M4', 'met3' )
    tech.addLayerAlias( 'M5', 'met4' )
    tech.addLayerAlias( 'M6', 'met5' )

    # ViaLayers
    createVia( tech, 'li_via12_M2'  , 'M1'  , 'via12', 'M2', u(0.17) )
    createVia( tech, 'M2_via_M3'   , 'M2'  , 'via23' , 'M3', u(0.15) )
    createVia( tech, 'M3_via34_M4'  , 'M3'  , 'via34', 'M4', u(0.2 ) )
    createVia( tech, 'M4_via45_M5'  , 'M4'  , 'via45', 'M5', u(0.2 ) )
    createVia( tech, 'M5_via56_M6'  , 'M5'  , 'via56', 'M6', u(0.8 ) )
    createVia( tech, 'capm_via56', 'capm', 'via56', 'M6', u(0.2 ) )

    # Blockages
    #ech.getLayer('activ'  ).setBlockageLayer( tech.getLayer('activ.block') )
    #ech.getLayer('poly')   .setBlockageLayer( tech.getLayer('poly.block') )
    #ech.getLayer('M1')     .setBlockageLayer( tech.getLayer('M1.block') )
    #ech.getLayer('M2')     .setBlockageLayer( tech.getLayer('M2.block') )
    #ech.getLayer('M3')     .setBlockageLayer( tech.getLayer('M3.block') )
    #ech.getLayer('M4')     .setBlockageLayer( tech.getLayer('M4.block') )
    #ech.getLayer('M5')     .setBlockageLayer( tech.getLayer('M5.block') )
    #ech.getLayer('M6')     .setBlockageLayer( tech.getLayer('M6.block') )
    #ech.getLayer('cont' )  .setBlockageLayer( tech.getLayer('cont.block') )
    #ech.getLayer('via12')   .setBlockageLayer( tech.getLayer('via12.block') )
    #ech.getLayer('via23')    .setBlockageLayer( tech.getLayer('via23.block') )
    #ech.getLayer('via34')   .setBlockageLayer( tech.getLayer('via34.block') )
    #ech.getLayer('via45')   .setBlockageLayer( tech.getLayer('via45.block') )
    #ech.getLayer('via56')   .setBlockageLayer( tech.getLayer('via56.block') )

    # Coriolis internal layers
    createBL( tech, 'text.cell'    , BasicLayer.Material.other, )
    createBL( tech, 'text.instance', BasicLayer.Material.other, )
    createBL( tech, 'SPL1'         , BasicLayer.Material.other, )
    createBL( tech, 'AutoLayer'    , BasicLayer.Material.other, )
    createBL( tech, 'gmetalh'      , BasicLayer.Material.metal, )
    createBL( tech, 'gcontact'     , BasicLayer.Material.cut, )
    createBL( tech, 'gmetalv'      , BasicLayer.Material.metal, )

    # Resistors
    # ResistorLayer.create(tech, 'poly_res', 'poly', 'polyres')
    # ResistorLayer.create(tech, 'active_res', 'activ'  , 'diffres')

    # Transistors
    # GateLayer.create(tech, 'hvmosgate' , 'activ'  , 'poly', 'hvi')
    # GateLayer.create(tech, 'mosgate'   , 'activ'  , 'poly')
    # GateLayer.create(tech, 'mosgate_sc', 'activ'  , 'poly')
    # TransistorLayer.create(tech, 'nfet_01v8'     , 'mosgate'   ,  'nsdm')
    # TransistorLayer.create(tech, 'nfet_01v8_lvt' , 'mosgate'   , ('nsdm', 'lvtn'))
    # TransistorLayer.create(tech, 'nfet_01v8_sc'  , 'mosgate_sc',  'nsdm')
    # TransistorLayer.create(tech, 'nfet_g5v0d10v5', 'hvmosgate' ,  'nsdm')
    # TransistorLayer.create(tech, 'pfet_01v8'     , 'mosgate'   ,  'psdm', 'nwm')
    # TransistorLayer.create(tech, 'pfet_01v8_hvt' , 'mosgate'   , ('psdm', 'hvtp'), 'nwm')
    # TransistorLayer.create(tech, 'pfet_01v8_lvt' , 'mosgate'   , ('psdm', 'lvtn'), 'nwm')
    # TransistorLayer.create(tech, 'pfet_g5v0d10v5', 'hvmosgate' ,  'psdm', 'nwm')

    # Bipolars
    # Not implemented: Bipolar 'pnp_05v5_w0u68l0u68'
    # Not implemented: Bipolar 'npn_05v5_w1u00l2u00'
    # Not implemented: Bipolar 'pnp_05v5_w3u40l3u40'
    # Not implemented: Bipolar 'npn_05v5_w1u00l1u00'


def _setup_display ():
    # ----------------------------------------------------------------------
    # Style: Alliance.Classic [black]

    threshold = 0.2 if Viewer.Graphics.isHighDpi() else 0.1

    style = Viewer.DisplayStyle( 'Alliance.Classic [black]' )
    style.setDescription( 'Alliance Classic Look - black background' )
    style.setDarkening  ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )

    # Viewer.
    style.addDrawingStyle( group='Viewer', name='fallback'      , color=toRGB('Gray238'    ), border=1, pattern='55AA55AA55AA55AA' )
    style.addDrawingStyle( group='Viewer', name='background'    , color=toRGB('Gray50'     ), border=1 )
    style.addDrawingStyle( group='Viewer', name='foreground'    , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='rubber'        , color=toRGB('192,0,192'  ), border=4, threshold=0.02 )
    style.addDrawingStyle( group='Viewer', name='phantom'       , color=toRGB('Seashell4'  ), border=1 )
    style.addDrawingStyle( group='Viewer', name='boundaries'    , color=toRGB('wheat1'     ), border=2, pattern='0000000000000000', threshold=0 )
    style.addDrawingStyle( group='Viewer', name='marker'        , color=toRGB('80,250,80'  ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionDraw' , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionFill' , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='grid'          , color=toRGB('White'      ), border=1, threshold=2.0 )
    style.addDrawingStyle( group='Viewer', name='spot'          , color=toRGB('White'      ), border=2, threshold=6.0 )
    style.addDrawingStyle( group='Viewer', name='ghost'         , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='text.ruler'    , color=toRGB('White'      ), border=1, threshold=  0.0 )
    style.addDrawingStyle( group='Viewer', name='text.instance' , color=toRGB('White'      ), border=1, threshold=400.0 )
    style.addDrawingStyle( group='Viewer', name='text.reference', color=toRGB('White'      ), border=1, threshold=200.0 )
    style.addDrawingStyle( group='Viewer', name='undef'         , color=toRGB('Violet'     ), border=0, pattern='2244118822441188' )

    # Active Layers.
    style.addDrawingStyle(group='Active Layers', name='nwm'        , color=toRGB('Tan'      ), pattern=toHexa('urgo.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='nsdm'       , color=toRGB('LawnGreen'), pattern=toHexa('antihash0.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='psdm'       , color=toRGB('Yellow'   ), pattern=toHexa('antihash0.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='hvtp'       , color=toRGB('Yellow'   ), pattern=toHexa('antihash0.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='lvtn'       , color=toRGB('Yellow'   ), pattern=toHexa('antihash0.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='activ'      , color=toRGB('White'    ), pattern=toHexa('antihash0.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='activ.pin', color=toRGB('White'    ), pattern=toHexa('antihash0.8'), border=2, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='poly'       , color=toRGB('Red'      ), pattern=toHexa('antihash0.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='poly.pin'   , color=toRGB('Red'      ), pattern=toHexa('antihash0.8'), border=2, threshold=threshold)

    # Routing Layers.
    style.addDrawingStyle(group='Routing Layers', name='M1'    , color=toRGB('Blue'     ), pattern=toHexa('slash.8' ), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M1.pin', color=toRGB('Blue'     ), pattern=toHexa('slash.8' ), border=2, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M2'    , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M2.pin', color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'), border=2, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M3'    , color=toRGB('LightPink'), pattern=toHexa('poids4.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M3.pin', color=toRGB('LightPink'), pattern=toHexa('poids4.8'), border=2, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M4'    , color=toRGB('Green'    ), pattern=toHexa('poids4.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M4.pin', color=toRGB('Green'    ), pattern=toHexa('poids4.8'), border=2, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M5'    , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M5.pin', color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'), border=2, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M6'    , color=toRGB('Violet'   ), pattern=toHexa('poids4.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='M6.pin', color=toRGB('Violet'   ), pattern=toHexa('poids4.8'), border=2, threshold=threshold)

    # Cuts (VIA holes).
    style.addDrawingStyle(group='Cuts (VIA holes', name='cont' , color=toRGB('0,150,150'), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='via12' , color=toRGB('Aqua'     ), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='via23'  , color=toRGB('LightPink'), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='via34' , color=toRGB('Green'    ), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='via45' , color=toRGB('Yellow'   ), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='via56' , color=toRGB('Violet'   ), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='pad'  , color=toRGB('Red'      ), threshold=threshold)

    # Blockages.
    #style.addDrawingStyle(group='Blockages', name='activ.block', color=toRGB('Blue'     ), pattern=toHexa('slash.8' ), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='poly.block'   , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='M1.block'     , color=toRGB('LightPink'), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='M2.block'     , color=toRGB('Green'    ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='M3.block'     , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='M4.block'     , color=toRGB('Violet'   ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='M5.block'     , color=toRGB('Red'      ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='M6.block'     , color=toRGB('Blue'     ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='cont.block'  , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='via12.block'   , color=toRGB('LightPink'), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='via23.block'    , color=toRGB('Green'    ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='via34.block'   , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='via45.block'   , color=toRGB('Violet'   ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    #style.addDrawingStyle(group='Blockages', name='via56.block'   , color=toRGB('Red'      ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
#
    # Knick & Kite.
    style.addDrawingStyle( group='Knik & Kite', name='SPL1'           , color=toRGB('Red'        ) )
    style.addDrawingStyle( group='Knik & Kite', name='AutoLayer'      , color=toRGB('Magenta'    ) )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalh'        , color=toRGB('128,255,200'), pattern=toHexa('antislash2.32'    ), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalv'        , color=toRGB('200,200,255'), pattern=toHexa('light_antihash1.8'), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gcontact'       , color=toRGB('255,255,190'),                                      border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::Edge' , color=toRGB('255,255,190'), pattern='0000000000000000'         , border=4, threshold=0.02 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::GCell', color=toRGB('255,255,190'), pattern='0000000000000000'         , border=2, threshold=threshold )

    Viewer.Graphics.addStyle( style )

    # ----------------------------------------------------------------------
    # Style: Alliance.Classic [white].

    style = Viewer.DisplayStyle( 'Alliance.Classic [white]' )
    style.inheritFrom( 'Alliance.Classic [black]' )
    style.setDescription( 'Alliance Classic Look - white background' )
    style.setDarkening  ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )

    style.addDrawingStyle( group='Viewer', name='background', color=toRGB('White'), border=1 )
    style.addDrawingStyle( group='Viewer', name='foreground', color=toRGB('Black'), border=1 )
    style.addDrawingStyle( group='Viewer', name='boundaries', color=toRGB('Black'), border=1, pattern='0000000000000000' )
    Viewer.Graphics.addStyle( style )

    Viewer.Graphics.setStyle( 'Alliance.Classic [black]' )


def setup ( coriolisTechDir ):
    _setup_techno( coriolisTechDir )
    _setup_display()
    try:
        from .techno_symb import setup as setupSymbolic
    except:
        pass
    else:
        setupSymbolic()
    _loadDtr()
    _loadDevices()
        
