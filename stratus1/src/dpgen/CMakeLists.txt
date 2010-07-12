
              set ( pysources ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_ADSB2F.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_Nul.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_RAM.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_Regular.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_RF1.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_RF2.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_ROM2.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_ROM4.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_ROM.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_Shifter.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/dpgen_Shrot.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/ROM_encours.py
                  )
         
          install ( FILES ${pysources} DESTINATION ${PYTHON_SITE_PACKAGES}/stratus )
