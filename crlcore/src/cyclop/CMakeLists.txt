
               include ( ${QT_USE_FILE} )
   include_directories ( ${Boost_INCLUDE_DIRS}
                         ${HURRICANE_INCLUDE_DIR}
                         ${CRLCORE_SOURCE_DIR}/src/ccore
                       )

                   set ( mocincludes   Cyclop.h
                                       COpenCellDialog.h
                       )
		   set ( cpps          COpenCellDialog.cpp
                                       DemoGo.cpp
                                       Cyclop.cpp
                                       CyclopMain.cpp
                       )
 
          qt4_wrap_cpp ( MOCcpps       ${mocincludes} )

      link_directories ( ${CRLCORE_BINARY_DIR}/src/ccore )

        add_executable ( cyclop        ${cpps} ${MOCcpps} )
 target_link_libraries ( cyclop        crlcore
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${IO_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${QT_LIBRARIES}
                                       ${Boost_LIBRARIES} )
               install ( TARGETS       cyclop        DESTINATION /bin )

