# -*- explicit-buffer-name: "CMakeLists.txt<unicorn/src>" -*-

#              include( ${QT_USE_FILE} )
   include_directories( ${UNICORN_SOURCE_DIR}/src
                        ${HURRICANE_INCLUDE_DIR}
                        ${CORIOLIS_INCLUDE_DIR}
                        ${BOOKSHELF_INCLUDE_DIR}
                        ${CONFIGURATION_INCLUDE_DIR}
                        ${QtX_INCLUDE_DIR}
                        ${Boost_INCLUDE_DIRS}
                        ${LEFDEF_INCLUDE_DIR}
                        ${PYTHON_INCLUDE_PATH}
                      )
       add_definitions( -DSYS_CONF_DIR="${SYS_CONF_DIR}" )

                   set( includes      unicorn/ImportCell.h )
                   set( mocincludes   unicorn/UnicornGui.h
                                      unicorn/OpenCellDialog.h
                                      unicorn/SaveCellDialog.h
                                      unicorn/ImportCellDialog.h
                                      unicorn/ExportCellDialog.h
                      )
                   set( pyIncludes    unicorn/PyUnicornGui.h
                      )
		   set( cpps          ImportCell.cpp
                                      OpenCellDialog.cpp
                                      SaveCellDialog.cpp
                                      ImportCellDialog.cpp
                                      ExportCellDialog.cpp
                                      UnicornGui.cpp
                      )
                   set( pyCpps        PyUnicorn.cpp
                                      PyUnicornGui.cpp
                      )
                   set( cgtcpp        CgtMain.cpp )
 
          qtX_wrap_cpp( mocCpps       ${mocincludes} )
     qtX_add_resources( RCC_SRCS      Unicorn.qrc )

                   set( depLibs       ${SOLSTICE_GRAPHICAL_LIBRARIES}
                                      ${SOLSTICE_LIBRARIES}
                                      ${EQUINOX_GRAPHICAL_LIBRARIES}
                                      ${EQUINOX_LIBRARIES}
                                      ${KITE_GRAPHICAL_LIBRARIES}
                                      ${KITE_LIBRARIES}
                                      ${KATABATIC_GRAPHICAL_LIBRARIES}
                                      ${KATABATIC_LIBRARIES}
                                      ${KNIK_GRAPHICAL_LIBRARIES}
                                      ${KNIK_LIBRARIES}
                                      ${ETESIAN_GRAPHICAL_LIBRARIES}
                                      ${ETESIAN_LIBRARIES}
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
                                      ${UTILITIES_LIBRARY}
                                      ${CONFIGURATION_LIBRARY}
                                      ${COLOQUINTE_LIBRARY}
                                      ${LEFDEF_LIBRARIES}
                                      ${OA_LIBRARIES}
                                      ${QtX_LIBRARIES}
                                      ${Boost_LIBRARIES}
                                      ${PYTHON_LIBRARIES}
                                      -lutil
                                      ${LIBXML2_LIBRARIES}
                      )

           add_library( unicorn       ${cpps} ${mocCpps} ${pyCpps} )
 set_target_properties( unicorn       PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( unicorn       ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "Do_not_generate_C_library"
                        Unicorn
                        "unicorn;${depLibs}"
                        include/coriolis2/unicorn
                      )

        add_executable( cgt.bin       ${cgtcpp} )
 target_link_libraries( cgt.bin       unicorn ${depLibs} )

               install( TARGETS       unicorn        DESTINATION lib${LIB_SUFFIX} )
               install( TARGETS       cgt.bin        DESTINATION bin )
               install( PROGRAMS      cgt.py         DESTINATION bin RENAME cgt )
               install( PROGRAMS      coriolis.py    DESTINATION bin RENAME coriolis )
               install( FILES         ${includes}
                                      ${mocincludes} DESTINATION include/coriolis2/unicorn ) 

