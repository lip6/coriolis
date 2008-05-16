
               include ( ${QT_USE_FILE} )

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane )

                   set ( includes    ScreenUtilities.h
                                     DisplayStyle.h
                                     Graphics.h
                       )
                   set ( mocincludes PaletteEntry.h
                                     LayerPaletteEntry.h
                                     GroupPaletteEntry.h
                                     ViewerPaletteEntry.h
                                     Palette.h
                                     CellWidget.h
                                     CellViewer.h
                       )
                   set ( exports     ScreenUtilities.h
                                     PaletteEntry.h
                                     Palette.h
                                     DisplayStyle.h
                                     Graphics.h
                                     CellViewer.h
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
                       )

          qt4_wrap_cpp ( MOC_SRCS    ${mocincludes} )
     qt4_add_resources ( RCC_SRCS    CellViewer.qrc )

           add_library ( hviewer     SHARED ${cpps} ${MOC_SRCS} ${RCC_SRCS} )
 target_link_libraries ( hviewer     ${QT_LIBRARIES} hurricane )

               install ( FILES       ${exports} DESTINATION /include/hurricane ) 
               install ( TARGETS     hviewer    DESTINATION /lib )
