if(UNIX AND NOT POETRY)
  if(NOT Python_FOUND)
    message(FATAL_ERROR "Python has not been found, maybe forgot to call find_package(Python). ")
  endif()
 # This way avoids newline in the script string.
  set(SCRIPT            "import os.path,distutils.sysconfig")
  set(SCRIPT "${SCRIPT}; pathes = distutils.sysconfig.get_python_lib('platstdlib').split('/')")
  set(SCRIPT "${SCRIPT}; print( os.path.join(pathes[-2],pathes[-1]) )")
 
  execute_process(COMMAND "${Python_EXECUTABLE}" "-c" "${SCRIPT}"
                  RESULT_VARIABLE RETURN_CODE
                  OUTPUT_VARIABLE Python_SITEARCH
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                 )

  set(SCRIPT            "import os.path,distutils.sysconfig")
  set(SCRIPT "${SCRIPT}; pathes = distutils.sysconfig.get_python_lib('stdlib').split('/')")
  set(SCRIPT "${SCRIPT}; print( os.path.join(pathes[-2],pathes[-1]) )")
 
  execute_process(COMMAND "${Python_EXECUTABLE}" "-c" "${SCRIPT}"
                  RESULT_VARIABLE RETURN_CODE
                  OUTPUT_VARIABLE Python_SITELIB
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                 )
 
  if(RETURN_CODE EQUAL 0)
    set(FindPythonSitePackages_FOUND TRUE)
  else(RETURN_CODE EQUAL 0)
    set(FindPythonSitePackages_FOUND FALSE)
  endif(RETURN_CODE EQUAL 0)

  set(Python_CORIOLISARCH "lib${LIB_SUFFIX}/${Python_SITEARCH}/coriolis"
      CACHE STRING        "Python platform dependent install directory (Coriolis modules)." FORCE)
  set(Python_SITELIB      "lib${LIB_SUFFIX}/${Python_SITELIB}"
      CACHE STRING        "Python platform independent install directory." FORCE)
  set(Python_CORIOLISLIB  "${Python_SITELIB}/coriolis"
      CACHE STRING        "Python platform independent install directory (Coriolis modules)." FORCE)
  mark_as_advanced(Python_CORIOLISARCH)
  mark_as_advanced(Python_CORIOLISLIB)
  mark_as_advanced(Python_SITELIB)
  
  set(PYTHON_SITE_PACKAGES "${Python_SITELIB}")

  if(FindPythonSitePackages_FOUND)
    if(NOT FindPythonSitePackages_FIND_QUIETLY)
      if(FindPythonSitePackages_FOUND)
        message(STATUS "Found FindPythonSitePackages : ${Python_CORIOLISARCH}")
      endif(FindPythonSitePackages_FOUND)
    endif(NOT FindPythonSitePackages_FIND_QUIETLY)
  else(FindPythonSitePackages_FOUND)
    message ( FATAL_ERROR "Python site packages directory was not found (pythonV.R/site-packages/)." )
  endif(FindPythonSitePackages_FOUND)
endif(UNIX AND NOT POETRY)
