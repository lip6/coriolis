
   include_directories ( ${PYTHON_INCLUDE_PATH}
                         ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/isobar
                       )
                   set ( sources  ProxyProperty.cpp
                                  PyBox.cpp
                                  PyCell.cpp
                                  PyCellCollection.cpp
                                  PyComponent.cpp
                                  PyComponentCollection.cpp
                                  PyContact.cpp
                                  PyDataBase.cpp
                                  PyEntity.cpp
                                  PyHorizontal.cpp
                                  PyHurricane.cpp
                                  PyHyperNet.cpp
                                  PyInstance.cpp
                                  PyInstanceCollection.cpp
                                  PyLayer.cpp
                                  PyLibrary.cpp
                                  PyName.cpp
                                  PyNet.cpp
                                  PyNetCollection.cpp
                                  PyOccurrence.cpp
                                  PyOccurrenceCollection.cpp
                                  PyPad.cpp
                                  PyPath.cpp
                                  PyPin.cpp
                                  PyPinCollection.cpp
                                  PyPlug.cpp
                                  PyPlugCollection.cpp
                                  PyPoint.cpp
                                  PyReference.cpp
                                  PyReferenceCollection.cpp
                                  PySegment.cpp
                                  PySegmentCollection.cpp
                                  PyTechnology.cpp
                                  PyTransformation.cpp
                                  PyDbU.cpp
                                  PyUpdateSession.cpp
                                  PyVertical.cpp
                                  
                       )
                   set ( includes hurricane/isobar/ProxyProperty.h
                                  hurricane/isobar/PyBox.h
                                  hurricane/isobar/PyCell.h
                                  hurricane/isobar/PyCellCollection.h
                                  hurricane/isobar/PyComponent.h
                                  hurricane/isobar/PyComponentCollection.h
                                  hurricane/isobar/PyContact.h
                                  hurricane/isobar/PyDataBase.h
                                  hurricane/isobar/PyEntity.h
                                  hurricane/isobar/PyHorizontal.h
                                  hurricane/isobar/PyHurricane.h
                                  hurricane/isobar/PyHyperNet.h
                                  hurricane/isobar/PyInstance.h
                                  hurricane/isobar/PyInstanceCollection.h
                                  hurricane/isobar/PyLayer.h
                                  hurricane/isobar/PyLibrary.h
                                  hurricane/isobar/PyName.h
                                  hurricane/isobar/PyNet.h
                                  hurricane/isobar/PyNetCollection.h
                                  hurricane/isobar/PyOccurrence.h
                                  hurricane/isobar/PyOccurrenceCollection.h
                                  hurricane/isobar/PyPad.h
                                  hurricane/isobar/PyPath.h
                                  hurricane/isobar/PyPin.h
                                  hurricane/isobar/PyPinCollection.h
                                  hurricane/isobar/PyPlug.h
                                  hurricane/isobar/PyPlugCollection.h
                                  hurricane/isobar/PyPoint.h
                                  hurricane/isobar/PyReference.h
                                  hurricane/isobar/PyReferenceCollection.h
                                  hurricane/isobar/PySegment.h
                                  hurricane/isobar/PySegmentCollection.h
                                  hurricane/isobar/PyTechnology.h
                                  hurricane/isobar/PyTransformation.h
                                  hurricane/isobar/PyDbU.h
                                  hurricane/isobar/PyUpdateSession.h
                                  hurricane/isobar/PyVertical.h
                       )


               install ( FILES   ${includes} DESTINATION /include/hurricane/isobar )

 if ( BUILD_STATIC )
           add_library ( isobar-static STATIC ${sources} )
 target_link_libraries ( isobar-static hurricane-static ${PYTHON_LIBRARIES} )

           add_library ( Hurricane-static MODULE ${sources} ) 
 set_target_properties ( Hurricane-static PROPERTIES
                                          COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                          PREFIX        ""
                       )
 target_link_libraries ( Hurricane-static isobar-static hurricane-static ${PYTHON_LIBRARIES} )

               install ( TARGETS isobar-static      DESTINATION /lib )
               install ( TARGETS Hurricane-static   DESTINATION /lib/python )
 else ( BUILD_STATIC )
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
 endif ( BUILD_STATIC )
