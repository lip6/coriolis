
if ( CHECK_DETERMINISM )
  add_definitions ( -DCHECK_DETERMINISM ) 
endif ( CHECK_DETERMINISM )

               include ( ${QT_USE_FILE} )

   include_directories ( ${KATABATIC_SOURCE_DIR}/src
                         ${CORIOLIS_INCLUDE_DIR} 
                         ${HURRICANE_INCLUDE_DIR} 
                         ${CONFIGURATION_INCLUDE_DIR} 
                         ${Boost_INCLUDE_DIRS}
                       )
                   set ( includes      katabatic/Configuration.h
                                       katabatic/ChipTools.h
                                       katabatic/AutoContact.h     katabatic/AutoContacts.h
                                       katabatic/AutoSegment.h     katabatic/AutoSegments.h
                                       katabatic/AutoHorizontal.h
                                       katabatic/AutoVertical.h
                                       katabatic/Grid.h            katabatic/GridCollections.h
				       katabatic/GridBox.h
                                       katabatic/GCell.h           katabatic/GCells.h
                                       katabatic/GCellGrid.h
                                       katabatic/Session.h
                                       katabatic/KatabaticEngine.h
                                       katabatic/GraphicKatabaticEngine.h
                       )
		   set ( mocIncludes   katabatic/GraphicKatabaticEngine.h )
                   set ( cpps          Configuration.cpp
                                       ChipTools.cpp
                                       AutoContact.cpp
                                       AutoContacts.cpp
                                       AutoSegment.cpp
                                       AutoSegments.cpp
                                       AutoHorizontal.cpp
                                       AutoVertical.cpp
                                       Grid.cpp
                                       GCell.cpp
                                       GCellGrid.cpp
                                       PowerRails.cpp
                                       Session.cpp
                                       LayerAssign.cpp
                                       LoadGrByNet.cpp
                                       NetConstraints.cpp
                                       NetOptimals.cpp
				       ChipTools.cpp
                                       KatabaticEngine.cpp
                                       GraphicKatabaticEngine.cpp
                       )
          qt4_wrap_cpp ( mocCpps       ${mocIncludes} )


           add_library ( katabatic  ${cpps} ${mocCpps} )
 set_target_properties ( katabatic  PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( katabatic  ${KNIK_LIBRARIES}
                                    ${CORIOLIS_LIBRARIES}
                                    ${HURRICANE_PYTHON_LIBRARIES}
                                    ${HURRICANE_GRAPHICAL_LIBRARIES}
                                    ${HURRICANE_LIBRARIES}
                                    ${CONFIGURATION_LIBRARY}
                                    ${CIF_LIBRARY}
                                    ${AGDS_LIBRARY}
                                    ${LEFDEF_LIBRARIES}
                                    ${OA_LIBRARIES}
                                    ${QT_LIBRARIES}
                                    ${Boost_LIBRARIES}
                                    ${LIBXML2_LIBRARIES}
                                    ${PYTHON_LIBRARIES} -lutil
                       )

               install ( TARGETS katabatic DESTINATION lib${LIB_SUFFIX} )
               install ( FILES ${includes}
                               ${mocIncludes} DESTINATION include/coriolis2/katabatic ) 
   
