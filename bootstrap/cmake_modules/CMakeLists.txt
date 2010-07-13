
     set ( finders FindFLEX.cmake
                   FindBISON.cmake
                   FindPythonSitePackages.cmake
                   FindGoodies.cmake
         )
               
 install ( FILES ${finders} DESTINATION share/cmake/Modules ) 
