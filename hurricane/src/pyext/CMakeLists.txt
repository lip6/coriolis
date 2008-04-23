INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH} ${HURRICANE_SOURCE_DIR}/src/hurricane)

SET(sources ProxyProperty.cpp PyBox.cpp PyCell.cpp PyCellLocator.cpp
    PyComponent.cpp PyComponentLocator.cpp PyContact.cpp
    PyDataBase.cpp PyEntity.cpp PyHorizontal.cpp
    PyHurricane.cpp PyHyperNet.cpp PyInstance.cpp PyInstanceLocator.cpp
    PyLayer.cpp PyLibrary.cpp PyName.cpp PyNet.cpp PyNetLocator.cpp
    PyOccurrence.cpp PyOccurrenceLocator.cpp PyPath.cpp PyPin.cpp PyPinLocator.cpp
    PyPlug.cpp PyPlugLocator.cpp PyPoint.cpp PyReference.cpp PyReferenceLocator.cpp
    PySegment.cpp PySegmentLocator.cpp PyTechnology.cpp PyTransformation.cpp
    PyUnit.cpp PyUpdateSession.cpp PyVertical.cpp 
)


SET(includes ProxyProperty.h PyBox.h PyCell.h PyCellLocator.h
    PyComponent.h PyComponentLocator.h PyContact.h
    PyDataBase.h PyEntity.h PyHorizontal.h
    PyHurricane.h PyHyperNet.h PyInstance.h PyInstanceLocator.h
    PyLayer.h PyLibrary.h PyName.h PyNet.h PyNetLocator.h
    PyOccurrence.h PyOccurrenceLocator.h PyPath.h PyPin.h PyPinLocator.h
    PyPlug.h PyPlugLocator.h PyPoint.h PyReference.h PyReferenceLocator.h
    PySegment.h PySegmentLocator.h PyTechnology.h PyTransformation.h
    PyUnit.h PyUpdateSession.h PyVertical.h 
)

ADD_LIBRARY(isobar SHARED ${sources})
TARGET_LINK_LIBRARIES(isobar hurricane ${PYTHON_LIBRARIES})

ADD_LIBRARY(Hurricane MODULE ${sources}) 
SET_TARGET_PROPERTIES(Hurricane
        PROPERTIES
        COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
        PREFIX ""
)
TARGET_LINK_LIBRARIES(Hurricane isobar hurricane ${PYTHON_LIBRARIES})

INSTALL(TARGETS isobar DESTINATION /lib)
INSTALL(TARGETS Hurricane DESTINATION /lib/python)
