cmake_minimum_required(VERSION 3.16)

project(coriolis)

if(NOT CMAKE_BUILD_PARALLEL_LEVEL)
    cmake_host_system_information(RESULT Ncpu QUERY NUMBER_OF_LOGICAL_CORES)
    math(EXPR CMAKE_BUILD_PARALLEL_LEVEL "${Ncpu}-1")
endif()

set(CORIOLIS_TMP_INSTALL_DIR  ${CMAKE_BINARY_DIR}/install)

set(ENV{CORIOLIS_TOP} ${CORIOLIS_TMP_INSTALL_DIR})

function (build_coriolis_module target)
    set(build_dir ${CMAKE_BINARY_DIR}/build_${target})
    file(MAKE_DIRECTORY ${build_dir} ${build_dir}/build)
    file(WRITE ${build_dir}/CMakeLists.txt "
cmake_minimum_required(VERSION 3.16)

project(build_${target})

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap/cmake_modules)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${target} ${build_dir})
    ")

    execute_process(COMMAND ${CMAKE_COMMAND} ${build_dir}
                                -D CMAKE_BUILD_TYPE=Release
                                -D CMAKE_INSTALL_PREFIX:PATH=${CORIOLIS_TMP_INSTALL_DIR}
                                -D "PYTHON_INCLUDE_DIR=\$(python -c \"from distutils.sysconfig import get_python_inc\; print(get_python_inc())\")"
                                -D "PYTHON_LIBRARY=\$(python -c \"import distutils.sysconfig as sysconfig\; print(sysconfig.get_config_var('LIBDIR'))\")"
                                -D PYTHON_EXECUTABLE:FILEPATH=`which python`
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

file(GLOB CORIOLIS_PACKAGE_SRCDIR "${CORIOLIS_TMP_INSTALL_DIR}/lib/*/*/coriolis")

function(coriolis_install_all)
    foreach(srcdir ${ARGN})
        file(GLOB_RECURSE CORIOLIS_PACKAGE_FILES RELATIVE "${srcdir}" "${srcdir}/*.*")
        foreach(name ${CORIOLIS_PACKAGE_FILES})
            if(NOT IS_DIRECTORY "${srcdir}/${name}")
                get_filename_component(dstdir ${name} DIRECTORY)
                install(FILES "${srcdir}/${name}" DESTINATION coriolis/${dstdir})
            endif()
        endforeach()
    endforeach()
endfunction()

set(CORIOLIS_EXTRAS_DIR ${CMAKE_BINARY_DIR}/extras)
file(MAKE_DIRECTORY ${CORIOLIS_EXTRAS_DIR})

file(WRITE ${CORIOLIS_EXTRAS_DIR}/__init__.py
"import .libs
"
)
file(READ ${CORIOLIS_PACKAGE_SRCDIR}/__init__.py CORIOLIS_INIT_PY)
file(APPEND ${CORIOLIS_EXTRAS_DIR}/__init__.py "${CORIOLIS_INIT_PY}")

file(WRITE ${CORIOLIS_EXTRAS_DIR}/libs/__init__.py
"import os, os.path, ctypes

__libs=[]
def _preload_shared_libraries(dirname):
    libs = []
    for name in os.listdir(dirname):
        if name.endswith('.so'):
            libs.append(os.path.join(dirname,name))
    nload = len(libs)
    while nload > 0:
        name=libs.pop(0)
        try:
            lib=ctypes.CDLL(name)
            __libs.append(lib)
            nload = len(libs)
        except OSError:
            libs.append(name)
            nload -= 1

_preload_shared_libraries(os.path.dirname(os.path.realpath(__file__)))
"
)

coriolis_install_all(${CORIOLIS_PACKAGE_SRCDIR} ${CORIOLIS_EXTRAS_DIR})
