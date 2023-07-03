# -*- explicit-buffer-name: "CMakeLists.txt<crlcore/src/ccore/cyclop>" -*-

   include_directories ( ${CRLCORE_SOURCE_DIR}/src/ccore
                         ${HURRICANE_INCLUDE_DIR}
                         ${UTILITIES_INCLUDE_DIR}
                         ${QtX_INCLUDE_DIRS}
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
 
          qtX_wrap_cpp ( moccpps       ${mocincludes} )

      link_directories ( ${CRLCORE_BINARY_DIR}/src/ccore )

        add_executable ( cyclop        ${cpps} ${moccpps} )
 target_link_libraries ( cyclop        crlcore
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${HURRICANE_PYTHON_NEW_LIBRARIES}
                                       ${HURRICANE_PYTHON_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${BOOKSHELF_LIBRARY}
                                       ${LEFDEF_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${QtX_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${Python_LIBRARIES}
                                       -lutil
                                       ${LIBXML2_LIBRARIES}
                                       ${BZIP2_LIBRARIES}
                                       ${LIBEXECINFO_LIBRARIES}
                                       ${LIBBFD_LIBRARIES}
                                       z
                       )
               install ( TARGETS       cyclop        DESTINATION bin )

