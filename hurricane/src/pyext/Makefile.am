all-local: python_EXTENSION

lib_LTLIBRARIES           = libIsobar.la  

libIsobar_la_SOURCES      = ProxyProperty.cpp       \
                            PyHurricane.cpp         \
                            PyUpdateSession.cpp     \
                            PyDataBase.cpp          \
                            PyCDataBase.cpp         \
                            PyTechnology.cpp        \
                            PyLibrary.cpp           \
                            PyLayer.cpp             \
                            PyUnit.cpp              \
                            PyPoint.cpp             \
                            PyBox.cpp               \
                            PyTransformation.cpp    \
                            PyName.cpp              \
                            PyPath.cpp              \
                            PyOccurrence.cpp        \
                            PyHook.cpp              \
                            PyOccurrenceLocator.cpp \
                            PyEntity.cpp            \
                            PyCell.cpp              \
                            PyCellLocator.cpp       \
                            PyInstance.cpp          \
                            PyInstanceLocator.cpp   \
                            PyNet.cpp               \
                            PyNetLocator.cpp        \
                            PyReference.cpp         \
                            PyReferenceLocator.cpp  \
                            PyComponent.cpp         \
                            PyComponentLocator.cpp  \
                            PyContact.cpp           \
                            PyPin.cpp               \
                            PyPinLocator.cpp        \
                            PyPlug.cpp              \
                            PyPlugLocator.cpp       \
                            PySegment.cpp           \
                            PySegmentLocator.cpp    \
                            PyHorizontal.cpp        \
                            PyVertical.cpp          \
                            PyWidget.cpp            \
                            PyCEditor.cpp           \
                            PyCProps.cpp            \
                            PyCToolBox.cpp          \
                            PyCIOC.cpp              \
                            PyCEngine.cpp           \
		            PyHyperNet.cpp          \
                            PyHierarchy.cpp         \
                            PyCLiberty.cpp

libIsobar_la_LDFLAGS      = -version-info @ISOBAR_DLL_VERSION@

bin_SCRIPTS               = hurricaneconsole.py

pythonincludedir          = $(includedir)/coriolis/isobar
python_EXTENSION_lib      = Hurricane.so

# Headers are installed by "distutils".
noinst_HEADERS            = ProxyProperty.h       \
                            PyHurricane.h         \
                            PyUnit.h              \
                            PyPoint.h             \
                            PyHook.h              \
                            PyBox.h               \
                            PyTransformation.h    \
                            PyInstanceLocator.h   \
                            PyPlugLocator.h       \
                            PyNetLocator.h        \
                            PyName.h              \
                            PyPath.h              \
                            PyEntity.h            \
                            PyOccurrence.h        \
                            PyDataBase.h          \
                            PyCDataBase.h         \
                            PyTechnology.h        \
                            PyLibrary.h           \
                            PyLayer.h             \
                            PyComponent.h         \
                            PySegment.h           \
                            PyHorizontal.h        \
                            PyVertical.h          \
                            PyContact.h           \
                            PyPin.h               \
                            PyNet.h               \
                            PyPlug.h              \
                            PyReference.h         \
                            PyReferenceLocator.h  \
                            PyCell.h              \
                            PyCellLocator.h       \
                            PyInstance.h          \
                            PyCEditor.h           \
                            PyWidget.h            \
                            PyUpdateSession.h     \
                            PyCProps.h            \
                            PyCToolBox.h          \
                            PyCIOC.h              \
                            PyHierarchy.h         \
                            PyCEngine.h           \
                            PyPinLocator.h        \
                            PySegmentLocator.h    \
                            PyComponentLocator.h  \
							PyHyperNet.h          \
                            PyOccurrenceLocator.h \
                            PyCLiberty.h

EXTRA_DIST                = hurricaneconsole.py setup.py

CLEANFILES                =


# ==================================================================== 
# NOTHING SHOULD CHANGES BELOW THIS POINT.


 site_packages = $(shell python -c "from distutils import sysconfig; print sysconfig.get_python_lib(1,0,\"$(PYTHON_EXEC_PREFIX)\")")


python_EXTENSION: $(lib_LTLIBRARIES)
	(test ! -e ./setup.py 2>/dev/null && ln -s $(srcdir)/setup.py ./setup.py) || echo
	PYTHONPATH=$${PYTHONPATH}:$(site_packages); export PYTHONPATH;                  \
	if [ "`uname`" = "SunOS" ]; then LDSHARED="$(CC) -Wl,-G"; export LDSHARED; fi;  \
	LDFLAGS="${LDFLAGS} @DEVEL_LIBS@"; export LDFLAGS;                              \
	python setup.py build;

#	  LDFLAGS="`echo \"@DEVEL_LIBS@\" | sed 's+-L\([^ ]*\)+-L\1 -Wl,-R\1+g'`";      \
#	if [ ! -z "@DEVEL_LIBS@" ]; then                                                \
#	  LDFLAGS="`echo \"@DEVEL_LIBS@\" | sed 's+-L\([^ ]*\)+-L\1+g'`";               \
#	  export LDFLAGS;                                                               \
#	fi;                                                                             \
#

install-exec-hook:
	PYTHONPATH=$${PYTHONPATH}:$(site_packages); export PYTHONPATH;                 \
	if [ "`uname`" = "SunOS" ]; then LDSHARED="$(CC) -Wl,-G"; export LDSHARED; fi; \
	python setup.py install         --prefix      $(DESTDIR)$(prefix)              \
	                install_headers --install-dir $(DESTDIR)$(pythonincludedir)


clean-local:
	PYTHONPATH=$${PYTHONPATH}:$(site_packages); export PYTHONPATH;                 \
	python setup.py clean;                                                         \
	rm -rf build


uninstall-local:
	for header in $(python_EXTENSION_headers); do    \
	  rm -f $(pythonincludedir)/$$header;            \
	done;                                            \
	rm -f "$(site_packages)/$(python_EXTENSION_lib)"
