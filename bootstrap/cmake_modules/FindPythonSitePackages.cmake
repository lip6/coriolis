
if(UNIX)
 # This way avoids newline in the script string.
  set(SCRIPT            "import os.path,distutils.sysconfig")
  set(SCRIPT "${SCRIPT}; pathes = distutils.sysconfig.get_python_lib().split('/')")
  set(SCRIPT "${SCRIPT}; print os.path.join(pathes[-2],pathes[-1])")
 
  execute_process(COMMAND "python" "-c" "${SCRIPT}"
                  RESULT_VARIABLE RETURN_CODE
                  OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                 )
 
  if(RETURN_CODE EQUAL 0)
    set(FindPythonSitePackages_FOUND TRUE)
  else(RETURN_CODE EQUAL 0)
    set(FindPythonSitePackages_FOUND FALSE)
  endif(RETURN_CODE EQUAL 0)

  set(PYTHON_SITE_PACKAGES "lib${LIB_SUFFIX}/${PYTHON_SITE_PACKAGES}"
      CACHE STRING         "Python site packages directory." FORCE)
  mark_as_advanced(PYTHON_SITE_PACKAGES)

  if(FindPythonSitePackages_FOUND)
    if(NOT FindPythonSitePackages_FIND_QUIETLY)
      if(FindPythonSitePackages_FOUND)
        message(STATUS "Found FindPythonSitePackages : ${PYTHON_SITE_PACKAGES}")
      endif(FindPythonSitePackages_FOUND)
    endif(NOT FindPythonSitePackages_FIND_QUIETLY)
  else(FindPythonSitePackages_FOUND)
    message ( FATAL_ERROR "Python site packages directory was not found (pythonV.R/site-packages/)." )
  endif(FindPythonSitePackages_FOUND)
endif(UNIX)
