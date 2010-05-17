
               include ( ${QT_USE_FILE} )

   include_directories ( ${Boost_INCLUDE_DIRS}
                         ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/viewer )

                   set ( mocincludes hurricane/viewer/HApplication.h
                                     hurricane/viewer/PaletteItem.h
                                     hurricane/viewer/PaletteNamedItem.h
                                     hurricane/viewer/PaletteLayerItem.h
                                     hurricane/viewer/PaletteExtensionGoItem.h
                                     hurricane/viewer/PaletteWidget.h
                                     hurricane/viewer/GraphicsWidget.h
                                     hurricane/viewer/ExceptionWidget.h
                                     hurricane/viewer/BreakpointWidget.h
                                     hurricane/viewer/GotoWidget.h
                                     hurricane/viewer/DynamicLabel.h
                                     hurricane/viewer/MousePositionWidget.h
                                     hurricane/viewer/SelectCommand.h
                                     hurricane/viewer/CellWidget.h
                                     hurricane/viewer/CellViewer.h
                                     hurricane/viewer/RecordModel.h
                                     hurricane/viewer/InspectorWidget.h
                                     hurricane/viewer/SelectionPopupModel.h
                                     hurricane/viewer/SelectionPopup.h
                                     hurricane/viewer/SelectionModel.h
                                     hurricane/viewer/SelectionWidget.h
                                     hurricane/viewer/NetlistModel.h
                                     hurricane/viewer/NetlistWidget.h
                                     hurricane/viewer/DisplayFilterWidget.h
                                     hurricane/viewer/ControllerWidget.h
                       )
                   set ( exports     hurricane/viewer/HApplication.h
                                     hurricane/viewer/ScreenUtilities.h
                                     hurricane/viewer/DisplayStyle.h
                                     hurricane/viewer/ColorScale.h
                                     hurricane/viewer/Graphics.h
                                     hurricane/viewer/GraphicsWidget.h
                                     hurricane/viewer/ExceptionWidget.h
                                     hurricane/viewer/BreakpointWidget.h
                                     hurricane/viewer/MousePositionWidget.h
                                     hurricane/viewer/GotoWidget.h
                                     hurricane/viewer/Selector.h
                                     hurricane/viewer/Command.h
                                     hurricane/viewer/AreaCommand.h
                                     hurricane/viewer/MoveCommand.h
                                     hurricane/viewer/ZoomCommand.h
                                     hurricane/viewer/RulerCommand.h
                                     hurricane/viewer/SelectCommand.h
                                     hurricane/viewer/HierarchyCommand.h
                                     hurricane/viewer/SelectorCriterion.h
                                     hurricane/viewer/CellWidget.h
                                     hurricane/viewer/CellWidgets.h
                                     hurricane/viewer/CellViewer.h
                                     hurricane/viewer/InspectorWidget.h
                                     hurricane/viewer/SelectionPopupModel.h
                                     hurricane/viewer/SelectionPopup.h
                                     hurricane/viewer/SelectionModel.h
                                     hurricane/viewer/SelectionWidget.h
                                     hurricane/viewer/Ruler.h
                                     hurricane/viewer/NetInformations.h
                                     hurricane/viewer/NetlistWidget.h
                                     hurricane/viewer/DisplayFilterWidget.h
                                     hurricane/viewer/ControllerWidget.h
                                     hurricane/viewer/PaletteWidget.h
                       )
                   set ( cpps        HApplication.cpp
                                     ScreenUtilities.cpp 
                                     DisplayStyle.cpp 
                                     ColorScale.cpp
                                     Graphics.cpp
                                     GraphicsWidget.cpp
                                     ExceptionWidget.cpp
                                     BreakpointWidget.cpp
                                     GotoWidget.cpp
                                     PaletteItem.cpp
                                     PaletteNamedItem.cpp
                                     PaletteLayerItem.cpp
                                     PaletteExtensionGoItem.cpp
                                     PaletteWidget.cpp
                                     DynamicLabel.cpp
                                     MousePositionWidget.cpp
                                     Selector.cpp
                                     Command.cpp
                                     AreaCommand.cpp
                                     MoveCommand.cpp
                                     ZoomCommand.cpp
                                     RulerCommand.cpp
                                     SelectCommand.cpp
                                     HierarchyCommand.cpp
                                     SelectorCriterion.cpp
                                     CellWidget.cpp
                                     CellViewer.cpp
                                     RecordModel.cpp
                                     InspectorWidget.cpp
                                     SelectionPopupModel.cpp
                                     SelectionPopup.cpp
                                     SelectionModel.cpp
                                     SelectionWidget.cpp
                                     Ruler.cpp
                                     NetInformations.cpp
                                     NetlistModel.cpp
                                     NetlistWidget.cpp
                                     DisplayFilterWidget.cpp
                                     ControllerWidget.cpp
                       )

          qt4_wrap_cpp ( MOC_SRCS    ${mocincludes} )
     qt4_add_resources ( RCC_SRCS    CellViewer.qrc )

               install ( FILES           ${exports}     DESTINATION include/hurricane/viewer ) 

           add_library ( viewer          ${cpps} ${MOC_SRCS} ${RCC_SRCS} )
 target_link_libraries ( viewer          hurricane
                                         ${QT_LIBRARIES}
#                                        ${Boost_LIBRARIES}
                       )
               install ( TARGETS         viewer        DESTINATION lib${LIB_SUFFIX} )
