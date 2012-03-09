
              set ( pysources ${CMAKE_CURRENT_SOURCE_DIR}/st_bool.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_cat.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_comp.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_config.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_const.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_extend.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_func_gen_wallace.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_generate.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_getrealmodel.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_instance.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_model.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_mult_blocs.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_mult_matrix.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_mult.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_mux.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_net.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_param.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_parser.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_placeAndRoute.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_placement.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/stratus.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_ref.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_shifter.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/st_slansky.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/util_Const.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/util_Defs.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/util_Gen.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/util_Misc.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/util_Place.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/util.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/util_Route.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/util_Shift.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/util_uRom.py
                  )
         
          install ( FILES ${pysources} DESTINATION ${PYTHON_SITE_PACKAGES}/stratus )
