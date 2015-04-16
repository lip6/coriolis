# -*- explicit-buffer-name: "CMakeLists.txt<cumulus/src>" -*-

              set ( pySources    ${CMAKE_CURRENT_SOURCE_DIR}/placeandroute.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/ref.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/Alliance.py
                  )
              set ( pyPlugins    ${CMAKE_CURRENT_SOURCE_DIR}/plugins/__init__.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/ChipPlugin.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/ClockTreePlugin.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/RSavePlugin.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/RSavePluginAll.py
                  )
              set ( pyPluginCT   ${CMAKE_CURRENT_SOURCE_DIR}/plugins/clocktree/__init__.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/clocktree/RSMT.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/clocktree/ClockTree.py
                  )
              set ( pyPluginChip ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/__init__.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/BlockPower.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/BlockCorona.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/PadsCorona.py
                                 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/Configuration.py
                  )
         
          install ( FILES ${pySources}    DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus )
          install ( FILES ${pyPlugins}    DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins )
          install ( FILES ${pyPluginCT}   DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins/clocktree )
          install ( FILES ${pyPluginChip} DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins/chip )
