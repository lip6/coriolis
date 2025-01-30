
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
Coriolis Design Technological Rules (DTR) for SkyWater 130nm CMOS General Purpose
=================================================================================

:Version: rev.LIP6-1
:Date:    December 21, 2022
:Date:    February 2, 2023
:Date:    April 20, 2023
:Authors: Marie-Minerve Louerat

Reference documents: 
    https://skywater-pdk.readthedocs.io/en/main/rules/masks.html
    https://skywater-pdk.readthedosc.io/en/main/rules/periphery.html#x

Beware of the existence of li1 local interconnect using licon to connect to
difftap or to poly and mcon ton connect to metal1

Beware that some rules are context dependent (via spacing at end of line or at
one side, wide metal3)

Beware different description exist of MIM capacitors
here met2, capm, met3 and via2 connects met2/capm to met3

=====================  =======  ==========    ====================================
SkyWater130 mask       Acronym  Layer name    Coriolis original
purpose                         for rule      layer name
=====================  =======  ==========    ====================================
N-Well                 NWM      nwm           nwell
Low Vt Nch             LVTNM    lvtn        
active diffusion                difftap       active
Poly 1                 P1M      poly          poly
P+ Implant             PSDM     psdm          pImplant
N+ Implant             NSDM     nsdm          nImplant
Local Intr Cont. 1     LICM1    licon         cut0 contact between difftap and li1,
                                              poly and li1
Local Intrcnct 1       LI1M     li            metal metal between poly and metal1
                                              for local interconnect
Contact                CTM1     mcon          cut1 contact between li1 and metal1
Metal 1                MM1      m1            metal1
Via                    VIM      via           cut2
Metal 2                MM2      m2            metal2
Via 2-PLM              VIM2     via2          cut3
Metal 3-PLM            MM3      m3            metal3
Via 2-PLM              VIM3     via3          cut4
Metal 4                MM4      m4            metal4
Via 4                  VIM4     via4          cut5
Metal 5                MM5      m5            metal5

capm                   CAPM     capm          metcap
Metal 2                MM2      bottom_plate  metbot
=====================  =======  ============  ====================================

"""


analogTechnologyTable = \
    ( ('Header', 'Sky130', DbU.UnitPowerMicro, 'rev.LIP6-1')
    # ------------------------------------------------------------------------------------
    # ( Rule name          , [Layer1]  , [Layer2]  , Value , Rule flags       , Reference )
    , ('physicalGrid'                              , 0.005 , Length           , 'GSF')
    , ('transistorMinL'                            , 0.15  , Length           , 'poly.1 and device details')
    #, ('transistorMinL'                            , 0.38  , Length           , 'lvtn.1a')
    , ('transistorMaxL'                            , 38    , Length           , 'rule0002')
    , ('transistorMinW'                            , 0.42  , Length           , 'difftap.2')
    #, ('transistorMinW'                            , 0.36  , Length           , 'difftap.2b')
    , ('transistorMaxW'                            , 4000  , Length           , 'rule0004')

    # N-WELL (nwm)
    , ('minWidth'          , 'nwm'                 , 0.84  , Length           , 'nwell.1')
    , ('minSpacing'        , 'nwm'                 , 1.27  , Length           , 'nwell.2a')
    , ('minArea'           , 'nwm'                 , 0     , Area             , 'N/A')

    # LVTN (lvtn)
    , ('minWidth'          , 'lvtn'                , 0.38  , Length           , 'lvtn.1a')
    , ('minSpacing'        , 'lvtn'                , 0.38  , Length           , 'lvtn.2')
    , ('minArea'           , 'lvtn'                , 0.265 , Area             , 'lvtn.13')
    , ('minEnclosure'      , 'nwm'     , 'lvtn'    , 0.38  , Length|Asymmetric, 'lvtn.10')

    # DIFF (difftap)
    , ('minWidth'          , 'difftap'             , 0.15  , Length           , 'difftap.1')
    , ('minSpacing'        , 'difftap'             , 0.27  , Length           , 'difftap.3')
    , ('minArea'           , 'difftap'             , 0     , Area             , 'N/A')
    , ('minEnclosure'      , 'nwm'     , 'difftap' , 0.18  , Length|Asymmetric, 'difftap.10')

    # Poly1 (poly)
    , ('minWidth'          , 'poly'                , 0.15  , Length           , 'poly.1a')
    , ('minSpacing'        , 'poly'                , 0.21  , Length           , 'poly.2')
    , ('minGateSpacing'    , 'poly'                , 0.21  , Length           , 'poly.2')
    , ('minArea'           , 'poly'                , 0     , Area             , 'N/A')
    , ('minSpacing'        , 'poly'    , 'difftap' , 0.075 , Length           , 'poly.4')
    , ('minExtension'      , 'poly'    , 'difftap' , 0.130 , Length|Asymmetric, 'poly.8')
    , ('minGateExtension'  , 'difftap' , 'poly'    , 0.25  , Length|Asymmetric, 'poly.7')
    , ('minExtension'      , 'difftap' , 'poly'    , 0.25  , Length|Asymmetric, 'poly.7')

    # 4.1.6 PPLUS (psdm)
    , ('minWidth'          , 'psdm'                , 0.38  , Length           , 'psd.1')
    , ('minSpacing'        , 'psdm'                , 0.38  , Length           , 'psd.2')
    , ('minArea'           , 'psdm'                , 0.255 , Area             , 'psd.10b')
    , ('minSpacing'        , 'psdm'    , 'difftap' , 0.130 , Length           , 'psd.7')
    , ('minGateExtension'  , 'psdm'    , 'poly'    , 0.00  , Length|Asymmetric, 'N/A')
    , ('minOverlap'        , 'psdm'    , 'difftap' , 0.00  , Length           , 'N/A')
    , ('minEnclosure'      , 'psdm'    , 'difftap' , 0.125 , Length|Asymmetric, 'psd.5a')
    , ('minStrapEnclosure' , 'psdm'    , 'difftap' , 0.125 , Length           , 'psd.5b')
    , ('minSpacing'        , 'nsdm'    , 'psdm'    , 0.00  , Length           , 'N/A')
    , ('minEnclosure'      , 'psdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    , ('minLengthEnclosure', 'psdm'    , 'difftap' , 0     , Length|Asymmetric, 'N/A')
    , ('minWidthEnclosure' , 'psdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'psdm'    , 'difftap' , 0.125 , Length|Asymmetric, 'dup. psd.5a')
    , ('minStrapEnclosure' , 'psdm'                , 0.125 , Length           , 'dup. psd.5b')

    # NPLUS (nsdm)
    , ('minWidth'          , 'nsdm'                , 0.38  , Length           , 'nsd.1')
    , ('minSpacing'        , 'nsdm'                , 0.38  , Length           , 'nsd.2')
    , ('minArea'           , 'nsdm'                , 0.265 , Area             , 'nsd.10a')
    , ('minSpacing'        , 'nsdm'    , 'difftap' , 0.130 , Length           , 'nsd.7')
    , ('minGateExtension'  , 'nsdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    , ('minOverlap'        , 'nsdm'    , 'difftap' , 0     , Length           , 'N/A')
    , ('minEnclosure'      , 'nsdm'    , 'difftap' , 0.125 , Length|Asymmetric, 'nsd.5a')
    , ('minStrapEnclosure' , 'nsdm'    , 'difftap' , 0.125 , Length           , 'nsd.5b')
    , ('minEnclosure'      , 'nsdm'    , 'nwm'     , 0     , Length|Asymmetric, 'N/A')
    , ('minEnclosure'      , 'nsdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    , ('minLengthEnclosure', 'nsdm'    , 'difftap' , 0     , Length|Asymmetric, 'N/A')
    , ('minWidthEnclosure' , 'nsdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    , ('minGateEnclosure'  , 'nsdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'nsdm'    , 'difftap' , 0.125 , Length|Asymmetric, 'dup. nsd.5a')
    , ('minStrapEnclosure' , 'nsdm'                , 0.215 , Length           , 'dup. nsd.5b')

    # LICM1 (licon)
    , ('minWidth'          , 'licon'               , 0.17  , Length           , 'licon.1')
    , ('minSpacing'        , 'licon'               , 0.17  , Length           , 'licon.2')
    , ('minGateSpacing'    , 'licon'   , 'poly'    , 0.25  , Length|Asymmetric, 'licon.10')
    , ('minSpacing'        , 'licon'   , 'poly'    , 0.25  , Length|Asymmetric, 'licon.10')
    , ('minSpacing'        , 'licon'   , 'difftap' , 0.19  , Length           , 'licon.14')
    #, ('minSpacing'        , 'licon'   , 'difftap' , 0.06  , Length           , 'licon.5b')
    , ('minEnclosure'      , 'difftap' , 'licon'   , 0.04  , Length|Asymmetric, 'licon.5a and licon.7 : 0.12 isolated tap')
    , ('minEnclosure'      , 'poly'    , 'licon'   , 0.05  , Length|Asymmetric, 'licon.8 and licon.8a : 0.08')
    , ('minEnclosure'      , 'psdm'    , 'licon'   , 0     , Length|Asymmetric, 'N/A')
    , ('minEnclosure'      , 'nsdm'    , 'licon'   , 0     , Length|Asymmetric, 'N/A')
    , ('minGateEnclosure'  , 'psdm'    , 'poly'    , 0     , Length|Asymmetric, 'N/A')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'poly'    , 'licon'   , 0.05  , Length|Asymmetric, 'dup. licon.8 and licon.8a')
    , ('minExtension'      , 'psdm'    , 'licon'   , 0.25  , Length|Asymmetric, 'dup.')
    , ('minExtension'      , 'nsdm'    , 'licon'   , 0.25  , Length|Asymmetric, 'dup.')

    # LI1M (li)
    , ('minWidth'          , 'li'                  , 0.17  , Length           , 'li.1')
    , ('minSpacing'        , 'li'                  , 0.17  , Length           , 'li.3')
    , ('minArea'           , 'li'                  , 0.0561, Area             , 'li.6')
    , ('minEnclosure'      , 'li'      , 'licon'   , 0.08  , Length|Asymmetric, 'li.5')
    , ('minEnclosure'      , 'li'      , 'mcon'    , 0.00  , Length|Asymmetric, 'ct.4')

    # CTM1 (mcon)
    , ('minWidth'          , 'mcon'                , 0.17  , Length           , 'ct.1')
    , ('minSpacing'        , 'mcon'                , 0.19  , Length           , 'ct.2')


    # MM1 (m1)
    , ('minWidth'          , 'm1'                  , 0.14  , Length           , 'm1.1')
    , ('minSpacing'        , 'm1'                  , 0.14  , Length           , 'm1.2')
    , ('minArea'           , 'm1'                  , 0.083 , Area             , 'm1.6')
    , ('minEnclosure'      , 'm1'      , 'mcon'    , 0.03  , Length|Asymmetric, 'm1.4 and m1.5 : 0.06 one side')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'm1'      , 'mcon'    , 0.03  , Length|Asymmetric, 'm1.4 and m1.5')

    # VIM (via)
    , ('minWidth'          , 'via'                 , 0.15  , Length           , 'via.1a')
    , ('minSpacing'        , 'via'                 , 0.17  , Length           , 'via.2')
    , ('minEnclosure'      , 'm1'      , 'via'     , 0.55  , Length|Asymmetric, 'via.4a and via.5a : 0.085 on one side')
    # Error: duplicated rule, needed by "old Pharos".
    , ('lineExtension'     , 'm1'      , 'via'     , 0.55  , Length|Asymmetric, 'dup. via.a4 and via.5a : 0.085 on one side')

    # MM2 (m2)
    , ('minWidth'          , 'm2'                  , 0.14  , Length           , 'm2.1')
    , ('minSpacing'        , 'm2'                  , 0.14  , Length           , 'm2.2')
    , ('minArea'           , 'm2'                  , 0.0676, Area             , 'm2.6')
    , ('minEnclosure'      , 'm2'      , 'via'     , 0.055 , Length|Asymmetric, 'm2.4 and m2.5 : 0.085 end of line')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'm2'      , 'via'     , 0.055 , Length|Asymmetric, 'dup. m2.4 and m2.5 : 0.085 end of line')

    # VIM2 (via2)
    , ('minWidth'          , 'via2'                , 0.20  , Length           , 'via2.1a')
    , ('minSpacing'        , 'via2'                , 0.20  , Length           , 'via2.2')
    , ('minEnclosure'      , 'm2'      , 'via2'    , 0.04  , Length|Asymmetric, 'via2.4 via2.5 : 0.085 and via2.14')

    # MM3 (m3)
    , ('minWidth'          , 'm3'                  , 0.30  , Length           , 'm3.1')
    , ('minSpacing'        , 'm3'                  , 0.30  , Length           , 'm3.2')
    , ('minSpacing'        , 'widem3'              , 0.40  , Length           , 'm3.3d')
    , ('minArea'           , 'm3'                  , 0.24  , Area             , 'm3.6'        )
    , ('minEnclosure'      , 'm3'      , 'via2'    , 0.065 , Length|Asymmetric, 'm3.4')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'm3'      , 'via2'    , 0.065 , Length|Asymmetric, 'dup. m3.4')

    # VIM3 (via3)
    , ('minWidth'          , 'via3'                , 0.20  , Length           , 'via3.1')
    , ('minSpacing'        , 'via3'                , 0.20  , Length           , 'via3.2')
    , ('minEnclosure'      , 'm3'      , 'via3'    , 0.060 , Length|Asymmetric, 'via3.4 and via3.5 end of line : 0.090')

    # MM4 (m4)
    , ('minWidth'          , 'm4'                  , 0.30  , Length           , 'm4.1')
    , ('minSpacing'        , 'm4'                  , 0.30  , Length           , 'm4.2')
    , ('minArea'           , 'm4'                  , 0.24  , Area             , 'm4.4a')
    , ('minEnclosure'      , 'm4'      , 'via3'    , 0.065 , Length|Asymmetric, 'm4.3')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'm4'      , 'via3'    , 0.065 , Length|Asymmetric, 'dup. m4.3 ')

    # VIM4 (via4)
    , ('minWidth'          , 'via4'                , 0.80  , Length           , 'via4.1')
    , ('minSpacing'        , 'via4'                , 0.80  , Length           , 'via4.2')
    , ('minEnclosure'      , 'm4'      , 'via4'    , 0.19  , Length|Asymmetric, 'via4.4')

    # MM5 (m5)
    , ('minWidth'          , 'm5'                  , 1.6   , Length           , 'm5.1')
    , ('minSpacing'        , 'm5'                  , 1.6   , Length           , 'm5.2')
    , ('minArea'           , 'm5'                  , 4.00  , Area             , 'm5.4')
    , ('minEnclosure'      , 'm5'      , 'via4'    , 0.310 , Length|Asymmetric, 'm5.3')
    # Error: duplicated rule, needed by "old Pharos".
    , ('minExtension'      , 'm5'      , 'via4'    , 0.310 , Length|Asymmetric, 'dup. m5.3 ')


    #capm
    #, ('minWidth'          , 'metcap'              , 1.0  , Length           , 'capm.1')
    #, ('minWidth'          , 'metcapdum'           , 0.5  , Length           , '')
    #, ('maxWidth'          , 'metcap'              , 300.0 , Length           , '')
    #, ('maxWidth'          , 'metbot'              , 350.0 , Length           , '')
    #, ('minSpacing'        , 'metcap'              , 0.84 , Length           , 'capm.2a')
    #, ('minSpacing'        , 'metbot'              , 0.8  , Length           , 'metcap.2b')
    #, ('minSpacing'        , 'cut1'    , 'metcap'  , 0.50 , Length           , '')
    #, ('minSpacing'        , 'cut2'    , 'metcap'  , 0.50 , Length           , 'capm.5')
    #, ('minSpacingOnMetbot', 'cut2'                , 0.2  , Length           , 'via2.2')
    #, ('minSpacingOnMetbot', 'via2'                , 0.2  , Length           , 'via2.2')
    #, ('minSpacingOnMetcap', 'cut2'                , 0.2  , Length           , 'via2.2')
    #, ('minEnclosure'      , 'm2'      , 'metcap'  , 0.14 , Length|Asymmetric, 'capm.3')
    #, ('minEnclosure'      , 'metbot'  , 'cut1'    , 0.055, Length|Asymmetric, 'via.4a')
    #, ('minEnclosure'      , 'metbot'  , 'cut2'    , 0.04 , Length|Asymmetric, 'via2.14')
    #, ('minEnclosure'      , 'metcap'  , 'cut2'    , 0.14 , Length|Asymmetric, 'capm.4')
    #, ('minArea'           , 'metcap'              , 0    , Area             , 'na')
    #, ('minAreaInMetcap'   , 'cut2'                , 0    , Area             , 'na')
    #, ('MIMCap'                                    , 1.25 , Unit             , 'na')
    #, ('MIMPerimeterCap'                           , 0.17 , Unit             , 'na')

      
    #capm
    , ('minWidth'          , 'capm'                , 1.0  , Length           , 'capm.1')
    , ('minWidth'          , 'capmdum'             , 0.5  , Length           , '')
    , ('maxWidth'          , 'capm'                , 30.0 , Length           , '')
    , ('maxWidth'          , 'metbot'              , 35.0 , Length           , '')
    , ('minSpacing'        , 'capm'                , 0.84 , Length           , 'capm.2a')
   #, ('minSpacing'        , 'm3'                  , 0.8  , Length           , 'capm.2b')
    , ('minSpacingWide1'   , 'm2'                  , 0.8  , Length           , 'capm.2b')
    , ('minSpacing'        , 'via'     , 'capm'    , 0.50 , Length           , 'fake')
    , ('minSpacing'        , 'via2'    , 'capm'    , 0.50 , Length           , 'capm.5')
    , ('minSpacingOnMetBot', 'via2'                , 0.2  , Length           , 'via2.2')
    , ('minSpacingOnMetCap', 'via2'                , 0.2  , Length           , 'via2.2')
    , ('minSpacingOnMetBot', 'via'                 , 0.2  , Length           , 'via2.2 fake')
    , ('minSpacingOnMetCap', 'via'                 , 0.2  , Length           , 'via2.2 fake')
    , ('minEnclosure'      , 'm2'      , 'capm'    , 0.14 , Length|Asymmetric, 'capm.3')
    , ('minEnclosure'      , 'm3'      , 'via'     , 0.055, Length|Asymmetric, 'via.4a')
   #, ('minEnclosure'      , 'm3'      , 'via2'    , 0.04 , Length|Asymmetric, 'via2.14')
    , ('minEnclosure'      , 'capm'    , 'via'     , 0.14 , Length|Asymmetric, 'capm.4 fake')
    , ('minEnclosure'      , 'capm'    , 'via2'    , 0.14 , Length|Asymmetric, 'capm.4')
    , ('minArea'           , 'capm'                , 0    , Area             , 'na')
    , ('minAreaInMetcap'   , 'via2'                , 0    , Area             , 'na')
    , ('MIMCap'                                    , 1.25 , Unit             , 'na')
    , ('MIMPerimeterCap'                           , 0.17 , Unit             , 'na')
    , ('PIPCap'                                    , 1.25 , Unit             , 'na')
    , ('PIPPerimeterCap'                           , 0.17 , Unit             , 'na')

    )


def _loadDtr ():
    """
    Load design kit physical rules for SkyWater 130nm.
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
    ShellEnv.RDS_TECHNO_NAME   = (coriolisTechDir / 'sky130_vsc' / 'sky130_vsc.rds').as_posix()
    ShellEnv.GRAAL_TECHNO_NAME = (coriolisTechDir / 'sky130_vsc' / 'symbolic.graal' ).as_posix()
    ShellEnv.DREAL_TECHNO_NAME = (coriolisTechDir / 'sky130_vsc' / 'symbolic.dreal' ).as_posix()

    db = DataBase.getDB()
    CRL.System.get()

    tech = Technology.create(db, 'Sky130_vsc')

    DbU.setPrecision( 2 )
    DbU.setPhysicalsPerGrid( 0.0025, DbU.UnitPowerMicro )
    with CfgCache(priority=Cfg.Parameter.Priority.ConfigurationFile) as cfg:
        cfg.gdsDriver.metricDbu = 1e-09
        cfg.gdsDriver.dbuPerUu  = 0.001
    DbU.setGridsPerLambda      ( 30 )
    DbU.setSymbolicSnapGridStep( DbU.fromGrid( 1.0 ))
    DbU.setPolygonStep         ( DbU.fromGrid( 1.0 ))
    DbU.setStringMode          ( DbU.StringModePhysical, DbU.UnitPowerMicro )

    createBL( tech, 'nwm'          , BasicLayer.Material.nWell   , size=u(0.84), spacing=u(1.27),              gds2Layer= 64, gds2DataType= 20 )
    createBL( tech, 'nsdm'         , BasicLayer.Material.nImplant, size=u(0.38), spacing=u(0.38), area=0.265,  gds2Layer= 93, gds2DataType= 44 )
    createBL( tech, 'psdm'         , BasicLayer.Material.pImplant, size=u(0.38), spacing=u(0.38), area=0.265,  gds2Layer= 94, gds2DataType= 20 )
    createBL( tech, 'hvi'          , BasicLayer.Material.other   ,                                             gds2Layer= 75, gds2DataType= 20 )
    createBL( tech, 'difftap.pin'  , BasicLayer.Material.other   ,                                             gds2Layer= 65, gds2DataType= 16 )
    createBL( tech, 'difftap.block', BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 10 )
    createBL( tech, 'poly.pin'     , BasicLayer.Material.other   ,                                             gds2Layer= 66, gds2DataType= 16 )
    createBL( tech, 'poly.block'   , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 20 )
    createBL( tech, 'li.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 67, gds2DataType= 16 )
    createBL( tech, 'li.block'     , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 40 )
    createBL( tech, 'm1.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 68, gds2DataType= 16 )
    createBL( tech, 'm1.block'     , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 60 )
    createBL( tech, 'm2.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 69, gds2DataType= 16 )
    createBL( tech, 'm2.block'     , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 80 )
    createBL( tech, 'm3.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 70, gds2DataType= 16 )
    createBL( tech, 'm3.block'     , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType=100 )
    createBL( tech, 'm4.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 71, gds2DataType= 16 )
    createBL( tech, 'm4.block'     , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType=120 )
    createBL( tech, 'm5.pin'       , BasicLayer.Material.other   ,                                             gds2Layer= 72, gds2DataType= 16 )
    createBL( tech, 'm5.block'     , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType=140 )
    createBL( tech, 'licon.block'  , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 30 )
    createBL( tech, 'mcon.block'   , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 50 )
    createBL( tech, 'via.block'    , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 70 )
    createBL( tech, 'via2.block'   , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType= 90 )
    createBL( tech, 'via3.block'   , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType=110 )
    createBL( tech, 'via4.block'   , BasicLayer.Material.blockage,                                             gds2Layer=100, gds2DataType=130 )
    createBL( tech, 'difftap'      , BasicLayer.Material.active  , size=u(0.15), spacing=u(0.27),              gds2Layer= 65, gds2DataType= 20 )
    createBL( tech, 'poly'         , BasicLayer.Material.poly    , size=u(0.15), spacing=u(0.21),              gds2Layer= 66, gds2DataType= 20 )
    createBL( tech, 'licon'        , BasicLayer.Material.cut     , size=u(0.17), spacing=u(0.17),              gds2Layer= 66, gds2DataType= 44 )
    createBL( tech, 'li'           , BasicLayer.Material.metal   , size=u(0.17), spacing=u(0.17),              gds2Layer= 67, gds2DataType= 20 )
    createBL( tech, 'mcon'         , BasicLayer.Material.cut     , size=u(0.17), spacing=u(0.19),              gds2Layer= 67, gds2DataType= 44 )
    createBL( tech, 'm1'           , BasicLayer.Material.metal   , size=u(0.14), spacing=u(0.14), area=0.083,  gds2Layer= 68, gds2DataType= 20 )
    createBL( tech, 'via'          , BasicLayer.Material.cut     , size=u(0.15), spacing=u(0.17),              gds2Layer= 68, gds2DataType= 44 )
    createBL( tech, 'm2'           , BasicLayer.Material.metal   , size=u(0.14), spacing=u(0.14), area=0.0676, gds2Layer= 69, gds2DataType= 20 )
    createBL( tech, 'capm'         , BasicLayer.Material.metal )
    createBL( tech, 'via2'         , BasicLayer.Material.cut     , size=u(0.2 ), spacing=u(0.2 ),              gds2Layer= 69, gds2DataType= 44 )
    createBL( tech, 'm3'           , BasicLayer.Material.metal   , size=u(0.3 ), spacing=u(0.3 ), area=0.24,   gds2Layer= 70, gds2DataType= 20 )
    createBL( tech, 'via3'         , BasicLayer.Material.cut     , size=u(0.2 ), spacing=u(0.2 ),              gds2Layer= 70, gds2DataType= 44 )
    createBL( tech, 'm4'           , BasicLayer.Material.metal   , size=u(0.3 ), spacing=u(0.3 ), area=0.24,   gds2Layer= 71, gds2DataType= 20 )
    createBL( tech, 'via4'         , BasicLayer.Material.cut     , size=u(0.8 ), spacing=u(0.8 ),              gds2Layer= 71, gds2DataType= 44 )
    createBL( tech, 'm5'           , BasicLayer.Material.metal   , size=u(1.6 ), spacing=u(1.6 ), area=4.0,    gds2Layer= 72, gds2DataType= 20 )
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

    tech.addLayerAlias( 'm1', 'met1' )
    tech.addLayerAlias( 'm2', 'met2' )
    tech.addLayerAlias( 'm3', 'met3' )
    tech.addLayerAlias( 'm4', 'met4' )
    tech.addLayerAlias( 'm5', 'met5' )

    # ViaLayers
    createVia( tech, 'li_mcon_m1'  , 'li'  , 'mcon', 'm1', u(0.17) )
    createVia( tech, 'm1_via_m2'   , 'm1'  , 'via' , 'm2', u(0.15) )
    createVia( tech, 'm2_via2_m3'  , 'm2'  , 'via2', 'm3', u(0.2 ) )
    createVia( tech, 'capm_via2_m3', 'capm', 'via2', 'm3', u(0.2 ) )
    createVia( tech, 'm3_via3_m4'  , 'm3'  , 'via3', 'm4', u(0.2 ) )
    createVia( tech, 'm4_via4_m5'  , 'm4'  , 'via4', 'm5', u(0.8 ) )

    # Blockages
    tech.getLayer('difftap').setBlockageLayer( tech.getLayer('difftap.block') )
    tech.getLayer('poly')   .setBlockageLayer( tech.getLayer('poly.block') )
    tech.getLayer('li')     .setBlockageLayer( tech.getLayer('li.block') )
    tech.getLayer('m1')     .setBlockageLayer( tech.getLayer('m1.block') )
    tech.getLayer('m2')     .setBlockageLayer( tech.getLayer('m2.block') )
    tech.getLayer('m3')     .setBlockageLayer( tech.getLayer('m3.block') )
    tech.getLayer('m4')     .setBlockageLayer( tech.getLayer('m4.block') )
    tech.getLayer('m5')     .setBlockageLayer( tech.getLayer('m5.block') )
    tech.getLayer('licon')  .setBlockageLayer( tech.getLayer('licon.block') )
    tech.getLayer('mcon')   .setBlockageLayer( tech.getLayer('mcon.block') )
    tech.getLayer('via')    .setBlockageLayer( tech.getLayer('via.block') )
    tech.getLayer('via2')   .setBlockageLayer( tech.getLayer('via2.block') )
    tech.getLayer('via3')   .setBlockageLayer( tech.getLayer('via3.block') )
    tech.getLayer('via4')   .setBlockageLayer( tech.getLayer('via4.block') )

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
    # ResistorLayer.create(tech, 'active_res', 'difftap', 'diffres')

    # Transistors
    # GateLayer.create(tech, 'hvmosgate' , 'difftap', 'poly', 'hvi')
    # GateLayer.create(tech, 'mosgate'   , 'difftap', 'poly')
    # GateLayer.create(tech, 'mosgate_sc', 'difftap', 'poly')
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
    style.addDrawingStyle(group='Active Layers', name='difftap'    , color=toRGB('White'    ), pattern=toHexa('antihash0.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='difftap.pin', color=toRGB('White'    ), pattern=toHexa('antihash0.8'), border=2, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='poly'       , color=toRGB('Red'      ), pattern=toHexa('antihash0.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Active Layers', name='poly.pin'   , color=toRGB('Red'      ), pattern=toHexa('antihash0.8'), border=2, threshold=threshold)

    # Routing Layers.
    style.addDrawingStyle(group='Routing Layers', name='li'    , color=toRGB('Blue'     ), pattern=toHexa('slash.8' ), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='li.pin', color=toRGB('Blue'     ), pattern=toHexa('slash.8' ), border=2, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='m1'    , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='m1.pin', color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'), border=2, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='m2'    , color=toRGB('LightPink'), pattern=toHexa('poids4.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='m2.pin', color=toRGB('LightPink'), pattern=toHexa('poids4.8'), border=2, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='m3'    , color=toRGB('Green'    ), pattern=toHexa('poids4.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='m3.pin', color=toRGB('Green'    ), pattern=toHexa('poids4.8'), border=2, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='m4'    , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='m4.pin', color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'), border=2, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='m5'    , color=toRGB('Violet'   ), pattern=toHexa('poids4.8'), border=1, threshold=threshold)
    style.addDrawingStyle(group='Routing Layers', name='m5.pin', color=toRGB('Violet'   ), pattern=toHexa('poids4.8'), border=2, threshold=threshold)

    # Cuts (VIA holes).
    style.addDrawingStyle(group='Cuts (VIA holes', name='licon', color=toRGB('0,150,150'), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='mcon' , color=toRGB('Aqua'     ), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='via'  , color=toRGB('LightPink'), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='via2' , color=toRGB('Green'    ), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='via3' , color=toRGB('Yellow'   ), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='via4' , color=toRGB('Violet'   ), threshold=threshold)
    style.addDrawingStyle(group='Cuts (VIA holes', name='pad'  , color=toRGB('Red'      ), threshold=threshold)

    # Blockages.
    style.addDrawingStyle(group='Blockages', name='difftap.block', color=toRGB('Blue'     ), pattern=toHexa('slash.8' ), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='poly.block'   , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='li.block'     , color=toRGB('LightPink'), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='m1.block'     , color=toRGB('Green'    ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='m2.block'     , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='m3.block'     , color=toRGB('Violet'   ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='m4.block'     , color=toRGB('Red'      ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='m5.block'     , color=toRGB('Blue'     ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='licon.block'  , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='mcon.block'   , color=toRGB('LightPink'), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='via.block'    , color=toRGB('Green'    ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='via2.block'   , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='via3.block'   , color=toRGB('Violet'   ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)
    style.addDrawingStyle(group='Blockages', name='via4.block'   , color=toRGB('Red'      ), pattern=toHexa('poids4.8'), border=4, threshold=threshold)

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
        
