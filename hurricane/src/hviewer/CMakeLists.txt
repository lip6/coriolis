
               include ( ${QT_USE_FILE} )

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/hviewer )

                   set ( mocincludes hurricane/viewer/PaletteEntry.h
                                     hurricane/viewer/LayerPaletteEntry.h
                                     hurricane/viewer/GroupPaletteEntry.h
                                     hurricane/viewer/ViewerPaletteEntry.h
                                     hurricane/viewer/Palette.h
                                     hurricane/viewer/CellWidget.h
                                     hurricane/viewer/CellViewer.h
                                     hurricane/viewer/RecordModel.h
                                     hurricane/viewer/HInspectorWidget.h
                       )
                   set ( exports     hurricane/viewer/ScreenUtilities.h
                                     hurricane/viewer/PaletteEntry.h
                                     hurricane/viewer/Palette.h
                                     hurricane/viewer/DisplayStyle.h
                                     hurricane/viewer/Graphics.h
                                     hurricane/viewer/CellViewer.h
                                     hurricane/viewer/HInspectorWidget.h
                       )
                   set ( cpps        ScreenUtilities.cpp 
                                     DisplayStyle.cpp 
                                     Graphics.cpp
                                     PaletteEntry.cpp
                                     LayerPaletteEntry.cpp
                                     GroupPaletteEntry.cpp
                                     ViewerPaletteEntry.cpp
                                     Palette.cpp
                                     CellWidget.cpp
                                     CellViewer.cpp
                                     RecordModel.cpp
                                     HInspectorWidget.cpp
                       )

          qt4_wrap_cpp ( MOC_SRCS    ${mocincludes} )
     qt4_add_resources ( RCC_SRCS    CellViewer.qrc )

           add_library ( hviewer     SHARED ${cpps} ${MOC_SRCS} ${RCC_SRCS} )
 target_link_libraries ( hviewer     ${QT_LIBRARIES} hurricane )

               install ( FILES       ${exports} DESTINATION /include/hurricane/viewer ) 
               install ( TARGETS     hviewer    DESTINATION /lib )
