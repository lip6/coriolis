


               include ( ${QT_USE_FILE} )
   include_directories ( ${UNICORN_SOURCE_DIR}/src
                         ${HURRICANE_INCLUDE_DIR}
                         ${CORIOLIS_INCLUDE_DIR}
                         ${BOOKSHELF_INCLUDE_DIR}
                         ${CONFIGURATION_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIRS}
                         ${LEFDEF_INCLUDE_DIR}
                         ${PYTHON_INCLUDE_PATH}
                       )
       add_definitions ( -DSYS_CONF_DIR=\\"${SYS_CONF_DIR}\\" )

                   set ( mocincludes   unicorn/UnicornGui.h
                                       unicorn/OpenCellDialog.h
                                       unicorn/SaveCellDialog.h
                                       unicorn/ImportCellDialog.h
                                       unicorn/ExportCellDialog.h
                       )
		   set ( cpps          OpenCellDialog.cpp
                                       SaveCellDialog.cpp
                                       ImportCellDialog.cpp
                                       ExportCellDialog.cpp
                                       UnicornGui.cpp
                       )
                   set ( cgtcpp        CgtMain.cpp )
 
          qt4_wrap_cpp ( MOCcpps       ${mocincludes} )
     qt4_add_resources ( RCC_SRCS      Unicorn.qrc )

           add_library ( unicorn       ${cpps} ${MOCcpps} )
 set_target_properties ( unicorn       PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( unicorn       ${SOLSTICE_GRAPHICAL_LIBRARIES}
                                       ${SOLSTICE_LIBRARIES}
                                       ${EQUINOX_GRAPHICAL_LIBRARIES}
                                       ${EQUINOX_LIBRARIES}
                                       ${KITE_GRAPHICAL_LIBRARIES}
                                       ${KITE_LIBRARIES}
                                       ${KATABATIC_GRAPHICAL_LIBRARIES}
                                       ${KATABATIC_LIBRARIES}
                                       ${KNIK_GRAPHICAL_LIBRARIES}
                                       ${KNIK_LIBRARIES}
                                       ${MAUKA_GRAPHICAL_LIBRARIES}
                                       ${MAUKA_LIBRARIES}
                                       ${METIS_LIBRARIES}
                                       ${HMETIS_LIBRARIES}
                                       ${NIMBUS_GRAPHICAL_LIBRARIES}
                                       ${NIMBUS_LIBRARIES}
                                       ${CORIOLIS_PYTHON_LIBRARIES}
                                       ${CORIOLIS_LIBRARIES}
                                       ${HURRICANE_PYTHON_LIBRARIES}
                                       ${HURRICANE_GRAPHICAL_LIBRARIES}
                                       ${HURRICANE_LIBRARIES}
                                       ${BOOKSHELF_LIBRARY}
                                       ${AGDS_LIBRARY}
                                       ${CIF_LIBRARY}
                                       ${CONFIGURATION_LIBRARY}
                                       ${LEFDEF_LIBRARIES}
                                       ${OA_LIBRARIES}
                                       ${QT_LIBRARIES}
                                       ${Boost_LIBRARIES}
                                       ${PYTHON_LIBRARIES}
                                       -lutil
                                       ${LIBXML2_LIBRARIES}
                       )
        add_executable ( cgt           ${cgtcpp} )
 target_link_libraries ( cgt           unicorn )
               install ( TARGETS       unicorn        DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS       cgt            DESTINATION bin )
               install ( FILES         ${mocincludes} DESTINATION include/coriolis2/unicorn ) 

