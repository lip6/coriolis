ADD_SUBDIRECTORY(cplusplus)
#ADD_SUBDIRECTORY(python)

SET ( SP_FILES OTA_miller.spi
    )

INSTALL ( FILES ${SP_FILES} DESTINATION share/doc/coriolis2/examples/vlsisapd/spice )
