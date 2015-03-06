
               set ( examplesInstallDir  share/doc/coriolis2/examples )

           install ( FILES       accu.vst
                                 alu.vst
                                 amd2901.vst
                                 coeur.vst
                                 muxe.vst
                                 muxs.vst
                                 ram.vst
                                 amd2901_chip.py  DESTINATION ${examplesInstallDir}/AM2901 )
           install ( FILES       doChip.py        PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
                                                              OWNER_READ    GROUP_READ    WORLD_READ
                                                              OWNER_WRITE
                                                  DESTINATION ${examplesInstallDir}/AM2901 )
           install ( DIRECTORY   .coriolis2       DESTINATION ${examplesInstallDir}/AM2901 )

