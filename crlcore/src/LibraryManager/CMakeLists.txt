# -*- explicit-buffer-name: "CMakeLists.txt<crlcore/src/LibraryManager>" -*-

   include_directories( ${CRLCORE_SOURCE_DIR}/src/LibraryManager
                        ${CRLCORE_SOURCE_DIR}/src/ccore
                        ${CRLCORE_SOURCE_DIR}/src/ccore/bookshelf
                        ${CRLCORE_SOURCE_DIR}/src/ccore/alliance/ap
                        ${CRLCORE_SOURCE_DIR}/src/ccore/alliance/vst
                        ${QtX_INCLUDE_DIRS}
                        ${Boost_INCLUDE_DIRS}
                        ${CONFIGURATION_INCLUDE_DIR}
                        ${HURRICANE_INCLUDE_DIR}
                      )

                   set( cpps         ViewsModel.cpp
                                     ViewsWidget.cpp
                                     CellDatas.cpp
                                     CellsModel.cpp
                                     CellsWidget.cpp
                                     LibrariesModel.cpp
                                     LibrariesWidget.cpp
                                     LibraryManager.cpp
                      )	    
                   set( includes     crlcore/CellDatas.h
                      )
                   set( mocincludes  crlcore/ViewsModel.h
                                     crlcore/ViewsWidget.h
                                     crlcore/CellsModel.h
                                     crlcore/CellsWidget.h
                                     crlcore/LibrariesModel.h
                                     crlcore/LibrariesWidget.h
                                     crlcore/LibraryManager.h
                      )

          qtX_wrap_cpp( moccpps      ${mocincludes} )
				     
           add_library( libmanager   ${cpps} ${moccpps} )
 set_target_properties( libmanager   PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( libmanager   crlcore
                                     ${HURRICANE_PYTHON_LIBRARIES}
                                     ${HURRICANE_GRAPHICAL_LIBRARIES}
                                     ${HURRICANE_LIBRARIES}
                                     ${BOOKSHELF_LIBRARY}
                                     ${CONFIGURATION_LIBRARY}
                                     ${CIF_LIBRARY}
                                     ${AGDS_LIBRARY}
                                     ${UTILITIES_LIBRARY}
                                     ${LEFDEF_LIBRARIES}
                                     ${OA_LIBRARIES}
                                     ${QtX_LIBRARIES}
                                     ${Boost_LIBRARIES}
                                     ${LIBXML2_LIBRARIES}
                                      -lutil
                      )

               install( TARGETS      libmanager DESTINATION lib${LIB_SUFFIX} )
               install( FILES        ${includes} ${mocincludes} DESTINATION include/coriolis2/crlcore ) 
