
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
                                    PyMaterial.cpp
                                    PyLayer.cpp
                                    PyLayerMask.cpp
                                    PyBasicLayer.cpp
                                    PyRegularLayer.cpp
                                    PyContactLayer.cpp
                                    PyDiffusionLayer.cpp
                                    PyTransistorLayer.cpp
                                    PyViaLayer.cpp
                                    PyLayerCollection.cpp
                                    PyBasicLayerCollection.cpp
                                    PyRegularLayerCollection.cpp
                                    PyViaLayerCollection.cpp
                                    PyLibrary.cpp
                                    PyNet.cpp
                                    PyNetCollection.cpp
                                    PyNetExternalComponents.cpp
                                    PyOccurrence.cpp
                                    PyOccurrenceCollection.cpp
                                    PyHook.cpp
                                    PyPad.cpp
                                    PyPath.cpp
                                    PyPin.cpp
                                    PyPinCollection.cpp
                                    PyPlug.cpp
                                    PyPlugCollection.cpp
                                    PyPoint.cpp
                                    PyReference.cpp
                                    PyReferenceCollection.cpp
                                    PyRoutingPad.cpp
                                    PySegment.cpp
                                    PySegmentCollection.cpp
                                    PyTechnology.cpp
                                    PyTransformation.cpp
                                    PyDbU.cpp
                                    PyUpdateSession.cpp
                                    PyVertical.cpp
                                    PyQueryMask.cpp
                                    PyQuery.cpp
                       )	    
                   set ( includes   hurricane/isobar/ProxyProperty.h
                                    hurricane/isobar/PyBreakpoint.h
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
                                    hurricane/isobar/PyMaterial.h
                                    hurricane/isobar/PyLayerMask.h
                                    hurricane/isobar/PyLayer.h
                                    hurricane/isobar/PyBasicLayer.h
                                    hurricane/isobar/PyRegularLayer.h
                                    hurricane/isobar/PyContactLayer.h
                                    hurricane/isobar/PyDiffusionLayer.h
                                    hurricane/isobar/PyTransistorLayer.h
                                    hurricane/isobar/PyLayerCollection.h
                                    hurricane/isobar/PyBasicLayerCollection.h
                                    hurricane/isobar/PyRegularLayerCollection.h
                                    hurricane/isobar/PyViaLayerCollection.h
                                    hurricane/isobar/PyViaLayer.h
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
                                    hurricane/isobar/PyRoutingPad.h
                                    hurricane/isobar/PySegment.h
                                    hurricane/isobar/PySegmentCollection.h
                                    hurricane/isobar/PyTechnology.h
                                    hurricane/isobar/PyTransformation.h
                                    hurricane/isobar/PyDbU.h
                                    hurricane/isobar/PyUpdateSession.h
                                    hurricane/isobar/PyVertical.h
                                    hurricane/isobar/PyQueryMask.h
                                    hurricane/isobar/PyQuery.h
                       )


           add_library ( isobar     ${sources} )
 set_target_properties ( isobar     PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( isobar     hurricane ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
           add_library ( Hurricane  MODULE ${sources} ) 
 set_target_properties ( Hurricane  PROPERTIES
                                    COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                    PREFIX        ""
                       )
 target_link_libraries ( Hurricane  isobar )
               install ( TARGETS    isobar      DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS    Hurricane   DESTINATION ${PYTHON_SITE_PACKAGES} )
               install ( FILES      ${includes} DESTINATION include/coriolis2/hurricane/isobar )
