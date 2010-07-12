
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
                       )
               install ( TARGETS       cyclop        DESTINATION bin )

