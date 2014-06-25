# -*- explicit-buffer-name: "CMakeLists.txt<cumulus/src>" -*-

              set ( pysources ${CMAKE_CURRENT_SOURCE_DIR}/placeandroute.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/ref.py
                  )
              set ( pyplugins ${CMAKE_CURRENT_SOURCE_DIR}/ClockTree.py
                  )
         
          install ( FILES ${pysources} DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus )
          install ( FILES ${pyplugins} DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins )
