
                     find_program( PELICAN_EXECUTABLE NAMES         pelican
                                                      HINTS         $ENV{PELICAN_DIR}
                                                      PATH_SUFFIXES bin
                                                      DOC           "Pelican blog generator"
                                 )
 
                          include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Pelican DEFAULT_MSG PELICAN_EXECUTABLE )
                 mark_as_advanced( PELICAN_EXECUTABLE )
