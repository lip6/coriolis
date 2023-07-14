
# Name:    dtr_scn6m_deep_09.py -- mosis scalable CMOS SCN6M_DEEP General Purpose with MIM capacitor
# Unit:    micro
# version: rev.LIP6-1
#
# The MOSIS Scalable (SCMOS) design rules can be found here:
#   https://www.mosis.com/files/scmos/scmos.pdf
#
# Used revision 8.00 of May 11, 2009.

from   coriolis.helpers    import truncPath
from   coriolis.helpers.io import vprint
vprint( 2, '     - "{}".'.format(truncPath(__file__)) )

from coriolis.Hurricane            import DbU
from coriolis.helpers.analogtechno import Length, Area, Asymmetric, loadAnalogTechno


analogTechnologyTable = \
    ( ('Header', 'scmos6m_deep_09', DbU.UnitPowerMicro, 'rev.LIP6-1')
    # ------------------------------------------------------------------------------------
    # ( Rule name          , [Layer1]  , [Layer2]  , Value, Rule flags       , Reference )
    , ('physicalGrid'                              , 0.09 , Length           , 'N/A')
    , ('transistorMinL'                            , 0.18 , Length           , '2l    rule 3.1    p.26')
    , ('transistorMaxL'                            , 18   , Length           , '200l  arbitrary')
    , ('transistorMinW'                            , 0.27 , Length           , '3l    arbitrary')
    , ('transistorMaxW'                            , 270  , Length           , '300l  arbitrary')

    , ('minWidth'          , 'nWell'               , 1.08 , Length           , '12l   rule 1.1    p.16')
    , ('minSpacing'        , 'nWell'               , 1.62 , Length           , '18l   rule 1.2    p.16')
    , ('minArea'           , 'nWell'               , 0    , Area             , 'N/A')
    , ('minWidth'          , 'active'              , 0.27 , Length           , '3l    rule 2.1    p.19')
    , ('minSpacing'        , 'active'              , 0.27 , Length           , '3l    rule 2.2    p.19')
    , ('minArea'           , 'active'              , 0    , Area             , 'N/A')
    , ('minEnclosure'      , 'nWell'   , 'active'  , 0.54 , Length|Asymmetric, 'rule X.Y, p.Z')
    , ('minEnclosure'      , 'nImplant', 'nWell'   , 0    , Length|Asymmetric, 'rule X.Y, p.Z')
    , ('minWidth'          , 'poly'                , 0.18 , Length           , '2l    rule 3.1    p.26')
    , ('minSpacing'        , 'poly'                , 0.27 , Length           , '3l    rule 3.2    p.26')
    , ('minGateSpacing'    , 'poly'                , 0.36 , Length           , '4l    rule 3.2.a  p.26')
    , ('minArea'           , 'poly'                , 0    , Area             , 'N/A')
    , ('minGateExtension'  , 'poly'    , 'active'  , 0.225, Length|Asymmetric, '2.5l  rule 3.3    p.26')
    , ('minGateExtension'  , 'active'  , 'poly'    , 0.225, Length|Asymmetric, 'N/A')
    , ('minExtension'      , 'poly'    , 'active'  , 0.225, Length|Asymmetric, 'N/A')
    , ('minExtension'      , 'active'  , 'poly'    , 0.36 , Length|Asymmetric, '4l    rule 3.4    p.26')
    , ('minSpacing'        , 'poly'    , 'active'  , 0.09 , Length           , '1l    rule 3.5    p.26')

    , ('minGateSpacing'    , 'nImplant', 'poly'    , 0.27 , Length           , '3l    rule 4.1 N+ p.29')
    , ('minEnclosure'      , 'nImplant', 'active'  , 0.18 , Length|Asymmetric, '2l    rule 4.2 N+ p.29')
    , ('minExtension'      , 'nImplant', 'active'  , 0.18 , Length|Asymmetric,       'dup. 4.2 N+'     )
    , ('minEnclosure'      , 'nImplant', 'cut0'    , 0.135, Length|Asymmetric, '1.5l  rule 4.3 N+ p.29')
    , ('minExtension'      , 'nImplant', 'cut0'    , 0.135, Length|Asymmetric,       'dup. 4.3 N+'     )
    , ('minWidth'          , 'nImplant'            , 0.36 , Length           , '4l    rule 4.4 N+ p.29')
    , ('minSpacing'        , 'nImplant'            , 0.36 , Length           , '4l    rule 4.4 N+ p.29')
    , ('minArea'           , 'nImplant'            , 0    , Area             , 'N/A')
    , ('minSpacing'        , 'nImplant', 'active'  , 0    , Length           , 'N/A')
    , ('minEnclosure'      , 'nImplant', 'poly'    , 0    , Length|Asymmetric, 'N/A')
    , ('minLengthEnclosure', 'nImplant', 'active'  , 0    , Length|Asymmetric, 'N/A')
    , ('minWidthEnclosure' , 'nImplant', 'poly'    , 0    , Length|Asymmetric, 'N/A')
    , ('minGateEnclosure'  , 'nImplant', 'poly'    , 0    , Length|Asymmetric, 'N/A')
    , ('minStrapEnclosure' , 'nImplant'            , 0.135, Length           , 'N/A')
    , ('minSpacing'        , 'nImplant', 'pImplant', 0    , Length           , 'N/A')

    , ('minGateSpacing'    , 'pImplant', 'poly'    , 0.27 , Length           , '3l    rule 4.1 P+ p.29')
    , ('minEnclosure'      , 'pImplant', 'active'  , 0.18 , Length|Asymmetric, '2l    rule 4.2 P+ p.29')
    , ('minExtension'      , 'pImplant', 'active'  , 0.18 , Length|Asymmetric,       'dup. 4.2 P+'     )
    , ('minEnclosure'      , 'pImplant', 'cut0'    , 0.135, Length|Asymmetric, '1.5l  rule 4.3 P+ p.29')
    , ('minExtension'      , 'pImplant', 'cut0'    , 0.135, Length|Asymmetric,       'dup. 4.3 P-'     )
    , ('minWidth'          , 'pImplant'            , 0.36 , Length           , '4l    rule 4.4 P+ p.29')
    , ('minSpacing'        , 'pImplant'            , 0.36 , Length           , '4l    rule 4.4 P+ p.29')
    , ('minArea'           , 'pImplant'            , 0    , Area             , 'N/A')
    , ('minSpacing'        , 'pImplant', 'active'  , 0    , Length           , 'N/A')
    , ('minEnclosure'      , 'pImplant', 'poly'    , 0    , Length|Asymmetric, 'N/A')
    , ('minLengthEnclosure', 'pImplant', 'active'  , 0    , Length|Asymmetric, 'N/A')
    , ('minWidthEnclosure' , 'pImplant', 'poly'    , 0    , Length|Asymmetric, 'N/A')
    , ('minGateEnclosure'  , 'pImplant', 'poly'    , 0    , Length|Asymmetric, 'N/A')
    , ('minStrapEnclosure' , 'pImplant'            , 0.135, Length           , 'N/A')

    , ('minWidth'          , 'cut0'                , 0.18 , Length           , '2l    rule 5.1  p.35')
    , ('minEnclosure'      , 'poly'    , 'cut0'    , 0.135, Length|Asymmetric, '1.5l  rule 5.2  p.35')
    , ('minExtension'      , 'poly'    , 'cut0'    , 0.135, Length|Asymmetric,       'dup. 5.2'      )
    , ('minSpacing'        , 'cut0'                , 0.36 , Length           , '4l    rule 5.3  p.35')
    , ('minGateSpacing'    , 'cut0'    , 'poly'    , 0.18 , Length|Asymmetric, '2l    rule 5.4  p.35')
    , ('minSpacing'        , 'cut0'    , 'poly'    , 0.18 , Length           ,       'dup. 5.4'      )
    , ('minSpacing'        , 'cut0'    , 'active'  , 0.18 , Length           ,       'dup. 5.4'      )
    # Rule 6.1, 6.3 & 6.4 are the same as 5.1, 5.3 & 5.4. 
    , ('minEnclosure'      , 'active'  , 'cut0'    , 0.135, Length|Asymmetric, '1.5l  rule 6.2  p.37')

    , ('minWidth'          , 'metal1'              , 0.27 , Length           , '3l    rule 7.1  p.39')
    , ('minSpacing'        , 'metal1'              , 0.27 , Length           , '3l    rule 7.2  p.39')
    , ('minArea'           , 'metal1'              , 0    , Area             , 'N/A')
    , ('minEnclosure'      , 'metal1'  , 'cut0'    , 0.09 , Length|Asymmetric, '1l    rule 7.3  p.39')
    , ('minExtension'      , 'metal1'  , 'cut0'    , 0.09 , Length|Asymmetric,       'dup. 7.3'      )

    , ('minWidth'          , 'cut1'                , 0.27 , Length           , '3l    rule 8.1  p.40')
    , ('minSpacing'        , 'cut1'                , 0.27 , Length           , '3l    rule 8.2  p.40')
    , ('minEnclosure'      , 'metal1'  , 'cut1'    , 0.09 , Length|Asymmetric, '1l    rule 8.3  p.40')
    , ('lineExtension'     , 'metal1'  , 'cut1'    , 0.09 , Length|Asymmetric, 'N/A')

    , ('minWidth'          , 'metal2'              , 0.27 , Length           , '3l    rule 9.1  p.41')
    , ('minSpacing'        , 'metal2'              , 0.36 , Length           , '4l    rule 9.2  p.41')
    , ('minArea'           , 'metal2'              , 0    , Area             , 'N/A')
    , ('minExtension'      , 'metal2'  , 'cut1'    , 0.09 , Length|Asymmetric, '1l    rule 9.3  p.41')
    , ('minEnclosure'      , 'metal2'  , 'cut1'    , 0.09 , Length|Asymmetric,       'dup. 9.3'      )

    , ('minWidth'          , 'cut2'                , 0.27 , Length           , '3l    rule 14.1 p.42')
    , ('minSpacing'        , 'cut2'                , 0.27 , Length           , '3l    rule 14.2 p.42')
    , ('minEnclosure'      , 'metal2'  , 'cut2'    , 0.09 , Length|Asymmetric, '1l    rule 14.3 p.42')

    , ('minWidth'          , 'metal3'              , 0.27 , Length           , '3l    rule 15.1 p.43')
    , ('minSpacing'        , 'metal3'              , 0.36 , Length           , '4l    rule 15.2 p.43')
    , ('minArea'           , 'metal3'              , 0    , Area             , 'N/A')
    , ('minExtension'      , 'metal3'  , 'cut2'    , 0.09 , Length|Asymmetric, '1l    rule 15.3 p.43')
    , ('minEnclosure'      , 'metal3'  , 'cut2'    , 0.09 , Length|Asymmetric,       'dup. 15.3'     )

    , ('minWidth'          , 'cut3'                , 0.27 , Length           , '3l    rule 21.1 p.44')
    , ('minSpacing'        , 'cut3'                , 0.27 , Length           , '3l    rule 21.2 p.44')
    , ('minEnclosure'      , 'metal3'  , 'cut3'    , 0.09 , Length|Asymmetric, '1l    rule 21.3 p.44')

    , ('minWidth'          , 'metal4'              , 0.27 , Length           , '3l    rule 22.1 p.45')
    , ('minSpacing'        , 'metal4'              , 0.36 , Length           , '4l    rule 22.2 p.45')
    , ('minArea'           , 'metal4'              , 0    , Area             , 'N/A')
    , ('minExtension'      , 'metal4'  , 'cut3'    , 0.09 , Length|Asymmetric, '1l    rule 22.3 p.45')
    , ('minEnclosure'      , 'metal4'  , 'cut3'    , 0.09 , Length|Asymmetric,       'dup. 22.3'     )

    , ('minWidth'          , 'cut4'                , 0.27 , Length           , '3l    rule 25.1 p.49')
    , ('minSpacing'        , 'cut4'                , 0.27 , Length           , '3l    rule 25.2 p.49')
    , ('minEnclosure'      , 'metal4'  , 'cut4'    , 0.09 , Length|Asymmetric, '1l    rule 25.3 p.49')

    , ('minWidth'          , 'metal5'              , 0.27 , Length           , '3l    rule 26.1 p.50')
    , ('minSpacing'        , 'metal5'              , 0.36 , Length           , '4l    rule 26.2 p.50')
    , ('minArea'           , 'metal5'              , 0    , Area             , 'N/A')
    , ('minExtension'      , 'metal5'  , 'cut4'    , 0.09 , Length|Asymmetric, '1l    rule 26.3 p.50')
    , ('minEnclosure'      , 'metal5'  , 'cut4'    , 0.09 , Length|Asymmetric,       'dup. 26.3'     )

    , ('minWidth'          , 'cut5'                , 0.36 , Length           , '4l    rule 29.1 p.51')
    , ('minSpacing'        , 'cut5'                , 0.36 , Length           , '4l    rule 29.2 p.51')
    , ('minEnclosure'      , 'metal5'  , 'cut5'    , 0.09 , Length|Asymmetric, '1l    rule 29.3 p.51')

    , ('minWidth'          , 'metal6'              , 0.45 , Length           , '5l    rule 30.1 p.52')
    , ('minSpacing'        , 'metal6'              , 0.45 , Length           , '5l    rule 30.2 p.52')
    , ('minArea'           , 'metal6'              , 0    , Area             , 'N/A')
    , ('minExtension'      , 'metal6'  , 'cut5'    , 0.18 , Length|Asymmetric, '2l    rule 30.3 p.52')
    , ('minEnclosure'      , 'metal6'  , 'cut5'    , 0.18 , Length|Asymmetric,       'dup. 30.3'     )

    # MIM Rules (Metal In Metal Capacitor).
    # In SCN6M_DEEP metbot is metal5 and metcap is *below* metal6.
    # metbot can be accessed with cut4 (down) and via5 (up).
    # metcap can only be accessed *from* up with cut5.
    , ('minWidth'          , 'metcap'              , 4.05 , Length           , '45l   rule 28.1   p.46')
    , ('minWidth'          , 'metcapdum'           , 0.45 , Length           , '9l    rule 28.8   p.47')
    , ('maxWidth'          , 'metcap'              , 30.0 , Length           , '30um  rule 28.12  p.47')
    , ('maxWidth'          , 'metbot'              , 35.0 , Length           , '35um  rule 28.13  p.47')
    , ('minSpacing'        , 'metcap'              , 1.26 , Length           , '14l   rule 28.2   p.46')
    , ('minSpacing'        , 'metbot'              , 0.81 , Length           , '9l    rule 28.9   p.47')
    , ('minSpacing'        , 'cut4'    , 'metcap'  , 0.45 , Length           , '5l    rule 28.5 D p.46')
    , ('minSpacing'        , 'cut5'    , 'metcap'  , 0.45 , Length           , '5l    rule 28.5 U p.46')
    , ('minSpacingOnMetbot', 'cut5'                , 4.05 , Length           , '45l   rule 28.11  p.47')
    , ('minSpacingOnMetcap', 'cut5'                , 2.07 , Length           , '23l   rule 28.10  p.47')
    , ('minEnclosure'      , 'metbot'  , 'metcap'  , 0.45 , Length|Asymmetric, '5l    rule 28.3   p.46')
    , ('minEnclosure'      , 'metbot'  , 'cut4'    , 0.18 , Length|Asymmetric, '2l    rule 28.6 D p.46')
    , ('minEnclosure'      , 'metbot'  , 'cut5'    , 0.18 , Length|Asymmetric, '2l    rule 28.6 U p.46')
    , ('minEnclosure'      , 'metcap'  , 'cut5'    , 0.27 , Length|Asymmetric, '3l    rule 28.4   p.46')
    , ('minArea'           , 'metcap'              , 0    , Area             , 'N/A')
    , ('minAreaInMetcap'   , 'cut5'                , 0    , Area             , 'N/A')
    )


loadAnalogTechno( analogTechnologyTable, __file__ )
