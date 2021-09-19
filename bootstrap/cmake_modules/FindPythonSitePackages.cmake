
if(UNIX)
  if(NOT Python_FOUND)
    message(FATAL_ERROR "Python has not been found, maybe forgot to call find_package(Python). ")
  endif()
 # This way avoids newline in the script string.
  set(SCRIPT            "import os.path,distutils.sysconfig")
  set(SCRIPT "${SCRIPT}; pathes = distutils.sysconfig.get_python_lib('platstdlib').split('/')")
  set(SCRIPT "${SCRIPT}; print( os.path.join(pathes[-2],pathes[-1]) )")
 
  execute_process(COMMAND "${Python_EXECUTABLE}" "-c" "${SCRIPT}"
                  RESULT_VARIABLE RETURN_CODE
                  OUTPUT_VARIABLE Python_CORIOLISARCH
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                 )

  set(SCRIPT            "import os.path,distutils.sysconfig")
  set(SCRIPT "${SCRIPT}; pathes = distutils.sysconfig.get_python_lib('stdlib').split('/')")
  set(SCRIPT "${SCRIPT}; print( os.path.join(pathes[-2],pathes[-1]) )")
 
  execute_process(COMMAND "${Python_EXECUTABLE}" "-c" "${SCRIPT}"
                  RESULT_VARIABLE RETURN_CODE
                  OUTPUT_VARIABLE Python_CORIOLISLIB
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                 )
 
  if(RETURN_CODE EQUAL 0)
    set(FindPythonSitePackages_FOUND TRUE)
  else(RETURN_CODE EQUAL 0)
    set(FindPythonSitePackages_FOUND FALSE)
  endif(RETURN_CODE EQUAL 0)

  set(Python_CORIOLISARCH "lib${LIB_SUFFIX}/${Python_CORIOLISARCH}"
      CACHE STRING         "Python platform dependent install directory." FORCE)
  set(Python_CORIOLISLIB  "lib${LIB_SUFFIX}/${Python_CORIOLISLIB}"
      CACHE STRING         "Python platform independent install directory." FORCE)
  mark_as_advanced(Python_CORIOLISARCH)
  mark_as_advanced(Python_CORIOLISLIB)

  if(FindPythonSitePackages_FOUND)
    if(NOT FindPythonSitePackages_FIND_QUIETLY)
      if(FindPythonSitePackages_FOUND)
        message(STATUS "Found FindPythonSitePackages : ${Python_CORIOLISARCH}")
      endif(FindPythonSitePackages_FOUND)
    endif(NOT FindPythonSitePackages_FIND_QUIETLY)
  else(FindPythonSitePackages_FOUND)
    message ( FATAL_ERROR "Python site packages directory was not found (pythonV.R/site-packages/)." )
  endif(FindPythonSitePackages_FOUND)
endif(UNIX)
