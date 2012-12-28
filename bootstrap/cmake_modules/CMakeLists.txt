
     set ( finders FindFLEX.cmake
                   FindBISON.cmake
                   FindPythonSitePackages.cmake
                   FindLEFDEF.cmake
                   FindBootstrap.cmake
                   FindLibexecinfo.cmake
                   FindQwt.cmake
         )
               
 install ( FILES ${finders} DESTINATION share/cmake/Modules ) 
