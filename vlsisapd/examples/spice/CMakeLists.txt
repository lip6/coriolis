ADD_SUBDIRECTORY(cplusplus)
#ADD_SUBDIRECTORY(python)

SET ( SP_FILES OTA.cir
               np_mos.spi
               otaTwoStage.spi
    )

INSTALL ( FILES ${SP_FILES} DESTINATION share/doc/coriolis2/examples/vlsisapd/spice )
