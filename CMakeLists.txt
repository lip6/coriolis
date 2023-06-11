cmake_minimum_required(VERSION 3.16)

project(coriolis)

if(NOT CMAKE_BUILD_PARALLEL_LEVEL)
    cmake_host_system_information(RESULT Ncpu QUERY NUMBER_OF_LOGICAL_CORES)
    math(EXPR CMAKE_BUILD_PARALLEL_LEVEL "${Ncpu}-1")
endif()

set(CORIOLIS_TMP_INSTALL_DIR  ${CMAKE_BINARY_DIR}/install)

set(ENV{CORIOLIS_TOP} ${CORIOLIS_TMP_INSTALL_DIR})

function (build_coriolis_module target)
    message("---------- Start building ${target} ----------")
    set(build_dir ${CMAKE_BINARY_DIR}/build_${target})
    file(MAKE_DIRECTORY ${build_dir} ${build_dir}/build)
    file(WRITE ${build_dir}/CMakeLists.txt "
cmake_minimum_required(VERSION ${CMAKE_VERSION})

project(build_${target})

find_package(Python 3 REQUIRED COMPONENTS Interpreter Development.Module)

set(POETRY TRUE) # this will prevent looking for python site-packages directory
                 # and next vars will not be overwritten
set(Python_CORIOLISARCH coriolis) # relative to install dir
set(Python_CORIOLISLIB coriolis)  # relative to install dir
set(PYTHON_SITE_PACKAGES ..) # python packages directory (relative to coriolis python dir)

if(NOT Python_LIBRARIES)
    set(Python_LIBRARIES \"-Wl,--unresolved-symbols=ignore-all\")
endif()

set(CXX_STANDARD \"-fPIC\")

if(NOT Python3_LIBRARIES)
    set(Python3_LIBRARIES \"\${Python_LIBRARIES}\")
endif()

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap/cmake_modules)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${target} ${build_dir})
    ")

    execute_process(
        COMMAND ${CMAKE_COMMAND} ${build_dir}
                                -D CMAKE_BUILD_TYPE:STRING=Release
                                -D CMAKE_INSTALL_PREFIX:PATH=${CORIOLIS_TMP_INSTALL_DIR}
                                -D CMAKE_BUILD_RPATH_USE_ORIGIN:BOOL=TRUE
                                -D CMAKE_SKIP_BUILD_RPATH:BOOL=FALSE
                                -D CMAKE_BUILD_WITH_INSTALL_RPATH:BOOL=FALSE
                                -D CMAKE_INSTALL_RPATH:STRING='$ORIGIN/libs:$ORIGIN'
                                -D CMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL=TRUE
                                -G "${CMAKE_GENERATOR}"
        WORKING_DIRECTORY ${build_dir}/build
        RESULT_VARIABLE ${target}_setup_result
        )
    if(NOT ${${target}_setup_result} EQUAL 0)
        message(FATAL_ERROR "Failed to setup ${target} build\n")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
        WORKING_DIRECTORY ${build_dir}/build
        RESULT_VARIABLE ${target}_build_result
        )
    if(NOT ${${target}_build_result} EQUAL 0)
        message(FATAL_ERROR "${target} build failed\n")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --install . --prefix "${CORIOLIS_TMP_INSTALL_DIR}"
        WORKING_DIRECTORY ${build_dir}/build
        RESULT_VARIABLE ${target}_install_result
        )       
    if(NOT ${${target}_install_result} EQUAL 0)
        message(FATAL_ERROR "Failed to installe ${target} into build environment\n")
    endif()
endfunction()

build_coriolis_module(lefdef)
build_coriolis_module(coloquinte)
build_coriolis_module(hurricane)
build_coriolis_module(crlcore)
build_coriolis_module(flute)
build_coriolis_module(etesian)
build_coriolis_module(anabatic)
build_coriolis_module(katana)
build_coriolis_module(equinox)
build_coriolis_module(solstice)
build_coriolis_module(oroshi)
build_coriolis_module(bora)
build_coriolis_module(karakaze)
#build_coriolis_module(knik)
build_coriolis_module(unicorn)
build_coriolis_module(tutorial)
build_coriolis_module(cumulus)
build_coriolis_module(stratus1)
##add_subdirectory(documentation)
#add_subdirectory(unittests)

file(GLOB SHARED_LIB_FILES LIST_DIRECTORIES false "${CORIOLIS_TMP_INSTALL_DIR}/lib/*.*")

install(FILES ${SHARED_LIB_FILES} DESTINATION coriolis/libs)

function(coriolis_install_folder dstdir srcdir)
    file(GLOB_RECURSE files RELATIVE "${srcdir}" "${srcdir}/*.*")
    foreach(name ${files})
        if(NOT IS_DIRECTORY "${srcdir}/${name}")
            get_filename_component(subdir ${name} DIRECTORY)
            install(FILES "${srcdir}/${name}" DESTINATION ${dstdir}/${subdir})
        endif()
    endforeach()
endfunction()

coriolis_install_folder(coriolis/share/flute ${CORIOLIS_TMP_INSTALL_DIR}/share/flute)
coriolis_install_folder(coriolis ${CORIOLIS_TMP_INSTALL_DIR}/coriolis)

set(CORIOLIS_EXTRAS_DIR ${CMAKE_BINARY_DIR}/extras)
file(MAKE_DIRECTORY ${CORIOLIS_EXTRAS_DIR})

# Put all extra python things into ${CORIOLIS_EXTRAS_DIR} directory

coriolis_install_folder(coriolis ${CORIOLIS_EXTRAS_DIR})

install(FILES "${CORIOLIS_TMP_INSTALL_DIR}/bin/cgt" DESTINATION coriolis RENAME __main__.py)
