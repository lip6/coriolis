# -*- explicit-buffer-name: "CMakeLists.txt<cumulus/src>" -*-

              set ( pySources     ${CMAKE_CURRENT_SOURCE_DIR}/placeandroute.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/ref.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/Alliance.py
                  )		  
              set ( pyPlugins     ${CMAKE_CURRENT_SOURCE_DIR}/plugins/__init__.py
                                 #${CMAKE_CURRENT_SOURCE_DIR}/plugins/vchannels.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/clocktree.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip_cmos.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip_phlib80.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chipplace.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chiproute.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/conductor.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/matrixplacer.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/rsave.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/rsaveall.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/s2r.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/aboutwindow.py
                  )		  
             #set ( pyPluginBlock ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/__init__.py
             #                    ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/vchannels.py
             #    )		  
              set ( pyPluginCTS   ${CMAKE_CURRENT_SOURCE_DIR}/plugins/cts/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/cts/rsmt.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/cts/clocktree.py
                  )		  
              set ( pyPluginC2C   ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/core2chip.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/cmos.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/phlib80.py
                  )		  
              set ( pyPluginChip  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/configuration.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/blockpower.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/blockcorona.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/padscorona.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/chip.py
                  )
              set ( pyTools       ${CMAKE_CURRENT_SOURCE_DIR}/tools/blif2vst.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/tools/yosys.py
                  )
         
          install ( FILES    ${pySources}     DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus )
          install ( FILES    ${pyPlugins}     DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins )
         #install ( FILES    ${pyPluginBlock} DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins/block )
          install ( FILES    ${pyPluginCTS}   DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins/cts )
          install ( FILES    ${pyPluginC2C}   DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins/core2chip )
          install ( FILES    ${pyPluginChip}  DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins/chip )
         #install ( PROGRAMS ${pyTools}       DESTINATION bin ) 
