
              set ( pysources ${CMAKE_CURRENT_SOURCE_DIR}/placeandroute.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/ref.py
                  )
         
          install ( FILES ${pysources} DESTINATION ${PYTHON_SITE_PACKAGES}/cumulus )
