# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<flute>" -*-

 set( CMAKE_LEGACY_CYGWIN_WIN32 0 )
 project( FLUTE )

 cmake_minimum_required( VERSION 2.8.9 )

 set( ignoreVariables "${BUILD_DOC} ${CMAKE_INSTALL_DIR}" )

 list( INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/" )
 find_package( Bootstrap REQUIRED )
 setup_project_paths( CORIOLIS )
 print_cmake_module_path()

 set_cmake_policies()
 check_distribution()
 setup_sysconfdir( "${CMAKE_INSTALL_PREFIX}" )

 add_subdirectory( src )
 add_subdirectory( cmake_modules )
