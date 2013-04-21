
               include ( ${QT_USE_FILE} )
   include_directories ( ${CRLCORE_SOURCE_DIR}/src/ccore
                         ${HURRICANE_INCLUDE_DIR}
                         ${UTILITIES_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIR}
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
                                       ${BOOKSHELF_LIBRARY}
                                       ${AGDS_LIBRARY}
                                       ${CIF_LIBRARY}
                                       ${CONFIGURATION_LIBRARY}
                                       ${UTILITIES_LIBRARY}
                                       ${LEFDEF_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${QT_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${PYTHON_LIBRARIES}
                                       -lutil
                                       ${LIBXML2_LIBRARIES}
                                       ${LIBEXECINFO_LIBRARIES}
                       )
               install ( TARGETS       cyclop        DESTINATION bin )

