
if ( UNIX )

  execute_process(COMMAND echo "import os.path,distutils.sysconfig;pathes = distutils.sysconfig.get_python_lib().split('/');print os.path.join ( pathes[-2], pathes[-1] )"
   		 OUTPUT_FILE /tmp/site.py)
  execute_process ( COMMAND python /tmp/site.py
                 OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE
                 RESULT_VARIABLE RCODE)
  execute_process ( COMMAND rm -f /tmp/site.py)

  if ( RCODE EQUAL 0 )
    set ( FindPythonSitePackages_FOUND TRUE )
  else ( RCODE EQUAL 0 )
    set ( FindPythonSitePackages_FOUND FALSE )
  endif ( RCODE EQUAL 0 )

  set ( PYTHON_SITE_PACKAGES "lib${LIB_SUFFIX}/${PYTHON_SITE_PACKAGES}"
        CACHE STRING "Python site packages directory." FORCE )
  mark_as_advanced ( PYTHON_SITE_PACKAGES )

  if ( FindPythonSitePackages_FOUND )
    if ( NOT FindPythonSitePackages_FIND_QUIETLY )
      if ( FindPythonSitePackages_FOUND )
        message ( STATUS "Found FindPythonSitePackages : ${PYTHON_SITE_PACKAGES}" )
      endif ( FindPythonSitePackages_FOUND )
    endif ( NOT FindPythonSitePackages_FIND_QUIETLY )
  else ( FindPythonSitePackages_FOUND )
   #if ( FindPythonSitePackages_FIND_REQUIRED )
      message ( FATAL_ERROR "Python site packages directory was not found (pythonV.R/site-packages/)." )
   #endif ( FindPythonSitePackages_FIND_REQUIRED )
  endif ( FindPythonSitePackages_FOUND )

endif ( UNIX )
