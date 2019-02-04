# -*- explicit-buffer-name: "CMakeLists.txt<unittests/src>" -*-

   include_directories ( ${CORIOLIS_INCLUDE_DIR}
                         ${HURRICANE_INCLUDE_DIR}
                         ${UTILITIES_INCLUDE_DIR}
                         ${QtX_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIR}
                       )

                   set ( mocincludes   
                       )
                   set ( cpps          unittests.cpp
                       )
 
          qtX_wrap_cpp ( mocCpps       ${mocincludes} )

if(NOT WITH_QT5)
                  list ( APPEND        cpps ${mocCpps} )
endif()

        add_executable ( unittests     ${cpps} )
 target_link_libraries ( unittests     ${CORIOLIS_PYTHON_LIBRARIES}
                                       ${CORIOLIS_LIBRARIES}
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
                                       ${LIBBFD_LIBRARIES}
                       )
               install ( TARGETS       unittests        DESTINATION bin )

