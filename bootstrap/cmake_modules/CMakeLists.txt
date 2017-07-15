
     set ( finders FindFLEX.cmake
                   FindBISON.cmake
                   FindPythonSitePackages.cmake
                   FindLEFDEF.cmake
                   FindBootstrap.cmake
                   FindLibexecinfo.cmake
                   FindLibbfd.cmake
                   FindQwt.cmake
                   FindSphinx.cmake
                   GetGitRevisionDescription.cmake
                   GetGitRevisionDescription.cmake.in
         )
               
 install ( FILES ${finders} DESTINATION share/cmake/Modules ) 
