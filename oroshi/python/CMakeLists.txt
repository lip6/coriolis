# -*- explicit-buffer-name: "CMakeLists.txt<oroshi/python>" -*-

 set( pythonFiles __init__.py
                  Rules.py
                  Stack.py
                  ParamsMatrix.py
                  WIP_Transistor.py
                  WIP_DP.py
                  WIP_CSP.py
                  NonUnitCapacitor.py
                  CapacitorUnit.py
                  CapacitorMatrix.py
                  CapacitorVRTracks.py
                  CapacitorRouted.py
                  CapacitorRoutedSingle.py
                  MultiCapacitor.py
                  ResistorSnake.py
                  Resistor.py
                )

 install( FILES ${pythonFiles}    DESTINATION ${PYTHON_SITE_PACKAGES}/oroshi ) 


#if(BUILD_DOC)
#  find_package(Doxygen)

#  if(DOXYGEN_FOUND)
#    add_custom_target( doc ALL cd ${PHAROS_SOURCE_DIR}/python && ${DOXYGEN_EXECUTABLE} doxyfile )
#    install( DIRECTORY html/             DESTINATION share/doc/coriolis2/en/html/oroshi ) 
#    install( FILES     images-sizes.css  DESTINATION share/doc/coriolis2/en/html/oroshi ) 
#    install( DIRECTORY latex/            DESTINATION share/doc/coriolis2/en/latex/oroshi ) 
#  else()
#    message( STATUS "doxygen not found, documentation not generated." )
#  endif()
#endif()
