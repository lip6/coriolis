# -*- explicit-buffer-name: "CMakeLists.txt<oroshi/python>" -*-

 set( pythonFiles __init__.py
                  dtr.py
                  stack.py
                  paramsmatrix.py
                  wip_transistor.py
                  wip_dp.py
                  wip_csp.py
                  nonunitcapacitor.py
                  capacitorunit.py
                  capacitormatrix.py
                  capacitorvrtracks.py
                  capacitorrouted.py
                  capacitorroutedsingle.py
                  multicapacitor.py
                  resistorsnake.py
                  resistor.py
                )

 install( FILES ${pythonFiles}    DESTINATION ${Python_CORIOLISLIB}/oroshi ) 


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
