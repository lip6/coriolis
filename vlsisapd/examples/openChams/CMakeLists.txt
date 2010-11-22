ADD_SUBDIRECTORY(cplusplus)
ADD_SUBDIRECTORY(python)

SET ( XML_FILES inverter.xml
                buffer.xml
    )

INSTALL ( FILES ${XML_FILES} DESTINATION share/doc/coriolis2/examples/vlsisapd/openChams )
