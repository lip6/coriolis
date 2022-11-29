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
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/rsave.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/rsaveall.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/checks.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/s2r.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/aboutwindow.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/stats.py
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
              set ( pyPluginSRAM  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/sram/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/sram/sram.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/sram/sram_256x32.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/sram/sramplacer1.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/sram/sramplacer2.py
                  )
              set ( pyPluginAlpha      ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/__init__.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/utils.py
                  )
              set ( pyPluginAlphaBlock ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/__init__.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/__init__.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/configuration.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/iospecs.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/bigvia.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/spares.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/block.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/htree.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/timing.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/rsmt.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/hfns1.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/hfns2.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/hfns3.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/hfns4.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/block/matrix.py
                  )
              set ( pyPluginAlphaC2C   ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/core2chip/__init__.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/core2chip/core2chip.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/core2chip/cmos.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/core2chip/niolib.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/core2chip/libresocio.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/core2chip/sky130.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/core2chip/gf180mcu.py
                  )
              set ( pyPluginAlphaChip  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/chip/__init__.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/chip/configuration.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/chip/power.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/chip/powerplane.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/chip/corona.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/chip/pads.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/chip/chip.py
                  )
              set ( pyPluginAlphaMacro ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/macro/__init__.py
                                       ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/macro/macro.py
                  )
              set ( pyPluginAlphaHarness ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/harness/__init__.py
                                         ${CMAKE_CURRENT_SOURCE_DIR}/plugins/alpha/harness/pads.py
                  )
         
          install ( FILES    ${pySources}            DESTINATION ${Python_CORIOLISLIB}/cumulus )
          install ( FILES    ${pyPlugins}            DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins )
          install ( FILES    ${pyPluginCTS}          DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/cts )
          install ( FILES    ${pyPluginC2C}          DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/core2chip )
          install ( FILES    ${pyPluginC2C}          DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/core2chip )
          install ( FILES    ${pyPluginChip}         DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/chip )
          install ( FILES    ${pyPluginSRAM}         DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/sram )
          install ( FILES    ${pyPluginAlpha}        DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/alpha )
          install ( FILES    ${pyPluginAlphaBlock}   DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/alpha/block )
          install ( FILES    ${pyPluginAlphaC2C}     DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/alpha/core2chip )
          install ( FILES    ${pyPluginAlphaChip}    DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/alpha/chip )
          install ( FILES    ${pyPluginAlphaMacro}   DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/alpha/macro )
          install ( FILES    ${pyPluginAlphaHarness} DESTINATION ${Python_CORIOLISLIB}/cumulus/plugins/alpha/harness )
          install ( PROGRAMS ${pyTools}              DESTINATION bin ) 
