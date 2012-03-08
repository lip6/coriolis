
              set ( pysources ${CMAKE_CURRENT_SOURCE_DIR}/patread.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/patterns.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/patwrite.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/synopsys.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/utils.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/stimuli.py
                  )
         
          install ( FILES ${pysources} DESTINATION ${PYTHON_SITE_PACKAGES}/stratus )
