
               include ( ${QT_USE_FILE} )

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/hviewer )

                   set ( mocincludes hurricane/viewer/PaletteItem.h
                                     hurricane/viewer/PaletteNamedItem.h
                                     hurricane/viewer/PaletteLayerItem.h
                                     hurricane/viewer/PaletteWidget.h
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
                                     hurricane/viewer/HSelectionPopupModel.h
                                     hurricane/viewer/HSelectionPopup.h
                                     hurricane/viewer/HSelectionModel.h
                                     hurricane/viewer/HSelection.h
                                     hurricane/viewer/HNetlistModel.h
                                     hurricane/viewer/HNetlist.h
                                     hurricane/viewer/HDisplayFilter.h
                                     hurricane/viewer/ControllerWidget.h
                       )
                   set ( exports     hurricane/viewer/ScreenUtilities.h
                                     hurricane/viewer/DisplayStyle.h
                                     hurricane/viewer/ColorScale.h
                                     hurricane/viewer/Graphics.h
                                     hurricane/viewer/HGraphics.h
                                     hurricane/viewer/Selector.h
                                     hurricane/viewer/Command.h
                                     hurricane/viewer/AreaCommand.h
                                     hurricane/viewer/MoveCommand.h
                                     hurricane/viewer/ZoomCommand.h
                                     hurricane/viewer/SelectCommand.h
                                     hurricane/viewer/SelectorCommand.h
                                     hurricane/viewer/CellWidget.h
                                     hurricane/viewer/CellWidgets.h
                                     hurricane/viewer/CellViewer.h
                                     hurricane/viewer/HInspectorWidget.h
                                     hurricane/viewer/HSelectionPopupModel.h
                                     hurricane/viewer/HSelectionPopup.h
                                     hurricane/viewer/HSelectionModel.h
                                     hurricane/viewer/HSelection.h
                                     hurricane/viewer/NetInformations.h
                                     hurricane/viewer/HNetlist.h
                                     hurricane/viewer/HDisplayFilter.h
                                     hurricane/viewer/ControllerWidget.h
                       )
                   set ( cpps        ScreenUtilities.cpp 
                                     DisplayStyle.cpp 
                                     ColorScale.cpp
                                     Graphics.cpp
                                     HGraphics.cpp
                                     PaletteItem.cpp
                                     PaletteNamedItem.cpp
                                     PaletteLayerItem.cpp
                                     PaletteWidget.cpp
                                     DynamicLabel.cpp
                                     HMousePosition.cpp
                                     Selector.cpp
                                     Command.cpp
                                     AreaCommand.cpp
                                     MoveCommand.cpp
                                     ZoomCommand.cpp
                                     SelectCommand.cpp
                                     SelectorCommand.cpp
                                     CellWidget.cpp
                                     CellViewer.cpp
                                     RecordModel.cpp
                                     HInspectorWidget.cpp
                                     HSelectionPopupModel.cpp
                                     HSelectionPopup.cpp
                                     HSelectionModel.cpp
                                     HSelection.cpp
                                     NetInformations.cpp
                                     HNetlistModel.cpp
                                     HNetlist.cpp
                                     HDisplayFilter.cpp
                                     ControllerWidget.cpp
                       )

          qt4_wrap_cpp ( MOC_SRCS    ${mocincludes} )
     qt4_add_resources ( RCC_SRCS    CellViewer.qrc )

               install ( FILES           ${exports}     DESTINATION /include/hurricane/viewer ) 

 if ( BUILD_STATIC )
           add_library ( hviewer-static  STATIC ${cpps} ${MOC_SRCS} ${RCC_SRCS} )
 target_link_libraries ( hviewer-static  hurricane-static ${QT_LIBRARIES} )
               install ( TARGETS         hviewer-static DESTINATION /lib )
 else ( BUILD_STATIC )
           add_library ( hviewer         SHARED ${cpps} ${MOC_SRCS} ${RCC_SRCS} )
 target_link_libraries ( hviewer         ${QT_LIBRARIES} hurricane )
               install ( TARGETS         hviewer        DESTINATION /lib )
 endif ( BUILD_STATIC )
