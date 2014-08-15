# -*- explicit-buffer-name: "CMakeLists.txt<crlcore/src/ccore/cyclop>" -*-

#              include ( ${QT_USE_FILE} )
   include_directories ( ${CRLCORE_SOURCE_DIR}/src/ccore
                         ${HURRICANE_INCLUDE_DIR}
                         ${UTILITIES_INCLUDE_DIR}
                         ${QtX_INCLUDE_DIR}
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
 
          qtX_wrap_cpp ( mocCpps       ${mocincludes} )

      link_directories ( ${CRLCORE_BINARY_DIR}/src/ccore )

#       add_executable ( cyclop        ${cpps} )
        add_executable ( cyclop        ${cpps} ${mocCpps} )
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
                                       ${QtX_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${PYTHON_LIBRARIES}
                                       -lutil
                                       ${LIBXML2_LIBRARIES}
                                       ${LIBEXECINFO_LIBRARIES}
                       )
               install ( TARGETS       cyclop        DESTINATION bin )

