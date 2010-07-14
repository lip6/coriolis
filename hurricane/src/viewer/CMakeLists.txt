
               include ( ${QT_USE_FILE} )

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/isobar
                         ${HURRICANE_SOURCE_DIR}/src/viewer
                         ${CONFIGURATION_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIRS}
                         ${PYTHON_INCLUDE_PATH}
                       )

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
                                     hurricane/viewer/ScriptWidget.h
                       )
                   set ( includes    hurricane/viewer/ScreenUtilities.h
                                     hurricane/viewer/DisplayStyle.h
                                     hurricane/viewer/ColorScale.h
                                     hurricane/viewer/Graphics.h
                                     hurricane/viewer/Selector.h
                                     hurricane/viewer/Ruler.h
                                     hurricane/viewer/Command.h
                                     hurricane/viewer/AreaCommand.h
                                     hurricane/viewer/MoveCommand.h
                                     hurricane/viewer/ZoomCommand.h
                                     hurricane/viewer/RulerCommand.h
                                     hurricane/viewer/SelectCommand.h
                                     hurricane/viewer/HierarchyCommand.h
                                     hurricane/viewer/SelectorCriterion.h
                                     hurricane/viewer/CellWidgets.h
                       )
                   set ( pyincludes  hurricane/viewer/PyCellViewer.h
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
                                     ScriptWidget.cpp
                       )
                   set ( pycpps      PyViewer.cpp
                                     PyCellViewer.cpp
                       )

          qt4_wrap_cpp ( MOC_SRCS    ${mocincludes} )
     qt4_add_resources ( RCC_SRCS    CellViewer.qrc )

           add_library ( viewer      ${cpps} ${MOC_SRCS} ${RCC_SRCS} ${pycpps} )
 target_link_libraries ( viewer      hurricane ${QT_LIBRARIES} ${Boost_LIBRARIES} )
           add_library ( Viewer      MODULE ${pycpps} )
 target_link_libraries ( Viewer      viewer )
 set_target_properties ( Viewer      PROPERTIES
                                     COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                     PREFIX        ""
                       )


               install ( FILES       ${includes}
                                     ${mocincludes}
                                     ${pyincludes}  DESTINATION include/coriolis2/hurricane/viewer ) 
               install ( TARGETS     viewer         DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS     Viewer         DESTINATION ${PYTHON_SITE_PACKAGES} )
