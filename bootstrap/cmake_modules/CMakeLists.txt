
     set ( finders FindFLEX.cmake
                   FindBISON.cmake
                   FindPythonSitePackages.cmake
                   FindLEFDEF.cmake
                   FindBootstrap.cmake
                   FindLibexecinfo.cmake
                   FindLibbfd.cmake
                   FindSphinx.cmake
                   FindPelican.cmake
                   FindCOLOQUINTE.cmake
                   GetGitRevisionDescription.cmake
                   GetGitRevisionDescription.cmake.in
         )
               
 install ( FILES ${finders} DESTINATION share/cmake/Modules ) 
