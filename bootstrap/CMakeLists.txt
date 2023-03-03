# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<bootstrap>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(Bootstrap)

 cmake_minimum_required(VERSION 3.16)

 set(ignoreVariables USE_LIBBFD "${BUILD_DOC} ${CMAKE_INSTALL_DIR}")

 add_subdirectory(cmake_modules)

 list(INSERT CMAKE_MODULE_PATH 0 "${Bootstrap_SOURCE_DIR}/cmake_modules/")
 find_package(Bootstrap          REQUIRED)
 # find_package(Python 3           REQUIRED COMPONENTS Interpreter Development )
 find_package(Python 3           REQUIRED COMPONENTS Interpreter Development )
 find_package(PythonSitePackages REQUIRED)
 print_cmake_module_path()

 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")

 install(FILES       coriolisEnv.py coriolis2.sh
         DESTINATION ${SYS_CONF_DIR}/coriolis2
         PERMISSIONS OWNER_WRITE
                     OWNER_READ    GROUP_READ    WORLD_READ
                     OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE) 

 install(DIRECTORY   builder
         DESTINATION ${Python_CORIOLISLIB} )
 
 install(FILES       ccb.py
         DESTINATION bin
         RENAME      ccb
         PERMISSIONS OWNER_WRITE
                     OWNER_READ    GROUP_READ    WORLD_READ
                     OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE) 

 install(FILES       crlenv.py
         DESTINATION bin
         RENAME      crlenv
         PERMISSIONS OWNER_WRITE
                     OWNER_READ    GROUP_READ    WORLD_READ
                     OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE) 
