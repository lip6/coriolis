
               include ( ${QT_USE_FILE} )

   include_directories ( ${SOLSTICE_SOURCE_DIR}/src
                         ${HURRICANE_INCLUDE_DIR}
                         ${CORIOLIS_INCLUDE_DIR}
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

          qt4_wrap_cpp ( mocCpps       ${mocIncludes} )

           add_library ( solstice       ${cpps} ${mocCpps} )
               install ( TARGETS        solstice DESTINATION lib${LIB_SUFFIX} )
               install ( FILES          ${includes}
                                        ${mocIncludes} DESTINATION include/coriolis2/solstice ) 


