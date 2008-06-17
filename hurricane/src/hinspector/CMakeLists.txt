
               include ( ${QT_USE_FILE} )

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/hviewer
                         ${HURRICANE_SOURCE_DIR}/src/hinspector
                       )

                   set ( mocincludes hurricane/inspector/RecordModel.h
                                     hurricane/inspector/HInspectorWidget.h
                       )
                   set ( cpps        RecordModel.cpp
                                     HInspectorWidget.cpp
                       )

          qt4_wrap_cpp ( MOC_SRCS ${mocincludes} )

           add_library ( hinspector SHARED ${cpps} ${MOC_SRCS} )
 target_link_libraries ( hinspector ${QT_LIBRARIES} hurricane )

               install ( FILES   hurricane/inspector/HInspectorWidget.h
                                                    DESTINATION /include/hurricane/inspector ) 
               install ( TARGETS hinspector         DESTINATION /lib )
