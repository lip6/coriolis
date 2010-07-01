
               include ( ${QT_USE_FILE} )
   include_directories ( ${CRLCORE_SOURCE_DIR}/src/ccore
                         ${HURRICANE_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIRS}
                       )

                   set ( mocincludes   Cyclop.h
                                       OpenCellDialog.h
                       )
		   set ( cpps          OpenCellDialog.cpp
                                       DemoGo.cpp
                                       Cyclop.cpp
                                       CyclopMain.cpp
                       )
 
          qt4_wrap_cpp ( MOCcpps       ${mocincludes} )

      link_directories ( ${CRLCORE_BINARY_DIR}/src/ccore )

        add_executable ( cyclop        ${cpps} ${MOCcpps} )
 target_link_libraries ( cyclop        crlcore
                                       ${HURRICANE_PYTHON_LIBRARIES}
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${CONFIGURATION_LIBRARY}
                                       ${AGDS_LIBRARY}
                                       ${CIF_LIBRARY}
                                       ${IO_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${LEFDEF_LIBRARIES}
                                       ${QT_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${LIBXML2_LIBRARIES}
                                       ${PYTHON_LIBRARIES}
                                       -lutil
                       )
               install ( TARGETS       cyclop        DESTINATION bin )

