
               include ( ${QT_USE_FILE} )

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/hviewer )

                   set ( mocincludes hurricane/viewer/HPaletteEntry.h
                                     hurricane/viewer/LayerPaletteEntry.h
                                     hurricane/viewer/GroupPaletteEntry.h
                                     hurricane/viewer/ViewerPaletteEntry.h
                                     hurricane/viewer/HPalette.h
                                     hurricane/viewer/HGraphics.h
                                     hurricane/viewer/DynamicLabel.h
                                     hurricane/viewer/HMousePosition.h
                                     hurricane/viewer/Selector.h
                                     hurricane/viewer/SelectCommand.h
                                     hurricane/viewer/CellWidget.h
                                     hurricane/viewer/CellWidgets.h
                                     hurricane/viewer/CellViewer.h
                                     hurricane/viewer/RecordModel.h
                                     hurricane/viewer/HInspectorWidget.h
                                     hurricane/viewer/HSelectionModel.h
                                     hurricane/viewer/HSelection.h
                                     hurricane/viewer/HNetlistModel.h
                                     hurricane/viewer/HNetlist.h
                                     hurricane/viewer/HDisplayFilter.h
                       )
                   set ( exports     hurricane/viewer/ScreenUtilities.h
                                     hurricane/viewer/HPaletteEntry.h
                                     hurricane/viewer/HPalette.h
                                     hurricane/viewer/DisplayStyle.h
                                     hurricane/viewer/Graphics.h
                                     hurricane/viewer/HGraphics.h
                                     hurricane/viewer/Selector.h
                                     hurricane/viewer/Command.h
                                     hurricane/viewer/AreaCommand.h
                                     hurricane/viewer/MoveCommand.h
                                     hurricane/viewer/ZoomCommand.h
                                     hurricane/viewer/SelectCommand.h
                                     hurricane/viewer/CellWidget.h
                                     hurricane/viewer/CellWidgets.h
                                     hurricane/viewer/CellViewer.h
                                     hurricane/viewer/HInspectorWidget.h
                                     hurricane/viewer/HSelectionModel.h
                                     hurricane/viewer/HSelection.h
                                     hurricane/viewer/NetInformations.h
                                     hurricane/viewer/HNetlist.h
                                     hurricane/viewer/HDisplayFilter.h
                       )
                   set ( cpps        ScreenUtilities.cpp 
                                     DisplayStyle.cpp 
                                     Graphics.cpp
                                     HGraphics.cpp
                                     HPaletteEntry.cpp
                                     LayerPaletteEntry.cpp
                                     GroupPaletteEntry.cpp
                                     ViewerPaletteEntry.cpp
                                     HPalette.cpp
                                     DynamicLabel.cpp
                                     HMousePosition.cpp
                                     Selector.cpp
                                     Command.cpp
                                     AreaCommand.cpp
                                     MoveCommand.cpp
                                     ZoomCommand.cpp
                                     SelectCommand.cpp
                                     CellWidget.cpp
                                     CellViewer.cpp
                                     RecordModel.cpp
                                     HInspectorWidget.cpp
                                     HSelectionModel.cpp
                                     HSelection.cpp
                                     NetInformations.cpp
                                     HNetlistModel.cpp
                                     HNetlist.cpp
                                     HDisplayFilter.cpp
                       )

          qt4_wrap_cpp ( MOC_SRCS    ${mocincludes} )
     qt4_add_resources ( RCC_SRCS    CellViewer.qrc )

           add_library ( hviewer     SHARED ${cpps} ${MOC_SRCS} ${RCC_SRCS} )
 target_link_libraries ( hviewer     ${QT_LIBRARIES} hurricane )

               install ( FILES       ${exports} DESTINATION /include/hurricane/viewer ) 
               install ( TARGETS     hviewer    DESTINATION /lib )
