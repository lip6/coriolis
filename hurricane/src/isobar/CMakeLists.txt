
               include ( ${QT_USE_FILE} )

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/viewer
                         ${HURRICANE_SOURCE_DIR}/src/isobar
                         ${PYTHON_INCLUDE_PATH}
                       )
                   set ( sources    ProxyProperty.cpp
                                    PyBreakpoint.cpp
                                    PyBox.cpp
                                    PyCell.cpp
                                    PyCellCollection.cpp
                                    PyCellViewer.cpp
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
                                    PyNet.cpp
                                    PyNetCollection.cpp
                                    PyNetExternalComponents.cpp
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
                   set ( includes   hurricane/isobar/ProxyProperty.h
                                    hurricane/isobar/PyBreakpoint.h
                                    hurricane/isobar/PyBox.h
                                    hurricane/isobar/PyCell.h
                                    hurricane/isobar/PyCellCollection.h
                                    hurricane/isobar/PyCellViewer.h
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
                                    hurricane/isobar/PyNet.h
                                    hurricane/isobar/PyNetCollection.h
                                    hurricane/isobar/PyNetExternalComponents.h
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
# source2 & include2 for module that are *not* Python wrappers but true
# Hurricane modules.
                   set ( sources2   Script.cpp )
                   set ( includes2  hurricane/isobar/Script.h)


               install ( FILES      ${includes}
                                    ${includes2} DESTINATION include/coriolis2/hurricane/isobar )
           add_library ( isobar     ${sources} ${sources2} )
 target_link_libraries ( isobar     viewer )
           add_library ( Hurricane  MODULE ${sources} ) 
 target_link_libraries ( Hurricane  viewer )
 set_target_properties ( Hurricane  PROPERTIES
                                    COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                    PREFIX        ""
                       )
               install ( TARGETS    isobar      DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS    Hurricane   DESTINATION ${PYTHON_SITE_PACKAGES} )
