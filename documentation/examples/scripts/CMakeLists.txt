
               set ( examplesInstallDir  share/doc/coriolis2/examples )

           install ( FILES       invertor.py
                                 fulladder.py
	                         toolengines.py   DESTINATION ${examplesInstallDir}/scripts )
           install ( DIRECTORY   .coriolis2       DESTINATION ${examplesInstallDir}/scripts )

