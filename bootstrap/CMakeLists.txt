# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<bootstrap>" -*-

 project(Bootstrap)

 cmake_minimum_required(VERSION 2.4.0)

 add_subdirectory(cmake_modules)

 list(INSERT CMAKE_MODULE_PATH 0 "${Bootstrap_SOURCE_DIR}/cmake_modules/")
 find_package(Bootstrap REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 print_cmake_module_path()

 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")

 install(FILES       coriolisEnv.py coriolis2.sh
         DESTINATION ${SYS_CONF_DIR}/coriolis2
         PERMISSIONS OWNER_WRITE
                     OWNER_READ    GROUP_READ    WORLD_READ
                     OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE) 

 install(DIRECTORY   builder
         DESTINATION ${PYTHON_SITE_PACKAGES} )
 
 install(FILES       ccb.py
         DESTINATION bin
         RENAME      ccb
         PERMISSIONS OWNER_WRITE
                     OWNER_READ    GROUP_READ    WORLD_READ
                     OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE) 
