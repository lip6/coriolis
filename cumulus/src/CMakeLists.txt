# -*- explicit-buffer-name: "CMakeLists.txt<cumulus/src>" -*-

              set ( pyDesignFlow  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/task.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/copy.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/vasy.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/boom.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/boog.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/loon.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/genpat.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/asimut.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/flatph.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/cougar.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/s2r.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/lvx.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/druc.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/graal.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/dreal.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/sv2v.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/svase.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/surelog.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/yosys.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/blif2vst.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/pnr.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/klayout.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/command.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/clean.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/alias.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/technos.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/routecheck.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/pnrcheck.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/designflow/alliancesynth.py
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
              set ( pyPlugins     ${CMAKE_CURRENT_SOURCE_DIR}/plugins/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/utils.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/checks.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/rsave.py
                  )
              set ( pyPluginBlock ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/configuration.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/iospecs.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/bigvia.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/spares.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/block.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/htree.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/timing.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/rsmt.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/hfns1.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/hfns2.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/hfns3.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/hfns4.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/block/matrix.py
                  )
              set ( pyPluginC2C   ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/core2chip.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/cmos.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/niolib.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/libresocio.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/core2chip/sky130.py
                  )
              set ( pyPluginChip  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/constants.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/configuration.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/power.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/powerplane.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/corona.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/pads.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/chip/chip.py
                  )
              set ( pyPluginMacro ${CMAKE_CURRENT_SOURCE_DIR}/plugins/macro/__init__.py
                                  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/macro/macro.py
                  )
              set ( pyPluginHarness ${CMAKE_CURRENT_SOURCE_DIR}/plugins/harness/__init__.py
                                    ${CMAKE_CURRENT_SOURCE_DIR}/plugins/harness/pads.py
                  )
         
          install ( FILES    ${pyDesignFlow}    DESTINATION ${Python_CORIOLISLIB}/designflow )
          install ( FILES    ${pyPlugins}       DESTINATION ${Python_CORIOLISLIB}/plugins )
          install ( FILES    ${pyPluginSRAM}    DESTINATION ${Python_CORIOLISLIB}/plugins/sram )
          install ( FILES    ${pyPluginBlock}   DESTINATION ${Python_CORIOLISLIB}/plugins/block )
          install ( FILES    ${pyPluginC2C}     DESTINATION ${Python_CORIOLISLIB}/plugins/core2chip )
          install ( FILES    ${pyPluginChip}    DESTINATION ${Python_CORIOLISLIB}/plugins/chip )
          install ( FILES    ${pyPluginMacro}   DESTINATION ${Python_CORIOLISLIB}/plugins/macro )
          install ( FILES    ${pyPluginHarness} DESTINATION ${Python_CORIOLISLIB}/plugins/harness )
          install ( PROGRAMS ${pyTools}         DESTINATION bin ) 
