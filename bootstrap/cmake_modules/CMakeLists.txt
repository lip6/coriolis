
     set ( finders FindFLEX.cmake
                   FindBISON.cmake
                   FindPythonSitePackages.cmake
                   FindLEFDEF.cmake
                   FindBootstrap.cmake
                   FindLibexecinfo.cmake
                   FindQwt.cmake
                   GetGitRevisionDescription.cmake
                   GetGitRevisionDescription.cmake.in
         )
               
 install ( FILES ${finders} DESTINATION share/cmake/Modules ) 
