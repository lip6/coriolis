
#              include ( ${QT_USE_FILE} )

   include_directories ( ${SOLSTICE_SOURCE_DIR}/src
                         ${HURRICANE_INCLUDE_DIR}
                         ${CORIOLIS_INCLUDE_DIR}
                         ${UTILITIES_INCLUDE_DIR}
                         ${QtX_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIRS}
                       )


                   set ( includes    solstice/Brick.h
                                     solstice/ShortCircuitError.h
				     solstice/DisconnectError.h	
		       	 	     solstice/SolsticeEngine.h
		       	 	     solstice/RoutingError.h
		       	 	     solstice/BrickSweepLine.h
				     solstice/RoutingErrorInformations.h
                       )
                   set ( mocIncludes solstice/RoutingErrorListModel.h 
                                     solstice/RoutingErrorListWidget.h 
                                     solstice/TabRoutingErrorList.h 
                                     solstice/GraphicSolsticeEngine.h )

		   set ( cpps           Brick.cpp
                                        RoutingError.cpp
		       	 	        BrickSweepLine.cpp
                                        SolsticeEngine.cpp
                                        ShortCircuitError.cpp	
                                        GraphicSolsticeEngine.cpp
					RoutingErrorListModel.cpp
					DisconnectError.cpp
					RoutingErrorListWidget.cpp
					TabRoutingErrorList.cpp
					RoutingErrorInformations.cpp
  
                       )

          qtX_wrap_cpp ( mocCpps        ${mocIncludes} )

           add_library ( solstice       ${cpps} ${mocCpps} )
 set_target_properties ( solstice       PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( solstice       ${EQUINOX_LIBRARIES}
                                        ${CORIOLIS_LIBRARIES}
                                        ${HURRICANE_PYTHON_LIBRARIES}
                                        ${HURRICANE_GRAPHICAL_LIBRARIES}
                                        ${HURRICANE_LIBRARIES}
                                        ${CONFIGURATION_LIBRARY}
                                        ${CIF_LIBRARY}
                                        ${AGDS_LIBRARY}
                                        ${LEFDEF_LIBRARIES}
                                        ${OA_LIBRARIES}
                                        ${QtX_LIBRARIES}
                                        ${Boost_LIBRARIES}
                                        ${LIBXML2_LIBRARIES}
                                        ${PYTHON_LIBRARIES} -lutil
                       )

               install ( TARGETS        solstice DESTINATION lib${LIB_SUFFIX} )
               install ( FILES          ${includes}
                                        ${mocIncludes} DESTINATION include/coriolis2/solstice ) 


