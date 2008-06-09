
   include_directories ( ${PYTHON_INCLUDE_PATH}
                         ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/isobar
                       )
                   set ( sources  ProxyProperty.cpp
                                  PyBox.cpp
                                  PyCell.cpp
                                  PyCellLocator.cpp
                                  PyComponent.cpp
                                  PyComponentLocator.cpp
                                  PyContact.cpp
                                  PyDataBase.cpp
                                  PyEntity.cpp
                                  PyHorizontal.cpp
                                  PyHurricane.cpp
                                  PyHyperNet.cpp
                                  PyInstance.cpp
                                  PyInstanceLocator.cpp
                                  PyLayer.cpp
                                  PyLibrary.cpp
                                  PyName.cpp
                                  PyNet.cpp
                                  PyNetLocator.cpp
                                  PyOccurrence.cpp
                                  PyOccurrenceLocator.cpp
                                  PyPath.cpp
                                  PyPin.cpp
                                  PyPinLocator.cpp
                                  PyPlug.cpp
                                  PyPlugLocator.cpp
                                  PyPoint.cpp
                                  PyReference.cpp
                                  PyReferenceLocator.cpp
                                  PySegment.cpp
                                  PySegmentLocator.cpp
                                  PyTechnology.cpp
                                  PyTransformation.cpp
                                  PyDbU.cpp
                                  PyUpdateSession.cpp
                                  PyVertical.cpp
                                  
                       )
                   set ( includes hurricane/isobar/ProxyProperty.h
                                  hurricane/isobar/PyBox.h
                                  hurricane/isobar/PyCell.h
                                  hurricane/isobar/PyCellLocator.h
                                  hurricane/isobar/PyComponent.h
                                  hurricane/isobar/PyComponentLocator.h
                                  hurricane/isobar/PyContact.h
                                  hurricane/isobar/PyDataBase.h
                                  hurricane/isobar/PyEntity.h
                                  hurricane/isobar/PyHorizontal.h
                                  hurricane/isobar/PyHurricane.h
                                  hurricane/isobar/PyHyperNet.h
                                  hurricane/isobar/PyInstance.h
                                  hurricane/isobar/PyInstanceLocator.h
                                  hurricane/isobar/PyLayer.h
                                  hurricane/isobar/PyLibrary.h
                                  hurricane/isobar/PyName.h
                                  hurricane/isobar/PyNet.h
                                  hurricane/isobar/PyNetLocator.h
                                  hurricane/isobar/PyOccurrence.h
                                  hurricane/isobar/PyOccurrenceLocator.h
                                  hurricane/isobar/PyPath.h
                                  hurricane/isobar/PyPin.h
                                  hurricane/isobar/PyPinLocator.h
                                  hurricane/isobar/PyPlug.h
                                  hurricane/isobar/PyPlugLocator.h
                                  hurricane/isobar/PyPoint.h
                                  hurricane/isobar/PyReference.h
                                  hurricane/isobar/PyReferenceLocator.h
                                  hurricane/isobar/PySegment.h
                                  hurricane/isobar/PySegmentLocator.h
                                  hurricane/isobar/PyTechnology.h
                                  hurricane/isobar/PyTransformation.h
                                  hurricane/isobar/PyDbU.h
                                  hurricane/isobar/PyUpdateSession.h
                                  hurricane/isobar/PyVertical.h
                       )

           add_library ( isobar SHARED ${sources} )
 target_link_libraries ( isobar hurricane ${PYTHON_LIBRARIES} )

           add_library ( Hurricane MODULE ${sources} ) 
 set_target_properties ( Hurricane PROPERTIES
                                   COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                   PREFIX        ""
                       )
 target_link_libraries ( Hurricane isobar hurricane ${PYTHON_LIBRARIES} )

               install ( TARGETS isobar      DESTINATION /lib )
               install ( TARGETS Hurricane   DESTINATION /lib/python )
               install ( FILES   ${includes} DESTINATION /include/hurricane/isobar )
