# -*- explicit-buffer-name: "CMakeLists.txt<cumulus/src>" -*-

              set ( pySources     ${CMAKE_CURRENT_SOURCE_DIR}/placeandroute.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/ref.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/Alliance.py
                  )		  
              set ( pyPlugins     ${CMAKE_CURRENT_SOURCE_DIR}/plugins/__init__.py
                                 #${CMAKE_CURRENT_SOURCE_DIR}/plugins/VChannelsPlugin.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/ClockTreePlugin.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/CoreToChip_cmos.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/CoreToChip_phlib80.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/ChipPlace.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/ChipRoute.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/ConductorPlugin.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/RSavePlugin.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/RSavePluginAll.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/S2R.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/AboutWindow.py
                  )		  
             #set ( pyPluginBlock ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/__init__.py
             #                    ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/VChannels.py
             #    )		  
              set ( pyPluginCT    ${CMAKE_CURRENT_SOURCE_DIR}/plugins/clocktree/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/clocktree/RSMT.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/clocktree/ClockTree.py
                  )		  
              set ( pyPluginC2C   ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/CoreToChip.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/cmos.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/phlib80.py
                  )		  
              set ( pyPluginChip  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/Configuration.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/BlockPower.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/BlockCorona.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/PadsCorona.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/Chip.py
                  )
         
          install ( FILES ${pySources}     DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus )
         #install ( FILES ${pyPlugins}     DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins )
          install ( FILES ${pyPluginBlock} DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins/block )
          install ( FILES ${pyPluginCT}    DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins/clocktree )
          install ( FILES ${pyPluginC2C}   DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins/core2chip )
          install ( FILES ${pyPluginChip}  DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins/chip )
