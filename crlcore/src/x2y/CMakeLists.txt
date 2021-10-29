# -*- explicit-buffer-name: "CMakeLists.txt<crlcore/src/x2y> -*-
  
   include_directories ( ${CRLCORE_SOURCE_DIR}/src/ccore
                         ${HURRICANE_INCLUDE_DIR}
                         ${UTILITIES_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIRS}
                       )

		           set ( cpps        x2y.cpp )
      link_directories ( ${CRLCORE_BINARY_DIR}/src/ccore )
 
        add_executable ( cx2y        ${cpps} )
 target_link_libraries ( cx2y        crlcore
                                     ${HURRICANE_PYTHON_NEW_LIBRARIES}
                                     ${HURRICANE_PYTHON_LIBRARIES}
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${BOOKSHELF_LIBRARY}
                                     ${LEFDEF_LIBRARIES}
                                     ${OA_LIBRARIES}
                                     ${QtX_LIBRARIES}
                                     ${Boost_LIBRARIES}
                                     ${Python_LIBRARIES}
                                     -lutil
                                     ${LIBXML2_LIBRARIES}
                                     ${LIBEXECINFO_LIBRARIES}
                                     ${LIBBFD_LIBRARIES}
                       )
               install ( TARGETS     cx2y DESTINATION bin )
