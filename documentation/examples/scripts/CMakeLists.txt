
               set ( examplesInstallDir  share/doc/coriolis2/examples )

           install ( FILES       invertor.py
                                fulladder.py
                                 polygons.py
                                diagonals.py
                              rectilinear.py
                             coriolisLogo.py
	                          toolengines.py   DESTINATION ${examplesInstallDir}/scripts )
           install ( DIRECTORY  coriolis2      DESTINATION ${examplesInstallDir}/scripts )

