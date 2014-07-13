# -*- explicit-buffer-name: "CMakeLists.txt<cumulus/src>" -*-

              set ( pysources ${CMAKE_CURRENT_SOURCE_DIR}/placeandroute.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/ref.py
                  )
              set ( pyplugins ${CMAKE_CURRENT_SOURCE_DIR}/plugins/__init__.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/plugins/ClockTree.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/plugins/Chip.py
                  )
         
          install ( FILES ${pysources} DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus )
          install ( FILES ${pyplugins} DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus/plugins )
